#pragma once

#include "Global.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class Shader
{
public:
	Shader(void);
	~Shader(void);
private:
	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
		float sTime;
	};

	ID3D11VertexShader* sVertexShader;
	ID3D11PixelShader* sPixelShader;
	ID3D11InputLayout* sVertexInputLayout;

	ID3D11Buffer* cbBuffer;
	
	//texture
	ID3D11ShaderResourceView* sShaderResourceView;

public:
	HRESULT Shader::CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void Shader::InitializeShader(ID3D11Device* device, ID3D11DeviceContext* devicecontext);
	void Shader::InitializeCbuffer(ID3D11Device* device, ID3D11DeviceContext* devicecontext);
	void Shader::Render(ID3D11DeviceContext* device,
		unsigned int numindex,
		const DirectX::XMMATRIX  &xworld,
		const DirectX::XMMATRIX  &xview,
		const DirectX::XMMATRIX  &xprojection);
	void Shader::PassTime(float sTime);

	void Shader::RenderV(ID3D11DeviceContext* devicecontext,
		unsigned int numverts,
		const DirectX::XMMATRIX  &xworld,
		const DirectX::XMMATRIX  &xview,
		const DirectX::XMMATRIX  &xprojection);

#ifdef ENVIRONMENT64
void Shader::Render(ID3D11DeviceContext* devicecontext,
					unsigned int numindex,
					const DirectX::XMMATRIX  &xworld,
					const DirectX::XMMATRIX  &xview,
					const DirectX::XMMATRIX  &xprojection,
					float TIME);
#else
void Shader::Render(ID3D11DeviceContext* devicecontext,
					unsigned int numindex,
					 DirectX::XMFLOAT4X4  xworld,
					 DirectX::XMFLOAT4X4  xview,
					 DirectX::XMFLOAT4X4  xprojection,
					float TIME);
#endif

#ifdef ENVIRONMENT64
	//void Shader::CreateFrustum(float FarZ ,DirectX::XMMATRIX projection);
#else

//	void Shader::CreateFrustum(float FarZ ,const DirectX::XMMATRIX &projection);
#endif

};

