#include "pch.h"
#include "GpuMaterialBufferPool.h"
#include "GpuMemoryPoolManager.h"

GpuMaterialBufferPool::GpuMaterialBufferPool(UINT32 poolID, UINT32 totalCount, UINT32 stride) 
: _poolID(poolID), _totalCount(totalCount), _stride(stride), _allocableCheckingArray(totalCount), _usableIndex(totalCount)
{
	for (int i = totalCount - 1; i >= 0; --i)
	{
		_allocableCheckingArray[i] = true;
		_usableIndex.Push(i);
	}

	CreateResource();
}

GpuMaterialBufferPool::~GpuMaterialBufferPool()
{
	_resource.Reset();
}

void GpuMaterialBufferPool::CreateResource()
{
	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = (UINT64)_totalCount * (UINT64)_stride;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));

	_currentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
}

void GpuMaterialBufferPool::GetUploadHeap(const D3D12_RESOURCE_DESC& desc, UINT64 size, OUT ComPtr<ID3D12Resource>& uploadResource, OUT GpuMemoryHandle& uploadHandle)
{
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT rootPrintLayout = {};
	uint32 numRows;
	uint64 rowSizeInBytes;
	uint64 totalByte;
	DEVICE->GetCopyableFootprints(&desc, 0, 1, 0, &rootPrintLayout, &numRows, &rowSizeInBytes, &totalByte);

	// Create Upload Buffer
	D3D12_RESOURCE_DESC uploadHeapBuffer = CD3DX12_RESOURCE_DESC::Buffer(totalByte);

	ComPtr<ID3D12Heap> uploadHeap = GPU_MEM_POOL->GetMemoryHeap(GpuMemoryPoolManager::eMemoryPoolID::DYNAMIC_UPLOAD);
	bool isSuccess = GPU_MEM_POOL->GetMemory(GpuMemoryPoolManager::eMemoryPoolID::DYNAMIC_UPLOAD, eGpuMemoryPoolType::SIZE_64KB, size, uploadHandle);
	assert(isSuccess);
	ThrowIfFailed(DEVICE->CreatePlacedResource(uploadHeap.Get(), uploadHandle.offset, &uploadHeapBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadResource.GetAddressOf())));
}

void GpuMaterialBufferPool::Upload(const ComPtr<ID3D12GraphicsCommandList>& commandList, const GpuMaterialBufferHandle& handle, const void* uploadData, UINT64 dataSize, OUT GpuMemoryHandle& memoryHandle)
{
	assert(handle.poolID == _poolID);
	assert(handle.index < _totalCount);
	assert(_allocableCheckingArray[handle.index] == false);
	assert(dataSize == _stride);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = dataSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> uploadResource;
	GetUploadHeap(desc, dataSize, uploadResource, memoryHandle);

	void* data;
	uploadResource->Map(0, nullptr, &data);
	memcpy(data, uploadData, dataSize);
	uploadResource->Unmap(0, nullptr);

	if (_currentState != D3D12_RESOURCE_STATE_COPY_DEST)
	{
		D3D12_RESOURCE_BARRIER prevBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource.Get(),
			_currentState,
			D3D12_RESOURCE_STATE_COPY_DEST);
		commandList->ResourceBarrier(1, &prevBarrier);
	}

	commandList->CopyBufferRegion(_resource.Get(), handle.index * (UINT64)_stride, uploadResource.Get(), 0, dataSize);

	D3D12_RESOURCE_BARRIER nextBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	_currentState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	commandList->ResourceBarrier(1, &nextBarrier);
}

bool GpuMaterialBufferPool::Alloc(OUT GpuMaterialBufferHandle& handle)
{
	if (_usableIndex.GetCount() <= 0)
	{
		return false;
	}

	handle.poolID = _poolID;

	UINT32 index = UINT32_MAX;
	bool isSuccess = _usableIndex.Pop(index);
	if (isSuccess == false)
	{
		return false;
	}

	assert(_allocableCheckingArray[index]);

	handle.index = index;
	_allocableCheckingArray[index] = false;
	return true;
}

bool GpuMaterialBufferPool::Free(const GpuMaterialBufferHandle& handle)
{
	// Pool ID가 다를 경우
	assert(handle.poolID == _poolID);
	// 범위 밖 반납
	assert(handle.index < _totalCount);
	// 두번 반납될 경우
	assert(_allocableCheckingArray[handle.index] == false);
	
	assert(_totalCount > _usableIndex.GetCount());

	_usableIndex.Push(handle.index);
	_allocableCheckingArray[handle.index] = true;
	return true;
}
