#pragma once
#include "CpuMemoryPool.h"

struct ShaderInfo;

struct ShaderPoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class ShaderPool
{
public:
	ShaderPool();
	~ShaderPool();

public:
	bool PoolInShader(const ShaderInfo& key);
	bool PoolOutShader(const ShaderInfo& key, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<ShaderInfo>::GetHash() Value : ShaderPoolBlock
	/// </summary>
	HashTable<UINT64, ShaderPoolBlock> _table;

	Stack<UINT32> _usableInstancingIDs;
	UINT32 _curInstancingID = 0;
};

