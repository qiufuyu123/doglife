#include "qfb.h"
#include "ui/widget.h"
#include <stdint.h>
#include <stdio.h>

void* qui_ctx_stack[QUI_STACK_MAX];
uint32_t qui_ctx_stack_top=0;
qui_framebuffer_t qui_renderer;
void qui_setup_renderer(void *fb, uint32_t width, uint32_t height)
{
    qui_renderer.pixels = fb;
    qui_renderer.height = height;
    qui_renderer.width = width;
}

void qui_push_ctx(void*ctx)
{
    if(qui_ctx_stack_top>=QUI_STACK_MAX)
        return;
    qui_ctx_stack[qui_ctx_stack_top]=ctx;
    qui_ctx_stack_top++;
}

void qui_pop_ctx()
{
    if(qui_ctx_stack_top>0)
        qui_ctx_stack_top--;
}

void* qui_top_ctx()
{
    if(!qui_ctx_stack_top)
        return 0;
    return qui_ctx_stack[qui_ctx_stack_top-1];
}

inline int qui_cvt_x(qui_widget_t*ctx, int x)
{
    int ox =x;
    x+=ctx->bound_rect.x;
    if(x<ctx->bound_rect.x || x>ctx->bound_rect.x2)
        return -1;
    if(ctx->parent)
    {
        while(ctx->parent)
            ctx=ctx->parent;
        if(x<ctx->bound_rect.x || x>ctx->bound_rect.x2)
        return -1;
    }
    return x;
}

inline int qui_cvt_y(qui_widget_t*ctx, int y)
{
    int oy=y;   
    y+=ctx->bound_rect.y;
    if(y<ctx->bound_rect.y || y>ctx->bound_rect.y2)
        return -1;
    if(ctx->parent)
    {
        while(ctx->parent)
            ctx=ctx->parent;
        if(y<ctx->bound_rect.y || y>ctx->bound_rect.y2)
            return -1;
    }
    return y;
}
#define CVT_X(v) qui_cvt_x(ctx,(v))
#define CVT_Y(v) qui_cvt_y(ctx,(v))
void qui_draw_line(int x1,int y1,int x2,int y2,uint32_t color)
{
    qui_widget_t*ctx =(qui_widget_t*)qui_top_ctx();
    if(!ctx)
        return;
    x1 = CVT_X(x1);
    x2 = CVT_X(x2);
    y1 = CVT_Y(y1);
    y2 = CVT_Y(y2);
    if(x1<0 || x2<0 || y1<0 || y2<0)
        return;
    if(x1 == x2)
    {
        // vertical line
        for (;y1<y2;y1++) {
            qui_renderer.pixels[y1*qui_renderer.width+x1] = color;
        }
    }
    else if(y1 == y2)
    {
        // horizontal line
        for (;x1<x2;x1++) {
            qui_renderer.pixels[y1*qui_renderer.width+x1] = color;
        }
    }
}

void qui_draw_rect(int x1, int y1, int w, int h,uint32_t color)
{
    qui_draw_line(0,0,w,0, color);
    qui_draw_line(0,h,w,h, color);
    qui_draw_line(0,0,0,h, color);
    qui_draw_line(w,0,w,h, color);
}

void qui_fill_rect(int x1, int y1, int w, int h,uint32_t color)
{
    int t = y1+h;
    for (;y1<t;y1++) {
        qui_draw_line(x1, y1, x1+w, y1, color);
    }
}
extern void fb_putchar(char ch, uint32_t x, uint32_t y, uint32_t color,uint32_t bg_color);


extern uint32_t CHAR_W,CHAR_H;

qui_point_t qui_draw_text(int x1, int y1, char*text, uint32_t color,uint32_t bg_color)
{
    qui_widget_t* ctx =qui_top_ctx();
    qui_point_t p;
    p.y = CHAR_H;
    while (*text) {
        int nx = CVT_X(x1);
        int ny = CVT_Y(y1);
        int nxx = CVT_X(x1+qui_font_width());
        int nyy = CVT_Y(y1+qui_font_height());
        if(nx>=0&&ny>=0&&nxx>=0&&nyy>=0)
        {
            //printf("draw@%d,%d;",nx,ny);
            fb_putchar(*text, nx, ny, color,bg_color);
        }
        text++;
        x1 += qui_font_width();
        p.x += CHAR_W;
    }
    return p;
}

uint32_t qui_font_width()
{
    return  CHAR_W;
}

uint32_t qui_font_height()
{
    return  CHAR_H;
}