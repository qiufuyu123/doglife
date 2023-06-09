/**
 * @file bitmap.h
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
 #ifndef _H_BITMAP
 #define _H_BITMAP

#include <stdint.h>
#include <stdbool.h>

#define BITMAP_MASK 1
typedef struct bitmap {
   uint32_t btmp_bytes_len;
/* 在遍历位图时,整体上以字节为单位,细节上是以位为单位,所以此处位图的指针必须是单字节 */
   uint8_t* bits;
}bitmap_t;

void bitmap_init(struct bitmap* btmp);
bool bitmap_scan_test(struct bitmap* btmp, uint32_t bit_idx);
int bitmap_scan(struct bitmap* btmp, uint32_t cnt);
void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value);
 #endif