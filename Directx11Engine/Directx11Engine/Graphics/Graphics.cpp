#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int heigth) {	
	if (!InitializeDirectX(hwnd, width, heigth)) {
		return false;
	}

	if (!InitializeShaders()) {
		return false;
	}
	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int heigth) {
	std::vector<VideoAdapterData> videoAdapters = VideoAdapterReader::GetVideoAdapters();

	if (videoAdapters.size() < 1) {
		ErrorLogger::Log("No IDXGI adapters found!");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDescription.BufferDesc.Width = width;
	swapChainDescription.BufferDesc.Height = heigth;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 75;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = hwnd;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = D3D11CreateDeviceAndSwapChain(videoAdapters.at(0).pAdapter, // IDXI Adapter
													D3D_DRIVER_TYPE_UNKNOWN, // Driver type
													NULL, // For software driver type
													NULL, // Flags for runtime layers
													NULL, // features levels array
													0,    // # of feature levels in array
													D3D11_SDK_VERSION,
													&swapChainDescription, //Swap chain description
													this->swapChain.GetAddressOf(), // Swap chain address
													this->device.GetAddressOf(), // Device address
													NULL, // Supported feature level
													this->deviceContext.GetAddressOf()); //Device context address
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create device and swapchain!");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hResult = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create texture buffer!");
		return false;
	}

	hResult = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetWiew.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create render target view!");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetWiew.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewport;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width    = width;
	viewport.Height   = heigth;

	this->deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeShaders() {
	std::wstring shaderFolder = L"";

	#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE) {
		#ifdef _DEBUG
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Debug\\";
			#else
				shaderFolder = L"..\\Debug\\";
			#endif
		#else
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Release\\";
			#else
				shaderFolder = L"..\\Release\\";
			#endif
		#endif
	}
		
	D3D11_INPUT_ELEMENT_DESC layout[] = {
											{"POSITION",
											 0,
											 DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
											 0,
											 0,
											 D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
											 0}
										};

	UINT numOfElements = ARRAYSIZE(layout);

	if (!this->vertexShader.Initialize(this->device, shaderFolder + L"VertexShader.cso", layout, numOfElements)) {
		return false;
	}

	if (this->pixelShader.Initialize(this->device, shaderFolder + L"PixelShader.cso")) {
		return false;
	}

	return true;
}

void Graphics::RenderFrame() {
	// Test
	float bgColor[] = {0.0f, 0.0f, 1.0f, 1.0f};

	this->deviceContext->ClearRenderTargetView(this->renderTargetWiew.Get(), bgColor);
	this->swapChain->Present(1, NULL);
}