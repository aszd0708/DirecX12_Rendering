#pragma once
struct ShaderInfo
{
	wstring _path;
	vector<D3D12_INPUT_ELEMENT_DESC> _inputLayoutDesc;

	D3D12_ROOT_PARAMETER* _signatureRootParam;
};

