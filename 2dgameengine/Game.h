#pragma once
#include "SDL.h"
#include "ECS.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_IsRunning;
	int m_MillisecsPreviousFrame = 0;
	std::unique_ptr<Registry> m_registry;


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

	int windowWidth;
	int windowsHeight;

};

