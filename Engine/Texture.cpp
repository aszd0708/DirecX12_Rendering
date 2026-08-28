#include "pch.h"
#include "Texture.h"
#include "Utils.h"

Texture::Texture(wstring filePath) : _filePath(filePath)
{
	CreateTexture();
}

Texture::~Texture()
{

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

	D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	ThrowIfFailed(DEVICE->CreateCommittedResource(&defaultHeapProperties, D3D12_HEAP_FLAG_NONE, &defaultHeapDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));


	// Create Upload Buffer
	ComPtr<ID3D12Resource> uploadResource;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT rootPrintLayout = {};
	uint32 numRows;
	uint64 rowSizeInBytes;
	uint64 totalByte;
	DEVICE->GetCopyableFootprints(&defaultHeapDesc, 0, 1, 0, &rootPrintLayout, &numRows, &rowSizeInBytes, &totalByte);

	D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC uploadHeapBuffer = CD3DX12_RESOURCE_DESC::Buffer(totalByte);

	ThrowIfFailed(DEVICE->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &uploadHeapBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadResource.GetAddressOf())));

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

	COMMAND_ALLOCATOR->Reset();
	COMMAND_LIST->Reset(COMMAND_ALLOCATOR.Get(), nullptr);

	COMMAND_LIST->CopyTextureRegion(&copyDist, 0, 0, 0, &copySrc, nullptr);

	// Barrier
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource.Get(), 
	D3D12_RESOURCE_STATE_COPY_DEST, 
	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	COMMAND_LIST->ResourceBarrier(1, &barrier);
	COMMAND_LIST->Close();

	// Wait GPU
	ID3D12CommandList* lists[] = { GRAPHICS->GetList().Get() };
	COMMAND_QUEUE->ExecuteCommandLists(1, lists);

	// TODO : 나중에 Fence도 풀로 만들어 관리
	GRAPHICS->WaitForGPU();

	stbi_image_free(textureInfo);
}

void Texture::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	// 셰이더에서 직접 참조할 수 있게 세팅
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NumDescriptors = 1;
	ThrowIfFailed(DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_descHeap.GetAddressOf())));

	
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resourceDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resourceDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(_resource.Get(), &resourceDesc, _descHeap->GetCPUDescriptorHandleForHeapStart());
}
