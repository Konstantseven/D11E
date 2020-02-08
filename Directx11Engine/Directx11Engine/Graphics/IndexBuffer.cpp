#include "IndexBuffer.h"

ID3D11Buffer* IndexBuffer::Get() const {
	return buffer.Get();
}

ID3D11Buffer* const* IndexBuffer::GetAddresOf() const {
	return buffer.GetAddressOf();
}

UINT IndexBuffer::BufferSize() const
{
	return this->bufferSize;
}

HRESULT IndexBuffer::Initialize(ID3D11Device* device, const DWORD* data, const UINT numIndices) {
	this->bufferSize = numIndices;

	D3D11_BUFFER_DESC indexBufferDescription;

	ZeroMemory(&indexBufferDescription, sizeof(indexBufferDescription));

	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDescription.ByteWidth = sizeof(DWORD) * numIndices;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = data;
	HRESULT hResult = device->CreateBuffer(&indexBufferDescription, &indexBufferData, buffer.GetAddressOf());

	return hResult;
}