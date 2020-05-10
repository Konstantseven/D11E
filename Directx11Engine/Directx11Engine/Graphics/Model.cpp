#include "Model.h"

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_VERTEXSHADER>& CBVSVertexShader) {
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->CBVSVertexShader = &CBVSVertexShader;

	Vertex vertexArray[] = {
		Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left - [0]
		Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left    - [1]
		Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f), //FRONT Top Right    - [2]
		Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f), //FRONT Bottom Right - [3]
		Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f), //BACK Bottom Left  - [4]
		Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 0.0f), //BACK Top Left     - [5]
		Vertex(0.5f,   0.5f,  0.5f, 1.0f, 0.0f), //BACK Top Right     - [6]
		Vertex(0.5f,  -0.5f,  0.5f, 1.0f, 1.0f), //BACK Bottom Right  - [7]
	};

	HRESULT hResult = this->vertexBuffer.Initialize(this->device, vertexArray, ARRAYSIZE(vertexArray));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create vertex buffer!");
		return false;
	}

	DWORD indicies[] =
	{
		0, 1, 2, //FRONT
		0, 2, 3, //FRONT
		4, 7, 6, //BACK 
		4, 6, 5, //BACK
		3, 2, 6, //RIGHT SIDE
		3, 6, 7, //RIGHT SIDE
		4, 5, 1, //LEFT SIDE
		4, 1, 0, //LEFT SIDE
		1, 5, 6, //TOP
		1, 6, 2, //TOP
		0, 3, 7, //BOTTOM
		0, 7, 4, //BOTTOM
	};

	hResult = this->indexBuffer.Initialize(this->device, indicies, ARRAYSIZE(indicies));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create indicies buffer!");
		return false;
	}

	this->UpdateWorldMatrix();

	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture) {
	this->texture = texture;
}

void Model::Draw(const DirectX::XMMATRIX& viewProjectionMatrix) {
	this->CBVSVertexShader->data.matrix_dxxmm = this->worldMatrix * viewProjectionMatrix;
	this->CBVSVertexShader->data.matrix_dxxmm = DirectX::XMMatrixTranspose(this->CBVSVertexShader->data.matrix_dxxmm);
	this->CBVSVertexShader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->CBVSVertexShader->GetAddressOf());

	this->deviceContext->PSSetShaderResources(0, 1, &this->texture);
	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0);
}

void Model::UpdateWorldMatrix() {
	this->worldMatrix = DirectX::XMMatrixIdentity();
}
