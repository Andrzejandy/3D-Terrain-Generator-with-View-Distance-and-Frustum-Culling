#include "Engine.h"
Engine engine;
static int TerrainSize = 4;
static float LineLength = 1;
static int NumberOfGrids = 3;
static float length = 1.0f;
static POINT MousePos;
static POINT OldMousePos;
static POINT centerpoint;
static LONG dmx = 0;
static LONG dmy = 0;
static bool optimized = true;

/*
Podzielic teren na chunki	X
Dokonczyc kamere	X	
Sprawdzic timer		X
dodac teksture		X
culling kamery X
//level of detail (opcjonalnie) 
*/


Engine::Engine(void)
{
	mouse = nullptr;
	window = nullptr;
	keyboard = nullptr;
	timer = nullptr;
	renderer = nullptr;
	camera = nullptr;
	//	terrain = NULL;
	shader = nullptr;
	_hinst = nullptr;
	_hwnd = nullptr;
	stanprogramu = MenuPoczatkowe;
}


Engine::~Engine(void)
{
	if(window)
	{
		delete window;
		window = nullptr;
	}
	if(timer)
	{
		delete timer;
		timer = nullptr;
	}
	if(rendertimer)
	{
		delete rendertimer;
		rendertimer = nullptr;
	}
	if(mouse)
	{
		delete mouse;
		mouse = nullptr;
	}
	if(keyboard)
	{
		delete keyboard;
		keyboard = nullptr;
	}
	if(camera)
	{
		delete camera;
		camera = nullptr;
	}
	while(!terrainmanager.GetTerrainsVector().empty())
	{
		std::vector<Terrain>::iterator terIter;
		for(terIter = terrainmanager.GetTerrainsVector().begin(); terIter != terrainmanager.GetTerrainsVector().end(); ++terIter)
		{
			terIter->ReleaseBuffer();
			terIter->DeleteSelf();
		}
		std::vector<Terrain>().swap(terrainmanager.GetTerrainsVector());
	}
	if(debug)
	{
		delete debug;
		debug = nullptr;
	}
	if(shader)
	{
		delete shader;
		shader = nullptr;
	}
	if(renderer)
	{
		delete renderer;
		renderer = nullptr;
	}

}

HWND Engine::eGetHWND(void)
{
	return _hwnd;
}
Camera *Engine::GetCamera()
{
	return camera;
}

TerrainManager& Engine::GetTerrainManager()
{
	return terrainmanager;
}
Renderer *Engine::GetRenderer()
{
	return renderer;
}
Timer *Engine::GetTimer()
{
	return timer;
}
Timer *Engine::GetRenderTimer()
{
	return rendertimer;
}
Window *Engine::GetWindow()
{
	return window;
}
Mouse *Engine::GetMouse()
{
	return mouse;
}
Keyboard *Engine::GetKeyboard()
{
	return keyboard;
}

void Engine::eSetHinstance(HINSTANCE ehinst)
{
	_hinst = ehinst;
}



bool Engine::InitializeEngine(void)
{
	mouse = new Mouse;
	keyboard = new Keyboard;
	window = new Window;
	window->wSetInstance(_hinst);
	window->InitializeWindow();
	timer = new Timer;
	rendertimer = new Timer;	
	renderer = new Renderer;
	renderer->Initialize(window->wGetHWND());
	camera = new Camera;
#ifdef _DEBUG
	debug = new Debug;
	debug->Init(renderer->GetDevice());
#endif
	shader = new Shader;
	shader->InitializeShader(renderer->GetDevice(), renderer->GetDeviceContext());
	shader->InitializeCbuffer(renderer->GetDevice(), renderer->GetDeviceContext());

	return true;
}

void Engine::Run(void)
{
	HWND hWnd = NULL;
	MSG Msg = { 0 };

#ifdef _DEBUG
	// Request information from pipieline.
	//////////////////////////
	ID3D11Query* pQuery;
	D3D11_QUERY_DESC queryDesc;
	queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;
	queryDesc.MiscFlags = 0;
	engine.GetRenderer()->GetDevice()->CreateQuery(&queryDesc, &pQuery);
	///////////////////////////
#endif
	Timer frametimer;
	clock_t deltatime = 0;
	frametimer.StartTime();
	timer->StartTime();
	while(WM_QUIT != Msg.message)
	{
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
#ifdef _DEBUG

			if(timer->GetTime() > 1.0/60)
			{
				HandleInput();
				if(optimized == true)
				{
					HandleRendering(pQuery);
				}
				else
				{
					HandleRenderingNotOptimized();
				}
			}
#else		 

			HandleInput();
			if(optimized == true)
			{
				HandleRendering();
			}
			else
			{
				HandleRenderingNotOptimized();
			}

			if(timer->GetTime() > 1.0f)
			{
				std::cout << "Frame Time: " << timer->GetFrameTime() << "s" << std::endl;
				timer->StartTime();
			}
#endif
#ifdef _DEBUG
			if(timer->GetTime() > 1.0f)
			{
				std::cout << "Frame Time: " << timer->GetFrameTime() << "s" << std::endl;
				if(terrainmanager.GetMaxRenderDistance() % 10 == 0)
				{
					std::cout << "Render Distance: " << terrainmanager.GetMaxRenderDistance() << std::endl;
				}
				timer->StartTime();
			}
#endif
			Sleep(15);
		}
	}
}


