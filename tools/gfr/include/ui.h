/**
 * Date:2018/10/5 18:26
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * UI interface defination
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#ifndef UI_H_
#define UI_H_

#include "types.h"

void ui_prog_output(db color);
void ui_prog_abort(void);
void ui_prog_finish(void);
void ui_prog_update(float d_prog, float d_total);
void ui_prog_start(const db *desc);

#endif