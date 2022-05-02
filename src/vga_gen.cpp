#include "vga_gen.h"

VGA_GEN::VGA_GEN() {
    vram = new uint16_t[4800];
    cram = new uint16_t[1024];
}

VGA_GEN::~VGA_GEN() {
    delete[] vram;
    delete[] cram;
}

void VGA_GEN::render(uint32_t* rgba) {
    for (int cy = 0; cy < 60; cy++) {
        for (int cx = 0; cx < 80; cx++) {
            // Get screen pos
            int x = cx * 8;
            int y = cy * 8;

            // Get character and decode color
            uint16_t ch = vram[(cy * 80) + cx];
            uint16_t* charpx = &cram[(ch & 0xFF) * 4];
            uint32_t fg = PALLET[(ch >> 8) & 0b111];
            uint32_t bg = PALLET[(ch >> 12) & 0b111];

            // Draw
            int line0 = (y * 640) + x;
            int line1 = ((y+1) * 640) + x;
            int line2 = ((y+2) * 640) + x;
            int line3 = ((y+3) * 640) + x;
            int line4 = ((y+4) * 640) + x;
            int line5 = ((y+5) * 640) + x;
            int line6 = ((y+6) * 640) + x;
            int line7 = ((y+7) * 640) + x;

            uint8_t pix0 = charpx[0];
            uint8_t pix1 = charpx[0] >> 8;
            uint8_t pix2 = charpx[1];
            uint8_t pix3 = charpx[1] >> 8;
            uint8_t pix4 = charpx[2];
            uint8_t pix5 = charpx[2] >> 8;
            uint8_t pix6 = charpx[3];
            uint8_t pix7 = charpx[3] >> 8;

            rgba[line0] = ((pix0 >> 0) & 1) ? fg : bg;
            rgba[line0 + 1] = ((pix0 >> 1) & 1) ? fg : bg;
            rgba[line0 + 2] = ((pix0 >> 2) & 1) ? fg : bg;
            rgba[line0 + 3] = ((pix0 >> 3) & 1) ? fg : bg;
            rgba[line0 + 4] = ((pix0 >> 4) & 1) ? fg : bg;
            rgba[line0 + 5] = ((pix0 >> 5) & 1) ? fg : bg;
            rgba[line0 + 6] = ((pix0 >> 6) & 1) ? fg : bg;
            rgba[line0 + 7] = ((pix0 >> 7) & 1) ? fg : bg;

            rgba[line1] = ((pix1 >> 0) & 1) ? fg : bg;
            rgba[line1 + 1] = ((pix1 >> 1) & 1) ? fg : bg;
            rgba[line1 + 2] = ((pix1 >> 2) & 1) ? fg : bg;
            rgba[line1 + 3] = ((pix1 >> 3) & 1) ? fg : bg;
            rgba[line1 + 4] = ((pix1 >> 4) & 1) ? fg : bg;
            rgba[line1 + 5] = ((pix1 >> 5) & 1) ? fg : bg;
            rgba[line1 + 6] = ((pix1 >> 6) & 1) ? fg : bg;
            rgba[line1 + 7] = ((pix1 >> 7) & 1) ? fg : bg;

            rgba[line2] = ((pix2 >> 0) & 1) ? fg : bg;
            rgba[line2 + 1] = ((pix2 >> 1) & 1) ? fg : bg;
            rgba[line2 + 2] = ((pix2 >> 2) & 1) ? fg : bg;
            rgba[line2 + 3] = ((pix2 >> 3) & 1) ? fg : bg;
            rgba[line2 + 4] = ((pix2 >> 4) & 1) ? fg : bg;
            rgba[line2 + 5] = ((pix2 >> 5) & 1) ? fg : bg;
            rgba[line2 + 6] = ((pix2 >> 6) & 1) ? fg : bg;
            rgba[line2 + 7] = ((pix2 >> 7) & 1) ? fg : bg;

            rgba[line3] = ((pix3 >> 0) & 1) ? fg : bg;
            rgba[line3 + 1] = ((pix3 >> 1) & 1) ? fg : bg;
            rgba[line3 + 2] = ((pix3 >> 2) & 1) ? fg : bg;
            rgba[line3 + 3] = ((pix3 >> 3) & 1) ? fg : bg;
            rgba[line3 + 4] = ((pix3 >> 4) & 1) ? fg : bg;
            rgba[line3 + 5] = ((pix3 >> 5) & 1) ? fg : bg;
            rgba[line3 + 6] = ((pix3 >> 6) & 1) ? fg : bg;
            rgba[line3 + 7] = ((pix3 >> 7) & 1) ? fg : bg;

            rgba[line4] = ((pix4 >> 0) & 1) ? fg : bg;
            rgba[line4 + 1] = ((pix4 >> 1) & 1) ? fg : bg;
            rgba[line4 + 2] = ((pix4 >> 2) & 1) ? fg : bg;
            rgba[line4 + 3] = ((pix4 >> 3) & 1) ? fg : bg;
            rgba[line4 + 4] = ((pix4 >> 4) & 1) ? fg : bg;
            rgba[line4 + 5] = ((pix4 >> 5) & 1) ? fg : bg;
            rgba[line4 + 6] = ((pix4 >> 6) & 1) ? fg : bg;
            rgba[line4 + 7] = ((pix4 >> 7) & 1) ? fg : bg;

            rgba[line5] = ((pix5 >> 0) & 1) ? fg : bg;
            rgba[line5 + 1] = ((pix5 >> 1) & 1) ? fg : bg;
            rgba[line5 + 2] = ((pix5 >> 2) & 1) ? fg : bg;
            rgba[line5 + 3] = ((pix5 >> 3) & 1) ? fg : bg;
            rgba[line5 + 4] = ((pix5 >> 4) & 1) ? fg : bg;
            rgba[line5 + 5] = ((pix5 >> 5) & 1) ? fg : bg;
            rgba[line5 + 6] = ((pix5 >> 6) & 1) ? fg : bg;
            rgba[line5 + 7] = ((pix5 >> 7) & 1) ? fg : bg;

            rgba[line6] = ((pix6 >> 0) & 1) ? fg : bg;
            rgba[line6 + 1] = ((pix6 >> 1) & 1) ? fg : bg;
            rgba[line6 + 2] = ((pix6 >> 2) & 1) ? fg : bg;
            rgba[line6 + 3] = ((pix6 >> 3) & 1) ? fg : bg;
            rgba[line6 + 4] = ((pix6 >> 4) & 1) ? fg : bg;
            rgba[line6 + 5] = ((pix6 >> 5) & 1) ? fg : bg;
            rgba[line6 + 6] = ((pix6 >> 6) & 1) ? fg : bg;
            rgba[line6 + 7] = ((pix6 >> 7) & 1) ? fg : bg;

            rgba[line7] = ((pix7 >> 0) & 1) ? fg : bg;
            rgba[line7 + 1] = ((pix7 >> 1) & 1) ? fg : bg;
            rgba[line7 + 2] = ((pix7 >> 2) & 1) ? fg : bg;
            rgba[line7 + 3] = ((pix7 >> 3) & 1) ? fg : bg;
            rgba[line7 + 4] = ((pix7 >> 4) & 1) ? fg : bg;
            rgba[line7 + 5] = ((pix7 >> 5) & 1) ? fg : bg;
            rgba[line7 + 6] = ((pix7 >> 6) & 1) ? fg : bg;
            rgba[line7 + 7] = ((pix7 >> 7) & 1) ? fg : bg;
        }
    }
}