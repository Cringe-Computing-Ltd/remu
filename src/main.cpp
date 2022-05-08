#include <SDL2/SDL.h>
#undef main
#include <stdio.h>
#include "raccoon_core.h"
#include <iostream>
#include <fstream>
#include "sdl_to_ps2.h"
#include "command_args.h"

#define CPU_FREQ    (25175000.0 / 4)

int main(int argc, char* argv[]) {
    // Parse arguments
    CommandArgsParser cargs;
    cargs.define('r', "rom", "Rom binary file", "");
    cargs.define<int>('w', "width", "Viewport width", 640*2);
    cargs.define<int>('h', "height", "Viewport height", 480*2);
    cargs.parse(argc, argv);

    // Check that a rom was given
    if (cargs["rom"].s().empty()) {
        fprintf(stderr, "No rom file given!\n");
        return -1;
    }

    // Init Computer
    VGA_GEN vga;
    PS2Driver ps2;
    MemoryMapper mapper(&vga, &ps2);
    RaccoonCore cpu(&mapper);

    // Load ROM
    std::ifstream file(cargs["rom"].s(), std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        fprintf(stderr, "Could not open ROM file\n");
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

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("REMU: The R.A.C.C.O.O.N. Emulator", 100, 100, cargs["width"].i(), cargs["height"].i(), SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture * tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
    uint32_t* fb = new uint32_t[640 * 480];

    // Mais loop
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