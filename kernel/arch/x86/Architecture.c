/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer x86平台硬件抽象层
 * Explorer/arch/x86/Architecture.c
 * version:Alpha
 * 7/14/2014 1:06 PM
 */

#include "include/function.h"
#include "include/address.h"
#include <task.h>
#include <types.h>

struct TSS_32 TSS;

/**架构初始化*/
void init_Architecture(void)
{
	/**准备GDT表*/
	write_GDTR(GDT_addr, GDT_size - 1);
	clean_GDT();
	
	/**创建数据段和代码段*/
	code_0_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_code_32_non_conforming);
	code_3_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_code_32_conforming);
	data_0_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_0 + GDT_data_32);
	data_3_selector = set_GDT(0, 0xfffff, GDT_G + GDT_P + GDT_DPL_3 + GDT_data_32);
	init_seg_reg(data_0_selector);
	
	/**准备TSS(真心不该有TSS):-(*/
	TSS_selector = set_GDT(((u32)&TSS), sizeof(TSS) - 1, GDT_P + GDT_DPL_0 + GDT_TSS_data);
	
	/**TSS初始赋值*/
	TSS.ss0 = data_0_selector;
	// TSS.esp0 = (unsigned long)&task_0 + TASK_SIZE;
	// TSS.ss1 = data_0_selector;
	// TSS.esp1 = 0;
	// TSS.ss2 = 0;
	// TSS.esp2 = 0;
	TSS.eip = 0;
	TSS.eflags = 0;
	TSS.eax = 0;
	TSS.ecx = 0;
	
	TSS.iomap = sizeof(TSS) - 2;
	TSS.ioend = 0xff;
	
	/**加载TSS*/
}

/**
 * 进入分页模式函数
 * 分页模式是保护模式下一种可选的模式，对内存进行分页管理
 * 实现分页模式可以
 */
void goto_paging(u32 pdt_addr)
{
	/**将页目录表的地址写入CR3寄存器*/
	write_CR3(pdt_addr);
	
	/**设置CR0的最高位，开启分页模式*/
	write_CR0(read_CR0() | 0x80000000);
}

/**重置函数*/
void reset(void)
{
	io_out8(0x64,0xfe);
}
