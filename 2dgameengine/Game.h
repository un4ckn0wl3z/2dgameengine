#pragma once
#include "SDL.h"

class Game
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_IsRunning;

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

