/**
 * Copyright2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Rize<heseri@GhostBirdOS.org>
 * Explorer Shell Header
 */
 
#ifndef SHELL_H_
#define SHELL_H_

#define WIDTH_MAX 100					// Temporary
#define HEIGHT_MAX 37					// Temporary
#define DEFAULT_FONT "Standard Font"
#define SCR_W Video_Info.xres			// Temporary
#define SCR_H Video_Info.yres			// Temporary
#define BYTEPERPIXEL 3					// Temporary
#define FONT_W 8						// Temporary
#define FONT_H 16						// Temporary
#define VIDEO_MEM_ADDR 0xe0000000		// Temporary
#define BGCOLOR 0x00000000
#define FGCOLOR 0x00c0c0c0
#define SIZE_OF_BUFFER 256

// These temporary values should be judged.
// In order to finish new shell in time, they were defined as normal values.


//open the shell
void enable_shell(void);

//close the shell
void disable_shell(void);

void clrscr(void);
void put_char(char ascii);
void roll_screen(void);
int printk(const char *fmt, ...);

#endif