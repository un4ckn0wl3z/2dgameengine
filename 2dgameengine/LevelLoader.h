#pragma once
#include "ECS.h"
#include <memory>
#include "AssetStore.h"
#include "SDL.h"

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(
		const std::unique_ptr<Registry>& registry, 
		const std::unique_ptr<AssetStore>& assetStore,
		SDL_Renderer* renderer,
		int level);
};