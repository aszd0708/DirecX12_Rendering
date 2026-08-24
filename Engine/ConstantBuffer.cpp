#include "pch.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(uint32 dataSize) : _dataSize(dataSize)
{
	CreateBuffer(dataSize);
}

ConstantBuffer::~ConstantBuffer()
{
	if (_buffer != nullptr)
	{
		_buffer->Unmap(0, nullptr);
	}
}

uint32 ConstantBuffer::GetSize(uint32 dataSize)
{
	// 무조건 ConstantBuffer 크기는 256 단위
	return (dataSize + 255) & ~255;
}

void ConstantBuffer::CreateBuffer(uint32 dataSize)
{
	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = GetSize(dataSize);
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(
		&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_buffer.GetAddressOf())));

	_buffer->Map(0, nullptr, &_mappedData);
}

void ConstantBuffer::PushData(const void* sendData, uint32 dataSize)
{
	memcpy(_mappedData, sendData, dataSize);
}
