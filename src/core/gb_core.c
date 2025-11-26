#include "gb/gb_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions */

// Core functions
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

  //  0x0147 in the ROM contains the MBC type of the ROM, TODO: double-check,
  //  set MBC accordingly
  //  gb->cart.mbc_type = gb->cart.rom[0x0147];
  gb->cart.mbc_type = 0;
}

//
//  Fetch opcode at PC
//  Increment PC
//  Decode/Execute Instruction
//  return cycles consumed for instruction
//
void gb_step(gb_core *gb) { printf("Taking a step....\n"); }

//
//  Accumulate cycles until you've executed one frames worth
//  ~ 70224 CPU Cycles per frame (~4.19MHz/59.7Hz)
//  Step CPU
//  Step PPU with cycle const
//  Step timers, handle interrupts
//
void gb_run_frame(gb_core *gb) { printf("Running a frame....\n"); }

// Memory Bus
uint8_t gb_read8(gb_core *gb, uint16_t addr) {

  if (addr <= 0x7FFF && addr < gb->cart.rom_size) {
    return gb->cart.rom[addr];
  } else if (0xC000 <= addr && addr <= 0xDFFF) {
    // read WRAM
    return gb->wram[addr - 0xC000];
  } else if (0xE000 <= addr && addr <= 0xFDFF) {
    // echo of 0xC000-0xDDFF
    return gb->wram[addr - 0x2000];
  } else if (0xFF80 <= addr && addr <= 0xFFFE) {
    // read HRAM
    return gb->hram[addr - 0xFF80];
  } else if (0xFFFF == addr) {
    return gb->ie;
  } else {
    // Invalid memory access
    return 0xFF;
  }
}
bool gb_write8(gb_core *gb, uint16_t addr, uint8_t value) {
  bool success = false;
  if (0xC000 <= addr && addr <= 0xDFFF) {
    // read WRAM
    gb->wram[addr - 0xC000] = value;
    success = true;
  } else if (0xE000 <= addr && addr <= 0xFDFF) {
    // echo of 0xC000-0xDDFF
    gb->wram[addr - 0x2000] = value;
    success = true;
  } else if (0xFF80 <= addr && addr <= 0xFFFE) {
    // read HRAM
    gb->hram[addr - 0xFF80] = value;
    success = true;
  } else if (0xFFFF == addr) {
    gb->ie = value;
    success = true;
  }
  return success;
}

// 16-bit Register views
uint16_t get_AF(gb_core *gb) { return (gb->a) << 8 | (gb->f & 0xF0); }
uint16_t get_BC(gb_core *gb) { return (gb->b) << 8 | gb->c; }
uint16_t get_DE(gb_core *gb) { return (gb->d) << 8 | gb->e; }
uint16_t get_HL(gb_core *gb) { return (gb->h) << 8 | gb->l; }

// 16-bit Register setters
void set_AF(gb_core *gb, uint16_t value) {
  gb->a = (uint8_t)((value >> 8) & 0xFF);
  gb->f = (uint8_t)value & 0xF0; // Lower nibble of F must be 0
}
void set_BC(gb_core *gb, uint16_t value) {
  gb->b = (uint8_t)((value >> 8) & 0xFF);
  gb->c = (uint8_t)value & 0xFF;
}
void set_DE(gb_core *gb, uint16_t value) {
  gb->d = (uint8_t)((value >> 8) & 0xFF);
  gb->e = (uint8_t)value & 0xFF;
}
void set_HL(gb_core *gb, uint16_t value) {
  gb->h = (uint8_t)((value >> 8) & 0xFF);
  gb->l = (uint8_t)value & 0xFF;
}

// 1-bit Register views
// Upper 4 bits of F are used for flags, lower 4 always 0
// Zero Flag
bool get_Z(gb_core *gb) { return (gb->f & (1 << 7)) != 0; }
// Subtraction Flag
bool get_N(gb_core *gb) { return (gb->f & (1 << 6)) != 0; }
// Half-Carry Flag
bool get_H(gb_core *gb) { return (gb->f & (1 << 5)) != 0; }
// Carry Flag
bool get_C(gb_core *gb) { return (gb->f & (1 << 4)) != 0; }

// 1-bit Register setters
// Zero Flag
void set_Z(gb_core *gb, bool value) {
  if (value) {
    gb->f |= (1 << 7);
  } else {
    gb->f &= ~(1 << 7);
  }
  gb->f &= 0xF0;
}
// Subtraction Flag
void set_N(gb_core *gb, bool value) {
  if (value) {
    gb->f |= (1 << 6);
  } else {
    gb->f &= ~(1 << 6);
  }
  gb->f &= 0xF0;
}
// Half-Carry Flag
void set_H(gb_core *gb, bool value) {
  if (value) {
    gb->f |= (1 << 5);
  } else {
    gb->f &= ~(1 << 5);
  }
  gb->f &= 0xF0;
}
// Carry Flag
void set_C(gb_core *gb, bool value) {
  if (value) {
    gb->f |= (1 << 4);
  } else {
    gb->f &= ~(1 << 4);
  }
  gb->f &= 0xF0;
}

// Helpers
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
