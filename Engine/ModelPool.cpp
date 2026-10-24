#include "pch.h"
#include "ModelPool.h"
#include "Model.h"
#include "ModelData.h"
#include "ModelInfo.h"
#include "ModelLoader.h"
#include "GpuCommandPool.h"

ModelPool::ModelPool() : _table(), _usableInstancingIDs(), _curInstancingID(0)
{

}

ModelPool::~ModelPool()
{

}

bool ModelPool::PoolInModel(const ModelInfo& key)
{
	UINT64 hashValue = Hash<ModelInfo>::GetHash(key);
	ModelPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		assert(poolBlock.refCount != 0);
		poolBlock.refCount--;

		if (poolBlock.refCount <= 0)
		{
			// 여기 지연 해제 하는 부분
			// 일단 CPU풀에 바로 제거
			CpuMemoryPool* pool = CPU_MEM_POOL->GetMemoryPool(poolBlock.block._poolID);

			Model* model = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &model);
			assert(isSuccess);
			//_usableInstancingIDs.Push(mesh->GetInstancingID());
			pool->ReleaseMemory(model);
			_table.RemoveKey(hashValue);
		}
		else
		{
			_table.Add(hashValue, poolBlock);
		}
	}

	return isSuccess;
}

bool ModelPool::PoolOutModel(ModelInfo& key, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<ModelInfo>::GetHash(key);
	ModelPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Model* model = nullptr;
		UINT8 cpuPoolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Model), cpuPoolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(cpuPoolID)->GetMemory(&model);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		/*
		UINT32 instancingID = _curInstancingID;
		bool usingCurInstancingID = true;
		if (_usableInstancingIDs.GetCount() > 0)
		{
			//isSuccess = _usableInstancingIDs.Pop(instancingID);
			usingCurInstancingID = isSuccess == false;
			if (isSuccess == false)
			{
				instancingID = _curInstancingID;
			}
		}
		if (usingCurInstancingID)
		{
			_curInstancingID++;
		}

		*/
		
		// 모델 생성
		ModelData modelData = {};
		isSuccess = ModelLoader::LoadModel(key.modelFilePath, key.materialFilePath, key.shaderInfo, key.modelData);
		assert(isSuccess && "Model Load 실패");

		model->SetModelData(move(key.modelData));
		commandInfo->AddCommandCount();

		memoryBlock = model->GetMemoryHandler();

		ModelPoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;

		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
