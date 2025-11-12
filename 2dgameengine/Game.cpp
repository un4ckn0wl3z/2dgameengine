#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "glm.hpp"
#include "Logger.h"
#include "ECS.h"
#include <iostream>
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "AnimationComponent.h"
#include "BoxColliderComponent.h"

#include "MovementSystem.h"
#include "SpriteComponent.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "RenderColliderSystem.h"
#include "DamageSystem.h"

#include "AnimationSystem.h"
#include "AssetStore.h"
#include <fstream>
#include "EventBus.h"

Game::Game() {
	m_isRunning = false;
	m_isDebug = false;
	m_registry = std::make_unique<Registry>();
	m_assetStore = std::make_unique<AssetStore>();
	m_eventBus = std::make_unique<EventBus>();
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
	m_isRunning = true;
};

void Game::Run() {
	Setup();
	while (m_isRunning) {
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
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) m_isRunning = false;
			if (sdlEvent.key.keysym.sym == SDLK_d) m_isDebug = !m_isDebug;
			break;


		}
	}
};

void Game::LoadLevel(int level) {

	// create system
	m_registry->AddSystem<MovementSystem>();
	m_registry->AddSystem<RenderSystem>();
	m_registry->AddSystem<AnimationSystem>();
	m_registry->AddSystem<CollisionSystem>();
	m_registry->AddSystem<RenderColliderSystem>();
	m_registry->AddSystem<DamageSystem>();
	

	// Adding assets
	m_assetStore->AddAssets(m_renderer, "tank-image", "./assets/images/tank-panther-left.png");
	m_assetStore->AddAssets(m_renderer, "truck-image", "./assets/images/truck-ford-right.png");
	m_assetStore->AddAssets(m_renderer, "chopper-image", "./assets/images/chopper.png");
	m_assetStore->AddAssets(m_renderer, "radar-image", "./assets/images/radar.png");

	// Load tilemap
	m_assetStore->AddAssets(m_renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;
	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");
	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();
			Entity tile = m_registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}

	mapFile.close();


	////// Create entity
	Entity tank = m_registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-30.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
	tank.AddComponent<BoxColliderComponent>(32, 32);

	////// Create entity
	Entity truck = m_registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	truck.AddComponent<BoxColliderComponent>(32, 32);

	// Create entity
	Entity chopper = m_registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(windowWidth/2, (windowsHeight / 2) - 50), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2,10,true);
	chopper.AddComponent<BoxColliderComponent>(32, 32);

	// Create entity
	Entity radar = m_registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1);
	radar.AddComponent<AnimationComponent>(8, 5, true);

}

void  Game::Setup() {
	LoadLevel(1);

}

void Game::Update() {
	// frame control
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_MillisecsPreviousFrame + MILLISECS_PER_FRAME));
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - m_MillisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);
	
	double deltaTime = (SDL_GetTicks() - m_MillisecsPreviousFrame) / 1000.0;
	m_MillisecsPreviousFrame = SDL_GetTicks();

	// reset all event handlers
	m_eventBus->Reset();

	// subscription events
	m_registry->GetSystem<DamageSystem>().SubscribeToEvents(m_eventBus);



	// invoke update system
	m_registry->GetSystem<MovementSystem>().Update(deltaTime);

	// invoke update system
	m_registry->GetSystem<AnimationSystem>().Update();

	// invoke update system
	m_registry->GetSystem<CollisionSystem>().Update(m_eventBus);


	// update entities
	m_registry->Update();


};

void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// invoke render system
	m_registry->GetSystem<RenderSystem>().Update(m_renderer, m_assetStore);
	if (m_isDebug) {
		m_registry->GetSystem<RenderColliderSystem>().Update(m_renderer);
	}


	// draw (switch-buffer)
	SDL_RenderPresent(m_renderer);

};

void Game::Destroy() {

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
