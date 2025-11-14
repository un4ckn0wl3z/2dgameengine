#pragma once
#include "ECS.h"
#include "CameraFollowComponent.h"
#include "TransformComponent.h"


class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update() {
		for (auto entity: GetSystemEntities()) {

		}
	}
};