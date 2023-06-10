#include "fb/img_arr.h"
#include <stdint.h>
#include <mem/mmu.h>
#include <pboot.h>

#include"klog.h"
#include "qfb.h"

pboot_config_t conf;
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
    qui_setup_renderer(conf.fb_vaddr, conf.fb_width, conf.fb_height);
    qui_setup_klog();
    if(qui_logger)
    {
        if(qui_logger->putstr)
        {
            qui_logger->putstr("Hello World!\nThis kernel -- DogLife was made by qiufuyu!\nWuhu ~ Now we hacked IPAD OS!\n:-)");
            qui_logger->drawpic(pic_data,pic_width,pic_height,0,4*16+16);
        }
    }
    
    while (1) {
    
    }
    //return 0;
}