#pragma once
#include "GpuBufferInfo.h"

class GpuDynamicBufferPage
{
	static const UINT32 DEFAULT_BUFFER_HANDLER_ARRAY_SIZE = 32;

public:
	GpuDynamicBufferPage(UINT8 pageIndex, UINT64 size);
	~GpuDynamicBufferPage();

private:
	void CreateResource();

public:
	/// <summary>
	/// 핸들 생성
	/// </summary>
	/// <param name="size"></param>
	/// <param name="aligneSize">하나의 사이즈 (Vertex는 12(4*3), Index는 2(UINT16사이즈), 4(UINT32사이즈)</param>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool Alloc(UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle);

	/// <summary>
	/// 강제 리셋 (MemoryHandle을 갖고있는게 살아있을 경우 에러가 발생할 수 있음)
	/// </summary>
	void Reset();

	bool Free(const GpuBufferHandle& handle);

private:
	void IncreaseHandlerSize(UINT32 size);

public:
	ComPtr<ID3D12Resource>& GetResource() { return _resource; }

private:
	UINT8 _pageIndex;
	UINT64 _totalSize;
	UINT64 _currentSize = 0;

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

	UINT32 _handlerCount = 0;
	Array<GpuBufferHandle> _handlers;
	Stack<UINT32> _indexes;

	ComPtr<ID3D12Resource> _resource;
};

class GpuDynamicBufferPool
{
	// 256*1024*1024
	static const UINT64 DEFAULT_SIZE = 268435456;
	const UINT8 DEFAULT_RESOURCE_COUNT = 1;

public:
	GpuDynamicBufferPool(UINT8 poolID);
	GpuDynamicBufferPool(UINT8 poolID, UINT64 size);
	~GpuDynamicBufferPool();

private:
	void IncreaseGpuResource();

public:
	ComPtr<ID3D12Resource>& GetMemoryHeap(UINT8 index);

	bool GetBufferHandle(eGpuBufferKind type, UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle);
	bool ReleaseBufferHandle(UINT8 index, const GpuBufferHandle& handle);

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
	UINT8 _totalPageCount;
	UINT64 _totalSize;
	Array<GpuDynamicBufferPage*> _pages;
};

