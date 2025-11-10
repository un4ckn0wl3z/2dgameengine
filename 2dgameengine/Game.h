#pragma once
#include "SDL.h"
#include "ECS.h"
#include "AssetStore.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_isRunning;
	bool m_isDebug;
	int m_MillisecsPreviousFrame = 0;
	std::unique_ptr<Registry> m_registry;
	std::unique_ptr<AssetStore> m_assetStore;


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
	void LoadLevel(int level);

	int windowWidth;
	int windowsHeight;

};

