#pragma once
#include "ModelData.h"
#include "Renderer.h"

class Mesh;
class Material;

struct SubMesh
{
	Mesh* mesh = nullptr;
	INT32 materialIndex = INVALID_INDEX;
	INT32 boneIndex = INVALID_INDEX;
};

class Model : public IMemoryBlockHanlde
{
public:
	Model();
	~Model();

public:
	bool SetModelData(ModelData&& data);

public:
	const Array<SubMesh>& GetSubMeshes() { return _subMeshes; }
	const Array<Material*>& GetMaterials() { return _materials; }
	const Array<BoneData>& GetBones() { return _bones; }

private:
	// 추후 추가
	// ModelInfo _modelInfo;
	Array<SubMesh> _subMeshes;
	Array<Material*> _materials;
	Array<BoneData> _bones;
};

