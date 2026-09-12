#include "pch.h"
#include "Texture.h"
#include "Utils.h"
#include "GpuMemoryPoolManager.h"

Texture::Texture(wstring filePath) : _filePath(filePath)
{
	_memoryHandle = {};
	CreateTexture();
}

Texture::~Texture()
{
	DESC_POOL->FreeAllocDescHandle(_descHandle);

	switch (_memoryHandle.memoryPoolID)
	{
		case (UINT8)GpuMemoryPoolManager::ePoolID::DYNAMIC:
		case (UINT8)GpuMemoryPoolManager::ePoolID::DYNAMIC_ONLY_64:
			D3D12_RESOURCE_DESC desc = _resource->GetDesc();
			D3D12_RESOURCE_ALLOCATION_INFO info = DEVICE->GetResourceAllocationInfo(0, 1, &desc);
			eGpuMemoryPoolType poolType = GetMemoryPoolType(info.Alignment);
			GPU_MEM_POOL->ReleaseMemory(_memoryHandle);

		break;
	}
	_resource.Reset();
}

void Texture::CreateTexture()
{
	CreateResource();
	CreateView();
}

void Texture::CreateResource()
{
	TextureFormat textureFormat = {};

	// Read Texture File
	string path = Utils::ToString(_filePath);
	BYTE* textureInfo = stbi_load(path.c_str(), &textureFormat.x, &textureFormat.y, &textureFormat.channelsInFile, 4);

	// Create Default Buffer
	D3D12_RESOURCE_DESC defaultHeapDesc = {};
	defaultHeapDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	defaultHeapDesc.Width = textureFormat.x;
	defaultHeapDesc.Height = textureFormat.y;
	defaultHeapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	defaultHeapDesc.DepthOrArraySize = 1;
	defaultHeapDesc.MipLevels = 1;
	defaultHeapDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	defaultHeapDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	defaultHeapDesc.SampleDesc.Count = 1;
	
	ComPtr<ID3D12Heap> heap = GPU_MEM_POOL->GetMemoryHeap(GpuMemoryPoolManager::ePoolID::BUMP_ONLY_64KB);
	D3D12_RESOURCE_ALLOCATION_INFO info = DEVICE->GetResourceAllocationInfo(0, 1, &defaultHeapDesc);
	eGpuMemoryPoolType poolType = GetMemoryPoolType(info.Alignment);
	bool isSuccess = GPU_MEM_POOL->GetMemory(GpuMemoryPoolManager::ePoolID::BUMP_ONLY_64KB, poolType, info.SizeInBytes, _memoryHandle);
	assert(isSuccess);
	ThrowIfFailed(DEVICE->CreatePlacedResource(heap.Get(), _memoryHandle.offset, &defaultHeapDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));

	// Create Upload Buffer
	ComPtr<ID3D12Resource> uploadResource;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT rootPrintLayout = {};
	uint32 numRows;
	uint64 rowSizeInBytes;
	uint64 totalByte;
	DEVICE->GetCopyableFootprints(&defaultHeapDesc, 0, 1, 0, &rootPrintLayout, &numRows, &rowSizeInBytes, &totalByte);

	D3D12_RESOURCE_DESC uploadHeapBuffer = CD3DX12_RESOURCE_DESC::Buffer(totalByte);

	ComPtr<ID3D12Heap> uploadHeap = GPU_MEM_POOL->GetMemoryHeap(GpuMemoryPoolManager::ePoolID::DYNAMIC_UPLOAD);
	D3D12_RESOURCE_ALLOCATION_INFO uploadInfo = DEVICE->GetResourceAllocationInfo(0, 1, &uploadHeapBuffer);
	eGpuMemoryPoolType uploadPoolType = GetMemoryPoolType(uploadInfo.Alignment);
	GpuMemoryHandle uploadGpuHandle = {};
	isSuccess = GPU_MEM_POOL->GetMemory(GpuMemoryPoolManager::ePoolID::DYNAMIC_UPLOAD, uploadPoolType, uploadInfo.SizeInBytes, uploadGpuHandle);
	assert(isSuccess);

	ThrowIfFailed(DEVICE->CreatePlacedResource(uploadHeap.Get(), uploadGpuHandle.offset, &uploadHeapBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadResource.GetAddressOf())));

	void* data;
	uploadResource->Map(0, nullptr, &data);
	for (int row = 0; row < numRows; ++row)
	{
		BYTE* dist = (BYTE*)data + rootPrintLayout.Footprint.RowPitch * row;
		BYTE* src = textureInfo + rowSizeInBytes * row;
		memcpy(dist, src, rowSizeInBytes);
	}
	uploadResource->Unmap(0, nullptr);

	// Upload Buffer To Default Buffer
	D3D12_TEXTURE_COPY_LOCATION copyDist = CD3DX12_TEXTURE_COPY_LOCATION(_resource.Get(), 0);
	D3D12_TEXTURE_COPY_LOCATION copySrc = CD3DX12_TEXTURE_COPY_LOCATION(uploadResource.Get(), rootPrintLayout);

	COMMAND_SUB_ALLOCATOR->Reset();
	COMMAND_SUB_LIST->Reset(COMMAND_SUB_ALLOCATOR.Get(), nullptr);

	COMMAND_SUB_LIST->CopyTextureRegion(&copyDist, 0, 0, 0, &copySrc, nullptr);

	// Barrier
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource.Get(), 
	D3D12_RESOURCE_STATE_COPY_DEST, 
	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	COMMAND_SUB_LIST->ResourceBarrier(1, &barrier);
	COMMAND_SUB_LIST->Close();

	// Wait GPU
	ID3D12CommandList* lists[] = { GRAPHICS->GetSubList().Get() };
	COMMAND_QUEUE->ExecuteCommandLists(1, lists);
	

	// TODO : 나중에 Fence도 풀로 만들어 관리
	GRAPHICS->WaitForGPU();

	//호출 위치를 "Fence 완료 확인 후"
	GPU_MEM_POOL->ReleaseMemory(uploadGpuHandle);

	stbi_image_free(textureInfo);
}

void Texture::CreateView()
{
	_descHandle = DESC_POOL->AllocDescHandle(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resourceDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(_resource.Get(), &resourceDesc, _descHandle.cpuDesc);
}
