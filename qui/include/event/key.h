/**
 * @file key.h
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

#ifndef _H_KEY
#define _H_KEY

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint64_t key_up;
    uint64_t key_down;
    uint64_t key_left;
    uint64_t key_right;
    uint64_t key_ok;
    uint64_t key_cancel;
}qui_keymap_t;

enum{
    QUI_KEY_UP,
    QUI_KEY_DOWN,
    QUI_KEY_LEFT,
    QUI_KEY_RIGHT,
    QUI_KEY_OK,
    QUI_KEY_CANCEL
};
extern qui_keymap_t qui_keys;

void qui_setup_keymap(qui_keymap_t map);

bool qui_getkey(uint8_t id);
#endif