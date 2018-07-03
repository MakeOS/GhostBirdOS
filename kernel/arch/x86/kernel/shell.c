/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * made by Rize<heseri@ghostbirdos.org>
 * Explorer kernel Shell
 * Explorer/arch/x86/kernel/shell_frame.c
 * version:3.0.1
 * 7/5/2014 7:08 PM
 */

#include <main.h>
#include "shell.h"
#include <stdbool.h>
#include <stdarg.h>
#include <lib/graphics.h>

/**shell的光标坐标*/
static unsigned long cursor_x = 0, cursor_y = 0;

/**shell的长度、宽度(字节算)*/
static unsigned long length, width;

/**启用标志*/
static bool enable = false;

/**开启shell函数*/
void enable_shell(void)
{
	/**先判断是否已经打开*/
	if (enable == true) return;
	
	/**清空屏幕*/
	clrscr();
	
	/**计算屏幕长度、宽度*/
	length = Video_Info.xres / 8;
	width = Video_Info.yres / 16;
	
	/**打开shell*/
	enable = true;
	
	/**打印内核信息*/
	output_kernel_info();
}

//close the shell
void disable_shell(void)
{
	//do not clean the screen
	
	enable = false;
}

/*Clear Screen*/
void clrscr(void)
{
	unsigned char *p = Video_Info.vram;
	unsigned int i;
	for (i=0;i<=Video_Info.xres * Video_Info.yres * BYTEPERPIXEL;i++)
	{
		p[i] = 0;
	}
	
	cursor_x = 0;
	cursor_y = 0;
}

/**
 *put_char: Put a ascii char on screen.
 *Example: put_char('A'); put_char('1');
 *And put a smile:put_char(1);(Smile's ascii number is 1)
 */
void put_char(char ascii)
{
	/**如果shell当前不可用，就直接返回*/
	if (enable == false) return;
	
	/**对控制字符的判断*/
	if (ascii < 0x20)
	{
		/**如果输入字符是换行符，则进行换行*/
		if(ascii == 0xA)
		{
			cursor_x = 0;
			cursor_y ++;
		}
	
	/**如果不是控制字符*/
	}else{
		/**在光标处位置显示字*/
		draw_font(cursor_x * FONT_W, cursor_y * FONT_H, FGCOLOR, font(DEFAULT_FONT), ascii);
		
		/**将光标移动到下一个位置*/
		cursor_x ++;
	}
	
	/**判断是否需要换行*/
	if (cursor_x >= length)
	{
		cursor_x = 0;
		cursor_y ++;
	}
	
	if (cursor_y >= width)
	{
		roll_screen();
	}
}

/* Roll Screen */
void roll_screen(void)
{
	unsigned long x, y;
	
	/**循环将第一行之外的其他行向上提一行*/
	for (y = 16; y < Video_Info.yres; y ++)
	{
		for (x = 0; x < Video_Info.xres; x ++)
		{
			putpixel(x, y - 16, getpixel(x, y));
		}
	}
	
	/**未结束*/
	for (y = (Video_Info.yres - ((Video_Info.yres % 16) + 16)); y < Video_Info.yres; y ++)
	{
		for (x = 0; x < Video_Info.xres; x ++)
		{
			putpixel(x, y, BGCOLOR);
		}
	}
	
	cursor_y --;
}

/**printk函数，格式化输出字符串到屏幕上*/
int printk(const char *fmt, ...)
{
	/**停止调度*/
	disable_schedule();
	
	char buffer[256];
	va_list arg;
	unsigned long n;
	
	/**format*/
	va_start(arg, fmt);/*init argument point*/
	vsprintf(buffer, fmt, arg);/*write format & get length*/
	
	/**循环将字写入屏幕*/
	for (n = 0; buffer[n] != 0; n ++) put_char(buffer[n]);
	
	/**防止空指针引发危险*/
	va_end(arg);
	
	/**允许调度*/
	enable_schedule();
	
	/**正常返回*/
	return n;
}