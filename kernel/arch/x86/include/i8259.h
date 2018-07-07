/*
 *Copyright 2013-2015 by Explorer Developers.
 *made by Hu wenjie<1@GhostBirdOS.org>
 *Explorer 8259 head
 *Explorer 0.01/arch/x86/include/i8259.h
 *version:Alpha
 *7/26/2014 5:26 PM
 */

#ifndef PIC_H_
#define PIC_H_

#include <types.h>

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

#define PIC0_intr_offset	0x20
#define PIC1_intr_offset	PIC0_intr_offset + 0x8

void init_PIC(void);
int32 open_PIC(u8 IRQ);
int32 register_PIC(u8 IRQ, void *function, u8 *info);
void EOI(void);

#endif