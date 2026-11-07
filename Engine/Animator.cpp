#include "pch.h"
#include "Animator.h"
#include "Animation.h"
#include "Model.h"
#include "ModelData.h"

AnimationFrameData Animator::GetFrameData(const AnimationData& animationData, float clipTime)
{
	float temp = 0;

	float ratio = clipTime * animationData.frameRate;
	ratio = std::modf(ratio, &temp);

	UINT32 frameIdx = clipTime * animationData.frameRate;
	if (frameIdx > animationData.duration)
	{
		frameIdx = 0;
	}
	UINT32 lerpCurrentFrameIdx = frameIdx;
	UINT32 lerpNextFrameIdx = lerpCurrentFrameIdx + 1;
	if (lerpCurrentFrameIdx >= animationData.duration)
	{
		lerpNextFrameIdx = 0;
	}

	AnimationFrameData frameData = {};
	frameData.ratio = ratio;
	frameData.curFrameIdx = lerpCurrentFrameIdx;
	frameData.nextFrameIdx = lerpNextFrameIdx;

	return frameData;
}

Matrix Animator::GetLocalMatrix(const KeyframeData& keyFrameData)
{
	Vec3 scale = keyFrameData.scale;
	Quaternion rotation = keyFrameData.rotation;
	Vec3 translation = keyFrameData.translation;

	Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
	Matrix R = XMMatrixRotationQuaternion(rotation);
	Matrix T = XMMatrixTranslation(translation.x, translation.y, translation.z);

	return S * R * T;
}

KeyframeData Animator::SampleKeyframes(const AnimationTrack& animTrack, const AnimationFrameData& frameData)
{
	const KeyframeData& currentKeyframe = animTrack.keyframes[frameData.curFrameIdx];
	const KeyframeData& nextKeyframe = animTrack.keyframes[frameData.nextFrameIdx];
	float ratio = frameData.ratio;

	KeyframeData keyFrame = GetLerpKeyframe(currentKeyframe, nextKeyframe, ratio);

	return keyFrame;
}

KeyframeData Animator::GetLerpKeyframe(const KeyframeData& curKeyframeData, const KeyframeData& nextKeyframeData, float ratio)
{
	Vec3 scale = Vec3::Lerp(curKeyframeData.scale, nextKeyframeData.scale, ratio);
	Quaternion rotation = Quaternion::Slerp(curKeyframeData.rotation, nextKeyframeData.rotation, ratio);
	Vec3 translation = Vec3::Lerp(curKeyframeData.translation, nextKeyframeData.translation, ratio);

	KeyframeData data = {};
	data.scale = scale;
	data.rotation = rotation;
	data.translation = translation;

	return data;
}

Animator::Animator() : Component(eComponentType::Animator), 
_boneIndexSettingFlag(false), _curClipIdx(0), _nextClipIdx(NON_CHANGE_ANIMATION_VALUE), _curClipTime(0.0f), _nextClipTime(0.0f), _fadeTime(0.0f),
_clips(), _model(nullptr), _globalMatrixArray(), _paletMatrixArray()
{
	
}

Animator::~Animator()
{
	for (int i = 0; i < _clips.GetCount(); ++i)
	{
		if (_clips[i] == nullptr) continue;

		RESOURCES->ReleaseAnimation(_clips[i]->clip->GetAnimationInfo());
		delete _clips[i];
	}
}

void Animator::LateUpdate()
{
	Component::LateUpdate();

	if (_boneIndexSettingFlag)
	{
		UpdateAllClipBoneIndexes();
	}

	if(_clips.GetCount() <= _curClipIdx || _clips[_curClipIdx]->clip == nullptr) return;
	if(_model == nullptr) return;
	_curClipTime += DT;

	AnimationClipData* curClipData = _clips[_curClipIdx];
	const AnimationData& curAnimData = curClipData->clip->GetAnimationData();
	float curLength = curAnimData.duration / curAnimData.frameRate;
	while(_curClipTime >= curLength)
	{
		_curClipTime -= curLength;
	}
	AnimationFrameData curFrameData = GetFrameData(curAnimData, _curClipTime);

	float fadeRatio = 0.0f;
	AnimationClipData* nextClipData = nullptr;
	AnimationFrameData nextFrameData = {};
	if (_nextClipIdx != NON_CHANGE_ANIMATION_VALUE)
	{
		nextClipData = _clips[_nextClipIdx];
		_nextClipTime += DT;
		_fadeTime += DT;
		const AnimationData& nextAnimData = nextClipData->clip->GetAnimationData();
		float nextLength = nextAnimData.duration / nextAnimData.frameRate;
		while (_nextClipTime >= nextLength)
		{
			_nextClipTime -= nextLength;
		}
		nextFrameData = GetFrameData(nextAnimData, _nextClipTime);
		fadeRatio = _fadeTime / BLEND_TIME;
		if (fadeRatio > 1.0f)
		{
			fadeRatio = 1.0f;
		}
	}

	SetMatrixArray(curClipData, nextClipData, curFrameData, nextFrameData, fadeRatio);

	if (fadeRatio >= 1.0f)
	{
		_curClipIdx = _nextClipIdx;
		_curClipTime = _nextClipTime;

		_nextClipIdx = NON_CHANGE_ANIMATION_VALUE;
		_nextClipTime = 0.0f;

		_fadeTime = 0.0f;
	}
}

