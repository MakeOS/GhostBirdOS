/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer task system
 * Explorer/arch/x86/kernel/task/task.c
 * version:Alpha
 * 10/4/2014 10:00 AM
 */

#include <memory.h>
#include <lib/mem.h>
#include <lib/string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <spinlock.h>
#include <task.h>

/**
 * Explorer 内核进程管理
 * Lab Explorer Developers<2322869088@qq.com>
 * 采用多级优先反馈队列算法
 */

/**允许调度标志*/
static bool schedule_flag = false;

unsigned long temp_stack;/*临时堆栈指针存放变量*/


/**队列：实时、高、正常、低优先、僵尸、等待，
 * 可以使用多级反馈调度算法。
 */
static union task *realtime, *high, *normal, *low, *zomble, *wait;

/**任务0的联合体定义*/
union task task_0;

/**初始化任务管理*/
void init_task(void)
{
	struct process_struct *process_0;
	
	/**对0任务的数据结构进行赋值*/
	task_0.info.counter = 60;		/**给0任务分配50微秒的时间片*/
	task_0.info.time_limit = 60;	/**0任务时间片长度为50微秒*/
	task_0.info.runtime = 0;		/**0任务总共运行时间归0*/
	task_0.info.state = TASK_HIGH;	/**设置0任务为高优先级任务*/
	
	/**将0任务做成双向循环链表*/
	task_0.info.next = &task_0;
	task_0.info.prev = &task_0;
	
	/**当前任务指向0任务*/
	current = &task_0;
	
	/**分配进程0结构体*/
	for (process_0 = NULL; process_0 == NULL; )
		process_0 = kmalloc(sizeof(struct process_struct), 0);
	
	/**清空进程0结构体*/
	memset(process_0, 0, sizeof(struct process_struct));
	
	/**将0任务升级成0进程*/
	task_0.info.pptr = process_0;
	
	/**0进程赋值*/
	process_0->cr3 = read_CR3();
	process_0->msg_list = NULL;
	process_0->nthread ++;
	
	schedule_flag = true;
}


/**创建内核级线程函数*/
union task* new_task(int (*function)(), void *argument)
{
	/**为新任务的数据结构分配空间*/
	union task *new_task;
	new_task = vmalloc(sizeof(union task));
	if (new_task == NULL) return 0;
	memset(new_task, 0, sizeof(union task));
	
	/**对新任务的数据结构进行赋值*/
	new_task->info.father = current;			/**设置父任务指针*/
	new_task->info.time_limit = 60;				/**时间片*/
	new_task->info.counter = 0;					/**创建时不分配时间片*/
	new_task->info.runtime = 0;					/**新任务总共执行时间归0*/
	new_task->info.state = TASK_HIGH;			/**设置新任务为高优先级任务*/
	new_task->info.pptr = current->info.pptr;	/**新任务同样是本进程的一个线程*/
	new_task->info.pptr->nthread ++;			/**本进程的线程数量加一*/
	
	/**对新任务的堆栈进行初始化*/
	new_task->info.stack = Init_Kernel_Task(((unsigned long)new_task + TASK_SIZE), function, argument);
	
	/**将新任务插入到任务列表中*/
	current->info.next->info.prev = new_task;
	new_task->info.prev = current;
	new_task->info.next = (*current).info.next;
	current->info.next = new_task;
	
	/**返回*/
	return new_task;
}

