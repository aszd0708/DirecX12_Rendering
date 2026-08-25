#include "pch.h"
#include "Camera.h"
#include "Transform.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Component(eComponentType::Camera)
{
	_width = SCREEN_WIDTH;
	_height = SCREEN_HEIGHT;
}

Camera::~Camera()
{

}

void Camera::Update()
{
	Component::Update();
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();

	_matView = S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	_matProjection = S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
}
