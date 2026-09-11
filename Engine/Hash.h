#pragma once

extern const Array<UINT32> PRIMES;

template<typename T>
struct Hash
{
	static UINT64 GetHash(const T& value)
	{
		static_assert(std::is_trivially_copyable<T>::value, "...");

		UINT64 hash = FNV_OFFSET_BASIS;

		int size = sizeof(T);
		const BYTE* byte = reinterpret_cast<const BYTE*>(&value);
		for (int i = 0; i < size; ++i)
		{
			hash = hash ^ byte[i];
			hash = hash * FNV_PRIME;
		}
		return hash;
	}
};

template<> struct Hash<wstring>
{
	static UINT64 GetHash(const wstring& key)
	{
		UINT64 hash = FNV_OFFSET_BASIS;
		for (int i = 0; i < key.size(); ++i)
		{
			hash = hash ^ key[i];
			hash = hash * FNV_PRIME;
		}
		return hash;
	}
};