void Engine::RenderChunks(D3DXVECTOR3 CamPos, int range, unsigned int terrainsize, unsigned int numofgrids, float linelength)
{
	int gridrange = 60;
	int CamPosX_Grid = floor((CamPos.x) /  (numofgrids-1) / linelength);
	int CamPosZ_Grid = floor((CamPos.z) /  (numofgrids-1) / linelength);
	int maxterrain = sqrt(terrainmanager.GetTerrainsVector().size());
	int currgrid = CamPosZ_Grid+CamPosX_Grid*terrainsize;

	int sumadraw = 0;
	static int oldsumadraw = -1;
	int maxdraw = 0;

	int minXtoRender = 0;
	int minYtoRender = 0;
	int maxXtoRender = 0;
	int maxYtoRender = 0;
	for(int x =0; x < terrainmanager.GetTerrainsVector().size(); x++)
	{
		maxdraw += terrainmanager.GetTerrainsVector()[x].GetNumVertexs();
	}
	//Chunks out of sight//
	/*
	#ifdef _DEBUG
	for(int rangexx = CamPosX_Grid - gridrange; rangexx <= CamPosX_Grid + gridrange; rangexx++)
	{
	for(int rangezz = CamPosZ_Grid - gridrange; rangezz <= CamPosZ_Grid + gridrange; rangezz++)
	{
	if(rangezz >= 0 && rangexx >= 0 && rangexx < terrainsize && rangezz < terrainsize)
	{
	if(rangezz + rangexx*terrainsize>=0 && rangezz + rangexx*terrainsize< terrainmanager.GetTerrainsVector().size() && terrainmanager.GetTerrainsVector().size() > 0)
	{
	renderer->ChangeFillMode();

	terrainmanager.GetTerrainsVector()[rangezz + rangexx*terrainsize].DrawIndexed(renderer->GetDeviceContext());
	shader->Render(
	renderer->GetDeviceContext(),
	terrainmanager.GetTerrainsVector()[rangezz + rangexx*terrainsize].GetNumIndices(),
	renderer->GetWorldMatrix(),
	camera->GetCamMatrix(),
	renderer->GetProjectionMatrix(),
	timer->GetTime());

	renderer->ChangeFillMode();
	}
	}
	}
	}
	#else
	#endif
	*/

	if(terrainmanager.GetTerrainsVector().size() > 0)
	{		
		int x1 = terrainmanager.GetTerrainsVector()[0 + 0*terrainsize].GetGridX();	// minx
		int y1 = terrainmanager.GetTerrainsVector()[0 + 0*terrainsize].GetGridY();	// miny
		int x2 = terrainmanager.GetTerrainsVector()[(terrainsize-1) + (terrainsize-1)*terrainsize].GetGridX();	// maxx
		int y2 = terrainmanager.GetTerrainsVector()[(terrainsize-1) + (terrainsize-1)*terrainsize].GetGridY();	// maxy
		int x4 = CamPosX_Grid + range;									
		int y4 = CamPosZ_Grid + range;
		int x3 = CamPosX_Grid - range;
		int y3 = CamPosZ_Grid - range;

		if(x1 > x3)
		{
			minXtoRender = x1;
		}
		else
		{
			minXtoRender = x3;
		}
		if(y1 > y3)
		{
			minYtoRender = y1;
		}
		else
		{
			minYtoRender = y3;
		}

		if(x2 > x4)
		{
			maxXtoRender = x4;
		}
		else
		{
			maxXtoRender = x2;
		}
		if(y2 > y4)
		{
			maxYtoRender = y4;
		}
		else
		{
			maxYtoRender = y2;
		}
		for(int rangeX = minXtoRender; rangeX <= maxXtoRender; rangeX++)
		{
			for(int rangeZ = minYtoRender; rangeZ <= maxYtoRender; rangeZ++)
			{

				//	std::cout << rangeZ << "," << rangeX;
				if(	camera->CheckRect(
					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetCenterXPos(),
					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetCenterYPos(),
					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetCenterZPos(),
					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetMaxGrid()
					*terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetGridLength())) 
				{
					//	if((CamPosX_Grid-rangeX)*(CamPosX_Grid-rangeX)+(CamPosZ_Grid-rangeZ)*(CamPosZ_Grid-rangeZ) < x4*x4 
					//	|| (CamPosX_Grid-rangeX)*(CamPosX_Grid-rangeX)+(CamPosZ_Grid-rangeZ)*(CamPosZ_Grid-rangeZ) > x4*x4 	)
					//	{
					//	}

					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].DrawIndexed(renderer->GetDeviceContext());
					shader->Render(
						renderer->GetDeviceContext(),
						terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetNumIndices(),
						renderer->GetWorldMatrix(),
						camera->GetCamMatrix(),
						renderer->GetProjectionMatrix(),
						timer->GetTime());

					sumadraw += terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetNumVertexs();
				}
				/*
				if(CamPosX_Grid-rangeX < -2 || CamPosZ_Grid-rangeZ < -2 || CamPosX_Grid-rangeX > 2 || CamPosZ_Grid-rangeZ > 2)
				{

				}
				else if(CamPosX_Grid-rangeX+CamPosZ_Grid-rangeZ < -8 || CamPosX_Grid-rangeX+CamPosZ_Grid-rangeZ > 8)
				{

				}
				else if(CamPosX_Grid-rangeX+CamPosZ_Grid-rangeZ < -16 || CamPosX_Grid-rangeX+CamPosZ_Grid-rangeZ > 16)
				{

				}
				*/
			}
			//		std::cout << std::endl;
		}
	}
	/*
	if(CamPosX_Grid < 0 && CamPosZ_Grid < 0)
	{
		//	if(finalRangeX > terrainmanager.GetTerrainsVector().size()
	}
	else if(CamPosX_Grid < 0 && CamPosZ_Grid > maxterrain)
	{

	}
	else if(CamPosX_Grid > maxterrain && CamPosZ_Grid < 0)
	{

	}
	else if(CamPosX_Grid > maxterrain && CamPosZ_Grid > maxterrain)
	{

	}
	*/
	if(sumadraw != oldsumadraw)
	{
		std::cout << "curr: " << sumadraw << std::endl;
		std::cout << "max: " << maxdraw << std::endl;
	}
	oldsumadraw = sumadraw;
}

