#pragma once
#include "SceneBuilder.h"
#include <chrono>

class MeshDemo : public SceneBuilder
{
private:
	static const UINT MAX_COUNT = 64;

public:
	MeshDemo(string sceneName);
	virtual ~MeshDemo();

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

public:
	void CreateCamera();
	void CreateMesh();
	void CreateTextureMesh(int index);
	void DelectTextureMesh(int index);

private:
	void CreateCallBack();
	void DeleteCallBack();

private:
	GameObject* _cameraObj;
	GameObject* _meshObj;

	GameObject** _objs;

	DXGI_QUERY_VIDEO_MEMORY_INFO* _vInfo;

	ComPtr<IDXGIAdapter3> _adapter;

private:
	UINT64 _curUsage = 0;
	UINT64 _endUsage = 0;
	UINT64 _totalTime = 0;

	UINT32 _objCreatedCount = 0;

	bool _isTestDone = false;
	bool _testBoolean = false;
	bool _testDecrease = false;


	std::chrono::steady_clock::time_point _increaseStart;
	std::chrono::steady_clock::time_point _deleteStart;
	UINT64 _totalIncreaseTime = 0;
	UINT64 _totalDeleteTime = 0;
};

