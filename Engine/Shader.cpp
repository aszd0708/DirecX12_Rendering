#include "pch.h"
#include "Shader.h"
#include "ShaderInfo.h"

Shader::Shader(ShaderInfo info) : _info(info)
{
	_fullPath = SHADER_PATH(info._path);

	ThrowIfFailed(D3DCompileFromFile(_fullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_SKIP_OPTIMIZATION, 0, _vsBlob.GetAddressOf(), _vsError.GetAddressOf()));
	ThrowIfFailed(D3DCompileFromFile(_fullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_SKIP_OPTIMIZATION, 0, _psBlob.GetAddressOf(), _psError.GetAddressOf()));

	CreateRootSignature();
	CreatePSO();
}

Shader::~Shader()
{

}

void Shader::CreateRootSignature()
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

void Shader::CreatePSO()
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

	vector< D3D12_INPUT_ELEMENT_DESC> desces = _info._inputLayoutDesc;
	desc.InputLayout.NumElements = desces.size();
	desc.InputLayout.pInputElementDescs = desces.data();

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
