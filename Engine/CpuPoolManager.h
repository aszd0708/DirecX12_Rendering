#pragma once
#include "CpuMemoryPool.h"

class CpuPoolManager
{
	DECLARE_SINGLE(CpuPoolManager);

public:
	enum ePoolID : UINT8
	{
		CPU_64 = 0,
		CPU_128,
		CPU_256,
	
		RENDERER,
		MAX,
	};
	
public:
	void Init();
	void Release();
	
	bool GetPoolID(UINT size, OUT UINT8& poolID);
	CpuMemoryPool* GetMemoryPool(UINT8& poolID);

private:
	CpuMemoryPool** _memoryPools;
};
