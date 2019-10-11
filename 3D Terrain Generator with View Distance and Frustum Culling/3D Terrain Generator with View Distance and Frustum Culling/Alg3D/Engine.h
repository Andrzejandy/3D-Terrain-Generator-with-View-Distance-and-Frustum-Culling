#pragma once
#include "Global.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "Camera.h"
#include "Debug.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Render.h"
#include "Shader.h"
#include "Terrain.h"
#include "TerrainManager.h"
#include "Timer.h"
#include "Window.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class Engine
{
public:
	Engine(void);
	~Engine(void);

	enum StanyProgramu
	{
		MenuPoczatkowe = 0,
		ZaladowanaMapa = 1,
		MenuPoZaladowaniu = 2
	};
	StanyProgramu stanprogramu;

private:
	Renderer* renderer;
	Window* window;
	Camera* camera;
	Debug* debug;
	TerrainManager terrainmanager;
//	Terrain terrain;
	Timer* timer;
	Timer* rendertimer;
	Shader* shader;
	Mouse* mouse;
	Keyboard* keyboard;
	//Window
	HINSTANCE _hinst;
	HWND _hwnd;

public:
	bool Engine::InitializeEngine(void);
	void Engine::Run(void);
	void Engine::HandleInput(void);
	void Engine::HandleRendering(ID3D11Query* query);
	void Engine::HandleRendering();
	void Engine::HandleRenderingNotOptimized();

	void Engine::RenderChunks(D3DXVECTOR3 CamPos, int range, unsigned int terrainsize, unsigned int numofgrids, float linelength);
	void Engine::RenderChunksNotOptimized(D3DXVECTOR3 CamPos, int range, unsigned int terrainsize, unsigned int numofgrids, float linelength);
	//Operacje na plikach
	bool Engine::SaveTerrainDataToFile(std::string filename);
	bool Engine::LoadTerrainDataFromFile(std::string filename);

	//Windows
	HINSTANCE Engine::eGetInstance();
	void Engine::eSetHinstance(HINSTANCE ehinst);
	HWND Engine::eGetHWND(void);
	void Engine::eSetHWND(HWND hwnd);

	//Klasy
	Camera *Engine::GetCamera();
	//Terrain &Engine::GetTerrain();
	TerrainManager &Engine::GetTerrainManager();
	Renderer *Engine::GetRenderer();
	Timer *Engine::GetTimer();
	Timer *Engine::GetRenderTimer();
	Window *Engine::GetWindow();
	Mouse *Engine::GetMouse();
	Keyboard *Engine::GetKeyboard();

	//	int WINAPI Engine::WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lLPstr, int iCmdShow);
};
