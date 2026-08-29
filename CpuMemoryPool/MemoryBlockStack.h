#pragma once

class MemoryPage;
struct MemoryBlock;

class CpuMemoryPool;

class MemoryBlockStack
{
public:
	MemoryBlockStack(UINT count);
	~MemoryBlockStack();

private:
	void CreateStack();

public:
	const bool Pop(OUT UINT& index);
	const bool GetIndex(OUT UINT& index);
	bool Push(const UINT index);

	bool CanPop();

	void AddTotalCount(UINT count);

private:
	UINT* _nodes;
	INT _currentCount;
	UINT _totalCount;

	UINT _capacity;
};