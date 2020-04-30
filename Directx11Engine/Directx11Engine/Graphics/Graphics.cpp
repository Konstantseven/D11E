#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int heigth) {
	this->windowWidth = width;
	this->windowHeight = heigth;

	this->FPSCountTimer.Start();

	if (!InitializeDirectX(hwnd)) {
		return false;
	}

	if (!InitializeShaders()) {
		return false;
	}

	if (!InitializeScene()) {
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

bool Graphics::InitializeDirectX(HWND hwnd) {
	std::vector<VideoAdapterData> videoAdapters = VideoAdapterReader::GetVideoAdapters();

	if (videoAdapters.size() < 1) {
		helpers::error_logger::Log("No IDXGI adapters found!");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDescription.BufferDesc.Width = this->windowWidth;
	swapChainDescription.BufferDesc.Height = this->windowHeight;
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
		helpers::error_logger::Log(hResult, "Failed to create device and swapchain!");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hResult = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create texture buffer!");
		return false;
	}

	hResult = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetWiew.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create render target view!");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthStencilDescription;

	ZeroMemory(&depthStencilDescription, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilDescription.Width = this->windowWidth;
	depthStencilDescription.Height = this->windowHeight;
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
		helpers::error_logger::Log(hResult, "Failed to create depth stencil buffer!");
		return false;
	}

	hResult = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this -> depthStencilView.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create depth stencil view!");
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
		helpers::error_logger::Log(hResult, "Failed to create depth stencil state!");
		return false;
	}

	D3D11_VIEWPORT viewport;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width    = static_cast<float> (this->windowWidth);
	viewport.Height   = static_cast<float> (this->windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	hResult = this->device->CreateRasterizerState(&rasterizerDescription, this->rasterizerState.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create rasterizer state!");
		return false;
	}

	D3D11_RASTERIZER_DESC rasterizerDescriptionCullFront;
	ZeroMemory(&rasterizerDescriptionCullFront, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDescriptionCullFront.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDescriptionCullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	hResult = this->device->CreateRasterizerState(&rasterizerDescriptionCullFront, this->rasterizerStateCullFront.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create rasterizer state!");
		return false;
	}

	D3D11_BLEND_DESC blendDescription;
	ZeroMemory(&blendDescription, sizeof(blendDescription));

	D3D11_RENDER_TARGET_BLEND_DESC RTBDescription;
	ZeroMemory(&RTBDescription, sizeof(RTBDescription));

	RTBDescription.BlendEnable = true;
	RTBDescription.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	RTBDescription.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	RTBDescription.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	RTBDescription.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	RTBDescription.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	RTBDescription.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	RTBDescription.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDescription.RenderTarget[0] = RTBDescription;

	hResult = this->device->CreateBlendState(&blendDescription, this->blendState.GetAddressOf());
	if (FAILED(hResult))
	{
		helpers::error_logger::Log(hResult, "Failed to create blend state!");
		return false;
	}

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\digital_7_fs_16.spritefont");

	D3D11_SAMPLER_DESC samplerDescription;
	ZeroMemory(&samplerDescription, sizeof(D3D11_SAMPLER_DESC));

	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = this->device->CreateSamplerState(&samplerDescription, this->samplerState.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create sampler state!");
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
											 DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,
											 0,
											 0,
											 D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
											 0},
											{"TEXTURE_COORDINATE",
											 0,
											 DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
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
	Vertex vertexArray[] = {
		Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left  - [0]
		Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left     - [1]
		Vertex(	0.5f,  0.5f, -0.5f, 1.0f, 0.0f), //FRONT Top Right    - [2]
		Vertex(	0.5f, -0.5f, -0.5f, 1.0f, 1.0f), //FRONT Bottom Right - [3]
		Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f), //BACK Bottom Left   - [4]
		Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 0.0f), //BACK Top Left      - [5]
		Vertex(	0.5f,  0.5f,  0.5f, 1.0f, 0.0f), //BACK Top Right     - [6]
		Vertex(	0.5f, -0.5f,  0.5f, 1.0f, 1.0f), //BACK Bottom Right  - [7]
							};

	HRESULT hResult = this->vertexBuffer.Initialize(this->device.Get(), vertexArray, ARRAYSIZE(vertexArray));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create vertex buffer!");
		return false;
	}

	DWORD indicies[] = 
	{
		0, 1, 2, //FRONT
		0, 2, 3, //FRONT
		4, 7, 6, //BACK 
		4, 6, 5, //BACK
		3, 2, 6, //RIGHT SIDE
		3, 6, 7, //RIGHT SIDE
		4, 5, 1, //LEFT SIDE
		4, 1, 0, //LEFT SIDE
		1, 5, 6, //TOP
		1, 6, 2, //TOP
		0, 3, 7, //BOTTOM
		0, 7, 4, //BOTTOM
	};

	hResult = this->indicesBuffer.Initialize(this->device.Get(), indicies, ARRAYSIZE(indicies));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create indicies buffer!");
		return false;
	}


	hResult = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, this->grassTexture.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create WIC texture from file!");
		return false;
	}

	hResult = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, this->pinkTexture.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create WIC texture from file!");
		return false;
	}

	hResult = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, this->pavementTexture.GetAddressOf());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create WIC texture from file!");
		return false;
	}

	hResult = this->CBVSVertexShader.Initialize(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to initialize constant buffer of vertex shaders!");
		return false;
	}

	hResult = this->CBPSPixelShader.Initialize(this->device.Get(), this->deviceContext.Get());
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to initialize constant buffer of pixel shaders!");
		return false;
	}

	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);

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
	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xffffffff);
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	static float alpha = 0.1f;
	static float translationOffset[3] = { 0.0f, 0.0f, -1.0f };
	{
		DirectX::XMMATRIX wordMatrix_dxxmm = DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);
		this->CBVSVertexShader.data.matrix_dxxmm = wordMatrix_dxxmm * camera.GetViewMatrix() * camera.GetProjectionMatrix();
		this->CBVSVertexShader.data.matrix_dxxmm = DirectX::XMMatrixTranspose(this->CBVSVertexShader.data.matrix_dxxmm);

		if (!CBVSVertexShader.ApplyChanges())
			return;

		this->deviceContext->VSSetConstantBuffers(0, 1, CBVSVertexShader.GetAddressOf());

		this->CBPSPixelShader.data.alpha = alpha;
		this->CBPSPixelShader.ApplyChanges();

		this->deviceContext->PSSetConstantBuffers(0, 1, this->CBPSPixelShader.GetAddressOf());

		this->deviceContext->PSSetShaderResources(0, 1, this->pavementTexture.GetAddressOf());
		this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
		this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		this->deviceContext->RSSetState(this->rasterizerStateCullFront.Get());
		this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
		this->deviceContext->RSSetState(this->rasterizerState.Get());
		this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
	}

	static int FPSCounter = 0;
	static std::string FPSString = "FPS: 0";
	static const double REFRESH_RATE_MILISECONDS = 1000.0;
	++FPSCounter;
	if (FPSCountTimer.GetMilisecondsElapsed() > REFRESH_RATE_MILISECONDS) {
		FPSString = "FPS: " + std::to_string(FPSCounter);
		FPSCounter = 0;
		FPSCountTimer.Restart();
	}

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), helpers::converter::StringToWString(FPSString).c_str(), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Translation");
	ImGui::DragFloat(":Alpha", &alpha, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3(":X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	this->swapChain->Present(0, NULL);
}
