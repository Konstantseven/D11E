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

	D3D11_TEXTURE2D_DESC depthStencilDescription;

	ZeroMemory(&depthStencilDescription, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilDescription.Width = width;
	depthStencilDescription.Height = heigth;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	hResult = this->device->CreateTexture2D(&depthStencilDescription, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create depth stencil buffer!");
		return false;
	}

	hResult = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this -> depthStencilView.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create depth stencil view!");
		return false;
	}
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetWiew.GetAddressOf(), this->depthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilDecription;

	ZeroMemory(&depthStencilDecription, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDecription.DepthEnable = true;
	depthStencilDecription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDecription.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hResult = this->device->CreateDepthStencilState(&depthStencilDecription, this->depthStencilState.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create depth stencil state!");
		return false;
	}

	D3D11_VIEWPORT viewport;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width    = static_cast<float> (width);
	viewport.Height   = static_cast<float> (heigth);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

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

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\digital_7_fs_16.spritefont");

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
											 DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
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
								Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
								Vertex(0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
								Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
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
	// test triangle
	Vertex testVertexArray[] = { // template
								Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
								Vertex(0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
								Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
	};

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(testVertexArray);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = testVertexArray;

	hResult = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer2.GetAddressOf());
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to create vertex buffer!");
		return false;
	}

	return true;
}

void Graphics::RenderFrame() {
	float bgColor[] = {0.0f, 0.0f, 0.0f, 1.0f};

	this->deviceContext->ClearRenderTargetView(this->renderTargetWiew.Get(), bgColor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// default triangle
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(3, 0);

	// test triangle
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer2.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(3, 0);

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), L"Hello World", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	this->swapChain->Present(1, NULL);
}