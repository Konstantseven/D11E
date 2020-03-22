#pragma once

#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include "../Helpers/ErrorLogger.h"

class PixelShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath);
	ID3D11PixelShader* GetShader() const;
	ID3D10Blob* GetBuffer() const;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
};