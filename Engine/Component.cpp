#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

Component::Component(eComponentType type) : _type(type)
{

}

void Component::Awake()
{

}

void Component::Start()
{

}

void Component::Update()
{

}

void Component::LateUpdate()
{

}

void Component::OnDestory()
{

}

std::shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

std::shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetComponent<Transform>();
}

