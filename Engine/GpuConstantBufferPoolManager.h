#pragma once

struct GpuConstantBufferHandle;
class GpuConstantBufferPool;

class GpuConstantBufferPoolManager
{
	DECLARE_SINGLE(GpuConstantBufferPoolManager);

public:
	void Init(UINT8 pageCount, UINT64 pageSize);
	void Release();

public:
	bool GetMemory(UINT64 size, OUT GpuConstantBufferHandle& handle);
	const ComPtr<ID3D12Resource>& GetMemoryResource();
	void* GetMappedBase();
	
	// ConstantPool에서 _currentPageIndex의 페이지를 Reset시킴
	void ResetCurrentPage();
	void ResetAllPage();
	void MoveNextPage(ID3D12Fence* fence, UINT64 fenceValue);

private:
	GpuConstantBufferPool* _constantBufferPool;
};

