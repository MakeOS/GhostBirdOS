/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer IDE Disk Support
 * Explorer/drivers/hdd.c
 * 2014.7.18 5:48 PM
 */
 
#include "hdd.h"
#include <lib/mem.h>
#include <types.h>
#include <stdlib.h>
#include <memory.h>

/**在ATA标准中，IDE命令共有30多个，其中有10个是通用型命令*/
#define HD_CMD_READ		0x20			//读取扇区命令
#define HD_CMD_WRITE	0x30			//写入扇区命令
#define HD_CMD_CHECK	0x90			//磁盘诊断命令
	
/**操作扇区时允许的最多出错次数*/
#define MAX_ERRORS	10

/**AT硬盘控制器寄存器端口及作用*/
/**读时*/
#define HD_DATA			0x1f0
#define HD_ERROR		0x1f1
#define HD_NSECTOR		0x1f2
#define HD_SECTOR		0x1f3
#define HD_LCYL			0x1f4
#define HD_HCYL			0x1f5
#define HD_CURRENT		0x1f6
#define HD_STATUS		0x1f7
/**写时*/
#define HD_PRECOMP		0x1f1
#define HD_COMMAND		0x1f7

unsigned int LBA_start;


/**初始化磁盘程序，由Lab Explorer Developers<2322869088@qq.com>实现
 * 功能：建立磁盘控制器的中断管理，检查磁盘的分区情况
 */
void init_hdd(void)
{
	/**建立中断控制器的中断处理程序*/
	register_PIC(0xe, &int_HDC_handle, "Hard Disk Control");
	
	/**分配一个扇区的空间*/
	void *point;
	point = vmalloc(512);
	if (point == NULL) error("HDD:memory allocate error!");		/**分配失败控制*/
	
	/**读取硬盘配置信息*/
	HD_get_phy_info(point);
	
	/**读取MBR，获取有用信息*/
	read_disk(0, (unsigned short int*) point, 1);
	memcpy(&LBA_start, (point + 0x1be + 8), 2);
	struct Master_Boot_Record *MBR;
	MBR = (struct Master_Boot_Record *) point;
	//printk("MBR's effect:%#x", (*MBR).valid);
	
	printk("LAB_start:%#X", LBA_start);
	
	/**释放这个空间*/
	vfree(point);
}

void read_disk(u32 LBA, u16 *buffer, u32 number)
{
	/**互斥操作，不允许在读取时有其他任务切换*/
	disable_schedule();
	u32 offset, i;
	io_out16(HD_NSECTOR,number);								/**数量*/
	io_out8(HD_SECTOR,(LBA & 0xff));							/**LBA地址7~0*/
	io_out8(HD_LCYL,((LBA >> 8) & 0xff));						/**LBA地址15~8*/
	io_out8(HD_HCYL,((LBA >> 16) & 0xff));						/**LBA地址23~16*/
	io_out8(HD_CURRENT,(((LBA >> 24) & 0xff) + 0xe0));			/**LBA地址27~24 + LBA模式，主硬盘*/
	io_out8(HD_STATUS,HD_CMD_READ);								/**读扇区*/
	/**循环从DATA端口读取数据*/
	for (i = 0; i != number; i ++)
	{
		hdd_wait();
		for (offset = 0; offset < 256; offset ++)
		{
			buffer[(i * 256) + offset] = io_in16(HD_DATA);		/**从DATA端口中读取数据*/
		}
	}
	/**允许调度*/
	enable_schedule();
	return;
}

void hdd_wait(void)
{
	/**等待次数计时*/
	//unsigned long n;
	for (; (io_in8(HD_STATUS) & 0x88) != 0x08;);				/**循环等待*/
}


/**磁盘参数获取函数*/
static void HD_get_phy_info(struct HD_info *info)
{
	
}


/**磁盘控制器中断处理程序*/
void int_HDC_handle(void)
{
	/**由于目前对于磁盘控制器的中断无处理方法，因此只是简单输出信息*/
	printk("int HDC.\n");
	EOI();
}

