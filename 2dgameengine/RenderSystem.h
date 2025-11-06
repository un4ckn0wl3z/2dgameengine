#pragma once

#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Logger.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include "AssetStore.h"

class RenderSystem : public System {
public:
	RenderSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();

	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
		// Update entity 
		for (auto entity : GetSystemEntity()) {
			// update
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();
 
			SDL_Rect dstRect{
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_Rect srcRect = sprite.srcRect;

			SDL_RenderCopy(
				renderer, 
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect
				);



		}
	}
};