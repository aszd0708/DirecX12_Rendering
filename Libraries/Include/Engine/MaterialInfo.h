#pragma once
#include "MaterialInfoEnum.h"
#include "ShaderInfo.h"
#include "TextureInfo.h"
#include "../Shaders/HLSL/ShaderShared.h"

struct MaterialInfo
{
public:
	static UINT64 GetDescSize();
	
public:
	eMaterialType materialType = eMaterialType::ERROR_TYPE;
	ShaderInfo shaderInfo;
	TextureInfo textureInfos[MAX_TEXTURE_COUNT];
	UINT8 textureCount = 0;
};

template<>
struct Hash<MaterialInfo>
{
	static UINT64 GetHash(const MaterialInfo& key)
	{
		UINT64 shaderHash = Hash<ShaderInfo>::GetHash(key.shaderInfo);
		UINT64 textureHash = 0;
		for (UINT8 i = 0; i < key.textureCount; ++i)
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

struct MaterialTextureDesc
{
	MaterialTextureDesc()
	{
		std::fill(textureIndexes, textureIndexes + MAX_TEXTURE_COUNT, UINT32_MAX);
	}
	UINT32 textureIndexes[MAX_TEXTURE_COUNT];
};
static_assert((sizeof(MaterialTextureDesc) & (16 - 1)) == 0);