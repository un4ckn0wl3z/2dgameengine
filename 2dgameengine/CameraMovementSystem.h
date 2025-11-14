#pragma once
#include "ECS.h"
#include "CameraFollowComponent.h"
#include "TransformComponent.h"
#include "SDL.h"
#include "Logger.h"

class CameraMovementSystem : public System {
public:
	CameraMovementSystem() {
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(SDL_Rect& camera) {
		for (auto entity: GetSystemEntities()) {
			auto transform = entity.GetComponent<TransformComponent>();
			// change camera base on entity
			camera.x = transform.position.x;
			camera.y = transform.position.y;
			Logger::Log("Camera changed position to :" + std::to_string(camera.x) + " , " + std::to_string(camera.y));
		}
	}
};