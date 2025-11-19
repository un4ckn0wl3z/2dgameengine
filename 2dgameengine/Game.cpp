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
#include "KeyboardControlledComponent.h"
#include "CameraFollowComponent.h"
#include "ProjectileEmitterComponent.h"
#include "HealthComponent.h"


#include "MovementSystem.h"
#include "SpriteComponent.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "RenderColliderSystem.h"
#include "DamageSystem.h"
#include "KeyboardControlSystem.h"
#include "CameraMovementSystem.h"
#include "ProjectileEmitSystem.h"
#include "ProjectileLifeCycleSystem.h"
#include "RenderTextSystem.h"
#include "RenderHealthBarSystem.h"
#include "RenderGUISystem.h"

#include "AnimationSystem.h"
#include "AssetStore.h"
#include <fstream>
#include "EventBus.h"
#include "KeypressedEvent.h"
#include "MousePressedEvent.h"

#include "TextLabelComponent.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

int Game::s_windowWidth;
int Game::s_windowsHeight;
int Game::s_mapWidth;
int Game::s_mapHeight;

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

	if (TTF_Init() != 0) {
		Logger::Err("Error initializing TTF.");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	s_windowWidth = 1280;// displayMode.w; 800
	s_windowsHeight = 720;// displayMode.h; 600

	m_window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		s_windowWidth,
		s_windowsHeight,
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

	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);


	// init imgui
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDLRenderer2_Init(m_renderer);
	ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);


	// init camera
	m_camera.x = 0;
	m_camera.y = 0;
	m_camera.w = s_windowWidth;
	m_camera.h = s_windowsHeight;


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
		// Passing event to Imgui
		//ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		//ImGuiIO& io = ImGui::GetIO();
		//int mouseX, mouseY;
		//const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		//io.MousePos = ImVec2(mouseX, mouseY);
		//io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		//io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

		switch (sdlEvent.type) {
		
		case SDL_QUIT:
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) m_isRunning = false;
			if (sdlEvent.key.keysym.sym == SDLK_p) m_isDebug = !m_isDebug;
			m_eventBus->EmitEvent<KeypressedEvent>(sdlEvent.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
				SDL_Keymod mods = SDL_GetModState();
				if (mods & KMOD_LCTRL) {
					m_eventBus->EmitEvent<MousePressedEvent>(sdlEvent.button.x, sdlEvent.button.y);
				}
			}
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
	m_registry->AddSystem<KeyboardControlSystem>();
	m_registry->AddSystem<CameraMovementSystem>();
	m_registry->AddSystem<ProjectileEmitSystem>();
	m_registry->AddSystem<ProjectileLifeCycleSystem>();
	m_registry->AddSystem<RenderTextSystem>();
	m_registry->AddSystem<RenderHealthBarSystem>();
	m_registry->AddSystem<RenderGUISystem>();
	
	// Adding assets
	m_assetStore->AddTexture(m_renderer, "tank-image", "./assets/images/tank-panther-right.png");
	m_assetStore->AddTexture(m_renderer, "truck-image", "./assets/images/truck-ford-right.png");
	m_assetStore->AddTexture(m_renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	m_assetStore->AddTexture(m_renderer, "radar-image", "./assets/images/radar.png");
	m_assetStore->AddTexture(m_renderer, "bullet-image", "./assets/images/bullet.png");

	// load fonts
	m_assetStore->AddFont("charriot-font-20", "./assets/fonts/charriot.ttf", 20);
	m_assetStore->AddFont("pico8-font-5", "./assets/fonts/pico-8.ttf", 5);
	m_assetStore->AddFont("pico8-font-10", "./assets/fonts/pico-8.ttf", 10);


	// Load tilemap
	m_assetStore->AddTexture(m_renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

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
			tile.Group("tiles");
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}

	mapFile.close();
	s_mapWidth = mapNumCols * tileSize * tileScale;
	s_mapHeight = mapNumRows * tileSize * tileScale;

	////// Create entity
	Entity tank = m_registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
	tank.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	tank.AddComponent<ProjectileEmitterComponent>(
		glm::vec2(100.0, 0.0), 500, 5000, 10, false
	);
	tank.AddComponent<HealthComponent>(100);
	tank.Group("enemies");

	////// Create entity
	Entity truck = m_registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(120.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	truck.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	truck.AddComponent<ProjectileEmitterComponent>(
		glm::vec2(00, 100.0), 500, 5000, 10, false
	);
	truck.AddComponent<HealthComponent>(100);
	truck.Group("enemies");

	// Create entity
	Entity chopper = m_registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(245.0, 110.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2,10,true);
	chopper.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	chopper.AddComponent<KeyboardControlledComponent>(
		glm::vec2(0.0, -80.0),
		glm::vec2(80.0, 0.0),
		glm::vec2(0.0, 80.0),
		glm::vec2(-80.0, 0.0)
	);
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100);
	chopper.AddComponent<ProjectileEmitterComponent>(
		glm::vec2(300.0, 300.0), 0, 10000, 10, true
	);
	chopper.Tag("player");

	// Create entity
	Entity radar = m_registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	radar.AddComponent<AnimationComponent>(8, 5, true);
	radar.Group("hud");

	SDL_Color green = { 0, 255, 0 };

	Entity label = m_registry->CreateEntity();
	label.AddComponent<TextLabelComponent>(
		glm::vec2(s_windowWidth/2 - 30, 10.0),
		"CHOPPER 1.0", 
		"charriot-font-20",
		green,
		true
	);
	label.Group("hud");

}

