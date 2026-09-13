#pragma once
#include "CpuMemoryPool.h"

class TexturePool;
class TextureInfo;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);

public:
	void Init();
	void Release();

	bool GetTexture(const TextureInfo& info, OUT MemoryBlock& memoryBlock);
	bool ReleaseTexture(const TextureInfo& info);

private:
	TexturePool* _texturePool;
};

