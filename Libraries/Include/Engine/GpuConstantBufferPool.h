#pragma once

#define CONSTANT_BUFFER_MEMORY_ALIGNMENT 256

struct GpuConstantBufferHandle
{
	UINT64 offset;
	UINT64 size;
};

class GpuConstantBufferPage
{
public:
	GpuConstantBufferPage(UINT8 pageIndex, UINT64 size);
	~GpuConstantBufferPage();

public:
	bool Alloc(UINT64 size, OUT GpuConstantBufferHandle& handle);
	void Reset(); 

	void SetFenceValue(UINT64 fenceValue) { _fenceValue = fenceValue; }
	UINT64 GetFenceValue() { return _fenceValue; }
	
private:
	UINT8 _pageIndex;
	UINT64 _offset;
	UINT64 _totalSize;
	UINT64 _fenceValue;
};

class GpuConstantBufferPool
{
private:
	static const UINT64 DEFAULT_PAGE_SIZE = 262144l;

public:
	GpuConstantBufferPool(UINT8 poolID);
	GpuConstantBufferPool(UINT8 poolID, UINT64 pageSize);
	GpuConstantBufferPool(UINT8 poolID, UINT64 pageSize, UINT8 pageCount);
	~GpuConstantBufferPool();

private:
	void CreateResource(UINT8 pageCount);

public:
	ComPtr<ID3D12Resource>& GetResource() { return _resource; }

	bool GetBufferHandle(UINT64 size, OUT GpuConstantBufferHandle& handle);
	void* GetMapedBase() { return _mappedBase; }

	void MoveNextPage(ID3D12Fence* fence, UINT64 fenceValue);
	void ResetCurrentPage();
	
	void ResetPage(UINT8 pageIndex);

	/// <summary>
	/// 모든 페이지를 싹 지울 떄 호출
	/// </summary>
	/// <param name="pageIndex"></param>
	void ResetAllPage();

private:
	UINT8 _poolID;
	UINT8 _currentPageIndex;
	UINT64 _pageSize;
	Array<GpuConstantBufferPage> _pages;
	ComPtr<ID3D12Resource> _resource;
	void* _mappedBase;
	HANDLE _fenceEvent;
};

