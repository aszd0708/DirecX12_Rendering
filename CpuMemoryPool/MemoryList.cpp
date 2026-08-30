#include "pch.h"
#include "MemoryList.h"
#include "MemoryPage.h"

MemoryList::MemoryList()
{
	_list = (MemoryEntry*)malloc(sizeof(MemoryEntry) * DEFAULT_CAPACITY);
	assert(_list != nullptr);
	_capacity = DEFAULT_CAPACITY;
	_count = 0;
}

MemoryList::MemoryList(UINT capacity) :  _capacity(capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}
	_list = (MemoryEntry*)malloc(sizeof(MemoryEntry) * _capacity);
	assert(_list != nullptr);
	_count = 0;
}

MemoryList::~MemoryList()
{
	free(_list);
}

bool MemoryList::GetMemoryBlock(UINT index, OUT MemoryEntry& block)
{
	if (index >= _count) return false;

	block = _list[index];

	return true;
}

void MemoryList::Add(const MemoryEntry& block)
{
	if (_capacity < _count + 1)
	{
		SetCapacity(_capacity * 2);
	}

	_list[_count] = block;
	_count++;
}

void MemoryList::AddAt(const MemoryEntry& block, UINT index)
{
	if(_capacity <= index) 
	{
		assert(false);
		return;
	}

	_list[index] = block;
	_count = std::max(_count, index + 1);
}

void MemoryList::AddRange(MemoryEntry* blocks, UINT addCount)
{
	if (_capacity < _count + addCount)
	{
		SetCapacity(_capacity * 2 + addCount);
	}

	memmove(&_list[_count], blocks, sizeof(MemoryEntry) * addCount);

	_count += addCount;
}

void MemoryList::Remove(const MemoryBlock& block)
{
	int index;
	if(FindMemory(block, index) == false) 
		return; 

	memmove(&_list[index], &_list[index + 1], sizeof(MemoryEntry) * (_count - index - 1));
	_count--;
}

void MemoryList::RemoveRange(UINT startIndex, UINT blockCount)
{
	memmove(&_list[startIndex], &_list[startIndex + blockCount], sizeof(MemoryEntry) * (blockCount));
	_count - startIndex - blockCount;
}

void MemoryList::RemoveUnordered(const MemoryBlock& block)
{
	int index;
	if (FindMemory(block, index) == false) return;

	if (index != _count - 1)
	{
		memmove(&_list[index], &_list[_count - 1], sizeof(MemoryEntry));
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

void MemoryList::RemoveAt(const UINT index)
{
	memmove(&_list[index], &_list[index + 1], sizeof(MemoryEntry) * (_count - index - 1));
	_count--;
}

void MemoryList::RemoveAtUnordered(const UINT index)
{
	if (index != _count - 1)
	{
		memmove(&_list[index], &_list[_count - 1], sizeof(MemoryEntry));
	}
	_count--;
}

void MemoryList::SetCapacity(UINT capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}

	MemoryEntry* newList = (MemoryEntry*)malloc(sizeof(MemoryEntry) * capacity);
	assert(newList != nullptr);

	memcpy(newList, _list, sizeof(MemoryEntry) * _capacity);
	free(_list);

	_capacity = capacity;
	_list = newList;
}

bool MemoryList::FindMemory(const MemoryBlock& block, OUT int& index)
{
	index = -1;
	for (int i = 0; i < _count; ++i)
	{
		if (_list[i].block == block)
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
