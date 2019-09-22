#include "PixelShader.h"

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath) {
	HRESULT hResult = D3DReadFileToBlob(shaderPath.c_str(), this->shaderBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += shaderPath;
		ErrorLogger::Log(hResult, errorMsg);

		return false;
	}

	hResult = device->CreatePixelShader(this->shaderBuffer.Get()->GetBufferPointer(), this->shaderBuffer.Get()->GetBufferSize(), NULL, this->pixelShader.GetAddressOf());
	if (FAILED(hResult)) {
		std::wstring errorMsg = L"Failed create pixel shader: ";
		errorMsg += shaderPath;
		ErrorLogger::Log(hResult, errorMsg);

		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() const {
	return this->pixelShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer() const {
	return this->shaderBuffer.Get();
}
