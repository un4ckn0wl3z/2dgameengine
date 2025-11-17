#pragma once
#include "glm.hpp"
#include <string>
#include "SDL.h"

struct TextLabelComponent {
	glm::vec2 postion;
	std::string text;
	std::string assetId;
	SDL_Color color;
	bool isFixed;

	TextLabelComponent(
		glm::vec2 postion = glm::vec2(0),
		std::string text = "",
		std::string assetId = "",
		const SDL_Color& color = { 0,0,0 },
		bool isFixed = true
	) {
		this->postion = postion;
		this->text = text;
		this->assetId = assetId;
		this->color = color;
		this->isFixed = isFixed;
	}
};