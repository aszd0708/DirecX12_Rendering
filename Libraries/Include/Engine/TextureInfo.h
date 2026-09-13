#pragma once
#include "Hash.h"

/// <summary>
/// 텍스처를 캐싱하기 위한 정보
/// </summary>
struct TextureInfo
{
	wstring filePath;
	DXGI_FORMAT format;
	UINT16 mipLevels;
};

template<> 
struct Hash<TextureInfo>
{
	static UINT64 GetHash(const TextureInfo& key)
	{
		UINT64 filePathHash = Hash<wstring>::GetHash(key.filePath);
		UINT64 formatHash = Hash<UINT32>::GetHash(key.format);
		UINT64 mipLevelsHash = Hash<UINT16>::GetHash(key.mipLevels);

		HashCombine(filePathHash, formatHash);
		HashCombine(filePathHash, mipLevelsHash);

		return filePathHash;
	}

	static void HashCombine(UINT64& seed, const UINT64& hashedValue)
	{
		seed ^= hashedValue + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
};