void Engine::RenderChunksNotOptimized(D3DXVECTOR3 CamPos, int range, unsigned int terrainsize, unsigned int numofgrids, float linelength)
{
	int gridrange = 60;
	int CamPosX_Grid = floor((CamPos.x) /  (numofgrids-1) / linelength);
	int CamPosZ_Grid = floor((CamPos.z) /  (numofgrids-1) / linelength);
	int maxterrain = sqrt(terrainmanager.GetTerrainsVector().size());
	int currgrid = CamPosZ_Grid+CamPosX_Grid*terrainsize;

	int sumadraw = 0;
	static int oldsumadraw = -1;
	int maxdraw = 0;

	int minXtoRender = 0;
	int minYtoRender = 0;
	int maxXtoRender = 0;
	int maxYtoRender = 0;
	for(int x =0; x < terrainmanager.GetTerrainsVector().size(); x++)
	{
		maxdraw += terrainmanager.GetTerrainsVector()[x].GetNumVertexs();
	}

	if(terrainmanager.GetTerrainsVector().size() > 0)
	{		
		int x1 = terrainmanager.GetTerrainsVector()[0 + 0*terrainsize].GetGridX();	// minx
		int y1 = terrainmanager.GetTerrainsVector()[0 + 0*terrainsize].GetGridY();	// miny
		int x2 = terrainmanager.GetTerrainsVector()[(terrainsize-1) + (terrainsize-1)*terrainsize].GetGridX() ;	// maxx
		int y2 = terrainmanager.GetTerrainsVector()[(terrainsize-1) + (terrainsize-1)*terrainsize].GetGridY() ;	// maxy
		int x4 = CamPosX_Grid + range;									
		int y4 = CamPosZ_Grid + range;
		int x3 = CamPosX_Grid - range;
		int y3 = CamPosZ_Grid - range;

		if(x1 > x3)
		{
			minXtoRender = x1;
		}
		else
		{
			minXtoRender = x3;
		}
		if(y1 > y3)
		{
			minYtoRender = y1;
		}
		else
		{
			minYtoRender = y3;
		}

		if(x2 > x4)
		{
			maxXtoRender = x4;
		}
		else
		{
			maxXtoRender = x2;
		}
		if(y2 > y4)
		{
			maxYtoRender = y4;
		}
		else
		{
			maxYtoRender = y2;
		}
		for(int rangeX = minXtoRender; rangeX <= maxXtoRender; rangeX++)
		{
			for(int rangeZ = minYtoRender; rangeZ <= maxYtoRender; rangeZ++)
			{

				terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].DrawIndexed(renderer->GetDeviceContext());
				shader->Render(
					renderer->GetDeviceContext(),
					terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetNumIndices(),
					renderer->GetWorldMatrix(),
					camera->GetCamMatrix(),
					renderer->GetProjectionMatrix(),
					timer->GetTime());

				sumadraw += terrainmanager.GetTerrainsVector()[rangeZ + rangeX*terrainsize].GetNumVertexs();
			}
		}
	}
	if(CamPosX_Grid < 0 && CamPosZ_Grid < 0)
	{

	}
	else if(CamPosX_Grid < 0 && CamPosZ_Grid > maxterrain)
	{

	}
	else if(CamPosX_Grid > maxterrain && CamPosZ_Grid < 0)
	{

	}
	else if(CamPosX_Grid > maxterrain && CamPosZ_Grid > maxterrain)
	{

	}
	if(sumadraw != oldsumadraw)
	{
		std::cout << "curr: " << sumadraw << std::endl;
		std::cout << "max: " << maxdraw << std::endl;
	}
	oldsumadraw = sumadraw;
}


