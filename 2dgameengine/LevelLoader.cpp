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

void LevelLoader::LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, int levelNumber) {
    // This checks the syntax of our script, but it does not execute the script
    sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
    if (!script.valid()) {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    // Executes the script using the Sol state
    lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

    // Read the big table for the current level
    sol::table level = lua["Level"];

    // Read the level assets
    sol::table assets = level["assets"];

    int i = 0;
    while (true) {
        sol::optional<sol::table> assetsOpt = assets[i];
        if (assetsOpt == sol::nullopt) {
            break;
        }
        sol::table asset = *assetsOpt;
        std::string assetType = asset["type"];
        std::string assetId = asset["id"];
        if (assetType == "texture") {
            assetStore->AddTexture(renderer, assetId, asset["file"]);
            Logger::Log("A new texture asset was added to the asset store, id: " + assetId);
        }
        if (assetType == "font") {
            assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
            Logger::Log("A new font asset was added to the asset store, id: " + assetId);
        }
        i++;
    }

    // Read the level tilemap information
    sol::table map = level["tilemap"];
    std::string mapFilePath = map["map_file"];
    std::string mapTextureAssetId = map["texture_asset_id"];
    int mapNumRows = map["num_rows"];
    int mapNumCols = map["num_cols"];
    int tileSize = map["tile_size"];
    double mapScale = map["scale"];
    std::fstream mapFile;
    mapFile.open(mapFilePath);
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)), glm::vec2(mapScale, mapScale), 0.0);
            tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    Game::s_mapWidth = mapNumCols * tileSize * mapScale;
    Game::s_mapHeight = mapNumRows * tileSize * mapScale;

    
    // Read the level entities and their components
    sol::table entities = level["entities"];
    i = 0;
    while (true) {
        sol::optional<sol::table> entityOps = entities[i];
        if (entityOps == sol::nullopt) {
            break;
        }

        sol::table entity = *entityOps;

        Entity newEntity = registry->CreateEntity();

        // Tag
        sol::optional<std::string> tagOps = entity["tag"];
        if (tagOps != sol::nullopt) {
            newEntity.Tag(entity["tag"]);
        }

        // Group
        sol::optional<std::string> groupOps = entity["group"];
        if (groupOps != sol::nullopt) {
            newEntity.Group(entity["group"]);
        }

        // Components
        sol::optional<sol::table> componentsOps = entity["components"];
        if (componentsOps != sol::nullopt) {
            // Transform
            sol::optional<sol::table> transformOps = entity["components"]["transform"];
            if (transformOps != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    glm::vec2(
                        entity["components"]["transform"]["position"]["x"],
                        entity["components"]["transform"]["position"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                        entity["components"]["transform"]["scale"]["y"].get_or(1.0)
                    ),
                    entity["components"]["transform"]["rotation"].get_or(0.0)
                );
            }

            // RigidBody
            sol::optional<sol::table> rigidbodyOps = entity["components"]["rigidbody"];
            if (rigidbodyOps != sol::nullopt) {
                newEntity.AddComponent<RigidBodyComponent>(
                    glm::vec2(
                        entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                        entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                    )
                );
            }

            // Sprite
            sol::optional<sol::table> spriteOps = entity["components"]["sprite"];
            if (spriteOps != sol::nullopt) {
                newEntity.AddComponent<SpriteComponent>(
                    entity["components"]["sprite"]["texture_asset_id"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["z_index"].get_or(1),
                    entity["components"]["sprite"]["fixed"].get_or(false),
                    entity["components"]["sprite"]["src_rect_x"].get_or(0),
                    entity["components"]["sprite"]["src_rect_y"].get_or(0)
                );
            }

            // Animation
            sol::optional<sol::table> animationOps = entity["components"]["animation"];
            if (animationOps != sol::nullopt) {
                newEntity.AddComponent<AnimationComponent>(
                    entity["components"]["animation"]["num_frames"].get_or(1),
                    entity["components"]["animation"]["speed_rate"].get_or(1)
                );
            }

            // BoxCollider
            sol::optional<sol::table> colliderOps = entity["components"]["boxcollider"];
            if (colliderOps != sol::nullopt) {
                newEntity.AddComponent<BoxColliderComponent>(
                    entity["components"]["boxcollider"]["width"],
                    entity["components"]["boxcollider"]["height"],
                    glm::vec2(
                        entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                        entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                    )
                );
            }

            // Health
            sol::optional<sol::table> healthOps = entity["components"]["health"];
            if (healthOps != sol::nullopt) {
                newEntity.AddComponent<HealthComponent>(
                    static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
                );
            }

            // ProjectileEmitter
            sol::optional<sol::table> projectileEmitterOps = entity["components"]["projectile_emitter"];
            if (projectileEmitterOps != sol::nullopt) {
                newEntity.AddComponent<ProjectileEmitterComponent>(
                    glm::vec2(
                        entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                        entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
                    ),
                    static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
                    entity["components"]["projectile_emitter"]["friendly"].get_or(false)
                );
            }

            // CameraFollow
            sol::optional<sol::table> cameraFollowOps = entity["components"]["camera_follow"];
            if (cameraFollowOps != sol::nullopt) {
                newEntity.AddComponent<CameraFollowComponent>();
            }

            // KeyboardControlled
            sol::optional<sol::table> keyboardControlledOps = entity["components"]["keyboard_controller"];
            if (keyboardControlledOps != sol::nullopt) {
                newEntity.AddComponent<KeyboardControlledComponent>(
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["up_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["right_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["down_velocity"]["y"]
                    ),
                    glm::vec2(
                        entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                        entity["components"]["keyboard_controller"]["left_velocity"]["y"]
                    )
                );
            }
        }
        i++;
    }
}
