/**
 * @file widget.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-11
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
#ifndef _H_WIDGET
#define _H_WIDGET

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    int x;
    int y;
}qui_point_t;

typedef struct
{
    int x,y,x2,y2;
}qui_rect_t;

typedef struct
{
    int x,y,w,h,now;
    bool is_vert;

}qui_widget_area_t;

typedef struct 
{
    void *parent;

    //int uid;
    uint32_t x,y,w,h;
    char *title;
    uint32_t title_len;

    qui_widget_area_t area;
    qui_point_t scroller;
    qui_rect_t bound_rect;
    bool focus;
}qui_widget_t;


void qui_create_widget(qui_widget_t*ctx, void *parent, int x, int y,int w,int h,char *title);

void qui_begin_widget(qui_widget_t *ctx);

qui_point_t qui_widget_space(qui_widget_t *ctx);

qui_point_t qui_point_comb(qui_point_t o, qui_point_t p);

void qui_widget_separate(int delta);

void qui_widget_spaceend(qui_widget_t *ctx, int w,int h);

void qui_begin_vertical(int x, int y);

void qui_begin_horizon(qui_widget_t *ctx);

void qui_end_widget();



#endif