/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Task Bar
 * Explorer/GUI/window/task_bar.c
 * version:Alpha
 * 2/5/2015 1:37 PM
 */

#include <GUI.h>
#include <time.h>
#include <studio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lib/graphics.h>
#include "window.h"
#include "../layer.h"

/**任务栏的规划*/
/**Application	Runing_window	Date*/

/**Application区域长度*/
unsigned long window_Application_length = 11 * 8 + 8;

/**Date区域长度*/
unsigned long window_Date_length = 15 * 8;

/**window系统时钟刷新函数*/
void window_refresh_date(void)
{
	/**时间字符数据缓冲区*/
	char date_buf[15];
	
	/**重绘时钟区域*/
	GUI_put_square(taskbar_layer, TASKBAR_COLOR, (Video_Info.xres - window_Date_length), 1, window_Date_length, TASKBAR_WIDTH - 1);			/**Date region*/
	
	/**字符串格式转换*/
	if (ms <= 500)					// 这里根据当前的微秒判断，以0.5秒为界限，小于0.5秒显示":"，大于0.5秒显示" "，可以达到一秒钟闪烁一次的效果
	{
		sprintf(date_buf, "%d:%02d %d/%d/%d", hour, min, mon, day, year);
	}else{
		sprintf(date_buf, "%d %02d %d/%d/%d", hour, min, mon, day, year);
	}
	
	/**输出时间*/
	GUI_put_string(taskbar_layer, TIME_COLOR, (Video_Info.xres - window_Date_length), 4, 0, 0, font("Standard Font"), date_buf);
}

/**Menu的项描述体*/
struct window_menu_one
{
	char *text;						// 项的名称（用于显示）
	char *filename;					// 项相关的函数（用于执行）
};

/**Menu的项数组*/
#define WINDOW_NUM_MENU		4
struct window_menu_one window_menu_lib[WINDOW_NUM_MENU] =
{
	{"Application1", "PROGRAM1.BIN"},
	{"Application2", "PROGRAM2.BIN"},
	{"Application3", "PROGRAM3.BIN"},
	{"Application4", "PROGRAM4.BIN"}
};

/**Menu区域的颜色*/
#define WINDOW_MENU_COLOR		TASKBAR_COLOR
#define WINDOW_MENU_LINE_COLOR	TASKBAR_LINE_COLOR

/**Menu区域的长宽*/
#define WINDOW_MENU_LENGTH	150
#define WINDOW_MENU_WIDTH	WINDOW_NUM_MENU * 16

/**Menu的处理函数*/
void window_menu_handle(unsigned int y)
{
	window_print(GUI_control, "Clicked:%s", window_menu_lib[y / 16].text);
	run(window_menu_lib[y / 16].filename, NULL, 0);
}

/**绘制Menu函数*/
void window_show_menu(void)
{
		/**设置附加图层为显示状态*/
		attach_layer->visiable = true;
		
		/**设置附加图层的位置在任务栏的左上方*/
		GUI_set_position(attach_layer, 0, Video_Info.yres - (TASKBAR_WIDTH + WINDOW_MENU_WIDTH));
		
		/**设置附加图层的长宽*/
		GUI_set_resolution(attach_layer, WINDOW_MENU_LENGTH, WINDOW_MENU_WIDTH);
		
		/**填充矩形*/
		GUI_put_square(attach_layer, WINDOW_MENU_COLOR, 0, 1, WINDOW_MENU_LENGTH - 1, WINDOW_MENU_WIDTH - 2);
		
		/**绘制边际线*/
		GUI_line(attach_layer, WINDOW_MENU_LINE_COLOR, 0, 0, WINDOW_MENU_LENGTH, 0);
		GUI_line(attach_layer, WINDOW_MENU_LINE_COLOR, WINDOW_MENU_LENGTH - 1, 1, WINDOW_MENU_LENGTH - 1, WINDOW_MENU_WIDTH - 2);
		
		unsigned long y_count;
		
		/**绘制全菜单信息*/
		for (y_count = 0; y_count < WINDOW_NUM_MENU; y_count ++)
		{
			GUI_put_string(attach_layer, 0xff000000, 10, y_count * 16, 0, 0, font("Standard Font"), window_menu_lib[y_count].text);
		}
}

/**Application是否在工作的标志*/
bool Application_work_flag = false;

/**点击Application处理程序*/
void window_click_Application(void)
{
/**判断是否被点击过*/

	/**如果被点击过*/
	if (Application_work_flag == true)
	{
		/**标记为未被点击*/
		Application_work_flag = false;
		
		/**绘制正常状态的Application*/
		window_draw_Application(TASKBAR_COLOR, 0xff000000);
		
		/**Application隐藏起来*/
		attach_layer->visiable = false;
		GUI_refresh_block(attach_layer->x, attach_layer->y, attach_layer->length, attach_layer->width);
		
	/**如果未被点击过*/
	}else
	{
		/**标记为已经点击*/
		Application_work_flag = true;
		
		/**绘制工作状态的Application*/
		window_draw_Application(0xff0000ff, 0xffffffff);
		
		/**显示菜单*/
		window_show_menu();
	}
}

/**绘制Application*/
void window_draw_Application(unsigned int backcolor, unsigned int fontcolor)
{
	/**绘制Application区*/
	GUI_put_square(taskbar_layer, backcolor, 0, 1, window_Application_length, 19);
	
	/**写字*/
	GUI_put_string(taskbar_layer, fontcolor, 4, 4, 0, 0, font("Standard Font"), "Application");
}
