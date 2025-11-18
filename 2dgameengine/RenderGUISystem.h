#pragma once
#include "ECS.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SDL.h"

class RenderGUISystem : public System {
public:
	RenderGUISystem() = default;

	void Update(SDL_Renderer* renderer) {
		// draw debug menu
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		
		//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
		//	ImGuiWindowFlags_NoNav |
		//	ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Spawn Enemies")) {


		}
		ImGui::End();



		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

	}

};