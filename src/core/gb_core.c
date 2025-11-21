#include "gb/gb_core.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gb_core_version(void) { return 1; }

void gb_init(gb_core *gb) {
  memset(gb, 0, sizeof(*gb));
  gb->halted = true;
  gb->stopped = true;
}

void gb_load_rom(gb_core *gb, const char *path) {
  printf("Loading %s into gb struct\n", path);

  uint32_t rom_size;

  uint8_t *rom = read_binary_file(path, &rom_size);

  if (!rom) {
    printf("Failed to load ROM\n");
    return;
  }

  gb->cart.rom = malloc(rom_size);
  gb->cart.rom_size = rom_size;
  memcpy(gb->cart.rom, rom, rom_size);
  free(rom);
}

uint8_t *read_binary_file(const char *path, uint32_t *out_size) {

  FILE *f = fopen(path, "rb");
  if (!f) {
    printf("Couldn't open file \n");
    return NULL;
  }

  // jump to the end, get the position (file length), then move file pointer to
  // beginning of file
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  if (size <= 0) {
    fclose(f);
    return NULL;
  }

  uint8_t *buf = malloc(size);
  if (!buf) {
    printf("Couldn't allocate space for ROM data \n");
    fclose(f);
    return NULL;
  }

  uint32_t read = fread(buf, 1, size, f);
  fclose(f);

  if (read != (uint32_t)size) {
    printf("Error reading ROM data \n");
    free(buf);
    return NULL;
  }

  if (out_size) {
    printf("Cartridge size: %lu\n", size);
    *out_size = (uint32_t)size;
  }

  return buf;
}

void gb_step(gb_core *gb) { printf("Taking a step....\n"); }
void gb_run_frame(gb_core *gb) { printf("Running a frame....\n"); }
