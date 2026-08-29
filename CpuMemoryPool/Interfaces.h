#pragma once
#include "MemoryPage.h"

class IMemoryBlockHanlde
{
public:
	IMemoryBlockHanlde() {};
	virtual ~IMemoryBlockHanlde() {};

	virtual void SetMemoryHandler(const MemoryBlock& handler) { _memoryHandler = handler; };
	MemoryBlock& GetMemoryHandler() { return _memoryHandler; }

protected:
	MemoryBlock _memoryHandler;
};