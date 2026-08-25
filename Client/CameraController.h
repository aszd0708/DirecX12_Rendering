#pragma once
#include "Component.h"

class CameraController : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Script);

public:
	CameraController();
	virtual ~CameraController() override;

public:
	void Start() override;
	void Update() override;

	float _speed = 10.f;

private:
	POINT _prevMousePos = {};
};

