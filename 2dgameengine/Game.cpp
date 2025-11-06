#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "glm.hpp"
#include "Logger.h"
#include "ECS.h"
#include <iostream>
#include "TransformComponent.h"
#include "RigidBodyComponent.h"


Game::Game() {
	m_IsRunning = false;
	m_registry = std::make_unique<Registry>();
	Logger::Log("Game contructor called");
};

Game::~Game() {
	Logger::Log("Game decontructor called");
};

void Game::Initialize() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL.");
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
		Logger::Err("Error initializing SDL window.");
		return;
	}

	m_renderer = SDL_CreateRenderer(
		m_window,
		-1,
		0
	);

	if (!m_renderer) {
		Logger::Err("Error initializing SDL renderer.");
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



void  Game::Setup() {
	// Create entity
	Entity tank = m_registry->CreateEntity();
	//m_registry->AddComponent<TransformComponent>(
	//	tank, 
	//	glm::vec2(10.0, 30.0),
	//	glm::vec2(1, 1),
	//	0.0
	//);

	//m_registry->AddComponent<RigidBodyComponent>(
	//	tank,
	//	glm::vec2(50.0, 0)
	//);
	tank.AddComponent<TransformComponent>(
		glm::vec2(10.0, 30.0),
		glm::vec2(1, 1),
		0.0
	);

	tank.AddComponent<RigidBodyComponent>(
		glm::vec2(50.0, 0)
	);

	tank.RemoveComponent<TransformComponent>();

}

void Game::Update() {
	// frame control
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_MillisecsPreviousFrame + MILLISECS_PER_FRAME));
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - m_MillisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);
	
	double deltaTime = (SDL_GetTicks() - m_MillisecsPreviousFrame) / 1000.0;
	m_MillisecsPreviousFrame = SDL_GetTicks();

	// delegate System update
	// MovementSystem.Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();
};

void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// Render system

	// draw (switch-buffer)
	SDL_RenderPresent(m_renderer);

};

void Game::Destroy() {

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
