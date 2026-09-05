#pragma once
#include "SceneBuilder.h"

class MeshDemo : public SceneBuilder
{
private:
	static const UINT MAX_COUNT = 10;

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

private:
	GameObject* _cameraObj;
	GameObject* _meshObj;

	GameObject** _objs;

	DXGI_QUERY_VIDEO_MEMORY_INFO* _vInfo;



private:
	UINT64 _curUsage = 0;
	UINT64 _endUsage = 0;
	UINT64 _totalTime = 0;
};

