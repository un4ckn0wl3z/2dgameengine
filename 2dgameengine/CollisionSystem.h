#pragma once

#pragma once
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

#include "SDL.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update() {
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




			}

		}
		
	}

};