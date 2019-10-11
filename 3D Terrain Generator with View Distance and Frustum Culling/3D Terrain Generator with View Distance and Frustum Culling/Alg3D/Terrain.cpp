#include "Terrain.h"

Terrain::Terrain(void)
{
	tvertexBuffer = nullptr;
	tindexBuffer = nullptr;
	MaxTerrains = NULL;
	GridX = NULL;
	GridY = NULL;
	MaxGrid = NULL;
	NumOfIndexes = NULL;
	NumOfvertexes = NULL;
	GridLength = NULL;
	NumOfFaces = NULL;
	tVerts = nullptr;
	tIndices = nullptr;

	MaxHeight = 10.0f;
}


Terrain::Terrain(const Terrain& c) 
{
	GridLength = c.GridLength;
	NumOfIndexes = c.NumOfIndexes;
	NumOfvertexes = c.NumOfvertexes;
	GridX = c.GridX;
	GridY = c.GridY;
	MaxGrid = c.MaxGrid;
	MaxTerrains = c.MaxTerrains;
	MaxHeight = c.MaxHeight;
	NumOfFaces = c.NumOfFaces;
	tvertexBuffer = c.tvertexBuffer;
	if(c.tvertexBuffer != nullptr)
	{	
		//	c.tvertexBuffer->Release();
		//tvertexBuffer->Release();
		//tvertexBuffer = nullptr;
	}

	tindexBuffer = c.tindexBuffer;
	if(c.tindexBuffer != nullptr)
	{		
		//c.tindexBuffer->Release();
		//tindexBuffer->Release();
		//tindexBuffer = nullptr;
	}


	tVerts = new tVertex[MaxGrid*MaxGrid];
	memcpy(tVerts, c.tVerts, sizeof(tVertex)*MaxGrid*MaxGrid);

	tIndices = new uint16_t[NumOfIndexes];
	memcpy(tIndices, c.tIndices, sizeof(uint16_t)*NumOfIndexes);
}


Terrain & Terrain::operator=(const Terrain& c)
{
	if(this==&c)
	{
		return *this;
	}

	this->GridLength = c.GridLength;
	this->NumOfIndexes = c.NumOfIndexes;
	this->NumOfvertexes = c.NumOfvertexes;
	this->GridX = c.GridX;
	this->GridY = c.GridY;
	this->MaxGrid = c.MaxGrid;
	this->MaxTerrains = c.MaxTerrains;
	this->NumOfFaces = c.NumOfFaces;
	this->MaxHeight = c.MaxHeight;
	this->tvertexBuffer = c.tvertexBuffer;
	this->tindexBuffer = c.tindexBuffer;

	//	c.tvertexBuffer->Release();
	//	c.tindexBuffer->Release();
	if(tVerts != nullptr)
	{
		delete[] tVerts;
		tVerts = nullptr;
	}
	if(tIndices != nullptr)
	{
		delete[] tIndices;
		tIndices = nullptr;
	}
	tVerts = new tVertex[MaxGrid*MaxGrid];
	memcpy(tVerts, c.tVerts, sizeof(tVertex)*MaxGrid*MaxGrid);

	tIndices = new uint16_t[NumOfIndexes];
	memcpy(tIndices, c.tIndices, sizeof(uint16_t)*NumOfIndexes);

	return *this;
}

Terrain::~Terrain(void)
{
	if(tVerts != nullptr)
	{
		delete[] tVerts;
		tVerts = nullptr;
	}
	if(tIndices != nullptr)
	{
		delete[] tIndices;
		tIndices = nullptr;
	}
}

void Terrain::Clean(ID3D11DeviceContext* devicecontext)
{
	UINT zero = 0;
	ID3D11Buffer* zeroBuffer = 0;
	devicecontext->IASetVertexBuffers(0, 1, &zeroBuffer, &zero, &zero);
	if(tVerts!= nullptr)
	{
		delete[] tVerts;
		tVerts = nullptr;
	}
	if(tIndices != nullptr)
	{
		delete[] tIndices;
		tIndices = nullptr;
	}
}

