#pragma once
#include "Renderer.h"
class MeshRenderer : public Renderer
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);
public:
	MeshRenderer();
	virtual ~MeshRenderer() override;

public:
	virtual void Render() override;
};

