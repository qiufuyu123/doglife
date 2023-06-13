#include "ui/widget.h"
#include "event/key.h"
#include "qfb.h"
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ui/label.h"
void qui_create_widget(qui_widget_t*ctx, uint16_t flag, int x, int y,int w,int h,char* title)
{
    //qui_widget_t *o = qui_alloc(sizeof(qui_widget_t));
    if(!ctx)
        return;
    ctx->parent = 0;
    ctx->flags = flag;
    ctx->x = x;
    ctx->y = y;
    ctx->w = w;
    ctx->h = h;
    ctx->title = title;
    if(ctx->title)
        ctx->title_len = strlen(title);
    ctx->scroller.x = ctx->scroller.y = 0;

    //return o;
}

void qui_widget_from_parent(qui_widget_t* ctx,qui_point_t delta)
{
    qui_widget_t *parent = qui_top_ctx();
    qui_point_t p = qui_widget_space(parent);
    ctx->x = delta.x+p.x+parent->bound_rect.x;
    
    ctx->y = delta.y+p.y+parent->bound_rect.y;
    qui_widget_separate(ctx->h);
}

void qui_begin_widget(qui_widget_t *ctx)
{
    qui_push_ctx(ctx);
    ctx->bound_rect.x = ctx->x;
    ctx->bound_rect.y = ctx->y;
    ctx->bound_rect.x2 = ctx->x + ctx->w;
    ctx->bound_rect.y2 = ctx->y + ctx->h;
    qui_fill_rect(0, 0, ctx->w, ctx->h, 0xff1c1c1c);
    qui_fill_rect(5, 5, ctx->w-10, ctx->h-10, 0xff9c9c9c);
    if(!(ctx->flags & QUI_WIDGET_NO_TITLE))
    {
        if(ctx->title)
        {
            qui_fill_rect(0, 0, ctx->w, qui_font_height()+10, 0xff1c1c1c);
            qui_draw_text(20, 5, ctx->title,0xffffffff,0xff1c1c1c);
            // ctx->y += qui_font_height()+10+3;
            // ctx->x += 3;
            ctx->bound_rect.y += qui_font_height()+10;
            ctx->bound_rect.x+=5;
        }
    }
    //scroller:
    if(ctx->focus)
    {
        if(qui_getkey(QUI_KEY_DOWN))
        {
            ctx->scroller.y-=qui_font_height();
            //qui_draw_text(20, 5, "press",0xffffffff,0xff1c1c1c);
        }else if(qui_getkey(QUI_KEY_UP))
        {
            if(ctx->scroller.y<0)
                ctx->scroller.y+=qui_font_height();
        }
        while (qui_getkey(QUI_KEY_DOWN)||qui_getkey(QUI_KEY_UP)) {
        
        }
    }

    
}

void qui_end_widget()
{
    qui_widget_t *ctx =qui_top_ctx();
    int hh =ctx->bound_rect.y2-ctx->bound_rect.y;
    qui_fill_rect(ctx->w-30,0, 23,hh, 0xff505050);
    int unit = hh*qui_font_height()/ctx->w;
    qui_fill_rect(ctx->w-30,-unit*ctx->scroller.y/qui_font_height(), 23,unit, 0xffe0e0e0);
    qui_pop_ctx();
}

qui_point_t qui_point_comb(qui_point_t o, qui_point_t p)
{
    o.x += p.x;
    o.y += p.y;
    return o;
}

void qui_begin_vertical( int x, int y)
{
    qui_widget_t *ctx = qui_top_ctx();
    ctx->area.x = x + 3 + ctx->scroller.x;
    ctx->area.y = y + ctx->scroller.y;
    ctx->area.is_vert = true; 
    ctx->area.now = ctx->area.y;
    //printf("area@%d,%d;",ctx->area.x,ctx->area.y);
    
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