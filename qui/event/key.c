#include "event/key.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
qui_keymap_t qui_keys;

void qui_setup_keymap(qui_keymap_t map)
{
    qui_keys = map;
}

#define IS_DOWN(button) ((~*(button)) & 1)

uint8_t qui_raw_key(uint8_t id)
{
    if(id == QUI_KEY_UP)
        return IS_DOWN((uint32_t*)qui_keys.key_up);
    else if(id == QUI_KEY_DOWN)
        return IS_DOWN((uint32_t*)qui_keys.key_down);
    else if(id == QUI_KEY_OK)
        return IS_DOWN((uint32_t*)qui_keys.key_ok);
    else if(id == QUI_KEY_CANCEL)
        return IS_DOWN((uint32_t*)qui_keys.key_cancel);
    return 0;
}

bool qui_getkey(uint8_t id)
{
    uint8_t r = qui_raw_key(id);
    if(r)
    {
        // while (qui_raw_key(id)!=0) {
        // }
        
        //return 1;
    }
    return r;
    //return qui_raw_key(id);
}