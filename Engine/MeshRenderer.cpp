#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "GlobalConstantBuffer.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer() : Renderer(eComponentType::Renderer)
{

}
MeshRenderer::~MeshRenderer()
{
	if (_mesh)
	{
		RESOURCES->ReleaseMesh(_mesh->GetMeshInfo());
	}
	if (_texture)
	{
		RESOURCES->ReleaseTexture(_texture->GetTextureInfo());
	}
}

void MeshRenderer::Init(MemoryBlock meshHandler, shared_ptr<Shader> shader)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(meshHandler._poolID)->GetObjectByMemoryBlock(meshHandler, &_mesh);
	assert(isSuccess);

	_shader = shader;
	_texture = nullptr;
}

void MeshRenderer::Init(MemoryBlock meshHandler, shared_ptr<Shader> shader, MemoryBlock & texture)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(meshHandler._poolID)->GetObjectByMemoryBlock(meshHandler, &_mesh);
	assert(isSuccess);
	_shader = shader;

	isSuccess = CPU_MEM_POOL->GetMemoryPool(texture._poolID)->GetObjectByMemoryBlock(texture, &_texture);
	assert(isSuccess);
}

void MeshRenderer::Render()
{
	Renderer::Render();

	COMMAND_LIST->SetGraphicsRootSignature(_shader->GetRootSignature().Get());
	COMMAND_LIST->SetPipelineState(_shader->GetPSO().Get());
	COMMAND_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Vertex 버퍼 전달
	D3D12_VERTEX_BUFFER_VIEW vertexView = _mesh->GetVertexView();
	COMMAND_LIST->IASetVertexBuffers(0, 1, &vertexView);

	// Index 버퍼 전달
	D3D12_INDEX_BUFFER_VIEW indexView = _mesh->GetIndexView();
	COMMAND_LIST->IASetIndexBuffer(&indexView);


	// Global 버퍼 전달
	// Push 이후에 GetAddress 호출
	COMMAND_LIST->SetGraphicsRootConstantBufferView(0, GlobalConstantBuffer::GetInstance()->GetCameraBufferAddress());

	// World Matrix 버퍼 전달
	// Push 이후에 GetAddress 호출
	D3D12_GPU_VIRTUAL_ADDRESS worldMaterialBufferAddress;
	PushWorldMatrixBuffer(worldMaterialBufferAddress);
	COMMAND_LIST->SetGraphicsRootConstantBufferView(1, worldMaterialBufferAddress);

	// Texture 전달
	if (_texture != nullptr)
	{
		COMMAND_LIST->SetGraphicsRootDescriptorTable(2, _texture->GetHandle());
	}

	COMMAND_LIST->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);
}
