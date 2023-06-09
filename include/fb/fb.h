/**
 * @file fb.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-08
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
#ifndef _H_FB
#define _H_FB
#include <stdint.h>
typedef struct kconsole
{
    void(*init)();
    void(*update)();
    void(*flush)();
    void(*pause)();
    void(*putstr)(char *str);
    void(*putchr)(char c);
    void(*setcolor)(int bg,int fg);
    void(*setcurse)(int x,int y);
    void(*drawcurse)(int x,int y);
    void(*drawpic)(char *bits, int w,int h, int xx, int yy);
    int id;
    void(*cls)();
    uint32_t frame_buffer;
    uint32_t page_cnt;
}kconsole_t;

extern kconsole_t *Klogger;
void init_fb();

#endif