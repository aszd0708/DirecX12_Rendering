#pragma once
#include "ShaderInfo.h"

struct ShaderInfo;
class ShaderCompilerInfo;

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

private:
	void CreateShader(const wchar_t* str, const wchar_t* entryPoint, const wchar_t* targetProfile, ComPtr<IDxcBlob>& blob, ShaderCompilerInfo* info);

public:
	void CreateRootSignature();
	void CreatePSO();

	ComPtr<IDxcBlob> GetVsBlob() { return _vsBlob; }
	ComPtr<IDxcBlob> GetPsBlob() { return _psBlob; }
	ComPtr<ID3D12RootSignature> GetRootSignature() { return _signature; }
	ComPtr<ID3D12PipelineState> GetPSO() { return _pso; }

private:
	ShaderInfo _info;
	wstring _fullPath;

	ComPtr<IDxcBlob> _vsBlob;
	ComPtr<IDxcBlob> _psBlob;

	ComPtr<ID3DBlob> _signatureBlob;
	ComPtr<ID3DBlob> _signatureError;
	ComPtr<ID3D12RootSignature> _signature;

	ComPtr<ID3D12PipelineState> _pso;
public:

	Shader() = default;
};

