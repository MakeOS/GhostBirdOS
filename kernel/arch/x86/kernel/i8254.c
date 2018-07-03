/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer intel 8254 support
 * Explorer/arch/x86/kernel/i8254.c
 * version:1.2
 * 7/26/2014 5:26 PM
 */

#include<stdlib.h>
#include<task.h>
#include<types.h>
#include "time.h"
#include "cmos.h"
#include "../include/i8254.h"

/**
 * Intel 8254\8253芯片(Programmable Interrupt Timer，PIT)
 * 整体介绍：Intel 8254需要维护系统运行时间和定时任务队列
 * 系统运营时间以微秒计时，整个系统可以以此为基准进行定时
 * 系统运营时间变量：system_runtime
 *
 * 定时任务队列：内核程序可以调用settimer()函数(具体使用方法看函数注释)
 * 设定一个定时执行函数任务。有定时只执行一次和定时执行多次两种选择
 
 * 任务队列中有多个需要定时执行具体函数的任务表项，为了维护
 * 方便，任务队列需要有动态增长的能力，且需要最大效率的执行
 * 所以任务队列被做成链表的形式，按唤醒时间从小到大排列
 * 当定时器中断发生后，需要对任务队列判断，如果发现了需要被执行
 * 的表项，就执行并且判断定时属性，如果是定时只执行一次就将其
 * 从链表中删除，如果是定时执行多次，就修改到合适的位置(重新加入链表)
 
 * intel 8254相关的编程资料暂无提供。
 */

/**指向任务结构链表开始处，如果是NULL，就说明链表不存在*/
static struct timer *current_task = NULL;

/**
 * 初始化intel 8254芯片函数
 */
void init_PIT(void)
{	
	/**向Intel 8259A注册Intel 8254*/
	register_PIC(0, &PIT_handle, "intel 8254");
	
	/**设定频率*/
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, LATCH & 0xff);
	io_out8(PIT_CNT0, LATCH >> 8);
	
	/**正常返回*/
	return;
}

/**BCD码转换成16进制方法*/
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf)

/**
 * intel 8254中断执行函数
 * 在这个函数中包含了定时任务的执行、时间的刷新、任务的切换
 */
void PIT_handle(void)
{	
	
	/**刷新时间*/
	system_runtime ++;			/**系统运行时间*/
	
	/**判断当前是不是999微妙*/
	if (ms == 999)
	{
		/**微秒归零*/
		ms = 0;
		
		/**判断秒表*/
		if (sec == 59)
		{
			/**秒归零*/
			sec = 0;
			
			/**判断分*/
			if (min == 59)
			{
				/**分归零*/
				min = 0;
				
				/**剩下的暂时不做判断，直接从CMOS中读取*/
				hour = BCD_HEX(read_cmos(CMOS_CUR_HOUR));		/**当前时*/
				week_day = BCD_HEX(read_cmos(CMOS_WEEK_DAY));	/**一周中当前天*/
				day = BCD_HEX(read_cmos(CMOS_MON_DAY));			/**一月中当前日*/
				mon = BCD_HEX(read_cmos(CMOS_CUR_MON));			/**当前月*/
				year = BCD_HEX(read_cmos(CMOS_CUR_YEAR));		/**当前年*/
				century = BCD_HEX(read_cmos(CMOS_CUR_CEN));		/**当前世纪*/
				/**CMOS中的年份仅仅是从本世纪开始到现在的绝对值，需要加上世纪*/
				year += century * 100;
			}else{
				min ++;
			}
		}else{
			sec ++;
		}
	}else{
		ms ++;					/**微秒计时*/
	}
	
/**执行定时任务*/
	/**判断此时是否有任务链表*/
	if (current_task != NULL)
	{
		/**执行定时任务*/
		struct timer *ptr;
		ptr = current_task;
		
		/**循环判断是否有任务到时需要执行*/
		while((ptr != NULL) && (ptr->time <= system_runtime))
		{
			/**执行定时任务*/
			(*ptr->function)();
			
			/**如果是定时执行多次的定时器任务，就继续插入链表中*/
			if (ptr->state == 0)
			{
				settimer((*ptr).function, (*ptr).time_size, (*ptr).state);
			}
			
			/**如果不是多次执行任务，就从链表中移除*/
			current_task = ptr->next;
			vfree(ptr);
			ptr = ptr->next;
		}
	}
	
	/**EOI，允许8259A继续接收中断*/
	EOI();
	
/**任务切换*/
	current->info.runtime ++;		/**运行时间加一*/
	
	/**判断该任务的时间片是否使用完，使用完了则选择新任务调度*/
	if ((*current).info.counter == 0)
	{
		schedule();
	}else{
		(*current).info.counter --;
	}
	
	/**返回*/
	return;
}

/**
 * 设置定时器函数
 * 其中void (*function)(void)为函数型指针(指向需要执行的地方)
 * time为定时间隔，如果是定时只执行一次，就会在time微秒后执行该函数
 * 如果是定时执行多次，就会在time微秒后执行该函数，而且该函数每隔time微妙后会再次执行
 * state为定时属性，有0(定时执行多次)和其它值(定时只执行一次)
 */
struct timer *settimer(void (*function)(void), unsigned long time, unsigned char state)
{	
	struct timer *retval, *ptr;
	
	/**参数检查*/
	if (time == 0) return NULL;
	
	/**创建新的定时任务结构体*/
	retval = (struct timer *) vmalloc(sizeof(struct timer));
	if (retval == NULL) return NULL;
	
	/**对定时任务结构体赋值*/
	retval->time_size = time;
	retval->time = time + system_runtime;
	retval->state = state;
	retval->function = function;

	/**判断链表是否为空*/
	if (current_task == NULL)
	{
		/**直接作为唯一表项*/
		current_task = retval;
		retval->next = NULL;
		
	/**判断链表中是否就只有一个表项*/
	}else if (current_task->next == NULL)
	{
		/**如果新定时任务的时间比该表项小，说明新任务要比该任务执行时间早*/
		if (retval->time < current_task->time)
		{
			/**加入前面*/
			retval->next = current_task;
			current_task = retval;
		/**新任务比该任务执行时间晚*/
		}else{
			/**加入后面*/
			current_task->next = retval;
			retval->next = NULL;
		}
	/**有不止一个表项*/
	}else{
		/**找到最优位置*/
		for (ptr = current_task; ; ptr = ptr->next)
		{
			/**如果该表项已经是最后一个表项，就退出寻找*/
			if (ptr->next == NULL) break;
			
			/**如果该表项是在新表项之后执行，就退出寻找*/
			if (ptr->next->time > retval->time) break;/**可能存在问题*/
		}
		
		/**加入单向链表中*/
		retval->next = ptr->next;
		ptr->next = retval;
	}
}
