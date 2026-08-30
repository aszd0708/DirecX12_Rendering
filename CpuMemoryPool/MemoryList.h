#pragma once
#include "MemoryPage.h"
#include <typeindex>
#include <typeinfo>

class CpuMemoryPool;

struct MemoryEntry
{
	MemoryBlock block;
	std::type_index type = typeid(void);;
};

class MemoryList
{
private:
	static const UINT DEFAULT_CAPACITY = 32;

public:
	MemoryList();
	MemoryList(UINT capacity);
	~MemoryList();

public:
	bool GetMemoryBlock(UINT index, OUT MemoryEntry& block);
	const UINT& GetCount() { return _count; }
	
	void Add(const MemoryEntry& block);
	void AddAt(const MemoryEntry& block, UINT index);
	void AddRange(MemoryEntry* blocks, UINT addCount);
	void Remove(const MemoryBlock& block);
	void RemoveRange(UINT startIndex, UINT blockCount);
	void RemoveUnordered(const MemoryBlock& block);
	void RemoveUnordered(const MemoryBlock* blocks, UINT count);
	void RemoveAt(const UINT index);
	void RemoveAtUnordered(const UINT index);
	void SetCapacity(UINT capacity);

	bool FindMemory(const MemoryBlock& block, OUT int& index);

private:
	UINT _count;
	UINT _capacity;
	MemoryEntry* _list;
};