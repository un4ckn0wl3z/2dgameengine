#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "glm.hpp"
#include "Logger.h"
#include "ECS.h"
#include <iostream>

#include "MovementSystem.h"
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
#include "SnapLinesSystem.h"

#include "AnimationSystem.h"
#include "AssetStore.h"
#include <fstream>
#include "EventBus.h"
#include "KeypressedEvent.h"
#include "MousePressedEvent.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "LevelLoader.h"
#include "lua.hpp"
#include "sol.hpp"

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

	// SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);


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
					m_eventBus->EmitEvent<MousePressedEvent>(sdlEvent.button.x + m_camera.x, sdlEvent.button.y + m_camera.y);
				}
			}
			break;


		}
	}
};

void  Game::Setup() {

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
	m_registry->AddSystem<SnapLinesSystem>();

	
	LevelLoader loader;
	m_lua.open_libraries(sol::lib::base, sol::lib::math);
	loader.LoadLevel(m_lua, m_registry, m_assetStore, m_renderer, 1);

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
	m_registry->GetSystem<MovementSystem>().SubscribeToEvents(m_eventBus);
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
		m_registry->GetSystem<SnapLinesSystem>().Update(m_renderer, m_camera);
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
