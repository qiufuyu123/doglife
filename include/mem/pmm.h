/**
 * @file pmm.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-07
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
 #ifndef _H_PMM
 #define  _H_PMM

#include <stdint.h>

void pmm_init();

uint64_t pmm_alloc_ttb();

 #endif