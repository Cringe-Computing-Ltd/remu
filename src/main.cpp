#include <SDL2/SDL.h>
#undef main
#include <stdio.h>
#include "raccoon_core.h"
#include <iostream>
#include <fstream>
#include "font.h"

#define CPU_FREQ    (25175000.0 / 4.0)

int main() {
    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("REMU: The R.A.C.C.O.O.N. Emulator", 100, 100, 640*2, 480*2, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture * tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    uint32_t* fb = new uint32_t[640 * 480];

    // Init Computer
    VGA_GEN vga;
    MemoryMapper mapper(&vga);
    RaccoonCore cpu(&mapper);

    // Load ROM
    std::ifstream file("../../bsm/output.bin", std::ios::in | std::ios::binary | std::ios::ate);
    int len = file.tellg();
    int wc = len / 2;
    file.seekg(0);
    uint16_t* rom = new uint16_t[wc];
    file.read((char*)rom, len);
    file.close();
    for (int i = 0; i < wc; i++) {
        mapper.write(i, rom[i]);
    }

    // Load font
    for (int i = 0; i < 128; i++) {
        mapper.write(0xF800 + (i * 4), (font8x8_basic[i][1] << 8) | font8x8_basic[i][0]);
        mapper.write(0xF800 + (i * 4) + 1, (font8x8_basic[i][3] << 8) | font8x8_basic[i][2]);
        mapper.write(0xF800 + (i * 4) + 2, (font8x8_basic[i][5] << 8) | font8x8_basic[i][4]);
        mapper.write(0xF800 + (i * 4) + 3, (font8x8_basic[i][7] << 8) | font8x8_basic[i][6]);
    }

    uint64_t lastTime = SDL_GetTicks();
    bool running = true;
    while (running) {
        // Calculate frame time
        uint64_t time = SDL_GetTicks();
        double delta = time - lastTime;
        lastTime = time;

        // Calculate number of CPU cycles to execute
        int cycles = round(delta * CPU_FREQ / 1000.0);

        // Execute cycles
        cpu.run(cycles);

        // Render
        vga.render(fb);
        SDL_UpdateTexture(tex, NULL, fb, 640 * sizeof(uint32_t));

        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EventType::SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}