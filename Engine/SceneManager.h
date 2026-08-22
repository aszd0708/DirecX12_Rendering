#pragma once

class Scene;

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();

public:
	void SetCurrentScene(shared_ptr<Scene> scene);
	
	shared_ptr<Scene> GetCurrentScene();

private:
	shared_ptr<Scene> _curScene;
};

