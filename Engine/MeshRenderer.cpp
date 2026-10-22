#include "pch.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "GlobalConstantBuffer.h"
#include "Mesh.h"
#include "ShaderInfo.h"
#include "InstancingController.h"
#include "Material.h"
#include "MaterialInfo.h"

MeshRenderer::MeshRenderer() : Renderer(eComponentType::Renderer), _instancingDesc(1)
{
	
}
MeshRenderer::~MeshRenderer()
{
	if (_mesh)
	{
		RESOURCES->ReleaseMesh(_mesh->GetMeshInfo());
	}
	if (_material)
	{
		RESOURCES->ReleaseMaterial(_material->GetMaterialInfoHash());
	}
}

void MeshRenderer::Init(MemoryBlock meshHandler, MemoryBlock materialHandler)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(meshHandler._poolID)->GetObjectByMemoryBlock(meshHandler, &_mesh);
	assert(isSuccess);

	isSuccess = CPU_MEM_POOL->GetMemoryPool(materialHandler._poolID)->GetObjectByMemoryBlock(materialHandler, &_material);
	assert(isSuccess);

	_textureIndexes = _material->GetTextureIndexes(_textureCount);
}

void MeshRenderer::Render(ID3D12GraphicsCommandList* commandList)
{
	Renderer::Render(commandList);

	commandList->SetGraphicsRootSignature(GRAPHICS->GetRootSignature().Get());
	commandList->SetPipelineState(_material->GetShader()->GetPSO().Get());
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
	if (_textureIndexes != nullptr)
	{
		for (int i = 0; i < _textureCount; ++i)
		{
			if (_textureIndexes[i] == UINT32_MAX)
			{
				continue;
			}
			commandList->SetGraphicsRoot32BitConstant((int)eShaderIndex::TEXTURE_INDEX, _textureIndexes[i], 0);
		}
	}

	commandList->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);
}

void MeshRenderer::RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count)
{
	Renderer::RenderInstancing(commandList, address, count);

	commandList->SetPipelineState(_material->GetShader()->GetPSO().Get());
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
	info.shaderID = _material->GetShader()->GetInstancingID();
	info.renderMemBlock = _memoryHandler;

	Transform* transform;
	bool isSuccess = GetTransform(&transform);
	if (isSuccess == false) return false;

	Matrix world = transform->GetWorldMatrix();
	_instancingDesc[0].W = world.Transpose();
	_instancingDesc[0].materialIndex = _material->GetSlotIndex();
	info.instanceDesc = _instancingDesc[0];

	return true;
}
