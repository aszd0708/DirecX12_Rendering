#include "pch.h"
#include "AnimationPool.h"
#include "AnimationData.h"
#include "Animation.h"
#include "AnimationInfo.h"
#include "ModelLoader.h"

AnimationPool::AnimationPool() : _table()
{

}

AnimationPool::~AnimationPool()
{

}

bool AnimationPool::PoolInAnimation(const AnimationInfo& key)
{
	UINT64 hashValue = Hash<AnimationInfo>::GetHash(key);
	AnimationPoolBlock poolBlock = {};
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

			Animation* animation = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &animation);
			assert(isSuccess);
			pool->ReleaseMemory(animation);
			_table.RemoveKey(hashValue);
		}
		else
		{
			_table.Add(hashValue, poolBlock);
		}
	}

	return isSuccess;
}

bool AnimationPool::PoolOutAnimation(const AnimationInfo& key, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<AnimationInfo>::GetHash(key);
	AnimationPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Animation* animation = nullptr;

		AnimationData data = {};
		isSuccess = ModelLoader::LoadAnimation(key.filePath, data);
		if (isSuccess == false)
		{
			return false;
		}

		UINT8 cpuPoolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Animation), cpuPoolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(cpuPoolID)->GetMemory(&animation);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		animation->Init(data);
		memoryBlock = animation->GetMemoryHandler();

		AnimationPoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;

		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
