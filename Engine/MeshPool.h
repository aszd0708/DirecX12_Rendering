#pragma once
#include "GpuCommandPool.h"

struct MeshInfo;

struct MeshPoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class MeshPool
{
public:
	MeshPool();
	~MeshPool();

public:
	bool PoolInMesh(const MeshInfo& key);
	bool PoolOutMesh(MeshInfo& key, const GpuBufferPoolManager::eBufferPoolID vertexPoolID, const GpuBufferPoolManager::eBufferPoolID indexPoolID, GpuCommandInfo* commandPool, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<TextureInfo>::GetHash() Value : TexturePoolBlock
	/// </summary>
	HashTable<UINT64, MeshPoolBlock> _table;
};

