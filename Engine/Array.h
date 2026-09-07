#pragma once

template<typename T>
class Array
{
private:

public:
	Array();
	Array(UINT32 count);
	~Array();

public:
	void SetCount(UINT32 count);
	bool Find(const T& value);
	const UINT32& GetCount() { return _count; }

	T& operator[](int index);
	const T& operator[](int index) const;

private:
	UINT32 _count = 0;
	T* _array;
};

template<typename T>
inline Array<T>::Array(UINT32 count) : _count(count)
{
	assert(count != 0);

	_array = (T*)malloc(sizeof(T) * count);
}

template<typename T>
inline Array<T>::~Array()
{
	free(_array);
}

/// <summary>
/// 늘리는것만 가능
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="count"></param>
template<typename T>
inline void Array<T>::SetCount(UINT32 count)
{
	assert(_count <= count);

	T* newArray = (T*)malloc(sizeof(T) * count);
	memmove(newArray, _array, sizeof(T) * _count);
	free(_array);

	_array = newArray;
	_count = count;
}

template<typename T>
inline bool Array<T>::Find(const T & value)
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
inline T& Array<T>::operator[](int index)
{
	assert(index >= 0 && index < _count);
	return _array[index];
}

template<typename T>
inline const T& Array<T>::operator[](int index) const
{
	assert(index >= 0 && index < _count);
	return _array[index];
}
