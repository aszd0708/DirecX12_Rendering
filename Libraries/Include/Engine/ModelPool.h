#pragma once
struct ModelInfo;

struct ModelPoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class ModelPool
{
public:
	ModelPool();
	~ModelPool();

public:
	bool PoolInModel(const ModelInfo& key);
	bool PoolOutModel(ModelInfo& key, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<TextureInfo>::GetHash() Value : TexturePoolBlock
	/// </summary>
	HashTable<UINT64, ModelPoolBlock> _table;

	Stack<UINT32> _usableInstancingIDs;
	UINT32 _curInstancingID = 0;
};

