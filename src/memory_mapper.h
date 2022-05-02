#pragma once
#include "vga_gen.h"

class MemoryMapper {
public:
    MemoryMapper(VGA_GEN* vga);
    ~MemoryMapper();
    uint16_t read(uint16_t addr);
    void write(uint16_t addr, uint16_t data);
    
private:
    VGA_GEN* _vga;
    uint16_t* ram;

};