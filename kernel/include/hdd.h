/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 磁盘支持程序头文件
 * ExplorerOS/Kernel/include/hdd.h
 * version:Alpha
 * 7/18/2014 5:48 PM
 */

#ifndef HDD_H_
#define HDD_H_

#include "types.h"

extern u32 LBA_start;
void inti_hdd(void);
void read_disk(u32 LBA, u16 *buffer, u32 number);
void hdd_wait(void);

#endif

