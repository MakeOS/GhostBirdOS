/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer FAT32文件系统支持代码
 * Explorer/fs/fat32/fat32.c
 * version:Alpha
 * 7/18/2014 12:06 AM
 */

#include "fat32.h"
#include <lib/mem.h>
#include <stdlib.h>
#include <memory.h>
#include <hdd.h>
#include <types.h>

struct PBR
{
	/*PBR直接提供*/
	u8 	OEM[8];				/*OEM name*/
	u8 	cluster_size;		/*How many sectors per cluster*/
	u16 reserve;			/*The number of reserved sectors*/
	u8 	FAT_num;			/*The number of FAT*/
	u16 root_max_num;		/*根目录最大数量*/
	u16 total_sector_num_16;/*2字节的总扇区数(如果是0，就使用total_sector_num_32的值)*/
	u32 total_sector_num_32;/*4字节的总扇区数(如果超过65535，就使用total_sector_num_32的值)*/
	u32 FAT_size;			/*The number of sector per FAT*/
	u16 FAT_version;		/*version*/
	u32 root_start;			/*根目录起始簇*/
	/*PBR间接提供*/
	u32 FAT_start;			/*FAT区的开始LBA*/
	u32 data_start;			/*data区的开始LBA*/
	u32 total_sector_num;	/*总扇区数*/
}PBR1;

#pragma pack(push)					//保存当前对齐信息
#pragma pack(1)						//设定结构体以一个字节为单位对齐

struct FAT32_PBR
{
	unsigned char	INS_JMP[3];		// 跳转指令
	unsigned char	OEM[8];			// OEM ID(tian&uli2k_X)
	unsigned short	bps;			// 每扇区字节数512
	unsigned char	spc;			// 每簇扇区数
	unsigned short	ressec;			// 保留扇区数(第一个FAT开始之前的扇区数)
	unsigned char	fats;			// FAT数一般为2
	unsigned short	rtents;			// 根目录项数(FAT32为0)
	unsigned short	smlsec;			// 小扇区数(FAT32为0)
	unsigned char	media;			// 媒体描述符硬盘0xF8
	unsigned short	spf;			// 每FAT扇区数(FAT32为0)
	unsigned short	spt;			// 每道扇区数
	unsigned short	heads;			// 磁头数
	unsigned int	relsec;			// 盘上引导扇区以前所有扇区数
	unsigned int	totsec;			// 总扇区数
	unsigned int	spfat;			// 每FAT扇区数FAT32使用
	unsigned short	exflg;			// 扩展标志
	unsigned short	fsver;			// 文件系统版本
	unsigned int	rtclu;			// 根目录簇号
	unsigned short	fsinfo;			// 文件系统信息扇区号一般为1
	unsigned short	bkbot;			// 备份引导扇区6
	unsigned char	reser[12];		// 保留12字节
	
	/*以下为扩展BPB*/
	unsigned char	pdn;			// 物理驱动器号,第一个驱动器为0x80
	unsigned char	exres;			// 保留
	unsigned char	exbtsg;			// 扩展引导标签为0x29
	unsigned int	volume;			// 分区序号,用于区分磁盘
	unsigned char	vollab[11];		// 卷标
	unsigned char	fsid[8];		// 系统ID
	unsigned char	code[420];		// 引导代码
	unsigned short	aa55;			// 引导标志
};

#pragma pack(pop)					//恢复原来的对齐单位

void init_FAT32(void)
{
	char *point;
	point = vmalloc(512);
	if (point == NULL) error("Initialize Memory Error!");
	read_disk(LBA_start, (u16 *) point, 1);
	
	/*拷贝直接得到的数据*/
	memcpy(&PBR1.OEM, (point + 0x03), 8);
	memcpy(&PBR1.cluster_size, (point + 0x0d), 1);
	memcpy(&PBR1.reserve, (point + 0x0e), 2);
	memcpy(&PBR1.FAT_num, (point + 0x10), 1);
	memcpy(&PBR1.root_max_num, (point + 0x11), 2);
	memcpy(&PBR1.total_sector_num_16, (point + 0x13), 2);
	memcpy(&PBR1.total_sector_num_32, (point + 0x20), 4);
	memcpy(&PBR1.FAT_size, (point + 0x24), 4);
	memcpy(&PBR1.FAT_version, (point + 0x2a), 2);
	memcpy(&PBR1.root_start, (point + 0x2c), 4);
	
	/*算出间接提供的数据*/
	PBR1.FAT_start = LBA_start + PBR1.reserve;
	PBR1.data_start = PBR1.FAT_start + (PBR1.FAT_num * PBR1.FAT_size);
	
	/*判断取哪个值*/
	if (PBR1.total_sector_num_16 == 0)
	{
		PBR1.total_sector_num = PBR1.total_sector_num_32;
	}else{
		PBR1.total_sector_num = PBR1.total_sector_num_16;
	}
	/*输出分区信息*/
	printk("Partition OEM:%s,FAT 32 version is 0x%X\n", &PBR1.OEM, PBR1.FAT_version);
	printk("Cluster size:%X,reserve:%X\n", PBR1.cluster_size, PBR1.reserve);
	printk("root max number:%X,root start:%X\n", PBR1.root_max_num, PBR1.root_start);
	
	/**测试结构体*/
	printk("size of struct PBR is:%d.\n", sizeof(struct FAT32_PBR));
}

