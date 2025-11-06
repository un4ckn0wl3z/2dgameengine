#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update() {
		// Update entity 
		for (auto entity : GetSystemEntity()) {
			// update
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidbody.velocity.x;
			transform.position.y += rigidbody.velocity.y;

		}
	}
};