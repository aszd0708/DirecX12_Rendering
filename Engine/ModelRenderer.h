#pragma once
#include "Renderer.h"

class Model;

class ModelRenderer : public Renderer
{
public:
	ModelRenderer();
	virtual ~ModelRenderer() override;

private:

public:
	void Init(MemoryBlock& modelHandler);
	void Render(ID3D12GraphicsCommandList* commandList) override;
	void RenderInstancing(ID3D12GraphicsCommandList* commandList, D3D12_GPU_VIRTUAL_ADDRESS address, UINT32 count) override;
	bool SetInstancingInfo(InstancingInfo& info) override;

public:
	

private:
	Array<InstanceDesc> _instancingDesces;
	Model* _model = nullptr;
};

