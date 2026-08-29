#pragma once
#include "Types.h"
#include "Defines.h"

class MemoryBlockStack;

class MemoryBlock
{
public:
	eBlockSize _size;

	UINT _pageIndex;

	UINT8 _index;
	UINT8 _gen;

	bool operator==(const MemoryBlock& other) const;
	bool IsValid(const MemoryBlock& other) const;
};

class MemoryPage
{
public:
	static const UINT TOTAL_PAGE_SIZE = 4096;

public:
	MemoryPage(eBlockSize size, UINT pageIndex);
	~MemoryPage();

private:
	void SetBlockCount();

public:
	bool ResolveMemory(MemoryBlock& blockInfo, OUT BYTE** memory);
	bool GetMemory(OUT MemoryBlock& blockInfo, OUT BYTE** memory);
	bool ReleaseMemory(MemoryBlock& blockInfo);
	const UINT& GetTotalBlockCount() { return _totalBlockCount; };
	bool CanGettingMemory();

private:
	const eBlockSize _size;
	UINT _pageIndex;

	UINT _totalBlockCount;

	BYTE* _blocks;
	MemoryBlock* _blockInfos;
	MemoryBlockStack* _usableIndexStack;
};

