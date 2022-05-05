#include "memory_mapper.h"
#include <stdlib.h>
#include <stdio.h>

MemoryMapper::MemoryMapper(VGA_GEN* vga, PS2Driver* ps2) {
    _vga = vga;
    _ps2 = ps2;
    ram = new uint16_t[0x10000];
}

MemoryMapper::~MemoryMapper() {
    delete[] ram;
}

uint16_t MemoryMapper::read(uint16_t addr) {
    if ((addr >> 8) == 0xDF) {
        return _ps2->buffer[addr - 0xDF00];
    }
    else if ((addr >> 8) >= 0xE0 && (addr >> 8) < 0xF8) {
        fprintf(stderr, "WARNING: DO NOT READ FROM VRAM!!!\n");
        return rand();
    }
    else if ((addr >> 8) >= 0xF8 && (addr >> 8) < 0xFC) {
        fprintf(stderr, "WARNING: DO NOT READ FROM CRAM!!!\n");
        return rand();
    }
    else {
        return ram[addr];
    }
}

void MemoryMapper::write(uint16_t addr, uint16_t data) {
    if ((addr >> 8) >= 0xE0 && (addr >> 8) < 0xF8) {
        _vga->vram[(addr - 0xE000) % 4800] = data;
    }
    else if ((addr >> 8) >= 0xF8 && (addr >> 8) < 0xFC) {
        _vga->cram[(addr - 0xF800) % 1024] = data;
    }
    else {
        ram[addr] = data;
    }
}