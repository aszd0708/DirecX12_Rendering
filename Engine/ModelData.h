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

// Converter asKeyframeData와 같은 바이트 구조여야 함 (트랙 단위로 통째로 읽음)
struct KeyframeData
{
	// 키가 적은 트랙은 마지막 키를 복제한 값이라 신뢰 불가 -> 프레임 인덱스로 샘플링할 것
	float time = 0.f;
	Vec3 scale = { 1, 1, 1 };
	Quaternion rotation = Quaternion::Identity;
	Vec3 translation = { 0, 0, 0 };
};

struct AnimationTrack
{
	string boneName;
	// count == AnimationData::frameCount
	Array<KeyframeData> keyframes;
};

struct AnimationData
{
	string name;

	// tick 단위, 마지막 프레임 인덱스와 같음
	float duration = 0.f;
	// 초당 tick
	float frameRate = 0.f;
	UINT32 frameCount = 0;

	Array<AnimationTrack> tracks;
};