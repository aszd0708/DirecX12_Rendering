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

void SceneBuilder::AddGameObject(shared_ptr<GameObject> obj)
{
	_scene->RegisterGameObject(obj);
	shared_ptr<Renderer> render = obj->GetComponent<Renderer>();
	if(render != nullptr)
	{
		_scene->RegisterRenderer(render);
	}
}

void SceneBuilder::RemoveGameObject(shared_ptr<GameObject> obj)
{
	shared_ptr<Renderer> render = obj->GetComponent<Renderer>();
	if (render != nullptr)
	{
		_scene->UnregisterRenderer(render);
	}
	_scene->UnregisterGameObject(obj);
}
