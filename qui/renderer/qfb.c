#include "qfb.h"
qui_framebuffer_t qui_renderer;
void qui_setup_renderer(void *fb, uint32_t width, uint32_t height)
{
    qui_renderer.pixels = fb;
    qui_renderer.height = height;
    qui_renderer.width = width;
}