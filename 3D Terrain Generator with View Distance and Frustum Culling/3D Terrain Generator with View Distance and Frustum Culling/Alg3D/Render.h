#pragma once
#include "Global.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>
#include <iostream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


using namespace DirectX;

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
private:
	// Device
	ID3D11Device *Device;                     
	ID3D11DeviceContext *DeviceContext;           
	ID3D11RenderTargetView *RenderTargetView;    
	IDXGISwapChain *SwapChain;             

	// Rasterizer
	D3D11_RASTERIZER_DESC rasterizerState;
	ID3D11RasterizerState* pRS;

	// Depth Stencil
	ID3D11DepthStencilState* DepthStencilState;
	ID3D11DepthStencilView* depthStencilView;


	//Depth
	float NearZ;
	float FarZ;

	/*
#ifdef ENVIRONMENT64
	//Vectors
	DirectX::XMVECTOR Eye;
	DirectX::XMVECTOR At;
	DirectX::XMVECTOR Up;
#else
	DirectX::XMFLOAT4 Eye;
	DirectX::XMFLOAT4 At;
	DirectX::XMFLOAT4 Up;
	XMFLOAT4X4 f_ProjectionMatrix;
	XMFLOAT4X4 f_ViewMatrix;
#endif
	*/

public:
	/*
#ifdef ENVIRONMENT64
	//Matrix
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX ProjectionMatrix;
	DirectX::XMMATRIX ViewMatrix;
#else
	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT4X4 ProjectionMatrix;
	DirectX::XMFLOAT4X4 ViewMatrix;
#endif
	*/

	//Matrix
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMMATRIX ProjectionMatrix;
	DirectX::XMMATRIX ViewMatrix;
	void Renderer::Initialize(ID3D11Device* rDevice, ID3D11DeviceContext* rDeviceContext, D3D_FEATURE_LEVEL rDeviceFeatureLevel);
	void Renderer::Initialize(HWND hwnd);
	void Renderer::SetScene(void);
	void Renderer::PresentScenece(void);
	void Renderer::ChangeFillMode(void);
	void Renderer::ChangeCullMode(void);
	void Renderer::ChangeFrontCounterClockwise(void);

	ID3D11Device* Renderer::GetDevice(void);
	ID3D11DeviceContext* Renderer::GetDeviceContext(void);


#ifdef ENVIRONMENT32
	DirectX::XMFLOAT4X4 Renderer::GetWorldMatrix(void);
	DirectX::XMFLOAT4X4 Renderer::GetViewdMatrix(void);
	DirectX::XMFLOAT4X4 Renderer::GetProjectionMatrix(void);
#else
	const   DirectX::XMMATRIX& Renderer::GetWorldMatrix(void);
	const	DirectX::XMMATRIX& Renderer::GetViewdMatrix(void);
	const	DirectX::XMMATRIX& Renderer::GetProjectionMatrix(void);
#endif

	float Renderer::GetFarZ(void);
	float Renderer::GetNearZ(void);
};

