#include "pch.h"
#include "GpuMemoryPoolManager.h"

void GpuMemoryPoolManager::Init(sMemoryPoolManagerInfo info)
{
	_usingMemoryPoolIDs = info.initPoolFlag;
	if ((info.initPoolFlag & eMemoryPoolID::BUMP) == eMemoryPoolID::BUMP)
	{
		_bumpPool = new GpuBumpMemoryPool((int)eMemoryPoolID::BUMP, info.bumbMaxSize, true);
	}
	if ((info.initPoolFlag & eMemoryPoolID::DYNAMIC) == eMemoryPoolID::DYNAMIC)
	{
		_dynamicPool = new GpuDynamicMemoryPool((int)eMemoryPoolID::DYNAMIC, info.dynamicMaxSize, true);
	}
	if ((info.initPoolFlag & eMemoryPoolID::BUMP_ONLY_64KB) == eMemoryPoolID::BUMP_ONLY_64KB)
	{
		_bumpPoolOnly64KB = new GpuBumpMemoryPool((int)eMemoryPoolID::BUMP_ONLY_64KB, info.bumbOnly64KBMaxSize, false);
	}
	if ((info.initPoolFlag & eMemoryPoolID::DYNAMIC_ONLY_64) == eMemoryPoolID::DYNAMIC_ONLY_64)
	{
		_dynamicPoolOnly64KB = new GpuDynamicMemoryPool((int)eMemoryPoolID::DYNAMIC_ONLY_64, info.dynamicOnly64KBMaxSize, false);
	}
	if ((info.initPoolFlag & eMemoryPoolID::DYNAMIC_UPLOAD) == eMemoryPoolID::DYNAMIC_UPLOAD)
	{
		_dyanamicPoolUpload = new GpuDynamicMemoryPool((int)eMemoryPoolID::DYNAMIC_UPLOAD, info.dynamicUploadMaxSize, false);
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

bool GpuMemoryPoolManager::GetMemory(eMemoryPoolID poolID, eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle)
{
	if (HasFlag(_usingMemoryPoolIDs, poolID) == false) return false;

	bool isSuccess = false;
	switch (poolID)
	{
	case eMemoryPoolID::BUMP:
		isSuccess = _bumpPool->GetMemoryHandle(type, size, handle);
		break;
	case eMemoryPoolID::DYNAMIC:
		isSuccess = _dynamicPool->GetMemoryHandle(type, size, handle);
		break;
	case eMemoryPoolID::BUMP_ONLY_64KB:
		isSuccess = _bumpPoolOnly64KB->GetMemoryHandle(type, size, handle);
		break;
	case eMemoryPoolID::DYNAMIC_ONLY_64:
		isSuccess = _dynamicPoolOnly64KB->GetMemoryHandle(type, size, handle);
		break;
	case eMemoryPoolID::DYNAMIC_UPLOAD:
		isSuccess = _dyanamicPoolUpload->GetMemoryHandle(type, size, handle);
		break;
	}
	return isSuccess;
}

bool GpuMemoryPoolManager::ReleaseMemory(const GpuMemoryHandle & handle)
{
	bool isSuccess = false;
	switch ((eMemoryPoolID)handle.memoryPoolID)
	{
	case eMemoryPoolID::BUMP:
	case eMemoryPoolID::BUMP_ONLY_64KB:
		// Bump 타입은 해제가 없음
		assert(false);
		break;
	case eMemoryPoolID::DYNAMIC:
		isSuccess = _dynamicPool->ReleaseMemoryHandle(handle.poolType, handle);
		break;
	case eMemoryPoolID::DYNAMIC_ONLY_64:
		isSuccess = _dynamicPoolOnly64KB->ReleaseMemoryHandle(handle.poolType, handle);
		break;
	case eMemoryPoolID::DYNAMIC_UPLOAD:
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

const ComPtr<ID3D12Heap>& GpuMemoryPoolManager::GetMemoryHeap(eMemoryPoolID poolID)
{
	if(HasFlag(_usingMemoryPoolIDs, poolID) == false) return nullptr;

	switch (poolID)
	{
	case eMemoryPoolID::BUMP:
		return _bumpPool->GetMemoryHeap();
	case eMemoryPoolID::DYNAMIC:
		return _dynamicPool->GetMemoryHeap();
	case eMemoryPoolID::BUMP_ONLY_64KB:
		return _bumpPoolOnly64KB->GetMemoryHeap();
	case eMemoryPoolID::DYNAMIC_ONLY_64:
		return _dynamicPoolOnly64KB->GetMemoryHeap();
	case eMemoryPoolID::DYNAMIC_UPLOAD:
		return _dyanamicPoolUpload->GetMemoryHeap();
	}
	return nullptr;
}