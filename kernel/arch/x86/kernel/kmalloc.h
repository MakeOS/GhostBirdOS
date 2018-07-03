/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer function kmalloc head
 * Explorer/include/kmalloc.h
 * version:Alpha
 * 2/10/2014 9:04 AM:created
 * 10/1/2015 8:15 AM:重写内存分配
 */

#ifndef KMALLOC_H_
#define KMALLOC_H_

/**内存池*/
struct mem_pool
{
	size_t size;
	unsigned long number;
	struct Memory_Descriptor *next;
};

/**内存描述符*/
struct Memory_Descriptor
{
	void *page;
	void *freeptr;
	unsigned short refcnt;
	struct Memory_Descriptor *next;
};
/**其中flags代表下面情况之一*/
#define MEM_INVALID	0
#define MEM_FREE	1
#define MEM_USED	2


/**内核小块内存分配函数
 * 参数:size - 请求的内存块长度
 * 返回值:NULL - 分配失败; !NULL - 分配成功，返回内存首地址
 */
void *kmalloc(size_t size, int flags);

/**内核小块内存释放函数
 * 参数:point - 指向要释放的内存首地址(之前kmalloc函数的返回值)
 */
void kfree(void *point);

#endif