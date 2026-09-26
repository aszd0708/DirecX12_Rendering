#include "pch.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "GlobalBuffer.h"

CpuMemoryPoolManager::ePoolID Renderer::s_PoolID = CpuMemoryPoolManager::ePoolID::RENDERER;

Renderer::Renderer(eComponentType type) : Component(type)
{
	_worldTransformBuffer = new ConstantBuffer(sizeof(TransformDesc));
}

Renderer::~Renderer()
{
	delete _worldTransformBuffer;
}

void Renderer::SetMemoryHandler(const MemoryBlock& handler)
{
	IMemoryBlockHanlde::SetMemoryHandler(handler);

	_memoryEntry.block = handler;
	_memoryEntry.type = typeid(Renderer);
}

void Renderer::Init()
{

}

void Renderer::Render()
{

}

void Renderer::PushWorldMatrixBuffer(OUT D3D12_GPU_VIRTUAL_ADDRESS& address)
{
	if(_worldTransformBuffer == nullptr) return;

	Transform* transform;
	bool isSuccess = GetTransform(&transform);
	if(isSuccess == false) return;

	Matrix world = transform->GetWorldMatrix();
	_worldTransformDesc.W = world.Transpose();

	_worldTransformBuffer->PushDataSafe<TransformDesc>(_worldTransformDesc, address);
}
