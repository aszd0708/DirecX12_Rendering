#include "pch.h"
#include "GpuMemoryPoolManager.h"

void GpuMemoryPoolManager::Init(sMemoryPoolManagerInfo info)
{
	_usingPoolIDs = info.initPoolFlag;
	if ((info.initPoolFlag & ePoolID::BUMP) == ePoolID::BUMP)
	{
		_bumpPool = new GpuBumpMemoryPool((int)ePoolID::BUMP, info.bumbMaxSize, true);
	}
	if ((info.initPoolFlag & ePoolID::DYNAMIC) == ePoolID::DYNAMIC)
	{
		_dynamicPool = new GpuDynamicMemoryPool((int)ePoolID::DYNAMIC, info.dynamicMaxSize, true);
	}
	if ((info.initPoolFlag & ePoolID::BUMP_ONLY_64KB) == ePoolID::BUMP_ONLY_64KB)
	{
		_bumpPoolOnly64KB = new GpuBumpMemoryPool((int)ePoolID::BUMP_ONLY_64KB, info.bumbOnly64KBMaxSize, false);
	}
	if ((info.initPoolFlag & ePoolID::DYNAMIC_ONLY_64) == ePoolID::DYNAMIC_ONLY_64)
	{
		_dynamicPoolOnly64KB = new GpuDynamicMemoryPool((int)ePoolID::DYNAMIC_ONLY_64, info.dynamicOnly64KBMaxSize, false);
	}
	if ((info.initPoolFlag & ePoolID::DYNAMIC_UPLOAD) == ePoolID::DYNAMIC_UPLOAD)
	{
		_dyanamicPoolUpload = new GpuDynamicMemoryPool((int)ePoolID::DYNAMIC_UPLOAD, info.dynamicUploadMaxSize, false);
	}
}

void GpuMemoryPoolManager::Release()
{
	delete _bumpPool;
	delete _dynamicPool;
	delete _bumpPoolOnly64KB;
	delete _dynamicPoolOnly64KB;
	delete _dyanamicPoolUpload;
}

bool GpuMemoryPoolManager::GetMemory(ePoolID poolID, eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle)
{
	if (HasFlag(_usingPoolIDs, poolID) == false) return false;

	bool isSuccess = false;
	switch (poolID)
	{
	case ePoolID::BUMP:
		isSuccess = _bumpPool->GetMemoryHandle(type, size, handle);
		break;
	case ePoolID::DYNAMIC:
		isSuccess = _dynamicPool->GetMemoryHandle(type, size, handle);
		break;
	case ePoolID::BUMP_ONLY_64KB:
		isSuccess = _bumpPoolOnly64KB->GetMemoryHandle(type, size, handle);
		break;
	case ePoolID::DYNAMIC_ONLY_64:
		isSuccess = _dynamicPoolOnly64KB->GetMemoryHandle(type, size, handle);
		break;
	case ePoolID::DYNAMIC_UPLOAD:
		isSuccess = _dyanamicPoolUpload->GetMemoryHandle(type, size, handle);
		break;
	}
	return isSuccess;
}

bool GpuMemoryPoolManager::ReleaseMemory(const GpuMemoryHandle & handle)
{
	bool isSuccess = false;
	switch ((ePoolID)handle.memoryPoolID)
	{
	case ePoolID::BUMP:
	case ePoolID::BUMP_ONLY_64KB:
		// Bump 타입은 해제가 없음
		assert(false);
		break;
	case ePoolID::DYNAMIC:
		isSuccess = _dynamicPool->ReleaseMemoryHandle(handle.poolType, handle);
		break;
	case ePoolID::DYNAMIC_ONLY_64:
		isSuccess = _dynamicPoolOnly64KB->ReleaseMemoryHandle(handle.poolType, handle);
		break;
	case ePoolID::DYNAMIC_UPLOAD:
		isSuccess = _dyanamicPoolUpload->ReleaseMemoryHandle(handle.poolType, handle);
		break;
	}
	return isSuccess;
}

void GpuMemoryPoolManager::ResetAllMemporyPool()
{
	if (_bumpPool != nullptr)
	{
		_bumpPool->ResetAllPage();
	}
	if (_dynamicPool != nullptr)
	{
		_dynamicPool->ResetAllPage();
	}
	if (_bumpPoolOnly64KB != nullptr)
	{
		_bumpPoolOnly64KB->ResetAllPage();
	}
	if (_dynamicPoolOnly64KB != nullptr)
	{
		_dynamicPoolOnly64KB->ResetAllPage();
	}
	if (_dyanamicPoolUpload != nullptr)
	{
		_dyanamicPoolUpload->ResetAllPage();
	}
}

const ComPtr<ID3D12Heap>& GpuMemoryPoolManager::GetMemoryHeap(ePoolID poolID)
{
	if(HasFlag(_usingPoolIDs, poolID) == false) return nullptr;

	switch (poolID)
	{
	case ePoolID::BUMP:
		return _bumpPool->GetMemoryHeap();
	case ePoolID::DYNAMIC:
		return _dynamicPool->GetMemoryHeap();
	case ePoolID::BUMP_ONLY_64KB:
		return _bumpPoolOnly64KB->GetMemoryHeap();
	case ePoolID::DYNAMIC_ONLY_64:
		return _dynamicPoolOnly64KB->GetMemoryHeap();
	case ePoolID::DYNAMIC_UPLOAD:
		return _dyanamicPoolUpload->GetMemoryHeap();
	}
	return nullptr;
}