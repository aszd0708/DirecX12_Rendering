#pragma once
#include "GpuMemoryInfo.h"

class GpuBumpMemoryPage
{
public:
	GpuBumpMemoryPage(UINT64 offset, UINT64 alignment, UINT64 size, UINT8 pageIndex);
	~GpuBumpMemoryPage();

public:
	bool Alloc(UINT64 size, OUT GpuMemoryHandle& handle);
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
	// 256*1024*1024
	static const UINT64 DEFAULT_SIZE = 268435456;

public:
	GpuBumpMemoryPool(UINT8 poolID, bool using4MBSize);
	GpuBumpMemoryPool(UINT8 poolID, UINT64 size, bool using4MBSize);
	~GpuBumpMemoryPool();
	
private:
	Array<GpuBumpMemoryPage*> ComputePageCount(bool using4MBSize);
	void CreateHeap();

public:
	ComPtr<ID3D12Heap>& GetMemoryHeap();

	bool GetMemoryHandle(eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle);

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
	UINT8 _poolID;
	UINT64 _totalSize;
	UINT8 _totalPageCount;
	Array<GpuBumpMemoryPage*> _pages;

	ComPtr<ID3D12Heap> _memoryHeap;
};