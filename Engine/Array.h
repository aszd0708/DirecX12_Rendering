#pragma once

template<typename T>
class Array
{
	static const UINT32 DEFAULT_SIZE = 0;

public:
	Array();
	Array(UINT32 count);
	Array(std::initializer_list<T> list);
	Array(Array<T>&& other);
	Array(const Array<T>& value);
	~Array();

private:
	void DestroyAllValue();

public:
	void SetCount(UINT32 count);
	bool Find(const T& value);
	UINT32 GetCount() const  { return _count; }
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
	_array = (T*)malloc(sizeof(T) * count);

	if constexpr (!std::is_trivially_copyable_v<T>)
	{
		for (UINT32 i = 0; i < count; ++i)
		{
			new (_array + i)T();
		}
	}
}

template<typename T>
inline Array<T>::Array(std::initializer_list<T> list)
{
	size_t count = list.size();
	_array = (T*)malloc(sizeof(T) * count);
	_count = (UINT32)count;

	int i = 0;
	constexpr  bool isTriviallyCopyableValue = std::is_trivially_copyable_v<T>;
	for (const T& value : list)
	{
		if (!isTriviallyCopyableValue)
		{
			new (_array + i)T(value);
		}
		else
		{
			_array[i] = value;
		}
		++i;
	}
}

template<typename T>
inline Array<T>::Array(Array<T>&& other) : _count(other._count), _array(other._array)
{
	other._array = nullptr;
	other._count = 0;
}

template<typename T>
inline Array<T>::Array(const Array<T>& other) : _count(other._count)
{
	_array = (T*)malloc(sizeof(T) * _count);

	if constexpr (std::is_trivially_copyable_v<T>)
	{
		std::memcpy(_array, other._array, sizeof(T) * _count);
	}
	else
	{
		for (UINT32 i = 0; i < _count; ++i)
		{
			new (_array + i)T(other[i]);
		}
	}
}

template<typename T>
inline Array<T>::~Array()
{
	DestroyAllValue();

	free(_array);
}

template<typename T>
inline void Array<T>::DestroyAllValue()
{
	if constexpr (!std::is_trivially_destructible_v<T>)
	{
		for (UINT32 i = 0; i < _count; ++i)
		{
			_array[i].~T();
		}
	}
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
	if constexpr (std::is_trivially_destructible_v<T>)
	{
		std::memmove(newArray, _array, sizeof(T) * _count);
	}
	else
	{
		for (UINT32 i = 0; i < _count; ++i)
		{
			new (newArray + i)T(std::move(_array[i]));
			_array[i].~T();
		}
		for (UINT32 i = _count; i < count; ++i)
		{
			new (newArray + i)T();
		}
	}

	free(_array);

	_array = newArray;
	_count = count;
}

template<typename T>
inline bool Array<T>::Find(const T & value)
{
	for (UINT32 i = 0; i < _count; ++i)
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
	if (this == &other) 
	{
		return *this;
	}

	DestroyAllValue();

	free(_array);
	_count = other._count;
	_array = other._array;
	other._array = nullptr;
	other._count = 0;

	return *this;
}

template<typename T>
inline Array<T>& Array<T>::operator=(const Array<T>& other)
{
	if (this == &other)
	{
		return *this;
	}

	DestroyAllValue();

	free(_array);
	_count = other._count;
	_array = (T*)std::malloc(sizeof(T) * _count);

	if constexpr (std::is_trivially_destructible_v<T>)
	{
		std::memcpy(_array, other._array, sizeof(T) * _count);
	}
	else
	{
		for (UINT32 i = 0; i < _count; ++i)
		{
			new (_array + i)T(other[i]);
		}
	}

	return *this;
}
