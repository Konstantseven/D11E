#include "Camera.h"

Camera::Camera() {
	this->positionFloat3 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->positionVector = DirectX::XMLoadFloat3(&this->positionFloat3);

	this->rotationFloat3 = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotationFloat3);

	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDeg, float aspectRatio, float nearZ, float farZ) {
	float fovRad = Converter::DegToRad(fovDeg);

	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const {
	return this->viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const {
	return this->projectionMatrix;
}

const DirectX::XMVECTOR& Camera::GetPositionVector() const {
	return this->positionVector;
}

const DirectX::XMFLOAT3& Camera::GetPositionFloat3() const {
	return this->positionFloat3;
}

const DirectX::XMVECTOR& Camera::GetRotationVector() const {
	return this->rotationVector;
}

const DirectX::XMFLOAT3& Camera::GetRotationFloat3() const {
	return this->rotationFloat3;
}

void Camera::SetPosition(const DirectX::XMVECTOR& position) {
	DirectX::XMStoreFloat3(&this->positionFloat3, position);
	this->positionVector = position;

	this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z) {
	this->positionFloat3 = DirectX::XMFLOAT3(x, y, z);
	this->positionVector = DirectX::XMLoadFloat3(&this->positionFloat3);

	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const DirectX::XMVECTOR& pos) {
	using DirectX::operator+=;

	this->positionVector += pos;
	DirectX::XMStoreFloat3(&this->positionFloat3, this->positionVector);

	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z) {
	this->positionFloat3.x += x;
	this->positionFloat3.y += y;
	this->positionFloat3.z += z;
	this->positionVector = DirectX::XMLoadFloat3(&this->positionFloat3);

	this->UpdateViewMatrix();
}

void Camera::SetRotation(const DirectX::XMVECTOR& rot) {
	this->rotationVector = rot;
	DirectX::XMStoreFloat3(&this->rotationFloat3, rot);

	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z) {
	this->rotationFloat3 = DirectX::XMFLOAT3(x, y, z);
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotationFloat3);

	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const DirectX::XMVECTOR& rot) {
	using DirectX::operator+=;

	this->rotationVector += rot;
	DirectX::XMStoreFloat3(&this->rotationFloat3, this->rotationVector);

	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z) {
	this->rotationFloat3.x += x;
	this->rotationFloat3.y += y;
	this->rotationFloat3.z += z;
	this->rotationVector = DirectX::XMLoadFloat3(&this->rotationFloat3);

	this->UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	using DirectX::operator+=;

	DirectX::XMMATRIX cameraRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotationFloat3.x, this->rotationFloat3.y, this->rotationFloat3.z);
	DirectX::XMVECTOR cameraTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, cameraRotationMatrix);
	cameraTarget += this->positionVector;

	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, cameraRotationMatrix);

	this->viewMatrix = DirectX::XMMatrixLookAtLH(this->positionVector, cameraTarget, upDir);
}