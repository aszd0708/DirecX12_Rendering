#include "pch.h"
#include "MeshDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "ShaderInfo.h"

MeshDemo::MeshDemo(string sceneName) : SceneBuilder(sceneName)
{}

MeshDemo::~MeshDemo()
{}

void MeshDemo::Init()
{
	SceneBuilder::Init();

	CreateCamera();
	CreateMesh();
}

void MeshDemo::Render()
{
	SceneBuilder::Render();


}

void MeshDemo::CreateCamera()
{
	_cameraObj = make_shared<GameObject>();
	shared_ptr<Transform> transform = _cameraObj->AddComponent<Transform>();
	transform->SetPosition(Vec3(0.0f, 0.0f, -15.0f));
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj);
}

void MeshDemo::CreateMesh()
{
	_meshObj = make_shared<GameObject>();
	_meshObj->AddComponent<Transform>();
	shared_ptr<Geometry<VertexColorData>> geometry = make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateSphere(geometry, Color(0.0f, 0.0f, 0.0f, 1.0f));
	shared_ptr<Mesh<VertexColorData>> mesh = make_shared<Mesh<VertexColorData>>(geometry);

	ShaderInfo shaderInfo = {};
	shaderInfo._path = L"Mesh";
	shaderInfo._inputLayoutDesc = VertexColorData::GetDesc();
	CD3DX12_ROOT_PARAMETER rootParams[2] = { {} };
	rootParams[0].InitAsConstantBufferView(0);
	rootParams[1].InitAsConstantBufferView(1);
	shaderInfo._signatureRootParam = rootParams;
	shaderInfo._signatureRootParamCount = 2;
	shared_ptr<Shader> shader = make_shared<Shader>(shaderInfo);

	shared_ptr<MeshRenderer<VertexColorData>> meshRednerer = _meshObj->AddComponent<MeshRenderer<VertexColorData>>();
	meshRednerer->Init(mesh, shader);

	AddGameObject(_meshObj);
}
