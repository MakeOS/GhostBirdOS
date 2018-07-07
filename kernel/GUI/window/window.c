/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Initialization
 * Explorer/GUI/window.c
 * version:Alpha
 * 8/16/2014 9:08 AM
 */

#include <lib/fonts/font.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <GUI.h>
#include <memory.h>
#include <arch.h>
#include <lib/graphics.h>
#include "window.h"
#include "unit.h"
#include "../recourse.h"
#include "../layer.h"
#include <mouse.h>
#include <task.h>

/**
 * 图层顺序：
 * 根图层位于最顶端
 * 光标图层位于次顶端
 * 附加图层位于次次顶端
 * 任务栏图层位于次次次顶端
 * 最新创建的窗口位于其他窗口顶部，任务栏图层的底部
 * 任何窗口都在这里
 * 背景图层位于最底端
 */

/**附加图层*/
struct layer *attach_layer = NULL;
 
/**背景图层和光标图层*/
struct layer *background_layer = NULL, *pointer_layer = NULL, *taskbar_layer = NULL;

/**测试窗口*/
struct window *new_window_2;

/**拖动窗口描述体*/
struct window *drag_window = NULL;
unsigned long drag_x, drag_y;

union task *task_GUI_mouse_handle;

/**鼠标光标点阵图转化成Explorer GUI 内部抽象图片结构*/
struct GUI_image pointer_recourse = 
{
	.length = 12,
	.width = 20,
	.data = Pointer_Bitmap,
	.count = 1,
	.next = NULL
};

/**窗口相关资源*/
struct GUI_image *close_f_botton, *close_p_botton, *mini_f_botton, *mini_p_botton;

/**鼠标消息的处理函数*/
bool mouse_press = false;
int GUI_mouse_handle(void *arg)
{
	unsigned char cmd;
	struct layer *layer;
	struct layer_unit *unit_ptr;
	
/**判断信息性质*/
cmp_info:
	cmd = get_mouse_cmd(MOUSE_TRY);
	switch(cmd)
	{
		/**0x09是鼠标左键按下命令*/
		case 0x9: goto press;
		
		/**0x08是鼠标释放(任意键释放都是0x08)函数*/
		case 0x8: goto release;
		
		/**无效命令返回*/
		case 0xff: sleep();
		
		/**其他命令忽略*/
		default: goto cmp_info;
	}

/**释放按键处理方法*/
release:
	/**标记鼠标未被按下*/
	mouse_press = false;
	
	/**拖拽参数清空*/
	drag_window = NULL;
	
	/**释放按键处理结束，正常返回*/
	goto cmp_info;
	
/**按下左键处理方法*/
press:
	/**判断是否已经按下，如果已经按下，则该消息无效*/
	if (mouse_press == true) goto cmp_info;;
	
	/**标记已经按下*/
	mouse_press = true;
	
	/**判断附加图层是否显示*/
	if (attach_layer->visiable == true)
	{
		/**判断鼠标是否点击到附加图层区域*/
		if ((mouse_x >= attach_layer->x) & (mouse_x < attach_layer->x + attach_layer->length) & (mouse_y >= attach_layer->y) & (mouse_y < attach_layer->y + attach_layer->width))
		{
			/**判断是否是Application在工作*/
			if (Application_work_flag == true)
			{
				window_menu_handle(mouse_y - attach_layer->y);
			}
			/**继续处理命令*/
			goto cmp_info;
		}
	}
	
	/**判断是否在任务栏区域*/
	if (mouse_y >= (Video_Info.yres - 20))
	{
	/**判断鼠标点击区域*/
	
		/**鼠标点击到了Application*/
		if (mouse_x < window_Application_length)
		{
			window_click_Application();
		/**鼠标点击到了Data*/
		}else if (mouse_x > (Video_Info.xres - window_Date_length))
		{
			
		}
		
		/**设置当前活动窗口为非活动窗口*/
		window_cancel_active();
		
		/**继续判断信息*/
		goto cmp_info;;
	}
	
	/**从最顶层窗口开始向下搜索*/
	for (layer = taskbar_layer->bottom; layer->winptr != NULL; layer = layer->bottom)
	{
		/**进行区域判断*/
		if ((mouse_x >= layer->x) & (mouse_x < layer->x + layer->length) & (mouse_y >= layer->y) & (mouse_y < layer->y + layer->width))
		{
			/**设置被点击的窗口为活动窗口*/
			window_set_active(layer->winptr);
			
			/**判断被点击的窗口是否是带边框窗口*/
			if (layer->winptr->style == WINDOW_NORMAL)
			{
				/**判断被点击的区域是否在窗口上*/
				if (mouse_y - layer->y < WINDOW_NORMAL_HEADER_WIDTH)
				{
					/**记录拖拽参数*/
					drag_window = layer->winptr;
					drag_x = layer->x - mouse_x;
					drag_y = layer->y - mouse_y;
				}
			}
			
			/**判断是否有单元*/
			if (layer->layer_unit_list != NULL)
			{
				/**循环判断是否处于单元范围内*/
				for (unit_ptr = layer->layer_unit_list; unit_ptr != NULL; unit_ptr = unit_ptr->next)
				{
					if ((mouse_x >= (layer->x + unit_ptr->x)) & (mouse_x < (layer->x + unit_ptr->x + unit_ptr->length)) & (mouse_y >= (layer->y + unit_ptr->y)) & (mouse_y < (layer->y + unit_ptr->y + unit_ptr->width)))
					{
						/**调用单元处理函数*/
						unit_handle[unit_ptr->type](layer, mouse_x - (layer->x + unit_ptr->x), mouse_y - (layer->y + unit_ptr->y), unit_ptr->ptr);
						
						/**退出循环*/
						break;
					}
				}
			}
			/**输出信息到相应窗口*/
			// window_print(layer->winptr, "Clicked in (%d,%d):%#X.", mouse_x - layer->x, mouse_y - layer->y, cmd);
			
			/**退出循环*/
			goto cmp_info;
		}
	}
	
	/**取消活动窗口*/
	window_cancel_active();
	goto cmp_info;
}

