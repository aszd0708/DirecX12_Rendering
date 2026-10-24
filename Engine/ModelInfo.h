#pragma once
#include "ModelData.h"

struct ShaderInfo;

struct ModelInfo
{
	wstring modelFilePath;
	wstring materialFilePath;
	ModelData modelData;
	ShaderInfo shaderInfo;
};

template<>
struct Hash<ModelInfo>
{
	static UINT64 GetHash(const ModelInfo& key)
	{
		UINT64 hash = 0;
		UINT64 modelPathHash = Hash<wstring>::GetHash(key.modelFilePath);
		UINT64 materialPathHash = Hash<wstring>::GetHash(key.materialFilePath);
		UINT64 shaderInfoHash = Hash<ShaderInfo>::GetHash(key.shaderInfo);

		Hash<UINT64>::HashCombine(hash, modelPathHash);
		Hash<UINT64>::HashCombine(hash, materialPathHash);
		Hash<UINT64>::HashCombine(hash, shaderInfoHash);

		return hash;
	}
};