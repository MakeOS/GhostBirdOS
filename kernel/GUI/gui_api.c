/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface API
 * Explorer/GUI/gui_api.c
 * version:Alpha
 * 2/8/2015 11:41 PM
 */

#include <syscalls.h>
#include <GUI.h>
#include "window/window.h"

/**API列表*/
#define NEW_WINDOW	1

/**GUI API调用函数*/
void sys_GUI_API(struct context context)
{
	/**不允许调度*/
	disable_schedule();
	
	/**分发功能*/
	switch(context.ebx)
	{
		case NEW_WINDOW: goto new_window;
		default: goto finish;
	}
	
/**
 * 新窗口请求处理程序
 * ebx = 1, ecx = ptr of title, edx = style, esi = length, edi = width
 * 返回值存放在eax中，为新窗口的结构指针
 */
new_window:

	/**调用程序*/
	context.eax = (int) GUI_window((char *)context.ecx, context.edx,0 , 0, context.esi, context.edi);
	goto finish;
	
/**结束系统调用*/
finish:
	/**允许调度*/
	enable_schedule();
	
	/**正常返回*/
	return;
}