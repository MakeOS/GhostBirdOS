/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window Management Definition
 * Explorer/GUI/window.h
 * version:Alpha
 * 12/14/2014
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "../layer.h"

/**Application是否在工作的标志*/
extern bool Application_work_flag;

/**Application区域长度*/
extern unsigned long window_Application_length;

/**Date区域长度*/
extern unsigned long window_Date_length;

/**任务栏颜色*/
#define TASKBAR_COLOR		0xc0ffffff
#define TASKBAR_LINE_COLOR	0xe0ffffff

/**任务栏高度（包括边际线）*/
#define TASKBAR_WIDTH		20

/**时钟字符串颜色*/
#define TIME_COLOR			0xff000000

/**GUI控制台窗口*/
struct window *GUI_control;

/**窗口结构体辨识特殊数*/
#define WINDOW_IDENT	11262

/**活动/非活动窗口的边框颜色（如果有边框）*/
#define WINDOW_ACTIVE_FRAME_COLOR				0xc00066FF			// 活动窗口的边框颜色
#define WINDOW_INACTIVE_FRAME_COLOR				0xc0c0c0c0			// 非活动窗口的边框颜色

/**活动/非活动窗口的标题颜色（如果有标题）*/
#define WINDOW_ACTIVE_TITLE_COLOR				0xffffffff			// 活动窗口的标题颜色
#define WINDOW_INACTIVE_TITLE_COLOR				0xff000000			// 非活动窗口的标题颜色

/**
 * 窗口的风格有以下种类
 * 无边框窗口：没有边框，在这种情况下，标题将会被忽略
 * 正常窗口：拥有正常的窗口边框
 */
#define WINDOW_NONE		0				// 无边框
#define WINDOW_NORMAL	1				// 正常边框

/**正常窗口的边框属性*/
#define WINDOW_NORMAL_HEADER_WIDTH		32
#define WINDOW_NORMAL_FRAME_WIDTH		5

/**窗口结构体*/
#define WINDOW_NUM_TITLE				64
struct window{
	short int ident;						/**窗口结构体辨识*/
	char title[WINDOW_NUM_TITLE];			/**窗口标题*/
	char style;								/**窗口的风格*/
	struct layer *layer;					/**窗口所对应的图层*/
	unsigned long length, width;			/**窗口的长、宽*/
	unsigned long print_x, print_y;			/**打印字符串的坐标*/
};

/**加载图片函数*/
struct GUI_image *window_load_image(char *filename);

/**初始化窗口系统*/
void init_Window(void);

/**创建Window函数
 * 创建一个窗口，标题为title，
 * 风格为style，
 * 当x = 0且y = 0时，窗口会创建在最中间
 * 窗口中有效内容尺寸为length*width
 */
struct window *GUI_window(char *title, char style, unsigned long x, unsigned long y, unsigned long length, unsigned long width);

/**纠正绘制参数函数
 * 窗体有无边框、边框风格不同时
 * 绘制到图层上的实际地址是不一样的
 */
void correct_para(struct window *target, unsigned long *x, unsigned long *y, unsigned long *length, unsigned long *width);

/**输出字符串到窗口*/
void window_string(struct window *target, unsigned long x, unsigned long y, char *s);

/**窗口信息输出函数*/
int window_print(struct window *target, const char *fmt, ...);

/**初始化附加图层函数*/
void init_attach_layer(void);

/**
 * 加载图片线程
 * filename是要加载的背景文件
 */
int load_background(void *filename);

/**初始化背景函数*/
void init_background(void);

/**刷新光标函数*/
void refresh_pointer(void);

/**初始化光标函数*/
void init_pointer(void);

/**初始化任务栏函数*/
void init_taskbar(void);

/**设置活动窗口函数*/
void window_set_active(struct window *target);

/**取消活动窗口函数*/
void window_cancel_active(void);

/**window系统时钟刷新函数*/
void window_refresh_date(void);

/**Menu的处理函数*/
void window_menu_handle(unsigned int y);

/**绘制Menu函数*/
void window_show_menu(void);

/**点击Application处理程序*/
void window_click_Application(void);

/**绘制Application*/
void window_draw_Application(unsigned int backcolor, unsigned int fontcolor);

#endif
