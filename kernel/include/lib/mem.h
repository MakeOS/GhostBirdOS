/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer C Library Memory definition
 * Explorer/include/lib/mem.h
 * version:Alpha
 * 11/8/2014 7:37 PM
 */

#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);

#endif