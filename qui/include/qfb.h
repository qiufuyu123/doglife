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


#include "ui/widget.h"
#include<stdint.h>
#define QUI_STACK_MAX 10
extern void* qui_ctx_stack[QUI_STACK_MAX];
extern uint32_t qui_ctx_stack_top;
typedef struct
{
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
}qui_framebuffer_t;

extern qui_framebuffer_t qui_renderer;

void qui_setup_renderer(void *fb, uint32_t width, uint32_t height);

void qui_push_ctx(void*ctx);

void qui_pop_ctx();

void* qui_top_ctx();

void qui_draw_line(int x1,int y1,int x2,int y2,uint32_t color);

void qui_draw_rect(int x1, int y1, int w, int h,uint32_t color);

void qui_fill_rect(int x1, int y1, int w, int h,uint32_t color);

qui_point_t qui_draw_text(int x1, int y1, char*text, uint32_t color,uint32_t bg_color);

uint32_t qui_font_width();

uint32_t qui_font_height();
#endif