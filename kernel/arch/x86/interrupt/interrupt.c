/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Interrupt Management
 * Explorer/arch/x86/interrupt/interrupt.c
 * version:Alpha
 * 12/22/2014 2:52 PM
 */

#include "interrupt.h"
#include <memory.h>
#include "../include/page.h"
#include "../include/function.h"
#include "../include/address.h"
#include <stdlib.h>
#include <types.h>

void init_Interrupt(void)
{
	/**初始化中断描述符表*/
	IDT_len = 256;
	IDT_size = IDT_len * 8;
	IDT_addr = (u32 *)vmalloc(IDT_size);
	write_IDTR((u32)IDT_addr, IDT_size - 1);
	clean_IDT();
	
	
	/**初始化irq数组*/
	unsigned long point;
	for (point = 0; point < NUMBER_INTERRUPT; point ++)
	{
		irqaction[point].name = NULL;
		irqaction[point].handle = &easy_handle;
	}
	
	
	/**创建中断描述符*/
	create_IDT(0 , code_0_selector, &int_0 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(1 , code_0_selector, &int_1 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(2 , code_0_selector, &int_2 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(3 , code_0_selector, &int_3 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(4 , code_0_selector, &int_4 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(5 , code_0_selector, &int_5 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(6 , code_0_selector, &int_6 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(7 , code_0_selector, &int_7 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(8 , code_0_selector, &int_8 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(9 , code_0_selector, &int_9 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(10, code_0_selector, &int_10, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(11, code_0_selector, &int_11, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(12, code_0_selector, &int_12, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(13, code_0_selector, &int_13, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(14, code_0_selector, &int_14, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(15, code_0_selector, &int_15, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(16, code_0_selector, &int_16, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(17, code_0_selector, &int_17, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(18, code_0_selector, &int_18, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(19, code_0_selector, &int_19, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(20, code_0_selector, &int_20, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(21, code_0_selector, &int_21, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(22, code_0_selector, &int_22, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(23, code_0_selector, &int_23, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(24, code_0_selector, &int_24, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(25, code_0_selector, &int_25, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(26, code_0_selector, &int_26, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(27, code_0_selector, &int_27, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(28, code_0_selector, &int_28, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(29, code_0_selector, &int_29, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(30, code_0_selector, &int_30, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(31, code_0_selector, &int_31, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(32, code_0_selector, &int_32, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(33, code_0_selector, &int_33, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(34, code_0_selector, &int_34, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(35, code_0_selector, &int_35, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(36, code_0_selector, &int_36, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(37, code_0_selector, &int_37, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(38, code_0_selector, &int_38, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(39, code_0_selector, &int_39, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(40, code_0_selector, &int_40, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(41, code_0_selector, &int_41, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(42, code_0_selector, &int_42, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(43, code_0_selector, &int_43, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(44, code_0_selector, &int_44, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(45, code_0_selector, &int_45, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(46, code_0_selector, &int_46, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(47, code_0_selector, &int_47, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(48, code_0_selector, &int_48, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_IDT(49, code_0_selector, &int_49, interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	
	/**专门用作系统调用的描述符*/
	create_IDT(50, code_0_selector, &int_50, interrupt_gate + IDT_32 + IDT_DPL_3 + IDT_P);
	
	
	/**初始化Intel 386保护模式的相关中断异常处理程序*/
	init_trap();
	
	
	/**正常返回*/
	return;
}


/**注册中断请求线函数
 * irq为中断号
 * name为对这个中断的描述
 * handle为处理这个中断的指针
 */
long register_irq(unsigned char irq, char *name, void (*handle)(int error_code))
{
	irqaction[irq].name = name;
	irqaction[irq].handle = handle;
	return 0;
}


/**初始化Intel 386保护模式的相关中断异常函数*/
void init_trap(void)
{
	register_irq(0, "Divide Error", &Divide_Error);
	register_irq(3, "Break Point", &Break_Point);
	register_irq(4, "Over Flow", &Over_Flow);
	register_irq(6, "Undefined", &Undefined);
	register_irq(8, "Double Fault", &Double_Fault);
	register_irq(10, "Invalid Task Segment", &Invalid_Task_Segment);
	register_irq(13, "General Protection", &General_Protection);
	register_irq(14, "Page Fault", &do_page_fault);
}


/**除法错误中断处理程序*/
static void Divide_Error(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Divide Error(#DE).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**断点中断处理程序*/
static void Break_Point(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Break Point(#BP).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**溢出中断处理程序*/
static void Over_Flow(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Over Flow(#OF).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**未定义指令中断处理程序*/
static void Undefined(int error_code)
{
	/*显示错误界面*/
	//rectangle(0, 0, 800, 600, 0x8040B0);
	//outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	//outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Un-Define(#UD).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**双重错误中断处理程序*/
static void Double_Fault(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Double Fault(#DF).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**无效任务段中断处理程序*/
static void Invalid_Task_Segment(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode Invalid TSS(#TS).");
	/*循环停机*/
	for (;;) io_hlt();
}


/**常规保护中断处理程序*/
static void General_Protection(int error_code)
{
	/*显示错误界面*/
	rectangle(0, 0, 800, 600, 0x8040B0);
	outtextxy(50, 200, 0xffffff, "An error occurred by Processor.Explorer Kernel has been stopped.");
	outtextxy(50, 216, 0xffffff, "ERROR information:Protect-Mode General Protection(#GP).");
	/*循环停机*/
	for (;;) io_hlt();
}
