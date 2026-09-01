#include "pch.h"
#include "MemoryList.h"
#include "MemoryPage.h"

MemoryList::MemoryList() : MemoryList(DEFAULT_CAPACITY)
{
	
}

MemoryList::MemoryList(UINT capacity) :  _capacity(capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}
	_list = (MemoryListSlot*)malloc(sizeof(MemoryListSlot) * _capacity);
	assert(_list != nullptr);
	for (int i = 0; i < _capacity; ++i)
	{
		_list[i].isValied = false;
	}
	_count = 0;
}

MemoryList::~MemoryList()
{
	free(_list);
}

bool MemoryList::GetMemoryBlock(UINT index, OUT MemoryEntry& block)
{
	if (index >= _count) return false;
	if(_list[index].isValied == false) return false;

	block = _list[index].entry;

	return true;
}

void MemoryList::Add(MemoryEntry& block)
{
	if (_capacity < _count + 1)
	{
		SetCapacity(_capacity * 2);
	}

	_list[_count].entry = block;
	_list[_count].isValied = true;
	_count++;
}

void MemoryList::AddAt(MemoryEntry& block, UINT index)
{
	if(_capacity <= index) 
	{
		assert(false);
		return;
	}

	_list[index].entry = block;
	_list[index].isValied = true;
	_count = max(_count, index + 1);
}

void MemoryList::AddRange(MemoryEntry* blocks, UINT addCount)
{
	if (_capacity < _count + addCount)
	{
		SetCapacity(_capacity * 2 + addCount);
	}

	for (int i = _count; i < _count + addCount; ++i)
	{
		_list[i].entry = blocks[i - _count];
		_list[i].isValied = true;
	}

	_count += addCount;
}

void MemoryList::Remove(const MemoryBlock& block)
{
	int index;
	if(FindMemory(block, index) == false) 
		return; 

	memmove(&_list[index], &_list[index + 1], sizeof(MemoryListSlot) * (_count - index - 1));
	_list[_count - 1].isValied = false;
	_count--;
}

void MemoryList::RemoveRange(UINT startIndex, UINT blockCount)
{
	if(startIndex + blockCount > _count) assert(false);

	memmove(&_list[startIndex], &_list[startIndex + blockCount], sizeof(MemoryListSlot) * (_count - blockCount - startIndex));
	_count = _count - startIndex - blockCount;
	for (int i = _count; i < _count + blockCount; ++i)
	{
		_list[i].isValied = false;
	}
}

void MemoryList::RemoveUnordered(const MemoryBlock& block)
{
	int index;
	if (FindMemory(block, index) == false) return;

	if (index != _count - 1)
	{
		memmove(&_list[index], &_list[_count - 1], sizeof(MemoryListSlot));
	}
	_list[_count - 1].isValied = false;
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
	memmove(&_list[index], &_list[index + 1], sizeof(MemoryListSlot) * (_count - index - 1));
	_list[_count - 1].isValied = false;
	_count--;
}

void MemoryList::RemoveAtUnordered(const UINT index)
{
	if (index != _count - 1)
	{
		memmove(&_list[index], &_list[_count - 1], sizeof(MemoryListSlot));
	}
	_list[_count - 1].isValied = false;
	_count--;
}

void MemoryList::SetCapacity(UINT capacity)
{
	if (_capacity == 0)
	{
		_capacity = DEFAULT_CAPACITY;
	}

	MemoryListSlot* newList = (MemoryListSlot*)malloc(sizeof(MemoryListSlot) * capacity);
	assert(newList != nullptr);

	memcpy(newList, _list, sizeof(MemoryListSlot) * _capacity);
	free(_list);

	for (int i = _capacity; i < capacity; ++i)
	{
		newList[i].isValied = false;
	}

	_capacity = capacity;
	_list = newList;
}

bool MemoryList::FindMemory(const MemoryBlock& block, OUT int& index)
{
	index = -1;
	for (int i = 0; i < _count; ++i)
	{
		if (_list[i].entry.block == block)
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
