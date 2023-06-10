#include <assert.h>
#include "klog.h"
#include "qfb.h"
#include <font.inc>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
kconsole_t *qui_logger = 0;
int color_16[16]={
    0x000000,
    0x0000AA,
    0X00AA00,
    0X00AAAA,
    0XAA0000,
    0XAA00AA,
    0xAA5500,
    0xAAAAAA,
    0x555555,
    0x5555FF,
    0x55FF55,
    0X55ffff,
    0xff5555,
    0xff55ff,
    0xffff55,
    0xffffff
};
/**
 * @brief NOTE
 * Anyway....
 * In fact, I dont want to set up fb at kernel mode
 * I once wanted to set up ascii mode (0xb8000) to printf in kernel 
 * and then, set up fb mode when we switch to user or use kernel modules
 * But...
 * I found it really difficult to switch to vesa/fb mode in x86 protected mode
 * So i just let it be inited in grub mode
 *  
 * TODO: I hope one day I will find out the way ....
 * @author: qiufuyu
 */
#define CHAR_W 8
#define CHAR_H 16
#define USE_fb
static uint32_t cur_x,cur_y;
//uint32_t max_text_x,max_text_y;
static uint32_t fb_width,fb_height;
static uint32_t* fb_buffer;
static uint32_t* fb_double_buff;
static bool is_big_endian = true;
//char is_doubled=0;
//char buffer_state=0;
static uint32_t vbg=0x00000000,vfg=0xffffffff;
void fb_put_pixel(uint32_t x,uint32_t y,uint32_t color)
{
    if (fb_width <= x ||  fb_height <= y)
        return;
    fb_buffer[ fb_width * y + x] = color;
}
uint32_t color_compose(uint8_t r,uint8_t g,uint8_t b) {
    return ((((uint32_t)0) & 0xff) << 24) | ((((uint32_t)r) & 0xff) << 16) | ((((uint32_t)g) & 0xff) << 8)  | ((((uint32_t)b) & 0xff) << 0); // works on ARGB8,8,8,8 only, i'll add ARGB5,9,9,9 eventually
}
void fb_draw_pic(char *bits,int w,int h, int xx,int yy)
{
    for (int y = 0; y<h; y++) {
        for (int x=0;x<w; x++) {
            int g = bits[y*w+x];
            fb_buffer[(yy+y)*fb_width+x] = color_compose(g, g, g);
        }
    }
}
void fb_putchar(char ch, uint32_t x, uint32_t y, uint32_t color) {
    #ifdef USE_fb
    uint32_t px = 0;  // position in the charactor - an 8x8 font = 64 bits
    uint64_t bCh = FONT[ch];
    
    // check if it will be drawn off screen
    //if (x+8 < 0 || x > fb_width || y+8 < 0 || y > fb_height)
    //    return;
    
    // test if the charactor will be clipped (will it be fully in the screen or partially)
    if (x >= 0 && x+8 <= fb_width && y >= 0 && y+8 <= fb_height) {
        // fully in the screen - pre calculate some of the values
        // so there is less going on in the loop
        int i = fb_width * (y - 1) + x + 8;
        int incAmount = fb_width - 8;
        for (int yy = 7; yy >= 0; yy-- ) {
            i += incAmount;
            int oldpx = px;
            for (int xx = 7; xx >= 0; xx-- ) {
                // test if a pixel is drawn here
                if ((bCh >> px++) & 1) {
                    fb_buffer[i] = vfg;
                }else
                {
                    fb_buffer[i]=vbg;
                }
                i++;
            }
            i += incAmount;
            px=oldpx;
            for (int xx = 7; xx >= 0; xx-- ) {
                // test if a pixel is drawn here
                if ((bCh >> px++) & 1) {
                    fb_buffer[i] = vfg;
                }else
                {
                    fb_buffer[i]=vbg;
                }
                i++;
            }
        } 
    } else {
        // partially in the screen
        int xpos = 0;
        int i = fb_width* (y - 1);    
        for (int yy = 0; yy < 8; yy++ ) {
            i += fb_width;
            xpos = x;
            for (int xx = 7; xx >= 0; xx-- ) {
                // test if a pixel is drawn here
                if ((bCh >> px++) & 1) {
                    // test if the pixel will be off screen
                    if (xpos > 0 && xpos < fb_width && yy+y > 0 && yy+y < fb_height)
                        fb_buffer[ i + xpos] = vfg;
                }else fb_buffer[i]=vbg;
                xpos++;
            }
        } 
    }
    #endif
}
void fb_scroll_up()
{
    //Prevent scrolling while inputing(printing)...  
    memcpy(fb_buffer,fb_buffer+fb_width*CHAR_H,(fb_height-CHAR_H)*fb_width*4);
    memset(fb_buffer+fb_width*(fb_height-CHAR_H),0,fb_width*CHAR_H*4);
}
void fb_gen_puchar(char ch)
{
        if(ch=='\r')
        {
            cur_x=0;
            return;
        }else if(ch=='\b')
        {
            if(cur_x>0)
                cur_x-=CHAR_W;
            else if(cur_y>0)
            {
                cur_x=(fb_width/CHAR_W)*CHAR_W;
                cur_y-CHAR_H;
            }
            
            fb_putchar(' ',cur_x,cur_y,0xffffffff);
            return;
        }else if(ch=='\n')
        {
            
            cur_y+=CHAR_H;
            if(cur_y>=fb_height)
            {
                cur_y-=CHAR_H;
                fb_scroll_up();
            }
            cur_x=0;
            
            return;
        }else fb_putchar(ch,cur_x,cur_y,0xffffffff);
        cur_x+=CHAR_W;
        if(cur_x>=fb_width)
        {
            cur_x=0;
            cur_y+=CHAR_H;
        }
        if(cur_y>=fb_height)
        {
            cur_x=0;
            cur_y-=CHAR_H;
            fb_scroll_up();
        }
}
void fb_printstr(char *str)
{
    while (*str)
    {
        fb_gen_puchar(*str);
        str++;
    }
    //fb_flush_buffer();
}
void fb_cls()
{
    #ifdef USE_fb
    memset(fb_buffer,0,fb_width*fb_height*4);
    //fb_flush_buffer();
    // for (uint32_t i = 0; i < fb_width*fb_height; i++)
    // {
    //     fb_buffer[i]=0;
    // }
    #endif
    cur_x=cur_y=0;
}
void fb_updata()
{
    //fb_buffer=Klogger->frame_buffer;
}
void fb_setcolor(int bbg,int ffg)
{
    vbg=bbg;
    vfg=ffg;
}
void fb_setcurse(int x,int y)
{
    cur_x=x*CHAR_W;
    cur_y=y*CHAR_H;
    if(cur_x>=fb_width || cur_y>=fb_height)
    {
        cur_x=cur_y=0;
    }
}
kconsole_t fb_console={
    .cls=&fb_cls,
    .id=1,
    .init=0,
    .pause=0,
    .putchr=&fb_gen_puchar,
    //.flush=fb_flush_buffer,
    .putstr=&fb_printstr,
    .setcolor=&fb_setcolor,
    .setcurse=&fb_setcurse,
    .update=&fb_updata,
    .drawpic=&fb_draw_pic
};
//kconsole_t *Klogger = &fb_console;
void qui_setup_klog()
{
    fb_width=qui_renderer.width;
    fb_height=qui_renderer.height;
    fb_buffer=qui_renderer.pixels;
    cur_x = cur_y = 0;
    //memset(fb_buffer,0,fb_width*fb_height*4);
    // for (uint32_t i = 0; i < fb_width*fb_height; i++)
    // {
    //     fb_buffer[i]=0;
    // }
    //max_text_x=fb_width/8;
    //max_text_y=fb_height/8;
    cur_x=cur_y=0;
    // fb_buffer=kmalloc_page(fb_console.page_cnt);
    // if(!fb_buffer)
    // {
    //     PANIC("Cannot Alloc MemBuffer For fb!\n(Double-Buffered Required!)\n");
    // }
    //vfg = color_compose(255, 255, 255);
    qui_logger=&fb_console;
    //init_printlock();
    //printf("[fb WIDTH:%d HEIGHT:%d PGNUM:%d]\n",fb_width,fb_height,fb_console.page_cnt);
    //fb_gen_puchar('h');
    //fb_gen_puchar('e');
    //fb_printstr("hello fb!\n");
    //while(1);
}
void fb_setbgcolor_16(char c)
{
    if(c<16)
        vbg=color_16[c];
}
void fb_setftcolor_16(char c)
{
    if(c<16)
        vfg=color_16[c];
}