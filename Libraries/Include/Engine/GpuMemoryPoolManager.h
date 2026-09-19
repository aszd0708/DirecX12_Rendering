#pragma once
class GpuMemoryPoolManager
{
	DECLARE_SINGLE(GpuMemoryPoolManager);

	// Memory Pool
public:
	enum class eMemoryPoolID : UINT8
	{
		NONE = 0,

		BUMP = 1 << 0,
		DYNAMIC = 1 << 1,

		BUMP_ONLY_64KB = 1 << 2,
		DYNAMIC_ONLY_64 = 1 << 3,

		DYNAMIC_UPLOAD = 1 << 4,
		
		MAX
	};
	struct sMemoryPoolManagerInfo
	{
		eMemoryPoolID initPoolFlag;
		UINT64 bumbMaxSize;
		UINT64 dynamicMaxSize;
		UINT64 bumbOnly64KBMaxSize;
		UINT64 dynamicOnly64KBMaxSize;
		UINT64 dynamicUploadMaxSize;
	};

public:
	void Init(sMemoryPoolManagerInfo info);
	void Release();

	bool GetMemory(eMemoryPoolID poolID, eGpuMemoryPoolType type, UINT64 size, OUT GpuMemoryHandle& handle);
	bool ReleaseMemory(const GpuMemoryHandle& handle);

	void ResetAllMemporyPool();

	const ComPtr<ID3D12Heap>& GetMemoryHeap(eMemoryPoolID poolID);

private:
	eMemoryPoolID _usingMemoryPoolIDs;

	GpuBumpMemoryPool* _bumpPool = nullptr;
	GpuDynamicMemoryPool* _dynamicPool = nullptr;

	GpuBumpMemoryPool* _bumpPoolOnly64KB = nullptr;
	GpuDynamicMemoryPool* _dynamicPoolOnly64KB = nullptr;

	GpuDynamicMemoryPool* _dyanamicPoolUpload = nullptr;
};

inline GpuMemoryPoolManager::eMemoryPoolID operator&(GpuMemoryPoolManager::eMemoryPoolID lhs, GpuMemoryPoolManager::eMemoryPoolID rhs)
{
	return static_cast<GpuMemoryPoolManager::eMemoryPoolID>(static_cast<UINT8>(lhs) & static_cast<UINT8>(rhs));
}

inline GpuMemoryPoolManager::eMemoryPoolID operator|(GpuMemoryPoolManager::eMemoryPoolID lhs, GpuMemoryPoolManager::eMemoryPoolID rhs)
{
	return static_cast<GpuMemoryPoolManager::eMemoryPoolID>(static_cast<UINT8>(lhs) | static_cast<UINT8>(rhs));
}

inline bool HasFlag(GpuMemoryPoolManager::eMemoryPoolID bitmask, GpuMemoryPoolManager::eMemoryPoolID flag)
{
	if (flag == GpuMemoryPoolManager::eMemoryPoolID::BUMP)
	{
		return bitmask == GpuMemoryPoolManager::eMemoryPoolID::BUMP;
	}

	return (bitmask & flag) == flag;
}