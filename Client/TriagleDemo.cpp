#include "pch.h"
#include "TriagleDemo.h"

TriagleDemo::TriagleDemo(string sceneName) : SceneBuilder(sceneName)
{

}

TriagleDemo::~TriagleDemo()
{
}

void TriagleDemo::Init()
{
	CreateTriangle();
	CreateResource();
	CreateRootSignature();
	CreatePSO();
}

void TriagleDemo::Render()
{
	COMMAND_LIST->SetGraphicsRootSignature(_signature.Get());
	COMMAND_LIST->SetPipelineState(_pso.Get());
	COMMAND_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	COMMAND_LIST->IASetVertexBuffers(0, 1, &_resourceView);
	COMMAND_LIST->DrawInstanced(3, 1, 0, 0);
}

void TriagleDemo::CreateTriangle()
{
	{
		Vec3 vertex = Vec3(0.0f, 0.5f, 0.0f);
		Vec4 color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
		VertexData data = {vertex, color};
		_vertexData.emplace_back(data);
	}

	{
		Vec3 vertex = Vec3(0.5f, -0.5f, 0.0f);
		Vec4 color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
		VertexData data = { vertex, color };
		_vertexData.emplace_back(data);
	}

	{
		Vec3 vertex = Vec3(-0.5f, -0.5f, 0.0f);
		Vec4 color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
		VertexData data = { vertex, color };
		_vertexData.emplace_back(data);
	}
}

void TriagleDemo::CreateResource()
{
	int vertexSize = _vertexData.size() * VertexData::GetSize();

	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = vertexSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
	&desc, D3D12_RESOURCE_STATE_GENERIC_READ, 
	nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));

	void* data;
	_resource->Map(0, nullptr, &data);
	memcpy(data, _vertexData.data(), vertexSize);
	_resource->Unmap(0, nullptr);

	_resourceView = {};
	_resourceView.BufferLocation = _resource->GetGPUVirtualAddress();
	_resourceView.SizeInBytes = vertexSize;
	_resourceView.StrideInBytes = VertexData::GetSize();

	ThrowIfFailed(D3DCompileFromFile(L"../Shaders/HLSL/Triangle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_SKIP_OPTIMIZATION, 0, _vsBlob.GetAddressOf(), _vsError.GetAddressOf()));
	ThrowIfFailed(D3DCompileFromFile(L"../Shaders/HLSL/Triangle.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_SKIP_OPTIMIZATION, 0, _psBlob.GetAddressOf(), _psError.GetAddressOf()));
}

void TriagleDemo::CreateRootSignature()
{
	CD3DX12_ROOT_SIGNATURE_DESC desc = {};
	desc.pParameters = 0;
	desc.pStaticSamplers = nullptr;
	desc.NumParameters = 0;
	desc.NumStaticSamplers = 0;
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ThrowIfFailed(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, _signatureBlob.GetAddressOf(), _signatureError.GetAddressOf()));

	ThrowIfFailed(DEVICE->CreateRootSignature(0, _signatureBlob->GetBufferPointer(), _signatureBlob->GetBufferSize(), IID_PPV_ARGS(&_signature)));
}

void TriagleDemo::CreatePSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = _signature.Get();

	// VS
	{
		D3D12_SHADER_BYTECODE vsByte = {};
		vsByte.BytecodeLength = _vsBlob->GetBufferSize();
		vsByte.pShaderBytecode = reinterpret_cast<BYTE*>(_vsBlob->GetBufferPointer());
		desc.VS = vsByte;
	}
	
	// PS
	{
		D3D12_SHADER_BYTECODE psByte = {};
		psByte.BytecodeLength = _psBlob->GetBufferSize();
		psByte.pShaderBytecode = reinterpret_cast<BYTE*>(_psBlob->GetBufferPointer());
		desc.PS = psByte;
	}

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC layoutDesc[2];

	// Position
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
		inputElementDesc.SemanticName = "POSITION";
		inputElementDesc.SemanticIndex = 0;
		inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = 0;
		inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;
		layoutDesc[0] = inputElementDesc;
	}
	
	// Color
	{
		D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
		inputElementDesc.SemanticName = "COLOR";
		inputElementDesc.SemanticIndex = 0;
		inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = sizeof(Vec3);
		inputElementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;
		layoutDesc[1] = inputElementDesc;
	}
	desc.InputLayout.NumElements = 2;
	desc.InputLayout.pInputElementDescs = layoutDesc;

	desc.SampleMask = UINT_MAX;
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.DepthStencilState.DepthEnable = false;

	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //스왑체인과 동일
	desc.SampleDesc.Count = 1;

	ThrowIfFailed(DEVICE->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&_pso)));
}
