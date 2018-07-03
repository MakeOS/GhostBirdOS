/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 8254 head
 * Explorer 0.01/arch/x86/include/i8254.h
 * version:1.1
 * 8/27/2014 12:31 PM
 */

#ifndef PIT_H_
#define PIT_H_

/**PIT的端口定义*/
#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040


/**Intel 8253\8254芯片晶振频率*/
#define CLOCK_TICK_RATE 1193180


/**设置PIT工作在1000HZ频率*/
#define PIT_HZ		1000
#define LATCH 		((CLOCK_TICK_RATE + PIT_HZ / 2) / PIT_HZ)

/**定时器任务结构体*/
struct timer
{
	unsigned long time_size;					//时间长度
	unsigned long time;							//定时
	unsigned char state;						//状态
	void (*function)(void);						//执行函数指针
	struct timer *next;							//组成单向链表
};

typedef struct timer* timer;

/**初始化可编程中断定时器函数*/
void init_PIT(void);

/**时钟中断处理函数*/
void PIT_handle(void);

/**设置定时器函数*/
struct timer *settimer(void (*function)(void), unsigned long time, unsigned char state);

#endif