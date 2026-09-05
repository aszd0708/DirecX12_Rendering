#include "pch.h"
#include "DescriptorHeapAllocator.h"
#include "Stack.h"

////////////////////////
// DescriptorHeapPage //
////////////////////////

DescriptorHeapPage::DescriptorHeapPage(UINT8 pageIndex, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT capacity) : _pageIndex(pageIndex), _capacity(capacity)
{
	CreateDescriptorHeap(heapType, capacity);

	_bumpCount = 0;
	_freeStack = new Stack<UINT32>(capacity);
}

DescriptorHeapPage::~DescriptorHeapPage()
{
	delete _freeStack;
}

bool DescriptorHeapPage::Alloc(OUT DescriptorHandle& handle)
{
	if (CanAlloc() == false)
	{
		return false;
	}

	handle.pageIndex = _pageIndex;

	UINT32 index = 0;
	if (_freeStack->Pop(index) == false)
	{
		index = _bumpCount;
		_bumpCount++;
	}

	handle.index = index;

	handle.cpuDesc = D3D12_CPU_DESCRIPTOR_HANDLE(_cpuDescStart.ptr + index * _descriptorSize);
	handle.gpuDesc = D3D12_GPU_DESCRIPTOR_HANDLE(_gpuDescStart.ptr + index * _descriptorSize);

	return true;
}

void DescriptorHeapPage::Free(const UINT32& index)
{
	_freeStack->Push(index);
}

void DescriptorHeapPage::ResetPage()
{
	_freeStack->Clear();
	_bumpCount = 0;
}

void DescriptorHeapPage::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT capacity)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Type = heapType;
	desc.NumDescriptors = capacity;
	
	switch (heapType)
	{
		case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		break;
		case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		break;
	}

	desc.NodeMask= 0;

	ThrowIfFailed(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(_descHeap.GetAddressOf())));

	_descriptorSize = DEVICE->GetDescriptorHandleIncrementSize(heapType);
	_cpuDescStart = _descHeap->GetCPUDescriptorHandleForHeapStart();
	_gpuDescStart = _descHeap->GetGPUDescriptorHandleForHeapStart();
}

/////////////////////////////
// DescriptorHeapAllocator //
/////////////////////////////

UINT32 DescriptorHeapAllocator::GetDefaultCapacity(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return 2048;
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		return 128;
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return 16;
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return 32;
	}

	return 0;
}

// D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER 은 하나의 페이지로 고정
bool DescriptorHeapAllocator::CanIncreasePageCount(D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		return false;

	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
	case D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return true;
	}
	return false;
}

DescriptorHeapAllocator::DescriptorHeapAllocator(UINT8 id, D3D12_DESCRIPTOR_HEAP_TYPE heapType) : DescriptorHeapAllocator(id, heapType, DEFAULT_PAGE_COUNT, GetDefaultCapacity(heapType))
{
	
}

DescriptorHeapAllocator::DescriptorHeapAllocator(UINT8 id, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT8 pageCount, UINT32 capacity) : _allocatorID(id), _descType(heapType), _capacity(capacity)
{
	if (CanIncreasePageCount(heapType))
	{
		_usablePageIndex = new Stack<UINT8>(pageCount * 2);
	}
	else
	{
		pageCount = 1;
		_usablePageIndex = new Stack<UINT8>(pageCount);
	}
	SetPageCount(pageCount);
}

DescriptorHeapAllocator::~DescriptorHeapAllocator()
{
	for (int i = 0; i < _pageCount; ++i)
	{
		delete _pages[i];
	}
	delete _usablePageIndex;
}

DescriptorHandle DescriptorHeapAllocator::Alloc()
{
	DescriptorHandle handle;
	handle.allocatorID = _allocatorID;
	UINT8 pageIndex;
	bool isSuccess = _usablePageIndex->Peak(pageIndex);
	if (isSuccess == false)
	{
		// 특정 타입 힙 용량 초과
		assert(CanIncreasePageCount(_descType));

		SetPageCount(_pageCount + 1);
	}
	isSuccess = _usablePageIndex->Peak(pageIndex);
	isSuccess = _pages[pageIndex]->Alloc(handle);
	assert(isSuccess);

	if (_pages[pageIndex]->CanAlloc() == false)
	{
		UINT8 temp;
		_usablePageIndex->Pop(temp);
	}

	return handle;
}

void DescriptorHeapAllocator::Free(const DescriptorHandle& handle)
{
	UINT8 pageIndex = handle.pageIndex;
	DescriptorHeapPage* page = _pages[pageIndex];
	bool prevAllocCondition = page->CanAlloc();

	page->Free(handle.index);
	if (prevAllocCondition == false && page->CanAlloc())
	{
		_usablePageIndex->Push(pageIndex);
	}
}

void DescriptorHeapAllocator::ResetAllocator()
{
	for (int i = 0; i < _pageCount; ++i)
	{
		if (_pages[i] != nullptr)
		{
			_pages[i]->ResetPage();
		}
	}
}

DescriptorHeapPage* DescriptorHeapAllocator::GetHeapPage()
{
	if (_pageCount > 0)
	{
		return _pages[0];
	}
	return nullptr;
}

void DescriptorHeapAllocator::SetPageCount(UINT8 count)
{
	if(_pageCount >= count) return;

	DescriptorHeapPage** newPage = (DescriptorHeapPage**)malloc(sizeof(DescriptorHeapPage*) * count);
	if (_pages == nullptr)
	{
		_pages = newPage;

		for (int i = 0; i < count; ++i)
		{
			_pages[i] = new DescriptorHeapPage(i, _descType, _capacity);
			_usablePageIndex->Push(i);
		}
		_pageCount = count;
		return;
	}

	memmove(newPage, _pages, sizeof(DescriptorHeapPage*) * _pageCount);

	free(_pages);
	_pages = newPage;

	for (int i = _pageCount; i < count; ++i)
	{
		_pages[i] = new DescriptorHeapPage(i, _descType, _capacity);
		_usablePageIndex->Push(i);
	}
	_pageCount = count;
}
