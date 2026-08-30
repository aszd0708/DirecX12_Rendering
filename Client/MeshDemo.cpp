#include "pch.h"
#include "MeshDemo.h"
#include "Camera.h"
#include "CameraController.h"
#include "Mesh.h"
#include "Texture.h"
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
	CreateTextureMesh();
}

void MeshDemo::Render()
{
	SceneBuilder::Render();


}

void MeshDemo::CreateCamera()
{
	bool isSuccess = GetScene()->CreateGameObject(&_cameraObj);
	assert(isSuccess);
	Transform* transform = _cameraObj->AddComponent<Transform>();
	transform->SetPosition(Vec3(0.0f, 0.0f, -15.0f));
	_cameraObj->AddComponent<Camera>();
	_cameraObj->AddComponent<CameraController>();

	AddGameObject(_cameraObj->GetMemoryEntry());
}

void MeshDemo::CreateMesh()
{
	bool isSuccess = GetScene()->CreateGameObject(&_meshObj);
	assert(isSuccess);
	_meshObj->AddComponent<Transform>();
	shared_ptr<Geometry<VertexColorData>> geometry = make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateCube(geometry, Color(0.79f, 0.66f, 0.79f, 1.0f));
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

	MeshRenderer<VertexColorData>* meshRednerer = _meshObj->AddComponent<MeshRenderer<VertexColorData>>();
	meshRednerer->Init(mesh, shader);

	AddGameObject(_meshObj->GetMemoryEntry());
}

void MeshDemo::CreateTextureMesh()
{
	bool isSuccess = GetScene()->CreateGameObject(&_textureMeshObj);
	assert(isSuccess);
	_textureMeshObj->AddComponent<Transform>();
	shared_ptr<Geometry<VertexTextureData>> geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateCube(geometry);
	shared_ptr<Mesh<VertexTextureData>> mesh = make_shared<Mesh<VertexTextureData>>(geometry);

	shared_ptr<Texture> texture = make_shared<Texture>(L"../Resources/Leather.jpg");


	ShaderInfo shaderInfo = {};
	shaderInfo._path = L"TextureMesh";
	shaderInfo._inputLayoutDesc = VertexTextureData::GetDesc();
	CD3DX12_ROOT_PARAMETER rootParams[3] = { {} };
	for (int i = 0; i < 2; ++i)
	{
		rootParams[i].InitAsConstantBufferView(i);
	}

	CD3DX12_DESCRIPTOR_RANGE srvRange;
	srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);   // SRV 1개, register t0
	rootParams[2].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL);

	shaderInfo._signatureRootParam = rootParams;
	shaderInfo._signatureRootParamCount = 3;
	shaderInfo._sampler = CD3DX12_STATIC_SAMPLER_DESC(0);

	shared_ptr<Shader> shader = make_shared<Shader>(shaderInfo);

	MeshRenderer<VertexTextureData>* meshRednerer = _textureMeshObj->AddComponent<MeshRenderer<VertexTextureData>>();
	meshRednerer->Init(mesh, shader, texture);

	AddGameObject(_textureMeshObj->GetMemoryEntry());
}
