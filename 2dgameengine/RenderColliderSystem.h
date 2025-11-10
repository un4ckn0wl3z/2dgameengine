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

	void Update(SDL_Renderer* renderer) {

		for (auto entity: GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect colliderRect = {
				static_cast<int>(transform.position.x + collider.offset.x),
				static_cast<int>(transform.position.y + collider.offset.y),
				static_cast<int>(collider.width),
				static_cast<int>(collider.height)
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &colliderRect);
		}

	}
};