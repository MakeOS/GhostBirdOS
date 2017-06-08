/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Hu Wenjie<1@hwj.me>
 * Explorer C Library Memory definition
 * Explorer/include/lib/mem.h
 * version:Alpha
 * 11/8/2014 7:37 PM
 */

#ifndef MEM_H_
#define MEM_H_

void *memset(void *s, int c, unsigned long count);
void *memcpy(void *dest, const void *src, unsigned long count);
void *memmove(void *dest, const void *src, unsigned long count);

#endif
