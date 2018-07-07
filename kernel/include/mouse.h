/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu.1314@gmail.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Mouse support head
 * Explorer/include/mouse.h
 * version:Alpha
 * 8/26/2014 10:30 PM
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include <task.h>

/**鼠标x\y全局变量*/
long mouse_x, mouse_y;

/**鼠标命令缓冲区最大数量*/
#define SIZE_OF_BUFFER_MOUSE	64

/**初始化鼠标函数*/
void init_mouse(void);

void put_mouse_x(long x);
long get_mouse_x(void);
void put_mouse_y(long y);
long get_mouse_y(void);

/**鼠标命令读取函数
 * flag为函数执行属性，属性定义如下。
 * 返回值：0xff则执行无效，其他值则为鼠标命令。
 */
unsigned char get_mouse_cmd(int flag);
/**其中flag有以下几种情况*/
#define MOUSE_TRY	1				// 试探获得，如果当前缓冲区为空，则立马返回，如果不为空，则取出数据
#define MOUSE_GET	2				// 直接获得，如果当前缓冲区为空，一直忙等待数据

/**注册鼠标侦听任务函数*/
void set_mouse_interception(union task *target);

/**鼠标中断处理函数*/
void int_mouse_handle(void);

#endif