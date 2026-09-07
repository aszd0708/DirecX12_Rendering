#pragma once
#include "GpuMemoryPoolInterface.h"

struct GpuBumpMemoryHandle
{
	UINT8 pageIndex;
	UINT64 size;
	UINT64 offset;
};

class GpuBumpMemoryPage
{
public:
	GpuBumpMemoryPage(UINT64 offset, UINT64 alignment, UINT64 size, UINT8 pageIndex);
	~GpuBumpMemoryPage();

public:
	bool Alloc(UINT64 size, OUT GpuBumpMemoryHandle& handle);
	void Reset();

private:

private:
	UINT64 _alignment;
	UINT8 _pageIndex;
	UINT64 _totalSize;

	UINT64 _defaultOffset = 0;
	UINT64 _offset = 0;
};

class GpuBumpMemoryPool
{
	enum eBumpMemoryPoolType : UINT8
	{
		SIZE_64KB = 0,
		SIZE_4MB = 1,
		MAX
	};

	// 256*1024*1024
	const UINT64 DEFAULT_SIZE = 268435456;

public:
	GpuBumpMemoryPool();
	GpuBumpMemoryPool(UINT64 size);
	~GpuBumpMemoryPool();
	
private:
	void CreateHeap();

public:
	const ComPtr<ID3D12Heap>& GetMemoryHeap();

	bool GetMemoryHandle(eBumpMemoryPoolType type, UINT64 size, OUT GpuBumpMemoryHandle& handle);

	/// <summary>
	/// 더이상 참조하는 오브젝트가 없을 때 호출.
	/// </summary>
	/// <param name="pageIndex"></param>
	void ResetPage(UINT8 pageIndex);

	/// <summary>
	/// 더이상 참조하는 오브젝트가 없을 때 호출.
	/// </summary>
	void ResetAllPage();

private:
	UINT64 _totalSize;
	UINT8 _totalPageCount;
	Array<GpuBumpMemoryPage*> _pages;

	ComPtr<ID3D12Heap> _memoryHeap;
};

