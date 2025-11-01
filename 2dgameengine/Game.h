#pragma once
#include "SDL.h"

class Game
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void ProcesInput();
	void Update();
	void Render();
	void Destroy();
};

