#include "ui.h"
#include "qfb.h"
#include "klog.h"
#include "ui/label.h"
#include "ui/widget.h"
#include "event/key.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void qui_thread(qui_emu_arg_t *config)
{
    char *buf2 = malloc(config->width*config->height*4);
    memset(buf2, 0x00, config->width*config->height*4);
    qui_setup_renderer(buf2,config->width , config->height);
    qui_setup_klog();
    qui_logger->putstr("Hello world multi-thread!");
    qui_widget_t ctx,panel_1;
    qui_label_view_t lview;
    qui_create_widget(&ctx, 0, 50, 50, 500, 400, "title");
    qui_create_widget(&panel_1, QUI_WIDGET_NO_TITLE, 0, 0, 450, 300,"");
    //qui_begin_widget(&ctx);
    qui_create_labelview(&lview, &panel_1);
    qui_labelview_add(&lview, "line first", 0);
    qui_labelview_add(&lview, "line 2", 1);
    qui_labelview_add(&lview, "line 3", 2);
    //panel_1.focus = 1;
    ctx.focus = 1;
    panel_1.parent= &ctx;
    int idx = 0;
    while (1) {
        qui_begin_widget(&ctx);
        if(qui_getkey(QUI_KEY_OK))
        {
            ctx.focus = 0;
            panel_1.focus=1;
        }else if(qui_getkey(QUI_KEY_CANCEL))
        {
            ctx.focus = 1;
            panel_1.focus=0;
        }
        while (qui_getkey(QUI_KEY_OK)||qui_getkey(QUI_KEY_CANCEL)) {
        
        }
        qui_begin_vertical(0, 0);
        qui_widget_from_parent(&panel_1, (qui_point_t){0,0});
        qui_labelview_show(&lview);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        qui_widget_separate(380);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        
        qui_end_widget();
        //printf("%d",idx%2);
        idx++;
        memcpy(config->vaddr, buf2, config->width*config->height*4);
    }
    
}