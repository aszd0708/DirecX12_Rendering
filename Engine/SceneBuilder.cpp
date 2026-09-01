#include "pch.h"
#include "SceneBuilder.h"

SceneBuilder::SceneBuilder(string sceneName)
{
	_scene = make_shared<Scene>(sceneName);
}

SceneBuilder::~SceneBuilder()
{

}

void SceneBuilder::Init()
{

}

void SceneBuilder::Update()
{
	_scene->Update();
	_scene->LateUpdate();
}

void SceneBuilder::Render()
{
	_scene->Render();
}

void SceneBuilder::AddGameObject(MemoryEntry& memoryEntry)
{
	_scene->RegisterGameObject(memoryEntry);

	GameObject* obj = nullptr;
	bool isSuccess = CpuPoolManager::GetInstance()->Resolve(memoryEntry, &obj);
	if (isSuccess)
	{
		Renderer* renderer = obj->GetComponent<Renderer>();
		if (renderer != nullptr)
		{
			_scene->RegisterRenderer(renderer->GetMemoryEntry());
		}
	}
}

void SceneBuilder::RemoveGameObject(MemoryEntry& memoryEntry)
{
	_scene->UnregisterRenderer(memoryEntry);

	GameObject* obj = nullptr;
	bool isSuccess = CpuPoolManager::GetInstance()->Resolve(memoryEntry, &obj);
	if (isSuccess)
	{
		Renderer* renderer = obj->GetComponent<Renderer>();
		if (renderer != nullptr)
		{
			_scene->UnregisterGameObject(renderer->GetMemoryEntry());
		}
	}
}