void Engine::HandleRendering(ID3D11Query* query)
{
	renderer->SetScene();
	camera->Update(engine.GetTimer()->GetFrameTime(), engine.GetRenderTimer()->GetTime());
	camera->CreateFrustum(renderer->GetFarZ(), renderer->GetProjectionMatrix());

	//pobierz informacje tylko z terenu
	//	renderer->GetDeviceContext()->Begin();
	if(terrainmanager.GetTerrainsVector().size() > 0)
	{
		RenderChunks(camera->GetCameraPos(),terrainmanager.GetMaxRenderDistance(), sqrt(terrainmanager.GetTerrainsVector().size()), terrainmanager.GetTerrainsVector()[0].GetMaxGrid(), terrainmanager.GetTerrainsVector()[0].GetGridLength());
	}
	//	renderer->GetDeviceContext()->End();
	//zakoncz pobieranie informacji

	/*
	debug->InitwPos(renderer->GetDevice(), renderer->GetDeviceContext(), camera->GetCameraAt(),camera->GetCameraForward());
	debug->Render(renderer->GetDeviceContext());
	shader->RenderV(
	renderer->GetDeviceContext(),
	debug->numverts,
	renderer->GetWorldMatrix(),
	camera->GetCamMatrix(),
	renderer->GetProjectionMatrix());
	*/
	renderer->PresentScenece();
}

void Engine::HandleRendering()
{
	renderer->SetScene();
	camera->Update(engine.GetTimer()->GetFrameTime(), engine.GetRenderTimer()->GetTime());
	camera->CreateFrustum(renderer->GetFarZ(), renderer->GetProjectionMatrix());
	if(terrainmanager.GetTerrainsVector().size() > 0)
	{
		RenderChunks(camera->GetCameraPos(),terrainmanager.GetMaxRenderDistance(), sqrt(terrainmanager.GetTerrainsVector().size()), terrainmanager.GetTerrainsVector()[0].GetMaxGrid(), terrainmanager.GetTerrainsVector()[0].GetGridLength());
	}

	renderer->PresentScenece();
}

void Engine::HandleRenderingNotOptimized()
{
	renderer->SetScene();
	camera->Update(engine.GetTimer()->GetFrameTime(), engine.GetRenderTimer()->GetTime());
	if(terrainmanager.GetTerrainsVector().size() > 0)
	{
		RenderChunksNotOptimized(camera->GetCameraPos(),10000000, sqrt(terrainmanager.GetTerrainsVector().size()), terrainmanager.GetTerrainsVector()[0].GetMaxGrid(), terrainmanager.GetTerrainsVector()[0].GetGridLength());
	}

	renderer->PresentScenece();
}

