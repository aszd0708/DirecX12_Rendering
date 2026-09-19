#pragma once
#include "GpuBufferPoolManager.h"
#include "MeshInfo.h"

class Mesh : public IMemoryBlockHanlde
{
public:
	Mesh(MeshInfo info, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID);
	virtual ~Mesh();

public:
	void CreateMesh(const ComPtr<ID3D12CommandAllocator>& commandAllocator, const ComPtr<ID3D12GraphicsCommandList>& commandList);

private:
	void CreateVertexResource(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& handle);
	void CreateIndexResource(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& handle);

	void GetUploadHeap(const D3D12_RESOURCE_DESC& dsec, UINT64 size, OUT ComPtr<ID3D12Resource>& uploadResource, OUT GpuMemoryHandle& uploadHandle);

public:
	const D3D12_VERTEX_BUFFER_VIEW GetVertexView() { return _vertexResourceView; }
	const D3D12_INDEX_BUFFER_VIEW GetIndexView() { return _indexResourceView; }

	UINT32 GetIndexCount() { return _meshInfo.geometry.indices.GetCount() / (UINT8)_meshInfo.geometry.indexSize; }

	const MeshInfo& GetMeshInfo() { return _meshInfo; }

	// Mesh Data
private:
	const Array<D3D12_INPUT_ELEMENT_DESC>& GetVertexDesc() { return _meshInfo.geometry.desces; }

private:
	MeshInfo _meshInfo;
	GpuBufferPoolManager::eBufferPoolID _vertexPoolID;
	GpuBufferPoolManager::eBufferPoolID _indexPoolID;

	D3D12_VERTEX_BUFFER_VIEW _vertexResourceView;
	D3D12_INDEX_BUFFER_VIEW _indexResourceView;

	UINT64 _vertexSizeInBytes = 0;
	UINT64 _vertexAlignment = 0;
	UINT64 _indexSizeInBytes = 0;
	UINT64 _indexAlignment = 0;

	GpuBufferHandle _vertexBufferHandle;
	GpuBufferHandle _indexBufferHandle;
};