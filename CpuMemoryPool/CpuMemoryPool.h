#pragma once
#include "MemoryPage.h"
#include "MemoryBlockStack.h"

#include "Types.h"
#include "Defines.h"
#include "Interfaces.h"
#include "MemoryList.h"

class CpuMemoryPool
{
public:
	CpuMemoryPool(eBlockSize blockSize);
	CpuMemoryPool(eBlockSize blockSize, UINT pageCount);
	~CpuMemoryPool();

private:
	void InitPage();
	void CreatePage(UINT count);

public:
	template<typename T, typename... Args>
	bool GetMemory(OUT T** memory, Args&&... args);
	template<typename T>
	bool ReleaseMemory(T* memory);

	template<typename T>
	bool GetObject(MemoryBlock& blockInfo, OUT T** obj);

	bool ReleaseMemory(MemoryBlock& blockInfo);

	const eBlockSize& GetBlockSize() { return _blockSize; }
	const UINT& GetPageCount() { return _pageCount; }

private:
	eBlockSize _blockSize;

	UINT _pageCount;
	MemoryPage** _pages;
	MemoryBlockStack* _pageStack;
};

template<typename T, typename... Args>
inline bool CpuMemoryPool::GetMemory(OUT T** memory, Args&&... args)
{
	if (sizeof(T) > (int)_blockSize)
	{
		// 사이즈 맞지 않음
		assert(false);
		return false;
	}

	bool isSuccess = false;
	BYTE* rawPtr = nullptr;
	MemoryBlock blockInfo;

	if (_pageStack->CanPop() == false)
	{
		// 만약, 페이지가 모자랄 경우
		CreatePage(_pageCount);
		return GetMemory(memory, std::forward<Args>(args)...);
	}

	UINT index = 0;
	bool isSuccecss = _pageStack->GetIndex(index);
	isSuccess = _pages[index]->GetMemory(blockInfo, &rawPtr);
	if (isSuccess == false) return false;

	if (_pages[index]->CanGettingMemory() == false)
	{
		UINT dummy = 0;
		_pageStack->Pop(dummy);
	}

	*memory = new (rawPtr)T(std::forward<Args>(args)...);
	(*memory)->SetMemoryHandler(blockInfo);
	return true;
}

template<typename T>
bool CpuMemoryPool::ReleaseMemory(T* obj)
{
	obj->~T();
	return ReleaseMemory(obj->GetMemoryHandler());
}

template<typename T>
inline bool CpuMemoryPool::GetObject(MemoryBlock& blockInfo, OUT T** obj)
{
	BYTE* rawPtr = nullptr;
	UINT pageIndex = blockInfo._pageIndex;
	bool isSuccess = _pages[pageIndex]->ResolveMemory(blockInfo, &rawPtr);
	if(isSuccess == false) return false;

	*obj = reinterpret_cast<T*>(rawPtr);
}
