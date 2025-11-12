#pragma once
#include "ECS.h"
#include "BoxColliderComponent.h"
#include "EventBus.h"
#include "CollisionEvent.h"

class DamageSystem : public System {
public:
	DamageSystem() {
		RequireComponent<BoxColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
	}

	void onCollision(CollisionEvent& event) {

	}

	void Update() {

	}
};