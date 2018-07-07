/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Initialization
 * Explorer/GUI/init.c
 * version:Alpha
 * 8/16/2014 9:08 AM
 */

#include <memory.h>
#include <stdlib.h>
#include <GUI.h>
#include <lib/mem.h>
#include <lib/graphics.h>
#include "layer.h"

/**GUI根图层*/
struct layer *layer_root = NULL;
#define LAYER_ROOT_LENGTH	320
#define LAYER_ROOT_WIDTH	48

/**initialization GUI*/
void init_GUI(void)
{
	/**关闭shell*/
	disable_shell();
	
	/**清除屏幕*/
	clear_screen();
	
	/**创建GUI根图层*/
	for (;layer_root == NULL;) layer_root = kmalloc(sizeof(struct layer), 0);
	
	/**清空获得的内存*/
	memset(layer_root, 0, sizeof(struct layer));
	
	/**填充相应数据*/
	for (;layer_root->buf == NULL;) layer_root->buf = vmalloc (LAYER_ROOT_LENGTH * LAYER_ROOT_WIDTH * 4);
	layer_root->visiable = true;
	layer_root->x = 0;
	layer_root->y = 0;
	layer_root->length = LAYER_ROOT_LENGTH;
	layer_root->width = LAYER_ROOT_WIDTH;
	layer_root->top = layer_root;
	layer_root->bottom = layer_root;
	layer_root->winptr = NULL;
	
	/**填充矩形*/
	GUI_put_square(layer_root, 0x400000ff, 0, 0, LAYER_ROOT_LENGTH, LAYER_ROOT_WIDTH);
	// GUI_put_square(layer_root, 0x40000000, 0, 0, LAYER_ROOT_LENGTH, LAYER_ROOT_WIDTH);
	
	/**信息输出*/
	GUI_put_string(layer_root, 0xffffffff, 0, 00, 0, 0, font("simsun"), "Explorer Graphical User Interface.");
	GUI_put_string(layer_root, 0xffffffff, 0, 16, 0, 0, font("simsun"), "Version:1.0");
	GUI_put_string(layer_root, 0xffffffff, 0, 32, 0, 0, font("simsun"), "Copyright 2015 Ghost Bird OS Developers.");

	/**This function in window.c*/
	init_Window();
}

/**禁用GUI*/
void disable_GUI(void)
{
	
}
