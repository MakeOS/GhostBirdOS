/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * studio.h
 * Explorer/include/studio.h
 * 2/5/2015 3:2 PM
 */

#ifndef STUDIO_H_
#define STUDIO_H_

#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#endif
