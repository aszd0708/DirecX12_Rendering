#pragma once
#include "CpuMemoryPoolInterfaces.h"

class GameObject;
class Transform;

enum class eComponentType
{
	Transform, Camera, Light, Renderer, END,
	Script
};

class Component : public IMemoryBlockHanlde
{
public:
	Component(eComponentType type);
	virtual ~Component();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestroy();

public:
	eComponentType GetType() const { return _type; }

	bool GetGameObject(OUT GameObject** gameObject);
	bool GetTransform(OUT Transform** transform);

	void SetGameObject(MemoryEntry& gameObjectMemory) { _gameObjectMemory = gameObjectMemory; }

private:
	eComponentType _type;

	MemoryEntry _gameObjectMemory;
};

