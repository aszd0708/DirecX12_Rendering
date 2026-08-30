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

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Awake();
		}
	}


	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Awake();
		}
	}
}

void GameObject::Start()
{
	bool isSuccess = false;

	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Start();
		}
	}


	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);
		
		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Start();
		}
	}
}

void GameObject::Update()
{
	bool isSuccess = false;

	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Update();
		}
	}


	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	bool isSuccess = false;

	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->LateUpdate();
		}
	}


	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->LateUpdate();
		}
	}
}

void GameObject::OnDestroy()
{
	bool isSuccess = false;

	for (int i = 0; i < _fixedComponentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _fixedComponentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->OnDestroy();
		}
	}


	for (int i = 0; i < _componentList->GetCount(); ++i)
	{
		MemoryEntry memoryEntry;
		isSuccess = _componentList->GetMemoryBlock(i, memoryEntry);
		assert(isSuccess);

		Component* component = nullptr;
		isSuccess = CpuPoolManager::GetInstance()->Resolve<Component>(memoryEntry, component);
		if (isSuccess == true)
		{
			component->OnDestroy();
		}
	}
}
