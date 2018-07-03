/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 FAT32文件系统头
 * Explorer\Kernel\include\fat32.h
 * version:Alpha
 * 2014.7.18 12:12 AM
 */

#ifndef FAT32_H_
#define FAT32_H_

#include "types.h"

typedef struct
{
	u32 start;
	u32 size;		// 以字节为单位
}file_info;

/**初始化FAT32文件系统*/
void init_FAT32(void);

/**获得下一个簇号*/
unsigned int get_next_clu(unsigned int clu);

/**将簇号转换为LBA扇区寻址*/
unsigned int clu_to_sector(unsigned int clu_num);

/**加载数据*/
void load_data(void *buf, u32 clu);

/**获取元数据*/
unsigned int get_metadata(unsigned char *name);

/**文件加载函数*/
void file_open(u8 *name, void *buf);

/**获取文件信息函数*/
file_info get_file_info(u8 *name);

#endif