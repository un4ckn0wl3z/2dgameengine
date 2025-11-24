#pragma once

#include "ECS.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "Logger.h"
#include "SDL.h"
#include "lua.hpp"
#include "sol.hpp"

// native c++ for binding
//GetEntityPosition(Entity entity) {
//
//}

void SetEntityPosition(Entity entity, double x, double y) {
	if (entity.HasComponent<TransformComponent>()) {
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position.x = x;
		transform.position.y = y;
	}
	else {
		Logger::Err("Target entity has no transform component");
	}
}


class ScriptSystem : public System {
public:
	ScriptSystem() {
		// Required components
		RequireComponent<ScriptComponent>();
	}

	void CreateLuaBindings(sol::state& lua) {
		// create "entity" user-type
		lua.new_usertype<Entity>(
			"entity",
			"get_id", &Entity::GetId,
			"destroy", &Entity::Kill,
			"has_tag", &Entity::Hastag,
			"belongs_to_group", &Entity::BelongsToGroup
		);

		lua.set_function("set_position", SetEntityPosition);
	}

	void Update(double deltaTime, int ellapsedTime) {
		for (auto entity: GetSystemEntities()) {
			const auto script = entity.GetComponent<ScriptComponent>();
			script.func(entity, deltaTime, ellapsedTime); // invoke lua script function
		}
	}
};