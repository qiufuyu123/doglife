#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include <klog.h>
#include <qfb.h>

#include <sys/mman.h>

const int WIDTH = 800;
const int HEIGHT = 600;
int frameIndex = 0;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Framebuffer test", SDL_WINDOWPOS_CENTERED | SDL_WINDOW_OPENGL, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    
    
    uint32_t* pixels = malloc(WIDTH * HEIGHT*sizeof(uint32_t)); // width x height x SDL_PIXELFORMAT_RGBA8888
    memset(pixels, 0xff, 4 * WIDTH * HEIGHT);
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, WIDTH, HEIGHT, 32, 4*WIDTH, SDL_PIXELFORMAT_RGB24);
    //SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    // Rect to draw on the screen
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 10;
    rect.h = 10;
    qui_setup_renderer((void*)pixels,WIDTH , HEIGHT);
    qui_setup_klog();
    qui_logger->putstr("Hello world!");
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);

        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_RenderDrawRect(renderer, &rect);
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderDrawRect(renderer, &rect);
        // SDL_LockSurface(surface);
        // memset(pixels,128,4 * WIDTH * HEIGHT);
        // SDL_UnlockSurface(surface);
        //SDL_Delay(1000);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rt = { 0, 0, WIDTH, HEIGHT };
        SDL_RenderCopy(renderer, tex, NULL, &rt);
        SDL_DestroyTexture(tex);
        SDL_RenderPresent(renderer);
        //SDL_RenderCopy()
    }

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}