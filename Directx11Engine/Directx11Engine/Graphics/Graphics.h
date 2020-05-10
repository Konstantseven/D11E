#pragma once

#include "../Helpers/Converter.h"
#include "../Helpers/Timer.h"
#include "VideoAdapterReader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "Model.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_win32.h>
#include <ImGUI/imgui_impl_dx11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class Graphics 
{
public:
	bool Initialize(HWND hwnd, int width, int heigth);
	void RenderFrame();
	Camera camera;
private:
	int windowWidth;
	int windowHeight;

	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetWiew;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStateCullFront;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	ConstantBuffer<CB_VS_VERTEXSHADER> CBVSVertexShader;
	ConstantBuffer<CB_PS_PIXELSHADER> CBPSPixelShader;

	Model model;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	VertexShader vertexShader;
	PixelShader pixelShader;

	Timer FPSCountTimer;
};
