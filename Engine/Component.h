#pragma once

class GameObject;

enum class eComponentType
{
	Transform, Camera, Light, Renderer, END,
	Script
};

class Component
{
public:
	Component(eComponentType type);
	virtual ~Component();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestory();

public:
	eComponentType GetType() const { return _type; }

	void SetGameObject(shared_ptr<GameObject> obj) { _gameObject.lock() = obj; }

private:
	eComponentType _type;
	weak_ptr<GameObject> _gameObject;
};

