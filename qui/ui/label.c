#include "ui/label.h"
#include "qfb.h"
#include "ui/widget.h"
#include <stdio.h>

void qui_static_label(qui_point_t p, char *text, uint32_t color)
{
    qui_widget_t *ctx =qui_top_ctx();
    qui_point_t o = qui_widget_space(ctx);
    o = qui_point_comb(o, p);
    qui_point_t r = qui_draw_text(o.x, o.y, text, color,0x00000000);
    //printf("label@%d,%d;",o.x,o.y);
    qui_widget_spaceend(ctx, r.x, r.y+3);
}