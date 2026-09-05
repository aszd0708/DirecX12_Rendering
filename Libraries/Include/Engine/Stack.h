#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

template<typename T>
class Stack
{
private:
	const UINT32 DEFAULT_CAPACITY = 32;

public:
	Stack();
	Stack(UINT32 capacity);
	~Stack();

private:
	void SetCapacity(UINT32 capacity);

public:
	void Push(T data);
	bool Pop(OUT T& data);
	bool Peak(OUT T& data);

	void Clear();

	const UINT32& GetCount() { return _count; }

private:
	UINT32 _capacity;
	UINT32 _count;
	T* _stack;
};

template<typename T>
inline Stack<T>::Stack() : Stack(DEFAULT_CAPACITY)
{

}

template<typename T>
inline Stack<T>::Stack(UINT32 capacity) : _capacity(capacity)
{
	_stack = (T*)malloc(sizeof(T) * capacity);
	_count = 0;
}

template<typename T>
inline Stack<T>::~Stack()
{
	free(_stack);
}

template<typename T>
inline void Stack<T>::SetCapacity(UINT32 capacity)
{
	T* newStack = (T*)malloc(sizeof(T) * capacity);
	memmove(newStack, _stack, sizeof(T) * _capacity);
	free(_stack);
	
	_stack = newStack;
	_capacity = capacity;
}

template<typename T>
inline void Stack<T>::Push(T data)
{
	if (_count >= _capacity)
	{
		SetCapacity(_capacity * 2);
	}
	_stack[_count] = data;
	_count++;
}

template<typename T>
inline bool Stack<T>::Pop(OUT T& data)
{
	bool isSuccess = Peak(data);
	if (isSuccess)
	{
		_count--;
	}
	return isSuccess;
}

template<typename T>
inline bool Stack<T>::Peak(OUT T& data)
{
	if (_count <= 0)
	{
		return false;
	}

	data = _stack[_count - 1];
	return true;
}

template<typename T>
inline void Stack<T>::Clear()
{
	_count = 0;
}
