#include "pch.h"
#include "MeshDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "ShaderInfo.h"

void MeshDemo::Init()
{
	SceneBuilder::Init();
}

void MeshDemo::Render()
{
	SceneBuilder::Render();


}

void MeshDemo::CreateCamera()
{
	_cameraObj = make_shared<GameObject>();
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj);
}

void MeshDemo::CreateMesh()
{
	_meshObj = make_shared<GameObject>();
	shared_ptr<Geometry<VertexTextureData>> geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateCube(geometry);
	shared_ptr<Mesh<VertexTextureData>> mesh = make_shared<Mesh<VertexTextureData>>(geometry);

	ShaderInfo shaderInfo = {};
	shaderInfo._path = L"Mesh.hlsl";
	shaderInfo._inputLayoutDesc = VertexColorData::GetDesc();
	CD3DX12_ROOT_PARAMETER rootParams[1] = { {} };
	rootParams[0].InitAsConstantBufferView(0);
	shaderInfo._signatureRootParam = rootParams;
	shared_ptr<Shader> shader = make_shared<Shader>(shaderInfo);

	shared_ptr<MeshRenderer<VertexTextureData>> meshRednerer = _meshObj->AddComponent<MeshRenderer<VertexTextureData>>();
	meshRednerer->Init(mesh, shader);

	AddGameObject(_cameraObj);
}
