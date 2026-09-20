#include "pch.h"
#include "ResourceManager.h"
#include "MeshPool.h"
#include "TexturePool.h"
#include "MeshInfo.h"
#include "GpuCommandPool.h"

void ResourceManager::Init()
{
	_meshPool = new MeshPool();
	_texturePool = new TexturePool();
}

void ResourceManager::Release()
{
	delete(_meshPool);
	delete(_texturePool);
}

void ResourceManager::BeginBatch()
{
	bool isSuccess = GPU_COMMAND_POOL->GetCommandPool(&_commandInfo);
	assert(isSuccess);
}

void ResourceManager::EndBatch()
{
	assert(_commandInfo != nullptr);

	UINT16 count = _commandInfo->GetCommandAddedCount();
	if (count > 0)
	{
		// 커맨드 리스트 전달
		GPU_COMMAND_POOL->AddSendingQueueIndex(_commandInfo->GetPoolID());
		_commandInfo->Close();
		GPU_COMMAND_POOL->SendQueue();
	}
	GPU_COMMAND_POOL->ReleaseCommandPool(_commandInfo);
	_commandInfo = nullptr;
}

bool ResourceManager::GetMesh(MeshInfo& info, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID, OUT MemoryBlock& memoryBlock)
{
	assert(_commandInfo != nullptr);

	return _meshPool->PoolOutMesh(info, vertexPoolID, indexPoolID, _commandInfo, memoryBlock);
}

bool ResourceManager::ReleaseMesh(const MeshInfo& info)
{
	return _meshPool->PoolInMesh(info);
}

bool ResourceManager::GetTexture(const TextureInfo& info, OUT MemoryBlock& memoryBlock)
{
	assert(_commandInfo != nullptr);

	return _texturePool->PoolOutTexture(info, _commandInfo, memoryBlock);
}

bool ResourceManager::ReleaseTexture(const TextureInfo& info)
{
	return _texturePool->PoolInTexture(info);
}
