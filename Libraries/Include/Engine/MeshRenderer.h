#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
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
	void Init(MemoryBlock meshHandler, MemoryBlock materialHandler);
	virtual void Render(ID3D12GraphicsCommandList* commandList) override;
	virtual void RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count) override;
	virtual bool SetInstancingInfo(InstancingInfo& info) override;

public:
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer() { return _mesh->GetVertexView(); }

private:
	Array<InstanceDesc> _instancingDesc;
	Mesh* _mesh;
	Material* _material;
	const UINT32* _textureIndexes;
	UINT8 _textureCount;
};