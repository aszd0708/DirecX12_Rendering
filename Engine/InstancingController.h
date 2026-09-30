#pragma once

struct InstancingInfo
{
	UINT32 meshID;
	UINT32 shaderID;
	MemoryBlock renderMemBlock;
	InstanceDesc instanceDesc;
};

class InstanceGroup
{
public:
	InstanceGroup(const MemoryBlock& memBlock);
	~InstanceGroup();

	void AddInstanceDesc(const InstanceDesc& instanceDesc);

	void SetRenderMemoryBlock(const MemoryBlock& memoryBlock) { _renderMemBlock = memoryBlock; }
	const MemoryBlock& GetRenderMemoryBlock() { return _renderMemBlock; }
	const InstanceDesc* GetDesces();
	UINT32 GetDescCount(){ return _instanceDesces.GetCount(); }

	void Clear();

private:
	MemoryBlock _renderMemBlock;
	DynamicArray<InstanceDesc> _instanceDesces;
};

/// <summary>
/// Scene에 하나씩 대응
/// </summary>
class InstancingController
{
public:
	InstancingController();
	~InstancingController();

public:
	void Add(const InstancingInfo& info);
	void Clear();
	void Render(GpuCommandInfo* commandInfo);

private:
	UINT64 GetKey(UINT32 meshID, UINT32 shaderID);

private:
	HashTable<UINT64, InstanceGroup*> _instanceTable;
	DynamicArray<InstanceGroup*> _instanceArray;
};

