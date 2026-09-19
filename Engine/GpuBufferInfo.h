#pragma once

enum class eGpuBufferKind : UINT8
{
	Vertex = 0,
	Index,
};

enum class eIndexBufferSize : UINT8
{
	UINT_16 = 2,
	UINT_32 = 4,
	MAX,
};

struct GpuBufferHandle
{
	// Bumb, Dynamic 공용
	UINT8 poolID;
	UINT8 pageIndex;

	// Dynamic Only
	UINT16 gen;
	UINT32 arrayIndex;


	// Bumb, Dynamic 공용
	UINT64 size;
	UINT64 offset;
};