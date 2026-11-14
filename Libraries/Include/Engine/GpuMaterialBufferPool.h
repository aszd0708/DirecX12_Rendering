#pragma once
#include "MaterialInfoEnum.h"
#include "GpuMemoryInfo.h"

class GpuCommandInfo;

struct GpuMaterialBufferHandle
{
	UINT32 poolID;
	UINT32 index = UINT32_MAX;
};

struct GpuMaterialWaitingInfo
{
	UINT32 index = UINT32_MAX;
	UINT64 fenceValue = UINT64_MAX;
};

class GpuMaterialBufferPage
{
public:
	GpuMaterialBufferPage(UINT32 totalCount, UINT64 stride, UINT64 offset);
	~GpuMaterialBufferPage();

public:
	bool Alloc(OUT GpuMaterialBufferHandle& handle);
	void Free(const GpuMaterialBufferHandle& handle, const UINT64 fenceValue);

	void ProcessRetired(UINT64 fenceValue);

	void Reset();

	bool IsValied(UINT32 index, UINT64 size);

	UINT64 GetOffset(UINT32 index) const;

private:
	UINT32 _totalCount;
	UINT64 _stride;
	UINT64 _offset;

	DynamicArray<eGpuSlotCondition> _allocableCheckingArray;
	Stack<UINT32> _usableIndex;

	DynamicArray<GpuMaterialWaitingInfo> _waitingInfos;
	UINT32 _waitingInfoHead = 0;
};

class GpuMaterialBufferPool
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="poolID"></param>
	GpuMaterialBufferPool(UINT32 poolID, UINT32 materialCount);
	~GpuMaterialBufferPool();

private:
	void CreateResource(UINT64 size);
	void GetUploadHeap(const D3D12_RESOURCE_DESC& desc, UINT64 size, OUT ComPtr<ID3D12Resource>& uploadResource, OUT GpuMemoryHandle& uploadHandle);

public:
	void Upload(GpuCommandInfo* commandInfo, const GpuMaterialBufferHandle& handle, const void* data, UINT64 dataSize, OUT GpuMemoryHandle& memoryHandle);

public:
	bool Alloc(OUT GpuMaterialBufferHandle& handle);
	void Free(const GpuMaterialBufferHandle& handle, const UINT64 fenceValue);

	void ProcessRetired(const UINT64 fenceValue);

	void ResetAllPage();

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress();

private:
	UINT32 _poolID = UINT32_MAX;
	UINT64 _totalSize = 0;

	GpuMaterialBufferPage* _page;

	ComPtr<ID3D12Resource> _resource;
	D3D12_RESOURCE_STATES _currentState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
};

