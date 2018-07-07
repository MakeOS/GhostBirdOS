/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Debug Output System
 * Explorer/arch/x86/kernel/error.c
 * version:Alpha
 * 12/6/2014 7:15 PM
 */

#include <debug.h>
#include <stdarg.h>
#include <lib/graphics.h>
#include <info.h>

#define NUM_BUFFER	256

/**错误处理函数
 * 本函数执行后，不能返回。
 * info为错误的信息，格式化输出。
 */
void error(const char *info, ...)
{
	/**禁止中断*/
	io_cli();
	
	va_list arg;
	
	/**缓冲区*/
	char buffer[NUM_BUFFER];
	
	/**初始化参数指针*/
	va_start(arg, info);
	
	/**格式化输出*/
	vsprintf(buffer, info, arg);
	
	/**启动shell*/
	enable_shell();
	
	/**打印至屏幕上*/
	printk(buffer);
	
	/**halt*/
	for (;;) io_hlt();
}
