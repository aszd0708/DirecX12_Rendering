#pragma once

struct GpuConstantBufferHandle;

class ConstantBuffer
{
public:
	ConstantBuffer(UINT32 dataSize);
	~ConstantBuffer();

	static UINT32 GetSize(UINT32 dataSize);

private:
	void CreateBuffer(UINT32 dataSize);

public:
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress();
	void PushData(const void* sendData, UINT32 dataSize, OUT D3D12_GPU_VIRTUAL_ADDRESS& address);

	template<typename T>
	void PushDataSafe(const T& sendData, OUT D3D12_GPU_VIRTUAL_ADDRESS& address);

private:
	void* _mappedData;
	UINT32 _dataSize;

	GpuConstantBufferHandle _handle;
};

template<typename T>
inline void ConstantBuffer::PushDataSafe(const T& sendData, OUT D3D12_GPU_VIRTUAL_ADDRESS& address)
{
	PushData((void*)&sendData, sizeof(T), address);
}
