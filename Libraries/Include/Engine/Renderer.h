#pragma once
#include "Component.h"
#include "GlobalBuffer.h"
#include "CpuMemoryPoolManager.h"

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
	virtual void Render();

public:
	void SetWorldMatrixBuffer(ConstantBuffer* worldMatrixBuffer) { _worldTransformBuffer = worldMatrixBuffer; };
	ConstantBuffer* GetWorldMatrixBuffer() { return _worldTransformBuffer; }

protected:
	void PushWorldMatrixBuffer(OUT D3D12_GPU_VIRTUAL_ADDRESS& address);

private:
	TransformDesc _worldTransformDesc;
	ConstantBuffer* _worldTransformBuffer;

	MemoryEntry _memoryEntry;
};

