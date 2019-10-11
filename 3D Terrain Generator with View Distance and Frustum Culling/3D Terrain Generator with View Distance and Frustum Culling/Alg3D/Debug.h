#pragma once
#include <d3d11.h>
#include <d3dx10.h>

class Debug
{

public:
	Debug(void);
	~Debug(void);
private:
	ID3D11Buffer *dvertexBuffer;
	ID3D11Buffer *dindexBuffer;
	ID3D11InputLayout *dVertexLayout;

	struct dVertex
	{
		D3DXVECTOR3 dvrtPos;
		D3DXCOLOR dvrtColor;
	};

	ID3DX10Font* font;

public:
	int numverts;
	void Debug::dDrawText(void);

	void Debug::Init(ID3D11Device* device);
	void Debug::Render(ID3D11DeviceContext* devicecontext);
	void Debug::InitwPos(ID3D11Device* device, ID3D11DeviceContext* devicecontext,
		D3DXVECTOR3 CamPos, D3DXVECTOR3 LookAt);


};

