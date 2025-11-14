#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "ProjectileEmitterComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "SDL.h"
#include "glm.hpp"

class ProjectileEmitSystem : public System {
public:
	ProjectileEmitSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileEmitterComponent>();
	}

	void Update(std::unique_ptr<Registry>& registry) {
		for (auto entity: GetSystemEntities()) {
			auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
			const auto transform = entity.GetComponent<TransformComponent>();

			// re-emit
			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
				// add new projectile entity
				glm::vec2 projectilePosition = transform.position;
				if (entity.HasComponent<SpriteComponent>()) {
					const auto sprite = entity.GetComponent<SpriteComponent>();
					projectilePosition.x += (transform.scale.x * sprite.width / 2);
					projectilePosition.y += (transform.scale.y * sprite.height / 2);
				}

				Entity projectile =  registry->CreateEntity();
				projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image",4,4,4);
				projectile.AddComponent<BoxColliderComponent>( 4,4);
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
				
			}

		}
	}


};