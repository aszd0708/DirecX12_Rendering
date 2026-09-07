#pragma once

struct GpuPoolMemoryHandle
{
	UINT8 gen;
	UINT64 size;
	UINT32 index;
};

class IGpuMemoryPoolInterface
{
public:
	virtual bool Alloc(UINT32 size, UINT32 index, OUT BYTE* result) = 0;
	virtual void Free(const GpuPoolMemoryHandle& handle) = 0;
	virtual void Reset() = 0;
	virtual void Release() = 0;
};