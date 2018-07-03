/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 x86平台硬件抽象层头文件
 * Explorer/Kernel/include/HAL/x86/function.h
 * 7/14/2014 1:49 PM
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <types.h>

/**代码段、数据段、TSS段的选择子*/
u32 code_0_selector, data_0_selector, TSS_selector;
u32 code_3_selector, data_3_selector;

/**TSS结构体*/
struct TSS_32
{
	u32 backlink;
	u32 esp0;
	u32 ss0;
	u32 esp1;
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es;
	u32 cs;
	u32 ss;
	u32 ds;
	u32 fs;
	u32 gs;
	u32 ldt;
	u32 iomap;
	u8 ioend;
};
//fun_c.c函数
void init_arch(void);
void inti_GDT(void);
void goto_paging(u32 pdt_addr);

//中断描述符表属性
#define IDT_P		0x8000
#define IDT_DPL_0	0x00
#define IDT_DPL_1	0x2000
#define IDT_DPL_2	0x4000
#define IDT_DPL_3	0x6000
#define IDT_16		0x00
#define IDT_32		0x800
#define trap_gate		0x700
#define interrupt_gate	0x600

u32 IDT_len, *IDT_addr, IDT_size;
//fun_asm.asm函数
//全局描述符表属性
#define GDT_G		0x800000
#define GDT_P		0x8000
#define GDT_DPL_0	0x00
#define GDT_DPL_1	0x2000
#define GDT_DPL_2	0x4000
#define GDT_DPL_3	0x6000
/*注意:都为可读的代码段*/
#define GDT_code_32_conforming		0x401E00
#define GDT_code_32_non_conforming	0x401A00
/*为向上的数据段，向下的数据段有风险，不可使用*/
#define GDT_data_32 0x401200
/*系统段*/
#define GDT_TSS_data 0x900
//读取tsc寄存器函数//
unsigned long long int read_tsc(void);
//初始化段寄存器函数
void init_seg_reg(u16 selector);
//描述符表寄存器操作函数
void write_IDTR(u32 base, u16 size);
void write_TR(u16 select);
void write_GDTR(u32 base, u16 size);
//GDT操作函数
void clean_GDT(void);
u16 set_GDT(u32 base_addr, u32 length, u32 attribute);
//IDT操作函数
void clean_IDT(void);

/**
 * 创建中断描述符函数
 * number为要创建的中断描述符的中断号
 * selector为该描述符的选择子
 * offset为中断处理函数指针
 * attribute为中断的相关属性
 */
void create_IDT(u32 number, u32 selector, void *offset, u32 attribute);
//控制寄存器读写
u32 read_cr0(void);
void write_CR0(u32 cr0);
u32 read_cr3(void);
void write_CR3(u32 cr0);
//特殊大小内存读写函数
void write_mem24(u32 addr, u32 data);


#endif

