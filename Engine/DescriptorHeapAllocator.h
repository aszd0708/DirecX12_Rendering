#pragma once
#include "Stack.h"

struct DescriptorHandle
{
	UINT8 allocatorID;
	UINT8 pageIndex;
	UINT32 index;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDesc;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDesc;
};

class DescriptorHeapPage
{
public:
	DescriptorHeapPage(UINT8 pageIndex, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT32 capacity);
	~DescriptorHeapPage();

public:
	bool Alloc(OUT DescriptorHandle& handle);
	void Free(const UINT32& index);

	void ResetPage();

	bool CanAlloc() { return _freeStack->GetCount() > 0 || _bumpCount < _capacity; }

	ID3D12DescriptorHeap* GetDescHeap() { return _descHeap.Get(); }

private:
	void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT32 capacity);

private:
	UINT8 _pageIndex;
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	UINT32 _descriptorSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE _cpuDescStart;
	D3D12_GPU_DESCRIPTOR_HANDLE _gpuDescStart;

	UINT32 _capacity;
	UINT32 _bumpCount;
	Stack<UINT32>* _freeStack;
};


class DescriptorHeapAllocator
{
private:
	const UINT8 DEFAULT_PAGE_COUNT = 1;

	static UINT32 GetDefaultCapacity(D3D12_DESCRIPTOR_HEAP_TYPE type);
	static bool CanIncreasePageCount(D3D12_DESCRIPTOR_HEAP_TYPE type);

public:
	DescriptorHeapAllocator(UINT8 id, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
	DescriptorHeapAllocator(UINT8 id, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT8 pageCount, UINT32 capacity);
	~DescriptorHeapAllocator();

public:
	DescriptorHandle Alloc();
	void Free(const DescriptorHandle& handle);

	void ResetAllocator();

	DescriptorHeapPage* GetHeapPage();

private:
	void SetPageCount(UINT8 count);

private:
	UINT8 _allocatorID;
	UINT8 _pageCount = 0;
	UINT32 _capacity = 0;
	Stack<UINT8>* _usablePageIndex;
	
	D3D12_DESCRIPTOR_HEAP_TYPE _descType;
	DescriptorHeapPage** _pages = nullptr;
};