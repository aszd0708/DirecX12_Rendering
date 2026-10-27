#pragma once
#include "MaterialInfoEnum.h"
#include "ShaderInfo.h"
#include "TextureInfo.h"

struct MaterialInfo
{
public:
	static UINT64 GetDescSize(eMaterialType type);

protected:
	MaterialInfo(eMaterialType type) : materialType(type) {}

public:
	virtual ~MaterialInfo() {}

public:
	eMaterialType materialType;
	ShaderInfo shaderInfo;
};

struct DefaultMaterialInfo : public MaterialInfo
{
	static const UINT8 TEX_COUNT = (UINT8)eDefaultTextureType::MAX;
	static const eMaterialType MAT_TYPE = eMaterialType::DEFAULT;

public:
	DefaultMaterialInfo() : MaterialInfo(MAT_TYPE) 
	{
		
	}
	~DefaultMaterialInfo() override {}; 
	
public:
	TextureInfo textureInfos[TEX_COUNT];
};
static_assert(DefaultMaterialInfo::TEX_COUNT <= MAX_TEXTURE_COUNT);

struct DefaultMaterialTextureDesc
{
	UINT32 normalTextureIndex = UINT32_MAX;
	UINT32 diffuseTextureIndex = UINT32_MAX;

	UINT32 padding0 = 0;
	UINT32 padding1 = 0;
};
static_assert(sizeof(DefaultMaterialTextureDesc) == 16);

template<>
struct Hash<DefaultMaterialInfo>
{
	static UINT64 GetHash(const DefaultMaterialInfo& key)
	{
		UINT64 shaderHash = Hash<ShaderInfo>::GetHash(key.shaderInfo);
		UINT64 textureHash = 0;
		for (int i = 0; i < (int)eDefaultTextureType::MAX; ++i)
		{
			UINT64 hash = Hash<TextureInfo>::GetHash(key.textureInfos[i]);
			Hash<UINT64>::HashCombine(textureHash, hash);
		}

		UINT64 result = 0;
		Hash<UINT64>::HashCombine(result, shaderHash);
		Hash<UINT64>::HashCombine(result, textureHash);
		
		return result;
	}
};

template<>
struct Hash<MaterialInfo>
{
	static UINT64 GetHash(const MaterialInfo& key)
	{
		UINT64 hash = 0;
		switch (key.materialType)
		{
		case eMaterialType::DEFAULT:
			hash = Hash<DefaultMaterialInfo>::GetHash(static_cast<const DefaultMaterialInfo&>(key));
			break;
		default:
			assert(false);
		break;
		}

		return hash;
	}
};