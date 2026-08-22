#include "pch.h"
#include "Scene.h"
#include "Renderer.h"
#include "GameObject.h"

Scene::Scene(string _sceneName) : _sceneName(_sceneName)
{

}

Scene::~Scene()
{
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
	for (weak_ptr<Renderer>& renderer : _renderers)
	{
		if (renderer.expired() == false)
		{
			shared_ptr<Renderer> render = renderer.lock();
			render->Render();
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

void Scene::RegisterRenderer(shared_ptr<Renderer> renderer)
{
	int index = -1;
	for (int i = 0 ; i < _renderers.size(); ++i)
	{
		if (_renderers[i].expired() == false)
		{
			shared_ptr<Renderer> curRender = _renderers[i].lock();
			if (curRender == renderer)
			{
				index = i;
				break;
			}
		}
	}
	if (index == -1)
	{
		_renderers.emplace_back(renderer);
	}
}

void Scene::UnregisterRenderer(shared_ptr<Renderer> renderer)
{
	int index = -1;
	for (int i = 0; i < _renderers.size(); ++i)
	{
		if (_renderers[i].expired() == false)
		{
			shared_ptr<Renderer> curRender = _renderers[i].lock();
			if (curRender == renderer)
			{
				index = i;
				break;
			}
		}
	}
	if (index != -1)
	{
		_renderers.erase(_renderers.begin() + index);
	}
}
