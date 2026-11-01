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
	bool SetInstancingInfo(InstancingController* controller) override;

public:
	

private:
	Array<InstanceDesc> _instancingDesces;
	Model* _model = nullptr;
};

