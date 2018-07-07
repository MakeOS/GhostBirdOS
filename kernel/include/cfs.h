/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Center File System Definition
 * Explorer/include/cfs.h
 * version:Alpha
 * 2/17/2015 11:56 AM
 */

#ifndef FAT32_H_
#define FAT32_H_

#include "types.h"

typedef struct
{
	u32 start;
	u32 size;/*×Ö½ÚËã*/
}file_info;

void init_FAT32(void);
unsigned int get_next_clu(unsigned int clu);
unsigned int clu_to_sector(unsigned int clu_num);
void load_data(void *buf, u32 clu);
unsigned int get_metadata(unsigned char *name);
void file_open(u8 *name, void *buf);
file_info get_file_info(u8 *name);

#endif