#pragma once

struct GpuMaterialBufferHandle
{
	UINT32 poolID;
	UINT32 index;
};

class GpuMaterialBufferPool
{
public:
	GpuMaterialBufferPool(UINT32 poolID, UINT32 totalCount, UINT32 stride);
	~GpuMaterialBufferPool();

private:
	void CreateResource();
	void GetUploadHeap(const D3D12_RESOURCE_DESC& desc, UINT64 size, OUT ComPtr<ID3D12Resource>& uploadResource, OUT GpuMemoryHandle& uploadHandle);

public:
	void Upload(const ComPtr<ID3D12GraphicsCommandList>& commandList, const GpuMaterialBufferHandle& handle, const void* data, UINT64 dataSize, OUT GpuMemoryHandle& memoryHandle);

public:
	bool Alloc(OUT GpuMaterialBufferHandle& handle);
	bool Free(const GpuMaterialBufferHandle& handle);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress() { return _resource->GetGPUVirtualAddress(); }

private:
	UINT32 _poolID;
	UINT32 _totalCount;
	UINT32 _stride;

	Array<bool> _allocableCheckingArray;
	Stack<UINT32> _usableIndex;

	ComPtr<ID3D12Resource> _resource;
	D3D12_RESOURCE_STATES _currentState;
};

