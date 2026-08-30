#pragma once
#include "MemoryPage.h"
#include "CpuMemoryPool.h"

class IMemoryBlockHanlde
{
public:
	IMemoryBlockHanlde() { };
	virtual ~IMemoryBlockHanlde() { };

	virtual void SetMemoryHandler(const MemoryBlock& handler) { _memoryHandler = handler; };
	virtual MemoryBlock& GetMemoryHandler() { return _memoryHandler; }

protected: 
	MemoryBlock _memoryHandler;
};