#include "pch.h"
#include "CameraController.h"
#include "Transform.h"

CameraController::CameraController() : Component(eComponentType::Script)
{

}

CameraController::~CameraController()
{

}

void CameraController::Start()
{
	_prevMousePos = INPUT->GetMousePos();
}

void CameraController::Update()
{
	float dt = TIME->GetDeltaTime();

	Transform* transform = nullptr;
	bool isSuccess = GetTransform(&transform);
	if(isSuccess == false) return;

	// Position
	{
		Vec3 pos = transform->GetPosition();

		if (INPUT->GetButton(KEY_TYPE::W))
		{
			pos += transform->GetLook() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			pos -= transform->GetLook() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			pos -= transform->GetRight() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			pos += transform->GetRight() * _speed * dt;
		}

		transform->SetPosition(pos);
	}

	// Rotation
	{
		if (INPUT->GetButton(KEY_TYPE::Q))
		{
			Vec3 rotation = transform->GetLocalRotation();
			rotation.x += dt * 0.5f;
			transform->SetLocalRotation(rotation);
		}
		if (INPUT->GetButton(KEY_TYPE::E))
		{
			Vec3 rotation = transform->GetLocalRotation();
			rotation.x -= dt * 0.5f;
			transform->SetLocalRotation(rotation);
		}
		if (INPUT->GetButton(KEY_TYPE::Z))
		{
			Vec3 rotation = transform->GetLocalRotation();
			rotation.y += dt * 0.5f;
			transform->SetLocalRotation(rotation);
		}
		if (INPUT->GetButton(KEY_TYPE::C))
		{
			Vec3 rotation = transform->GetLocalRotation();
			rotation.y -= dt * 0.5f;
			transform->SetLocalRotation(rotation);
		}
	}

	{
		POINT mousePos = INPUT->GetMousePos();

		if (INPUT->GetButton(KEY_TYPE::RBUTTON))
		{
			float dx = static_cast<float>(mousePos.x - _prevMousePos.x);
			float dy = static_cast<float>(mousePos.y - _prevMousePos.y);

			Vec3 rotation = transform->GetLocalRotation();
			rotation.y += dx * dt * 0.2f; // Yaw (좌우 회전)
			rotation.x += dy * dt * 0.2f; // Pitch (상하 회전)
			transform->SetLocalRotation(rotation);
		}

		_prevMousePos = mousePos;
	}
}
