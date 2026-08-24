#include "pch.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "GlobalBuffer.h"

Renderer::Renderer(eComponentType type) : Component(type)
{

}

void Renderer::PushGlobalBuffer(const Matrix& view, const Matrix& projection)
{
	if (_globalBuffer == nullptr) return;

	_globalDesc.V = view;
	_globalDesc.P = projection;
	_globalDesc.VP = view * projection;
	_globalDesc.VInv = view.Invert();

	_globalBuffer->PushDataSafe<GlobalDesc>(_globalDesc);
}

void Renderer::PushWorldMatrixBuffer()
{
	if(_worldTransformBuffer == nullptr) return;

	Matrix world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
	_worldTransformDesc.W = world;

	_worldTransformBuffer->PushDataSafe<TransformDesc>(_worldTransformDesc);
}
