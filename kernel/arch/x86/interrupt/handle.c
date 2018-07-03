/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Interruption handle
 * Explorer/arch/x86/interrupt/handle.c
 * version:Alpha
 * 1/2/2015 9:44 AM: created
 */

#include "interrupt.h"

/**中断应答函数*/
void interrupt_handle(struct context context, unsigned char irq, int error_code)
{
	irqaction[irq].handle(error_code);
}

/**简单应答函数*/
void easy_handle(int error_code)
{
	/**无操作*/
	printk("Unknown Interrupt. error:%#x.\n", error_code);
}
