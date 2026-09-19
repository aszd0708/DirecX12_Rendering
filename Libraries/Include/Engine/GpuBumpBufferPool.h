#pragma once
#include "GpuBufferInfo.h"

class GpuBumpBufferPage
{
public:
	GpuBumpBufferPage(UINT8 pageIndex, UINT64 size);
	~GpuBumpBufferPage();

private:
	void CreateResource();

public:
	/// <summary>
	/// 핸들 반환
	/// </summary>
	/// <param name="size"></param>
	/// <param name="aligneSize">하나의 사이즈 (Vertex는 12(4*3), Index는 2(UINT16사이즈), 4(UINT32사이즈)</param>
	/// <param name="handle"></param>
	/// <returns></returns>
	bool Alloc(UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle);
	void Reset();

	UINT64 GetFreeByte();

private:

public:
	ComPtr<ID3D12Resource>& GetResource() { return _resource; }

private:
	UINT8 _pageIndex;
	UINT64 _totalSize;
	UINT64 _offset = 0;

	ComPtr<ID3D12Resource> _resource;
};

class GpuBumpBufferPool
{
	// 256*1024*1024
	static const UINT64 DEFAULT_SIZE = 268435456;
	const UINT8 DEFAULT_RESOURCE_COUNT = 1;

public:
	GpuBumpBufferPool(UINT8 poolID);
	GpuBumpBufferPool(UINT8 poolID, UINT64 size);
	~GpuBumpBufferPool();

private:
	void IncreaseGpuResource();

public:
	ComPtr<ID3D12Resource>& GetMemoryHeap(UINT8 index);

	bool GetBufferHandle(eGpuBufferKind type, UINT64 size, UINT8 aligneSize, OUT GpuBufferHandle& handle);

	/// <summary>
	/// 더이상 참조하는 오브젝트가 없을 때 호출.
	/// </summary>
	/// <param name="pageIndex"></param>
	void ResetAllPage();

private:
	UINT8 _poolID;
	UINT64 _totalSize;
	Array<GpuBumpBufferPage*> _pages;
};

