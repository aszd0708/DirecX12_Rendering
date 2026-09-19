#pragma once
#include "GpuBufferInfo.h"
#include "GpuBumpBufferPool.h"
#include "GpuDynamicBufferPool.h"

class GpuBufferPoolManager
{
	DECLARE_SINGLE(GpuBufferPoolManager);
	// Buffer Pool
public:
	enum class eBufferPoolID : UINT8
	{
		NONE = 0,

		INDEX_BUMP = 1 << 0,
		VERTEX_BUMP = 1 << 1,

		INDEX_DYNAMIC = 1 << 2,
		VERTEX_DYNAMIC = 1 << 3,

		MAX
	};
	struct sBufferPoolManagerInfo
	{
		eBufferPoolID initPoolFlag;

		UINT64 bumpIndexMaxSize;
		UINT64 bumpVertexMaxSize;

		UINT64 dynamicIndexMaxSize;
		UINT64 dynamicVertexMaxSize;
	};

public:
	void Init(sBufferPoolManagerInfo info);
	void Release();

	bool GetMemory(eBufferPoolID poolID, eGpuBufferKind type, UINT64 size, UINT8 alignment, OUT GpuBufferHandle& handle);
	bool ReleaseMemory(const GpuBufferHandle& handle);

	void ResetAllMemporyPool();

	const ComPtr<ID3D12Resource>& GetMemoryHeap(eBufferPoolID poolID, UINT8 pageIndex);
	
private:
	eBufferPoolID _usingBufferPoolIDs;

	GpuBumpBufferPool* _bumpIndexPool = nullptr;
	GpuBumpBufferPool* _bumpVertexPool = nullptr;
	
	GpuDynamicBufferPool* _dynamicIndexPool = nullptr;
	GpuDynamicBufferPool* _dynamicVertexPool = nullptr;
};

inline GpuBufferPoolManager::eBufferPoolID operator&(GpuBufferPoolManager::eBufferPoolID lhs, GpuBufferPoolManager::eBufferPoolID rhs)
{
	return static_cast<GpuBufferPoolManager::eBufferPoolID>(static_cast<UINT8>(lhs) & static_cast<UINT8>(rhs));
}

inline GpuBufferPoolManager::eBufferPoolID operator|(GpuBufferPoolManager::eBufferPoolID lhs, GpuBufferPoolManager::eBufferPoolID rhs)
{
	return static_cast<GpuBufferPoolManager::eBufferPoolID>(static_cast<UINT8>(lhs) | static_cast<UINT8>(rhs));
}

inline bool HasFlag(GpuBufferPoolManager::eBufferPoolID bitmask, GpuBufferPoolManager::eBufferPoolID flag)
{
	return (bitmask & flag) == flag;
}