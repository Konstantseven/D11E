#pragma once

#include "VideoAdapterReader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>


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
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer2; // test
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	VertexShader vertexShader;
	PixelShader pixelShader;
};
