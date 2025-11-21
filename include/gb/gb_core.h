#ifndef GB_CORE_H
#define GB_CORE_H

// NOTE: Anything needed by C++ needs to be wrapped in extern "C" for the linker
// to resolve references.
//
#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include <stdbool.h>
#include <stdint.h>

// Constants
#define MAX_ROM_SIZE 8 * 1024 * 1024 // 8 MB
#define WORK_RAM_SIZE_BYTES 8 * 1024 // 8 KB
#define HIGH_RAM_SIZE_BYTES 127      // 127 B

// Types

typedef struct {
  uint8_t *rom;
  uint32_t rom_size;
  int mbc_type;

} gb_cart;

/* Gameboy Memory Map:
 *
 * 0000–3FFF  ROM Bank 0
 * 4000–7FFF  ROM Bank N
 * 8000–9FFF  VRAM
 * A000–BFFF  External Cartridge RAM
 * C000–CFFF  Internal WRAM (Bank 0)
 * D000–DFFF  Internal WRAM (Bank 1 on CGB, same as bank 0 on DMG)
 * E000–FDFF  Echo RAM (C000–DDFF mirror)
 * FE00–FE9F  OAM (sprite attribute table)
 * FEA0–FEFF  Unusable
 * FF00–FF7F  I/O registers
 * FF80–FFFE  **HRAM**
 * FFFF        IE register
 * */

typedef struct {
  // Cartridge
  gb_cart cart;

  // registers
  uint8_t a, f;
  uint8_t b, c;
  uint8_t d, e;
  uint8_t h, l;

  uint16_t sp;
  uint16_t pc;

  // memory maps
  uint8_t wram[WORK_RAM_SIZE_BYTES];
  uint8_t hram[HIGH_RAM_SIZE_BYTES];

  // flags
  bool ime;
  bool halted;
  bool stopped;

} gb_core;

// Functions

int gb_core_version(void);
void gb_init(gb_core *gb);
void gb_load_rom(gb_core *gb, const char *path);
void gb_step(gb_core *gb);
void gb_run_frame(gb_core *gb);

// Helpers
uint8_t *read_binary_file(const char *path, uint32_t *out_size);

#ifdef __cplusplus
}
#endif
#endif
