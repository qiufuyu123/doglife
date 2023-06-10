/**
 * @file fb.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-10
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */

#ifndef _H_QFB
#define _H_QFB


#include<stdint.h>

typedef struct
{
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
}qui_framebuffer_t;

extern qui_framebuffer_t qui_renderer;

void qui_setup_renderer(void *fb, uint32_t width, uint32_t height);

#endif