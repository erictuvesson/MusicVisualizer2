#pragma once

#include "imgui/imgui.h"

namespace ImGui {
	inline void Initialize() {
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::StyleColorsDark();
	}

	inline bool BeginSimple(const char* name, bool* p_open = NULL, int corner = 0, float distance = 0.0f, float alpha = 0.5f) {
		ImVec2 window_pos = ImVec2((corner & 1) ? 
			ImGui::GetIO().DisplaySize.x - distance : distance, 
			(corner & 2) ? ImGui::GetIO().DisplaySize.y - distance : distance);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);

		if (corner != -1)
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

		ImGui::SetNextWindowBgAlpha(alpha);
		return ImGui::Begin(name, p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
	}
}