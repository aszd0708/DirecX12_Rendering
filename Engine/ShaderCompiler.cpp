#include "pch.h"
#include "ShaderCompiler.h"

ShaderCompilerInfo::ShaderCompilerInfo(UINT8 index, ComPtr<IDxcUtils> utils, ComPtr<IDxcCompiler3> compiler) : _index(index), _utils(utils), _compiler(compiler)
{
	_utils->CreateDefaultIncludeHandler(_handler.GetAddressOf());
}

ShaderCompilerInfo::~ShaderCompilerInfo()
{
	_utils.Reset();
	_compiler.Reset();
	_handler.Reset();
}

void ShaderCompiler::Init(UINT8 count)
{
	_infos = Array<ShaderCompilerInfo*>(count);
	for (int i = 0; i < count; ++i)
	{
		ComPtr<IDxcUtils> utils;
		ComPtr<IDxcCompiler3> compiler;
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

		ShaderCompilerInfo* info = new ShaderCompilerInfo(i, utils, compiler);
		_infos[i] = info;
	}
}

void ShaderCompiler::Release()
{
	for (int i = 0; i < _infos.GetCount(); ++i)
	{
		delete _infos[i];
	}
}

ComPtr<IDxcCompiler3> ShaderCompiler::GetShaderCompiler(UINT8 index)
{
	assert(_infos.GetCount() > index);

	return _infos[index]->GetCompiler();
}

ShaderCompilerInfo* ShaderCompiler::GetShaderComplierInfo(UINT8 index)
{
	assert(_infos.GetCount() > index);

	return _infos[index];
}

