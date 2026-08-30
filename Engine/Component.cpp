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

bool Component::GetGameObject(OUT GameObject** gameObject)
{
	bool isSuccess = CpuPoolManager::GetInstance()->Resolve(_gameObjectMemory, gameObject);
	return isSuccess;
}

bool Component::GetTransform(OUT Transform** transform)
{	
	bool isSuccess = false;
	GameObject* gameObject = nullptr;
	isSuccess = GetGameObject(&gameObject);
	if (isSuccess == false)
	{
		return false;
	}

	*transform = gameObject->GetComponent<Transform>();
	return isSuccess;
}

