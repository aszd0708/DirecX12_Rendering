#include "pch.h"
#include "GpuBumpBufferPool.h"

///////////////////////
// GpuBumpBufferPage //
///////////////////////

GpuBumpBufferPage::GpuBumpBufferPage(UINT8 pageIndex, UINT64 size) : _pageIndex(pageIndex), _totalSize(size)
{
	_offset = 0;
	CreateResource();
}

GpuBumpBufferPage::~GpuBumpBufferPage()
{
	_resource.Reset();
}

void GpuBumpBufferPage::CreateResource()
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

bool GpuBumpBufferPage::Alloc(UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle)
{
	assert(aligneSize != 0 && (aligneSize & (aligneSize - 1)) == 0);

	UINT64 alignedOffset = (_offset + aligneSize - 1) & ~(aligneSize - 1);

	// 용량이 모자란경우
	if (alignedOffset + size > _totalSize)
	{
		return false;
	}

	handle.gen = 0;
	handle.offset = alignedOffset;
	handle.size = size;
	handle.pageIndex = _pageIndex;

	_offset = alignedOffset + size;

	return true;
}

void GpuBumpBufferPage::Reset()
{
	_offset = 0;
}

UINT64 GpuBumpBufferPage::GetFreeByte()
{
	return _totalSize - _offset;
}

///////////////////////
// GpuBufferBumpPool //
///////////////////////

GpuBumpBufferPool::GpuBumpBufferPool(UINT8 poolID) : GpuBumpBufferPool(poolID, DEFAULT_SIZE)
{
	
}

GpuBumpBufferPool::GpuBumpBufferPool(UINT8 poolID, UINT64 size) : _poolID(poolID), _totalSize(size), _pages(DEFAULT_RESOURCE_COUNT)
{
	for (int i = 0; i < DEFAULT_RESOURCE_COUNT; ++i)
	{
		GpuBumpBufferPage* page = new GpuBumpBufferPage(i, _totalSize);
		_pages[i] = page;
	}
}

GpuBumpBufferPool::~GpuBumpBufferPool()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		delete _pages[i];
	}
}

void GpuBumpBufferPool::IncreaseGpuResource()
{
	UINT32 index = _pages.GetCount();
	_pages.SetCount(_pages.GetCount() + 1);
	GpuBumpBufferPage* page = new GpuBumpBufferPage(index, _totalSize);
	_pages[index] = page;
}

ComPtr<ID3D12Resource>& GpuBumpBufferPool::GetMemoryHeap(UINT8 index)
{
	assert(_pages.GetCount() > index);

	return _pages[index]->GetResource();
}

bool GpuBumpBufferPool::GetBufferHandle(eGpuBufferKind type, UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle)
{
	bool allocSuccess = false;
	int count = _pages.GetCount();
	for (int i = 0; i < count; ++i)
	{
		GpuBumpBufferPage* page = _pages[i];

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

void GpuBumpBufferPool::ResetAllPage()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		_pages[i]->Reset();
	}
}
