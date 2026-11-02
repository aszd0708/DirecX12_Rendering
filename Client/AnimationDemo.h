#pragma once
#include "SceneBuilder.h"
#include <chrono>

class AnimationDemo : public SceneBuilder
{
public:
	AnimationDemo(string sceneName);
	virtual ~AnimationDemo();

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
};