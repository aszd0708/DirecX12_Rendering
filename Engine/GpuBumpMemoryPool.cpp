#include "pch.h"
#include "GpuBumpMemoryPool.h"
#include "GpuMemoryPoolManager.h"

///////////////////////
// GpuBumpMemoryPage //
///////////////////////

GpuBumpMemoryPage::GpuBumpMemoryPage(UINT64 offset, UINT64 alignment, UINT64 size, UINT8 pageIndex) : _offset(offset), _defaultOffset(offset), _alignment(alignment), _totalSize(size), _pageIndex(pageIndex)
{

}

GpuBumpMemoryPage::~GpuBumpMemoryPage()
{
}

bool GpuBumpMemoryPage::Alloc(UINT64 size, OUT GpuMemoryHandle& handle)
{
	assert(_alignment != 0 && (_alignment & (_alignment - 1)) == 0);
	
	UINT64 alignedOffset = (_offset + _alignment - 1) & ~(_alignment - 1);

	if (alignedOffset + size > _defaultOffset + _totalSize)
	{
		return false;
	}

	handle.gen = 0;
	handle.offset = alignedOffset;
	handle.pageIndex = _pageIndex;
	handle.size = size;

	_offset = alignedOffset + size;

	return true;
}

void GpuBumpMemoryPage::Reset()
{
	_offset = _defaultOffset;
}

///////////////////////
// GpuBumpMemoryPool //
///////////////////////

GpuBumpMemoryPool::GpuBumpMemoryPool(UINT8 poolID, bool using4MBSize) : GpuBumpMemoryPool(poolID, DEFAULT_SIZE, using4MBSize)
{
}

GpuBumpMemoryPool::GpuBumpMemoryPool(UINT8 poolID, UINT64 size, bool using4MBSize) : _poolID(poolID), _totalSize(size), _pages(ComputePageCount(using4MBSize))
{
	assert(poolID != (int)GpuMemoryPoolManager::ePoolID::DYNAMIC_UPLOAD);
	
	CreateHeap();
	
	if (using4MBSize)
	{
		UINT64 msaaSize = size / 8;
		UINT64 alignement = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		UINT64 msaaPageStartOffset = ((size - msaaSize) + alignement - 1) & ~(alignement - 1);

		_totalPageCount = (int)eGpuMemoryPoolType::MAX;
		_pages[(int)eGpuMemoryPoolType::SIZE_64KB] = new GpuBumpMemoryPage(0, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, msaaPageStartOffset, (int)eGpuMemoryPoolType::SIZE_64KB);
		_pages[(int)eGpuMemoryPoolType::SIZE_4MB] = new GpuBumpMemoryPage(msaaPageStartOffset, D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT, msaaSize, (int)eGpuMemoryPoolType::SIZE_4MB);
	}
	else
	{
		_totalPageCount = (int)eGpuMemoryPoolType::SIZE_64KB + 1;
		_pages[(int)eGpuMemoryPoolType::SIZE_64KB] = new GpuBumpMemoryPage(0, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, size, (int)eGpuMemoryPoolType::SIZE_64KB);
	}
}

GpuBumpMemoryPool::~GpuBumpMemoryPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
	}
}

Array<GpuBumpMemoryPage*> GpuBumpMemoryPool::ComputePageCount(bool using4MBSize)
{
	if (using4MBSize)
	{
		return Array<GpuBumpMemoryPage*>((int)eGpuMemoryPoolType::MAX);
	}
	else
	{
		return Array<GpuBumpMemoryPage*>((int)eGpuMemoryPoolType::SIZE_64KB + 1);
	}
}

void GpuBumpMemoryPool::CreateHeap()
{
	D3D12_HEAP_DESC desc;
	desc.SizeInBytes = _totalSize;

	// Property
	D3D12_HEAP_PROPERTIES properties;

	properties.Type = D3D12_HEAP_TYPE_DEFAULT;
	properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	properties.CreationNodeMask = 1;
	properties.VisibleNodeMask = 1;
	desc.Properties = properties;

	desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	desc.Flags = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

	ThrowIfFailed(DEVICE->CreateHeap(&desc, IID_PPV_ARGS(_memoryHeap.GetAddressOf())));
}

ComPtr<ID3D12Heap>& GpuBumpMemoryPool::GetMemoryHeap()
{
	return _memoryHeap;
}

bool GpuBumpMemoryPool::GetMemoryHandle(eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle)
{
	assert((int)type < _pages.GetCount());

	bool isSuccess = _pages[(int)type]->Alloc(size, handle);
	handle.poolType = type;
	handle.memoryPoolID = _poolID;
	return isSuccess;
}

void GpuBumpMemoryPool::ResetPage(UINT8 pageIndex)
{
	assert(pageIndex < _pages.GetCount());
	_pages[pageIndex]->Reset();
}

void GpuBumpMemoryPool::ResetAllPage()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		ResetPage(i);
	}
}
