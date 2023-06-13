#include "ui/label.h"
#include "qfb.h"
#include "ui/widget.h"
#include <stdio.h>
#include <string.h>

void qui_static_label(qui_point_t p, char *text, uint32_t color)
{
    qui_widget_t *ctx =qui_top_ctx();
    qui_point_t o = qui_widget_space(ctx);
    o = qui_point_comb(o, p);
    qui_point_t r = qui_draw_text(o.x, o.y, text, color,0x00000000);
    //printf("label@%d,%d;",o.x,o.y);
    qui_widget_spaceend(ctx, r.x, r.y+3);
}

void qui_create_labelview(qui_label_view_t *view, qui_widget_t *panel)
{
    view->panel = panel;
    memset(view->lines, 0, sizeof(char*)*QUI_LABELVIEW_MAX_LINE);
}

void qui_labelview_add(qui_label_view_t *view, char *text, int line)
{
    if(line >= QUI_LABELVIEW_MAX_LINE)
        return;
    view->lines[line]=text;
}

void qui_labelview_show(qui_label_view_t *view)
{
    qui_begin_widget(view->panel);
    qui_begin_vertical(0, 0);
    for (int i=0; i<QUI_LABELVIEW_MAX_LINE; i++) {
        char *str = view->lines[i];
        if(str)
        {
            qui_static_label((qui_point_t){0,0}, str, 0xff000000);
        }
    }
    
    qui_end_widget();
}