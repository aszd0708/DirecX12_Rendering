#include "pch.h"
#include "SceneManager.h"

void SceneManager::Update()
{
	if (_curScene == nullptr) return;

	_curScene->Update();
	_curScene->LateUpdate();
}

void SceneManager::Render()
{
	if (_curScene == nullptr) return;

	_curScene->Render();
}

void SceneManager::SetCurrentScene(shared_ptr<Scene> scene)
{
	_curScene = scene;
}

shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return _curScene;
}
