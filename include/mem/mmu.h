/**
 * @file mmu.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief MMU
 * @version 0.1
 * @date 2023-06-07
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
 #ifndef _H_MMU
 #define _H_MMU

#include <pboot.h>

#include <stdint.h>
typedef enum {
    PROT_READ = 1,
    PROT_WRITE = 2,
    PROT_EXEC = 4,
    PROT_KERN_ONLY = 8,
    PROT_DEVICE = 16,
    PROT_PAGING_INFO = 32
} vm_protect_t;
union tte
{
    struct
    {
        uint64_t valid :  1,
                 table :  1,
                 attr  :  3,
                 ns    :  1,
                 ap    :  2,
                 sh    :  2,
                 af    :  1,
                 nG    :  1,
                 oa    : 36,
                 res00 :  3,
                 dbm   :  1,
                 cont  :  1,
                 pxn   :  1,
                 uxn   :  1,
                 ign0  :  4,
                 pbha  :  4,
                 ign1  :  1;
    };
    struct
    {
        uint64_t res01  : 12,
                 oahigh :  4,
                 nT     :  1,
                 res02  : 42,
                 pxntab :  1,
                 uxntab :  1,
                 aptab  :  2,
                 nstab  :  1;
    };
    uint64_t u64;
};

extern void _enable_mmu_el1(uint64_t ttbr0,uint64_t flag,uint64_t flags,uint64_t ttbr1);

void init_mmu();

 #endif