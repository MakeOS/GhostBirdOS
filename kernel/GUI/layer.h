/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Layer Definition
 * Explorer/GUI/layer.h
 * version:Alpha
 * 2/10/2015 1:38 PM
 */

#ifndef GUI_LAYER_H_
#define GUI_LAYER_H_

#include "window/window.h"
#include <stdlib.h>

/**图层结构体*/
struct layer
{
	bool visiable;							// 图层是否可见
	long x, y;								// 图层位置
	unsigned long length, width;			// 图层的长宽
	unsigned int *buf;						// 图层的画布
	struct layer *top, *bottom;				// 图层双向链表
	struct window *winptr;					// 指向相应窗口的指针
	struct layer_unit *layer_unit_list;		// 图层单元单向链表
};

/**根图层*/
extern struct layer *layer_root;

/**附加图层*/
extern struct layer *attach_layer;

/**背景图层和光标图层*/
extern struct layer *background_layer, *pointer_layer, *taskbar_layer;

/**Explorer GUI 内部抽象图片结构*/
struct GUI_image
{
	/**图片长、宽*/
	unsigned long length, width;
	
	/**图片数据指针*/
	unsigned int *data;
	
	/**引用计数，当该变量为1，且窗口要关闭时一起释放*/
	unsigned long count;
	
	/**将一个窗口加载的多个图形资源串成一个单向链表，以便于图片的回收*/
	struct GUI_image *next;
};

#endif
