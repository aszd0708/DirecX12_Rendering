#pragma once

class ConstantBuffer;

class GlobalConstantBuffer
{
	DECLARE_SINGLE(GlobalConstantBuffer);

public:
	void Init();
	void Release();

public:
	D3D12_GPU_VIRTUAL_ADDRESS GetCameraBufferAddress();
	void PushCameraBuffer(const Matrix& view, const Matrix& projection);

private:
	GlobalDesc _cameraDesc;
	ConstantBuffer* _cameraBuffer;
};

