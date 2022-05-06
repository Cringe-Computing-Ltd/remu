#include <SDL2/SDL.h>
#undef main
#include <stdio.h>
#include "raccoon_core.h"
#include <iostream>
#include <fstream>
#include "sdl_to_ps2.h"

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
    PS2Driver ps2;
    MemoryMapper mapper(&vga, &ps2);
    RaccoonCore cpu(&mapper);

    // Load ROM
    std::ifstream file("../../bas/output.bin", std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        fprintf(stderr, "Could not open ROM file");
        return -1;
    }
    int len = file.tellg();
    int wc = len / 2;
    file.seekg(0);
    uint16_t* rom = new uint16_t[wc];
    file.read((char*)rom, len);
    file.close();
    for (int i = 0; i < wc; i++) {
        mapper.write(i, rom[i]);
    }
    delete[] rom;

    printf("EXEC START\n");

    uint64_t lastTime = SDL_GetTicks();
    bool running = true;
    while (running) {
        // Calculate frame time
        uint64_t time = SDL_GetTicks();
        double delta = time - lastTime;
        lastTime = time;

        // Calculate number of CPU cycles to execute
        int cycles = round(delta * CPU_FREQ / 1000.0);

        // Check for any event
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EventType::SDL_KEYDOWN) {
                ps2.pressKey(SDL_TO_PS2[event.key.keysym.scancode]);
            }
            else if (event.type == SDL_EventType::SDL_KEYUP) {
                ps2.releaseKey(SDL_TO_PS2[event.key.keysym.scancode]);
            }
            else if (event.type == SDL_EventType::SDL_QUIT) {
                running = false;
            }
        }

        // Interrupt cycles
        cpu.interrupt(true);
        cpu.run(1000);
        cpu.interrupt(false);

        // Execute cycles
        cpu.run(cycles - 1000);

        // Render
        vga.render(fb);
        SDL_UpdateTexture(tex, NULL, fb, 640 * sizeof(uint32_t));
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