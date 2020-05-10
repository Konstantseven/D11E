#pragma once

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Model
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_VERTEXSHADER>& CBVSVertexShader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);

private:
	void UpdateWorldMatrix();

	ID3D11Device*                       device           = nullptr;
	ID3D11DeviceContext*                deviceContext    = nullptr;
	ID3D11ShaderResourceView*           texture          = nullptr;
	ConstantBuffer<CB_VS_VERTEXSHADER>* CBVSVertexShader = nullptr;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer          indexBuffer;

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
};
