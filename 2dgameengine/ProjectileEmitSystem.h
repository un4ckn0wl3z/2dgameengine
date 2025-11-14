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
				Entity projectile =  registry->CreateEntity();
				projectile.AddComponent<TransformComponent>(transform.position, glm::vec2(1.0, 1.0), 0.0);
				projectile.AddComponent<RigidBodyComponent>(projectileEmitter.projectileVelocity);
				projectile.AddComponent<SpriteComponent>("bullet-image",4,4,4);
				projectile.AddComponent<BoxColliderComponent>( 4,4);
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
				
			}

		}
	}


};