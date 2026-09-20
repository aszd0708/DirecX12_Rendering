#pragma once
#include "CpuMemoryPool.h"
#include "GpuBufferPoolManager.h"

class MeshPool;
class MeshInfo;

class TexturePool;
class TextureInfo;

class GpuCommandInfo;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);

public:
	void Init();
	void Release();

	void BeginBatch();
	void EndBatch();

	bool GetMesh(MeshInfo& info, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID, OUT MemoryBlock& memoryBlock);
	bool ReleaseMesh(const MeshInfo& info);

	bool GetTexture(const TextureInfo& info, OUT MemoryBlock& memoryBlock);
	bool ReleaseTexture(const TextureInfo& info);

private:
	MeshPool* _meshPool;
	TexturePool* _texturePool;

	GpuCommandInfo* _commandInfo = nullptr;
};

