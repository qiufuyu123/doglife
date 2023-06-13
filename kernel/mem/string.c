/**
 * @file string.c
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief STRING LIB
 * @version 0.1
 * @date 2023-06-07
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */

 #include <stddef.h>
#include <stdint.h>
void memcpy(char *dest, char *src, size_t len)
{
    for (size_t i = 0; i<len; i++) {
        dest[i] = src[i];
    }
}

void memset(char *dest, char c, size_t len)
{
    for (size_t i = 0; i<len; i++) {
        dest[i] = c;
    }
}

size_t strlen(char *str)
{
    size_t r=0;
    while (*str++) {
        r++;
    }
    return r;;
}