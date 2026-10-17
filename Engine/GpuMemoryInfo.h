#pragma once

enum class eGpuMemoryPoolType : UINT8
{
	SIZE_64KB = 0,
	SIZE_4MB = 1,

	MAX
};

enum class eGpuSlotCondition : UINT8
{
	FREE,		// 스택에 있음, 누구나 사용 가능.
	USED,		// Material이 사용중
	PENDING,	// 반납 요청 됨, GPU가 아직 쓰고 있을 수 있음.
};

struct GpuMemoryHandle
{
	// Dynamic
	UINT16 gen;
	UINT8 memoryPoolID;

	// Bumb, Dynamic 공용
	UINT8 pageIndex;
	eGpuMemoryPoolType poolType;

	// Dynamic
	UINT32 arrayIndex;

	// Bumb, Dynamic 공용
	UINT64 size;
	UINT64 offset;
};

/// <summary>
/// INT64를 여기서 사용하는 UINT8로 변환 함수
/// </summary>
/// <param name="size">D3D12_RESOURCE_ALLOCATION_INFO.Alignment 을 넣음</param>
/// <returns></returns>
static eGpuMemoryPoolType GetMemoryPoolType(UINT64 size)
{
	switch (size)
	{
		case D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT:
		return eGpuMemoryPoolType::SIZE_64KB;
		break;
		case D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT:
		return eGpuMemoryPoolType::SIZE_4MB;
		break;
	}
	return eGpuMemoryPoolType::MAX;
}