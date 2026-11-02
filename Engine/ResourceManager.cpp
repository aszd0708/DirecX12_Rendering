#include "pch.h"
#include "ResourceManager.h"
#include "MeshInfo.h"
#include "MaterialInfo.h"
#include "MeshPool.h"
#include "TexturePool.h"
#include "ShaderPool.h"
#include "Material.h"
#include "MaterialPool.h"
#include "ModelPool.h"
#include "AnimationPool.h"
#include "GpuCommandPool.h"

void ResourceManager::Init()
{
	_meshPool = new MeshPool();
	_texturePool = new TexturePool();
	_shaderPool = new ShaderPool();
	_materialPool = new MaterialPool();
	_modelPool = new ModelPool();
	_animationPool = new AnimationPool();

	CreateErrorResources();
}

void ResourceManager::Release()
{
	delete(_meshPool);
	delete(_texturePool);
	delete(_shaderPool);
	delete(_materialPool);
	delete(_modelPool);
	delete(_animationPool);

	CPU_MEM_POOL->GetMemoryPool(_materialErrorMemoryBlock._poolID)->ReleaseMemory(_materialErrorMemoryBlock);
	CPU_MEM_POOL->GetMemoryPool(_textureErrorMemoryBlock._poolID)->ReleaseMemory(_textureErrorMemoryBlock);
	CPU_MEM_POOL->GetMemoryPool(_errorShaderMemoryBlock._poolID)->ReleaseMemory(_errorShaderMemoryBlock);
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
	bool isSuccess = _materialPool->PoolOutMaterial(info, _commandInfo, memoryBlock);
	if (isSuccess == false)
	{
		return isSuccess;
	}

	return isSuccess;
}

void ResourceManager::ReleaseMaterial(const UINT64 key)
{
	_materialPool->PoolInMaterial(key);
}

bool ResourceManager::GetModel(ModelInfo& info, OUT MemoryBlock& memoryBlock)
{
	assert(_commandInfo != nullptr);

	return _modelPool->PoolOutModel(info, _commandInfo, memoryBlock);
}

bool ResourceManager::ReleaseModel(const ModelInfo& key)
{
	return _modelPool->PoolInModel(key);
}

bool ResourceManager::GetAnimation(const AnimationInfo& key, OUT MemoryBlock& memoryBlock)
{
	return _animationPool->PoolOutAnimation(key, memoryBlock);
}

bool ResourceManager::ReleaseAnimation(const AnimationInfo& key)
{
	return _animationPool->PoolInAnimation(key);
}

Shader* ResourceManager::GetErrorShader()
{	
	return _errorShader;
}

const MemoryBlock& ResourceManager::GetErrorShaderMemoryBlock()
{
	return _errorShaderMemoryBlock;
}

const MemoryBlock& ResourceManager::GetErrorTexture()
{
	return _textureErrorMemoryBlock;
}

const MemoryBlock& ResourceManager::GetErrorMaterial()
{
	return _materialErrorMemoryBlock;
}

const UINT64 ResourceManager::GetErrorMaterialSlotIdx()
{
	return _errorMaterialSlotIdx;
}

const TextureInfo& ResourceManager::GetErrorTextureInfo()
{
	return _errorTextureInfo;
}

void ResourceManager::CreateErrorResources()
{
	BeginBatch();

	ShaderInfo meshErrorShader = {};
	meshErrorShader.path = L"ERROR_COLOR";
	meshErrorShader.inputLayoutDesc = ErrorVertexData::GetDesc();

	_errorShaderMemoryBlock = {};
	bool isSuccess = GetShader(meshErrorShader, _errorShaderMemoryBlock);
	if (isSuccess == false)
	{
		assert(isSuccess);
	}
	_errorShader = nullptr;
	isSuccess = CPU_MEM_POOL->GetMemoryPool(_errorShaderMemoryBlock._poolID)->GetObjectByMemoryBlock(_errorShaderMemoryBlock, &_errorShader);

	if (isSuccess == false)
	{
		assert(isSuccess);
	}

	ShaderInfo materialErrorShader = {};
	materialErrorShader.path = L"ERROR_TEXTURE";
	materialErrorShader.inputLayoutDesc = VertexTextureData::GetDesc();

	_errorTextureInfo = {};
	_errorTextureInfo.filePath = L"../Resources/Missing_Texture.png";
	_errorTextureInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_errorTextureInfo.mipLevels = 1;
	isSuccess = GetTexture(_errorTextureInfo, _textureErrorMemoryBlock);
	if (isSuccess == false)
	{
		assert(isSuccess);
	}

	DefaultMaterialInfo errorMaterialInfo = {};
	errorMaterialInfo.shaderInfo = materialErrorShader;
	errorMaterialInfo.textureInfos[(UINT8)eDefaultTextureType::DIFFUSE] = _errorTextureInfo;
	_materialErrorMemoryBlock = {};
	isSuccess = GetMaterial(errorMaterialInfo, _materialErrorMemoryBlock);
	if (isSuccess == false)
	{
		assert(isSuccess);
	}

	Material* material = nullptr;
	isSuccess = CPU_MEM_POOL->GetMemoryPool(_materialErrorMemoryBlock._poolID)->GetObjectByMemoryBlock(_materialErrorMemoryBlock, &material);

	if (isSuccess == false)
	{
		assert(isSuccess);
	}

	_errorMaterialSlotIdx = material->GetSlotIndex();

	EndBatch();
}
