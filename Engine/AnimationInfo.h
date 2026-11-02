#pragma once

struct AnimationInfo
{
	wstring filePath;
}; 

template<>
struct Hash<AnimationInfo>
{
	static UINT64 GetHash(const AnimationInfo& key)
	{
		UINT64 hash = Hash<wstring>::GetHash(key.filePath);

		return hash;
	}
};