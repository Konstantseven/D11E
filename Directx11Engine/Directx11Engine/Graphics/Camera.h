#pragma once

#include "../Helpers/Converter.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera();

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;
	const DirectX::XMVECTOR& GetPositionVector() const;
	const DirectX::XMFLOAT3& GetPositionFloat3() const;
	const DirectX::XMVECTOR& GetRotationVector() const;
	const DirectX::XMFLOAT3& GetRotationFloat3() const;
	const DirectX::XMVECTOR& GetForwardVector() const;
	const DirectX::XMVECTOR& GetRightVector() const;
	const DirectX::XMVECTOR& GetBackwardVector() const;
	const DirectX::XMVECTOR& GetLeftVector() const;

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	void SetPosition(const DirectX::XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const DirectX::XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const DirectX::XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPosition(DirectX::XMFLOAT3 lookAtPosition);

private:
	void UpdateViewMatrix();

	DirectX::XMVECTOR positionVector;
	DirectX::XMVECTOR rotationVector;
	DirectX::XMVECTOR forwardVector;
	DirectX::XMVECTOR leftVector;
	DirectX::XMVECTOR rightVector;
	DirectX::XMVECTOR backwardVector;
	DirectX::XMFLOAT3 positionFloat3;
	DirectX::XMFLOAT3 rotationFloat3;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR  = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR    = DirectX::XMVectorSet(1.0F, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR     = DirectX::XMVectorSet(1.0F, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR       = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};

