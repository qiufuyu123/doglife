#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <bits/pthreadtypes.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "event/key.h"

#include <sys/mman.h>
#include <pthread.h>

qui_keymap_t kmap;

uint32_t key_down = 1;


const int WIDTH = 800;
const int HEIGHT = 600;
int frameIndex = 0;

int main(int argc, char *argv[]) {
    kmap.key_down = (uint64_t)&key_down;
    qui_setup_keymap(kmap);
    pthread_t ui_task;
    SDL_Init(SDL_INIT_VIDEO);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Window* win = SDL_CreateWindow("Framebuffer test", SDL_WINDOWPOS_CENTERED | SDL_WINDOW_OPENGL, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    
    
    uint32_t* pixels = malloc(WIDTH * HEIGHT*sizeof(uint32_t)); // width x height x SDL_PIXELFORMAT_RGBA8888
    memset(pixels, 0xff, 4 * WIDTH * HEIGHT);
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, WIDTH, HEIGHT, 32, 4*WIDTH, SDL_PIXELFORMAT_RGBA32);
    SDL_SetSurfaceRLE(surface, SDL_RLEACCEL);
    //SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
    //SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    // Rect to draw on the screen

    qui_emu_arg_t args;
    args.height = HEIGHT;
    args.width = WIDTH;
    args.vaddr = pixels;
    SDL_Rect rt = { 0, 0, WIDTH, HEIGHT };
    //printf("t");
    pthread_create(&ui_task, NULL, qui_thread, &args);
    
    pthread_detach(ui_task);
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }else if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_DOWN)
                {
                    printf("key down!");
                    key_down = 0;
                }
            }
            else if(e.type == SDL_KEYUP){
                key_down = 1;
            }
        }
        //SDL_MUSTLOCK(S)
        // SDL_RenderClear(renderer);
        // SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        
        // //SDL_BlitSurface(surface,&rt,)
        // SDL_RenderCopy(renderer, tex, NULL, &rt);
        // SDL_DestroyTexture(tex);
        // SDL_RenderPresent(renderer);
        SDL_BlitSurface(surface,&rt,SDL_GetWindowSurface(win),&rt);
        SDL_UpdateWindowSurface(win);
        //SDL_RenderCopy()
    }

    SDL_FreeSurface(surface);
    //SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}