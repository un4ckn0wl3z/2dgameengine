#pragma once
#include <iostream>
#include <string>
#include <SDL.h>

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	SDL_Rect srcRect;

	SpriteComponent(
		std::string assetId = "",
		int width = 0,
		int height = 0,
		int srcRectX = 0,
		int srcRectY = 0
	) {
		this->assetId = "";
		this->width = width;
		this->height = height;
		this->srcRect.x = srcRectX;
		this->srcRect.y = srcRectY;
		this->srcRect.w = width;
		this->srcRect.h = height;
	}
};