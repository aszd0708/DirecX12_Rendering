#pragma once

template<typename T>
class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

private:
	void CreateResource(const vector<T>& vertices);

private:
	ComPtr<ID3D12Resource> _resource;
	D3D12_VERTEX_BUFFER_VIEW _resourceView;
};

template<typename T>
inline void VertexBuffer<T>::CreateResource(const vector<T>& vertices)
{
	int vertexSize = vertices.size() * sizeof(T);

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

	ThrowIfFailed(DEVICE->CreateCommittedResource(
	&properties, D3D12_HEAP_FLAG_NONE,
	&desc, D3D12_RESOURCE_STATE_GENERIC_READ,
	nullptr, IID_PPV_ARGS(_resource.GetAddressOf())));

	void* data;
	_resource->Map(0, nullptr, &data);
	memcpy(data, vertices.data(), vertexSize);
	_resource->Unmap(0, nullptr);

	_resourceView = {};
	_resourceView.BufferLocation = _resource->GetGPUVirtualAddress();
	_resourceView.SizeInBytes = vertexSize;
	_resourceView.StrideInBytes = sizeof(T);
}
