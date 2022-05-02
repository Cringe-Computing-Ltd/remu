#pragma once
#include <stdint.h>

class VGA_GEN {
public:
    VGA_GEN();
    ~VGA_GEN();

    void render(uint32_t* rgba);

    uint16_t* vram;
    uint16_t* cram;

private:
    const uint32_t PALLET[8] = {
        0xFF000000,
        0xFFFF0000,
        0xFF00FF00,
        0xFFFFFF00,
        0xFF0000FF,
        0xFFFF00FF,
        0xFF00FFFF,
        0xFFFFFFFF
    };

};