#include "gb/gb_core.h"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

static void DrawGbState(const gb_core &gb) {
  ImGui::Begin("Emulator State");
  ImGui::Text("Core @ %p", (const void *)&gb);
  ImGui::Separator();
  ImGui::Text("Halted: %s", gb.halted ? "true" : "false");
  ImGui::Text("Stopped: %s", gb.stopped ? "true" : "false");
  ImGui::End();
}

int main() {

  gb_core gb;
  gb_init(&gb);

  InitWindow(800, 600, "gbemu scaffolding test");
  SetTargetFPS(60);

  rlImGuiSetup(true);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    rlImGuiBegin();

    ImGui::Begin("Debug");
    ImGui::Text("Raylib OK");
    ImGui::Text("ImGui OK");
    ImGui::End();

    DrawGbState(gb);

    rlImGuiEnd();

    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();
  return 0;
}
