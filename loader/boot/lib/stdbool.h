/* Copyright (C) 1998, 1999, 2000 Free Software Foundation, Inc.
        This file is part of GCC.
 */

#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifndef __cplusplus

/**C code*/
#define bool _Bool
#define true 1
#define false 0

#else

/**C++ code*/
#define _Bool bool
#define bool bool
#define false false
#define true true

#endif

#endif	/* stdbool.h */
