#pragma once

#include "VideoAdapterReader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vertex.h"


class Graphics 
{
public:
	bool Initialize(HWND hwnd, int width, int heigth);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hwnd, int width, int heigth);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetWiew;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

	VertexShader vertexShader;
	PixelShader pixelShader;
};
