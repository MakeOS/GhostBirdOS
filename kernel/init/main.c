/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Function main
 * Explorer/init/main.c
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

/**
 * Explorer操作系统内核初始化主函数
 * 进行相关的初始化工作
 */

#include <lib/mem.h>
#include <lib/graphics.h>
#include <stdarg.h>
#include <stdlib.h>
#include <../fs/fat32/fat32.h>
#include <GUI.h>
#include <main.h>
#include <types.h>
#include <task.h>
#include <info.h>
#include <arch.h> // Archtecture - 架构层
#include <keyboard.h>
#include <memory.h>
#include <video.h>

/**在_start.asm中放置了boot_info_ptr指针指向boot_info*/
extern struct boot_info *boot_info_ptr;

/**任务测试函数*/
int task_test()
{
	unsigned long n;
	for (;;){
		for (n = 0; n < 0xffffff; n++);
		printk("process:%d.\n", get_id());
	}
}


/**内核主函数*/
void main(void)
{
	/**判断是否成功获取启动信息*/
	if (boot_info_ptr->flag[0] != 'E' |
		boot_info_ptr->flag[1] != 'B' |
		boot_info_ptr->flag[2] != 'I' )
	{
		reset();		/**系统重置*/
	}
	
	/**判断启动信息的长度是否符合要求*/
	if (boot_info_ptr->size != sizeof(struct boot_info))
	{
		reset();		/**系统重置*/
	}
	
	// int *ptr = (int *)0xe0000000;
	// unsigned int n;
	// for (n = 0; n < 1024 * 768; n ++)
	// {
		// ptr[n] = 0xffff0000;
	// }
	// fin:goto fin;
	
	/**初始化内存管理单元*/
	init_MMU(boot_info_ptr);
	
	/**进入分页模式*/
	init_paging();
	init_time();
	init_Interrupt();
	init_task();
	// init_CPU();
	init_PIC();
	init_PIT();
	
	/**初始化图形*/
	Inti_Graph();				// 初始化图形系统
	init_Font();				// 初始化字库管理
	enable_shell();				// 开启shell
	
	/**打印信息*/
	// output_CPU_info();			// 打印处理器信息
	output_mem_info();			// 打印内存管理相关信息
	output_viedo_info();		// 打印VBE的相关信息
	
	/**初始化设备*/
	init_keyboard();			// 初始化键盘
	init_mouse();				// 初始化鼠标
	init_hdd();					// 初始化磁盘
	init_FAT32();				// 初始化FAT32文件系统
	
	/**多任务性能测试*/
	// new_task(&task_test, NULL);
	
	printk("Hello, world!asfasfd");
	
	/**GUI初始化*/
	// init_GUI();
}

/**输出内核信息*/
void output_kernel_info(void)
{	
	/**内核名称、版本、作者、版权*/
	printk("Kernel Name:" KERNEL_NAME " ");
	printk(KERNEL_VERSION "\n");
	printk("Kernel Author:" AUTHOR "\n");
	printk(COPYRIGHT "\n");
	
	/**其他信息的输出*/
	if (BASE != NULL) printk("It is based on " BASE ".\n");
}

/**内存管理相关信息输出函数*/
void output_mem_info(void)
{
	unsigned long n;
	
	/**空出一行*/
	printk("\n");
	
	/**打印内存分布信息*/
	printk("Address Range Descriptor Structure:\nBaseAddrLow   BaseAddrHigh  LengthLow     LengthHigh    Type\n");
	for (n = 0; n < BOOT_ARDS_NUM; n++)
	{
		printk("%#010x    %#010x    %#010x    %#010x    %#010x\n", boot_info_ptr->ARDS[n].BaseAddrLow, boot_info_ptr->ARDS[n].BaseAddrHigh, boot_info_ptr->ARDS[n].LengthLow, boot_info_ptr->ARDS[n].LengthHigh, boot_info_ptr->ARDS[n].Type);
	}
	
	/**内存信息*/
	extern unsigned int all_mem, real_mem;
	
	/**打信息*/
	printk("Installed memory(RAM):%dMB(%dKB is available).\n", all_mem / 1048576, real_mem / 1024);
}

/**输出VBE信息*/
void output_viedo_info(void)
{
	struct VbeInfoBlock *VbeInfoBlock = &boot_info_ptr->VbeInfoBlock;
	
	/**空出一行*/
	printk("\n");
	
	/**视频控制器信息*/
	printk("Video Controler:%s,Version:%d.%d.\n", VbeInfoBlock->VbeSignature, (VbeInfoBlock->VbeVersion >> 8), VbeInfoBlock->VbeVersion & 0xff);
	
	/**品牌*/
	printk("OEM:%s.\n", real_addr_in_pm(VbeInfoBlock->OemStringPtr_Seg, VbeInfoBlock->OemStringPtr_Off));
	
	/**详细屏幕信息*/
	printk("XResolution:%d, YResolution:%d, BitsPerPixel:%d.\n", boot_info_ptr->ModeInfoBlock.XResolution, boot_info_ptr->ModeInfoBlock.YResolution, boot_info_ptr->ModeInfoBlock.BitsPerPixel);
	
	/**显存信息*/
	printk("PhysBasePtr:%#X, Size of vram:%dBytes, vitual address of vram in kernel:%#X.\n", boot_info_ptr->ModeInfoBlock.PhysBasePtr, Video_Info.vram_length, Video_Info.vram);
}