void Terrain::ReleaseBuffer(void)
{
	if(tvertexBuffer != nullptr)
	{
		tvertexBuffer->Release();
		ZeroMemory(&tvertexBuffer, sizeof(ID3D11Buffer));
		tvertexBuffer = nullptr;
	}
	if(tindexBuffer != nullptr)
	{
		tindexBuffer->Release();
		ZeroMemory(&tindexBuffer, sizeof(ID3D11Buffer));
		tindexBuffer = nullptr;
	}
}



void Terrain::DeleteSelf(void)
{

}

D3DXVECTOR3 Terrain::GetVertexPos(int index)
{
	return tVerts[index].tvrtPos;
}

tVertex* Terrain::GetVertexData(void)
{
	return tVerts;
}

void Terrain::SetVertexData(tVertex* tverts, unsigned int numofvertexes)
{
	memcpy(tVerts, tverts, sizeof(tVertex)*numofvertexes);
}

void Terrain::SetIndexData(uint16_t* tindices, unsigned int numofindices)
{
	memcpy(tIndices, tindices, sizeof(uint16_t)*numofindices);
}


uint16_t* Terrain::GetIndicesData(void)
{
	return tIndices;
}

unsigned int Terrain::GetGridX(void)
{
	return GridX;
}

unsigned int Terrain::GetGridY(void)
{
	return GridY;
}

void Terrain::SetGridX(unsigned int numgridx)
{
	GridX = numgridx;
}

void Terrain::SetGridY(unsigned int numgridy)
{
	GridY = numgridy;
}

void Terrain::SetGridXY(unsigned int numgridx, unsigned int numgridy)
{
	GridX = numgridx;
	GridY = numgridy;
}
/*

_ _ _
|_|_|_|			
|_|_|_|	
|_|_|_|

|,_ = linelength
_
|_|= GridX lub GridY

_ _ _
|_|_|_|,
_
|_|		 = chunk_index
|_|
|_|

_
|_|
|_|
.
.		= numgrids
.
|_|
|_|


*/

