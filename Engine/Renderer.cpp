#include "pch.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "GlobalBuffer.h"

CpuPoolManager::ePoolID Renderer::s_PoolID = CpuPoolManager::ePoolID::RENDERER;

Renderer::Renderer(eComponentType type) : Component(type)
{
	_globalBuffer = make_shared<ConstantBuffer>(sizeof(GlobalDesc));
	_worldTransformBuffer = make_shared<ConstantBuffer>(sizeof(TransformDesc));
}

Renderer::~Renderer()
{

}

void Renderer::Init()
{

}

void Renderer::Render()
{

}

void Renderer::PushGlobalBuffer(const Matrix& view, const Matrix& projection)
{
	if (_globalBuffer == nullptr) return;

	_globalDesc.V = view.Transpose();
	_globalDesc.P = projection.Transpose();
	_globalDesc.VP = (view * projection).Transpose();;
	_globalDesc.VInv = view.Invert().Transpose();

	_globalBuffer->PushDataSafe<GlobalDesc>(_globalDesc);
}

void Renderer::PushWorldMatrixBuffer()
{
	if(_worldTransformBuffer == nullptr) return;

	Matrix world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
	_worldTransformDesc.W = world.Transpose();

	_worldTransformBuffer->PushDataSafe<TransformDesc>(_worldTransformDesc);
}
