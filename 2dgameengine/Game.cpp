#include "Game.h"
#include "SDL.h"
#include <iostream>

Game::Game() {
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

	m_window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
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


};

void Game::Run() {
	
	while (1) {
		ProcesInput();
		Update();
		Render();
	}
};

void Game::ProcesInput() {

	SDL_Event sdlEvent;
	SDL_PollEvent(&sdlEvent);




};

void Game::Update() {};

void Game::Render() {};

void Game::Destroy() {

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
