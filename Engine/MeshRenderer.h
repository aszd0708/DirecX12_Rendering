#pragma once
#include "Renderer.h"
#include "Mesh.h"

class Shader;

template<typename T>
class MeshRenderer : public Renderer
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	inline MeshRenderer(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader);
	inline virtual ~MeshRenderer() override;

public:
	inline virtual void Render() override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	shared_ptr<Mesh<T>> _mesh;
	shared_ptr<Shader> _shader;
};

template<typename T>
inline MeshRenderer<T>::MeshRenderer(shared_ptr<Mesh<T>> mesh, shared_ptr<Shader> shader) : Renderer(eComponentType::Renderer), _mesh(mesh), _shader(shader)
{
	
}

template<typename T>
inline MeshRenderer<T>::~MeshRenderer()
{

}

template<typename T>
inline void MeshRenderer<T>::Render()
{
	Renderer::Render();

	COMMAND_LIST->SetGraphicsRootSignature(this->_shader->GetRootSignature().Get());
	COMMAND_LIST->SetPipelineState(_shader->GetPSO().Get());
	COMMAND_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	COMMAND_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexView());
	COMMAND_LIST->IASetIndexBuffer(&_mesh->GetIndexView());
	COMMAND_LIST->DrawIndexedInstanced(3, 1, 0, 0);
}
