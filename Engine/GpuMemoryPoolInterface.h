#pragma once

// Bump에서 3바이트가 남아서 그냥 두개 같이 (나 혼자) 합의
struct GpuMemoryHandle
{
	// Dynamic
	UINT8 gen;
	UINT8 memoryPoolID;

	// Bumb, Dynamic 공용
	UINT8 pageIndex;

	// Dynamic
	UINT32 arrayIndex;

	// Bumb, Dynamic 공용
	UINT64 size;
	UINT64 offset;
};


enum class eMemoryPoolType : UINT8
{
	SIZE_64KB = 0,
	SIZE_4MB = 1,

	MAX
};