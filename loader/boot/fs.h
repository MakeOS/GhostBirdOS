/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 FAT32文件系统头
 * Explorer\Kernel\include\fat32.h
 * version:Alpha
 * 2014.7.18 12:12 AM
 */

#ifndef FS_H_
#define FS_H_

/**文件系统的文件名和路径+文件名的最大长度定义*/
#define FILENAME_MAX 256			// 最长文件名

typedef struct file_info
{
	char name[FILENAME_MAX];
	unsigned long size;
	unsigned int cluster;
	char detail;
}file_info;

/**初始化FAT32文件系统*/
void init_FAT32(void);

/**获得下一个簇号*/
unsigned int get_next_clu(unsigned int clu);

/**将簇号转换为LBA扇区寻址*/
unsigned int clu_to_sector(unsigned int clu_num);

/**加载数据*/
void load_data(void *buf, unsigned long clu);

/**获取元数据*/
unsigned int get_metadata(char *name);

/**FAT32读取文件函数*/
struct file_info fat32_read_file(unsigned long storage_number, unsigned long partition, const char *name, void *addr, char detail);

/**获取文件信息函数*/
file_info fat32_read_file_info(const char *name);

/**读取文件信息函数*/
struct file_info read_file_info(unsigned long storage_number, unsigned long partition, char *name);

/**读取文件函数*/
struct file_info read_file(unsigned long storage_number, unsigned long partition, char *name, void *addr, char detail);

#endif
