#include "pch.h"
#include "Mesh.h"
#include "GpuMemoryPoolManager.h"
#include "GpuBufferPoolManager.h"

Mesh::Mesh(MeshInfo info, GpuBufferPoolManager::eBufferPoolID vertexPoolID, GpuBufferPoolManager::eBufferPoolID indexPoolID) : 
_meshInfo(move(info)), _vertexPoolID(vertexPoolID), _indexPoolID(indexPoolID)
{

}

Mesh::~Mesh()
{
	switch (_vertexBufferHandle.poolID)
	{
	case (UINT8)GpuBufferPoolManager::eBufferPoolID::VERTEX_DYNAMIC:
		GPU_BUFFER_POOL->ReleaseMemory(_vertexBufferHandle);
		break;
	}

	switch (_indexBufferHandle.poolID)
	{
	case (UINT8)GpuBufferPoolManager::eBufferPoolID::INDEX_DYNAMIC:
		GPU_BUFFER_POOL->ReleaseMemory(_vertexBufferHandle);
		break;
	}
}
void Mesh::CreateMesh(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& vertexUploadHandle, OUT GpuMemoryHandle& indexUploadHandle)
{
	CreateVertexResource(commandList, vertexUploadHandle);
	CreateIndexResource(commandList, indexUploadHandle);
}

void Mesh::CreateVertexResource(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& handle)
{
	UINT64 vertexTotalSize = _meshInfo.geometry.vertices.GetCount();

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = vertexTotalSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 힙 할당
	bool isSuccess = GPU_BUFFER_POOL->GetMemory(_vertexPoolID, eGpuBufferKind::Vertex, vertexTotalSize, 4, _vertexBufferHandle);
	assert(isSuccess);

	ComPtr<ID3D12Resource> uploadResource;
	GetUploadHeap(desc, vertexTotalSize, uploadResource, handle);

	D3D12_RESOURCE_ALLOCATION_INFO info = DEVICE->GetResourceAllocationInfo(0, 1, &desc);
	_vertexSizeInBytes = info.SizeInBytes;
	_vertexAlignment = info.Alignment;

	void* data;
	uploadResource->Map(0, nullptr, &data);
	memcpy(data, _meshInfo.geometry.vertices.GetData(), vertexTotalSize);
	uploadResource->Unmap(0, nullptr);

	ComPtr<ID3D12Resource> resource = GPU_BUFFER_POOL->GetMemoryHeap((GpuBufferPoolManager::eBufferPoolID)_vertexBufferHandle.poolID, _vertexBufferHandle.pageIndex);

	commandList->CopyBufferRegion(resource.Get(), _vertexBufferHandle.offset, uploadResource.Get(), 0, vertexTotalSize);

	assert(resource != nullptr);

	_vertexResourceView = {};
	_vertexResourceView.BufferLocation = resource->GetGPUVirtualAddress() + _vertexBufferHandle.offset;
	_vertexResourceView.SizeInBytes = vertexTotalSize;
	_vertexResourceView.StrideInBytes = _meshInfo.geometry.vertexSize;

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	commandList->ResourceBarrier(1, &barrier);
}

void Mesh::CreateIndexResource(const ComPtr<ID3D12GraphicsCommandList>& commandList, OUT GpuMemoryHandle& handle)
{
	uint32 indexTotalSize = _meshInfo.geometry.indices.GetCount();

	D3D12_RESOURCE_DESC desc = {};
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = indexTotalSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 힙 할당
	bool isSuccess = GPU_BUFFER_POOL->GetMemory(_indexPoolID, eGpuBufferKind::Index, indexTotalSize, (UINT8)_meshInfo.geometry.indexSize, _indexBufferHandle);
	assert(isSuccess);

	ComPtr<ID3D12Resource> uploadResource;
	GetUploadHeap(desc, indexTotalSize, uploadResource, handle);

	void* data;
	uploadResource->Map(0, nullptr, &data);
	memcpy(data, _meshInfo.geometry.indices.GetData(), indexTotalSize);
	uploadResource->Unmap(0, nullptr);

	ComPtr<ID3D12Resource> resource = GPU_BUFFER_POOL->GetMemoryHeap((GpuBufferPoolManager::eBufferPoolID)_indexBufferHandle.poolID, _indexBufferHandle.pageIndex);

	commandList->CopyBufferRegion(resource.Get(), _indexBufferHandle.offset, uploadResource.Get(), 0, indexTotalSize);

	_indexResourceView = {};
	_indexResourceView.BufferLocation = resource->GetGPUVirtualAddress() + _indexBufferHandle.offset;
	_indexResourceView.SizeInBytes = indexTotalSize;

	switch (_meshInfo.geometry.indexSize)
	{
		case eIndexBufferSize::UINT_16:
			_indexResourceView.Format = DXGI_FORMAT_R16_UINT;
			break;
		case eIndexBufferSize::UINT_32:
			_indexResourceView.Format = DXGI_FORMAT_R32_UINT;
			break;
	}

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_INDEX_BUFFER);

	commandList->ResourceBarrier(1, &barrier);
}

void Mesh::GetUploadHeap(const D3D12_RESOURCE_DESC& desc, UINT64 size, OUT ComPtr<ID3D12Resource>& uploadResource, OUT GpuMemoryHandle& uploadHandle)
{
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT rootPrintLayout = {};
	uint32 numRows;
	uint64 rowSizeInBytes;
	uint64 totalByte;
	DEVICE->GetCopyableFootprints(&desc, 0, 1, 0, &rootPrintLayout, &numRows, &rowSizeInBytes, &totalByte);

	// Create Upload Buffer
	D3D12_RESOURCE_DESC uploadHeapBuffer = CD3DX12_RESOURCE_DESC::Buffer(totalByte);

	ComPtr<ID3D12Heap> uploadHeap = GPU_MEM_POOL->GetMemoryHeap(GpuMemoryPoolManager::eMemoryPoolID::DYNAMIC_UPLOAD);
	bool isSuccess = GPU_MEM_POOL->GetMemory(GpuMemoryPoolManager::eMemoryPoolID::DYNAMIC_UPLOAD, eGpuMemoryPoolType::SIZE_64KB, size, uploadHandle);
	assert(isSuccess);
	ThrowIfFailed(DEVICE->CreatePlacedResource(uploadHeap.Get(), uploadHandle.offset, &uploadHeapBuffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadResource.GetAddressOf())));
}