/**初始化窗口系统*/
void init_Window(void)
{
	/**初始化背景*/
	init_background();
	
	/**初始化光标*/
	init_pointer();
	// fin:goto fin;
	/**初始化附加图层*/
	init_attach_layer();
	
	/**初始化任务栏*/
	init_taskbar();
	
	task_GUI_mouse_handle = new_task(&GUI_mouse_handle, NULL);
	
	/**注册鼠标侦听*/
	set_mouse_interception(task_GUI_mouse_handle);
	
	/**加载相关资源*/
	close_f_botton = window_load_image("CLOSE_F_.BMP");
	close_p_botton = window_load_image("CLOSE_P_.BMP");
	mini_f_botton = window_load_image("MINI_F_.BMP");
	mini_p_botton = window_load_image("MINI_P_.BMP");
	
	/**GUI控制台窗口*/
	GUI_control = GUI_window("Explorer GUI Control", WINDOW_NORMAL, 0, 0, 480, 320);
	
	/**加载背景*/
	new_task(&load_background, "WALLPAP2.BMP");
	
	/**窗口测试*/
	new_window_2 = GUI_window("Ghost Bird OS", WINDOW_NORMAL, 0, 0, 300, 300);
	window_string(new_window_2, 0, 50, "Explorer 0.84 Window system testing...");
	unit_new_botton(new_window_2, 125, 200, 60, 25, "botton1");
}

/**初始化附加图层函数*/
void init_attach_layer(void)
{
	/**创建附加图层*/
	for (attach_layer = NULL; attach_layer == NULL;)
		attach_layer = GUI_new_layer(0, 0, 8, 8);
	
	/**将该附加图层隐藏起来*/
	attach_layer->visiable = false;
	
	/**将附加图层从图层链表中脱离出来**/
	attach_layer->top->bottom = attach_layer->bottom;
	attach_layer->bottom->top = attach_layer->top;
	
	/**设定好光标的层级位于光标图层之下其他图层之上*/
	attach_layer->top = pointer_layer;
	attach_layer->bottom = pointer_layer->bottom;
	pointer_layer->bottom = attach_layer;
	attach_layer->bottom->top = attach_layer;
}

/**背景资源*/
struct GUI_image *wallpaper;

/**加载图片线程
 * filename是要加载的背景文件
 */
int load_background(void *filename)
{
	/**加载图片*/
	wallpaper = window_load_image(filename);
	
	/**映射图片*/
	GUI_map(background_layer, wallpaper, 0, 0, 0, 0, 0);
}


