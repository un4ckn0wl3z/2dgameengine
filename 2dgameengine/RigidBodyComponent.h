#pragma once
#include "glm.hpp"

enum { UP, RIGHT, DOWN, LEFT };


struct RigidBodyComponent {
	glm::vec2 velocity;
	int direction;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0), int direction = UP) {
		this->velocity = velocity;
		this->direction = direction;
	}
};