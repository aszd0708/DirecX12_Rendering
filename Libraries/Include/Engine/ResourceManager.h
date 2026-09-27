#pragma once
#include "CpuMemoryPool.h"
#include "GpuBufferPoolManager.h"

class MeshPool;
struct MeshInfo;

class TexturePool;
struct TextureInfo;

class ShaderPool;
struct ShaderInfo;

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

	bool GetShader(const ShaderInfo& info, OUT MemoryBlock& memoryBlock);
	bool ReleaseShader(const ShaderInfo& info);

private:
	MeshPool* _meshPool;
	TexturePool* _texturePool;
	ShaderPool* _shaderPool;

	GpuCommandInfo* _commandInfo = nullptr;
};

