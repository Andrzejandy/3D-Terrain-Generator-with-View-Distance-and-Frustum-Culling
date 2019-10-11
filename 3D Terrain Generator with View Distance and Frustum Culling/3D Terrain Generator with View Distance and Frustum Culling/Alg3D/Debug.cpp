#include "Debug.h"
#include <iostream>

Debug::Debug(void)
{
	font = NULL;

}


Debug::~Debug(void)
{

}

void Debug::Init(ID3D11Device* device)
{
	dVertex* vertices;

	D3D11_BUFFER_DESC dVertexBufferDesc;
	D3D11_SUBRESOURCE_DATA dVertexSubResData;
	//vertices = new dVertex[3];
	numverts = 6;
	vertices = new dVertex[numverts];
	//x
		vertices[0].dvrtPos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//x
	vertices[0].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
		vertices[1].dvrtPos = D3DXVECTOR3(3.0f,0.0f,0.0f);	//x
	vertices[1].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	//y
		vertices[2].dvrtPos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//y
	vertices[2].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
		vertices[3].dvrtPos = D3DXVECTOR3(0.0f,3.0f,0.0f);	//y
	vertices[3].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

		vertices[4].dvrtPos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//z
	vertices[4].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
		vertices[5].dvrtPos = D3DXVECTOR3(0.0f,0.0f,3.0f);	//z
	vertices[5].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
		
	dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dVertexBufferDesc.ByteWidth = sizeof(dVertex) * (numverts);
	dVertexBufferDesc.CPUAccessFlags = 0;
	dVertexBufferDesc.MiscFlags = 0;
	dVertexBufferDesc.StructureByteStride = 0;
	dVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	dVertexSubResData.pSysMem = vertices;
	dVertexSubResData.SysMemPitch = 0;
	dVertexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&dVertexBufferDesc, &dVertexSubResData, &dvertexBuffer);
	
	delete[] vertices;
	vertices = NULL;
}


void Debug::InitwPos(ID3D11Device* device,ID3D11DeviceContext* devicecontext, D3DXVECTOR3 CamPos, 
					 D3DXVECTOR3 LookAt)
{
	dVertex* vertices;
	static bool created = false;
	D3D11_BUFFER_DESC dVertexBufferDesc;
	D3D11_SUBRESOURCE_DATA dVertexSubResData;
	ZeroMemory(&dVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&dVertexSubResData, sizeof(D3D11_SUBRESOURCE_DATA));
	//vertices = new dVertex[3];
	numverts = 8;
	vertices = new dVertex[numverts];
	/*
	//x
		vertices[0].dvrtPos = D3DXVECTOR3(1.0f,1.0f,1.0f);	//x
	vertices[0].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
		vertices[1].dvrtPos = D3DXVECTOR3(2.0f,1.0f,1.0f);	//x
	vertices[1].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	//y
		vertices[2].dvrtPos = D3DXVECTOR3(1.0f,1.0f,1.0f);	//y
	vertices[2].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
		vertices[3].dvrtPos = D3DXVECTOR3(1.0f,2.0f,1.0f);	//y
	vertices[3].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

		vertices[4].dvrtPos = D3DXVECTOR3(1.0f,1.0f,1.0f);	//z
	vertices[4].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
		vertices[5].dvrtPos = D3DXVECTOR3(1.0f,1.0f,2.0f);	//z
	vertices[5].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
		*/

		//x
		vertices[0].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+1.0f,CamPos.z+1.0f);	//x
	vertices[0].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
		vertices[1].dvrtPos = D3DXVECTOR3(CamPos.x+2.0f,CamPos.y+1.0f,CamPos.z+1.0f);	//x
	vertices[1].dvrtColor = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	//y
		vertices[2].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+1.0f,CamPos.z+1.0f);	//y
	vertices[2].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
		vertices[3].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+2.0f,CamPos.z+1.0f);	//y
	vertices[3].dvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);

		vertices[4].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+1.0f,CamPos.z+1.0f);	//z
	vertices[4].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
		vertices[5].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+1.0f,CamPos.z+2.0f);	//z
	vertices[5].dvrtColor = D3DXCOLOR(0.0f,0.0f,1.0f,1.0f);
	
	vertices[6].dvrtPos = D3DXVECTOR3(CamPos.x+1.0f,CamPos.y+1.0f,CamPos.z+1.0f);	//At
	vertices[6].dvrtColor = D3DXCOLOR(0.0f,0.7f,1.0f,1.0f);
	//vertices[7].dvrtPos = D3DXVECTOR3(LookAt.x+CamPos.x,LookAt.y+CamPos.y,LookAt.z+CamPos.z);	//At

	vertices[7].dvrtPos = D3DXVECTOR3(LookAt.x+CamPos.x,LookAt.y+CamPos.y,LookAt.z+CamPos.z);	//At
	vertices[7].dvrtColor = D3DXCOLOR(1.0f,1.0f,0.1f,1.0f);

//	std::cout << "lookAt: x:" << LookAt.x << " y:" << LookAt.y << " z:" << LookAt.z << std::endl;

	dVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dVertexBufferDesc.ByteWidth = sizeof(dVertex) * (numverts);
	dVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dVertexBufferDesc.MiscFlags = 0;
	dVertexBufferDesc.StructureByteStride = 0;
	dVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	dVertexSubResData.pSysMem = vertices;
	dVertexSubResData.SysMemPitch = 0;
	dVertexSubResData.SysMemSlicePitch = 0;

	if(created == false)
	{
		device->CreateBuffer(&dVertexBufferDesc, &dVertexSubResData, &dvertexBuffer);
		created = true;
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE map_subres;
		ZeroMemory(&map_subres, sizeof(D3D11_MAPPED_SUBRESOURCE));
		devicecontext->Map(dvertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map_subres);
		memcpy(map_subres.pData, vertices, sizeof(dVertex)*numverts);
		devicecontext->Unmap(dvertexBuffer, 0);
	}
	delete[] vertices;
	vertices = NULL;
}


void Debug::Render(ID3D11DeviceContext* devicecontext)
{
	unsigned int stride, offset;
	stride = sizeof(dVertex);
	offset = 0;
	devicecontext->IASetVertexBuffers(0,1, &dvertexBuffer,&stride,&offset);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
}

void Debug::dDrawText(void)
{
	//font->DrawTextA(
}