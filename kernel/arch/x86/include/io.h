/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 Intel x86架构输入输出指令函数功能定义
 * Explorer/arch/x86/include/io.h
 * 7/26/2014 7:05 PM
 */

#ifndef IO_H_
#define IO_H_

#include <types.h>

/**停机函数
 * 该函数执行实体是汇编指令hlt
 * 执行该函数后，计算机会一直处于停机状态(低功耗)，
 * 直到有硬件中断的发生
 */
void io_hlt(void);


/**关闭打开中断位函数
 * 这两个函数执行实体是汇编指令cli和sti
 * io_cli函数执行后，会将PSW寄存器的中断位置0，关闭硬件中断
 * io_sti函数执行后，会将PSW寄存器的中断位置1，接收硬件中断
 */
void io_cli(void);
void io_sti(void);


/**读取恢复标志寄存器函数*/
ulong io_read_PSW();
void io_restore_PSW(ulong value);


/**输入函数*/
u8 io_in8(u32 port);
u16 io_in16(u32 port);
u32 io_in32(u32 port);


/**输出函数*/
void io_out8(u32 port, u8 data);
void io_out16(u32 port, u8 data);
void io_out32(u32 port, u8 data);

#endif
