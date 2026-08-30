#include "pch.h"
#include "Scene.h"
#include "Renderer.h"
#include "GameObject.h"

Scene::Scene(string _sceneName) : _sceneName(_sceneName)
{
	_renderList = new MemoryList();
}

Scene::~Scene()
{
	delete _renderList;
}

void Scene::Awake()
{
	for (shared_ptr<GameObject>& obj : _objs)
	{
		if (obj != nullptr)
		{
			obj->Awake();
		}
	}
}

void Scene::Start()
{
	for (shared_ptr<GameObject>& obj : _objs)
	{
		if (obj != nullptr)
		{
			obj->Start();
		}
	}
}

void Scene::Update()
{
	for (shared_ptr<GameObject>& obj : _objs)
	{
		if (obj != nullptr)
		{
			obj->Update();
		}
	}
}

void Scene::LateUpdate()
{
	for (shared_ptr<GameObject>& obj : _objs)
	{
		if (obj != nullptr)
		{
			obj->LateUpdate();
		}
	}
}

void Scene::OnDestory()
{
	for (shared_ptr<GameObject>& obj : _objs)
	{
		if (obj != nullptr)
		{
			obj->OnDestroy();
		}
	}
}

void Scene::Render()
{
	for (int i = 0; i < _renderList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess = _renderList->GetMemoryBlock(i, entity);
		if (isSuccess)
		{
			Renderer* render = nullptr;
			bool isSuccess = CpuPoolManager::GetInstance()->Resolve(entity, render);
			if (isSuccess)
			{
				render->Render();
			}
		}
	}
}

void Scene::RegisterGameObject(shared_ptr<GameObject> obj)
{
	vector<shared_ptr<GameObject>>::iterator iterator = find(_objs.begin(), _objs.end(), obj);
	if (iterator == _objs.end())
	{
		_objs.emplace_back(obj);
		obj->Awake();
		obj->Start();
	}
}

void Scene::UnregisterGameObject(shared_ptr<GameObject> obj)
{
	vector<shared_ptr<GameObject>>::iterator iterator = find(_objs.begin(), _objs.end(), obj);
	if (iterator != _objs.end())
	{
		obj->OnDestroy();
		_objs.erase(iterator);
	}
}

void Scene::RegisterRenderer(MemoryEntry& memoryEntity)
{
	int index = -1;
	for (int i = 0 ; i < _renderList->GetCount(); ++i)
	{
		MemoryEntry entity;
		bool isSuccess =_renderList->GetMemoryBlock(i, entity);
		if (entity == memoryEntity)
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
		if (entity == memoryEntity)
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
