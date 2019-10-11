#include "TerrainManager.h"


TerrainManager::TerrainManager(void)
{
	Range = 2;
}


TerrainManager::~TerrainManager(void)
{
}

std::vector<Terrain>& TerrainManager::GetTerrainsVector(void)
{
	return terrains;
}

unsigned int TerrainManager::GetMaxRenderDistance()
{
	return Range;
}

void TerrainManager::LowerMaxRenderDistance(void)
{
	Range = Range-1;
}

void TerrainManager::IncreaseMaxRenderDistance(void)
{
	Range = Range+1;
}

void TerrainManager::SetMaxRenderDistance(unsigned int maxrange)
{
	Range = maxrange;
}

void TerrainManager::CreateChunks(Renderer* renderer, unsigned int numchunks, unsigned int linelength, unsigned int numgrids)
{
	if(terrains.size() <= 0)
	{
		for(unsigned int x = 0; x < numchunks; x++)
		{
			for(unsigned int y = 0; y < numchunks; y++)
			{
				terrain.GenerateTerrainChunk(renderer->GetDevice(), numchunks,
					x, y, linelength, numgrids);
				terrains.push_back(terrain);
				terrain.DeleteSelf();
				terrain.ReleaseBuffer();
			}
		}
		AdjustHeight(renderer, numchunks, numgrids, linelength);
		for(int terrains_index = 0; terrains_index < terrains.size(); terrains_index++)
		{
			terrains[terrains_index].FillBuffer(renderer->GetDevice());
		}
	}
	else
	{

		std::vector<Terrain>::iterator terIter;
		for(terIter = terrains.begin(); terIter != terrains.end(); ++terIter)
		{
			terIter->ReleaseBuffer();
			terIter->DeleteSelf();
		}
		std::vector<Terrain>().swap(terrains);
		CreateChunks(renderer, numchunks, linelength, numgrids);
	}
}

void TerrainManager::AdjustHeight(Renderer* renderer, unsigned int numchunks, unsigned int numgrids, float linelength)
{
	for(unsigned int chunk_x = 0; chunk_x < numchunks; chunk_x++)
	{
		for(unsigned int chunk_y = 0; chunk_y < numchunks; chunk_y++)
		{
			int maxchunk = numchunks;
			int maxgrid = numgrids+1;

			float sumofy = 0;

			if(chunk_x % numchunks < numchunks-1 && chunk_y % numchunks == numchunks-1) //2,5
			{

				for(unsigned int sgx = 0; sgx<numgrids; sgx++)		// gora dol
				{
					sumofy = 
						(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(sgx+(numgrids*maxgrid)).y+
						terrains[chunk_y+((chunk_x+1)*maxchunk)].GetVertexPos(sgx+(0*maxgrid)).y)/2;

					terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(sgx+(numgrids*maxgrid),sumofy);
					terrains[chunk_y+((chunk_x+1)*maxchunk)].SetVertexDataY(sgx+(0*maxgrid),sumofy);
				}

				sumofy = 
					(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(numgrids+numgrids*maxgrid).y+
					terrains[chunk_y+((chunk_x+1)*maxchunk)].GetVertexPos(numgrids+0*(maxgrid)).y)/2;

				terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(numgrids+numgrids*maxgrid,sumofy);
				terrains[chunk_y+((chunk_x+1)*maxchunk)].SetVertexDataY(numgrids+0*(maxgrid),sumofy);

			}
			if(chunk_y % numchunks < numchunks-1 && chunk_x % numchunks == numchunks-1) //6,7
			{

				for(unsigned int sgy = 0; sgy<numgrids; sgy++)	// lewo prawo
				{
					sumofy = 
						(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(numgrids+(sgy*maxgrid)).y+
						terrains[chunk_y+1+chunk_x*maxchunk].GetVertexPos(0+(sgy*maxgrid)).y)/2;

					terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(numgrids+(sgy*maxgrid),sumofy);
					terrains[chunk_y+1+chunk_x*maxchunk].SetVertexDataY(0+(sgy*maxgrid),sumofy);
				}



				sumofy = 
					(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(numgrids+numgrids*maxgrid).y+
					terrains[chunk_y+1+chunk_x*maxchunk].GetVertexPos(0+numgrids*(maxgrid)).y)/2;

				terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(numgrids+numgrids*maxgrid,sumofy);
				terrains[chunk_y+1+chunk_x*maxchunk].SetVertexDataY(0+numgrids*(maxgrid),sumofy);

			}
			if(chunk_x % numchunks == numchunks-1 && chunk_y % numchunks == numchunks-1)// 8
			{

			}

			if( chunk_x % numchunks < numchunks-1 && chunk_y % numchunks < numchunks-1) //0,1,3,4
			{

				for(unsigned int sgx = 0; sgx<numgrids; sgx++)		// gora dol
				{
					sumofy = 
						(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(sgx+(numgrids*maxgrid)).y+
						terrains[chunk_y+((chunk_x+1)*maxchunk)].GetVertexPos(sgx+(0*maxgrid)).y)/2;

					terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(sgx+(numgrids*maxgrid),sumofy);
					terrains[chunk_y+((chunk_x+1)*maxchunk)].SetVertexDataY(sgx+(0*maxgrid),sumofy);
				}
				for(unsigned int sgy = 0; sgy<numgrids; sgy++)	// lewo prawo
				{
					sumofy = 
						(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(numgrids+(sgy*maxgrid)).y+
						terrains[chunk_y+1+chunk_x*maxchunk].GetVertexPos(0+(sgy*maxgrid)).y)/2;

					terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(numgrids+(sgy*maxgrid),sumofy);
					terrains[chunk_y+1+chunk_x*maxchunk].SetVertexDataY(0+(sgy*maxgrid),sumofy);
				}

				sumofy = 
					(terrains[chunk_y+chunk_x*maxchunk].GetVertexPos(numgrids+numgrids*maxgrid).y+
					terrains[chunk_y+1+chunk_x*maxchunk].GetVertexPos(0+numgrids*(maxgrid)).y+
					terrains[chunk_y+((chunk_x+1)*maxchunk)].GetVertexPos(numgrids+0*(maxgrid)).y+
					terrains[chunk_y+1+((chunk_x+1)*maxchunk)].GetVertexPos(0+0*(maxgrid)).y)/4;

				terrains[chunk_y+chunk_x*maxchunk].SetVertexDataY(numgrids+numgrids*maxgrid,sumofy);
				terrains[chunk_y+1+chunk_x*maxchunk].SetVertexDataY(0+numgrids*(maxgrid),sumofy);
				terrains[chunk_y+((chunk_x+1)*maxchunk)].SetVertexDataY(numgrids+0*(maxgrid),sumofy);
				terrains[chunk_y+1+((chunk_x+1)*maxchunk)].SetVertexDataY(0+0*(maxgrid),sumofy);

			}
		}
	}

	for(int terrains_index = 0; terrains_index < terrains.size(); terrains_index++)
	{
		terrains[terrains_index].FillBuffer(renderer->GetDevice());
	}
}

