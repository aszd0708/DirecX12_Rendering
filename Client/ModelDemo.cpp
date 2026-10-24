#include "pch.h"
#include "ModelDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Model.h"
#include "ModelData.h"
#include "ModelInfo.h"
#include "ModelRenderer.h"
#include "ModelLoader.h"

ModelDemo::ModelDemo(string sceneName) : SceneBuilder("ModelDemo")
{
	
}

ModelDemo::~ModelDemo()
{

}

void ModelDemo::Init()
{
	SceneBuilder::Init();

	CreateCamera();
	CreateModel();
}

void ModelDemo::Update()
{
	SceneBuilder::Update();
}

void ModelDemo::Render()
{
	SceneBuilder::Render();
}

void ModelDemo::CreateCamera()
{
	bool isSuccess = GetScene()->CreateGameObject(&_cameraObj);
	assert(isSuccess);
	Transform* transform = _cameraObj->AddComponent<Transform>();
	transform->SetPosition(Vec3(0, 5, -30));
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj->GetMemoryEntry());
}

void ModelDemo::CreateModel()
{
	GameObject* obj = nullptr;
	bool isSuccess = GetScene()->CreateGameObject(&obj);
	assert(isSuccess);

	Transform* transform = obj->GetComponent<Transform>();
	Vec3 position = Vec3(0.0f, 0.0f, 0.0f);
	transform->SetPosition(position);

	RESOURCES->BeginBatch();
	wstring modelPath = L"../Resources/Models/Kachujin/Kachujin.mesh";
	wstring xmlPath = L"../Resources/Models/Kachujin/Kachujin.xml";
	ShaderInfo shaderInfo = {};
	shaderInfo.path = L"Model";
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

	RESOURCES->EndBatch();

	AddGameObject(obj->GetMemoryEntry());
}

void ModelDemo::CreateTextureModel(int index)
{

}

void ModelDemo::DelectTextureModel(int index)
{

}
