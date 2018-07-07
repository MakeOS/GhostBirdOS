/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer PS/2 Mouse Support
 * Explorer/drivers/mouse.c
 * version:Alpha
 * 8/26/2014 10:30 AM
 */

#include <mouse.h>
#include <keyboard.h>
#include <arch.h>
#include <lib/graphics.h>
#include <GUI.h>
#include "i8042.h"

/** A FIFO buffer for mouse*/
static char mouse_buffer[SIZE_OF_BUFFER_MOUSE];
static unsigned long untreated = 0;
static unsigned long write_point = 0, read_point = 0;

/**description of mouse x&y*/
unsigned char mouse_cmd;

/**save mouse info temporary*/
static char mouse_info[3];
static unsigned long mouse_info_point;

/**intercept task*/
union task *mouse_intercept;

void init_mouse(void)
{
	/**initialize mouse_info_point zero*/
	mouse_info_point = 0;
	
	/**set the location of mouse in centre of screen*/
	mouse_x = (Video_Info.xres / 2);
	mouse_y = (Video_Info.yres / 2);
	
	/**register to 8259*/
	register_PIC(12, &int_mouse_handle, "Mouse");
	
	/**allow mouse send info to CPU*/
	cmd_mouse(0xF4);
	/**open mouse*/
	enable_mouse();
}

void put_mouse_x(long x)
{
	mouse_x = x;
}

long get_mouse_x(void)
{
	return mouse_x;
}

void put_mouse_y(long y)
{
	mouse_y = y;
}

long get_mouse_y(void)
{
	return mouse_y;
}

/**鼠标命令读取函数
 * flag为函数执行属性，属性定义在mouse.h中
 * MOUSE_TRY:试探获得，如果当前缓冲区为空，则立马返回，如果不为空，则取出数据
 * MOUSE_GET:直接获得，如果当前缓冲区为空，一直忙等待数据
 * 返回值：0xff则执行无效，其他值则为鼠标命令。
 */
unsigned char get_mouse_cmd(int flag)
{
	/**返回值*/
	unsigned char retval;
	
	/**如果当前没数据，try类型的请求直接返回*/
	if ((flag == MOUSE_TRY) & (untreated == 0)) return 0xff;
	
/**如果非try请求，或者try请求且有数据的情况，响应*/
handle:

	/**忙等待有数据*/
	for (; untreated == 0;);
	
	/**运行到这里一定有数据*/
	retval = mouse_buffer[read_point];
	
	/**缓冲区更新*/
	untreated --;
	read_point ++;
	
	/**判断指针是否指向缓冲之外*/
	if (read_point >= SIZE_OF_BUFFER_MOUSE) read_point = 0;
	
	/**正常返回*/
	return retval;
}

/**注册鼠标侦听任务函数*/
void set_mouse_interception(union task *target)
{
	mouse_intercept = target;
}

/**鼠标数据处理函数*/
void int_mouse_handle(void)
{
	mouse_info[mouse_info_point] = read_8042();
	if (mouse_info_point == 2)
	{
		
		// printk("mouse:(%d, %d, %d)\n", mouse_info[0], mouse_info[1], mouse_info[2]);
		
		/**重新指向数组开始处*/
		mouse_info_point = 0;
		
		/**处理信息*/
		mouse_cmd = mouse_info[0];
		mouse_x += mouse_info[1];
		mouse_y += -(mouse_info[2]);
		
		/**判断此时缓冲区是否已满*/
		if (untreated < SIZE_OF_BUFFER_MOUSE)
		{
			/**将指针指向下一个缓存*/
			write_point ++;
			
			/**判断指针有没有超过缓存区域，如果超过则返回开始处*/
			if (write_point >= SIZE_OF_BUFFER_MOUSE) write_point = 0;
			
			/**将命令放入缓存中*/
			mouse_buffer[write_point] = mouse_info[0];
			
			/**增加缓冲区数据数量计数*/
			untreated ++;
			
			/**唤醒侦听任务*/
			if (mouse_intercept != NULL)
				wakeup(mouse_intercept);
		}
	}else{
		mouse_info_point ++;
	}
	EOI();
}