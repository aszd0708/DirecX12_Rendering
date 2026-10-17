#include "pch.h"
#include "Material.h"
#include "MaterialInfo.h"
#include "Shader.h"
#include "Texture.h"
#include "GpuCommandPool.h"
#include "GpuMaterialBufferPool.h"
#include "../Shaders/HLSL/ShaderShared.h"

Material::Material(const MaterialInfo& info, GpuCommandInfo* commandInfo) : _infoHash(Hash<MaterialInfo>::GetHash(info)), _shader(nullptr), _textures(), _textureIndexes()
{
	Init(info, commandInfo);
}

Material::~Material()
{
	for (int i = 0; i < _textureCount; ++i)
	{
		if (_textures[i] != nullptr)
		{
			RESOURCES->ReleaseTexture(_textures[i]->GetTextureInfo());
		}
	}

	if (_shader != nullptr)
	{
		RESOURCES->ReleaseShader(_shader->GetShaderInfo());
	}

	GPU_BUFFER_POOL->ReleaseMaterialMemory(_slot);
}

void Material::Init(const MaterialInfo& info, GpuCommandInfo* commandInfo)
{
	MemoryBlock shaderMemoryBlock = {};
	bool isSuccess =  RESOURCES->GetShader(info.shaderInfo, shaderMemoryBlock);
	if (isSuccess == false)
	{
		shaderMemoryBlock = RESOURCES->GetErrorShader();
	}

	isSuccess = CPU_MEM_POOL->GetMemoryPool(shaderMemoryBlock._poolID)->GetObjectByMemoryBlock(shaderMemoryBlock, &_shader);

	SetTextures(info);

	_slot = {};
	isSuccess = GPU_BUFFER_POOL->GetMaterialMemory(GpuBufferPoolManager::eBufferPoolID::MATERIAL, info.materialType, _slot);
	assert(isSuccess);
	GpuMemoryHandle handle = {};
	UINT64 dataSize = 0;
	void* data = malloc(MaterialInfo::GetDescSize(info.materialType));
	GetMaterialDesc(info, data, dataSize);
	GPU_BUFFER_POOL->GetMaterialPool()->Upload(commandInfo, _slot, data, dataSize, handle);
	commandInfo->AddCommandCount();

	free(data);
}

void Material::SetTextures(const MaterialInfo& info)
{
	UINT8 textureCount = 0;
	const TextureInfo* textureInfos = nullptr;

	switch (info.materialType)
	{
		case eMaterialType::DEFAULT:
		{
			const DefaultMaterialInfo& matInfo = static_cast<const DefaultMaterialInfo&>(info);
			textureInfos = matInfo.textureInfos;
			textureCount = DefaultMaterialInfo::TEX_COUNT;
			break;
		}

		default:
		{
			assert(false);
			break;
		}
	}

	if(textureInfos == nullptr || textureCount == 0) return;

	// 텍스처 세팅
	UINT8 index = 0;
	_textureCount = textureCount;
	for (int i = 0; i < textureCount; ++i)
	{
		_textureIndexes[i] = UINT32_MAX;

		if(textureInfos[i].IsValidFilePath() == false) continue;

		MemoryBlock memBlock = {};
		bool isSuccess = RESOURCES->GetTexture(textureInfos[i], memBlock);
		assert(isSuccess);

		isSuccess = CPU_MEM_POOL->GetMemoryPool(memBlock._poolID)->GetObjectByMemoryBlock(memBlock, &_textures[i]);
		assert(isSuccess);

		_textureIndexes[i] = _textures[i]->GetDescHandle().index;
	}
}

const UINT32* Material::GetTextureIndexes(OUT UINT8& count) const
{
	count = _textureCount;
	if (_textureCount == 0)
	{
		return nullptr;
	}

	return _textureIndexes;
}

const UINT64 Material::GetSlotIndex()
{
	return _slot.index;
}

void Material::GetMaterialDesc(const MaterialInfo& info, OUT void* data, OUT UINT64& size)
{
	switch (info.materialType)
	{
	case eMaterialType::DEFAULT:
		{
			const DefaultMaterialInfo& matInfo = static_cast<const DefaultMaterialInfo&>(info);
			DefaultMaterialTextureDesc desc = {};
			desc.normalTextureIndex = _textureIndexes[(UINT8)eDefaultTextureType::NORMAL];
			desc.diffuseTextureIndex = _textureIndexes[(UINT8)eDefaultTextureType::DIFFUSE];
			size = sizeof(desc);
			memcpy(data, &desc, size);
			break;
		}

	default:
		{
			assert(false);
			break;
		}
	}
}
