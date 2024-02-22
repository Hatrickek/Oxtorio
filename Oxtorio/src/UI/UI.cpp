#include "UI.h"

#include <UI/OxUI.h>
#include <Render/Window.h>
#include <Scene/Scene.h>

#include <UI/RuntimeConsole.h>
#include <Utils/ImGuiScoped.h>


namespace Oxtorio {
void UI::draw_fps_counter() {
  constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                                            ImGuiWindowFlags_NoMove;
  constexpr float PAD = 10.0f;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImVec2 work_pos = viewport->WorkPos;
  ImVec2 window_pos;
  window_pos.x = (work_pos.x + PAD);
  window_pos.y = work_pos.y + PAD;
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.35f);
  if (ImGui::Begin("Performance Overlay", nullptr, window_flags)) {
    ImGui::Text("%.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
  }
}

void UI::draw_runtime_console(Ox::RuntimeConsole& console, bool* block_player_input) {
  if (console.visible) {
    console.on_imgui_render();
    *block_player_input = true;
  }
  else {
    *block_player_input = false;
  }
}
}
