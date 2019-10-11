#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Render.h"



struct tVertex
{
	D3DXVECTOR3 tvrtPos;
	D3DXCOLOR tvrtColor;
	D3DXVECTOR2 tvrtTexCord;
	D3DXVECTOR3 tvrtNormal;
};

class Terrain
{
private:
	float MaxHeight;

	//Teren
	unsigned int MaxTerrains;

	//ilosc
	unsigned int NumOfIndexes;
	unsigned int NumOfvertexes;
	unsigned int NumOfFaces;

	//grid 
	unsigned int GridX;
	unsigned int GridY;
	unsigned int MaxGrid;
	float GridLength;


	ID3D11Buffer *tvertexBuffer;
	ID3D11Buffer *tindexBuffer;

	tVertex *tVerts;
	uint16_t *tIndices;
public:
	Terrain(void);
	~Terrain(void);
	Terrain(const Terrain& c);
	Terrain& operator=(const Terrain& c);

	void Terrain::GenerateTerrain(ID3D11Device* device, unsigned int numgridx, unsigned int numgridy, float length);
	void Terrain::GenerateTerrainV(ID3D11Device* device, unsigned int numgridx, unsigned int numgridy, float length);

	void Terrain::Draw(ID3D11DeviceContext* devicecontext);
	void Terrain::DrawIndexed(ID3D11DeviceContext* device);
	void Terrain::ReGenerateTerrain(ID3D11Device* device);
	void Terrain::ReleaseBuffer(void);
	void Terrain::Clean(ID3D11DeviceContext* devicecontext);

	void Terrain::GenerateTerrainChunk(ID3D11Device* device,
		unsigned int MaxTerrains, unsigned int numgridx,
		unsigned int numgridy, unsigned int length, unsigned int numgrids);

	void Terrain::RegenIndices(ID3D11Device* device, unsigned int numgrids);

	void Terrain::FillBuffer(ID3D11Device* device);

	D3DXVECTOR3 Terrain::GetVertexPos(int index);
	tVertex* Terrain::GetVertexData(void);

	void Terrain::SetVertexData(tVertex* tverts, unsigned int numofvertexes);
	void Terrain::SetVertexDataY(int index, float posy);

	void Terrain::SetIndexData(uint16_t* tindices, unsigned int numofindices);
	void Terrain::SetNumIndices(unsigned int numofindices);
	void Terrain::SetNumVertexs(unsigned int numofvertexes);
	void Terrain::SetNumFaces(unsigned int numoffaces);
	void Terrain::SetGridX(unsigned int numgridx);
	void Terrain::SetGridY(unsigned int numgridy);
	void Terrain::SetMaxTerrains(unsigned int maxchunks);
	void Terrain::SetMaxGrid(unsigned int maxgrid);
	void Terrain::SetGridLength(float gridllength);

	uint16_t* Terrain::GetIndicesData(void);
	unsigned int Terrain::GetNumIndices(void);
	unsigned int Terrain::GetNumVertexs(void);
	unsigned int Terrain::GetNumFaces(void);
	unsigned int Terrain::GetGridX(void);
	unsigned int Terrain::GetGridY(void);
	unsigned int Terrain::GetMaxTerrains(void);
	unsigned int Terrain::GetMaxGrid(void);
	float Terrain::GetGridLength(void);


	void Terrain::SetGridXY(unsigned int numgridx, unsigned int numgridy);

	float Terrain::GetFullLength(void);

	float Terrain::GetCenterXPos(void);
	float Terrain::GetCenterYPos(void);
	float Terrain::GetCenterZPos(void);

	void Terrain::DeleteSelf(void);
};