void Terrain::FillBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexSubResData, IndexSubResData;

	ZeroMemory(&IndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&VertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&VertexSubResData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&IndexSubResData, sizeof(D3D11_SUBRESOURCE_DATA));

	ZeroMemory(&tvertexBuffer, sizeof(ID3D11Buffer));
	ZeroMemory(&tindexBuffer, sizeof(ID3D11Buffer));

	tvertexBuffer = nullptr;
	tindexBuffer = nullptr;

	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.ByteWidth = sizeof(tVertex)* (MaxGrid*MaxGrid);
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VertexSubResData.pSysMem = &tVerts[0];
	VertexSubResData.SysMemPitch = 0;
	VertexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&VertexBufferDesc, &VertexSubResData, &tvertexBuffer);

	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t)* ((MaxGrid-1)*(MaxGrid-1) * 6);
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	IndexSubResData.pSysMem = &tIndices[0];
	IndexSubResData.SysMemPitch = 0;
	IndexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&IndexBufferDesc, &IndexSubResData, &tindexBuffer);
}
// maxchunks - maksymalna ilosc chunkow (ilosc terenow)
// chunk_x, chunk_y - identyfikator chunka (terenu) do obliczenia pozycji
// linelength - dlugosc linii kwadratu, 
// numgrids - ilosc kwadratow w jednym chunku
void Terrain::GenerateTerrainChunk(ID3D11Device* device, unsigned int maxchunks,  unsigned int chunk_x,
								   unsigned int chunk_y, unsigned int linelength, unsigned int numgrids)
{
	MaxTerrains = maxchunks;
	GridX = chunk_x;
	GridY = chunk_y;
	MaxGrid = numgrids + 1;
	GridLength = linelength;
	tVerts = new tVertex[MaxGrid*MaxGrid];
	NumOfvertexes = MaxGrid*MaxGrid;
	NumOfFaces = numgrids*numgrids*2;

	MaxHeight = 1.0f;
int MinHeight = 0.0f;
	for(unsigned int x = 0; x < MaxGrid; x++)
	{
		for(unsigned int y = 0; y < MaxGrid; y++)
		{
			tVerts[y + x*MaxGrid].tvrtPos.x = (float)((x+1) * linelength)
				+ (((MaxGrid - 1) * linelength)*chunk_x) - linelength;
			tVerts[y + x*MaxGrid].tvrtPos.z = (float)((y+1)* linelength)
				+ (((MaxGrid - 1) * linelength)*chunk_y) - linelength;

			float texX = ((float)x)/numgrids;
			float texY = ((float)y)/numgrids;

											//(float)rand() / (0.1 + RAND_MAX);
											//rand() % (MaxHeight - MinHeight)+MinHeight;
			tVerts[y + x*MaxGrid].tvrtPos.y = ((float)rand() / (0.1 + RAND_MAX));
			tVerts[y + x*MaxGrid].tvrtColor = D3DXCOLOR(0.09, 0.9, 0.092, 1.0f);
			//	tVerts[y + x*MaxGrid].tvrtColor = D3DXCOLOR(0, 1*tVerts[y + x*MaxGrid].tvrtPos.y/1, 0, 1.0f);
			tVerts[y + x*MaxGrid].tvrtTexCord = D3DXVECTOR2(texX,texY);


			if(	(int)(((x+1) * linelength)
				+ (((numgrids) * linelength)*chunk_x) - linelength) % numgrids == 0 || 
				(int)(((y+1)* linelength)
				+ (((numgrids) * linelength)*chunk_y) - linelength) % numgrids == 0)
			{
		//		tVerts[y + x*MaxGrid].tvrtColor = D3DXCOLOR(1.0, 0.0, 0.0, 1.0f);
				//	tVerts[y + x*MaxGrid].tvrtPos.y = 2.0f;

			}
			else
			{
		//		tVerts[y + x*MaxGrid].tvrtColor = D3DXCOLOR(0.1, 0.9, 0.2, 1.0f);
			}


			//tVerts[y + x*MaxGrid].tvrtColor = D3DXCOLOR(0, 1*tVerts[y + x*MaxGrid].tvrtPos.y/1, 0, 1.0f);

		}
	}


	//Dokonczyc normalne
	for(unsigned int cur_vert = 0; cur_vert < NumOfvertexes; cur_vert++)
	{
		DirectX::XMFLOAT3 NormalValue;

		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert].tvrtPos.x, tVerts[cur_vert].tvrtPos.y, tVerts[cur_vert].tvrtPos.z));
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert+1].tvrtPos.x, tVerts[cur_vert+1].tvrtPos.y, tVerts[cur_vert+1].tvrtPos.z));
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert+2].tvrtPos.x, tVerts[cur_vert+2].tvrtPos.y, tVerts[cur_vert+2].tvrtPos.z));
		DirectX::XMVECTOR n  = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v2 ,v1), DirectX::XMVectorSubtract(v3 ,v1));
		DirectX::XMStoreFloat3(&NormalValue, n);
		tVerts[cur_vert].tvrtNormal.x = 1.0;
		tVerts[cur_vert].tvrtNormal.y = tVerts[cur_vert].tvrtPos.y + 0.50f;
		tVerts[cur_vert].tvrtNormal.z = 1.0;
	}

	/*
	for(int cur_vert = 0; cur_vert < NumOfvertexes; cur_vert++)
	{
	DirectX::XMFLOAT3 NormalValue;

	DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert].tvrtPos.x, tVerts[cur_vert].tvrtPos.y, tVerts[cur_vert].tvrtPos.z));
	DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert+1].tvrtPos.x, tVerts[cur_vert+1].tvrtPos.y, tVerts[cur_vert+1].tvrtPos.z));
	DirectX::XMVECTOR v3 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(tVerts[cur_vert+2].tvrtPos.x, tVerts[cur_vert+2].tvrtPos.y, tVerts[cur_vert+2].tvrtPos.z));
	DirectX::XMVECTOR n  = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v2 ,v1), DirectX::XMVectorSubtract(v3 ,v1));
	DirectX::XMStoreFloat3(&NormalValue, n);
	tVerts[cur_vert].tvrtNormal.x = NormalValue.x;
	tVerts[cur_vert].tvrtNormal.y = NormalValue.y;
	tVerts[cur_vert].tvrtNormal.z = NormalValue.z;
	}
	*/


	//	std::cout << "gridposx: " << gridposy << std::endl;
	//	std::cout << "x:" << chunk_x << " y:" << chunk_y << " =" << " <" <<
	//	abs(tVerts[1].tvrtPos.x) << "-" << abs(tVerts[numgrids + numgrids*MaxGrid].tvrtPos.x) << ">" << std::endl;

	//gen indeksy
	unsigned int indicesNumGrid = MaxGrid - 1;
	tIndices = new uint16_t[indicesNumGrid*indicesNumGrid * 6];
	NumOfIndexes = indicesNumGrid*indicesNumGrid * 6;
	int offset = 0;
	for(unsigned int z = 0; z < indicesNumGrid; z++)
	{
		for(unsigned int y = 0; y < indicesNumGrid; y++)
		{
			tIndices[offset] = y + (z*MaxGrid);			//0	
			tIndices[offset + 1] = y + (z*MaxGrid) + MaxGrid;		    //2
			tIndices[offset + 2] = y + (z*MaxGrid) + 1;    //1

			tIndices[offset + 3] = y + (z*MaxGrid) + MaxGrid;   //2
			tIndices[offset + 4] = y + (z*MaxGrid) + MaxGrid + 1;			//3
			tIndices[offset + 5] = y + (z*MaxGrid) + 1;    //1
			offset += 6;
		}
	}


}


