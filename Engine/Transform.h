#pragma once
#include "Component.h"
class Transform : public Component
{
	DECLARE_COMPONENT_TYPE(eComponentType::Transform);

public:
	Transform();
	virtual ~Transform() override;

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestory() override;
};