void TerrainManager::AddChunks(Renderer* renderer, unsigned int numchunks_toadd)
{
	int old_nof_chunks = sqrt(terrains.size());	//3	
	int final_nof_chunks = old_nof_chunks + numchunks_toadd;
	int numgrids = terrains.begin()->GetMaxGrid()-1;
	for(int chunk_x = 0; chunk_x < final_nof_chunks; chunk_x++)	// 0<4
	{
		for(int chunk_z = 0; chunk_z < final_nof_chunks; chunk_z++) // 0<4
		{
			/*
					z	z+1 z+2
			x		0	1	2
			x+1		3	4	5
			x+2		6	7	8

					z	z+1 z+2 z+3
			x		0	1	2	3
			x+1		4	5	6	7
			x+2		8	9	10	11
			x+3		12	13	14	15

			*/


			if(chunk_z>=final_nof_chunks-1 || chunk_x>=final_nof_chunks-1)
			{
				terrain.GenerateTerrainChunk(renderer->GetDevice(), old_nof_chunks,
					chunk_x, chunk_z, terrains[0].GetGridLength(), numgrids);

				terrains.insert(terrains.begin()+chunk_z+chunk_x*(final_nof_chunks),terrain);
				terrains[chunk_z+chunk_x*(final_nof_chunks)].SetGridXY(chunk_x,chunk_z);
				terrains[chunk_z+chunk_x*(final_nof_chunks)].FillBuffer(renderer->GetDevice());
				terrain.ReleaseBuffer();
				//	terrain.Clean(renderer->GetDeviceContext());
				terrain.DeleteSelf();
			}
			else
			{
				terrains[chunk_z+chunk_x*(final_nof_chunks)].SetGridXY(chunk_x,chunk_z);
			}
		}
	}
	AdjustHeight(renderer, final_nof_chunks, numgrids, terrains[0].GetGridLength());
}

void TerrainManager::SubChunks(Renderer* renderer, unsigned int numchunks_tosub)
{
	int old_nof_chunks = sqrt(terrains.size());	//4
	int final_nof_chunks = old_nof_chunks - numchunks_tosub; //3

	int TerrainSize = terrains.size() - numchunks_tosub;

	for(int chunk_x = old_nof_chunks-1; chunk_x >= 0; chunk_x--)	// 0<3
	{
		for(int chunk_z = old_nof_chunks-1; chunk_z >= 0; chunk_z--) // 0<3
		{
			/*
			z	z+1 z+2 z+3
			x		0	1	2	3
			x+1		4	5	6	7
			x+2		8	9	10	11
			x+3		12	13	14	15

			z	z+1 z+2
			x		0	1	2
			x+1		3	4	5
			x+2		6	7	8
			
					0	1
					2	3
			*/

			if(chunk_z>=final_nof_chunks || chunk_x>=final_nof_chunks)
			{
				terrains[chunk_z+chunk_x*(old_nof_chunks)].ReleaseBuffer();
				terrains[chunk_z+chunk_x*(old_nof_chunks)].Clean(renderer->GetDeviceContext());
				terrains[chunk_z+chunk_x*(old_nof_chunks)].DeleteSelf();
				terrains.erase(terrains.begin()+chunk_z+chunk_x*(old_nof_chunks));
			}
			else
			{
				terrains[chunk_z+chunk_x*(final_nof_chunks)].SetGridXY(chunk_x,chunk_z);
			}
		}
	}
}
