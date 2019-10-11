#include "Shader.h"

using namespace DirectX;

Shader::Shader(void)
{
	cbBuffer = nullptr;
	sVertexShader = nullptr;
	sPixelShader = nullptr;
	sVertexInputLayout = nullptr;

	//texture
	sShaderResourceView = nullptr;
}


Shader::~Shader(void)
{
	if(cbBuffer)
	{
		cbBuffer->Release();
		cbBuffer = nullptr;
	}
	if(sVertexShader)
	{
		sVertexShader->Release();
		sVertexShader = nullptr;
	}
	if(sPixelShader)
	{
		sPixelShader->Release();
		sPixelShader = nullptr;
	}
	if(sVertexInputLayout)
	{
		sVertexInputLayout->Release();
		sVertexInputLayout = nullptr;
	}
	if(sShaderResourceView)
	{
		sShaderResourceView->Release();
		sShaderResourceView = nullptr;
	}
}

HRESULT Shader::CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	
	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if(FAILED(hr))
	{
		if(pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if(pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if(pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void Shader::InitializeShader(ID3D11Device* device, ID3D11DeviceContext* devicecontext)
{
	/*	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};*/
	ID3DBlob* VSBlob = NULL;
	ID3DBlob* PSBlob = NULL;
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);


	hr = CompileShaderFromFile("shader.fx", "VS", "vs_4_0", &VSBlob);
	if(FAILED(hr))
	{
		MessageBox(NULL, "Cannot find FX file", "Error", MB_OK);
	}
	device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(),
		NULL, &sVertexShader);
	device->CreateInputLayout(layout, numElements, VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(), &sVertexInputLayout);
	VSBlob->Release();
	
	devicecontext->IASetInputLayout(sVertexInputLayout);


	hr = CompileShaderFromFile("shader.fx", "PS", "ps_4_0", &PSBlob);
	if(FAILED(hr))
	{
		MessageBox(NULL, "Cannot find FX file", "Error", MB_OK);
	}
	device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(),
		NULL, &sPixelShader);
	PSBlob->Release();

	//////////////////////////////       texture	/////////////////////////////////////
	hr =	D3DX11CreateShaderResourceViewFromFile(device, "texture.jpg", 0,0, &sShaderResourceView,0);
	if(FAILED(hr))
	{
		MessageBox(NULL, "Plik texture.jpg nie istnieje", "Error", MB_OK);
	}
	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Texture2D *texture2D;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
//	textureDesc.SampleDesc.Count = 1;
//s	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	textureDesc.Width = 512;
	textureDesc.Height = 512;


	if(FAILED(hr))
	{
		device->CreateTexture2D(&textureDesc, 0, &texture2D);
		MessageBox(NULL, "Nie mozna dolaczyc pliku texture.jpg", "Error", MB_OK);
	}
	/*
	// Various settings like sampling 
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	*/
	/*
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	*/
	/*
	ID3D11SamplerState* SamplerState;
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState( &sampDesc, &SamplerState );
	devicecontext->PSSetSamplers( 0, 1, &SamplerState);
	*/
	devicecontext->PSSetShaderResources( 0, 1, &sShaderResourceView );

	//	device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);


	/////////////////////////////////////////////////////////////////////////////////
}


void Shader::InitializeCbuffer(ID3D11Device* device, ID3D11DeviceContext* devicecontext)
{
	D3D11_BUFFER_DESC cbBufferDesc;
	
	cbBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cbBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	cbBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBufferDesc.CPUAccessFlags = 0;
	cbBufferDesc.MiscFlags = 0;
	cbBufferDesc.StructureByteStride = 0;


	device->CreateBuffer(&cbBufferDesc, NULL, &cbBuffer);
}

void Shader::Render(ID3D11DeviceContext* devicecontext,
					unsigned int numindex,
					const DirectX::XMMATRIX  &xworld,
					const DirectX::XMMATRIX  &xview,
					const DirectX::XMMATRIX  &xprojection)
{
	ConstantBuffer cb;
	//	DirectX::XMMATRIX world = xworld;
	//	DirectX::XMMATRIX view = xview;
	//	DirectX::XMMATRIX projection = xprojection;

	cb.mWorld = DirectX::XMMatrixTranspose(xworld);
	cb.mView = DirectX::XMMatrixTranspose(xview);
	cb.mProjection = DirectX::XMMatrixTranspose(xprojection);
	devicecontext->UpdateSubresource(cbBuffer, 0, NULL, &cb, 0, 0);

	devicecontext->PSSetShader(sPixelShader, NULL, 0);
	devicecontext->VSSetShader(sVertexShader, NULL, 0);
	devicecontext->VSSetConstantBuffers(0, 1, &cbBuffer);

	devicecontext->DrawIndexed(numindex, 0, 0);
}
#ifdef ENVIRONMENT64
void Shader::Render(ID3D11DeviceContext* devicecontext,
					unsigned int numindex,
					const DirectX::XMMATRIX  &xworld,
					const DirectX::XMMATRIX  &xview,
					const DirectX::XMMATRIX  &xprojection,
					float TIME)
{
	ConstantBuffer cb;
	//	DirectX::XMMATRIX world = xworld;
	//	DirectX::XMMATRIX view = xview;
	//	DirectX::XMMATRIX projection = xprojection;

	cb.mWorld = DirectX::XMMatrixTranspose(xworld);
	cb.mView = DirectX::XMMatrixTranspose(xview);
	cb.mProjection = DirectX::XMMatrixTranspose(xprojection);
//	cb.sTime = TIME;
	devicecontext->UpdateSubresource(cbBuffer, 0, NULL, &cb, 0, 0);

	devicecontext->PSSetShader(sPixelShader, NULL, 0);
	devicecontext->VSSetShader(sVertexShader, NULL, 0);
	devicecontext->VSSetConstantBuffers(0, 1, &cbBuffer);

	devicecontext->DrawIndexed(numindex, 0, 0);
}
#else

void Shader::Render(ID3D11DeviceContext* devicecontext,
					unsigned int numindex,
					 DirectX::XMFLOAT4X4  xworld,
					 DirectX::XMFLOAT4X4  xview,
					 DirectX::XMFLOAT4X4  xprojection,
					float TIME)
{
	ConstantBuffer cb;
	//	DirectX::XMMATRIX world = xworld;
	//	DirectX::XMMATRIX view = xview;
	//	DirectX::XMMATRIX projection = xprojection;
	XMFLOAT4X4 fm_world = xworld;
	XMFLOAT4X4 fm_view = xview;
	XMFLOAT4X4 fm_projection = xprojection;

	cb.mWorld = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&fm_world));
	cb.mView = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&fm_view));
	cb.mProjection = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&fm_projection));
	cb.sTime = TIME;
	devicecontext->UpdateSubresource(cbBuffer, 0, NULL, &cb, 0, 0);

	devicecontext->PSSetShader(sPixelShader, NULL, 0);
	devicecontext->VSSetShader(sVertexShader, NULL, 0);
	devicecontext->VSSetConstantBuffers(0, 1, &cbBuffer);

	devicecontext->DrawIndexed(numindex, 0, 0);
}


#endif
void Shader::RenderV(ID3D11DeviceContext* devicecontext,
					 unsigned int numverts,
					 const DirectX::XMMATRIX  &xworld,
					 const DirectX::XMMATRIX  &xview,
					 const DirectX::XMMATRIX  &xprojection)
{
	ConstantBuffer cb;
	//	DirectX::XMMATRIX world = xworld;
	//	DirectX::XMMATRIX view = xview;
	//	DirectX::XMMATRIX projection = xprojection;

	cb.mWorld = DirectX::XMMatrixTranspose(xworld);
	cb.mView = DirectX::XMMatrixTranspose(xview);
	cb.mProjection = DirectX::XMMatrixTranspose(xprojection);
	devicecontext->UpdateSubresource(cbBuffer, 0, NULL, &cb, 0, 0);

	devicecontext->PSSetShader(sPixelShader, NULL, 0);
	devicecontext->VSSetShader(sVertexShader, NULL, 0);
	devicecontext->VSSetConstantBuffers(0, 1, &cbBuffer);

	devicecontext->Draw(numverts, 0);
}