void Animator::SetMatrixArray(AnimationClipData* curClipData, AnimationClipData* nextClipData, const AnimationFrameData& curframeData, const AnimationFrameData& nextFrameData, float fadeRatio)
{
	const AnimationData& curAnimationData = curClipData->clip->GetAnimationData();
	const Array<AnimationTrack>& tracks = curAnimationData.tracks;
	const Array<INT32>& boneIndexes = curClipData->boneIndexes;
	const Array<Matrix>& invertedMatrixArray = _model->GetBonesInvertedMatrix();
	const Array<BoneData>& boneDataArray = _model->GetBones();
	UINT32 matrixCount = invertedMatrixArray.GetCount();

	for (UINT32 i = 0; i < matrixCount; ++i)
	{
		const BoneData& boneData = boneDataArray[i];

		Matrix rootMatrix = boneData.rootMatrix;
		Matrix invertedMatrix = invertedMatrixArray[i];
		if (boneIndexes[i] != INVALID_INDEX)
		{
			const AnimationTrack& curTrack = tracks[boneIndexes[i]];
			KeyframeData keyFrame = SampleKeyframes(curTrack, curframeData);
			Matrix localMatrix = {};
			if (nextClipData != nullptr)
			{
				const AnimationData& nextAnimationData = nextClipData->clip->GetAnimationData();
				INT32 boneIdx = nextClipData->boneIndexes[i];
				if (boneIdx == INVALID_INDEX)
				{
					localMatrix = GetLocalMatrix(keyFrame);
				}
				else
				{
					const AnimationTrack& nextTrack = nextAnimationData.tracks[boneIdx];

					KeyframeData nextKeyFrame = SampleKeyframes(nextTrack, nextFrameData);
					KeyframeData lerpKeyframe = GetLerpKeyframe(keyFrame, nextKeyFrame, fadeRatio);
					localMatrix = GetLocalMatrix(lerpKeyframe);
				}
			}
			else
			{
				localMatrix = GetLocalMatrix(keyFrame);
			}

			if (boneData.parentBoneIndex != INVALID_INDEX)
			{
				rootMatrix = localMatrix * _globalMatrixArray[boneData.parentBoneIndex];
			}
			else
			{
				rootMatrix = localMatrix;
			}
		}
		invertedMatrix *= rootMatrix;
		_globalMatrixArray[i] = rootMatrix;
		_paletMatrixArray[i] = invertedMatrix;
	}
}

void Animator::UpdateAllClipBoneIndexes()
{
	for (int i = 0; i < _clips.GetCount(); ++i)
	{
		if(_clips[i] == nullptr) continue;

		SetBoneIndexes(_clips[i]);
	}
	_boneIndexSettingFlag = false;
}

void Animator::SetBoneIndexes(AnimationClipData* clipData)
{
	if (clipData->clip == nullptr || _model == nullptr)
	{
		clipData->boneIndexes = Array<INT>(0);
		return;
	}

	const Array<BoneData>& bones = _model->GetBones();
	clipData->boneIndexes = Array<INT>(bones.GetCount());
	const Array<AnimationTrack>& tracks = clipData->clip->GetAnimationData().tracks;

	for (UINT32 i = 0; i < clipData->boneIndexes.GetCount(); ++i)
	{
		clipData->boneIndexes[i] = INVALID_INDEX;
		const string& boneName = bones[i].name;
		for (UINT32 j = 0; j < tracks.GetCount(); ++j)
		{
			if (tracks[j].boneName == boneName)
			{
				clipData->boneIndexes[i] = j;
				break;
			}
		}
	}
}

void Animator::AddAnimationClip(MemoryBlock& clipMemoryBlock)
{
	Animation* animator = nullptr;
	CPU_MEM_POOL->GetMemoryPool(clipMemoryBlock._poolID)->GetObjectByMemoryBlock(clipMemoryBlock, &animator);

	AnimationClipData* data = new AnimationClipData();
	data->clip = animator;
	SetBoneIndexes(data);
	_clips.Add(data);
}

void Animator::SetModel(MemoryBlock& modelMemoryBlock)
{
	bool isSuccess = CPU_MEM_POOL->GetMemoryPool(modelMemoryBlock._poolID)->GetObjectByMemoryBlock(modelMemoryBlock, &_model);

	if (isSuccess == false)
	{
		return;
	}

	UINT32 matrixCount = _model->GetBonesInvertedMatrix().GetCount();
	_globalMatrixArray = Array<Matrix>(matrixCount);
	_paletMatrixArray = Array<Matrix>(matrixCount);

	_boneIndexSettingFlag = true;
}

const Array<Matrix>& Animator::GetGlobalMatrixArray()
{
	return _globalMatrixArray;
}

void Animator::SetAnimationIndex(UINT8 index)
{
	if (_clips.GetCount() <= index) return;

	if (_curClipIdx == index)
	{
		if (_nextClipIdx != NON_CHANGE_ANIMATION_VALUE)
		{
			_curClipIdx = index;

			_nextClipIdx = NON_CHANGE_ANIMATION_VALUE;
			_nextClipTime = 0.0f;
		}
		_curClipTime = 0.0f;
	}
	else if (_nextClipIdx == index)
	{
		return;
	}

	else
	{
		if (_nextClipIdx == NON_CHANGE_ANIMATION_VALUE)
		{
			_nextClipIdx = index;
			_nextClipTime = 0.0f;
		}
		else
		{
			_curClipIdx = _nextClipIdx;
			_curClipTime = _nextClipTime;

			_nextClipIdx = index;
			_nextClipTime = 0.0f;
		}
		_fadeTime = 0.0f;
	}
}
