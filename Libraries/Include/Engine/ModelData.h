#pragma once
#include "MeshInfo.h"
#include "MaterialInfo.h"

using MaterialInfoVariant = std::variant<DefaultMaterialInfo>;

struct SubMeshData
{
	MeshInfo meshInfo;
	INT32 materialIndex = INVALID_INDEX;
	INT32 boneIndex = INVALID_INDEX;
};

struct BoneData
{
	// bone → 모델 루트 공간
	Matrix rootMatrix;
	INT32 parentBoneIndex = INVALID_INDEX;
	string name;
};

struct ModelData
{
	string name;

	Array<SubMeshData> meshData;
	Array<BoneData> boneData;
	Array<MaterialInfoVariant> materials;
};

