#include "pch.h"
#include "GpuMaterialBufferPool.h"
#include "GpuMemoryPoolManager.h"
#include "MaterialInfo.h"

GpuMaterialBufferPage::GpuMaterialBufferPage(eMaterialType materialType, UINT32 totalCount, UINT64 stride, UINT64 offset)
: _materialType(materialType), _totalCount(totalCount), _stride(stride), _offset(offset), _allocableCheckingArray(), _usableIndex(totalCount)
{
	for (int i = totalCount - 1; i >= 0; --i)
	{
		_allocableCheckingArray.Add(true);
		_usableIndex.Push(i);
	}
}

GpuMaterialBufferPage::~GpuMaterialBufferPage()
{}

bool GpuMaterialBufferPage::Alloc(OUT GpuMaterialBufferHandle& handle)
{
	if (_usableIndex.GetCount() <= 0)
	{
		// 사용할 수 없는 갯수 없음
		return false;
	}

	UINT32 index = UINT32_MAX;
	bool isSuccess = _usableIndex.Pop(index);
	if (isSuccess == false)
	{
		return false;
	}

	assert(_allocableCheckingArray[index]);

	handle.materialType = _materialType;
	handle.index = index;
	_allocableCheckingArray[index] = false;
	return true;
}

void GpuMaterialBufferPage::Free(const GpuMaterialBufferHandle& handle)
{
	// 범위 밖 반납
	assert(handle.index < _totalCount);
	// 두번 반납될 경우
	assert(_allocableCheckingArray[handle.index] == false);

	assert(_totalCount > _usableIndex.GetCount());

	_usableIndex.Push(handle.index);
	_allocableCheckingArray[handle.index] = true;
}

bool GpuMaterialBufferPage::IsValied(UINT32 index, UINT64 size)
{
	bool isValied = _stride == size && index < _totalCount && _allocableCheckingArray[index] == false;
	return isValied;
}

 UINT64 GpuMaterialBufferPage::GetOffset(UINT32 index) const
{
	UINT64 offset = _offset + (index * _stride);
	return offset;
}

GpuMaterialBufferPool::GpuMaterialBufferPool(UINT32 poolID, const Array<UINT32>& materialCountArray)
: _poolID(poolID), _pages((UINT32)eMaterialType::MAX)
{
	UINT64 resourceSize = 0;

	for (int i = 0; i < materialCountArray.GetCount(); ++i)
	{
		eMaterialType type = (eMaterialType)i;
		UINT32 totalCount = materialCountArray[i];
		if (totalCount <= 0)
		{
			_pages[i] = nullptr;
			continue;
		}

		UINT64 stride = MaterialInfo::GetDescSize(type);
		assert(stride != 0);

		GpuMaterialBufferPage* page = new GpuMaterialBufferPage(type, totalCount, stride, resourceSize);
		_pages[i] = page;
		resourceSize += totalCount * stride;

		// 256 단위로 올림
		resourceSize = (255 + resourceSize) & ~255;
	}
	_totalSize = resourceSize;
	
	CreateResource(resourceSize);
}

GpuMaterialBufferPool::~GpuMaterialBufferPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
	}

	_resource.Reset();
}

void GpuMaterialBufferPool::CreateResource(UINT64 size)
{
	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = size;
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
	assert(_pages[(UINT8)handle.materialType] != nullptr);
	assert(_pages[(UINT8)handle.materialType]->IsValied(handle.index, dataSize));

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

	GpuMaterialBufferPage* page = _pages[(UINT8)handle.materialType];

	UINT64 offset = page->GetOffset(handle.index);
	commandList->CopyBufferRegion(_resource.Get(), offset, uploadResource.Get(), 0, dataSize);

	D3D12_RESOURCE_BARRIER nextBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	_currentState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	commandList->ResourceBarrier(1, &nextBarrier);
}

bool GpuMaterialBufferPool::Alloc(const eMaterialType materialType, OUT GpuMaterialBufferHandle& handle)
{
	assert(_pages[(UINT8)materialType] != nullptr);

	handle.poolID = _poolID;
	bool isSuccess = _pages[(UINT8)materialType]->Alloc(handle);
	return isSuccess;
}

void GpuMaterialBufferPool::Free(const GpuMaterialBufferHandle& handle)
{
	assert(_pages[(UINT8)handle.materialType] != nullptr);
	// Pool ID가 다를 경우
	assert(handle.poolID == _poolID);

	_pages[(UINT8)handle.materialType]->Free(handle);
}

D3D12_GPU_VIRTUAL_ADDRESS GpuMaterialBufferPool::GetGpuAddress(eMaterialType type)
{
	assert(_pages[(UINT8)type] != nullptr);

	UINT64 offset = _pages[(UINT8)type]->GetOffset(0);
	return _resource->GetGPUVirtualAddress() + offset;
}
