#include "pch.h"
#include "ModelRenderer.h"
#include "GameObject.h"
#include "GlobalConstantBuffer.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "InstancingController.h"
#include "Animator.h"


ModelRenderer::ModelRenderer() : Renderer(eComponentType::Renderer), _instancingDesces(0), _model(nullptr)
{

}

ModelRenderer::~ModelRenderer()
{
	if (_model != nullptr)
	{
		RESOURCES->ReleaseModel(_model->GetModelInfo());
	}
}

void ModelRenderer::Init(MemoryBlock& modelHandler)
{
	SetModel(modelHandler);
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
		// PSO 전달
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
		Matrix matrix = Matrix::Identity;
		if (subMesh.boneIndex != INVALID_INDEX)
		{
			const BoneData& boneData = bones[subMesh.boneIndex];
			matrix = boneData.rootMatrix;
		}
		PushTransformMatrixBuffer(meshTransformAddress, matrix);
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

bool ModelRenderer::SetInstancingInfo(InstancingController* controller)
{
	// Animation
	GameObject* gameObject = nullptr;
	bool isSuccess = GetGameObject(&gameObject);
	if (isSuccess == false)
	{
		return false;
	}

	Transform* transform;
	isSuccess = GetTransform(&transform);
	if (isSuccess == false) return false;

	// Animator 먼저
	UINT32 animatorStartIdx = 0;

	Matrix world = transform->GetWorldMatrix();
	Animator* animator = gameObject->GetComponent<Animator>();
	bool existAnimator = animator != nullptr;
	if (existAnimator)
	{
		const Array<Matrix>& palet = animator->GetPaletMatrixArray();
		animatorStartIdx = controller->AddAnimationPaletMatrix(palet);
	}

	UINT32 meshCount = _model->GetSubMeshes().GetCount();
	const Array<SubMesh>& meshes = _model->GetSubMeshes();
	const Array<Material*>& materials = _model->GetMaterials();
	const Array<BoneData>& boneData = _model->GetBones();
	for (UINT32 i = 0; i < meshCount; ++i)
	{
		InstancingInfo info = {};

		_instancingDesces[i].paletStartIndex = animatorStartIdx;

		INT32 materialIdx = meshes[i].materialIndex;
		if (materialIdx == INVALID_INDEX)
		{
			info.shader = RESOURCES->GetErrorShader();
			_instancingDesces[i].materialIndex = RESOURCES->GetErrorMaterialSlotIdx();
		}
		else
		{
			Material* material = materials[materialIdx];
			_instancingDesces[i].materialIndex = material->GetSlotIndex();
			info.shader = material->GetShader();
		}

		INT32 boneIdx = meshes[i].boneIndex;
		Matrix boneMatrix = {};
		if (boneIdx == INVALID_INDEX)
		{
			boneMatrix = Matrix::Identity;
		}
		else
		{
			boneMatrix = boneData[boneIdx].rootMatrix;
		}
		Matrix resultMatrix = {};
		if (meshes[i].isSkinned)
		{
			resultMatrix = world;
		}
		else
		{
			if (existAnimator)
			{
				if (boneIdx != INVALID_INDEX)
				{
					boneMatrix = animator->GetGlobalMatrixArray()[boneIdx];
				}
			}
			resultMatrix = boneMatrix * world;
		}
		_instancingDesces[i].W = resultMatrix.Transpose();

		info.instanceDesc = _instancingDesces[i];
		info.mesh = meshes[i].mesh;

		controller->Add(info);
	}

	return true;
}

void ModelRenderer::SetModel(MemoryBlock& modelHandler)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(modelHandler._poolID)->GetObjectByMemoryBlock(modelHandler, &_model);
	assert(isSuccess);

	_instancingDesces.SetCount(_model->GetSubMeshes().GetCount());

	GameObject* obj = nullptr;
	isSuccess = GetGameObject(&obj);
	assert(isSuccess);

	Animator* animator = obj->GetComponent<Animator>();
	if (animator != nullptr)
	{
		animator->SetModel(modelHandler);
	}
}
