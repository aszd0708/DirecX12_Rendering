#pragma once

struct MeshInfo
{
	wstring filePath;
	Geometry geometry;
};

template<>
struct Hash<MeshInfo>
{
	static UINT64 GetHash(const MeshInfo& key)
	{
		UINT64 filePathHash = Hash<wstring>::GetHash(key.filePath);

		return filePathHash;
	}
};