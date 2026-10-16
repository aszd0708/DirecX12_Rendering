#include "pch.h"
#include "MaterialPool.h"
#include "Material.h"

MaterialPool::MaterialPool() : _table()
{}

MaterialPool::~MaterialPool()
{}

bool MaterialPool::PoolInMaterial(const UINT64 key)
{
	MaterialPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(key, poolBlock);
	if (isSuccess)
	{
		assert(poolBlock.refCount != 0);
		poolBlock.refCount--;

		if (poolBlock.refCount <= 0)
		{
			// 여기 지연 해제 하는 부분
			// 일단 CPU풀에 바로 제거
			CpuMemoryPool* pool = CPU_MEM_POOL->GetMemoryPool(poolBlock.block._poolID);

			Material* material = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &material);
			assert(isSuccess);

			pool->ReleaseMemory(material);
			_table.RemoveKey(key);
		}
		else
		{
			_table.Add(key, poolBlock);
		}
	}

	return isSuccess;
}

bool MaterialPool::PoolOutMaterial(const MaterialInfo& key, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<MaterialInfo>::GetHash(key);
	MaterialPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Material* material = nullptr;
		UINT8 cpuPoolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Material), cpuPoolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(cpuPoolID)->GetMemory(&material, key, commandInfo);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		// Material 생성


		memoryBlock = material->GetMemoryHandler();

		MaterialPoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;

		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
