#include "pch.h"
#include "Animator.h"
#include "Animation.h"
#include "Model.h"

Animator::Animator() : Component(eComponentType::Animator), 
_clip(nullptr), _time(0), _model(nullptr), _globalMatrixArray(), _paletMatrixArray(), _boneIndexes()
{
	
}

Animator::~Animator()
{
}

void Animator::Init(MemoryBlock& clipMemoryBlock, MemoryBlock& modelMemoryBlock)
{
	SetAnimationClip(clipMemoryBlock);
	SetModel(modelMemoryBlock);
}

void Animator::LateUpdate()
{
	Component::LateUpdate();

	if(_clip == nullptr) return;
	if(_model == nullptr) return;
	_time += DT;

	// 애니메이션 계산 부분 추가
	const Array<Matrix>& invertedMatrixArray = _model->GetBonesInvertedMatrix();
	const Array<BoneData>& boneDataArray = _model->GetBones();
	UINT32 matrixCount = invertedMatrixArray.GetCount();

	const AnimationData& animationData = _clip->GetAnimationData();
	const Array<AnimationTrack>& tracks = animationData.tracks;
	float length = animationData.duration / animationData.frameRate;
	while(_time > length)
	{
		_time -= length;
	}
	UINT32 frameIdx = _time * animationData.frameRate;
	if (frameIdx > animationData.duration)
	{
		frameIdx = 0;
	}
	UINT lerpCurrentFrameIdx = frameIdx;
	UINT lerpNextFrameIdx = lerpCurrentFrameIdx + 1;
	if (lerpCurrentFrameIdx > animationData.duration)
	{
		lerpNextFrameIdx = 0;
	}
	float ratio = _time * animationData.frameRate;
	float temp = 0;
	ratio = std::modf(ratio, &temp);
	for (UINT32 i = 0; i < matrixCount; ++i)
	{
		const BoneData& boneData = boneDataArray[i];

		if (i >= tracks.GetCount())
		{
			Matrix staticMatrix = boneData.rootMatrix;
			Matrix invertedMatrix = invertedMatrixArray[i];
			invertedMatrix *= staticMatrix;
			_globalMatrixArray[i] = staticMatrix;
			_paletMatrixArray[i] = invertedMatrix;
		}
		else
		{
			Matrix rootMatrix = boneData.rootMatrix;
			const AnimationTrack& track = tracks[i];

			const KeyframeData& currentKeyframe = track.keyframes[lerpCurrentFrameIdx];
			const KeyframeData& nextKeyframe = track.keyframes[lerpNextFrameIdx];

			Matrix localMatrix = GetLerpMatrix(currentKeyframe, nextKeyframe, ratio);

			if (boneData.parentBoneIndex != INVALID_INDEX)
			{
				rootMatrix = localMatrix * _globalMatrixArray[boneData.parentBoneIndex];
			}
			else
			{
				rootMatrix = localMatrix;
			}
			Matrix invertedMatrix = invertedMatrixArray[i];
			invertedMatrix *= rootMatrix;

			_globalMatrixArray[i] = rootMatrix;
			_paletMatrixArray[i] = invertedMatrix;
		}
	}
}

void Animator::SetAnimationClip(MemoryBlock & clipMemoryBlock)
{
	CPU_MEM_POOL->GetMemoryPool(clipMemoryBlock._poolID)->GetObjectByMemoryBlock(clipMemoryBlock, &_clip);
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

	/*
	if (_clip != nullptr)
	{
		const Array<BoneData>& bones = _model->GetBones();
		_boneIndexes.SetCount(bones.GetCount());

		
	}
	*/
}

Matrix Animator::GetLerpMatrix(const KeyframeData& currentKeyframe, const KeyframeData& nextKeyframe, float ratio)
{
	Vec3 scale = Vec3::Lerp(currentKeyframe.scale, nextKeyframe.scale, ratio);
	Quaternion rotation = Quaternion::Slerp(currentKeyframe.rotation, nextKeyframe.rotation, ratio);
	Vec3 translation = Vec3::Lerp(currentKeyframe.translation, nextKeyframe.translation, ratio);

	Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
	Matrix R = XMMatrixRotationQuaternion(rotation);
	Matrix T = XMMatrixTranslation(translation.x, translation.y, translation.z);

	Matrix localMatrix = S * R * T;
	return localMatrix;
}
