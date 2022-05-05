#include "ps2_driver.h"
#include <string.h>

PS2Driver::PS2Driver() {
    buffer = new uint16_t[256];
    memset(buffer, 0, 256 * sizeof(uint16_t));
}

PS2Driver::~PS2Driver() {
    delete[] buffer;
}

void PS2Driver::pressKey(Key key) {
    uint16_t keycode = key;
    uint8_t ext = (keycode >> 8) & 0xFF;
    uint8_t code = keycode & 0xFF;
    if (ext) { writeByte(ext); }
    writeByte(code);
}

void PS2Driver::releaseKey(Key key) {
    uint16_t keycode = key;
    uint8_t ext = (keycode >> 8) & 0xFF;
    uint8_t code = keycode & 0xFF;
    if (ext) { writeByte(ext); }
    writeByte(0xF0);
    writeByte(code);
}

void PS2Driver::writeByte(uint8_t data) {
    buffer[pos++] = data;
    pos &= 0x7F;
    buffer[0xFF] = pos;
}