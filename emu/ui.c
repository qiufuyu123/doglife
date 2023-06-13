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
    memset(buf2, 0xff, config->width*config->height*4);
    qui_setup_renderer(buf2,config->width , config->height);
    qui_setup_klog();
    qui_logger->putstr("Hello world multi-thread!");
    qui_widget_t ctx;
    qui_create_widget(&ctx, 0, 50, 50, 500, 400, "title");

    //qui_begin_widget(&ctx);
    int idx = 0;
    while (1) {
        qui_begin_widget(&ctx);
        qui_begin_vertical(0, 0);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        qui_widget_separate(380);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        qui_end_widget();
        //printf("%d",idx%2);
        idx++;
        memcpy(config->vaddr, buf2, config->width*config->height*4);
    }
    
}