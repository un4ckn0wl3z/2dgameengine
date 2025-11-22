#pragma once
#include "SDL.h"
#include "ECS.h"
#include "AssetStore.h"
#include "EventBus.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_isRunning;
	bool m_isDebug;
	int m_millisecsPreviousFrame = 0;
	std::unique_ptr<Registry> m_registry;
	std::unique_ptr<AssetStore> m_assetStore;
	std::unique_ptr<EventBus> m_eventBus;
	SDL_Rect m_camera;


public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void Setup();
	void ProcesInput();
	void Update();
	void Render();
	void Destroy();
 
	static int s_windowWidth;
	static int s_windowsHeight;
	static int s_mapWidth;
	static int s_mapHeight;

};

