#pragma once

#pragma once
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include "SDL.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update() {


		
	}

};