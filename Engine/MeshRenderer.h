#pragma once
#include "Renderer.h"
#include "Mesh.h"
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
	virtual void Render() override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	shared_ptr<Mesh<T>> _mesh;
	shared_ptr<Shader> _shader;
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
}

template<typename T>
inline void MeshRenderer<T>::Render()
{
	Renderer::Render();

	PushGlobalBuffer(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
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
	COMMAND_LIST->SetGraphicsRootConstantBufferView(0, GetWorldMatrixBuffer()->GetAddress());

	// World Matrix 버퍼 전달
	COMMAND_LIST->SetGraphicsRootConstantBufferView(1, GetGlobalBuffer()->GetAddress());

	COMMAND_LIST->DrawIndexedInstanced(_mesh->GetMesh()->GetIndexCount(), 1, 0, 0);
}
