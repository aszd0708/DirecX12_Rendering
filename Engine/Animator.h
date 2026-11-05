#pragma once
#include "Component.h"

class Animation;
class Model;
struct BoneData;
struct KeyframeData;

class Animator : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Animator);

public:
	Animator();
	virtual ~Animator() override;
	
public:
	void Init(MemoryBlock& clipMemoryBlock, MemoryBlock& modelMemoryBlock);
	void LateUpdate() override;

public:
	void SetAnimationClip(MemoryBlock& clipMemoryBlock);
	void SetModel(MemoryBlock& modelMemoryBlock);

	const Array<Matrix>& GetPaletMatrixArray() { return _paletMatrixArray; }

private:
	Matrix GetLerpMatrix(const KeyframeData& currentKeyframe, const KeyframeData& nextKeyframe, float ratio);

private:
	Animation* _clip;
	float _time;

	Model* _model;
	Array<Matrix> _globalMatrixArray;
	Array<Matrix> _paletMatrixArray;

	Array<INT> _boneIndexes;
};

