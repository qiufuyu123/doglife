#ifndef _H_UI
#define _H_UI
#include <stdint.h>
typedef struct
{
    uint32_t *vaddr;
    uint32_t width;
    uint32_t height;
}qui_emu_arg_t;
void qui_thread(qui_emu_arg_t *config);

#endif