void Terrain::RegenIndices(ID3D11Device* device, unsigned int numgrids)
{

	D3D11_BUFFER_DESC  IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA  IndexSubResData;
	//gen indeksy
	MaxGrid = numgrids + 1;
	unsigned int indicesNumGrid = MaxGrid - 1;
	uint16_t* indices = new uint16_t[indicesNumGrid*indicesNumGrid * 6];

	NumOfIndexes = indicesNumGrid*indicesNumGrid * 6;
	int offset = 0;
	for(unsigned int z = 0; z < indicesNumGrid; z++)
	{
		for(unsigned int y = 0; y < indicesNumGrid; y++)
		{
			indices[offset] = y + (z*MaxGrid);			//0	
			indices[offset + 1] = y + (z*MaxGrid) + MaxGrid;		    //2
			indices[offset + 2] = y + (z*MaxGrid) + 1;    //1

			indices[offset + 3] = y + (z*MaxGrid) + MaxGrid;   //2
			indices[offset + 4] = y + (z*MaxGrid) + MaxGrid + 1;			//3
			indices[offset + 5] = y + (z*MaxGrid) + 1;    //1
			offset += 6;
		}
	}

	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t)* (indicesNumGrid*indicesNumGrid * 6);
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	IndexSubResData.pSysMem = indices;
	IndexSubResData.SysMemPitch = 0;
	IndexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&IndexBufferDesc, &IndexSubResData, &tindexBuffer);

	delete[] indices;
	indices = NULL;
}

void Terrain::GenerateTerrainV(ID3D11Device* device, unsigned int numgridx, unsigned int numgridy, float length)
{
	ReleaseBuffer();

	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexSubResData;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA IndexSubResData;
	tVertex* vert;
	GridX = numgridx + 1;
	GridY = numgridy + 1;
	vert = new tVertex[GridX*GridY];
	NumOfvertexes = GridX*GridY;
	GridLength = length;
	for(unsigned int z = 0; z < GridX; z++)
	{
		for(unsigned int y = 0; y < GridY; y++)
		{
			vert[z + y*GridY].tvrtPos.x = (float)y * GridLength;
			vert[z + y*GridY].tvrtPos.z = (float)z * GridLength;
			vert[z + y*GridY].tvrtPos.y = (float)rand() / (0.1 + RAND_MAX);
			//	vert[z+y*GridY].tvrtPos.y = (float)0.0f;
			//	vert[z+y*GridY].tvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
			vert[z + y*GridY].tvrtColor =
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.ByteWidth = sizeof(tVertex)* (GridX*GridY);
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VertexSubResData.pSysMem = vert;
	VertexSubResData.SysMemPitch = 0;
	VertexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&VertexBufferDesc, &VertexSubResData, &tvertexBuffer);


	//gen indeksy
	uint16_t* indices;
	indices = new uint16_t[numgridx*numgridy * 6];
	NumOfIndexes = numgridx*numgridy * 6;
	int index = 0;
	for(unsigned int z = 0; z < numgridx; z++)
	{
		for(unsigned int y = 0; y < numgridy; y++)
		{
			indices[index] = y + (z*GridY);			//0	
			indices[index + 1] = y + (z*GridY) + GridY;		    //2
			indices[index + 2] = y + (z*GridY) + 1;    //1

			indices[index + 3] = y + (z*GridY) + GridY;   //2
			indices[index + 4] = y + (z*GridY) + GridY + 1;			//3
			indices[index + 5] = y + (z*GridY) + 1;    //1
			index += 6;
		}
	}

	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t)* (numgridx*numgridy * 6);
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	IndexSubResData.pSysMem = indices;
	IndexSubResData.SysMemPitch = 0;
	IndexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&IndexBufferDesc, &IndexSubResData, &tindexBuffer);


	delete[] vert;
	delete[] indices;
	vert = NULL;
	indices = NULL;


}

