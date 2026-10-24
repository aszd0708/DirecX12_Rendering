#pragma once
#include "SceneBuilder.h"
class ModelDemo : public SceneBuilder
{
public:
	ModelDemo(string sceneName);
	virtual ~ModelDemo();

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

public:
	void CreateCamera();
	void CreateModel();
	void CreateTextureModel(int index);
	void DelectTextureModel(int index);

private:
	GameObject* _cameraObj;
	GameObject* _modelObj;

	//GameObject** _objs;
};

