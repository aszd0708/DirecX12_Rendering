#include "pch.h"
#include "GpuConstantBufferPool.h"

/////////////////////////////
/// GpuConstantBufferPage ///
/////////////////////////////

GpuConstantBufferPage::GpuConstantBufferPage(UINT64 size) :_offset(0), _totalSize(size), _fenceValue(0)
{
}

GpuConstantBufferPage::~GpuConstantBufferPage()
{

}

bool GpuConstantBufferPage::Alloc(UINT64 size, OUT GpuConstantBufferHandle & handle)
{
	UINT64 alignedOffset = (_offset + CONSTANT_BUFFER_MEMORY_ALIGNMENT - 1) & ~(CONSTANT_BUFFER_MEMORY_ALIGNMENT - 1);

	if (alignedOffset + size > _totalSize)
	{
		return false;
	}

	handle.offset = alignedOffset;
	handle.size = size;

	_offset = alignedOffset + size;

	return true;
}

void GpuConstantBufferPage::Reset()
{
	_offset = 0;
}

/////////////////////////////
/// GpuConstantBufferPool ///
/////////////////////////////

GpuConstantBufferPool::GpuConstantBufferPool() : GpuConstantBufferPool(DEFAULT_PAGE_SIZE)
{

}

GpuConstantBufferPool::GpuConstantBufferPool(UINT64 pageSize) : GpuConstantBufferPool(pageSize, SWAP_CHAIN_BUFFER_COUNT)
{
}

GpuConstantBufferPool::GpuConstantBufferPool(UINT64 pageSize, UINT8 pageCount) :
_currentPageIndex(0), _pageSize(pageSize), _pages(pageCount), _mappedBase(nullptr), _fenceEvent(nullptr)
{
	for (int i = 0; i < pageCount; ++i)
	{
		GpuConstantBufferPage page = GpuConstantBufferPage(_pageSize);
		_pages[i] = page;
	}
	CreateResource(pageCount);
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

GpuConstantBufferPool::~GpuConstantBufferPool()
{
	_resource.Reset();
	::CloseHandle(_fenceEvent);
}

void GpuConstantBufferPool::CreateResource(UINT8 pageCount)
{
	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = _pageSize * pageCount;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));

	D3D12_RANGE range = {};
	range.Begin = 0;
	range.End = 0;
	ThrowIfFailed(_resource->Map(0, &range, &_mappedBase));
}

bool GpuConstantBufferPool::GetBufferHandle(UINT64 size, OUT GpuConstantBufferHandle& handle)
{
	return _pages[_currentPageIndex].Alloc(size, handle);
}

void GpuConstantBufferPool::MoveNextPage(ID3D12Fence* fence, UINT64 fenceValue)
{
	_pages[_currentPageIndex].SetFenceValue(fenceValue);
	_currentPageIndex++;
	_currentPageIndex %= _pages.GetCount();

	bool isComplete = fence->GetCompletedValue() >= _pages[_currentPageIndex].GetFenceValue();
	if (isComplete == false)
	{
		fence->SetEventOnCompletion(_pages[_currentPageIndex].GetFenceValue(), _fenceEvent);
		WaitForSingleObject(_fenceEvent, INFINITE);
	}

	ResetPage(_currentPageIndex);
}

void GpuConstantBufferPool::ResetCurrentPage()
{
	ResetPage(_currentPageIndex);
}

void GpuConstantBufferPool::ResetPage(UINT8 pageIndex)
{
	assert(pageIndex < _pages.GetCount());
	_pages[pageIndex].Reset();
}

void GpuConstantBufferPool::ResetAllPage()
{
	for (int i = 0; i < _pages.GetCount(); ++i)
	{
		ResetPage(i);
	}
}
