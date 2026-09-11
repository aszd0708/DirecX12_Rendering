#pragma once
#include "CpuMemoryPool.h"

class CpuMemoryPoolManager
{
	DECLARE_SINGLE(CpuMemoryPoolManager);

public:
	enum class ePoolID : UINT8
	{
		CPU_64 = 0,
		CPU_128,
		CPU_256,
		CPU_512,
	
		RENDERER,
		MAX,
	};
	
public:
	void Init();
	void Release();
	
	bool GetPoolID(UINT size, OUT UINT8& poolID);
	CpuMemoryPool* GetMemoryPool(UINT8& poolID);
	
	template<typename T>
	bool Resolve(MemoryEntry& entry, OUT T** obj);

private:
	CpuMemoryPool** _memoryPools;
};

template<typename T>
inline bool CpuMemoryPoolManager::Resolve(MemoryEntry& entry, OUT T** obj)
{
	CpuMemoryPool* pool = CpuMemoryPoolManager::GetInstance()->GetMemoryPool(entry.block._poolID);
	
	// isSuccess 는 Generation 값이 다를 경우
	bool isSuccess = pool->GetObjectByMemoryBlock<T>(entry.block, obj);
	return isSuccess;
}
