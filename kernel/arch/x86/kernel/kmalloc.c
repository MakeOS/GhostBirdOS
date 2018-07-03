/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer function kmalloc
 * Explorer/arch/x86/kernel/kmalloc.c
 * version:Alpha
 * 1/10/2014 6:00 PM:created
 * 10/1/2015 8:15 AM:重写内存分配
 */

#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include "kmalloc.h"

#define PAGE_SIZE	4096

#define MD_PER_PAGE	4096 / sizeof(struct Memory_Descriptor)


/**维护内存池
 * 注意：内存池中允许最大内存不得超过4096
 */
#define POOL_SIZE	9
static struct mem_pool mem_pool[POOL_SIZE] =
{
	{16  , 0, NULL},
	{32  , 0, NULL},
	{64  , 0, NULL},
	{128 , 0, NULL},
	{256 , 0, NULL},
	{512 , 0, NULL},
	{1024, 0, NULL},
	{2048, 0, NULL},
	{4096, 0, NULL}
};

/**空闲内存描述符表的队列*/
static struct Memory_Descriptor *empty = NULL;

/**准备更多空闲内存描述符表函数*/
static void prepare_MD(void)
{
	/**分配一个新页储存描述符*/
	struct Memory_Descriptor *MD = vmalloc(PAGE_SIZE);
	if (MD == NULL) error("fill pool error!");
	
	/**初始化该页*/
	unsigned i;
	for (i = 0; i < MD_PER_PAGE; i ++)
	{
		MD[i].next = &MD[i + 1];
	}
	
	/**将该页描述符加入到内存池中*/
	MD[i - 1].next = empty;
	empty = MD;
}

/**填充内存池函数*/
void fill_pool(unsigned long n)
{
	/**判断空闲内存描述符表是否空*/
	if (empty == NULL) prepare_MD();
	
	/**获得一个页*/
	void *new_page = vmalloc(PAGE_SIZE);
	
	/**判断是否获取成功*/
	if (new_page == NULL) error("No enough memory!");
	
	/**获取一个空闲内存描述符表*/
	struct Memory_Descriptor *new_MD = empty;
	empty = empty->next;
	
	/**获取相关信息*/
	size_t size = mem_pool[n].size;
	unsigned long number = PAGE_SIZE / size;
	
	/**初始化内存描述符表和这个页*/
	new_MD->page = new_page;
	new_MD->freeptr = new_page;
	new_MD->refcnt = 0;
	
	/**将内存描述符表加入到内存池中*/
	new_MD->next = mem_pool[n].next;
	mem_pool[n].next = new_MD;
	mem_pool[n].number = number;
}

/**内核小块内存分配函数
 * 参数:size - 请求的内存块长度
 * 返回值:NULL - 分配失败; !NULL - 分配成功，返回内存首地址
 */
void *kmalloc(size_t size, int flags)
{
	void *retval;
	
	/**不允许调度*/
	disable_schedule();
	
	/**内存描述符指针*/
	struct Memory_Descriptor *point;
	
	/**寻找合适大小的内存池*/
	unsigned long n;
	for (n = 0; n < POOL_SIZE; n ++)
	{
		if (mem_pool[n].size >= size)
		{
			/**执行到这里说明已经找到合适大小的内存池*/
			
			/**判断内存池中是否有足够的内存*/
			if (mem_pool[n].number == 0) fill_pool(n);
			
			/**获取可用内存*/
			retval = mem_pool[n].next->freeptr;
			mem_pool[n].next->refcnt ++;
			mem_pool[n].next->freeptr += mem_pool[n].size;
			mem_pool[n].number --;
			goto finish;
		}
	}
	/**运行到这里说明没有找到合适大小的内存池，
	 * 具体原因可能是参数size大于现有的最大内存池
	 * 可以分配的内存，这样的分配应当直接使用以页
	 * 为单位分配、回收的函数。
	 */
	error("argument is too long!");
	return NULL;
	
finish:
	/**已经完成了内存分配的处理*/
	/**允许调度*/
	enable_schedule();
	return retval;
}


/**内核小块内存释放函数
 * 参数:point - 指向要释放的内存首地址(之前kmalloc函数的返回值)
 */
void kfree(void *point)
{
	void *page;
	unsigned long n;
	struct Memory_Descriptor *MD, *prev = NULL;			/**指向当前描述符和上一个描述符*/
	
	/**计算获得该块内存所在的页面*/
	page = (void *)((unsigned long) point & 0xfffff000);
	
	/**寻找每个大小的内存池*/
	for (n = 0; n < POOL_SIZE; n ++)
	{
		/**寻找每个内存描述符*/
		for (MD = mem_pool[n].next; MD != NULL; MD = MD->next)
		{
			if (MD->page == page)
			{
				/**就是这个内存描述符*/
				MD->refcnt --;
				
				/**如果这个内存描述符引用为0*/
				if (MD->refcnt == 0)
				{
					if (prev != NULL)
					{
					}
				}
			}
			prev = MD;
		}
	}
}
