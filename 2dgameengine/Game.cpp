#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "glm.hpp"
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

	windowWidth = 800;// displayMode.w;
	windowsHeight = 600;// displayMode.h;

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

	// SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
	m_IsRunning = true;
};

void Game::Run() {
	Setup();
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

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void  Game::Setup() {
	// setup game object
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(100, 0.0);

}

void Game::Update() {
	// frame control
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_MillisecsPreviousFrame + MILLISECS_PER_FRAME));
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - m_MillisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);
	
	double deltaTime = (SDL_GetTicks() - m_MillisecsPreviousFrame) / 1000.0;
	m_MillisecsPreviousFrame = SDL_GetTicks();

	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;
};

void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// render game object
	//SDL_Rect player = {
	//	10, 10, 20, 20
	//};
	//SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(m_renderer, &player);

	// draw texture
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = {
		static_cast<int>(playerPosition.x), 
		static_cast<int>(playerPosition.y), 
		32, 
		32
	};
	SDL_RenderCopy(m_renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	// draw (switch-buffer)
	SDL_RenderPresent(m_renderer);

};

void Game::Destroy() {

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
