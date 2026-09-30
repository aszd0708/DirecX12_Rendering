#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "GlobalConstantBuffer.h"
#include "Mesh.h"
#include "ShaderInfo.h"
#include "InstancingController.h"

MeshRenderer::MeshRenderer() : Renderer(eComponentType::Renderer)
{

}
MeshRenderer::~MeshRenderer()
{
	if (_mesh)
	{
		RESOURCES->ReleaseMesh(_mesh->GetMeshInfo());
	}
	if (_shader)
	{
		RESOURCES->ReleaseShader(_shader->GetShaderInfo());
	}
	if (_texture)
	{
		RESOURCES->ReleaseTexture(_texture->GetTextureInfo());
	}
}

void MeshRenderer::Init(MemoryBlock meshHandler, MemoryBlock shaderHandler)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(meshHandler._poolID)->GetObjectByMemoryBlock(meshHandler, &_mesh);
	assert(isSuccess);

	isSuccess = CPU_MEM_POOL->GetMemoryPool(shaderHandler._poolID)->GetObjectByMemoryBlock(shaderHandler, &_shader);
	assert(isSuccess);

	_texture = nullptr;
}

void MeshRenderer::Init(MemoryBlock meshHandler, MemoryBlock shaderHandler, MemoryBlock texture)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(meshHandler._poolID)->GetObjectByMemoryBlock(meshHandler, &_mesh);
	assert(isSuccess);

	isSuccess = CPU_MEM_POOL->GetMemoryPool(shaderHandler._poolID)->GetObjectByMemoryBlock(shaderHandler, &_shader);
	assert(isSuccess);

	isSuccess = CPU_MEM_POOL->GetMemoryPool(texture._poolID)->GetObjectByMemoryBlock(texture, &_texture);
	assert(isSuccess);
}

void MeshRenderer::Render(ID3D12GraphicsCommandList* commandList)
{
	Renderer::Render(commandList);

	commandList->SetGraphicsRootSignature(GRAPHICS->GetRootSignature().Get());
	commandList->SetPipelineState(_shader->GetPSO().Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Vertex 버퍼 전달
	D3D12_VERTEX_BUFFER_VIEW vertexView = _mesh->GetVertexView();
	commandList->IASetVertexBuffers(0, 1, &vertexView);

	// Index 버퍼 전달
	D3D12_INDEX_BUFFER_VIEW indexView = _mesh->GetIndexView();
	commandList->IASetIndexBuffer(&indexView);


	// Global 버퍼 전달
	// Push 이후에 GetAddress 호출
	commandList->SetGraphicsRootConstantBufferView((int)eShaderIndex::GLOBAL, GlobalConstantBuffer::GetInstance()->GetCameraBufferAddress());

	// World Matrix 버퍼 전달
	// Push 이후에 GetAddress 호출
	D3D12_GPU_VIRTUAL_ADDRESS worldMaterialBufferAddress;
	PushWorldMatrixBuffer(worldMaterialBufferAddress);
	commandList->SetGraphicsRootConstantBufferView((int)eShaderIndex::TRANSFORM, worldMaterialBufferAddress);

	// Texture 전달
	if (_texture != nullptr)
	{
		commandList->SetGraphicsRoot32BitConstant((int)eShaderIndex::TEXTURE_INDEX, _texture->GetDescHandle().index, 0);
	}

	commandList->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);
}

void MeshRenderer::RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count)
{
	Renderer::RenderInstancing(commandList, address, count);

	commandList->SetPipelineState(_shader->GetPSO().Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Vertex 버퍼 전달
	D3D12_VERTEX_BUFFER_VIEW vertexView = _mesh->GetVertexView();
	commandList->IASetVertexBuffers(0, 1, &vertexView);

	// Index 버퍼 전달
	D3D12_INDEX_BUFFER_VIEW indexView = _mesh->GetIndexView();
	commandList->IASetIndexBuffer(&indexView);

	// 인스턴싱 세팅
	commandList->SetGraphicsRootShaderResourceView((UINT)eShaderIndex::INSTANCE, address);

	commandList->DrawIndexedInstanced(_mesh->GetIndexCount(), count, 0, 0, 0);
}

bool MeshRenderer::SetInstancingInfo(InstancingInfo& info)
{
	info.meshID = _mesh->GetInstancingID();
	info.shaderID = _shader->GetInstancingID();
	info.renderMemBlock = _memoryHandler;

	Transform* transform;
	bool isSuccess = GetTransform(&transform);
	if (isSuccess == false) return false;

	Matrix world = transform->GetWorldMatrix();
	_instancingDesc.W = world.Transpose();
	_instancingDesc.texIndex = _texture->GetDescHandle().index;
	info.instanceDesc = _instancingDesc;

	return true;
}
