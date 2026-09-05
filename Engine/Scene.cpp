#include "pch.h"
#include "Scene.h"
#include "Renderer.h"
#include "GameObject.h"

Scene::Scene(string _sceneName) : _sceneName(_sceneName)
{
	_objList = new MemoryList();
	_deletedObjs = new MemoryList();
	_renderList = new MemoryList();
}

Scene::~Scene()
{
	delete _objList;
	delete _deletedObjs;
	delete _renderList;
}

void Scene::DeleteObjs()
{	
	for (int i = 0; i < _deletedObjs->GetCount(); ++i)
	{
		MemoryEntry entry;
		bool isSuccess = _deletedObjs->GetMemoryBlock(i, entry);
		if (isSuccess)
		{
			_objList->RemoveUnordered(entry.block);
			_renderList->RemoveUnordered(entry.block);
		}
	}
}

void Scene::Awake()
{
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _objList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			GameObject* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->Awake();
			}
		}
	}
}

void Scene::Start()
{
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _objList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			GameObject* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->Start();
			}
		}
	}
}

void Scene::Update()
{
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _objList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			GameObject* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->Update();
			}
		}
	}
}

void Scene::LateUpdate()
{
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _objList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			GameObject* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->LateUpdate();
			}
		}
	}

	DeleteObjs();
}

void Scene::OnDestory()
{
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _objList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			GameObject* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->OnDestroy();
			}
		}
	}
}

void Scene::Render()
{
	ID3D12DescriptorHeap* descHeap = DESC_POOL->GetDescriptorHeapAllocator(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	if (descHeap != nullptr)
	{
		COMMAND_LIST->SetDescriptorHeaps(1, &descHeap);
	}
	for (int i = 0; i < _renderList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _renderList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			Renderer* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, &render);
			if (isSuccess)
			{
				render->Render();
			}
		}
	}
}

void Scene::RegisterGameObject(MemoryEntry& objMemory)
{
	bool found = false;
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entry;
		bool isSuccess = _objList->GetMemoryBlock(i, entry);
		if (isSuccess)
		{
			if (entry == objMemory)
			{
				found = true;
				break;
			}
		}
	}
	if (found == false)
	{
		_objList->Add(objMemory);
		GameObject* obj = nullptr;
		bool isSuccess = CpuPoolManager::GetInstance()->Resolve(objMemory, &obj);
		assert(isSuccess);
		obj->Awake();
		obj->Start();
	}
}

void Scene::UnregisterGameObject(MemoryEntry& objMemory)
{
	bool found = false;
	for (int i = 0; i < _objList->GetCount(); ++i)
	{
		MemoryEntry entry;
		bool isSuccess = _objList->GetMemoryBlock(i, entry);
		if (isSuccess)
		{
			if (entry == objMemory)
			{
				found = true;
				break;
			}
		}
	}
	if (found == true)
	{
		_deletedObjs->Add(objMemory);
	}
}

void Scene::RegisterRenderer(MemoryEntry& memoryEntity)
{
	int index = -1;

	for (int i = 0 ; i < _renderList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess =_renderList->GetMemoryBlock(i, entity);
		if (isSuccess && entity == memoryEntity)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		_renderList->Add(memoryEntity);
	}
}

void Scene::UnregisterRenderer(MemoryEntry& memoryEntity)
{
	int index = -1;
	for (int i = 0 ; i < _renderList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess =_renderList->GetMemoryBlock(i, entity);
		if (isSuccess && entity == memoryEntity)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		_renderList->RemoveAtUnordered(index);
	}
}

bool Scene::CreateGameObject(OUT GameObject** obj)
{
	UINT8 poolID;
	bool isSuccess = CpuPoolManager::GetInstance()->GetPoolID(sizeof(GameObject), poolID);
	if (isSuccess == false)
	{
		return isSuccess;
	}

	CpuMemoryPool* pool = CpuPoolManager::GetInstance()->GetMemoryPool(poolID);

	isSuccess = pool->GetMemory(obj);
	return isSuccess;
}
