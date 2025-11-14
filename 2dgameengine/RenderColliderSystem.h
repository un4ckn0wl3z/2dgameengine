#pragma once

#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "Logger.h"
#include "SDL.h"

class RenderColliderSystem : public System {
public:
	RenderColliderSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();

	}

	void Update(SDL_Renderer* renderer, SDL_Rect& camera) {

		for (auto entity: GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect colliderRect = {
				static_cast<int>((transform.position.x - camera.x) + collider.offset.x),
				static_cast<int>((transform.position.y - camera.y) + collider.offset.y),
				static_cast<int>(collider.width * transform.scale.x),
				static_cast<int>(collider.height * transform.scale.y)
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &colliderRect);
		}

	}
};