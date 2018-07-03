/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window Unit Management Definition
 * Explorer/GUI/unit.h
 * version:Alpha
 * 2/19/2015
 */

#ifndef UNIT_H_
#define UNIT_H_

/**单元消息处理函数指针数组*/
#define NUM_UNIT_HANDLE		3
extern void (*unit_handle[NUM_UNIT_HANDLE])(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**单元种类列表*/
#define UNIT_CLOSE			0
#define UNIT_MINI			1
#define UNIT_BOTTON			2

/**单元结构*/
struct layer_unit
{
	unsigned int type;				/**单元的种类*/
	unsigned long x, y;				/**单元的位置*/
	unsigned long length, width;	/**单元的长宽*/
	struct layer_unit *next;		/**下一个单元结构*/
	void *ptr;						/**各个单元自定义的指针*/
};

/**按钮消息处理函数*/
void unit_botton_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**新建按钮函数*/
struct layer_unit *unit_new_botton(struct window *target, unsigned long x, unsigned long y, unsigned long length, unsigned long width, char *text);

/**窗体关闭按钮处理函数*/
void unit_close_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**窗体最小化按钮处理函数*/
void unit_mini_handle(struct layer *target, unsigned long x, unsigned long y, void *ptr);

/**创建新单元函数*/
struct layer_unit *GUI_new_unit(struct layer *target, unsigned int type, unsigned long x, unsigned long y, unsigned long length, unsigned long width);


#endif
