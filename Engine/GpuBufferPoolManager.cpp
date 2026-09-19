#include "pch.h"
#include "GpuBufferPoolManager.h"

void GpuBufferPoolManager::Init(sBufferPoolManagerInfo info)
{
	_usingBufferPoolIDs = info.initPoolFlag;
	if ((info.initPoolFlag & eBufferPoolID::INDEX_BUMP) == eBufferPoolID::INDEX_BUMP)
	{
		_bumpIndexPool = new GpuBumpBufferPool((int)eBufferPoolID::INDEX_BUMP, info.bumpIndexMaxSize);
	}
	if ((info.initPoolFlag & eBufferPoolID::VERTEX_BUMP) == eBufferPoolID::VERTEX_BUMP)
	{
		_bumpVertexPool = new GpuBumpBufferPool((int)eBufferPoolID::VERTEX_BUMP, info.bumpVertexMaxSize);
	}
	if ((info.initPoolFlag & eBufferPoolID::INDEX_DYNAMIC) == eBufferPoolID::INDEX_DYNAMIC)
	{
		_dynamicIndexPool = new GpuDynamicBufferPool((int)eBufferPoolID::INDEX_DYNAMIC, info.dynamicIndexMaxSize);
	}
	if ((info.initPoolFlag & eBufferPoolID::VERTEX_DYNAMIC) == eBufferPoolID::VERTEX_DYNAMIC)
	{
		_dynamicVertexPool = new GpuDynamicBufferPool((int)eBufferPoolID::VERTEX_DYNAMIC, info.dynamicVertexMaxSize);
	}
}

void GpuBufferPoolManager::Release()
{
	delete _bumpIndexPool;
	delete _bumpVertexPool;
	delete _dynamicIndexPool;
	delete _dynamicVertexPool;
}

bool GpuBufferPoolManager::GetMemory(eBufferPoolID poolID, eGpuBufferKind type, UINT64 size, UINT8 alignment, OUT GpuBufferHandle & handle)
{
	if (HasFlag(_usingBufferPoolIDs, poolID) == false) return false;

	bool isSuccess = false;
	switch (poolID)
	{
	case eBufferPoolID::INDEX_BUMP:
		isSuccess = _bumpIndexPool->GetBufferHandle(type, size, alignment, handle);
		break;
	case eBufferPoolID::VERTEX_BUMP:
		isSuccess = _bumpVertexPool->GetBufferHandle(type, size, alignment, handle);
		break;
	case eBufferPoolID::INDEX_DYNAMIC:
		isSuccess = _dynamicIndexPool->GetBufferHandle(type, size, alignment, handle);
		break;
	case eBufferPoolID::VERTEX_DYNAMIC:
		isSuccess = _dynamicVertexPool->GetBufferHandle(type, size, alignment, handle);
		break;
	}
	return isSuccess;
}

bool GpuBufferPoolManager::ReleaseMemory(const GpuBufferHandle& handle)
{
	bool isSuccess = false;
	switch ((eBufferPoolID)handle.poolID)
	{
	case eBufferPoolID::INDEX_BUMP:
	case eBufferPoolID::VERTEX_BUMP:
		// Bump 타입은 해제가 없음
		assert(false);
		break;
	case eBufferPoolID::INDEX_DYNAMIC:
		isSuccess = _dynamicIndexPool->ReleaseBufferHandle(handle.pageIndex, handle);
		break;
	case eBufferPoolID::VERTEX_DYNAMIC:
		isSuccess = _dynamicVertexPool->ReleaseBufferHandle(handle.pageIndex, handle);
		break;
	}
	return isSuccess;
}

void GpuBufferPoolManager::ResetAllMemporyPool()
{
	if (_bumpIndexPool != nullptr)
	{
		_bumpIndexPool->ResetAllPage();
	}
	if (_bumpVertexPool != nullptr)
	{
		_bumpVertexPool->ResetAllPage();
	}
	if (_dynamicIndexPool != nullptr)
	{
		_dynamicIndexPool->ResetAllPage();
	}
	if (_dynamicVertexPool != nullptr)
	{
		_dynamicVertexPool->ResetAllPage();
	}
}

const ComPtr<ID3D12Resource>& GpuBufferPoolManager::GetMemoryHeap(eBufferPoolID poolID, UINT8 pageIndex)
{
	if (HasFlag(_usingBufferPoolIDs, poolID) == false) return nullptr;

	switch (poolID)
	{
	case eBufferPoolID::INDEX_BUMP:
		return _bumpIndexPool->GetMemoryHeap(pageIndex);
	case eBufferPoolID::VERTEX_BUMP:
		return _bumpVertexPool->GetMemoryHeap(pageIndex);
	case eBufferPoolID::INDEX_DYNAMIC:
		return _dynamicIndexPool->GetMemoryHeap(pageIndex);
	case eBufferPoolID::VERTEX_DYNAMIC:
		return _dynamicVertexPool->GetMemoryHeap(pageIndex);
	}
	return nullptr;
}