void Terrain::GenerateTerrain(ID3D11Device* device, unsigned int numgridx, unsigned int numgridy, float length)
{
	ReleaseBuffer();

	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexSubResData, IndexSubResData;
	tVertex* vert;
	GridX = numgridx + 1;
	GridY = numgridy + 1;
	vert = new tVertex[GridX*GridY];
	NumOfvertexes = GridX*GridY;
	GridLength = length;
	for(unsigned int z = 0; z < GridX; z++)
	{
		for(unsigned int y = 0; y < GridY; y++)
		{
			vert[z + y*GridY].tvrtPos.x = (float)y * GridLength;
			vert[z + y*GridY].tvrtPos.z = (float)z * GridLength;
			vert[z + y*GridY].tvrtPos.y = (float)rand() / (0.1 + RAND_MAX);
			//	vert[z+y*GridY].tvrtPos.y = (float)0.0f;
			//	vert[z+y*GridY].tvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
			vert[z + y*GridY].tvrtColor = D3DXCOLOR(0.0f, vert[z + y*GridY].tvrtPos.y, 0.0f, 1.0f);
		}
	}
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.ByteWidth = sizeof(tVertex)* (GridX*GridY);
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VertexSubResData.pSysMem = vert;
	VertexSubResData.SysMemPitch = 0;
	VertexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&VertexBufferDesc, &VertexSubResData, &tvertexBuffer);

	//gen indeksy
	uint16_t* indices;
	indices = new uint16_t[numgridx*numgridy * 6];
	NumOfIndexes = numgridx*numgridy * 6;
	int index = 0;
	for(unsigned int z = 0; z < numgridx; z++)
	{
		for(unsigned int y = 0; y < numgridy; y++)
		{
			indices[index] = y + (z*GridY);			//0	
			indices[index + 1] = y + (z*GridY) + GridY;		    //2
			indices[index + 2] = y + (z*GridY) + 1;    //1

			indices[index + 3] = y + (z*GridY) + GridY;   //2
			indices[index + 4] = y + (z*GridY) + GridY + 1;			//3
			indices[index + 5] = y + (z*GridY) + 1;    //1
			index += 6;
		}
	}

	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t)* (numgridx*numgridy * 6);
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	IndexSubResData.pSysMem = indices;
	IndexSubResData.SysMemPitch = 0;
	IndexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&IndexBufferDesc, &IndexSubResData, &tindexBuffer);


	delete[] vert;
	delete[] indices;
	vert = NULL;
	indices = NULL;
}

