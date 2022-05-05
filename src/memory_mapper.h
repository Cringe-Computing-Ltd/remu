#pragma once
#include "vga_gen.h"
#include "ps2_driver.h"

class MemoryMapper {
public:
    MemoryMapper(VGA_GEN* vga, PS2Driver* ps2);
    ~MemoryMapper();
    uint16_t read(uint16_t addr);
    void write(uint16_t addr, uint16_t data);
    
private:
    VGA_GEN* _vga;
    PS2Driver* _ps2;
    uint16_t* ram;

};