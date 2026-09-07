#pragma once

template<typename T>
class DynamicArray
{
private:
	const UINT32 DEFAULT_CAPACITY = 32;

public:
	DynamicArray();
	DynamicArray(UINT32 capacity);
	~DynamicArray();

private:
	void SetCapacity(UINT32 capacity);

public:
	void Add(const T& value);
	bool Remove(const T& value);
	bool RemoveAt(UINT32 index);
	bool Find(const T& value);
	const UINT32& GetCount() { return _count; }

	T& operator[](int index);
	const T& operator[](int index) const;

private:
	UINT32 _capacity = 0;
	UINT32 _count = 0;
	T* _array;
};

template<typename T>
inline DynamicArray<T>::DynamicArray() : DynamicArray(DEFAULT_CAPACITY)
{

}

template<typename T>
inline DynamicArray<T>::DynamicArray(UINT32 capacity) : _capacity(capacity)
{
	assert(capacity != 0);

	_array = (T*)malloc(sizeof(T) * capacity);
	_count = 0;
}

template<typename T>
inline DynamicArray<T>::~DynamicArray()
{
	free(_array);
}

template<typename T>
inline void DynamicArray<T>::SetCapacity(UINT32 capacity)
{
	T* newArray = (T*)malloc(sizeof(T) * capacity);
	memmove(newArray, _array, sizeof(T) * _capacity);
	free(_array);

	_array = newArray;
	_capacity = capacity;
}

template<typename T>
inline void DynamicArray<T>::Add(const T& value)
{
	if (_count >= _capacity)
	{
		SetCapacity(_capacity * 2);
	}

	_array[_count] = value;
	_count++;
}

/// <summary>
/// 쓰기 전 후로 포인터 일 시, delete 호출 필요
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="value"></param>
/// <returns></returns>
template<typename T>
inline bool DynamicArray<T>::Remove(const T& value)
{
	for (UINT32 i = 0; i < _count; ++i)
	{
		if (_array[i] == value)
		{
			memmove(&_array[i], &_array[i+1], sizeof(T) * (_count - i - 1));
			_count--;
			return true;
		}
	}

	return false;
}

/// <summary>
/// 쓰기 전 후로 포인터 일 시, delete 호출 필요
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="value"></param>
/// <returns></returns>
template<typename T>
inline bool DynamicArray<T>::RemoveAt(UINT32 index)
{
	if (index >= _count)
	{
		return false;
	}

	memmove(&_array[index], &_array[index + 1], sizeof(T) * (_count - index - 1));
	_count--;
	return true;
}

template<typename T>
inline bool DynamicArray<T>::Find(const T& value)
{
	for (int i = 0; i < _count; ++i)
	{
		if (_array[i] == value)
		{
			return true;
		}
	}

	return false;
}

template<typename T>
inline T& DynamicArray<T>::operator[](int index)
{
	assert(index >= 0 && index < _count); 
	return _array[index];
}

template<typename T>
inline const T& DynamicArray<T>::operator[](int index) const
{
	assert(index >= 0 && index < _count);
	return _array[index];
}
