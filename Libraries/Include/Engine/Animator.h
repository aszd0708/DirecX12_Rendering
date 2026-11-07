#pragma once
#include "Component.h"

class Animation;
class Model;
struct BoneData;
struct KeyframeData;
struct AnimationData;
struct AnimationTrack;

struct AnimationClipData
{
	Animation* clip;
	Array<INT32> boneIndexes;
};

struct AnimationFrameData
{
	float ratio = 0.0f;
	UINT32 curFrameIdx = 0;
	UINT32 nextFrameIdx = 0;
};

class Animator : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Animator);

private:
	const float BLEND_TIME = 0.2f;
	const UINT8 NON_CHANGE_ANIMATION_VALUE = UINT8_MAX;

	static AnimationFrameData GetFrameData(const AnimationData& animationData, float clipTime);
	static Matrix GetLocalMatrix(const KeyframeData& keyFrameData);
	static KeyframeData SampleKeyframes(const AnimationTrack& animTrack, const AnimationFrameData& frameData);
	static KeyframeData GetLerpKeyframe(const KeyframeData& curKeyframeData, const KeyframeData& nextKeyframeData, float ratio);

public:
	Animator();
	virtual ~Animator() override;
	
public:
	void LateUpdate() override;

public:
	void SetTime(float time) { _curClipTime = time; }
	void SetAnimationIndex(UINT8 index);

private:
	void SetMatrixArray(AnimationClipData* curClipData, AnimationClipData* nextClipData, const AnimationFrameData& curframeData, const AnimationFrameData& nextFrameData, float fadeRatio);

private:
	void UpdateAllClipBoneIndexes();
	void SetBoneIndexes(AnimationClipData* clipData);

public:
	void AddAnimationClip(MemoryBlock& clipMemoryBlock);
	void SetModel(MemoryBlock& modelMemoryBlock);
	const Array<Matrix>& GetPaletMatrixArray() { return _paletMatrixArray; }
	const Array<Matrix>& GetGlobalMatrixArray();

private:
	bool _boneIndexSettingFlag = false;

	UINT8 _curClipIdx;
	UINT8 _nextClipIdx;
	float _curClipTime;
	float _nextClipTime;
	float _fadeTime;

	DynamicArray<AnimationClipData*> _clips;

	Model* _model;
	Array<Matrix> _globalMatrixArray;
	Array<Matrix> _paletMatrixArray;
};

