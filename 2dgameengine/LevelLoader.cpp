#include "LevelLoader.h"
#include <fstream>
#include "glm.hpp"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"
#include "TextLabelComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "ProjectileEmitterComponent.h"
#include "AnimationComponent.h"
#include "KeyboardControlledComponent.h"
#include "CameraFollowComponent.h"
#include "Logger.h"
#include "lua.hpp"
#include "sol.hpp"


LevelLoader::LevelLoader() {
	Logger::Log("LevelLoader constrctor called!");
}

LevelLoader::~LevelLoader() {
	Logger::Log("LevelLoader deconstrctor called!");
}

void LevelLoader::LoadLevel(
	const std::unique_ptr<Registry>& registry,
	const std::unique_ptr<AssetStore>& assetStore,
	SDL_Renderer* renderer,
	int level) {

	// dynamic loading from lua script





	//// Adding assets
	//assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	//assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	//assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	//assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	//assetStore->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
	//assetStore->AddTexture(renderer, "tree-image", "./assets/images/tree.png");

	//// load fonts
	//assetStore->AddFont("charriot-font-20", "./assets/fonts/charriot.ttf", 20);
	//assetStore->AddFont("pico8-font-5", "./assets/fonts/pico8.ttf", 5);
	//assetStore->AddFont("pico8-font-10", "./assets/fonts/pico8.ttf", 10);


	//// Load tilemap
	//assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	//int tileSize = 32;
	//double tileScale = 2.0;
	//int mapNumCols = 25;
	//int mapNumRows = 20;
	//std::fstream mapFile;
	//mapFile.open("./assets/tilemaps/jungle.map");
	//for (int y = 0; y < mapNumRows; y++) {
	//	for (int x = 0; x < mapNumCols; x++) {
	//		char ch;
	//		mapFile.get(ch);
	//		int srcRectY = std::atoi(&ch) * tileSize;
	//		mapFile.get(ch);
	//		int srcRectX = std::atoi(&ch) * tileSize;
	//		mapFile.ignore();
	//		Entity tile = registry->CreateEntity();
	//		tile.Group("tiles");
	//		tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
	//		tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
	//	}
	//}

	//mapFile.close();
	//Game::s_mapWidth = mapNumCols * tileSize * tileScale;
	//Game::s_mapHeight = mapNumRows * tileSize * tileScale;

	//////// Create entity
	//Entity tank = registry->CreateEntity();
	//tank.AddComponent<TransformComponent>(glm::vec2(500.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	//tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
	//tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
	//tank.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	////tank.AddComponent<ProjectileEmitterComponent>(
	////	glm::vec2(100.0, 0.0), 500, 5000, 10, false
	////);
	//tank.AddComponent<HealthComponent>(100);
	//tank.Group("enemies");

	//Entity treeA = registry->CreateEntity();
	//treeA.Group("obstacles");
	//treeA.AddComponent<TransformComponent>(glm::vec2(600.0, 495.0), glm::vec2(1.0, 1.0), 0.0);
	//treeA.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	//treeA.AddComponent<BoxColliderComponent>(16, 32);


	//Entity treeB = registry->CreateEntity();
	//treeB.Group("obstacles");
	//treeB.AddComponent<TransformComponent>(glm::vec2(400, 495.0), glm::vec2(1.0, 1.0), 0.0);
	//treeB.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	//treeB.AddComponent<BoxColliderComponent>(16, 32);

	//////// Create entity
	//Entity truck = registry->CreateEntity();
	//truck.AddComponent<TransformComponent>(glm::vec2(120.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	//truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	//truck.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	//truck.AddComponent<ProjectileEmitterComponent>(
	//	glm::vec2(00, 100.0), 500, 5000, 10, false
	//);
	//truck.AddComponent<HealthComponent>(100);
	//truck.Group("enemies");

	//// Create entity
	//Entity chopper = registry->CreateEntity();
	//chopper.AddComponent<TransformComponent>(glm::vec2(245.0, 110.0), glm::vec2(1.0, 1.0), 0.0);
	//chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	//chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 4);
	//chopper.AddComponent<AnimationComponent>(2, 10, true);
	//chopper.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
	//chopper.AddComponent<KeyboardControlledComponent>(
	//	glm::vec2(0.0, -80.0),
	//	glm::vec2(80.0, 0.0),
	//	glm::vec2(0.0, 80.0),
	//	glm::vec2(-80.0, 0.0)
	//);
	//chopper.AddComponent<CameraFollowComponent>();
	//chopper.AddComponent<HealthComponent>(100);
	//chopper.AddComponent<ProjectileEmitterComponent>(
	//	glm::vec2(300.0, 300.0), 0, 10000, 10, true
	//);
	//chopper.Tag("player");

	//// Create entity
	//Entity radar = registry->CreateEntity();
	//radar.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	//radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	//radar.AddComponent<AnimationComponent>(8, 5, true);
	//radar.Group("hud");

	//SDL_Color green = { 0, 255, 0 };

	//Entity label = registry->CreateEntity();
	//label.AddComponent<TextLabelComponent>(
	//	glm::vec2(Game::s_windowWidth / 2 - 30, 10.0),
	//	"CHOPPER 1.0",
	//	"charriot-font-20",
	//	green,
	//	true
	//);
	//label.Group("hud");

}