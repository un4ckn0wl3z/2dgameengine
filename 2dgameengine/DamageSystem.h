#pragma once
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"
#include "EventBus.h"
#include "CollisionEvent.h"
#include "Logger.h"

class DamageSystem : public System {
public:
	DamageSystem() {
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
	}

	void onCollision(CollisionEvent& event) {
		Entity a = event.a;
		Entity b = event.b;
		Logger::Log("DamageSystem got CollisionEvent! [entity]: " + std::to_string(a.GetId()) + "  and " + std::to_string(b.GetId()));

		if (a.BelongsToGroup("projectiles") && b.Hastag("player")) {
			OnProjectileHitsPlayer(a, b);
		}

		if (b.BelongsToGroup("projectiles") && a.Hastag("player")) {
			OnProjectileHitsPlayer(b, a);
		}

		if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) {
			// todo:

		}

		if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) {
			// todo:

		}
	}

	void OnProjectileHitsPlayer(Entity projectile, Entity player) {
		auto projectileComponent = projectile.GetComponent<ProjectileComponent>();
		if (!projectileComponent.isFriendly) {
			// reduce player health
			auto& health = player.GetComponent<HealthComponent>();
			health.healthPercentage -= projectileComponent.hitPercentDamage;
		
			// player died
			if (health.healthPercentage <= 0) {
				player.Kill();
			}

			projectile.Kill();
		}
	}

	void Update() {

	}
};