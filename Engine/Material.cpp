#include "pch.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(const MaterialInfo& info) : _shader(nullptr), _textures(), _textureIndexes()
{
	Init(info);
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
}

void Material::Init(const MaterialInfo& info)
{
	MemoryBlock shaderMemoryBlock = {};
	bool isSuccess =  RESOURCES->GetShader(info.shaderInfo, shaderMemoryBlock);
	if (isSuccess == false)
	{
		shaderMemoryBlock = RESOURCES->GetErrorShader();
	}

	isSuccess = CPU_MEM_POOL->GetMemoryPool(shaderMemoryBlock._poolID)->GetObjectByMemoryBlock(shaderMemoryBlock, &_shader);

	SetTextures(info);
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
