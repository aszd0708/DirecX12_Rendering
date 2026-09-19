#pragma once

template<typename T>
class Array
{
	const UINT32 DEFAULT_SIZE = 32;

public:
	Array();
	Array(UINT32 count);
	Array(std::initializer_list<T> list);
	Array(Array<T>&& other);
	Array(const Array<T>& value);
	~Array();

public:
	void SetCount(UINT32 count);
	bool Find(const T& value);
	const UINT32& GetCount() { return _count; }
	void* GetData();

	T& operator[](int index);
	const T& operator[](int index) const;

	Array<T>& operator=(Array<T>&& other);
	Array<T>& operator=(const Array<T>& other);

private:
	UINT32 _count = 0;
	T* _array;
};

template<typename T>
inline Array<T>::Array() : Array(DEFAULT_SIZE)
{
	
}

template<typename T>
inline Array<T>::Array(UINT32 count) : _count(count)
{
	assert(count != 0);

	_array = (T*)malloc(sizeof(T) * count);
}

template<typename T>
inline Array<T>::Array(std::initializer_list<T> list)
{
	int count = list.size();
	_array = (T*)malloc(sizeof(T) * count);
	_count = count;

	int i = 0;
	for (const T& value : list)
	{
		_array[i] = value;
		++i;
	}
}

template<typename T>
inline Array<T>::Array(Array<T>&& other) : _count(other._count), _array(other._array)
{
	other._array = nullptr;
}

template<typename T>
inline Array<T>::Array(const Array<T>& other) : _count(other._count)
{
	_array = (T*)malloc(sizeof(T) * _count);
	memcpy(_array, other._array, sizeof(T) * _count);
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
inline void* Array<T>::GetData()
{
	return _array;
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

template<typename T>
inline Array<T>& Array<T>::operator=(Array<T>&& other)
{
	free(_array);
	_count = other._count;
	_array = other._array;
	other._array = nullptr;

	return *this;
}

template<typename T>
inline Array<T>& Array<T>::operator=(const Array<T>& other)
{
	free(_array);
	_count = other._count;
	_array = (T*)malloc(sizeof(T) * _count);
	memcpy(_array, other._array, sizeof(T) * _count);

	return *this;
}
