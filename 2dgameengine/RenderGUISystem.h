#pragma once
#include "ECS.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SDL.h"
#include "glm.hpp"

class RenderGUISystem : public System {
public:
	RenderGUISystem() = default;

	void Update(const std::unique_ptr<Registry>& registry, SDL_Renderer* renderer) {
		// draw debug menu
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
		//	ImGuiWindowFlags_NoNav |
		//	ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Spawn Enemies")) {
			static int enemyXPos = 0;
			static int enemyYPos = 0;
			ImGui::InputInt("enemy x position", &enemyXPos);
			ImGui::InputInt("enemy y position", &enemyYPos);

			if (ImGui::Button("Create new enemy")) {
				Entity enemy = registry->CreateEntity();
				enemy.AddComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(1.0, 1.0), 0.0);
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
				enemy.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				enemy.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(100.0, 0.0), 500, 5000, 10, false
				);
				enemy.AddComponent<HealthComponent>(100);
				enemy.Group("enemies");
			}

		}
		ImGui::End();


		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

	}

};