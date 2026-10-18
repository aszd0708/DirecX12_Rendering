#pragma once

struct MeshInfo
{
	/// <summary>
	/// Model을 위한 값 Mesh 단독으로는 INVALIED_INDEX값을 사용
	/// </summary>
	INT32 subMeshIndex = INVALID_INDEX;
	wstring filePath;
	Geometry geometry;
};

template<>
struct Hash<MeshInfo>
{
	static UINT64 GetHash(const MeshInfo& key)
	{
		UINT64 hash = Hash<wstring>::GetHash(key.filePath);
		if (key.subMeshIndex != INVALID_INDEX)
		{
			UINT64 indexHash = Hash<UINT32>::GetHash(key.subMeshIndex);
			Hash<UINT64>::HashCombine(hash, indexHash);
		}

		return hash;
	}
};