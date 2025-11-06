#pragma once

#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Logger.h"
#include "SpriteComponent.h"
#include "SDL.h"

class RenderSystem : public System {
public:
	RenderSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();

	}

	void Update(SDL_Renderer* renderer) {
		// Update entity 
		for (auto entity : GetSystemEntity()) {
			// update
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();
 
			SDL_Rect object{
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};
			SDL_SetRenderDrawColor(renderer, 255,255,255,255);
			SDL_RenderFillRect(renderer, &object);

		}
	}
};