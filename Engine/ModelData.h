#pragma once
#include "MeshInfo.h"
#include "MaterialInfo.h"

using MaterialInfoVariant = std::variant<DefaultMaterialInfo>;

struct SubMeshData
{
	MeshInfo meshInfo;
	INT32 materialIndex = INVALID_INDEX;
	INT32 boneIndex = INVALID_INDEX;

	// 가중치가 있는 정점이 하나라도 있으면 true
	// true : 정점마다 팔레트로 본을 따라감 -> W는 World만
	// false : 메시 전체가 boneIndex 본을 따라감 (고정 부착)
	bool isSkinned = false;
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