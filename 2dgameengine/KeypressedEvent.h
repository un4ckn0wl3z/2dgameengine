#pragma once
#include "Event.h"
#include "SDL.h"
#include "ECS.h"

class KeypressedEvent : public Event {
public:
	SDL_Keycode symbol;
	KeypressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};