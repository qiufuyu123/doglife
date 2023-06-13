#include "fb/img_arr.h"
#include <stdint.h>
#include <mem/mmu.h>
#include <pboot.h>
#include <string.h>

#include"klog.h"
#include "qfb.h"
#include "ui/widget.h"
#include "ui/label.h"
#include "event/key.h"
pboot_config_t conf;
uint64_t get_ticks(void)
{
    //return __builtin_arm_rsr64("cntpct_el0");
}
void kernel_main()
{
    
    conf = *(pboot_config_t*)(0x300000000ULL);
    uint32_t fbsz=conf.fb_width*conf.fb_height*4;
    uint32_t *fb = (uint32_t*)(conf.fb_vaddr);
    uint32_t *fb2 = (uint32_t*)(conf.fb_vaddr+fbsz);
    memset(fb2, 0xff, fbsz);
    for (int i =0; i<0x200000; i++) {
        fb[i]=0x00ff0000;
    }
    init_mmu();
    //fb = 0x600000000;
    for (int i =0; i<0x200000; i++) {
        fb[i]=0x0000ff00;
    }
    qui_keymap_t maps;
    maps.key_down = 0x20f100164;
    maps.key_up = 0x20f100168;
    maps.key_ok = 0x20f1002d0;
    maps.key_cancel = 0x20f1002cc;
    qui_setup_renderer(fb2, conf.fb_width, conf.fb_height);
    qui_setup_klog();
    qui_setup_keymap(maps);
    if(qui_logger)
    {
        if(qui_logger->putstr)
        {
            qui_logger->putstr("Hello World!\nThis kernel -- DogLife was made by qiufuyu!\nWuhu ~ Now we hacked IPAD OS!\n:-)");
            qui_logger->drawpic(pic_data,pic_width,pic_height,0,4*16+16);
        }
    }
    //_cache_clean(0x600000000,conf.fb_width*conf.fb_height*4);
    
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
        qui_widget_separate(30);
        qui_widget_from_parent(&panel_1, (qui_point_t){0,0});
        qui_labelview_show(&lview);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        qui_widget_separate(380);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        
        qui_end_widget();
        //printf("%d",idx%2);
        idx++;
        memcpy(conf.fb_vaddr, fb2, fbsz);
    }
    //return 0;
}