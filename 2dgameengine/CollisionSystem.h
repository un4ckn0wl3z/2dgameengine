#pragma once
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "Logger.h"
#include "SDL.h"
#include <Windows.h>
#include "CollisionEvent.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(std::unique_ptr<EventBus>& eventBus) {
		auto entities = GetSystemEntities();
		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity a = *i;
			auto aTransfrom = a.GetComponent<TransformComponent>();
			auto aCollider = a.GetComponent<BoxColliderComponent>();
			// check right entities
			for (auto j = i; j != entities.end(); j++) {
				Entity b = *j;
				if (a == b) continue; // bypass same entity
				auto bTransfrom = b.GetComponent<TransformComponent>();
				auto bCollider = b.GetComponent<BoxColliderComponent>();
				// check collision

				bool collisionHappened = CheckAABBCollision(
					aTransfrom.position.x + aCollider.offset.x,
					aTransfrom.position.y + aCollider.offset.y,
					aCollider.width,
					aCollider.height,
					bTransfrom.position.x + bCollider.offset.x,
					bTransfrom.position.y + bCollider.offset.y,
					bCollider.width,
					bCollider.height
				);

				if (collisionHappened) {
					Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));
					// emit event
					// MessageBox(NULL, L"Collision Detected", L"Is collision happened?", MB_OK);
					eventBus->EmitEvent<CollisionEvent>(a, b);

				}

			}

		}
		
	}

	bool CheckAABBCollision(
		double aX, 
		double aY, 
		double aW,
		double aH,
		double bX,
		double bY,
		double bW,
		double bH
		) {

		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
			);

	}
};