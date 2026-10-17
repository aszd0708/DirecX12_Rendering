#include "pch.h"
#include "ResourceManager.h"
#include "MeshInfo.h"
#include "MeshPool.h"
#include "TexturePool.h"
#include "ShaderPool.h"
#include "MaterialPool.h"
#include "GpuCommandPool.h"

void ResourceManager::Init()
{
	_meshPool = new MeshPool();
	_texturePool = new TexturePool();
	_shaderPool = new ShaderPool();
	_materialPool = new MaterialPool();

	CreateErrorResources();
}

void ResourceManager::Release()
{
	delete(_meshPool);
	delete(_texturePool);
	delete(_shaderPool);
	delete(_materialPool);

	CPU_MEM_POOL->GetMemoryPool(_meshErrorMemoryBlock._poolID)->ReleaseMemory(_meshErrorMemoryBlock);
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
	else
	{
		_commandInfo->Close();
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

bool ResourceManager::GetShader(const ShaderInfo& info, OUT MemoryBlock& memoryBlock)
{
	return _shaderPool->PoolOutShader(info, memoryBlock);
}

bool ResourceManager::ReleaseShader(const ShaderInfo& info)
{
	return _shaderPool->PoolInShader(info);
}

bool ResourceManager::GetMaterial(const MaterialInfo& info, OUT MemoryBlock& memoryBlock)
{
	return _materialPool->PoolOutMaterial(info, _commandInfo, memoryBlock);
}

void ResourceManager::ReleaseMaterial(const UINT64 key)
{
	_materialPool->PoolInMaterial(key);
}

const MemoryBlock& ResourceManager::GetErrorShader()
{	
	return _meshErrorMemoryBlock;
}

void ResourceManager::CreateErrorResources()
{
	ShaderInfo _meshErrorShader = {};
	_meshErrorShader.path = L"ERROR_COLOR";
	_meshErrorShader.inputLayoutDesc = ErrorVertexData::GetDesc();

	_meshErrorMemoryBlock = {};
	assert(GetShader(_meshErrorShader, _meshErrorMemoryBlock));
}