void Terrain::ReGenerateTerrain(ID3D11Device* device)
{
	srand(time(NULL));
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexSubResData, IndexSubResData;
	tVertex* vert;
	vert = new tVertex[GridX*GridY];
	NumOfvertexes = GridX*GridY;

	for(unsigned int z = 0; z < GridX; z++)
	{
		for(unsigned int y = 0; y < GridY; y++)
		{
			vert[z + y*GridY].tvrtPos.x = (float)y * GridLength;
			vert[z + y*GridY].tvrtPos.z = (float)z * GridLength;
			vert[z + y*GridY].tvrtPos.y = (float)rand() / (0.2 + RAND_MAX);
			//	vert[z+y*GridY].tvrtPos.y = (float)0.0f;
			//	vert[z+y*GridY].tvrtColor = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
			vert[z + y*GridY].tvrtColor = D3DXCOLOR(0.0f, vert[z + y*GridY].tvrtPos.y, 0.0f, 1.0f);

		}
	}
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.ByteWidth = sizeof(tVertex) * (GridX*GridY);
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	VertexSubResData.pSysMem = vert;
	VertexSubResData.SysMemPitch = 0;
	VertexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&VertexBufferDesc, &VertexSubResData, &tvertexBuffer);

	// 1----3-4---
	// |    |    | 
	// |    |    | 
	//2|5---6----|

	uint16_t* indices;
	indices = new uint16_t[(GridX - 1)*(GridX - 1) * 6];
	NumOfIndexes = (GridX - 1)*(GridX - 1) * 6;
	int index = 0;
	for(unsigned int z = 0; z < (GridX - 1); z++)
	{
		for(unsigned int y = 0; y < (GridY - 1); y++)
		{
			indices[index] = y + (z*GridY);			//0	
			indices[index + 1] = y + (z*GridY) + GridY;		    //2
			indices[index + 2] = y + (z*GridY) + 1;    //1

			indices[index + 3] = y + (z*GridY) + GridY;   //2
			indices[index + 4] = y + (z*GridY) + GridY + 1;			//3
			indices[index + 5] = y + (z*GridY) + 1;    //1
			index += 6;
		}
	}

	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t)* ((GridX - 1)*(GridY - 1) * 6);
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	IndexSubResData.pSysMem = indices;
	IndexSubResData.SysMemPitch = 0;
	IndexSubResData.SysMemSlicePitch = 0;

	device->CreateBuffer(&IndexBufferDesc, &IndexSubResData, &tindexBuffer);


	delete[] vert;
	delete[] indices;
	vert = NULL;
	indices = NULL;
}

void Terrain::Draw(ID3D11DeviceContext* devicecontext)
{

	unsigned int stride, offset;
	stride = sizeof(tVertex);
	offset = 0;
	devicecontext->IASetVertexBuffers(0, 1, &tvertexBuffer, &stride, &offset);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	devicecontext->Draw(NumOfvertexes,0);

}

void Terrain::DrawIndexed(ID3D11DeviceContext* devicecontext)
{
	unsigned int stride, offset;
	stride = sizeof(tVertex);
	offset = 0;
	devicecontext->IASetVertexBuffers(0, 1, &tvertexBuffer, &stride, &offset);
	devicecontext->IASetIndexBuffer(tindexBuffer, DXGI_FORMAT_R16_UINT, 0);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	devicecontext->DrawIndexed(NumOfIndexes,0,0);
}

void Terrain::SetVertexDataY(int index, float posy)
{
	tVerts[index].tvrtPos.y = posy;
}

float Terrain::GetCenterXPos(void)
{
	return tVerts[((MaxGrid*MaxGrid)/2)-1].tvrtPos.x;
}
float Terrain::GetCenterYPos(void)
{
	return MaxHeight/2;
	//return tVerts[((MaxGrid-1)+(MaxGrid-1)*MaxGrid)/2].tvrtPos.y;
}

float Terrain::GetCenterZPos(void)
{
	return tVerts[((MaxGrid*MaxGrid)/2)-1].tvrtPos.z;
}

float Terrain::GetFullLength(void)
{
	return MaxGrid*GridLength;
}

float Terrain::GetGridLength(void)
{
	return GridLength;
}

void Terrain::SetGridLength(float gridllength)
{
	GridLength = gridllength;
}

unsigned int Terrain::GetNumIndices(void)
{
	return NumOfIndexes;
}

void Terrain::SetNumIndices(unsigned int numofindices)
{
	NumOfIndexes = numofindices;
}

unsigned int Terrain::GetNumVertexs(void)
{
	return NumOfvertexes;
}

void Terrain::SetNumVertexs(unsigned int numofvertexes)
{
	NumOfvertexes = numofvertexes;
}

unsigned int Terrain::GetNumFaces(void)
{
	return NumOfFaces;
}

void Terrain::SetNumFaces(unsigned int numoffaces)
{
	NumOfFaces = numoffaces;
}

unsigned int Terrain::GetMaxTerrains(void)
{
	return MaxTerrains;
}

void Terrain::SetMaxTerrains(unsigned int maxchunks)
{
	MaxTerrains = maxchunks;
}

unsigned int Terrain::GetMaxGrid(void)
{
	return MaxGrid;
}

void Terrain::SetMaxGrid(unsigned int maxgrid)
{
	MaxGrid = maxgrid;
}