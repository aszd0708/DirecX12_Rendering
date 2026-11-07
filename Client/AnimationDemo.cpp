#include "pch.h"
#include "AnimationDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Model.h"
#include "ModelData.h"
#include "ModelInfo.h"
#include "ModelRenderer.h"
#include "ModelLoader.h"
#include "Animator.h"
#include "Animation.h"
#include "AnimationInfo.h"
#include "InstancingController.h"

AnimationDemo::AnimationDemo(string sceneName) : SceneBuilder("ModelDemo")
{

}

AnimationDemo::~AnimationDemo()
{

}

void AnimationDemo::Init()
{
	SceneBuilder::Init();
	_idx = 0;

	CreateCamera();

	for (int i = 0; i < 5; ++i)
	{
		CreateModel();
	}

	_clipNames[(UINT8)eAnimationClip::Idle] = "Idle";
	_clipNames[(UINT8)eAnimationClip::Run] = "Run";
	_clipNames[(UINT8)eAnimationClip::Slash] = "Slash";
	_curAnimationClip = eAnimationClip::Idle;
}
 
void AnimationDemo::Update()
{
	SceneBuilder::Update();

	ImGui::Checkbox("Instancing", &InstancingController::TURN_ON_INSTANCING);
	ImGui::LabelText("Current Draw Call  ", "%d", InstancingController::DRAW_CALL_COUNT);
	
	INT clipIdx = (INT)_curAnimationClip;
	if (ImGui::Combo("Animation Clip", &clipIdx, _clipNames, (INT)eAnimationClip::MAX))
	{
		_curAnimationClip = (eAnimationClip)clipIdx;
		_testAnimator->SetAnimationIndex((UINT8)_curAnimationClip);
	}
}

void AnimationDemo::Render()
{
	SceneBuilder::Render();
}

void AnimationDemo::CreateCamera()
{
	bool isSuccess = GetScene()->CreateGameObject(&_cameraObj);
	assert(isSuccess);
	Transform* transform = _cameraObj->AddComponent<Transform>();
	transform->SetPosition(Vec3(0, 5, -30));
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj->GetMemoryEntry());
}

void AnimationDemo::CreateModel()
{
	GameObject* obj = nullptr;
	bool isSuccess = GetScene()->CreateGameObject(&obj);
	assert(isSuccess);

	Transform* transform = obj->GetComponent<Transform>();
	Vec3 position = Vec3(rand() % 100, 0.0f, rand() % 100);
	transform->SetPosition(position);
	transform->SetScale(Vec3(0.1f, 0.1f, 0.1f));

	RESOURCES->BeginBatch();
	wstring modelPath = L"../Resources/Models/Kachujin/Kachujin.mesh";
	wstring xmlPath = L"../Resources/Models/Kachujin/Kachujin.xml";
	ShaderInfo shaderInfo = {};
	shaderInfo.path = L"ModelAnimation";
	shaderInfo.inputLayoutDesc = ModelVertexType::GetDesc();

	ModelInfo modelInfo = {};
	modelInfo.modelFilePath = modelPath;
	modelInfo.materialFilePath = xmlPath;
	modelInfo.shaderInfo = shaderInfo;

	MemoryBlock modelMemBlock = {};
	isSuccess = RESOURCES->GetModel(modelInfo, modelMemBlock);
	assert(isSuccess);

	ModelRenderer* renderer = obj->AddComponent<ModelRenderer>();
	renderer->Init(modelMemBlock);

	Animator* animator = obj->AddComponent<Animator>();
	animator->SetModel(modelMemBlock);
	animator->SetTime((float)_idx);
	_testAnimator = animator;

	for (UINT8 i = 0; i < (UINT8)eAnimationClip::MAX; ++i)
	{
		AnimationInfo animationInfo = {};
		switch (i)
		{
		case (UINT8)eAnimationClip::Idle:
			animationInfo.filePath = L"../Resources/Models/Kachujin/Idle.clip";
			break;
		case (UINT8)eAnimationClip::Run:
			animationInfo.filePath = L"../Resources/Models/Kachujin/Slash.clip";
			break;
		case (UINT8)eAnimationClip::Slash:
			animationInfo.filePath = L"../Resources/Models/Kachujin/Run.clip";
			break;
		}

		MemoryBlock animationBlock = {};
		isSuccess = RESOURCES->GetAnimation(animationInfo, animationBlock);
		assert(isSuccess);
		animator->AddAnimationClip(animationBlock);
	}

	RESOURCES->EndBatch();

	AddGameObject(obj->GetMemoryEntry());
}

void AnimationDemo::CreateTextureModel(int index)
{
	
}

void AnimationDemo::DelectTextureModel(int index)
{
	
}
