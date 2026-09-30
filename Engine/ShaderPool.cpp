#include "pch.h"
#include "Shader.h"
#include "ShaderPool.h"

ShaderPool::ShaderPool() : _table()
{

}

ShaderPool::~ShaderPool()
{

}

bool ShaderPool::PoolInShader(const ShaderInfo & key)
{
	UINT64 hashValue = Hash<ShaderInfo>::GetHash(key);
	ShaderPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		assert(poolBlock.refCount != 0);
		poolBlock.refCount--;

		if (poolBlock.refCount <= 0)
		{
			// 여기 지연 해제 하는 부분
			// 일단 CPU풀에 바로 제거
			CpuMemoryPool* pool = CPU_MEM_POOL->GetMemoryPool(poolBlock.block._poolID);

			Shader* shader = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &shader);
			assert(isSuccess);
			_usableInstancingIDs.Push(shader->GetInstancingID());
			pool->ReleaseMemory(shader);
			_table.RemoveKey(hashValue);
		}
		else
		{
			_table.Add(hashValue, poolBlock);
		}
	}

	return isSuccess;
}

bool ShaderPool::PoolOutShader(const ShaderInfo& key, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<ShaderInfo>::GetHash(key);
	ShaderPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Shader* shader = nullptr;
		UINT8 poolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Shader), poolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(poolID)->GetMemory(&shader, key);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		UINT32 instancingID = _curInstancingID;
		bool usingCurInstancingID = true;
		if (_usableInstancingIDs.GetCount() > 0)
		{
			isSuccess = _usableInstancingIDs.Pop(instancingID);
			usingCurInstancingID = isSuccess == false;
			if (isSuccess == false)
			{
				instancingID = _curInstancingID;
			}
		}
		if (usingCurInstancingID)
		{
			_curInstancingID++;
		}

		shader->SetInstancingID(instancingID);

		memoryBlock = shader->GetMemoryHandler();

		ShaderPoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;

		

		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
