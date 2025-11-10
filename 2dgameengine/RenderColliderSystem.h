#pragma once

#include "ECS.h"
#include "BoxColliderComponent.h"
#include "Logger.h"
#include "SDL.h"

class RenderColliderSystem : public System {
public:
	RenderColliderSystem() {
		// Required components
		RequireComponent<BoxColliderComponent>();

	}

	void Update() {
	
		
	}
};