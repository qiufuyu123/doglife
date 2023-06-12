#include "event/key.h"
#include <stdbool.h>
qui_keymap_t qui_keys;

void qui_setup_keymap(qui_keymap_t map)
{
    qui_keys = map;
}

#define IS_DOWN(button) ((~*(button)) & 1)

bool qui_getkey(uint8_t id)
{
    if(id == QUI_KEY_UP)
        return IS_DOWN((uint32_t*)qui_keys.key_up);
    else if(id == QUI_KEY_DOWN)
        return IS_DOWN((uint32_t*)qui_keys.key_down);
}