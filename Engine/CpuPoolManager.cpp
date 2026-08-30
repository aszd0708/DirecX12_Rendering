#include "pch.h"
#include "CpuPoolManager.h"

void CpuPoolManager::Init()
{
	_memoryPools = (CpuMemoryPool**)malloc(sizeof(CpuMemoryPool*) * (int)eBlockSizeIndex::MAX);

	int temp = 64;
	for (int i = 0; i < (int)ePoolID::CPU_256; ++i)
	{
		eBlockSize size = (eBlockSize)((eBlockSizeIndex)i * temp);
		_memoryPools[i] = new CpuMemoryPool(size, i);
		temp*= 2;
	}

	_memoryPools[(UINT8)ePoolID::RENDERER] = new CpuMemoryPool(eBlockSize::BYTE_256, (int)ePoolID::RENDERER);
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
	int poolIndex = (int)eBlockSizeIndex::MAX;
	int poolSize = (int)eBlockSize::BYTE_64;
	for (int sizeIndex = (int)eBlockSizeIndex::MIN; sizeIndex < (int)eBlockSizeIndex::MAX; ++sizeIndex)
	{
		if (poolSize >= size)
		{
			poolIndex = sizeIndex;
			break;
		}
		poolSize *= 2;
	}

	if (poolIndex == (int)eBlockSizeIndex::MAX)
	{
		return false;
	}

	poolID = _memoryPools[poolIndex]->GetPoolID();
	return true;
}

const CpuMemoryPool* CpuPoolManager::GetMemoryPool(UINT8& poolID)
{
	return _memoryPools[poolID];
}
