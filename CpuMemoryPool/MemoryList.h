#pragma once

class CpuMemoryPool;
struct MemoryBlock;

class MemoryList
{
private:
	static const UINT DEFAULT_CAPACITY = 32;

public:
	MemoryList(CpuMemoryPool* pool);
	MemoryList(CpuMemoryPool* pool, UINT capacity);
	~MemoryList();

public:
	bool GetMemoryBlock(UINT index, OUT MemoryBlock& block);
	const UINT& GetCount() { return _count; }
	
	void Add(const MemoryBlock& block);
	void AddRange(MemoryBlock* blocks, UINT addCount);
	void Remove(const MemoryBlock& block);
	void RemoveRange(UINT startIndex, UINT blockCount);
	void RemoveUnordered(const MemoryBlock& block);
	void RemoveUnordered(const MemoryBlock* blocks, UINT count);
	void RemoveAt(const UINT index);
	void RemoveAtUnordered(const UINT index);
	void SetCapacity(UINT capacity);

	bool FindMemory(const MemoryBlock& block, OUT int& index);

private:
	CpuMemoryPool* _pool;

	UINT _count;
	UINT _capacity;
	MemoryBlock* _list;
};