#pragma once
#include "MemoryPage.h"
#include "CpuMemoryPool.h"

class IMemoryBlockHanlde
{
public:
	IMemoryBlockHanlde() {};
	virtual ~IMemoryBlockHanlde() {};

	//void SetMemoryBlockHandle(const UINT8& poolID, const MemoryBlock& handler) { SetPoolID(poolID); SetMemoryHandler(handler); }

	//virtual void SetPoolID(const UINT8& poolID) { _poolID = poolID; }
	//virtual const UINT8& GetPoolID() { return _poolID; }

	virtual void SetMemoryHandler(const MemoryBlock& handler) { _memoryHandler = handler; };
	virtual MemoryBlock& GetMemoryHandler() { return _memoryHandler; }

protected: 
	MemoryBlock _memoryHandler;
};