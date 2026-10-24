#include "pch.h"
#include "ModelRenderer.h"
#include "GlobalConstantBuffer.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"


ModelRenderer::ModelRenderer() : Renderer(eComponentType::Renderer), _instancingDesces(0), _model(nullptr)
{

}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::Init(MemoryBlock& modelHandler)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(modelHandler._poolID)->GetObjectByMemoryBlock(modelHandler, &_model);
	assert(isSuccess);

	_instancingDesces.SetCount(_model->GetSubMeshes().GetCount());
}

void ModelRenderer::Render(ID3D12GraphicsCommandList * commandList)
{
	Renderer::Render(commandList);

	// World Matrix 버퍼 전달
	// Push 이후에 GetAddress 호출

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const Array<Material*>& materials = _model->GetMaterials();
	const Array<SubMesh>& subMeshes = _model->GetSubMeshes();
	const Array<BoneData>& bones = _model->GetBones();
	for (UINT32 i = 0; i < subMeshes.GetCount(); ++i)
	{
		SubMesh subMesh = subMeshes[i];
		Mesh* mesh = subMesh.mesh;
		// POS 전달
		Material* material = materials[subMesh.materialIndex];
		commandList->SetPipelineState(material->GetShader()->GetPSO().Get());

		// Vertex 버퍼 전달
		D3D12_VERTEX_BUFFER_VIEW vertexView = mesh->GetVertexView();
		commandList->IASetVertexBuffers(0, 1, &vertexView);

		// Index 버퍼 전달
		D3D12_INDEX_BUFFER_VIEW indexView = mesh->GetIndexView();
		commandList->IASetIndexBuffer(&indexView);

		// Bone Root Matrix 전달
		D3D12_GPU_VIRTUAL_ADDRESS meshTransformAddress = 0;
		const BoneData& boneData = bones[subMesh.boneIndex];
		PushTransformMatrixBuffer(meshTransformAddress, boneData.rootMatrix);
		commandList->SetGraphicsRootConstantBufferView((int)eShaderIndex::TRANSFORM, meshTransformAddress);

		UINT8 count = 0;
		const UINT32* textureIndexes = material->GetTextureIndexes(count);
		for (UINT8 i = 0; i < count; ++i)
		{
			UINT32 index = textureIndexes[i];
			if (textureIndexes[i] == UINT32_MAX)
			{
				continue;
			}
			commandList->SetGraphicsRoot32BitConstant((int)eShaderIndex::TEXTURE_INDEX, textureIndexes[i], 0);
		}

		commandList->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
	}
}

void ModelRenderer::RenderInstancing(ID3D12GraphicsCommandList * commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count)
{
	Renderer::RenderInstancing(commandList, address, count);


}

bool ModelRenderer::SetInstancingInfo(InstancingInfo & info)
{
	return false;
}
