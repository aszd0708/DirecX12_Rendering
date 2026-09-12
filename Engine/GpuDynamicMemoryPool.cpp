#include "pch.h"
#include "GpuDynamicMemoryPool.h"
#include "GpuMemoryPoolManager.h"

//////////////////////////
///GpuDynamicMemoryPage///
//////////////////////////

GpuDynamicMemoryPage::GpuDynamicMemoryPage(UINT64 offset, UINT64 alignment, UINT64 size, UINT8 pageIndex) 
: _offset(offset), _defaultOffset(offset), _alignment(alignment), _currentSize(size), _totalSize(size), _pageIndex(pageIndex),
_handlerCount(DEFAULT_MEMORY_HANDLER_ARRAY_SIZE), _handlers(DEFAULT_MEMORY_HANDLER_ARRAY_SIZE), _indexes(DEFAULT_MEMORY_HANDLER_ARRAY_SIZE),
_startReleaseTable(), _endReleaseTable()
{
	for (int i = 0; i < _handlers.GetCount(); ++i)
	{
		_indexes.Push(i);
		_handlers[i].gen = 0;
		_handlers[i].arrayIndex = i;
		_handlers[i].pageIndex = _pageIndex;
	}

	_startReleaseTable.Add(_defaultOffset, _totalSize);
	_endReleaseTable.Add(_defaultOffset + _totalSize, _defaultOffset);
}

GpuDynamicMemoryPage::~GpuDynamicMemoryPage()
{

}

bool GpuDynamicMemoryPage::Alloc(UINT64 size, OUT GpuMemoryHandle & handle)
{
	if(_currentSize < size) 
	{
		assert(false);
		return false;
	}

	if (_indexes.GetCount() <= 0)
	{
		IncreaseHandlerSize(_handlerCount * 2);
	}

	UINT32 index = 0;
	bool isSuccess = _indexes.Pop(index);
	if (isSuccess == false)
	{
		return false;
	}

	UINT32 tableCapacity = _startReleaseTable.GetCapacity();
	for (UINT32 i = 0; i < tableCapacity; ++i)
	{
		UINT64 offset;
		UINT64 emptySize;
		if (_startReleaseTable.IsOccupiedAt(i, offset, emptySize))
		{
			UINT64 alignedOffset = (offset + _alignment - 1) & ~(_alignment - 1);
			if ((alignedOffset + size) <= (offset + emptySize))
			{
				_handlers[index].arrayIndex = index;
				_handlers[index].offset = alignedOffset;
				_handlers[index].size = size;
				_handlers[index].pageIndex = _pageIndex;
				handle = _handlers[index];
				_currentSize -= size;

				// 앞 비어있는 공간 존재할 시
				if (alignedOffset > offset)
				{
					_startReleaseTable.Add(offset, alignedOffset - offset);
					_endReleaseTable.Add(alignedOffset, offset);
				}
				else
				{
					_startReleaseTable.RemoveKey(offset);
				}

				// 뒤 비어있는 공간 존재할 시
				if ((alignedOffset + size) < (offset + emptySize))
				{
					_startReleaseTable.Add(alignedOffset + size, (offset + emptySize) - (alignedOffset + size));
					_endReleaseTable.Add(offset + emptySize, alignedOffset + size);
				}
				else
				{
					_endReleaseTable.RemoveKey(offset + emptySize);
				}



				return true;
			}
		}
	}

	// 남는 사이즈는 있지만 자리가 없음
	return false;
}

bool GpuDynamicMemoryPage::Free(const GpuMemoryHandle& handle)
{
	if (_handlers.GetCount() <= handle.arrayIndex) { assert(false); return false; }
	if (_handlers[handle.arrayIndex].gen != handle.gen) { assert(false); return false; }

	UINT64 offset = handle.offset;
	UINT64 size = handle.size;

	UINT64 frontNeighborStart = _totalSize + 1;
	UINT64 backNeighborSize = _totalSize + 1;
	// 블록 앞에 빈 블록이 있음
	_endReleaseTable.GetValue(offset, frontNeighborStart);
	// 블록 뒤에 빈 블록이 있음
	_startReleaseTable.GetValue((offset + size), backNeighborSize);

	// 구간 시작 끝 둘다
	if (frontNeighborStart != _totalSize + 1 && backNeighborSize != _totalSize + 1)
	{
		_startReleaseTable.Add(frontNeighborStart, offset + size + backNeighborSize - frontNeighborStart);
		_endReleaseTable.Add((offset + size + backNeighborSize), frontNeighborStart);

		_startReleaseTable.RemoveKey(offset + size);
		_endReleaseTable.RemoveKey(offset);
	}

	else if (frontNeighborStart != _totalSize + 1)
	{
		_startReleaseTable.Add(frontNeighborStart, (offset + size) - frontNeighborStart);
		_endReleaseTable.Add(offset + size, frontNeighborStart);

		_endReleaseTable.RemoveKey(offset);
	}

	else if (backNeighborSize != _totalSize + 1)
	{
		_startReleaseTable.Add(offset, backNeighborSize + size);
		_endReleaseTable.Add(offset + size + backNeighborSize, offset);

		_startReleaseTable.RemoveKey(offset + size);
	}

	else if (frontNeighborStart == _totalSize + 1 && backNeighborSize == _totalSize + 1)
	{
		_startReleaseTable.Add(offset, size);
		_endReleaseTable.Add(offset + size, offset);
	}

	_handlers[handle.arrayIndex].gen++;
	_handlers[handle.arrayIndex].size = 0;
	_handlers[handle.arrayIndex].offset = 0;

	_indexes.Push(handle.arrayIndex);

	_currentSize += handle.size;

	return true;
}

