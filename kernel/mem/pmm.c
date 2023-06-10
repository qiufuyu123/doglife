#include <mem/pmm.h>
#include <mem/bitmap.h>
#include <pboot.h>
#include <stdbool.h>
#include <stdint.h>

#define TTB_SIZE 0x4000
extern pboot_config_t conf;


bitmap_t pmm_bitmap;
int i=0;
char pmm_bitmap_data[512];

void pmm_set_npage(uint64_t start, uint64_t end)
{
    for (uint64_t i =start;i<=end;start+=TTB_SIZE) {
        int idx = (conf.ttbr_base_start-i)/TTB_SIZE;
        bitmap_set(&pmm_bitmap, idx, 1);
    }
}

void pmm_init()
{
    pmm_bitmap.bits = &pmm_bitmap_data[0];
    pmm_bitmap.btmp_bytes_len = 512;
    bitmap_init(&pmm_bitmap);  
    pmm_bitmap_data[0]=0xff;
    pmm_bitmap_data[1]=0xff;
    //pmm_set_npage(conf.ttbr_base_upper - 0x4000*16,conf.ttbr_base_upper,1);
}

uint64_t pmm_alloc_ttb()
{
    int idx = bitmap_scan(&pmm_bitmap,1);
    if(idx < 0)
        return 0;
    return conf.ttbr_base_start - idx *TTB_SIZE;
}