/**
 * Copyright 2013-2016 by Explorer Developers.
 * made by Hu Wenjie<1@hwj.me>
 * Explorer loader storage Support Definition
 * Explorer loader/boot/storage.h
 * version:2.0
 * 6/23/2016 12:09 PM
 */

#ifndef STORAGE_H_
#define STORAGE_H_

/**定义扇区大小*/
#define SECTOR_SIZE 512

/**储存器号分配*/
#define SD_IDE_00 0
#define SD_IDE_01 1
#define SD_IDE_10 2
#define SD_IDE_11 3
#define SD_NUM (SD_IDE_11 + 1)

/**储存器描述结构所需要的内存大小*/
#define storage_SD_size SD_MAX * sizeof(struct Storage_Description)					// 储存器描述符表大小

#pragma pack(push)					// 保存当前对齐信息
#pragma pack(1)						// 设定结构体以一个字节为单位对齐

/**储存器分区结构体*/
struct Storage_Partition{
	unsigned char active;
	unsigned char CHS_start[3];
	unsigned char fs;
	unsigned char CHS_end[3];
	unsigned int start_LBA;
	unsigned int size_sector;
};

#pragma pack(pop)					// 恢复原来的对齐单位

#ifndef STORAGE_C_

/**存放扇区内存指针*/
extern void *sector_buffer;

/**储存器描述符表*/
struct Storage_Description
{
	_Bool exist;
	struct Storage_Partition SP[4];
};
extern struct Storage_Description *SD;

#endif

/**返回值情况*/
#define STORAGE_RETVAL_NORMAL 0				// 不支持
#define STORAGE_RETVAL_ERR_NOT_SUPPORT -1	// 不支持

/**读储存器扇区函数*/
int storage_read(unsigned long storage_number, void *dest, unsigned long LBA_addr, unsigned long n);

/**返回活动分区的数量*/
unsigned long storage_active_partition(void);

/**初始化储存器管理函数*/
unsigned int init_storage(void);

#endif

