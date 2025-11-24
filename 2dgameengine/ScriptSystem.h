#pragma once

#include "ECS.h"
#include "ScriptComponent.h"
#include "Logger.h"
#include "SDL.h"

class ScriptSystem : public System {
public:
	ScriptSystem() {
		// Required components
		RequireComponent<ScriptComponent>();
	}

	void Update() {
		for (auto entity: GetSystemEntities()) {
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(); // invoke lua script function
		}
	}
};