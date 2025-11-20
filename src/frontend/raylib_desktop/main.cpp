#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

int main() {
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
        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
