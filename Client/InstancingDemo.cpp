#include "pch.h"
#include "InstancingDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "ShaderInfo.h"
#include "GpuCommandPool.h"
#include "ShaderInfo.h"
#include <chrono>

InstancingDemo::InstancingDemo(string sceneName) : SceneBuilder(sceneName)
{

}

InstancingDemo::~InstancingDemo()
{

}

void InstancingDemo::Init()
{
	SceneBuilder::Init();

	_vInfo = new DXGI_QUERY_VIDEO_MEMORY_INFO();
	ComPtr<IDXGIAdapter1> adapter;

	FACTORY->EnumAdapters1(0, adapter.GetAddressOf());
	adapter->QueryInterface(IID_PPV_ARGS(&_adapter));

	CreateCamera();

	_objs = (GameObject**)malloc(sizeof(GameObject*) * MAX_COUNT);
}

void InstancingDemo::Update()
{
	SceneBuilder::Update();

	uint32 fps = TIME->GetFps();
	ImGui::LabelText("FPS  ", "%d", fps);
	//ImGui::Text("Not Used GPU Pool");

	ImGui::LabelText("Start Usage  ", "%llu byte", _curUsage);
	ImGui::LabelText("End Usage  ", "%llu byte", _endUsage);

	UINT64 totalUsage = _endUsage - _curUsage;
	ImGui::LabelText("Total Usage  ", "%llu byte", totalUsage);

	ImGui::LabelText("Total Time  ", "%llu ms", _totalTime);

	ImGui::LabelText("Current Count  ", "%d", _objCreatedCount);

	if (INPUT->GetButtonUp(KEY_TYPE::I))
	{
		CreateCallBack();
	}
}

void InstancingDemo::Render()
{
	SceneBuilder::Render();


}

void InstancingDemo::CreateCamera()
{
	bool isSuccess = GetScene()->CreateGameObject(&_cameraObj);
	assert(isSuccess);
	Transform* transform = _cameraObj->AddComponent<Transform>();
	transform->SetPosition(Vec3(50.0f, 50.0f, -15.0f));
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj->GetMemoryEntry());
}

void InstancingDemo::CreateTextureMesh(int index)
{
	GameObject* obj;
	bool isSuccess = GetScene()->CreateGameObject(&obj);
	assert(isSuccess);
	_objs[index] = obj;

	Transform* t = obj->AddComponent<Transform>();
	Vec3 pos = Vec3(rand() % 100, rand() % 100, rand() % 100);
	t->SetPosition(pos);


	RESOURCES->BeginBatch();

	MeshInfo meshInfo = {};
	meshInfo.filePath = L"CubeVertexTextureData";
	meshInfo.geometry = GeometryHelper::CreateSphereVertexTextureData();
	MemoryBlock meshMemoryBlock = {};
	RESOURCES->GetMesh(meshInfo, GpuBufferPoolManager::eBufferPoolID::VERTEX_BUMP, GpuBufferPoolManager::eBufferPoolID::INDEX_BUMP, meshMemoryBlock);

	ShaderInfo shaderInfo = {};
	shaderInfo._path = L"TextureMesh";
	shaderInfo._inputLayoutDesc = meshInfo.geometry.desces;

	MemoryBlock shaderMemoryBlock = {};
	RESOURCES->GetShader(shaderInfo, shaderMemoryBlock);

	MeshRenderer* meshRednerer = obj->AddComponent<MeshRenderer>();

	TextureInfo textureInfo = {};
	textureInfo.filePath = L"../Resources/Leather.jpg";
	textureInfo.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureInfo.mipLevels = 1;

	MemoryBlock textureMemoryBlock = {};
	RESOURCES->GetTexture(textureInfo, textureMemoryBlock);

	meshRednerer->Init(meshMemoryBlock, shaderMemoryBlock, textureMemoryBlock);
	RESOURCES->EndBatch();

	AddGameObject(obj->GetMemoryEntry());
}

void InstancingDemo::DelectTextureMesh(int index)
{
	auto block = _objs[index]->GetMemoryEntry();
	RemoveGameObject(block);
}

void InstancingDemo::CreateCallBack()
{
	auto start = std::chrono::steady_clock::now();
	_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, _vInfo);

	_curUsage = _vInfo->CurrentUsage;

	for (int i = 0; i < 1; ++i)
	{
		if (_objCreatedCount >= MAX_COUNT) break;

		CreateTextureMesh(_objCreatedCount);
		_objCreatedCount++;
	}

	_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, _vInfo);
	_endUsage = _vInfo->CurrentUsage;
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	_totalTime = elapsed.count();
}

void InstancingDemo::DeleteCallBack()
{
	auto start = std::chrono::steady_clock::now();
	_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, _vInfo);

	_curUsage = _vInfo->CurrentUsage;

	for (int i = 0; i < 2; ++i)
	{
		if (_objCreatedCount <= 0) break;
		_objCreatedCount--;

		DelectTextureMesh(_objCreatedCount);
	}

	_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, _vInfo);
	_endUsage = _vInfo->CurrentUsage;
	auto end = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	_totalTime = elapsed.count();
}
