#include "pch.h"
#include "MemoryBlockStack.h"
#include "MemoryPage.h"

MemoryBlockStack::MemoryBlockStack(UINT count) : _totalCount(count)
{
    CreateStack();
}

MemoryBlockStack::~MemoryBlockStack()
{
    free(_nodes);
}

void MemoryBlockStack::CreateStack()
{
    _nodes = (UINT*)malloc(sizeof(UINT) * _totalCount);
    _currentCount = _totalCount;

    for (int index = 0; index < _totalCount; ++index)
    {
        _nodes[index] = index;
    }

    _capacity = _totalCount;
}

const bool MemoryBlockStack::Pop(OUT UINT& index)
{
    bool isSuccess = GetIndex(index);
    if (isSuccess == false) return false;
    _currentCount--;
    return true;
}

const bool MemoryBlockStack::GetIndex(OUT UINT& index)
{
    if (CanPop() == false) return false;

    UINT& returnNode = _nodes[_totalCount - _currentCount];

    index = returnNode;
    return true;
}

bool MemoryBlockStack::Push(const UINT index)
{
    if(_currentCount >= _totalCount) return false;

    _currentCount++;
    _nodes[_totalCount - _currentCount] = index;

    return true;
}

bool MemoryBlockStack::CanPop()
{
    return _currentCount > 0;
}

void MemoryBlockStack::AddTotalCount(UINT count)
{
    int amount = _totalCount + count;
    UINT* newNodes = (UINT*)malloc(sizeof(UINT) * amount);
    memmove(newNodes, _nodes, sizeof(UINT) * _totalCount);
    free(_nodes);
    _nodes = newNodes;

    for (int i = 0; i < count; ++i)
    {
        int index = i + _totalCount;
        _nodes[index] = index;
    }

    _totalCount = amount;
    _currentCount += count;
}
