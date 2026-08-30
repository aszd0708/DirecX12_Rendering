#pragma once

class Scene
{
public:
	Scene(string sceneName);
	~Scene();

private:
	void DeleteObjs();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestory();

	virtual void Render();

	void RegisterGameObject(MemoryEntry& objMemory);
	void UnregisterGameObject(MemoryEntry& objMemory);

	void RegisterRenderer(MemoryEntry& memoryEntry);
	void UnregisterRenderer(MemoryEntry& memoryEntry);
	
	bool CreateGameObject(OUT GameObject** obj);

private:
	string _sceneName;
	int _id;

	MemoryList* _objList;
	// 한 프레임 이후에 지우는 로직을 위한 오브젝트들
	MemoryList* _deletedObjs;


	MemoryList* _renderList;
};

