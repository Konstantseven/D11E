#pragma once

#include "ConstantBufferTypes.h"
#include "../Helpers/ErrorLogger.h"
#include <wrl/client.h>
#include <d3d11.h>

template <class T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}

	ID3D11Buffer* Get() const {
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const {
		return buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC bufferDescription;
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.MiscFlags = 0;
		bufferDescription.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		bufferDescription.StructureByteStride = 0;

		HRESULT hResult = device->CreateBuffer(&bufferDescription, 0, buffer.GetAddressOf());
		return hResult;
	}

	bool ApplyChanges() {
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hResult = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hResult)) {
			helpers::error_logger::Log(hResult, "Failed to map constant buffer!");
			return false;
		}

		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(buffer.Get(), 0);

		return true;
	}

	T data;

private:
	ConstantBuffer(const ConstantBuffer<T>&) = delete;

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;
};
