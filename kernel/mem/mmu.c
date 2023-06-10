/**
 * @file mmu.c
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
 #include "pboot.h"
#include <mem/mmu.h>
 #include <mem/pmm.h>

 #include <stdbool.h>
 #include <string.h>
#include <stdint.h>
extern pboot_config_t conf;
 uint64_t tt_bits, tg0, tg1, t0sz, t1sz;
 char __start,__end;

int map_range_map(uint64_t* tt0, uint64_t va, uint64_t pa, uint64_t size, uint64_t sh, uint64_t attridx, bool overwrite, uint64_t paging_info, vm_protect_t prot, bool is_tt1)
{
    // NOTE: Blind assumption that all TT levels support block mappings.
    // Currently we configure TCR that way, we just need to ensure that we will continue to do so.

    uint64_t bits = 64ULL;
    if (is_tt1) {
        bits -= t1sz;
        va -= (0xffffffffffffffff - ((1ULL << (65 - t1sz)) - 1));
        va &= (1ULL << bits) - 1;
    } else {
        bits -= t0sz;
        va &= (1ULL << bits) - 1;
    }

    uint64_t pgsz = 1ULL << (tt_bits + 3ULL);
    if((va & (pgsz - 1ULL)) || (pa & (pgsz - 1ULL)) || (size & (pgsz - 1ULL)) || size < pgsz || (va + size < va) || (pa + size < pa))
    {
        return -1;
    }

    union tte tte;

    volatile uint64_t *tt = (volatile uint64_t*)tt0;
    if((bits - 3) % tt_bits != 0)
    {
        bits += tt_bits - ((bits - 3) % tt_bits);
    }
    while(true)
    {
        uint64_t blksz = 1ULL << (bits - tt_bits),
                 lo = va & ~(blksz - 1ULL),
                 hi = (va + size + (blksz - 1ULL)) & ~(blksz - 1ULL);

        if(size < blksz && hi - lo == blksz) // Sub-block, but fits into single TT
        {
         //下一级页表查询
            uint64_t idx = (va >> (bits - tt_bits)) & ((1ULL << tt_bits) - 1ULL);
            tte.u64 = tt[idx];
            if(tte.valid && tte.table && !overwrite)
            {
                tt = (volatile uint64_t*)((uint64_t)tte.oa << 12);
            }
            else if(!tte.valid || overwrite)
            {
               uint64_t *newtt = pmm_alloc_ttb();
                tte.u64 = 0;
                tte.valid = 1;
                tte.table = 1;
                tte.oa = (uint64_t)newtt >> 12;
                tt[idx] = tte.u64;
                tt = newtt;
            }
            else
            {
                return -1;
            }
            bits -= tt_bits;
            continue;
        }

        while(lo < hi)
        {
            uint64_t sz = blksz;
            if(lo < va)
            {
                sz -= va - lo;
            }
            if(sz > size)
            {
                sz = size;
            }
            if(sz < blksz) // Need to traverse anew
            {
                map_range_map((uint64_t*)tt0, va, pa, sz, sh, attridx, overwrite, paging_info, prot, is_tt1);
            }
            else if((pa & (blksz - 1ULL))) // Cursed case
            {
                uint64_t frag = pa & (blksz - 1ULL);
                map_range_map((uint64_t*)tt0, va, pa, sz - frag, sh, attridx, overwrite, paging_info, prot, is_tt1);
                map_range_map((uint64_t*)tt0, va + sz - frag, pa + sz - frag, frag, sh, attridx, overwrite, paging_info, prot, is_tt1);
            }
            else
            {
                uint64_t idx = (va >> (bits - tt_bits)) & ((1ULL << tt_bits) - 1);
                union tte otte;
                otte.u64 = tt[idx];
                if(otte.valid)
                {
                    if (!overwrite)
                        return -1;
                }
                if (prot & PROT_PAGING_INFO) {
                    tte.u64 = paging_info;
                    tte.valid = 0;
                    tte.table = 1;
                    tt[idx] = tte.u64;
                } else {
                    tte.u64 = 0;
                    tte.valid = 1;
                    tte.table = blksz == pgsz ? 1 : 0; // L3
                    tte.attr = attridx;
                    tte.sh = sh;
                    tte.af = 1;

                    tte.oa = pa >> 12;
                    tte.uxn = 1;

                    if (!(prot & PROT_EXEC)) {
                        tte.pxn = 1;
                    } else {
                        tte.pxn = 0;
                    }

                    if (!(prot & PROT_WRITE)) {
                        tte.ap |= 0b10;
                    }

                    if (is_tt1) {
                        if (!(prot & PROT_KERN_ONLY)) {
                            tte.ap |= 0b01;
                            tte.pxn = 1;
                        }
                        if (prot & PROT_EXEC) {
                            tte.uxn = 0;
                        }
                        tte.nG = 1;
                    }
                    if (pa && (prot & PROT_READ)) {
                        if (is_tt1) {
                            //phys_reference((tte.oa << 12) & (~0x3fff), (((tte.oa << 12) - ((tte.oa << 12) & (~0x3fff)) + blksz) + 0x3fff) & ~0x3fff);
                        }
                        tt[idx] = tte.u64;
                    } else {
                        tt[idx] = 0;
                    }
                }
                if (otte.valid && otte.table == (blksz == pgsz ? 1 : 0))
                {
                    if (is_tt1) {
                        //phys_dereference((otte.oa << 12) & (~0x3fff), (((otte.oa << 12) - ((otte.oa << 12) & (~0x3fff)) + blksz) + 0x3fff) & ~0x3fff);
                    }
                }
            }
            lo += blksz;
            va += sz;
            pa += sz;
            size -= sz;
        }
        break;
    }


}
static inline void flush_tlb(void)
{
    __asm__ volatile("isb");
    __asm__ volatile("tlbi vmalle1\n");
    __asm__ volatile("dsb sy");
}
void map_range_noflush_rwx(uint64_t va, uint64_t pa, uint64_t size, uint64_t sh, uint64_t attridx, bool overwrite) {
    map_range_map((void*)conf.ttbr0, va, pa, size, sh, attridx, overwrite, 0, PROT_READ|PROT_WRITE|PROT_EXEC|PROT_KERN_ONLY, false);
}
void map_range(uint64_t va, uint64_t pa, uint64_t size, uint64_t sh, uint64_t attridx, bool overwrite)
{
    map_range_noflush_rwx(va, pa, size, sh, attridx, overwrite);
    flush_tlb();
}
 void init_mmu()
 {
    //_disable_mmu_el1();
    pmm_init();
    tt_bits = 11;
    tg0 = 0b10; // 16K
    tg1 = 0b01; // 16K
    t0sz = 28;
    t1sz = 28;
   //  uint64_t pgsz = 1ULL << (tt_bits + 3);
   //  uint64_t ttbr0 = ((uint64_t)&__end+0x3fff )& (~0x3fff) + pgsz ;
   //  uint64_t ttbr1 = ((uint64_t)&__end+0x3fff )& (~0x3fff) ;
   //  memcpy(ttbr0,conf.ttbr0,pgsz);
   //  memcpy(ttbr1,conf.ttbr1,pgsz);
   //  //set_vbar_el1((uint64_t)&exception_vector);
   //  
   map_range(0x400000000,conf.loaded_phy_addr+0x800000, 0x800000, 3, 1,true);
   //_enable_mmu_el1((uint64_t)conf.ttbr0, 0x13A402A00 | (tg0 << 14) | (tg1 << 30) | (t1sz << 16) | t0sz, 0x04ff00, (uint64_t)conf.ttbr1);
    //kernel_vm_space.ttbr0 = (uint64_t)ttbr0;
    //kernel_vm_space.ttbr1 = (uint64_t)ttbr1;
 }