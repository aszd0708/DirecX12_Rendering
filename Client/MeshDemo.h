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
	void CreateTextureMesh();

private:
	GameObject* _cameraObj;
	GameObject* _meshObj;
	GameObject* _textureMeshObj;
};

