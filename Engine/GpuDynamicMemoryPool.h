#pragma once
#include "GpuMemoryPoolInterface.h"

class GpuDynamicMemoryPage
{
	static const UINT32 DEFAULT_MEMORY_HANDLER_ARRAY_SIZE;

public:
	GpuDynamicMemoryPage(UINT64 offset, UINT64 alignment, UINT64 size, UINT8 pageIndex);
	~GpuDynamicMemoryPage();

public:
	bool Alloc(UINT64 size, OUT GpuMemoryHandle& handle);
	bool Free(const GpuMemoryHandle& handle);

	/// <summary>
	/// 강제 리셋 (MemoryHandle을 갖고있는게 살아있을 경우 에러가 발생할 수 있음)
	/// </summary>
	void Reset();

private:
	void IncreaseHandlerSize(UINT32 size);

private:
	UINT64 _alignment;
	UINT8 _pageIndex;
	UINT64 _totalSize;
	UINT64 _currentSize;

	UINT64 _defaultOffset = 0;
	UINT64 _offset = 0;

	/// <summary>
	/// 구간의 크기
	/// Key : Offset, Value : Size
	/// </summary>
	HashTable<UINT64, UINT64> _startReleaseTable;

	/// <summary>
	/// 구간의 시작
	/// Key : Offset, Value : Offset
	/// </summary>
	HashTable<UINT64, UINT64> _endReleaseTable;


	UINT32 _handlerCount;
	Array<GpuMemoryHandle> _handlers;
	Stack<UINT32> _indexes;
};

class GpuDynamicMemoryPool
{
	// 256*1024*1024
	static const UINT64 DEFAULT_SIZE = 268435456;

public:
	GpuDynamicMemoryPool(UINT8 poolID);
	GpuDynamicMemoryPool(UINT8 poolID, UINT64 size);
	~GpuDynamicMemoryPool();

private:
	void CreateHeap();

public:
	const ComPtr<ID3D12Heap>& GetMemoryHeap();

	bool GetMemoryHandle(eMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle);
	bool ReleaseMemoryHandle(eMemoryPoolType type, const GpuMemoryHandle& handle);

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
	Array<GpuDynamicMemoryPage*> _pages;

	ComPtr<ID3D12Heap> _memoryHeap;
};

