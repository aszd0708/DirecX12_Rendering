#include "pch.h"
#include "ConstantBuffer.h"
#include "GpuConstantBufferPoolManager.h"

ConstantBuffer::ConstantBuffer(UINT32 dataSize) : _dataSize(dataSize)
{
	_handle = {};
}

ConstantBuffer::~ConstantBuffer()
{
	
}

void ConstantBuffer::CreateBuffer(UINT32 dataSize)
{
	GPU_CONSTNAT_POOL->GetMemory(dataSize, _handle);
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetAddress()
{
	return GPU_CONSTNAT_POOL->GetMemoryResource()->GetGPUVirtualAddress() + _handle.offset;
}

void ConstantBuffer::PushData(const void* sendData, UINT32 dataSize, OUT D3D12_GPU_VIRTUAL_ADDRESS& address)
{
	CreateBuffer(dataSize);
	address = GetAddress();
	CHAR* startOffset = (CHAR*)GPU_CONSTNAT_POOL->GetMappedBase() + _handle.offset;
	memcpy(startOffset, sendData, dataSize);
}
