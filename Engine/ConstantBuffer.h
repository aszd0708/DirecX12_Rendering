#pragma once

struct GpuConstantBufferHandle;

class ConstantBuffer
{
public:
	ConstantBuffer(UINT32 dataSize);
	~ConstantBuffer();

private:
	void CreateBuffer(UINT32 dataSize);

public:
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress();
	void PushDataWithGetAddress(const void* sendData, UINT32 dataSize, OUT D3D12_GPU_VIRTUAL_ADDRESS& address);
	void PushData(const void* sendData, UINT32 dataSize);

	template<typename T>
	void PushDataSafe(const T& sendData, OUT D3D12_GPU_VIRTUAL_ADDRESS& address);
	template<typename T>
	void PushDataSafe(const T& sendData);

private:
	UINT32 _dataSize;

	GpuConstantBufferHandle _handle;
};

template<typename T>
inline void ConstantBuffer::PushDataSafe(const T& sendData, OUT D3D12_GPU_VIRTUAL_ADDRESS& address)
{
	PushDataWithGetAddress((void*)&sendData, sizeof(T), address);
}

template<typename T>
inline void ConstantBuffer::PushDataSafe(const T& sendData)
{
	PushData((void*)&sendData, sizeof(T));
}
