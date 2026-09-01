#pragma once
#include "Component.h"
#include "GlobalBuffer.h"
#include "CpuPoolManager.h"

class ConstantBuffer;

class Renderer : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	static CpuPoolManager::ePoolID s_PoolID;

public:
	Renderer(eComponentType type);
	virtual ~Renderer() override;

	virtual void SetMemoryHandler(const MemoryBlock& handler) override;

	MemoryEntry& GetMemoryEntry() { return _memoryEntry; }

public:
	virtual void Init();
	virtual void Render();

public:
	void SetGlobalBuffer(shared_ptr<ConstantBuffer> globalBuffer) { _globalBuffer = globalBuffer; };
	const shared_ptr<ConstantBuffer> GetGlobalBuffer() { return _globalBuffer; }
	void SetWorldMatrixBuffer(shared_ptr<ConstantBuffer> worldMatrixBuffer) { _worldTransformBuffer = worldMatrixBuffer; };
	const shared_ptr<ConstantBuffer> GetWorldMatrixBuffer() { return _worldTransformBuffer; }

protected:
	void PushGlobalBuffer(const Matrix& view, const Matrix& projection);
	void PushWorldMatrixBuffer();

private:
	GlobalDesc _globalDesc;
	shared_ptr<ConstantBuffer> _globalBuffer;

	TransformDesc _worldTransformDesc;
	shared_ptr<ConstantBuffer> _worldTransformBuffer;

	MemoryEntry _memoryEntry;
};

