#pragma once
#include "Geometry.h"

template<typename T>
class Mesh
{
public:
	Mesh(shared_ptr<Geometry<T>> data);
	~Mesh();

public:
	void CreateMesh();

private:
	void CreateVertexResource();
	void CreateIndexResource();

public:
	shared_ptr<Geometry<T>> GetMesh() { return _meshData; }

	ComPtr<ID3D12Resource> GetVertexResource() { return _vertexResource; };
	const D3D12_VERTEX_BUFFER_VIEW GetVertexView() { return _vertexResourceView; }
	ComPtr<ID3D12Resource> GetIndexResource() { return _indexResource; };
	const D3D12_VERTEX_BUFFER_VIEW GetIndexView() { return _indexResourceView; }
	
private:
	shared_ptr<Geometry<T>> _meshData;

	ComPtr<ID3D12Resource> _vertexResource;
	D3D12_VERTEX_BUFFER_VIEW _vertexResourceView;

	ComPtr<ID3D12Resource> _indexResource;
	D3D12_INDEX_BUFFER_VIEW _indexResourceView;
};

template<typename T>
inline Mesh<T>::Mesh(shared_ptr<Geometry<T>> data) : _meshData(data)
{
	CreateMesh();
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	CreateVertexResource();
	CreateIndexResource();
}

template<typename T>
inline void Mesh<T>::CreateVertexResource()
{
	uint32 vertexSize = _meshData->GetVertexTotalSize();

	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = vertexSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_vertexResource.GetAddressOf())));

	void* data;
	_vertexResource->Map(0, nullptr, &data);
	memcpy(data, _meshData->GetVertexData(), vertexSize);
	_vertexResource->Unmap(0, nullptr);

	_vertexResourceView = {};
	_vertexResourceView.BufferLocation = _vertexResource->GetGPUVirtualAddress();
	_vertexResourceView.SizeInBytes = vertexSize;
	_vertexResourceView.StrideInBytes = _meshData->GetVertexSize();
}

template<typename T>
inline void Mesh<T>::CreateIndexResource()
{
	uint32 indexTotalSize = _meshData->GetIndexTotalSize();

	D3D12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = indexTotalSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ThrowIfFailed(DEVICE->CreateCommittedResource(&properties, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(_indexResource.GetAddressOf())));

	void* data;
	_indexResource->Map(0, nullptr, &data);
	memcpy(data, _meshData->GetIndexData(), indexTotalSize);
	_indexResource->Unmap(0, nullptr);

	_indexResourceView = {};
	_indexResourceView.BufferLocation = _indexResource->GetGPUVirtualAddress();
	_indexResourceView.SizeInBytes = indexTotalSize;
	_indexResourceView.Format = DXGI_FORMAT_R32_UINT;
}
