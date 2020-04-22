#include "VertexShader.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDescription, UINT numOfElements) {
	HRESULT hResult = D3DReadFileToBlob(shaderPath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		std::wstring errorMesage = L"Failed to load shader: " + shaderPath;
		helpers::error_logger::Log(hResult, errorMesage);
		return false;
	}

	hResult = device->CreateVertexShader(this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hResult)) {
		std::wstring errorMesage = L"Failed to create vertex shader: " + shaderPath;
		helpers::error_logger::Log(hResult, errorMesage);
		return false;
	}

	hResult = device->CreateInputLayout(layoutDescription, numOfElements, this->shaderBuffer->GetBufferPointer(), this->shaderBuffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create input layout!");
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() const {
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer() const {
	return this->shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() const {
	return this->inputLayout.Get();
}
