#include "pch.h"
#include "ResourceManager.h"
#include "TexturePool.h"

void ResourceManager::Init()
{
	_texturePool = new TexturePool();
}

void ResourceManager::Release()
{
	delete(_texturePool);
}

bool ResourceManager::GetTexture(const TextureInfo& info, OUT MemoryBlock& memoryBlock)
{
	return _texturePool->PoolOutTexture(info, memoryBlock);
}

bool ResourceManager::ReleaseTexture(const TextureInfo& info)
{
	return _texturePool->PoolInTexture(info);
}
