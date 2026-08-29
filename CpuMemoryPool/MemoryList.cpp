#include "pch.h"
#include "MemoryList.h"
#include "MemoryPage.h"

MemoryList::MemoryList(CpuMemoryPool* pool) : _pool(pool)
{
	_list = (MemoryBlock*)malloc(sizeof(MemoryBlock) * DEFAULT_CAPACITY);
	assert(_list != nullptr);
	_capacity = DEFAULT_CAPACITY;
	_count = 0;
}

MemoryList::MemoryList(CpuMemoryPool * pool, UINT capacity) : _pool(pool), _capacity(capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}
	_list = (MemoryBlock*)malloc(sizeof(MemoryBlock) * _capacity);
	assert(_list != nullptr);
	_count = 0;
}

MemoryList::~MemoryList()
{
	free(_list);
}

bool MemoryList::GetMemoryBlock(UINT index, OUT MemoryBlock& block)
{
	if (index >= _count) return false;

	block = _list[index];

	return true;
}

void MemoryList::Add(const MemoryBlock& block)
{
	if (_capacity < _count + 1)
	{
		SetCapacity(_capacity * 2);
	}

	_list[_count] = block;
	_count++;
}

void MemoryList::AddRange(MemoryBlock* blocks, UINT addCount)
{
	if (_capacity < _count + addCount)
	{
		SetCapacity(_capacity * 2);
	}

	memmove(&_list[_count], blocks, sizeof(MemoryBlock) * addCount);

	_count += addCount;
}

void MemoryList::Remove(const MemoryBlock& block)
{
	int index;
	if(FindMemory(block, index) == false) return; 

	memmove(&_list[index], &_list[index + 1], sizeof(MemoryBlock) * (_count - index - 1));
	_count--;
}

void MemoryList::RemoveRange(UINT startIndex, UINT blockCount)
{
	memmove(&_list[startIndex], &_list[startIndex + blockCount], sizeof(MemoryBlock) * (blockCount));
}

void MemoryList::RemoveUnordered(const MemoryBlock& block)
{
	int index;
	if (FindMemory(block, index) == false) return;

	if (index != _count - 1)
	{
		memmove(&_list[index], &_list[_count - 1], sizeof(MemoryBlock));
	}
	_count--;
}

void MemoryList::RemoveUnordered(const MemoryBlock* blocks, UINT count)
{
	for (int i = 0; i < count; ++i)
	{
		RemoveUnordered(blocks[i]);
	}
}

void MemoryList::SetCapacity(UINT capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}

	MemoryBlock* newList = (MemoryBlock*)malloc(sizeof(MemoryBlock) * capacity);
	assert(newList != nullptr);

	memcpy(newList, _list, sizeof(MemoryBlock) * _capacity);
	free(_list);

	_capacity = capacity;
	_list = newList;
}

bool MemoryList::FindMemory(const MemoryBlock& block, OUT int& index)
{
	index = -1;
	for (int i = 0; i < _count; ++i)
	{
		if (_list[i] == block)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		// Not Found
		return false;
	}
	return true; 
}
