#include "Render.h"


Renderer::Renderer(void)
{
	SwapChain = 0;              
	Device = 0;                
	DeviceContext = 0;          
	RenderTargetView = 0;      
	pRS = 0;

	DepthStencilState = 0;
	depthStencilView = 0;
	/*
#ifdef ENVIRONMENT64
	Eye = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	At = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
#else 
	Eye = XMFLOAT4(1.0f,0.0f,0.0f,0.0f);
	At = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	Up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
#endif
	*/
	NearZ = 0.1f;
	FarZ = 1000.0f;
}


Renderer::~Renderer(void)
{
	if(Device)
	{
		Device->Release();
		Device = nullptr;
	}
	if(DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext->ClearState();
		DeviceContext = nullptr;
	}
	if(RenderTargetView)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}
	if(SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}
	if(pRS)
	{
		pRS->Release();
		pRS = nullptr;
	}
	if(DepthStencilState)
	{
		DepthStencilState->Release();
		DepthStencilState = nullptr;
	}
	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}
}

void Renderer::Initialize(HWND hwnd)
{
	HRESULT hr = S_OK;


	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = hwnd;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	SwapChainDesc.Windowed = TRUE;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	std::cout << "debug mode" << std::endl;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else

#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&Device,
		&featureLevel,
		&DeviceContext);


	std::ostringstream oss;
	oss << "Blad przy inicjalizacji COM. Error code = 0x" << std::hex << hr << std::endl;
	std::string hexerrorcode = oss.str();

	if(FAILED(hr))
	{
		MessageBox(NULL,
			"Failed to create device", "", MB_OK);
	}

	ID3D11Texture2D *pBackBuffer = NULL;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);


	Device->CreateRenderTargetView(pBackBuffer
		, nullptr, &RenderTargetView);

	pBackBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = wind_width;
	viewport.Height = wind_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	DeviceContext->RSSetViewports(1, &viewport);

	WorldMatrix = DirectX::XMMatrixIdentity();
	//	ViewMatrix = DirectX::XMMatrixLookAtLH( Eye, At, Up );
	ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(75.0f), wind_width / wind_height, NearZ, FarZ);

	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = true;
	rasterizerState.DepthBiasClamp = 1;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;

	Device->CreateRasterizerState(&rasterizerState, &pRS);
	DeviceContext->RSSetState(pRS);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ID3D11Texture2D* depthStencilBuffer;

	depthStencilDesc.Width = wind_width;
	depthStencilDesc.Height = wind_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	depthStencilBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, depthStencilView);

/*
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 0);
	*/


	//	SwapChain->Present(0,0);	

}

void Renderer::ChangeFillMode(void)
{
	if(rasterizerState.FillMode == D3D11_FILL_WIREFRAME)
	{
		rasterizerState.FillMode = D3D11_FILL_SOLID;
	}
	else
	{
		rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
	}
	Device->CreateRasterizerState(&rasterizerState, &pRS);
	DeviceContext->RSSetState(pRS);
}

void Renderer::ChangeCullMode(void)
{
	if(rasterizerState.CullMode == D3D11_CULL_BACK)
	{
		rasterizerState.CullMode = D3D11_CULL_FRONT;
		std::cout << "FRONT" << std::endl;
	}
	else if(rasterizerState.CullMode == D3D11_CULL_FRONT)
	{
		rasterizerState.CullMode = D3D11_CULL_NONE;
		std::cout << "NONE" << std::endl;
	}
	else
	{
		rasterizerState.CullMode = D3D11_CULL_BACK;
		std::cout << "BACK" << std::endl;
	}
	Device->CreateRasterizerState(&rasterizerState, &pRS);
	DeviceContext->RSSetState(pRS);
}

void Renderer::ChangeFrontCounterClockwise(void)
{
	if(rasterizerState.FrontCounterClockwise == false)
	{
		rasterizerState.FrontCounterClockwise = true;
		std::cout << "FCC: TRUE" << std::endl;
	}
	else
	{
		rasterizerState.FrontCounterClockwise = false;
		std::cout << "FCC: FALSE" << std::endl;
	}
	Device->CreateRasterizerState(&rasterizerState, &pRS);
	DeviceContext->RSSetState(pRS);
}

void Renderer::SetScene(void)
{
	DeviceContext->ClearRenderTargetView(RenderTargetView, D3DXCOLOR(0.08f, 0.16f, 0.2f, 1.0f));
	DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::PresentScenece(void)
{
	SwapChain->Present(0, 0);
}

float Renderer::GetFarZ(void)
{
	return FarZ;
}

float Renderer::GetNearZ(void)
{
	return NearZ;
}

ID3D11Device* Renderer::GetDevice(void)
{
	return this->Device;
}

ID3D11DeviceContext* Renderer::GetDeviceContext(void)
{
	return DeviceContext;
}

#ifdef ENVIRONMENT32
DirectX::XMFLOAT4X4 Renderer::GetWorldMatrix(void)
{
	XMFLOAT4X4 f_WorldMatrix;
	XMStoreFloat4x4(&f_WorldMatrix, WorldMatrix);
	return f_WorldMatrix;
}
DirectX::XMFLOAT4X4 Renderer::GetViewdMatrix(void)
{
	XMFLOAT4X4 f_ViewMatrix;
	XMStoreFloat4x4(&f_ViewMatrix, ViewMatrix);
	return f_ViewMatrix;
}
DirectX::XMFLOAT4X4 Renderer::GetProjectionMatrix(void)
{
	XMFLOAT4X4 f_ProjectionMatrix;
	XMStoreFloat4x4(&f_ProjectionMatrix, ProjectionMatrix);
	return f_ProjectionMatrix;
}
#else
const   DirectX::XMMATRIX& Renderer::GetWorldMatrix(void)
{
	return WorldMatrix;
}
const	DirectX::XMMATRIX& Renderer::GetViewdMatrix(void)
{
	return ViewMatrix;
}
const	DirectX::XMMATRIX& Renderer::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}
#endif