#include "ui/widget.h"
#include "qfb.h"
#include "config.h"
#include <stdbool.h>
#include <string.h>
#include <time.h>
void qui_create_widget(qui_widget_t*ctx, void *parent, int x, int y,int w,int h,char* title)
{
    //qui_widget_t *o = qui_alloc(sizeof(qui_widget_t));
    if(!ctx)
        return;
    ctx->parent = parent;
    ctx->x = x;
    ctx->y = y;
    ctx->w = w;
    ctx->h = h;
    ctx->title = title;
    if(ctx->title)
        ctx->title_len = strlen(title);
    //return o;
}

void qui_begin_widget(qui_widget_t *ctx)
{
    qui_push_ctx(ctx);
    qui_fill_rect(0, 0, ctx->w, ctx->h, 0x1c1c1c);
    qui_fill_rect(5, 5, ctx->w-10, ctx->h-10, 0xff9c9c9c);
    if(ctx->title)
    {
        qui_fill_rect(0, 0, ctx->w, qui_font_height()+10, 0x1c1c1c);
        qui_draw_text(20, 5, ctx->title,0xffffffff,0);
        // ctx->y += qui_font_height()+10+3;
        // ctx->x += 3;
    }
}

void qui_end_widget()
{
    qui_pop_ctx();
}

void qui_begin_vertical( int x, int y)
{
    qui_widget_t *ctx = qui_top_ctx();
    ctx->area.x = x + 3;
    ctx->area.y = y + qui_font_height()+10;
    ctx->area.is_vert = true; 
    ctx->area.now = ctx->area.y;
}

static bool qui_inarea(qui_widget_area_t area,qui_point_t xy,qui_point_t wh)
{
    return xy.x>=area.x && xy.y >= area.y && wh.x<=area.w && wh.y<=area.y;
}

qui_point_t qui_widget_space(qui_widget_t *ctx)
{
    qui_point_t r;
    if(ctx->area.is_vert)
    {
        r = (qui_point_t){ctx->area.x, ctx->area.now};
    }else {
        r = (qui_point_t){ctx->area.now, ctx->area.y};
    }
    return r;;
}

qui_point_t qui_point_comb(qui_point_t o, qui_point_t p)
{
    o.x += p.x;
    o.y += p.y;
    return o;
}

void qui_widget_separate(int delta)
{
    qui_widget_t *ctx = qui_top_ctx();
    ctx->area.now += delta;
}

void qui_widget_spaceend(qui_widget_t *ctx, int w, int h)
{
    if(ctx->area.is_vert)
        ctx->area.now += h;
    else
        ctx->area.now += w;
}