#include "pch.h"
#include "MemoryPage.h"
#include "MemoryBlockStack.h"

MemoryPage::MemoryPage(UINT8 poolID, eBlockSize size, UINT pageIndex) : _poolID(poolID), _size(size), _pageIndex(pageIndex)
{
    SetBlockCount();
}

MemoryPage::~MemoryPage()
{
    free(_blocks);
    free(_blockInfos);
    delete _usableIndexStack;
}

void MemoryPage::SetBlockCount()
{
    _totalBlockCount = TOTAL_PAGE_SIZE / (UINT)_size;

    _blocks = (BYTE*)malloc(TOTAL_PAGE_SIZE);
    _usableIndexStack = new MemoryBlockStack(_totalBlockCount);

    _blockInfos = (MemoryBlock*)malloc(sizeof(MemoryBlock) * _totalBlockCount);
    for (int i = 0; i < _totalBlockCount; ++i)
    {
        MemoryBlock& blockInfo = _blockInfos[i];
        blockInfo._index = i;
        blockInfo._poolID = _poolID;
        blockInfo._pageIndex = _pageIndex;
        blockInfo._gen = 0;
    }
}

bool MemoryPage::ResolveMemory(MemoryBlock& blockInfo, OUT BYTE** memory)
{
    int index = blockInfo._index;
    if(blockInfo._gen != _blockInfos[index]._gen) 
        return false;

    *memory = &_blocks[blockInfo._index * (int)_size];
    return true;
}

bool MemoryPage::GetMemory(OUT MemoryBlock& blockInfo, OUT BYTE** memory)
{
    UINT index = 0;
    bool isSuccess = _usableIndexStack->Pop(index);
    if(isSuccess == false) return false;

    blockInfo = _blockInfos[index];
    *memory = &_blocks[index * (int)_size];
    return true;
}

bool MemoryPage::ReleaseMemory(MemoryBlock& blockInfo)
{
    _usableIndexStack->Push(blockInfo._index);
    _blockInfos[blockInfo._index]._gen++;
    return true;
}

bool MemoryPage::CanGettingMemory()
{
    return _usableIndexStack->CanPop();
}

bool MemoryBlock::operator==(const MemoryBlock& other) const
{
    return _index == other._index && _pageIndex == other._pageIndex;
}

bool MemoryBlock::IsValid(const MemoryBlock& other) const
{
    return _index == other._index && _pageIndex == other._pageIndex && _gen == other._gen;
}
