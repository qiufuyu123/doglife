#include "fb/fb.h"
#include <stdint.h>
#include <mem/mmu.h>
#include <pboot.h>
#include <fb/img_arr.h>
pboot_config_t conf;

extern uint32_t cur_x,cur_y;
void kernel_main()
{
    
    conf = *(pboot_config_t*)(0x300000000ULL);
    uint32_t *fb = (uint32_t*)(conf.fb_vaddr);
    for (int i =0; i<0x200000; i++) {
        fb[i]=0x00ff0000;
    }
    init_mmu();
    for (int i =0; i<0x200000; i++) {
        fb[i]=0x0000ff00;
    }
    init_fb();
    if(Klogger)
    {
        if(Klogger->putstr)
            Klogger->putstr("Hello World!\nThis kernel was made by qiufuyu!\nWooooo~~~~ Now we hacked IPAD OS!\n:-)");
        else
         fb_printstr("NO KLOGGER SUPPORT!");
    }
    Klogger->drawpic(pic_data,pic_width,pic_height,0,cur_y+16);
    while (1) {
    
    }
    //return 0;
}