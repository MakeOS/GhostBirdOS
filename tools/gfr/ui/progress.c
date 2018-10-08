/**
 * Date:2018/10/5 16:54
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * UI - Progress
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/types.h"
#include "../include/config.h"

bool	ui_enable = false;
float	ui_prog;
float	ui_total;
db		ui_desc[CFG_UI_PROG_DESC_LEN + 1];

#define UI_PROG_OUT_CLR_FINISH	7
#define UI_PROG_OUT_CLR_ABORT	1
#define UI_PROG_OUT_CLR_UPDATE	3

void ui_prog_output(db color)
{
	db i, j;
	db num[80];
	db num_len;
	float percent;
	
	if (!ui_enable) return;
	
	percent = (ui_prog / ui_total) * 100;
	if (ui_total == 0) percent = 0;
	if (percent > 100) percent = 100;
	
	num_len = sprintf(num, "%.2f%%", percent);
	
	printf("\033[%dm%s\033[7m\033[1m", color + 30, ui_desc);
	
	for (i = j = 0; i < CFG_UI_PROG_BAR_LEN; i ++)
	{
		if (((i + 1) * ui_total) > (ui_prog * CFG_UI_PROG_BAR_LEN))
			printf("\033[0m\033[%dm\033[7m", color + 30);
		
		if (i < (CFG_UI_PROG_BAR_LEN - num_len) / 2)
			printf(" ");
		else if (i < ((CFG_UI_PROG_BAR_LEN - num_len) / 2) + num_len)
		{
			printf("%c", num[j ++]);
		}else{
			printf(" ");
		}
	}
	
	printf("\033[0m\n\033[1A");
}

void ui_prog_abort(void)
{
	ui_enable = false;
}

void ui_prog_finish(void)
{
	ui_prog_output(UI_PROG_OUT_CLR_FINISH);
	
	ui_prog = 0;
	ui_total = 0;
	ui_enable = false;
	
	printf("\n");
}

void ui_prog_update(float d_prog, float d_total)
{
	ui_prog += d_prog;
	ui_total += d_total;
	
	ui_prog_output(UI_PROG_OUT_CLR_UPDATE);
}

void ui_prog_start(const db *desc)
{
	dd desc_len = strlen(desc);
	
	memcpy(ui_desc, desc, desc_len);
	memset(&ui_desc[desc_len], ' ', \
	CFG_UI_PROG_DESC_LEN - desc_len);
	
	if (desc_len > CFG_UI_PROG_DESC_LEN)
	{
		ui_desc[CFG_UI_PROG_DESC_LEN - 0] = '\0';
		ui_desc[CFG_UI_PROG_DESC_LEN - 1] = '.';
		ui_desc[CFG_UI_PROG_DESC_LEN - 2] = '.';
		ui_desc[CFG_UI_PROG_DESC_LEN - 3] = '.';
	}
	
	ui_prog = 0;
	ui_total = 0;
	ui_enable = true;
	
	ui_prog_output(UI_PROG_OUT_CLR_UPDATE);
}
