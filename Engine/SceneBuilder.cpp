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
	_scene->RegisterRenderer(memoryEntry);
}

void SceneBuilder::RemoveGameObject(MemoryEntry& memoryEntry)
{
	_scene->UnregisterRenderer(memoryEntry);
	_scene->UnregisterGameObject(memoryEntry);
}