void  Game::Setup() {
	LoadLevel(1);

}

void Game::Update() {
	// frame control
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_MillisecsPreviousFrame + MILLISECS_PER_FRAME));
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - m_millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) SDL_Delay(timeToWait);
	
	double deltaTime = (SDL_GetTicks() - m_millisecsPreviousFrame) / 1000.0;
	m_millisecsPreviousFrame = SDL_GetTicks();

	// reset all event handlers
	m_eventBus->Reset();

	// subscription events
	m_registry->GetSystem<DamageSystem>().SubscribeToEvents(m_eventBus);
	m_registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(m_eventBus);
	m_registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(m_eventBus);
	m_registry->GetSystem<RenderGUISystem>().SubscribeToEvents(m_eventBus);
	// invoke update system
	m_registry->GetSystem<MovementSystem>().Update(deltaTime);

	// invoke update system
	m_registry->GetSystem<AnimationSystem>().Update();

	// invoke update system
	m_registry->GetSystem<CollisionSystem>().Update(m_eventBus);

	m_registry->GetSystem<CameraMovementSystem>().Update(m_camera);

	m_registry->GetSystem<ProjectileEmitSystem>().Update(m_registry);

	m_registry->GetSystem<ProjectileLifeCycleSystem>().Update();
	// update entities
	m_registry->Update();


};

void Game::Render() {

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// invoke render system
	m_registry->GetSystem<RenderSystem>().Update(m_renderer, m_camera, m_assetStore);
	m_registry->GetSystem<RenderTextSystem>().Update(m_renderer, m_camera, m_assetStore);
	m_registry->GetSystem<RenderHealthBarSystem>().Update(m_renderer, m_camera, m_assetStore);

	if (m_isDebug) {
		m_registry->GetSystem<RenderColliderSystem>().Update(m_renderer, m_camera);
		//ImGui_ImplSDLRenderer2_NewFrame();
		//ImGui_ImplSDL2_NewFrame();
		//ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		//ImGui::Render();
		//ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);

		m_registry->GetSystem<RenderGUISystem>().Update(m_registry ,m_renderer, m_camera);


	}
	// draw (switch-buffer)
	SDL_RenderPresent(m_renderer);

};

void Game::Destroy() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

};
