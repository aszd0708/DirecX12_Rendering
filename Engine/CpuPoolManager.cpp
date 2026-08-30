#include "pch.h"
#include "CpuPoolManager.h"

void CpuPoolManager::Init()
{
	_memoryPools = (CpuMemoryPool**)malloc(sizeof(CpuMemoryPool*) * (int)ePoolID::MAX);

	int temp = 64;
	for (int i = 0; i < (int)ePoolID::RENDERER; ++i)
	{
		eBlockSize size = (eBlockSize)(temp);
		_memoryPools[i] = new CpuMemoryPool(size, i);
		temp*= 2;
	}

	_memoryPools[(UINT8)ePoolID::RENDERER] = new CpuMemoryPool(eBlockSize::BYTE_512, (int)ePoolID::RENDERER);
}

void CpuPoolManager::Release()
{
	for (int i = 0; i < (int)ePoolID::MAX; ++i)
	{
		delete _memoryPools[i];
	}
	free(_memoryPools);
}

bool CpuPoolManager::GetPoolID(UINT size, OUT UINT8& poolID)
{
	int poolIndex = (int)ePoolID::MAX;
	int poolSize = (int)eBlockSize::BYTE_64;
	for (int sizeIndex = (int)ePoolID::CPU_64; sizeIndex < (int)ePoolID::MAX; ++sizeIndex)
	{
		if (poolSize >= size)
		{
			poolIndex = sizeIndex;
			break;
		}
		poolSize *= 2;
	}

	if (poolIndex == (int)ePoolID::MAX)
	{
		return false;
	}

	poolID = _memoryPools[poolIndex]->GetPoolID();
	return true;
}

CpuMemoryPool* CpuPoolManager::GetMemoryPool(UINT8& poolID)
{
	return _memoryPools[poolID];
}
