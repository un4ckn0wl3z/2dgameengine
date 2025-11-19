#pragma once
#include "Event.h"
#include "ECS.h"

class MousePressedEvent : public Event {
public:
	float mouseX;
	float mouseY;
	MousePressedEvent(float mouseX = 0, float mouseY = 0) : mouseX(mouseX), mouseY(mouseY) {}
};