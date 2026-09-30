#pragma once
#include "Component.h"
#include "GlobalBuffer.h"
#include "CpuMemoryPoolManager.h"

struct InstancingInfo;
class ConstantBuffer;

class Renderer : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	static CpuMemoryPoolManager::ePoolID s_PoolID;

public:
	Renderer(eComponentType type);
	virtual ~Renderer() override;

	virtual void SetMemoryHandler(const MemoryBlock& handler) override;

	MemoryEntry& GetMemoryEntry() { return _memoryEntry; }

public:
	virtual void Init();
	virtual void Render(ID3D12GraphicsCommandList* commandList);
	virtual void RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count);
	virtual bool SetInstancingInfo(InstancingInfo& info) = 0;

public:
	void SetWorldMatrixBuffer(ConstantBuffer* worldMatrixBuffer) { _worldTransformBuffer = worldMatrixBuffer; };
	ConstantBuffer* GetWorldMatrixBuffer() { return _worldTransformBuffer; }

protected:
	void PushWorldMatrixBuffer(OUT D3D12_GPU_VIRTUAL_ADDRESS& address);

protected:
	InstanceDesc _instancingDesc;
private:
	TransformDesc _worldTransformDesc;
	ConstantBuffer* _worldTransformBuffer;

	MemoryEntry _memoryEntry;
};

