#pragma once
#include "ECS.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "KeypressedEvent.h"
#include "EventBus.h"
#include "Logger.h"

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {

	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeypressedEvent>(this, &KeyboardControlSystem::OnKeypressed);
	}

	void OnKeypressed(KeypressedEvent& event) {
		std::string keyCode = std::to_string(event.symbol);
		std::string keySymbol(1, event.symbol);
		Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
	}

	void Update() {

	}
};