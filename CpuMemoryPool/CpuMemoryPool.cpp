#include "pch.h"
#include "CpuMemoryPool.h"
#include "MemoryPage.h"
#include "MemoryBlockStack.h"

UINT8 CpuMemoryPool::s_nextPoolID = 0;

CpuMemoryPool::CpuMemoryPool(eBlockSize blockSize) : _blockSize(blockSize), _pageCount(MemoryPage::TOTAL_PAGE_SIZE / blockSize)
{
	_poolID = AllocatePoolId();

	InitPage();
}

CpuMemoryPool::CpuMemoryPool(eBlockSize blockSize, UINT pageCount) : _blockSize(blockSize), _pageCount(pageCount)
{
	InitPage();
}

CpuMemoryPool::~CpuMemoryPool()
{
	for (int i = 0; i < _pageCount; ++i)
	{
		delete _pages[i];
	}
	free(_pages);

	delete _pageStack;
}

void CpuMemoryPool::InitPage()
{
	_pages = (MemoryPage**)malloc(sizeof(MemoryPage*) * _pageCount);

	for (int i = 0; i < _pageCount; ++i)
	{
		_pages[i] = new MemoryPage(_poolID, _blockSize, i);
	}

	_pageStack = new MemoryBlockStack(_pageCount);
}

void CpuMemoryPool::CreatePage(UINT count)
{
	if(count <= 0) return;
	UINT totalCount = _pageCount + count;
	MemoryPage** newPage = (MemoryPage**)malloc(sizeof(MemoryPage*) * totalCount);
	memmove(newPage, _pages, sizeof(MemoryPage*) * (_pageCount));

	for (int i = 0; i < count; ++i)
	{
		int index = i + _pageCount;
		newPage[index] = new MemoryPage(_poolID, _blockSize, index);
	}
	free(_pages);

	_pages = newPage;
	_pageCount = totalCount;
	_pageStack->AddTotalCount(count);
}

bool CpuMemoryPool::ReleaseMemory(MemoryBlock & blockInfo)
{
	if(_poolID != blockInfo._poolID) return false;

	UINT pageIndex = blockInfo._pageIndex;
	if(pageIndex >= _pageCount) return false;

	_pages[pageIndex]->ReleaseMemory(blockInfo);

	if (_pages[pageIndex]->CanGettingMemory())
	{
		_pageStack->Push(pageIndex);
	}

	return true;
}
