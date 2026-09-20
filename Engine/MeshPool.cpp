#include "pch.h"
#include "MeshPool.h"
#include "MeshInfo.h"
#include "Mesh.h"
#include "GpuCommandPool.h"

MeshPool::MeshPool() : _table()
{}

MeshPool::~MeshPool()
{}

bool MeshPool::PoolInMesh(const MeshInfo& key)
{
	UINT64 hashValue = Hash<MeshInfo>::GetHash(key);
	MeshPoolBlock poolBlock = {};
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

			Mesh* mesh = nullptr;
			isSuccess = pool->GetObjectByMemoryBlock(poolBlock.block, &mesh);
			assert(isSuccess);
			pool->ReleaseMemory(mesh);
			_table.RemoveKey(hashValue);
		}
		else
		{
			_table.Add(hashValue, poolBlock);
		}
	}

	return isSuccess;
}

bool MeshPool::PoolOutMesh(MeshInfo& key, const GpuBufferPoolManager::eBufferPoolID vertexPoolID, const GpuBufferPoolManager::eBufferPoolID indexPoolID, GpuCommandInfo* commandInfo, OUT MemoryBlock& memoryBlock)
{
	UINT64 hashValue = Hash<MeshInfo>::GetHash(key);
	MeshPoolBlock poolBlock = {};
	bool isSuccess = _table.GetValue(hashValue, poolBlock);
	if (isSuccess)
	{
		poolBlock.refCount++;
		memoryBlock = poolBlock.block;
		_table.Add(hashValue, poolBlock);
	}
	else
	{
		Mesh* mesh = nullptr;
		UINT8 cpuPoolID = 0;
		bool isSuccess = CPU_MEM_POOL->GetPoolID(sizeof(Mesh), cpuPoolID);
		assert(isSuccess && "CPU 메모리 풀에 맞는 메모리 없음");

		isSuccess = CPU_MEM_POOL->GetMemoryPool(cpuPoolID)->GetMemory(&mesh, key, vertexPoolID, indexPoolID);
		assert(isSuccess && "CPU 메모리 풀 메모리 부족");

		// 매시 생성
		GpuMemoryHandle vertexUploadHandler;
		GpuMemoryHandle indexUploadHandler;
		mesh->CreateMesh(commandInfo->GetCommandList(), vertexUploadHandler, indexUploadHandler);
		commandInfo->AddUploadHandle(vertexUploadHandler);
		commandInfo->AddUploadHandle(indexUploadHandler);
		commandInfo->AddCommandCount();

		memoryBlock = mesh->GetMemoryHandler();

		MeshPoolBlock newPoolBlock = {};
		newPoolBlock.refCount = 1;
		newPoolBlock.block = memoryBlock;

		_table.Add(hashValue, newPoolBlock);
	}

	return true;
}
