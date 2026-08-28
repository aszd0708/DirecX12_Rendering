#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "Camera.h"

class Shader;
class ConstantBuffer;

template<typename T>
class MeshRenderer : public Renderer
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	MeshRenderer();
	virtual ~MeshRenderer() override;

public:
	void Init(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader);
	void Init(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader, shared_ptr<Texture> texture);
	virtual void Render() override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	shared_ptr<Mesh<T>> _mesh;
	shared_ptr<Shader> _shader;
	shared_ptr<Texture> _texture;
};

template<typename T>
inline MeshRenderer<T>::MeshRenderer() : Renderer(eComponentType::Renderer)
{
	
}

template<typename T>
inline MeshRenderer<T>::~MeshRenderer()
{

}

template<typename T>
inline void MeshRenderer<T>::Init(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader)
{
	_mesh = mesh;
	_shader = shader;
	_texture = nullptr;
}

template<typename T>
inline void MeshRenderer<T>::Init(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader, shared_ptr<Texture> texture)
{
	_mesh = mesh;
	_shader = shader;
	_texture = texture;
}

template<typename T>
inline void MeshRenderer<T>::Render()
{
	Renderer::Render();

	PushGlobalBuffer(Camera::S_MatView, Camera::S_MatProjection);
	PushWorldMatrixBuffer();

	COMMAND_LIST->SetGraphicsRootSignature(this->_shader->GetRootSignature().Get());
	COMMAND_LIST->SetPipelineState(_shader->GetPSO().Get());
	COMMAND_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Vertex 버퍼 전달
	D3D12_VERTEX_BUFFER_VIEW vertexView = _mesh->GetVertexView();
	COMMAND_LIST->IASetVertexBuffers(0, 1, &vertexView);

	// Index 버퍼 전달
	D3D12_INDEX_BUFFER_VIEW indexView = _mesh->GetIndexView();
	COMMAND_LIST->IASetIndexBuffer(&indexView);

	// Global 버퍼 전달
	COMMAND_LIST->SetGraphicsRootConstantBufferView(0, GetGlobalBuffer()->GetAddress());

	// World Matrix 버퍼 전달
	COMMAND_LIST->SetGraphicsRootConstantBufferView(1, GetWorldMatrixBuffer()->GetAddress());

	// Texture 전달
	if(_texture != nullptr)
	{
		ID3D12DescriptorHeap* heaps[] = { _texture->GetDescHeap().Get() };
		COMMAND_LIST->SetDescriptorHeaps(1, heaps);
		COMMAND_LIST->SetGraphicsRootDescriptorTable(2, _texture->GetHandle());
	}

	COMMAND_LIST->DrawIndexedInstanced(_mesh->GetMesh()->GetIndexCount(), 1, 0, 0, 0);
}
