#pragma once
#include "ECS.h"
#include <memory>
#include "AssetStore.h"
#include "SDL.h"
#include "lua.hpp"
#include "sol.hpp"

class LevelLoader {
public:
	LevelLoader();
	~LevelLoader();

	void LoadLevel(
		sol::state& lua,
		const std::unique_ptr<Registry>& registry, 
		const std::unique_ptr<AssetStore>& assetStore,
		SDL_Renderer* renderer,
		int level);
};