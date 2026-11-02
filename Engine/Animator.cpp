#include "pch.h"
#include "Animator.h"
#include "Model.h"

Animator::Animator() : Component(eComponentType::Animator), _clip(nullptr), _time(0), _model(nullptr)
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

	//if(_clip == nullptr) return;
	if(_model == nullptr) return;

	// 애니메이션 계산 부분 추가
	const Array<Matrix>& invertedMatrixArray = _model->GetBonesInvertedMatrix();
	const Array<BoneData>& boneData = _model->GetBones();
	UINT32 matrixCount = invertedMatrixArray.GetCount();

	for (UINT32 i = 0; i < matrixCount; ++i)
	{
		Matrix rootMatrix = boneData[i].rootMatrix;
		Matrix invertedMatrix = invertedMatrixArray[i];
		invertedMatrix *= rootMatrix;

		_globalMatrixArray[i] = rootMatrix;
		_paletMatrixArray[i] = invertedMatrix;
	}

#if _DEBUG
	for (UINT32 i = 0; i < matrixCount; ++i)
	{
		Matrix plateMatrix = _paletMatrixArray[i];

		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				float invertexMatValue = plateMatrix.m[y][x];
				float identityMatValue = Matrix::Identity.m[y][x];
				float diff = invertexMatValue - identityMatValue;
				if (diff < 0)
				{
					diff *= -1;
				}
				assert(diff <= 0.001f);
			}
		}
	}
#endif
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
}
