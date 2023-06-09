/**
 * @file pboot.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief Describe the config data passing from PongoOS
 * @version 0.1
 * @date 2023-06-07
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
 #ifndef _H_PBOOT
 #define _H_PBOOT

#include <stdint.h>

typedef struct
{
    uint64_t pongo_base_vaddr;
    uint64_t pongo_base_size;

    uint64_t ios_base_vaddr;
    uint64_t ios_base_size;

    uint64_t ttbr0, ttbr1;
    uint64_t ttbr_base_upper;
    uint64_t ttbr_base_lower;
    uint64_t ttbr_base_start;

    uint64_t loaded_phy_addr;

    uint64_t fb_vaddr;
    uint32_t fb_width;
    uint32_t fb_height;
}pboot_config_t;
 #endif