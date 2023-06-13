/**
 * @file label.h
 * @author qiufuyu (qiufuyutony@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-12
 * 
 * THIS FILE IS PART OF DOGLIFE
 * -----------------------------
 * Copyright (C) 2023 qiufuyu
 * This project use PongoOS to boot.
 * 
 */
#ifndef _H_LABEL
#define _H_LABEL

#include "ui/widget.h"
#include <stdbool.h>
#include <stdint.h>

#define QUI_LABELVIEW_MAX_LINE 64
typedef struct
{
    qui_widget_t *panel;
    char *lines[QUI_LABELVIEW_MAX_LINE];
}qui_label_view_t;

void qui_static_label(qui_point_t p, char *text, uint32_t color);

void qui_create_labelview(qui_label_view_t*view,qui_widget_t *panel);

void qui_labelview_add(qui_label_view_t *view,char *text,int line);

void qui_labelview_show(qui_label_view_t*view);
#endif