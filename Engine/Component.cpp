#include "pch.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

Component::Component(eComponentType type) : IMemoryBlockHanlde(), _type(type)
{

}

Component::~Component()
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

void Component::OnDestroy()
{

}

std::shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock();
}

Transform* Component::GetTransform()
{
	return _gameObject.lock()->GetComponent<Transform>();
}

