#include "pch.h"
#include "GpuBumpMemoryPool.h"

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

GpuBumpMemoryPool::GpuBumpMemoryPool(UINT8 poolID) : GpuBumpMemoryPool(poolID, DEFAULT_SIZE)
{
}

GpuBumpMemoryPool::GpuBumpMemoryPool(UINT8 poolID, UINT64 size) : _poolID(poolID), _totalSize(size), _pages((int)eMemoryPoolType::MAX)
{
	CreateHeap();

	UINT64 msaaSize = size / 8;
	UINT64 alignement = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	UINT64 msaaPageStartOffset = ((size - msaaSize) + alignement - 1) & ~(alignement - 1);
	
	_totalPageCount = (int)eMemoryPoolType::MAX;
	_pages[(int)eMemoryPoolType::SIZE_64KB] = new GpuBumpMemoryPage(0, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, msaaPageStartOffset, (int)eMemoryPoolType::SIZE_64KB);
	_pages[(int)eMemoryPoolType::SIZE_4MB] = new GpuBumpMemoryPage(msaaPageStartOffset, D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT, msaaSize, (int)eMemoryPoolType::SIZE_4MB);
}

GpuBumpMemoryPool::~GpuBumpMemoryPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
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
	desc.Properties = properties;

	desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	desc.Flags = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

	ThrowIfFailed(DEVICE->CreateHeap(&desc, IID_PPV_ARGS(_memoryHeap.GetAddressOf())));
}

const ComPtr<ID3D12Heap>& GpuBumpMemoryPool::GetMemoryHeap()
{
	return _memoryHeap;
}

bool GpuBumpMemoryPool::GetMemoryHandle(eMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle)
{
	assert((int)type < _pages.GetCount());

	bool isSuccess = _pages[(int)type]->Alloc(size, handle);
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
