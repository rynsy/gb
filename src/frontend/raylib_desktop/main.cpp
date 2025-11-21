#include "gb/gb_core.h"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include <cstdint>

static void DrawGbState(const gb_core &gb) {

  // Get readable size
  char sizes[] = {'\0', 'K', 'M', 'G', 'T', 'P'};
  uint32_t size = gb.cart.rom_size;
  uint8_t s_ptr = 0;

  while ((size / 1024) > 0) {
    size = size / 1024;
    s_ptr++;
  }

  ImGui::Begin("Emulator State");
  ImGui::Text("Core @ %p", (const void *)&gb);
  ImGui::Separator();
  ImGui::Text("Cartridge");
  ImGui::Text("Cartridge Ptr: %p", gb.cart.rom);
  ImGui::Text("Cartridge Size: %d %cB", size, sizes[s_ptr]);
  ImGui::Separator();
  ImGui::Text("CPU Pointers");
  ImGui::Text("SP: %d", gb.sp);
  ImGui::Text("PC: %d", gb.pc);
  ImGui::Separator();
  ImGui::Text("RAM Pointers");
  ImGui::Text("HRAM: %p", gb.hram);
  ImGui::Text("WRAM: %p", gb.wram);
  ImGui::Separator();
  ImGui::Text("Registers");
  ImGui::Text("Register A: %d", gb.a);
  ImGui::Text("Register B: %d", gb.b);
  ImGui::Text("Register C: %d", gb.c);
  ImGui::Text("Register D: %d", gb.d);
  ImGui::Text("Register E: %d", gb.e);
  ImGui::Text("Register F: %d", gb.f);
  ImGui::Text("Register H: %d", gb.h);
  ImGui::Text("Register L: %d", gb.l);
  ImGui::Separator();
  ImGui::Text("Flags");
  ImGui::Text("IME: %s", gb.ime ? "true" : "false");
  ImGui::Text("Halted: %s", gb.halted ? "true" : "false");
  ImGui::Text("Stopped: %s", gb.stopped ? "true" : "false");
  ImGui::End();
}

int main() {

  gb_core gb;
  gb_init(&gb);

  gb_load_rom(&gb, "external/gb-test-roms/cpu_instrs/individual/01-special.gb");

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
