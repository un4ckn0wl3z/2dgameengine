#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "Logger.h"
#include "Game.h"
#include "Event.h"
#include "CollisionEvent.h"
#include "SDL.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	void OnCollision(CollisionEvent& event) {
		Entity a = event.a;
		Entity b = event.b;

		if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles")) {
			OnEnemyHitsObstacle(a, b);
		}

		if (a.BelongsToGroup("obstacles") && b.BelongsToGroup("enemies")) {
			OnEnemyHitsObstacle(b, a);
		}

	}

	void OnEnemyHitsObstacle(Entity enemy, Entity obstacle) {
		if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>()) {
			auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();
			auto& sprite = enemy.GetComponent<SpriteComponent>();
			if (rigidbody.velocity.x != 0) {
				rigidbody.velocity.x *= -1;
				sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
			}
			if (rigidbody.velocity.y != 0) {
				rigidbody.velocity.y *= -1;
				sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
			}
		}
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