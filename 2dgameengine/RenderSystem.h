#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Logger.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include "AssetStore.h"
#include <algorithm>

class RenderSystem : public System {
public:
	RenderSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();

	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera, std::unique_ptr<AssetStore>& assetStore) {
		// get entities
		struct RenderableEntity {
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		std::vector<RenderableEntity> renderableEntities;

		for (auto entity : GetSystemEntities()) {
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();

			renderableEntities.emplace_back(renderableEntity);
		}

		// sort
		std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
			return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
		});

		// Update entity 
		for (auto entity : renderableEntities) {
			// update
			const auto transform = entity.transformComponent;
			const auto sprite = entity.spriteComponent;
 
			SDL_Rect dstRect{
				static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
				static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_Rect srcRect = sprite.srcRect;

			SDL_RenderCopyEx(
				renderer, 
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE
				);



		}
	}
};