/**初始化背景函数*/
void init_background(void)
{
	/*initialization picture layer*/
	for (background_layer = NULL; background_layer == NULL;)
		background_layer = GUI_new_layer(0, 0, Video_Info.xres, Video_Info.yres);
	
	/**相关参数设置*/
	background_layer->visiable = true;

	/**填充灰色*/
	GUI_put_square(background_layer, 0xffa0a0a0, 0, 0, Video_Info.xres, Video_Info.yres);
	//GUI_put_square(background_layer, 0x80ffffff, 0, 0, 800, 600);
	
	/**加载图片*/
	// new_task(&load_background, "BACKDROP.PIC");
}

/**软系数，用来确定软鼠标的跟随灵敏度
 * 大于等于1，越接近1则越硬(整数)
 * 1代表不用该功能
 */
#define MOUSE_SOFT_INDEX	1

/**刷新光标函数*/
void refresh_pointer(void)
{
		/**判断鼠标的横坐标是否超过了边界*/
		mouse_x = get_mouse_x();
		if (mouse_x < 0)
		{
			mouse_x = (0);
			put_mouse_x(mouse_x);
		}else if (mouse_x >= Video_Info.xres)
		{
			mouse_x = (Video_Info.xres - 1);
			put_mouse_x(mouse_x);
		}
		
		/**判断鼠标的纵坐标是否超过了边界*/
		mouse_y = get_mouse_y();
		if (mouse_y < 0)
		{
			mouse_y = (0);
			put_mouse_y(mouse_y);
		}else if (mouse_y >= Video_Info.yres)
		{
			mouse_y = (Video_Info.yres - 1);
			put_mouse_y(mouse_y);
		}

		/**判断是否在拖拽状态*/
		if (drag_window != NULL){
			/**设置窗口坐标，因为窗口通常大于鼠标，所以窗口先移动，再移动鼠标，得出最好效果*/
			GUI_set_position(drag_window->layer, drag_x + mouse_x, drag_y + mouse_y);
		}
		
		/**设置鼠标的坐标*/
		GUI_set_position(pointer_layer,
			(((mouse_x - pointer_layer->x) / MOUSE_SOFT_INDEX) + pointer_layer->x),
			(((mouse_y - pointer_layer->y) / MOUSE_SOFT_INDEX) + pointer_layer->y));
}

/**初始化光标函数*/
void init_pointer(void)
{
	/**创建个新图层*/
	for (pointer_layer = NULL; pointer_layer == NULL;)
		pointer_layer = GUI_new_layer((Video_Info.xres / 2), (Video_Info.yres / 2), 12, 20);
	
	/**从图层链表中脱离出来*/
	pointer_layer->top->bottom = pointer_layer->bottom;
	pointer_layer->bottom->top = pointer_layer->top;
	
	/**设定好光标的层级位于根图层之下其他图层之上*/
	pointer_layer->bottom = layer_root->bottom;
	pointer_layer->top = layer_root;
	layer_root->bottom = pointer_layer;
	pointer_layer->bottom->top = pointer_layer;
	
	/**将光标点阵图映射到图层上*/
	GUI_map(pointer_layer, &pointer_recourse, 0, 0, pointer_layer->length, pointer_layer->width, 0);
	
	/**使图层可以看到*/
	pointer_layer->visiable = true;
	
	/**设置刷新*/
	settimer(&refresh_pointer, 1000 / 50, 0);
}

/**
 * 初始化任务栏函数
 * 时钟格式：10:49 2/4/2015
 */
void init_taskbar(void)
{
	/**初始化任务栏图层*/
	for (taskbar_layer = NULL; taskbar_layer == NULL;)
		taskbar_layer = GUI_new_layer(0, Video_Info.yres - 20, Video_Info.xres, 20);
	
	/**将任务栏图层从图层链表中脱离出来*/
	taskbar_layer->top->bottom = taskbar_layer->bottom;
	taskbar_layer->bottom->top = taskbar_layer->top;
	
	/**将任务栏置于光标图层之下，其他图层之上*/
	taskbar_layer->top = attach_layer;
	taskbar_layer->bottom = attach_layer->bottom;
	attach_layer->bottom = taskbar_layer;
	taskbar_layer->bottom->top = taskbar_layer;
	
	/**使任务栏可见*/
	taskbar_layer->visiable = true;
	
	
	/**绘制任务栏*/
	GUI_put_square(taskbar_layer, TASKBAR_COLOR, 0, 0, Video_Info.xres, 20);			/**任务区*/
	GUI_line(taskbar_layer, TASKBAR_LINE_COLOR, 0, 0, Video_Info.xres, 0);				/**边际线*/
	
	/**设定定时器定时刷新时钟*/
	settimer(&window_refresh_date, 500, 0);
	
	/**显示时钟*/
	window_refresh_date();
	
	/**显示Application*/
	window_draw_Application(TASKBAR_COLOR, 0xff000000);
}

