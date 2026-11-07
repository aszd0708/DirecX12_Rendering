#pragma once
#include "SceneBuilder.h"
#include <chrono>

class Animator;

class AnimationDemo : public SceneBuilder
{
private:
	enum class eAnimationClip : UINT8
	{
		Idle = 0,
		Run,
		Slash,

		MAX
	};

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

	UINT8 _idx;

	eAnimationClip _curAnimationClip;
	const char* _clipNames[(UINT8)eAnimationClip::MAX];

	Animator* _testAnimator = nullptr;
};