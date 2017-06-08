/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Hu Wenjie<1@hwj.me>
 * Explorer C library memory
 * Explorer/lib/mm.c
 * version:Alpha
 * 11/8/2014 7:37 PM
 */

#include "mem.h"

/**
 * memset - Fill a region of memory with the given value
 * @buffer: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 *
 * Do not use memset() to access IO space, use memset_io() instead.
 */
void *memset(void *buffer, int c, unsigned long count)
{
	char *xs = buffer;

	while (count--)
		*xs++ = c;
	return buffer;
}

/**
 * memcpy - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
void *memcpy(void *dest, const void *src, unsigned long count)
{
	char *tmp = dest;  
	const char *s = src;  

    while (count--)  
		*tmp++ = *s++;  
	return dest;  
}

/**
 * memmove - Copy one area of memory to another
 * @dest: Where to copy to
 * @src: Where to copy from
 * @count: The size of the area.
 *
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *memmove(void *dest, const void *src, unsigned long count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}
