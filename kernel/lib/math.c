/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Math Library
 * Explorer/lib/math.c
 * version:Alpha
 * 12/6/2014 11:10 AM
 */

#include <lib/math.h>

/**取一个有符号数的绝对值函数*/
long abs(long number)
{
	if (number < 0) return number * (-1);
	return number;
}