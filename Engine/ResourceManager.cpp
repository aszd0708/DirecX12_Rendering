#include "pch.h"
#include "ResourceManager.h"
#include "MeshPool.h"
#include "TexturePool.h"
#include "MeshInfo.h"

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

bool ResourceManager::GetMesh(MeshInfo& info, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID, OUT MemoryBlock& memoryBlock)
{
	return _meshPool->PoolOutMesh(info, vertexPoolID, indexPoolID, memoryBlock);
}

bool ResourceManager::ReleaseMesh(const MeshInfo& info)
{
	return _meshPool->PoolInMesh(info);
}

bool ResourceManager::GetTexture(const TextureInfo& info, OUT MemoryBlock& memoryBlock)
{
	return _texturePool->PoolOutTexture(info, memoryBlock);
}

bool ResourceManager::ReleaseTexture(const TextureInfo& info)
{
	return _texturePool->PoolInTexture(info);
}
