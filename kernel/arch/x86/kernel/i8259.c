/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 8259\8259A支持程序
 * ExplorerOS\Kernel\drivers\i8259.c
 * version:1
Updates:
	7/26/2014 5:02 PM Lab Explorer created.
	11/29/2014 2:40 PM Lab Explorer find a error at int32 open_PIC(u8 IRQ) function:require open Slave 8259 IRQ cannot be opened
 */

/**
 * NOTICE
 * 8259/8259A为可编程中断控制器(Programmable interrupt controller,PIC)。
 * 直接连接到CPU的8259为主8259(master PIC)，连接到主8259的8259都为从8259(slave PIC)。
 * 从8259和主8259连接这种方式叫级联
 * 标准x86平台拥有两块8259，以级联的方式使用。
 * 一片8259管理8个中断信号(IRQ)。
 * 标准x86平台上master PIC 管理IRQ0~IRQ7，slave PIC 管理IRQ8~IRQ15，slave PIC连接到master PIC IRQ2 上。
 */
#include "../include/i8259.h"
#include "../include/function.h"
#include <types.h>

/*8259初始化函数*/
void init_PIC(void)
{
	/*Initialization dev_table*/
	
	/*Initialization 8259A*/
	io_out8(PIC0_IMR, 0xff);				/*禁止主8259的所有中断*/
	io_out8(PIC1_IMR, 0xff);				/*禁止从8259的所有中断*/

	io_out8(PIC0_ICW1, 0x11);				/*边沿触发模式*/
	io_out8(PIC0_ICW2, PIC0_intr_offset);	/*PIC 0~7 由PIC0_intr_offset ~ PIC0_intr_offset+8负责*/
	io_out8(PIC0_ICW3, 1 << 2);				/*从8259跟PIC 2 相连*/
	io_out8(PIC0_ICW4, 0x01);				/*无缓冲模式*/
	
	io_out8(PIC1_ICW1, 0x11);				/*边沿触发*/
	io_out8(PIC1_ICW2, PIC1_intr_offset);	/*PIC 8~15 由int 0x28~0x2f负责*/
	io_out8(PIC1_ICW3, 2);					/*从8259跟PIC 2 相连*/
	io_out8(PIC1_ICW4, 0x01);				/*无缓冲模式*/

	io_out8(PIC0_IMR, 0xfb);				/*禁止所有中断*/
	io_out8(PIC1_IMR, 0xff);				/*禁止所有中断*/
	
	//register_PIC(2, &int_Slave_8259A, "salve 8259A");
	
	io_sti();/*开启可屏蔽中断*/
}

/**
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * 开放8259特定中断的函数
 * 入口：IRQ号
 * 出口：(-1):错误 0:正确
 */
int32 open_PIC(u8 IRQ)
{
	if (IRQ > 15)
	{
		return (-1);
	}else if (IRQ <= 7)/*主8259*/
	{
		io_out8(PIC0_IMR, (io_in8(PIC0_IMR) & (~(1 << IRQ))));
	}else{/*从8259*/
		io_out8(PIC1_IMR, (io_in8(PIC1_IMR) & (~(1 << IRQ - 8))));
	}	
	return 0;
}

int32 register_PIC(u8 IRQ, void *function, u8 *info)
{
	/*error if the IRQ > 15*/
	if (IRQ > 15)
	{
		return -1;
	}
	//create_IDT(PIC0_intr_offset + IRQ, code_0_selector, function, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	register_irq(PIC0_intr_offset + IRQ, info, function);
	open_PIC(IRQ);
}

void EOI(void)
{
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}
