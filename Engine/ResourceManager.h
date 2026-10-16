#pragma once
#include "CpuMemoryPool.h"
#include "GpuBufferPoolManager.h"
#include "ShaderInfo.h"
#include "MaterialPool.h"

class MeshPool;
struct MeshInfo;

class TexturePool;
struct TextureInfo;

class ShaderPool;

struct MaterialInfo;

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

	bool GetMaterial(const MaterialInfo& info, OUT MemoryBlock& memoryBlock);
	void ReleaseMaterial(const UINT64 key);

	const MemoryBlock& GetErrorShader();

private:
	void CreateErrorResources();

private:
	MeshPool* _meshPool;
	TexturePool* _texturePool;
	ShaderPool* _shaderPool;
	MaterialPool* _materialPool;

	GpuCommandInfo* _commandInfo = nullptr;

	// Instancing을 위한 ID
	Stack<UINT32> _usableMeshID;
	Stack<UINT32> _usableShaderID;

	MemoryBlock _meshErrorMemoryBlock;
};

