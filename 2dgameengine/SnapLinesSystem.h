#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Logger.h"
#include "SDL.h"

class SnapLinesSystem : public System {
private:
	int m_playerPosX;
	int m_playerPosY;
public:
	SnapLinesSystem() {
		// Required components
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		m_playerPosX = 0;
		m_playerPosY = 0;
	}

	void Update(SDL_Renderer* renderer, const SDL_Rect& camera) {

		for (auto entity : GetSystemEntities()) {
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& sprite = entity.GetComponent<SpriteComponent>();

 
			if (entity.Hastag("player")) {
				m_playerPosX = (transform.position.x - camera.x) + (transform.scale.x * sprite.width /2);
				m_playerPosY = (transform.position.y - camera.y) + (transform.scale.y * sprite.height /2);
			}

			if (entity.BelongsToGroup("enemies")) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawLine(renderer, m_playerPosX, m_playerPosY, (transform.position.x - camera.x) + (transform.scale.x * sprite.width / 2), (transform.position.y - camera.y) + (transform.scale.y * sprite.height / 2));
			}

		}

	}
};