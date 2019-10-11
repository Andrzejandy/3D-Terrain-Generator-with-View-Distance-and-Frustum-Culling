#include <stdio.h>
#include <vector>
#include <iostream>
#include "Terrain.h"
#pragma once
class TerrainManager
{
public:
	TerrainManager(void);
	~TerrainManager(void);

private:
	Terrain terrain;
	std::vector<Terrain> terrains;
	std::vector<Terrain>::iterator veciter;
	unsigned int Range;

public:
	//Teren wektor
	std::vector<Terrain>& TerrainManager::GetTerrainsVector(void);

	//Teren chunki
	void TerrainManager::CreateChunks(Renderer* renderer, unsigned int numchunks, unsigned int linelength, unsigned int numgrids);
	void TerrainManager::AddChunks(Renderer* renderer, unsigned int numchunks_toadd);
	void TerrainManager::SubChunks(Renderer* renderer, unsigned int numchunks_tosub);
	void TerrainManager::AdjustHeight(Renderer* renderer, unsigned int numchunks, unsigned int numgrids, float linelength);

	//Opcje
	unsigned int TerrainManager::GetMaxRenderDistance();
	void TerrainManager::SetMaxRenderDistance(unsigned int maxrange);
	void TerrainManager::LowerMaxRenderDistance(void);
	void TerrainManager::IncreaseMaxRenderDistance(void);

};

