#ifndef GB_CORE_H
#define GB_CORE_H

// NOTE: Anything needed by C++ needs to be wrapped in extern "C" for the linker
// to resolve references.
//
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  // registers
  uint8_t a, f;
  uint8_t b, c;
  uint8_t d, e;
  uint8_t h, l;

  uint16_t sp;
  uint16_t pc;

  // memory map
  uint8_t mem[0x10000];

  // flags
  bool ime;
  bool halted;
  bool stopped;

} gb_core;

int gb_core_version(void);

void gb_init(gb_core *gb);
void gb_load_rom(gb_core *gb, const char *path);
void gb_step(gb_core *gb);
void gb_run_frame(gb_core *gb);

#ifdef __cplusplus
}
#endif
#endif
