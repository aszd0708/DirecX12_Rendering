#pragma once
#include "GpuMemoryPoolInterface.h"

class GpuBumpMemoryPage : IGpuMemoryPoolInterface
{
public:
	GpuBumpMemoryPage();
	GpuBumpMemoryPage(UINT64 size);

public:
	bool GetMemory(OUT GpuPoolMemoryHandle& handle);

private:
	void CreateHeap();

private:
	D3D12_HEAP_TYPE _type;
	UINT64 _size;
	ComPtr<ID3D12Heap> _memoryHeap;
};

class GpuBumpMemoryPool
{
public:
	GpuBumpMemoryPool();
	GpuBumpMemoryPool(UINT64 size);
	GpuBumpMemoryPool(UINT64 size, UINT8 pageCount);
	~GpuBumpMemoryPool();

public:

private:
	void CreateMemory();

private:
	D3D12_HEAP_TYPE _type;
	UINT64 _size;

	UINT8 _totalPageCount;

	GpuBumpMemoryPage* _pages;
};

