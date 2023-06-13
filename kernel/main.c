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
    
    qui_widget_t ctx;
    qui_create_widget(&ctx, 0, 300, 50, 500, 400, "title");

    //qui_begin_widget(&ctx);
    int idx = 0;
    while (1) {
        qui_begin_widget(&ctx);
        qui_begin_vertical(0, 0);
        qui_static_label((qui_point_t){0,0}, "Label 1", 0xffffffff);
        qui_widget_separate(380);
        qui_static_label((qui_point_t){0,0}, "Label 2", 0xffff0000);
        qui_end_widget();
        //printf("%d",idx%2);
        idx++;
        //usleep(1000*100);
        memcpy(conf.fb_vaddr, fb2, conf.fb_width*conf.fb_height*4);
        
    }
    //return 0;
}