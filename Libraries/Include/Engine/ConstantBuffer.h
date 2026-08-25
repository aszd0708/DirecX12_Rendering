#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(uint32 dataSize);
	~ConstantBuffer();

	static uint32 GetSize(uint32 dataSize);

private:
	void CreateBuffer(uint32 dataSize);

public:
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() { return _buffer->GetGPUVirtualAddress(); }
	void PushData(const void* sendData, uint32 dataSize);

	template<typename T>
	void PushDataSafe(const T& sendData);

private:
	ComPtr<ID3D12Resource> _buffer;
	void* _mappedData;
	uint32 _dataSize;
};

template<typename T>
inline void ConstantBuffer::PushDataSafe(const T& sendData)
{
	PushData((void*)&sendData, sizeof(T));
}
