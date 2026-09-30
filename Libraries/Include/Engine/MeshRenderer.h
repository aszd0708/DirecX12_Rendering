#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "Camera.h"

class ConstantBuffer;

class MeshRenderer : public Renderer
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	MeshRenderer();
	virtual ~MeshRenderer() override;

public:
	void Init(MemoryBlock meshHandler, MemoryBlock shaderHandler);
	void Init(MemoryBlock meshHandler, MemoryBlock shaderHandler, MemoryBlock texture);
	virtual void Render(ID3D12GraphicsCommandList* commandList) override;
	virtual void RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count) override;
	virtual bool SetInstancingInfo(InstancingInfo& info) override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	Mesh* _mesh;

	Shader* _shader;
	Texture* _texture;
};