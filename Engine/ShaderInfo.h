#pragma once
#include "Hash.h"

struct ShaderInfo
{
	wstring _path;
	Array<D3D12_INPUT_ELEMENT_DESC> _inputLayoutDesc;
};

template<>
struct Hash<ShaderInfo>
{
	static UINT64 GetHash(const ShaderInfo& key)
	{
		UINT64 hash = Hash<wstring>::GetHash(key._path);
		return hash;
	}
};

enum class eShaderIndex
{
	GLOBAL = 0,
	TRANSFORM = 1,
	TEXTURE_INDEX = 2,

	INSTANCE = 3,

	MAX
};