/*NOTES:由于目前完成的功能有限，下列函数中很多应有返回值来确定成功与失败，但是未实现*/

/*made by Lab Explorer Developers<1@GhostBirdOS.org>
 *open file function
 *entry:(path)file name, address
 *NOTE:目前获取元数据的函数仅仅支持根目录下的大写文件，且是8个字符的文件名+3个字符的后缀名！
 */
void file_open(u8 *name, void *buf)
{
	file_info read_file;
	read_file = get_file_info(name);
	//printk("read_file.start = 0x%X",read_file.start);
	load_data(buf, read_file.start);
}

/*made by 迷彩红星<1@ghostbirdos.org>
 *读取文件数据函数
 *入口：(路径)文件名
 *NOTE:目前获取元数据的函数仅仅支持根目录下的大写文件(文件名8个字或者以内，不支持小写和空格)，一定要有后缀名(后缀名3个字或以内)！
 */
file_info get_file_info(u8 *name)
{
	u8 *root_point, tar_buffer[13], src_buffer[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};/*源buffer初始化*/;
	u32 table_point, offset_point, sector, clu, buffer_point;
	file_info s;
	/*处理文件路径：使nasm指向第一个不为"\"或者"/"的字符*/
	if (*name == 92 | *name == 47)/*"\"字符ASCII是92*/
	{
		name += 1;
	}
	/*组织好scr_buffer*/
	for (offset_point = 0; (offset_point < 12) && name[offset_point] != 0x00; offset_point ++)
	{
		src_buffer[offset_point] = name[offset_point];
	}
	/*读取根目录*/
	root_point = (u8 *) vmalloc(PBR1.cluster_size * 512);/*512unsigned char是一个标准磁盘扇区的大小，乘上每个簇包含扇区的数量得一个簇的大小*/
	if (root_point == NULL) error("get file information is error!");
	for (clu = PBR1.root_start; (clu & 0x0FFFFFF0) != 0x0FFFFFF0;)
	{
		sector = clu_to_sector(clu);
		read_disk(sector, (u16*) root_point, PBR1.cluster_size);
		/*一簇的页目录表中逐个匹配*/
		for (table_point = 0; table_point < (PBR1.cluster_size * 512); table_point += 32)		/*目录项每项32unsigned char*/
		{
			u8 tar_buffer[]={0,0,0,0,0,0,0,0,0,0,0,0,0};/*buffer初始化*/
			/*循环读入该表项表示的文件名*/
			for (offset_point = 0; (root_point[table_point + offset_point] != 0x20) && (offset_point < 8); offset_point ++)
			{
				tar_buffer[offset_point] = root_point[table_point + offset_point];
			}
			tar_buffer[offset_point] = '.';/*加上点*/
			buffer_point = offset_point + 1;/*整合指针*/
			/*循环读入该表项的后缀名*/
			for (offset_point = 0; (root_point[table_point + offset_point] != 0x20) && (offset_point < 3); offset_point ++)
			{
				tar_buffer[buffer_point + offset_point] = root_point[table_point + offset_point + 8];
			}
			/*判断该文件名是否是要寻找的文件名*/
			for (offset_point = 0; (offset_point < 13) && src_buffer[offset_point] == tar_buffer[offset_point]; offset_point ++)
			{
				if (offset_point == 12)
				{
					/*到这里就是匹配成功*/
					/*起始簇号*/
					s.start = ((u16 *) root_point)[((table_point + 0x1a) / sizeof(u16))] + (((u16 *) root_point)[((table_point + 0x14) / sizeof(u16))] << 16);
					/*长度(unsigned char)*/
					s.size = ((int *)root_point)[(table_point + 0x1C) / sizeof(u32)];
					/*释放读取缓冲*/
					vfree(root_point);
					return s;
				}
			}
		}
		clu = get_next_clu(clu);
	}
	printk("Not found file.");
	io_hlt();
}

u32 clu_to_sector(u32 clu_num)
{
	return PBR1.data_start + ((clu_num - 2) * PBR1.cluster_size);
}

u32 get_next_clu(u32 clu)
{
	u32 *point, next_clu;
	point = (u32 *) vmalloc(PBR1.cluster_size * 512);
	/*每个FAT表项4个字节，一个扇区可以装128个FAT表项*/
	read_disk((PBR1.FAT_start + (clu / 128)), (unsigned short int*) point, 1);
	next_clu = point[clu % 128];
	vfree(point);
	return next_clu;
}

void load_data(void *buf, u32 clu)
{
	u32 sector;
	for (; (clu & 0x0FFFFFF0) != 0x0FFFFFF0;)
	{
		sector = clu_to_sector(clu);
		read_disk(sector, (unsigned short int*) buf, PBR1.cluster_size);
		buf += (PBR1.cluster_size * 512);		/*addr指向内存中下个扇区的首地址*/
		clu = get_next_clu(clu);
	}
	return;
}

