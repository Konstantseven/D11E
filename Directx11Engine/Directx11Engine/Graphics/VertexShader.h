#pragma once

#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include "../Helpers/ErrorLogger.h"

class VertexShader
{
public:
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDescription, UINT numOfElements);
	ID3D11VertexShader* GetShader() const;
	ID3D10Blob* GetBuffer() const;
	ID3D11InputLayout* GetInputLayout() const;
private:
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

