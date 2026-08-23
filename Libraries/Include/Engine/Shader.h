#pragma once
#include "ShaderInfo.h"

struct ShaderInfo;

class Shader
{
private:
	inline wstring SHADER_PATH(wstring fileName)
	{
		return L"../Shaders/HLSL/" + wstring(fileName) + L".hlsl";
	}

public:
	Shader(ShaderInfo info);
	~Shader();

public:
	void CreateRootSignature();
	void CreatePSO();

	ShaderInfo GetShaderInfo() { return _info; }
	void SetShaderInfo(ShaderInfo info) { _info = info; }

	ComPtr<ID3DBlob> GetVsBlob() { return _vsBlob; }
	ComPtr<ID3DBlob> GetPsBlob() { return _psBlob; }
	ComPtr<ID3D12RootSignature> GetRootSignature() { return _signature; }
	ComPtr<ID3D12PipelineState> GetPSO() { return _pso; }

private:
	ShaderInfo _info;
	wstring _fullPath;

	ComPtr<ID3DBlob> _vsBlob;
	ComPtr<ID3DBlob> _psBlob;

	ComPtr<ID3DBlob> _vsError;
	ComPtr<ID3DBlob> _psError;

	ComPtr<ID3DBlob> _signatureBlob;
	ComPtr<ID3DBlob> _signatureError;
	ComPtr<ID3D12RootSignature> _signature;

	ComPtr<ID3D12PipelineState> _pso;
public:

	Shader() = default;
};