/**绘制窗体边框函数*/
void window_draw(struct window *target, unsigned int title_color, unsigned int frame_color)
{
	/**如果该窗体的风格不是带边框风格，直接返回*/
	if (target->style != WINDOW_NORMAL) return;
	
	/**绘制窗体边框*/
	GUI_put_square(target->layer, frame_color, WINDOW_NORMAL_FRAME_WIDTH, 0, target->length - (close_f_botton->length + mini_f_botton->length), WINDOW_NORMAL_HEADER_WIDTH);
	// GUI_put_square(target->layer, frame_color, WINDOW_NORMAL_FRAME_WIDTH, 0, target->length, WINDOW_NORMAL_HEADER_WIDTH);
		
	/**左右边框*/
	GUI_put_square(target->layer, frame_color, 0, 0, WINDOW_NORMAL_FRAME_WIDTH, target->layer->width);
	GUI_put_square(target->layer, frame_color, target->layer->length - WINDOW_NORMAL_FRAME_WIDTH, 0, WINDOW_NORMAL_FRAME_WIDTH, target->layer->width);
		
	/**下边框*/
	GUI_put_square(target->layer, frame_color, WINDOW_NORMAL_FRAME_WIDTH, target->layer->width - WINDOW_NORMAL_FRAME_WIDTH, target->length, WINDOW_NORMAL_FRAME_WIDTH);
		
	/**窗体标题*/
	GUI_put_string(target->layer, title_color, 5, 8, 0, 0, font("simsun"), target->title);
}

/**活动窗口指针*/
static struct window *window_active = NULL;

/**设置活动窗口函数*/
void window_set_active(struct window *target)
{
	/**如果活动窗口就是当前窗口，直接返回*/
	if (window_active == target) return;

	/**判断是否已经有活动窗口
	 * 如果当前还没有活动窗口，就省略这个步骤
	 */
	if (window_active != NULL)
	{		
		/**有则需要将当前活动窗口设置为非活动窗口*/
		window_draw(window_active, WINDOW_INACTIVE_TITLE_COLOR, WINDOW_INACTIVE_FRAME_COLOR);
	}
		
		/**设置新窗口为活动窗口*/
		window_active = target;
		
		/**判断新窗口是否位于所有窗口之上(上一个图层是任务栏)*/
		if (target->layer->top != taskbar_layer)
		{
			/**将新窗口从图层链表中剥离出来*/
			target->layer->top->bottom = target->layer->bottom;
			target->layer->bottom->top = target->layer->top;
			
			/**将新窗口移动到所有窗口之上*/
			target->layer->top = taskbar_layer;
			target->layer->bottom = taskbar_layer->bottom;
			taskbar_layer->bottom = target->layer;
			target->layer->bottom->top = target->layer;
			
			/**这里没有依据风格判断实际上有效的部分，在后期需要改正*/
			GUI_refresh_block(target->layer->x, target->layer->y, target->layer->length, target->layer->width);
		}
		
		/**将新窗口绘制成活动窗口*/
		window_draw(target, WINDOW_ACTIVE_TITLE_COLOR, WINDOW_ACTIVE_FRAME_COLOR);
}

/**取消活动窗口函数*/
void window_cancel_active(void)
{
	/**首先判断是否有活动窗口*/
	if (window_active == NULL) return;
	
	/**绘制成非活动窗口*/
	window_draw(window_active, WINDOW_INACTIVE_TITLE_COLOR, WINDOW_INACTIVE_FRAME_COLOR);
	
	/**指针清空*/
	window_active = NULL;
	
	/**正常返回*/
	return;
}
