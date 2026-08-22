#pragma once

class Scene
{
public:
	Scene(string sceneName);
	~Scene();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestory();

	virtual void Render();

	void RegisterGameObject(shared_ptr<GameObject> obj);
	void UnregisterGameObject(shared_ptr<GameObject> obj);

	void RegisterRenderer(shared_ptr<Renderer> renderer);
	void UnregisterRenderer(shared_ptr<Renderer> renderer);
	
private:
	string _sceneName;
	int _id;

	vector<shared_ptr<GameObject>> _objs;
	vector<weak_ptr<Renderer>> _renderers;
};

