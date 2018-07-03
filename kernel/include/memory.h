/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management definition
 * Explorer/include/memory.h
 * 11/15/2014 12:05 PM
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

void *vmalloc(size_t size);
void vfree(void *addr);

void *kmalloc(size_t size, int flags);
void kfree(void *point);

/**创建新页目录函数*/
unsigned long new_pdt(void);

#ifdef __cplusplus
}
#endif

#endif
