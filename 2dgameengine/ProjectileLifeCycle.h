#pragma once
#include "ECS.h"
#include "ProjectileComponent.h"
#include "SDL.h"
class ProjectileLifeCycle : public System {
public:

	ProjectileLifeCycle() {
		RequireComponent<ProjectileComponent>();
	}

	void Update() {
		for (auto entity: GetSystemEntities()) {
			auto projectile = entity.GetComponent<ProjectileComponent>();

			// kill projectile
			if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
				entity.Kill();
			}

		}
	}
};