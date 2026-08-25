#pragma once
#include "SceneBuilder.h"

class MeshDemo : public SceneBuilder
{
public:
	MeshDemo(string sceneName);
	virtual ~MeshDemo();

public:
	virtual void Init() override;
	virtual void Render() override;

public:
	void CreateCamera();
	void CreateMesh();

private:
	shared_ptr<GameObject> _cameraObj;
	shared_ptr<GameObject> _meshObj;
};

