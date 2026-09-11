#pragma once
class GpuMemoryPoolManager
{
	DECLARE_SINGLE(GpuMemoryPoolManager);

public:
	enum class ePoolID : UINT8
	{
		NONE = 0,

		BUMP = 1 << 0,
		DYNAMIC = 1 << 1,

		BUMP_ONLY_64KB = 1 << 2,
		DYNAMIC_ONLY_64 = 1 << 3,
		
		MAX
	};
	struct sMemoryPoolManagerInfo
	{
		ePoolID initPoolFlag;
		UINT64 bumbMaxSize;
		UINT64 dynamicMaxSize;
		UINT64 bumbOnly64KBMaxSize;
		UINT64 dynamicOnly64KBMaxSize;
	};

public:
	void Init(sMemoryPoolManagerInfo info);
	void Release();

	bool GetMemory(ePoolID poolID, eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle);
	bool ReleaseMemory(const GpuMemoryHandle& handle);

	void ResetAllMemporyPool();

	const ComPtr<ID3D12Heap>& GetMemoryHeap(ePoolID poolID);

private:
	ePoolID _usingPoolIDs;

	GpuBumpMemoryPool* _bumpPool = nullptr;
	GpuDynamicMemoryPool* _dynamicPool = nullptr;

	GpuBumpMemoryPool* _bumpPoolOnly64KB = nullptr;
	GpuDynamicMemoryPool* _dynamicPoolOnly64KB = nullptr;
};

inline GpuMemoryPoolManager::ePoolID operator&(GpuMemoryPoolManager::ePoolID lhs, GpuMemoryPoolManager::ePoolID rhs)
{
	return static_cast<GpuMemoryPoolManager::ePoolID>(static_cast<UINT8>(lhs) & static_cast<UINT8>(rhs));
}

inline GpuMemoryPoolManager::ePoolID operator|(GpuMemoryPoolManager::ePoolID lhs, GpuMemoryPoolManager::ePoolID rhs)
{
	return static_cast<GpuMemoryPoolManager::ePoolID>(static_cast<UINT8>(lhs) | static_cast<UINT8>(rhs));
}

inline bool HasFlag(GpuMemoryPoolManager::ePoolID bitmask, GpuMemoryPoolManager::ePoolID flag)
{
	if (flag == GpuMemoryPoolManager::ePoolID::BUMP)
	{
		return bitmask == GpuMemoryPoolManager::ePoolID::BUMP;
	}

	return (bitmask & flag) == flag;
}