void GpuDynamicMemoryPage::Reset()
{
	_currentSize = _totalSize;
	_indexes.Clear();
	for (int i = 0; i < _handlers.GetCount(); ++i)
	{
		_indexes.Push(i);
		_handlers[i].gen = 0;
		_handlers[i].arrayIndex = i;
		_handlers[i].pageIndex = _pageIndex;
	}

	_startReleaseTable.Add(_defaultOffset, _totalSize);
	_endReleaseTable.Add(_defaultOffset + _totalSize, _defaultOffset);
}

void GpuDynamicMemoryPage::IncreaseHandlerSize(UINT32 size)
{
	_handlers.SetCount(size);
	
	for (int i = _handlerCount; i < size; ++i)
	{
		_indexes.Push(i);
		_handlers[i].gen = 0;
		_handlers[i].arrayIndex = i;
		_handlers[i].pageIndex = _pageIndex;
	}

	_handlerCount = size;
}


//////////////////////////
///GpuDynamicMemoryPool///
//////////////////////////

GpuDynamicMemoryPool::GpuDynamicMemoryPool(UINT8 poolID, bool using4MBSize) : GpuDynamicMemoryPool(poolID, DEFAULT_SIZE, using4MBSize)
{

}

GpuDynamicMemoryPool::GpuDynamicMemoryPool(UINT8 poolID, UINT64 size, bool using4MBSize) : _poolID(poolID), _totalSize(size), _pages(ComputePageCount(using4MBSize))
{
	CreateHeap();

	if (_poolID == (int)(GpuMemoryPoolManager::ePoolID::DYNAMIC_UPLOAD))
	{
		using4MBSize = false;
	}

	if (using4MBSize)
	{
		UINT64 msaaSize = size / 8;
		UINT64 alignement = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		UINT64 msaaPageStartOffset = ((size - msaaSize) + alignement - 1) & ~(alignement - 1);

		_totalPageCount = (int)eGpuMemoryPoolType::MAX;
		_pages[(int)eGpuMemoryPoolType::SIZE_64KB] = new GpuDynamicMemoryPage(0, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, msaaPageStartOffset, (int)eGpuMemoryPoolType::SIZE_64KB);
		_pages[(int)eGpuMemoryPoolType::SIZE_4MB] = new GpuDynamicMemoryPage(msaaPageStartOffset, D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT, msaaSize, (int)eGpuMemoryPoolType::SIZE_4MB);
	}
	else
	{
		_totalPageCount = (int)eGpuMemoryPoolType::SIZE_64KB + 1;
		_pages[(int)eGpuMemoryPoolType::SIZE_64KB] = new GpuDynamicMemoryPage(0, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, size, (int)eGpuMemoryPoolType::SIZE_64KB);
	}
}

GpuDynamicMemoryPool::~GpuDynamicMemoryPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
	}
}

Array<GpuDynamicMemoryPage*> GpuDynamicMemoryPool::ComputePageCount(bool using4MBSize)
{
	if (using4MBSize)
	{
		return Array<GpuDynamicMemoryPage*>((int)eGpuMemoryPoolType::MAX);
	}
	else
	{
		return Array<GpuDynamicMemoryPage*>((int)eGpuMemoryPoolType::SIZE_64KB + 1);
	}
}

void GpuDynamicMemoryPool::CreateHeap()
{
	D3D12_HEAP_DESC desc;
	desc.SizeInBytes = _totalSize;

	// Property
	D3D12_HEAP_PROPERTIES properties;

	if (_poolID == (int)(GpuMemoryPoolManager::ePoolID::DYNAMIC_UPLOAD))
	{
		properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		properties.CreationNodeMask = 1;
		properties.VisibleNodeMask = 1;
		desc.Properties = properties;
		desc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
	}
	else
	{
		properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		properties.CreationNodeMask = 1;
		properties.VisibleNodeMask = 1;
		desc.Properties = properties;
		desc.Flags = D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;
	}

	desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

	ThrowIfFailed(DEVICE->CreateHeap(&desc, IID_PPV_ARGS(_memoryHeap.GetAddressOf())));
}

const ComPtr<ID3D12Heap>& GpuDynamicMemoryPool::GetMemoryHeap()
{
	return _memoryHeap;
}

bool GpuDynamicMemoryPool::GetMemoryHandle(eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle)
{
	assert((int)type < _pages.GetCount());

	bool isSuccess = _pages[(int)type]->Alloc(size, handle);
	handle.poolType = type;
	handle.memoryPoolID = _poolID;
	return isSuccess;
}

bool GpuDynamicMemoryPool::ReleaseMemoryHandle(eGpuMemoryPoolType type, const GpuMemoryHandle& handle)
{
	bool isSuccess = _pages[(int)type]->Free(handle);
	return isSuccess;
}

void GpuDynamicMemoryPool::ResetPage(UINT8 pageIndex)
{
	assert(pageIndex < _pages.GetCount());
	_pages[pageIndex]->Reset();
}

void GpuDynamicMemoryPool::ResetAllPage()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		ResetPage(i);
	}
}
