#pragma once
#include "CpuMemoryPool.h"

struct TextureInfo;
class GpuCommandInfo;

struct TexturePoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class TexturePool
{
public:
	TexturePool();
	~TexturePool();

public:
	bool PoolInTexture(const TextureInfo& key);
	bool PoolOutTexture(const TextureInfo& key, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock);

private:
	/// <summary>
	/// Key : Hash<TextureInfo>::GetHash() Value : TexturePoolBlock
	/// </summary>
	HashTable<UINT64, TexturePoolBlock> _table;
};