void Engine::HandleInput(void)
{
	static bool KeyUp = false;
	//////////	W
	if(keyboard->AsyncIsKeyDown('W'))
	{
		camera->Move(1.0f*keyboard->Keytimer->GetTime()+0.8f, timer->GetFrameTime());
		//	std::cout << keyboard->Keytimer->GetTime() << std::endl;
		KeyUp = true;
	}

	if(keyboard->AsyncIsKeyDown('W') == false && KeyUp == true)
	{
		//	std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

	/////////// A
	if(keyboard->AsyncIsKeyDown('A'))
	{
		engine.GetCamera()->Strafe(-0.2f*keyboard->Keytimer->GetTime()-0.8f, timer->GetFrameTime());
		KeyUp = true;
	}
	if(keyboard->AsyncIsKeyDown('A') == false && KeyUp == true)
	{
		//		std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

	if(keyboard->AsyncIsKeyDown('A') && keyboard->AsyncIsKeyDown('W'))
	{
		camera->Move(1.0f*keyboard->Keytimer->GetTime()+0.8f, timer->GetFrameTime());
		camera->Strafe(-1.0f*keyboard->Keytimer->GetTime()-0.8f, timer->GetFrameTime());
	}

	/////////// S
	if(keyboard->AsyncIsKeyDown('S'))
	{
		camera->Move(-1.0f*keyboard->Keytimer->GetTime()-0.8f, timer->GetFrameTime());
		KeyUp = true;
	}
	if(keyboard->AsyncIsKeyDown('S') == false && KeyUp == true)
	{
		//		std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

	////////// D
	if(keyboard->AsyncIsKeyDown('D'))
	{
		engine.GetCamera()->Strafe(0.2f*keyboard->Keytimer->GetTime()+0.8f, timer->GetFrameTime());
		KeyUp = true;
	}
	if(keyboard->AsyncIsKeyDown('D') == false && KeyUp == true)
	{
		//	std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

	////////// z
	if(keyboard->AsyncIsKeyDown('Z'))
	{
		engine.GetCamera()->Move(D3DXVECTOR3(0.0f, 1.0f, 0.0f), engine.GetTimer()->GetFrameTime());
		KeyUp = true;
	}
	if(keyboard->AsyncIsKeyDown('Z') == false && KeyUp == true)
	{
		//	std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

	////////// x
	if(keyboard->AsyncIsKeyDown('X'))
	{
		engine.GetCamera()->Move(D3DXVECTOR3(0.0f, -1.0f, 0.0f), engine.GetTimer()->GetFrameTime());
		KeyUp = true;
	}
	if(keyboard->AsyncIsKeyDown('X') == false && KeyUp == true)
	{
		//	std::cout << keyboard->KeyDownTime << std::endl;
		KeyUp = false;
	}

}

bool Engine::SaveTerrainDataToFile(std::string filename)
{
	std::ofstream plik;
	plik.open(filename , std::ios::trunc);
	if (plik.is_open())
	{
		if(terrainmanager.GetTerrainsVector().size()>0)
		{
			////////// Naglowki
			plik << terrainmanager.GetTerrainsVector()[0].GetNumVertexs() << "\n";	//Wrzucamy ilosc vertexow
			plik << terrainmanager.GetTerrainsVector()[0].GetNumIndices() << "\n";	//Ilosc indeksow
			plik << terrainmanager.GetTerrainsVector()[0].GetNumFaces() << "\n";		//Ilosc faceow
			plik << terrainmanager.GetTerrainsVector()[0].GetGridLength() << "\n";	//Dlugosc boku kwadratu
			plik << terrainmanager.GetTerrainsVector()[0].GetMaxGrid() << "\n";		//Ilosc max. kwadratow
			plik << sqrt(terrainmanager.GetTerrainsVector().size()) << "\n";		// teren sqrt(x) (x*x) = ilosc pol
			///////////////////////////////
			for(unsigned int terloop = 0; terloop < terrainmanager.GetTerrainsVector().size(); terloop++)
			{
				plik << "t:" << terloop << "\n"; 
				for(unsigned int verts = 0; verts < terrainmanager.GetTerrainsVector()[terloop].GetNumVertexs(); verts++)
				{
					plik << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtPos.x << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtPos.y << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtPos.z << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtColor.r << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtColor.g << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtColor.b << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtColor.a << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtTexCord.x << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtTexCord.y << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtNormal.x << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtNormal.y << "\n" << 
						terrainmanager.GetTerrainsVector()[terloop].GetVertexData()[verts].tvrtNormal.z << "\n";
				}
				for(unsigned int indics = 0; indics < terrainmanager.GetTerrainsVector()[terloop].GetNumIndices(); indics++)
				{
					plik <<
						terrainmanager.GetTerrainsVector()[terloop].GetIndicesData()[indics] << "\n";
				}
				plik << terrainmanager.GetTerrainsVector()[terloop].GetGridX() << "\n"; 
				plik << terrainmanager.GetTerrainsVector()[terloop].GetGridY() << "\n"; 
			}
		}
		plik.close();
		return true;
	}
	else
	{
		//std::cout << "Nie mozna otworzyc pliku o nazwie: " << filename << std::endl;
		std::cout << "Cannot open file: " << filename << std::endl;
		return false;
	}
}

bool Engine::LoadTerrainDataFromFile(std::string filename)
{
	std::ifstream plik;
	plik.open(filename, std::ios::in);
	Terrain terentemp;
	tVertex *tvertstemp;
	uint16_t *tindices;
	unsigned int ilsc_vert;
	unsigned int ilsc_inds;
	unsigned int ilsc_face;
	unsigned int maxgrid;
	int maxterrains;
	float gridlngth;

	if (plik.is_open())
	{
		if(plik.peek() != std::ifstream::traits_type::eof())
		{
			std::string wiersz;
			////////// Naglowki
			plik >> ilsc_vert;
			tvertstemp = new tVertex[ilsc_vert];
			plik >> ilsc_inds;
			tindices = new uint16_t[ilsc_inds];
			plik >> ilsc_face;
			plik >> gridlngth;
			plik >> maxgrid;
			plik >> maxterrains;
			///////////////////////////////
			int trnr = 0;

			while(maxterrains*maxterrains > terrainmanager.GetTerrainsVector().size())
			{
				terentemp.GenerateTerrainChunk(renderer->GetDevice(),
					maxterrains, 0, 0, 1.0f, maxgrid);
				terrainmanager.GetTerrainsVector().push_back(terentemp);
				terentemp.DeleteSelf();
				terentemp.ReleaseBuffer();
			}
			while(terrainmanager.GetTerrainsVector().size() > maxterrains*maxterrains)
			{
				terrainmanager.GetTerrainsVector().pop_back();
			}

			while ( std::getline (plik,wiersz) )
			{
				std::stringstream ss;
				ss.seekp(0);
				ss.str("");
				ss.clear();
				//naglowki
				ss << "t:";
				ss << trnr;
				//	str = ss.str();

				if(wiersz == ss.str())	// t:0..T:1..T:2....
				{

					/*
					if(trnr >= terrainmanager.GetTerrainsVector().size())
					{

					terentemp.SetNumVertexs(ilsc_vert);
					terentemp.SetNumIndices(ilsc_inds);
					terentemp.SetNumFaces(ilsc_face);
					terentemp.SetGridLength(gridlngth);
					terentemp.SetChunkIndex(chunkindex);
					terentemp.SetMaxGrid(maxgrid);

					}
					*/
					/*	
					else if (trnr < terrainmanager.GetTerrainsVector().size())
					{
					terrainmanager.GetTerrainsVector()[trnr].ReleaseBuffer();
					terrainmanager.GetTerrainsVector().erase(terrainmanager.GetTerrainsVector().begin() + trnr);
					}
					*/
					//terrainmanager.GetTerrainsVector()[trnr].Clean(renderer->GetDeviceContext());

					terrainmanager.GetTerrainsVector()[trnr].SetNumVertexs(ilsc_vert);
					terrainmanager.GetTerrainsVector()[trnr].SetNumIndices(ilsc_inds);
					terrainmanager.GetTerrainsVector()[trnr].SetNumFaces(ilsc_face);
					terrainmanager.GetTerrainsVector()[trnr].SetGridLength(gridlngth);
					terrainmanager.GetTerrainsVector()[trnr].SetMaxGrid(maxgrid);
					terrainmanager.GetTerrainsVector()[trnr].SetMaxTerrains(maxterrains-1);

					for(unsigned int vertnr = 0; vertnr < ilsc_vert; vertnr++)
					{
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtPos.x = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtPos.y = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtPos.z = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtColor.r = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtColor.g = std::strtod(wiersz.c_str(),NULL); 
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtColor.b = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtColor.a = std::strtod(wiersz.c_str(),NULL); 
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtTexCord.x = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtTexCord.y = std::strtod(wiersz.c_str(),NULL);  
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtNormal.x = std::strtod(wiersz.c_str(),NULL); 
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtNormal.y = std::strtod(wiersz.c_str(),NULL);
						std::getline(plik,wiersz);
						tvertstemp[vertnr].tvrtNormal.z = std::strtod(wiersz.c_str(),NULL);
					}

					terrainmanager.GetTerrainsVector()[trnr].SetVertexData(tvertstemp, ilsc_vert);
					for(unsigned int indnr = 0; indnr < ilsc_inds; indnr++)
					{
						std::getline(plik,wiersz);
						tindices[indnr] = std::atoi(wiersz.c_str());
					}

					terrainmanager.GetTerrainsVector()[trnr].SetIndexData(tindices, ilsc_inds);
					std::getline(plik,wiersz);
					terrainmanager.GetTerrainsVector()[trnr].SetGridX(std::atoi(wiersz.c_str()));
					std::getline(plik,wiersz);
					terrainmanager.GetTerrainsVector()[trnr].SetGridY(std::atoi(wiersz.c_str()));

					trnr++;
				}
				else
				{

				}
				ss.clear();
			}

			delete[] tvertstemp;
			tvertstemp = nullptr;
			delete[] tindices;
			tindices = nullptr;

			for(int trit = 0; trit < terrainmanager.GetTerrainsVector().size(); trit++)
			{
				terrainmanager.GetTerrainsVector()[trit].FillBuffer(renderer->GetDevice());
			}

			plik.close();
			return true;
		}
		else
		{
			plik.close();
			return false;
		}
	}
	else
	{
		std::cout << "Cannot load file: " << filename << std::endl;
		//std::cout << "Nie mozna wczytac pliku o nazwie: " << filename << std::endl;
		return false;
	}

}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lLPstr, int iCmdShow)
{
	//Konsola
	//////////////////////////
#ifdef _DEBUG
	CONSOLE_SCREEN_BUFFER_INFO csrBufferInfo;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	LPCSTR LPstrCT = "Konsola";
	LPCSTR lpcmsg = "";
	SetConsoleTitle(LPstrCT);
	freopen("CONOUT$", "w", stdout);
	DrawMenuBar(GetConsoleWindow());
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csrBufferInfo);
#else
	CONSOLE_SCREEN_BUFFER_INFO csrBufferInfo;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	LPCSTR LPstrCT = "Konsola";
	LPCSTR lpcmsg = "";
	SetConsoleTitle(LPstrCT);
	freopen("CONOUT$", "w", stdout);
	DrawMenuBar(GetConsoleWindow());
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csrBufferInfo);
#endif
	//////////////////////////


	engine.eSetHinstance(hInst);
	engine.InitializeEngine();
	if(engine.LoadTerrainDataFromFile("teren.dat"))
	{
		engine.GetCamera()->SetCameraPos(1.0,2.0,1.0);
		engine.GetCamera()->SetCameraAt(1.0,2.0,4.0);
		engine.stanprogramu = engine.ZaladowanaMapa;
	}
	else
	{
		engine.GetCamera()->SetCameraPos(1.0,2.0,1.0);
		engine.GetCamera()->SetCameraAt(1.0,2.0,4.0);
		engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),3,1,3);
	}
	engine.Run();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	RECT rc;

	switch(Msg)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_LBUTTONDOWN:
		if(engine.GetMouse()->GetMouseLook() == false)
		{
			GetWindowRect(hwnd, &rc);
			centerpoint.y = rc.top + (rc.bottom - rc.top) / 2;
			centerpoint.x = rc.left + (rc.right - rc.left) / 2;
			SetCursorPos(centerpoint.x, centerpoint.y);
			engine.GetMouse()->SetMouseLook(true);
			ShowCursor(false);
		}
		else
		{
			engine.GetMouse()->SetMouseLook(false);
			ShowCursor(true);
		}
		break;
	case WM_RBUTTONUP:
		if(engine.GetMouse()->GetMouseLook() == true)
		{
			if(engine.GetTerrainManager().GetTerrainsVector().size() > 0)
			{
				int CamPosX_Grid = floor((engine.GetCamera()->GetCameraPos().x) /  (engine.GetTerrainManager().GetTerrainsVector()[0].GetMaxGrid()-1) / engine.GetTerrainManager().GetTerrainsVector()[0].GetGridLength()) ;
				int CamPosZ_Grid = floor((engine.GetCamera()->GetCameraPos().z) /  (engine.GetTerrainManager().GetTerrainsVector()[0].GetMaxGrid()-1) / engine.GetTerrainManager().GetTerrainsVector()[0].GetGridLength()) ;

				int currgrid = CamPosZ_Grid+CamPosX_Grid*sqrt(engine.GetTerrainManager().GetTerrainsVector().size());

				std::cout << "Cam: <" << CamPosX_Grid << "," 
					<< CamPosZ_Grid << ">" << ":" << currgrid << std::endl;

				if(currgrid >= 0 && currgrid < engine.GetTerrainManager().GetTerrainsVector().size())
				{

					std::cout << "Grid: " << engine.GetTerrainManager().GetTerrainsVector()[currgrid].GetGridX() 
						<< "," << engine.GetTerrainManager().GetTerrainsVector()[currgrid].GetGridY()
						<< std::endl;
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		if(engine.GetMouse()->GetMouseLook() == true)
		{
			GetWindowRect(hwnd, &rc);

			centerpoint.y = rc.top + (rc.bottom - rc.top) / 2;
			centerpoint.x = rc.left + (rc.right - rc.left) / 2;

			GetCursorPos(&MousePos);
			dmx = MousePos.x - centerpoint.x;
			dmy = MousePos.y - centerpoint.y;

			if(dmx || dmy)
			{
				GetCursorPos(&MousePos);
				engine.GetCamera()->RotByMouseCords(dmx, dmy);
				SetCursorPos(centerpoint.x, centerpoint.y);
			}
		}
		break;
	case WM_RBUTTONDOWN:
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'W':
			if(engine.GetKeyboard()->isKeyDown('w') == false)
			{
				engine.GetKeyboard()->SetKeyDown('w');
			}
			engine.GetRenderTimer()->StartTime();
			break;
		case 'S':
			if(engine.GetKeyboard()->isKeyDown('s') == false)
			{
				engine.GetKeyboard()->SetKeyDown('s');
			}
			engine.GetRenderTimer()->StartTime();
			break;
		case 'A':
			if(engine.GetKeyboard()->isKeyDown('a') == false)
			{
				engine.GetKeyboard()->SetKeyDown('a');
			}
			engine.GetRenderTimer()->StartTime();
			break;
		case 'D':
			if(engine.GetKeyboard()->isKeyDown('d') == false)
			{
				engine.GetKeyboard()->SetKeyDown('d');
			}
			engine.GetRenderTimer()->StartTime();
			break;
		case 'E':
			engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),TerrainSize, LineLength ,++NumberOfGrids);
			break;
		case 'Q':
			engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),TerrainSize, LineLength ,--NumberOfGrids);
			break;
		case 'Z':
			if(engine.GetKeyboard()->isKeyDown('z') == false)
			{
				engine.GetKeyboard()->SetKeyDown('z');
			}
			break;
		case 'X':
			if(engine.GetKeyboard()->isKeyDown('x') == false)
			{
				engine.GetKeyboard()->SetKeyDown('x');
			}
			break;
		case 'R':
			engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),TerrainSize, LineLength , NumberOfGrids);
			break;
		case 'F':
			engine.GetTerrainManager().AddChunks(engine.GetRenderer(),1);
			break;
		case 'V':
			engine.GetTerrainManager().SubChunks(engine.GetRenderer(),1);
			break;
		case 'C':
			engine.GetTerrainManager().AdjustHeight(engine.GetRenderer(), sqrt(engine.GetTerrainManager().GetTerrainsVector().size()), NumberOfGrids, LineLength);
			break;
		case 'H':
			engine.GetTerrainManager().IncreaseMaxRenderDistance();
