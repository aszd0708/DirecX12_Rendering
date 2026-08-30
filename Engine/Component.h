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

	shared_ptr<GameObject> GetGameObject();
	Transform* GetTransform();

	void SetGameObject(shared_ptr<GameObject> obj) { _gameObject = obj; }

private:
	eComponentType _type;
	weak_ptr<GameObject> _gameObject;
};

