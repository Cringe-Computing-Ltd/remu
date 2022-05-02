#include "memory_mapper.h"
#include <stdlib.h>
#include <stdio.h>

MemoryMapper::MemoryMapper(VGA_GEN* vga) {
    _vga = vga;
    ram = new uint16_t[0x10000];
}

MemoryMapper::~MemoryMapper() {
    delete[] ram;
}

uint16_t MemoryMapper::read(uint16_t addr) {
    if ((addr >> 11) >= 0b11100 && (addr >> 11) < 0b11111) {
        fprintf(stderr, "WARNING: DO NOT READ FROM VRAM!!!\n");
        return rand();
    }
    else if ((addr >> 11) == 0b11111) {
        fprintf(stderr, "WARNING: DO NOT READ FROM CRAM!!!\n");
        return rand();
    }
    else {
        return ram[addr];
    }
}

void MemoryMapper::write(uint16_t addr, uint16_t data) {
    if ((addr >> 11) >= 0b11100 && (addr >> 11) < 0b11111) {
        _vga->vram[(addr - 0xE000) % 4800] = data;
    }
    else if ((addr >> 11) == 0b11111) {
        _vga->cram[(addr - 0xF800) % 1024] = data;
    }
    else {
        ram[addr] = data;
    }
}