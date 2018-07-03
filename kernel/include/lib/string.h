/**
 *Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 *Explorer C standard string head
 *Explorer 0.01/include/lib/string.h
 *version:Alpha
 *9/7/2014 2:19 PM
 */

#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
size_t strlcpy(char *dest, const char *src, size_t size);
int strncmp(const char *cs, const char *ct, size_t count);
size_t strlen(const char *s);
size_t strspn(const char *s, const char *accept);

#endif