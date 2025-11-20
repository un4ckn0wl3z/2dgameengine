#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "Logger.h"
#include "Game.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deleta_time) {
		// Update entity 
		for (auto entity : GetSystemEntities()) {
			// update
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x * deleta_time;
			transform.position.y += rigidbody.velocity.y * deleta_time;
			
			bool isEntityOutsideMap = (
				transform.position.x < 0 ||
				transform.position.x > Game::s_mapWidth ||
				transform.position.y < 0 ||
				transform.position.y > Game::s_mapHeight
				);

			// remove entity that outside the map, except `player`
			if (isEntityOutsideMap && !entity.Hastag("player")) {
				entity.Kill();
			}

			// Logger::Log("Entity Id = " + std::to_string(entity.GetId()) + " position is now ( " + std::to_string(transform.position.x) + " ," + std::to_string(transform.position.y) + ")");

		}
	}
};