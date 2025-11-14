#pragma once
#include "ECS.h"
#include "CameraFollowComponent.h"
#include "TransformComponent.h"
#include "SDL.h"
#include "Logger.h"
#include "Game.h"

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

			if (transform.position.x < Game::s_mapWidth) {
				camera.x = transform.position.x;
			}

			if (transform.position.y < Game::s_windowsHeight) {
				camera.y = transform.position.y;
			}

			// keep camera still insde screen limits
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;

			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.h : camera.y;


			Logger::Log("Camera changed position to :" + std::to_string(camera.x) + " , " + std::to_string(camera.y));
		}
	}
};