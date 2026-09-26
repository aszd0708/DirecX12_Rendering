#pragma once

class ShaderCompilerInfo
{
public:
	ShaderCompilerInfo(UINT8 index, ComPtr<IDxcUtils> utils, ComPtr<IDxcCompiler3> compiler);
	~ShaderCompilerInfo();

public:
	const ComPtr<IDxcUtils>& GetUtils() { return _utils; }
	const ComPtr<IDxcCompiler3>& GetCompiler() { return _compiler; }
	const ComPtr<IDxcIncludeHandler>& GetHanlder() { return _handler; }

private:
	UINT8 _index;
	ComPtr<IDxcUtils> _utils;
	ComPtr<IDxcCompiler3> _compiler;
	ComPtr<IDxcIncludeHandler> _handler;
};

class ShaderCompiler
{
	DECLARE_SINGLE(ShaderCompiler);

public:
	void Init(UINT8 count);
	void Release();

public:
	ComPtr<IDxcCompiler3> GetShaderCompiler(UINT8 index);
	ShaderCompilerInfo* GetShaderComplierInfo(UINT8 index);

private:
	Array<ShaderCompilerInfo*> _infos;
};

