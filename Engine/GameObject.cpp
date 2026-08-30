#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "CpuPoolManager.h"

GameObject::GameObject()
{
	_fixedComponentList = new MemoryList((int)eComponentType::END);
	_componentList = new MemoryList();
}

GameObject::~GameObject()
{
	bool isSuccess = false;
	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);
		pool->ReleaseMemory(memoryEntry.block);
	}
	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);
		pool->ReleaseMemory(memoryEntry.block);
	}
	delete _fixedComponentList;
	delete _componentList;
}

void GameObject::Awake()
{
	bool isSuccess = false;
	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);
		Component* component = nullptr;
		isSuccess = pool->GetObjectByMemoryBlock<Component>(memoryEntry.block, &component);
		assert(isSuccess);
		component->Awake();
	}

	bool isSuccess = false;
	for (int i = 0; i < _componentsList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _components->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(memoryEntry.block._poolID);
		Component* component = nullptr;
		isSuccess = pool->GetObjectByMemoryBlock<Component>(memoryEntry.block, &component);
		assert(isSuccess);
		component->Awake();
	}
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
