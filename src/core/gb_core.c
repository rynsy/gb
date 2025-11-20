#include "gb/gb_core.h"
#include <stdio.h>
#include <string.h>

int gb_core_version(void) { return 1; }

void gb_init(gb_core *gb) {
  memset(gb, 0, sizeof(*gb));
  gb->halted = true;
  gb->stopped = true;
}

void gb_load_rom(gb_core *gb, const char *path) {
  printf("Loading %s into gb struct\n", path);
}

void gb_step(gb_core *gb) { printf("Taking a step....\n"); }
void gb_run_frame(gb_core *gb) { printf("Running a frame....\n"); }
