#pragma once
#include "Component.h"
#include "GlobalBuffer.h"

class ConstantBuffer;

class Renderer : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	Renderer(eComponentType type);
	virtual ~Renderer() override;

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
	
};

