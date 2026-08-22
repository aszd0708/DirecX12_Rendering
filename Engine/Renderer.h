#pragma once
#include "Component.h"
class Renderer : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Renderer);

public:
	Renderer(eComponentType type);
	virtual ~Renderer() override;

public:
	virtual void Render();

private:
	
};

