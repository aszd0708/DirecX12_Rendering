#pragma once

struct MaterialInfo;
class GpuCommandInfo;

struct MaterialPoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class MaterialPool
{
public:
	MaterialPool();
	~MaterialPool();

public:
	bool PoolInMaterial(const UINT64 key);
	bool PoolOutMaterial(const MaterialInfo& key, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<MaterialInfo>::GetHash() Value : MaterialPoolBlock
	/// </summary>
	HashTable<UINT64, MaterialPoolBlock> _table;
};

