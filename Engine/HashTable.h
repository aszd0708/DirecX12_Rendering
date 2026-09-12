#pragma once
#include "Hash.h"

template<typename K, typename V>
class HashTable
{
	enum class eHashTableCondition : UINT8
	{
		EMPTY = 0,
		OCCUPIED = 1,
		TOMBSTONE = 2
	};

	struct TableNode
	{
		eHashTableCondition condition = eHashTableCondition::EMPTY;
		K key;
		V value;
	};

public:
	HashTable();
	~HashTable();

private:
	void SetCapacity(UINT8 nextIndex);
	bool FindIndex(const K& key, OUT UINT32& index);

public:
	void Add(const K& key, const V& value);
	bool GetValue(const K& key, OUT V& value);
	bool RemoveAndGetValue(const K& key, OUT V& value);
	bool RemoveKey(const K& key);
	bool FindValue(const K& key);

	UINT32 GetCapacity();
	bool IsOccupiedAt(UINT32 index, OUT K& key, OUT V& value);

private:
	/// <summary>
	/// PRIMES 배열 참고
	/// </summary>
	UINT8 _capacityIndex;
	UINT32 _count;

	TableNode* _table;
};

template<typename K, typename V>
inline HashTable<K, V>::HashTable()
{
	_capacityIndex = 0;
	_count = 0;
	
	_table = (TableNode*)malloc(sizeof(TableNode) * PRIMES[_capacityIndex]);

	for (int i = 0; i < PRIMES[_capacityIndex]; ++i)
	{
		_table[i].condition = eHashTableCondition::EMPTY;
	}
}

template<typename K, typename V>
inline HashTable<K, V>::~HashTable()
{
	free(_table);
}

template<typename K, typename V>
inline void HashTable<K, V>::SetCapacity(UINT8 nextIndex)
{
	assert(_capacityIndex < nextIndex);

	TableNode* newTable = (TableNode*)malloc(sizeof(TableNode) * PRIMES[nextIndex]);

	for (int i = 0; i < PRIMES[nextIndex]; ++i)
	{
		newTable[i].condition = eHashTableCondition::EMPTY;
	}

	for (int i = 0; i < PRIMES[_capacityIndex]; ++i)
	{
		if (_table[i].condition == eHashTableCondition::OCCUPIED)
		{
			UINT32 arrayTotalCount = PRIMES[nextIndex];
			UINT64 hashResult = Hash<K>::GetHash(_table[i].key);
			hashResult %= arrayTotalCount;
			for (int j = 0; j < arrayTotalCount; ++j)
			{
				if (newTable[hashResult].condition == eHashTableCondition::EMPTY)
				{
					UINT32 index = hashResult;
					newTable[index].condition = eHashTableCondition::OCCUPIED;
					newTable[index].key = _table[i].key;
					newTable[index].value = _table[i].value;
					break;
				}
				hashResult++;
				hashResult %= arrayTotalCount;
			}
		}
	}

	free(_table);
	_table = newTable;
	_capacityIndex = nextIndex;
}

template<typename K, typename V>
inline bool HashTable<K, V>::FindIndex(const K& key, OUT UINT32& index)
{
	UINT32 arrayTotalCount = PRIMES[_capacityIndex];
	UINT64 hashResult = Hash<K>::GetHash(key);
	hashResult %= arrayTotalCount;

	for (int i = 0; i < arrayTotalCount; ++i)
	{
		switch (_table[hashResult].condition)
		{
		case eHashTableCondition::EMPTY: return false;
		case eHashTableCondition::OCCUPIED:
			if (_table[hashResult].key == key)
			{
				index = hashResult;
				return true;;
			}
			break;
		}
		hashResult++;
		hashResult %= arrayTotalCount;
	}
	return false;
}

template<typename K, typename V>
inline void HashTable<K, V>::Add(const K & key, const V & value)
{
	if (_count >= PRIMES[_capacityIndex])
	{
		SetCapacity(_capacityIndex + 1);
	}
	UINT32 arrayTotalCount = PRIMES[_capacityIndex];
	UINT64 hashResult = Hash<K>::GetHash(key);
	hashResult %= arrayTotalCount;

	UINT32 tombstoneIndex = 0;
	bool foundTombstone = false;
	for (int i = 0; i < arrayTotalCount; ++i)
	{
		bool isBreak = false;
		switch (_table[hashResult].condition)
		{
			case eHashTableCondition::EMPTY:
			{
				UINT32 index = hashResult;
				if (foundTombstone)
				{
					index = tombstoneIndex;
				}
				_table[index].condition = eHashTableCondition::OCCUPIED;
				_table[index].key = key;
				_table[index].value = value;
				isBreak = true;
				_count++;
				break;
			}
			case eHashTableCondition::TOMBSTONE:
			{
				tombstoneIndex = hashResult;
				foundTombstone = true;
			}
			break;
			case eHashTableCondition::OCCUPIED:
			if (_table[hashResult].key == key)
			{
				_table[hashResult].value = value;
				isBreak = true;
			}
			break;
		}
		if(isBreak)break;

		hashResult++;
		hashResult %= arrayTotalCount;
	}
}

template<typename K, typename V>
inline bool HashTable<K, V>::GetValue(const K & key, OUT V & value)
{
	UINT32 index = 0;
	bool isSuccess = FindIndex(key, index);
	if (isSuccess)
	{
		value = _table[index].value;
	}
	return isSuccess;
}

template<typename K, typename V>
inline bool HashTable<K, V>::RemoveAndGetValue(const K& key, OUT V& value)
{
	UINT32 index = 0;
	bool isSuccess = FindIndex(key, index);
	if (isSuccess)
	{
		_table[index].condition = eHashTableCondition::TOMBSTONE;
		value = _table[index].value;
		_count--;
	}
	return isSuccess;
}

template<typename K, typename V>
inline bool HashTable<K, V>::RemoveKey(const K& key)
{
	UINT32 index = 0;
	bool isSuccess = FindIndex(key, index);
	if (isSuccess)
	{
		_table[index].condition = eHashTableCondition::TOMBSTONE;
		_count--;
	}
	return isSuccess;
}

template<typename K, typename V>
inline bool HashTable<K, V>::FindValue(const K& key)
{
	UINT32 index = 0;
	return FindIndex(key, index);
}

template<typename K, typename V>
inline UINT32 HashTable<K, V>::GetCapacity()
{
	return PRIMES[_capacityIndex];
}

template<typename K, typename V>
inline bool HashTable<K, V>::IsOccupiedAt(UINT32 index, OUT K& key, OUT V& value)
{
	if(PRIMES[_capacityIndex] <= index) return false;
	
	if (_table[index].condition == eHashTableCondition::OCCUPIED)
	{
		key = _table[index].key;
		value = _table[index].value;
		return true;
	}
	return false;
}