/**执行应用程序函数（非POSIX标准）*/
int run_exec(void *arg)
{
	struct GBOS_head *head;
	struct exec_file_para *file_para = arg;
	struct process_struct *new_process;
	/**停止调度*/
	disable_schedule();
	
	/**分配进程结构体*/
	for (new_process = NULL; new_process == NULL; )
		new_process = kmalloc(sizeof(struct process_struct), 0);
	
	/**清空进程结构体*/
	memset(new_process, 0, sizeof(struct process_struct));
	
	/**将当前任务与当前进程脱钩*/
	current->info.pptr->nthread --;
	
	/**将任务升级成进程*/
	current->info.pptr = new_process;
	
	/**进程赋值*/
	new_process->cr3 = new_pdt();
	new_process->msg_list = NULL;
	new_process->nthread ++;

	/**切换页目录表*/
	write_CR3(new_process->cr3);
	
	/**允许调度*/
	enable_schedule();
	
	/**加载文件*/
	file_open(file_para->filename, (void *)0x10000000);
	
	/**回收结构体*/
	kfree(arg);
	
	/**对准文件头*/
	head = (void *)0x10000000;
	
	/**检查是否是有效应用程序*/
	if (head->flag[0] != 'G' |
		head->flag[1] != 'B' |
		head->flag[2] != 'O' |
		head->flag[3] != 'S' )
	{
		/**失败返回*/
		return 0;
	}
	
	/**执行应用程序*/
	head->entry();
}

/**运行任务函数*/
union task* run(char *filename, char *para, int flag)
{
	struct exec_file_para *new_arg;
	
	/**至少filename字符串指针不能为NULL*/
	if (filename == NULL) return NULL;
	
	/**分配一个exec_file_para结构体装载执行run_exec线程的参数*/
	for (new_arg = NULL; new_arg == NULL; )
		new_arg = kmalloc(sizeof(struct exec_file_para), 0);
	
	/**清空exec_file_para结构体防止造成干扰*/
	memset(new_arg, 0, sizeof(struct exec_file_para));
	
	/**拷贝文件名信息进入exec_file_para中*/
	strncpy(new_arg->filename, filename, 256);
	
	/**如果有参数则同时拷贝参数*/
	if (new_arg->para != NULL)
		strncpy(new_arg->para, para, 256);
	
	/**创建新线程运行run_exec*/
	new_task(&run_exec, new_arg);
}

/**获得任务的id*/
union task* get_id(void)
{
	return current;
}

/**获得父任务的id*/
union task* get_pid(void)
{
	return current->info.father;;
}


/**退出任务*/
void exit(int code)
{
	/**指向自身的结构体*/
	union task *my = current;
	
	/**将自己从任务链表中剥离*/
	my->info.next->info.prev = my->info.prev;
	my->info.prev->info.next = my->info.next;
	
	/**启动调度函数*/
	schedule();
	
finish:
	goto finish;
}

/**允许调度函数*/
void enable_schedule(void)
{
	schedule_flag = true;
}

/**禁止调度函数*/
void disable_schedule(void)
{
	schedule_flag = false;
}

/**切换到目标任务*/
extern void switch_to(union task* id);

/**任务切换函数，使用算法选择下个任务并加载*/
void schedule(void)
{
	union task *new_task;
	
	/**判断是否允许调度*/
	if (schedule_flag == false) return;
	
	/**选择下一个要运行的进程*/
	new_task = current->info.next;
		
	/**分配时间片*/
	new_task->info.counter = new_task->info.time_limit;
	
	
	/**切换页目录表*/
	write_CR3(new_task->info.pptr->cr3);
	
	/**切换至新任务*/
	switch_to(new_task);
}


/**任务挂起函数*/
void sleep(void)
{
	/**脱离链表*/
	current->info.next->info.prev = current->info.prev;
	current->info.prev->info.next = current->info.next;
	
	/**设置睡眠属性*/
	current->info.state = TASK_SLEEP;
	
	/**调度任务*/
	schedule();
}

/**任务唤醒函数*/
void wakeup(union task* target)
{
	/**判断该任务是否已经挂起*/
	if (target->info.state == TASK_SLEEP)
	{
		/**刚刚唤醒的任务加入正常队列*/
		target->info.state = TASK_HIGH;
		
		/**加入队列中*/
		target->info.prev = current;
		target->info.next = current->info.next;
		current->info.next = target;
		target->info.next->info.prev = target;
	}
	
	/**正常返回*/
	return;
}