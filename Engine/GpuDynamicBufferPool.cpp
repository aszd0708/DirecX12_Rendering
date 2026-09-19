#include "pch.h"
#include "GpuDynamicBufferPool.h"

GpuDynamicBufferPage::GpuDynamicBufferPage(UINT8 pageIndex, UINT64 size)
	: _pageIndex(pageIndex), _currentSize(size), _totalSize(size),
	_handlerCount(DEFAULT_BUFFER_HANDLER_ARRAY_SIZE), _handlers(DEFAULT_BUFFER_HANDLER_ARRAY_SIZE), _indexes(DEFAULT_BUFFER_HANDLER_ARRAY_SIZE),
	_startReleaseTable(), _endReleaseTable()
{
	CreateResource();

	for (int i = 0; i < _handlers.GetCount(); ++i)
	{
		_indexes.Push(i);
		_handlers[i].gen = 0;
		_handlers[i].arrayIndex = i;
		_handlers[i].pageIndex = _pageIndex;
	}

	_startReleaseTable.Add(0, _totalSize);
	_endReleaseTable.Add(_totalSize, 0);
}

GpuDynamicBufferPage::~GpuDynamicBufferPage()
{
	_resource.Reset();
}

void GpuDynamicBufferPage::CreateResource()
{
	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = _totalSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));
}

bool GpuDynamicBufferPage::Alloc(UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle & handle)
{
	assert(aligneSize != 0 && (aligneSize & (aligneSize - 1)) == 0);

	if (_currentSize < size)
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
			UINT64 alignedOffset = (offset + aligneSize - 1) & ~(aligneSize - 1);
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

void GpuDynamicBufferPage::Reset()
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

	_startReleaseTable.Add(0, _totalSize);
	_endReleaseTable.Add(_totalSize, 0);
}

bool GpuDynamicBufferPage::Free(const GpuBufferHandle& handle)
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

void GpuDynamicBufferPage::IncreaseHandlerSize(UINT32 size)
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

GpuDynamicBufferPool::GpuDynamicBufferPool(UINT8 poolID) : GpuDynamicBufferPool(poolID, DEFAULT_SIZE)
{

}

GpuDynamicBufferPool::GpuDynamicBufferPool(UINT8 poolID, UINT64 size) : _poolID(poolID), _totalSize(size), _pages(DEFAULT_RESOURCE_COUNT)
{
	for (int i = 0; i < DEFAULT_RESOURCE_COUNT; ++i)
	{
		GpuDynamicBufferPage* page = new GpuDynamicBufferPage(i, _totalSize);
		_pages[i] = page;
	}
}

GpuDynamicBufferPool::~GpuDynamicBufferPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
	}
}

void GpuDynamicBufferPool::IncreaseGpuResource()
{
	UINT32 index = _pages.GetCount();
	_pages.SetCount(_pages.GetCount() + 1);
	GpuDynamicBufferPage* page = new GpuDynamicBufferPage(index, _totalSize);
	_pages[index] = page;
}

ComPtr<ID3D12Resource>& GpuDynamicBufferPool::GetMemoryHeap(UINT8 index)
{
	assert(_pages.GetCount() > index);

	return _pages[index]->GetResource();
}

bool GpuDynamicBufferPool::GetBufferHandle(eGpuBufferKind type, UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle)
{
	bool allocSuccess = false;
	int count = _pages.GetCount();
	for (int i = 0; i < count; ++i)
	{
		GpuDynamicBufferPage* page = _pages[i];

		bool isSuccess = page->Alloc(size, aligneSize, handle);
		if (isSuccess == false)
		{
			if (i + 1 >= count)
			{
				IncreaseGpuResource();
				count++;
			}
			continue;
		}
		allocSuccess = true;

		handle.poolID = _poolID;
		break;
	}
	return allocSuccess;
}

bool GpuDynamicBufferPool::ReleaseBufferHandle(UINT8 index, const GpuBufferHandle& handle)
{
	bool isSuccess = _pages[index]->Free(handle);
	return isSuccess;
}

void GpuDynamicBufferPool::ResetPage(UINT8 pageIndex)
{
	assert(pageIndex < _pages.GetCount());
	_pages[pageIndex]->Reset();
}

void GpuDynamicBufferPool::ResetAllPage()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		ResetPage(i);
	}
}