//			std::cout << "Odleglosc rysowania: " << engine.GetTerrainManager().GetMaxRenderDistance() << std::endl;
			std::cout << "Rendering distance: " << engine.GetTerrainManager().GetMaxRenderDistance() << std::endl;
			break;
		case 'N':
			engine.GetTerrainManager().LowerMaxRenderDistance();
//			std::cout << "Odleglosc rysowania: " << engine.GetTerrainManager().GetMaxRenderDistance() << std::endl;
			std::cout << "Rendering distance: " << engine.GetTerrainManager().GetMaxRenderDistance() << std::endl;
			break;
		case 'G':
			LineLength++;
			engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),TerrainSize, LineLength , NumberOfGrids);
			break;
		case 'B':
			LineLength--;
			engine.GetTerrainManager().CreateChunks(engine.GetRenderer(),TerrainSize, LineLength , NumberOfGrids);

			break;
		case 'T':
			engine.GetRenderer()->ChangeFillMode();
			break;
		case 'Y':
			optimized = !optimized;
			//std::cout << "optymalizacja: " << optimized << std::endl;
			std::cout << "Optimization: " << optimized << std::endl;
			break;
		case 'U':
			engine.GetRenderer()->ChangeFrontCounterClockwise();
			break;
		case 'J':
			std::cout << "camPos: " << engine.GetCamera()->GetCameraPos().x << "," <<  engine.GetCamera()->GetCameraPos().z<< std::endl;
			break;
		case 'K':
			if(engine.SaveTerrainDataToFile("teren.dat"))
			{
				std::cout << "Saved as teren.dat" << std::endl;
//				std::cout << "Zapisano jako teren.dat" << std::endl;
			}
			break;
		case 'L':
			if(	engine.LoadTerrainDataFromFile("teren.dat"))
			{
//				std::cout << "Zaladowano teren.dat" << std::endl;
				std::cout << "Loaded teren.dat" << std::endl;
			}
			break;
		case VK_ESCAPE:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case 'W':
			engine.GetKeyboard()->SetKeyUp('w');
			break;
		case 'S':
			engine.GetKeyboard()->SetKeyUp('s');
			break;
		case 'A':
			engine.GetKeyboard()->SetKeyUp('a');
			break;
		case 'D':
			engine.GetKeyboard()->SetKeyUp('d');
			break;
		case 'Z':
			engine.GetKeyboard()->SetKeyUp('z');
			break;
		case 'X':
			engine.GetKeyboard()->SetKeyUp('x');
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	case WM_QUIT:

		break;
	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
	//handle messages
	return 0;
}
