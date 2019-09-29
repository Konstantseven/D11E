#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int heigth) {	
	if (!InitializeDirectX(hwnd, width, heigth)) {
		return false;
	}

	if (!InitializeShaders()) {
		return false;
	}

	if (!InitializeScene()) {
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
	viewport.Width    = static_cast<float> (width);
	viewport.Height   = static_cast<float> (heigth);

	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDescription;

	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterizerDescription.FrontCounterClockwise = TRUE;

	hResult = this->device->CreateRasterizerState(&rasterizerDescription, this->rasterizerState.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create rasterizer state!");
		return false;
	}

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
											 0},
											{"COLOR",
											 0,
											 DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
											 0,
											 D3D11_APPEND_ALIGNED_ELEMENT,
											 D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
											 0}
										};

	UINT numOfElements = ARRAYSIZE(layout);

	if (!this->vertexShader.Initialize(this->device, shaderFolder + L"VertexShader.cso", layout, numOfElements)) {
		return false;
	}

	if (!this->pixelShader.Initialize(this->device, shaderFolder + L"PixelShader.cso")) {
		return false;
	}

	return true;
}

bool Graphics::InitializeScene()
{
	Vertex vertexArray[] = { // template
								Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
								Vertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
								Vertex(1.0f, -1.0f, 0.0f, 1.0f, 0.0f),
							};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertexArray);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = vertexArray;

	HRESULT hResult = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create vertex buffer!");
		return false;
	}

	return true;
}

void Graphics::RenderFrame() {
	float bgColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

	this->deviceContext->ClearRenderTargetView(this->renderTargetWiew.Get(), bgColor);
	
	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());

	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(4, 0);

	this->swapChain->Present(1, NULL);
}