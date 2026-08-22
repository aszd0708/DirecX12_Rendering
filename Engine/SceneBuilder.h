#pragma once
class SceneBuilder
{
public:
	SceneBuilder(string sceneName);
	~SceneBuilder();

public:
	// 원하는 오브젝트 세팅 및 등등
	virtual void Init();

	virtual void Update();
	virtual void Render();

public:
	virtual void AddGameObject(shared_ptr<GameObject> obj);
	virtual void RemoveGameObject(shared_ptr<GameObject> obj);

public:
	shared_ptr<Scene> GetScene() { return _scene; }

private:
	shared_ptr<Scene> _scene;
};

