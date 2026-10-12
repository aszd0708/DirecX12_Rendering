#pragma once
#include "Hash.h"

/// <summary>
/// 텍스처를 캐싱하기 위한 정보
/// </summary>
struct TextureInfo
{
	wstring filePath;
	DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	UINT16 mipLevels = 0;

public:
	bool IsValidFilePath() const 
	{
		return filePath.empty() == false;
	}
};

template<> 
struct Hash<TextureInfo>
{
	static UINT64 GetHash(const TextureInfo& key)
	{
		UINT64 filePathHash = Hash<wstring>::GetHash(key.filePath);
		UINT64 formatHash = Hash<UINT32>::GetHash(key.format);
		UINT64 mipLevelsHash = Hash<UINT16>::GetHash(key.mipLevels);

		Hash<UINT64>::HashCombine(filePathHash, formatHash);
		Hash<UINT64>::HashCombine(filePathHash, mipLevelsHash);

		return filePathHash;
	}
};