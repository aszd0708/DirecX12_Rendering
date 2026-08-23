#include "pch.h"
#include "Camera.h"
#include "Transform.h"

Camera::Camera() : Component(eComponentType::Camera)
{}

Camera::~Camera()
{}

void Camera::Update()
{}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();

	_matView = S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	_matProjection = S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
}
