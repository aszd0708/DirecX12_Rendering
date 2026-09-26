#include "pch.h"
#include "GlobalConstantBuffer.h"
#include "ConstantBuffer.h"

void GlobalConstantBuffer::Init()
{
	_cameraDesc = {};
	_cameraBuffer = new ConstantBuffer(sizeof(GlobalDesc));
}

void GlobalConstantBuffer::Release()
{
	delete _cameraBuffer;
}

D3D12_GPU_VIRTUAL_ADDRESS GlobalConstantBuffer::GetCameraBufferAddress()
{
	assert(_cameraBuffer != nullptr);

	return _cameraBuffer->GetAddress();
}

void GlobalConstantBuffer::PushCameraBuffer(const Matrix& view, const Matrix& projection)
{
	if (_cameraBuffer == nullptr) return;

	_cameraDesc.V = view.Transpose();
	_cameraDesc.P = projection.Transpose();
	_cameraDesc.VP = (view * projection).Transpose();;
	_cameraDesc.VInv = view.Invert().Transpose();

	_cameraBuffer->PushDataSafe<GlobalDesc>(_cameraDesc);
}
