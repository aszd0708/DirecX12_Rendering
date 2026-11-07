#pragma once
#include "ModelData.h"
#include "Renderer.h"
#include "ModelInfo.h"

class Mesh;
class Material;

struct SubMesh
{
	Mesh* mesh = nullptr;
	INT32 materialIndex = INVALID_INDEX;
	INT32 boneIndex = INVALID_INDEX;
	bool isSkinned = false;
};

class Model : public IMemoryBlockHanlde
{
public:
	Model();
	~Model();

public:
	bool SetModelData(ModelData&& data, const ModelInfo& modelInfo);

public:
	const ModelInfo& GetModelInfo() { return _modelInfo; }

	const Array<SubMesh>& GetSubMeshes() { return _subMeshes; }
	const Array<Material*>& GetMaterials() { return _materials; }
	const Array<BoneData>& GetBones() { return _bones; }
	const Array<Matrix>& GetBonesInvertedMatrix() { return _bonesInvertedMatrix; }

private:
	ModelInfo _modelInfo;

	Array<SubMesh> _subMeshes;
	Array<Material*> _materials;
	Array<BoneData> _bones;
	Array<Matrix> _bonesInvertedMatrix;
};

