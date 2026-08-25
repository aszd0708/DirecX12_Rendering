#include "pch.h"
#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _fixedComponent)
	{
		if (component != nullptr)
		{
			component->Awake();
		}
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component != nullptr)
		{
			component->Awake();
		}
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _fixedComponent)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _fixedComponent)
	{
		if (component != nullptr)
		{
			component->Update();
		}
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component != nullptr)
		{
			component->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _fixedComponent)
	{
		if (component != nullptr)
		{
			component->LateUpdate();
		}
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component != nullptr)
		{
			component->LateUpdate();
		}
	}
}

void GameObject::OnDestroy()
{
	for (shared_ptr<Component>& component : _fixedComponent)
	{
		if (component != nullptr)
		{
			component->OnDestory();
		}
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component != nullptr)
		{
			component->OnDestory();
		}
	}
}
