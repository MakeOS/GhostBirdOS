/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Unit
 * Explorer/GUI/window/unit.c
 * version:Alpha
 * 2/15/2015 7:58 PM
 */

#include <GUI.h>
#include "memory.h"
#include "window.h"
#include "unit.h"


/**单元消息处理函数指针数组*/
void (*unit_handle[NUM_UNIT_HANDLE])(struct layer *target, unsigned long x, unsigned long y, void *ptr) =
{
	unit_close_handle,
	unit_mini_handle,
	unit_botton_handle
};

/**按钮控件结构描述*/
struct unit_botton
{
	char *text;
};

/**按钮消息处理函数*/
void unit_botton_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	window_print(target->winptr, "botton:(%d,%d)", x, y);
}

/**新建按钮函数*/
struct layer_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width, char *text)
{
	/**纠正参数*/
	correct_para(target, &x, &y, &length, &width);
	
	/**新建窗口单元*/
	GUI_new_unit(target->layer, UNIT_BOTTON, x, y, length, width);
	
	/**绘制按钮*/
	GUI_put_square(target->layer, 0xff0066ff, x, y, length, width);
	
	/**显示按钮上面的文字*/
	GUI_put_string(target->layer, 0xffffffff, x, y + 4, x + length, y + width, font("Standard Font"), text);
}

/**窗体关闭按钮处理函数*/
void unit_close_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	/**首先改变窗口结构辨识*/
	target->winptr->ident = 0;
	/**释放相应窗口*/
	GUI_free_window(target->winptr);
}

/**窗体最小化按钮处理函数*/
void unit_mini_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr)
{
	window_print(target->winptr, "click mini:(%d,%d)", x, y);
}

/**创建新单元函数*/
struct layer_unit *GUI_new_unit(struct layer *target, unsigned int type, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	struct layer_unit *new_unit;
	
	/**申请一块内存空间放下新的单元*/
	for (new_unit = NULL; new_unit == NULL; )
		new_unit = kmalloc(sizeof(struct layer_unit), 0);
	
	/**参数赋值*/
	new_unit->x = x;
	new_unit->y = y;
	new_unit->length = length;
	new_unit->width = width;
	new_unit->type = type;
	
	/**加入图层中单元单向链表*/
	new_unit->next = target->layer_unit_list;
	target->layer_unit_list = new_unit;
}

/**获取新单元位置*/