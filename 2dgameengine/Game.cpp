#include "Game.h"
#include "SDL.h"
#include <iostream>

Game::Game() {
	m_IsRunning = false;
	std::cout << "Game contructor called" << std::endl;
};

Game::~Game() {
	std::cout << "Game decontructor called" << std::endl;
};

void Game::Initialize() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = displayMode.w;
	windowsHeight = displayMode.h;

	m_window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowsHeight,
		SDL_WINDOW_BORDERLESS
	);

	if (!m_window) {
		std::cerr << "Error initializing SDL window." << std::endl;
		return;
	}

	m_renderer = SDL_CreateRenderer(
		m_window,
		-1,
		0
	);

	if (!m_renderer) {
		std::cerr << "Error initializing SDL renderer." << std::endl;
		return;
	}

	m_IsRunning = true;


};

void Game::Run() {
	
	while (m_IsRunning) {
		ProcesInput();
		Update();
		Render();
	}
};

void Game::ProcesInput() {

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) m_IsRunning = false;
			break;


		}
	}
};

void Game::Update() {};

void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	SDL_RenderPresent(m_renderer);

};

void Game::Destroy() {

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
