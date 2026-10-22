#include "pch.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshInfo.h"
#include "Material.h"
#include "MaterialInfo.h"

Model::Model()
{
}

Model::~Model()
{
	for (UINT32 i = 0; i < _subMeshes.GetCount(); ++i)
	{
		if (_subMeshes[i].mesh != nullptr)
		{
			RESOURCES->ReleaseMesh(_subMeshes[i].mesh->GetMeshInfo());
		}
	}

	for (UINT32 i = 0; i < _materials.GetCount(); ++i)
	{
		if (_materials[i] != nullptr)
		{
			RESOURCES->ReleaseMaterial(_materials[i]->GetMaterialInfoHash());
		}
	}
}

bool Model::SetModelData(ModelData&& data)
{
	CpuMemoryPoolManager* cpuMemPool = CPU_MEM_POOL;
	ResourceManager* resourcePool = RESOURCES;

	// Mesh 세팅
	UINT32 meshCount = data.meshData.GetCount();
	_subMeshes.SetCount(meshCount);
	for (UINT32 i = 0; i < meshCount; ++i)
	{
		MemoryBlock memoryBlock = {};
		MeshInfo& meshInfo = data.meshData[i].meshInfo;
		bool isSuccess = resourcePool->GetMesh(meshInfo, GpuBufferPoolManager::eBufferPoolID::VERTEX_BUMP, GpuBufferPoolManager::eBufferPoolID::INDEX_BUMP, memoryBlock);
		if (isSuccess == false)
		{
			return false;
		}

		Mesh* mesh = nullptr;
		isSuccess = cpuMemPool->GetMemoryPool(memoryBlock._poolID)->GetObjectByMemoryBlock(memoryBlock, &mesh);
		if (isSuccess == false)
		{
			return false;
		}
		_subMeshes[i] = {};
		_subMeshes[i].mesh = mesh;
		_subMeshes[i].boneIndex = data.meshData[i].boneIndex;
		_subMeshes[i].materialIndex = data.meshData[i].materialIndex;
	}
	
	UINT32 materialCount = data.materials.GetCount();
	_materials.SetCount(materialCount);
	for (UINT32 i = 0; i < materialCount; ++i)
	{
		MemoryBlock memoryBlock = {};

		// 추후 MaterialInfo 별로 분리
		bool isContainDefaultMaterialInfo = std::holds_alternative<DefaultMaterialInfo>(data.materials[i]);
		const DefaultMaterialInfo* info = std::get_if<DefaultMaterialInfo>(&data.materials[i]);

		bool isSuccess = info != nullptr;
		if (isSuccess == false)
		{
			return isSuccess;
		}

		isSuccess = resourcePool->GetMaterial(*info, memoryBlock);
		if (isSuccess == false)
		{
			return isSuccess;
		}

		Material* material = nullptr;
		isSuccess = cpuMemPool->GetMemoryPool(memoryBlock._poolID)->GetObjectByMemoryBlock(memoryBlock, &material);
		if (isSuccess == false)
		{
			return false;
		}
		_materials[i] = material;
	}

	UINT32 boneCount = data.boneData.GetCount();
	_bones = move(data.boneData);

	return true;
}
