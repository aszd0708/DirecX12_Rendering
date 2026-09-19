#pragma once

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
	bool PoolOutMesh(MeshInfo& key, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<TextureInfo>::GetHash() Value : TexturePoolBlock
	/// </summary>
	HashTable<UINT64, MeshPoolBlock> _table;
};

