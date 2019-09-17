#pragma once

#include "VideoAdapterReader.h"
#include "Shader.h"

class Graphics 
{
public:
	bool Initialize(HWND hwnd, int width, int heigth);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hwnd, int width, int heigth);
	bool InitializeShaders();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetWiew;
	VertexShader vertexShader;
};
