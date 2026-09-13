#include "pch.h"
#include "TexturePool.h"
#include "TextureInfo.h"
#include "Texture.h"

TexturePool::TexturePool() : _table()
{

}

TexturePool::~TexturePool()
{

}

bool TexturePool::PoolInTexture(const TextureInfo& key)
{
	UINT64 hashValue = Hash<TextureInfo>::GetHash(key);
	TexturePoolBlock poolBlock = {};
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

			Texture* texture = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &texture);
			assert(isSuccess);
			pool->ReleaseMemory(texture);
			_table.RemoveKey(hashValue);
		}
		else
		{
			_table.Add(hashValue, poolBlock);
		}
	}

	return isSuccess;
}

bool TexturePool::PoolOutTexture(const TextureInfo& key, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<TextureInfo>::GetHash(key);
	TexturePoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Texture* texture = nullptr;
		UINT8 poolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Texture), poolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(poolID)->GetMemory(&texture, key);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		memoryBlock = texture->GetMemoryHandler();

		TexturePoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;
		
		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
