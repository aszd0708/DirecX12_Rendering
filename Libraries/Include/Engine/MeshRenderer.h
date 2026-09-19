#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "Camera.h"

class Shader;
class ConstantBuffer;

class MeshRenderer : public Renderer
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	MeshRenderer();
	virtual ~MeshRenderer() override;

public:
	void Init(MemoryBlock meshHandler, shared_ptr<Shader> shader);
	void Init(MemoryBlock meshHandler, shared_ptr<Shader> shader, MemoryBlock& texture);
	virtual void Render() override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	Mesh* _mesh;

	shared_ptr<Shader> _shader;
	Texture* _texture;
};