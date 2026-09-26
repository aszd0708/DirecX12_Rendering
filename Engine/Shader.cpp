#include "pch.h"
#include "Shader.h"
#include "ShaderInfo.h"
#include "ShaderCompiler.h"

Shader::Shader(ShaderInfo info) : _info(info)
{
	_fullPath = SHADER_PATH(info._path);

	const wchar_t* str = _fullPath.c_str();

	ShaderCompilerInfo* compilerInfo = SHADER_COMPILER->GetShaderComplierInfo(0);

	// Vertex Shader
	CreateShader(str, L"VS", L"vs_6_6", _vsBlob, compilerInfo);

	// Pixel Shader
	CreateShader(str, L"PS", L"ps_6_6", _psBlob, compilerInfo);

	CreateRootSignature();
	CreatePSO();
}

Shader::~Shader()
{

}

void Shader::CreateShader(const wchar_t* str, const wchar_t* entryPoint, const wchar_t* targetProfile, ComPtr<IDxcBlob>& blob, ShaderCompilerInfo* info)
{	
	UINT32 codePage = DXC_CP_UTF8;
	ComPtr<IDxcBlobEncoding> blobEncoding;
	ThrowIfFailed(info->GetUtils()->LoadFile(str, &codePage, blobEncoding.GetAddressOf()));

	DxcBuffer source = {};
	source.Ptr = blobEncoding->GetBufferPointer();
	source.Size = blobEncoding->GetBufferSize();
	source.Encoding = codePage;

	LPCWSTR args[] =
	{
		str,
		L"-E", entryPoint,
		L"-T", targetProfile,
#ifdef _DEBUG
		L"-Zi",
		L"-Od",
		L"-Qembed_debug",
#else
		L"-Qstrip_reflect",
#endif // _DEBUG
	};

	ComPtr<IDxcCompiler3> compiler = info->GetCompiler();
	ComPtr<IDxcResult> results;
	ThrowIfFailed(compiler->Compile(&source, args, _countof(args), info->GetHanlder().Get(), IID_PPV_ARGS(results.GetAddressOf())));

	ComPtr<IDxcBlobUtf8> error = nullptr;
	results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error), nullptr);
	if (error != nullptr && error->GetStringLength() > 0)
	{
		OutputDebugStringA(error->GetStringPointer());
	}

	HRESULT hrStatus;
	ThrowIfFailed(results->GetStatus(&hrStatus));
	ThrowIfFailed(hrStatus);

	ThrowIfFailed(results->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&blob), nullptr));
}

void Shader::CreateRootSignature()
{
	CD3DX12_ROOT_SIGNATURE_DESC desc = CD3DX12_ROOT_SIGNATURE_DESC(	
	_info._signatureRootParamCount, _info._signatureRootParam, 
	1, &_info._sampler,
	D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

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

	desc.InputLayout.NumElements = _info._inputLayoutDesc.GetCount();
	desc.InputLayout.pInputElementDescs = &_info._inputLayoutDesc[0];

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
