#pragma once

struct AnimationInfo;

struct AnimationPoolBlock
{
	UINT16 refCount;
	MemoryBlock block;
};

class AnimationPool
{
public:
	AnimationPool();
	~AnimationPool();

public:
	bool PoolInAnimation(const AnimationInfo& key);
	bool PoolOutAnimation(const AnimationInfo& key, OUT MemoryBlock& memoryBlock);
	
private:
	/// <summary>
	/// Key : Hash<AnimationInfo>::GetHash() Value : AnimationPoolBlock
	/// </summary>
	HashTable<UINT64, AnimationPoolBlock> _table;
};

