#pragma once
#include "ECS.h"
#include "AnimationComponent.h"
#include "SpriteComponent.h"


class AnimationSystem : System {
public:
	AnimationSystem() {
		RequireComponent<AnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update() {
		// Update entity 
		for (auto entity : GetSystemEntities()) {
			auto animation = entity.GetComponent<AnimationComponent>();
			auto sprite = entity.GetComponent<SpriteComponent>();

			// 
			// change the current frame
			// change srcrect
			// ...



		}
	}

};