#pragma once
#include "ECS.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "KeypressedEvent.h"
#include "EventBus.h"
#include "Logger.h"
#include "KeyboardControlledComponent.h"
#include "SDL.h"
 
class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeypressedEvent>(this, &KeyboardControlSystem::OnKeypressed);
	}

	void OnKeypressed(KeypressedEvent& event) {
		//std::string keyCode = std::to_string(event.symbol);
		//std::string keySymbol(1, event.symbol);
		//Logger::Log("Key pressed event emitted: [" + keyCode + "] " + keySymbol);

		// change sprite and velocity
		for (auto entity : GetSystemEntities()) {
			auto keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

			switch (event.symbol) {
			case SDLK_w: 
				break;
			case SDLK_d:
				break;
			case SDLK_s:
				break;
			case SDLK_a:
				break;
			}

		}

	}

	void Update() {

	}
};