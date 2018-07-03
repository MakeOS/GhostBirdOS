/*
 *Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 *modified by Rose<roselyu@163.com>
 *Explorer Keyboard support head
 *ExplorerOS/Kernel/include/keyboard.h
 *version:Alpha
 *7/18/2014 5:48 PM
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdbool.h>

/**键盘缓冲区的最大数量*/
#define MAX_KEYBUFFER			128

/**初始化键盘函数*/
void init_keyboard(void);

/**键盘中断处理函数*/
void int_keyboard_handle(void);

/**获取键盘扫描码函数*/
unsigned char get_key(void);

/**扫描码转换为ASCII码函数*/
unsigned char key_to_ascii(unsigned char code);

/**左Shift是否被触发判断*/
bool L_Shift;

/**右Shift是否被触发判断*/
bool R_Shift;

/**大小写是否被设置*/
bool CapsLock;

/**转换数组*/
static unsigned char key_table[0x7f] = {
	0x00,0x1B,'1','2','3','4','5','6','7','8','9','0','-','=',0x08,0x09,'Q','W','E','R','T','Y','U','I','O','P','[',']',0x0D,0x00,'A','S','D','F','G','H','J','K','L',';',0x60,0x00,0x00,0x5c,'Z','X','C','V','B','N','M',',','.','/',0x00,'*',0x00,' ',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0X00,0X00,'7','8','9','-','4','5','6','+','1','2','3','0','.',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'_',0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0x5C,0X00,0x00,0X00
};

#endif