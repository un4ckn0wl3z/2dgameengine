#pragma once
#include "ECS.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<AnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update() {
		// Update entity 
		for (auto entity : GetSystemEntities()) {
			auto& animation = entity.GetComponent<AnimationComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			// 
			// change the current frame
			// change srcrect
			// ...
			animation.currentFrame = 
				((SDL_GetTicks() - animation.startTime) *
					animation.frameRateSpeed / 1000) % animation.numFrames;


			sprite.srcRect.x = animation.currentFrame * sprite.width;


		}
	}

};