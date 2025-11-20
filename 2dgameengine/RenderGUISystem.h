#pragma once
#include "ECS.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SDL.h"
#include "glm.hpp"
#include <cmath>
#include "Event.h"
#include "MousePressedEvent.h"

class RenderGUISystem : public System {
private:
	int m_posX;
	int m_posY;
public:
	RenderGUISystem(){
		this->m_posX = 0;
		this->m_posY = 0;
	};

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<MousePressedEvent>(this, &RenderGUISystem::OnMousePressed);
	}

	void OnMousePressed(MousePressedEvent& event) {
		m_posX = event.mouseX;
		m_posY = event.mouseY;
	}

	void Update(const std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, const SDL_Rect& camera) {
		// draw debug menu
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
		//	ImGuiWindowFlags_NoNav |
		//	ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Spawn enemies")) {

			static int scaleX = 1;
			static int scaleY = 1;
			static int velX = 0;
			static int velY = 0;
			static int health = 100;
			static float rotation = 0.0;
			static float projAngle = 0.0;
			static float projSpeed = 100.0;
			static int projRepeat = 10;
			static int projDuration = 10;
			const char* sprites[] = { "tank-image", "truck-image" };
			static int selectedSpriteIndex = 0;


			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Combo("texture id", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
			}
			ImGui::Spacing();


			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Text("Mouse click + LCtrl to automated get position");
				ImGui::InputInt("position x", &m_posX);
				ImGui::InputInt("position y", &m_posY);
				ImGui::SliderInt("scale x", &scaleX, 1, 10);
				ImGui::SliderInt("scale y", &scaleY, 1, 10);
				ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
 			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Rigid body", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt("velocity x", &velX);
				ImGui::InputInt("velocity y", &velY);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Projectile emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderAngle("angle (deg)", &projAngle, 0, 360);
				ImGui::SliderFloat("speed (px/sec)", &projSpeed, 10, 500);
				ImGui::InputInt("repeat (sec)", &projRepeat);
				ImGui::InputInt("duration (sec)", &projDuration);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderInt("%", &health, 0, 100);
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();


			if (ImGui::Button("Spawn new enemy")) {
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(m_posX, m_posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
				enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, 2);
				enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
				double projVelX = cos(projAngle) * projSpeed;
				double projVelY = sin(projAngle) * projSpeed;

				enemy.AddComponent<ProjectileEmitterComponent>(
					glm::vec2(projVelX, projVelY), projRepeat * 1000, projDuration * 1000, 10, false
				);
				enemy.AddComponent<HealthComponent>(health);
				
				// restore data
				//posX = posY = rotation = projAngle = 0;
				//scaleX = scaleY = 1;
				//projRepeat = projDuration = 10;
				//projSpeed = 100;
				//health = 100;
				
			}

		}
		ImGui::End();

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoNav;

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.3f);
		if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
			ImGui::Text("Map coordinates (x=%.1f, y=%.1f)", 
				ImGui::GetIO().MousePos.x + camera.x,
				ImGui::GetIO().MousePos.y + camera.y
				);
		}
		ImGui::End();


		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

	}

};