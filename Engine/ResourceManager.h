#pragma once
#include "CpuMemoryPool.h"
#include "GpuBufferPoolManager.h"
#include "ShaderInfo.h"
#include "MaterialPool.h"
#include "TextureInfo.h"

class MeshPool;
struct MeshInfo;

class TexturePool;

class ShaderPool;

struct MaterialInfo;

struct ModelInfo;
class ModelPool;

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

	bool GetModel(ModelInfo& info, OUT MemoryBlock& memoryBlock);
	bool ReleaseModel(const ModelInfo& key);

	const MemoryBlock& GetErrorShader();
	const MemoryBlock& GetErrorTexture();
	const MemoryBlock& GetErrorMaterial();

	const TextureInfo& GetErrorTextureInfo();

private:
	void CreateErrorResources();

private:
	MeshPool* _meshPool;
	TexturePool* _texturePool;
	ShaderPool* _shaderPool;
	MaterialPool* _materialPool;
	ModelPool* _modelPool;

	GpuCommandInfo* _commandInfo = nullptr;

	// Instancing을 위한 ID
	Stack<UINT32> _usableMeshID;
	Stack<UINT32> _usableShaderID;

	MemoryBlock _meshErrorMemoryBlock;

	MemoryBlock _textureErrorMemoryBlock;
	TextureInfo _errorTextureInfo;


	MemoryBlock _materialErrorMemoryBlock;
};

