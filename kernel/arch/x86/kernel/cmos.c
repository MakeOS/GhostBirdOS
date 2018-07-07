/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer CMOS support
 * Explorer/arch/x86/kernel/cmos.c
 * version:Aplha
 * 12/6/2014 8:58 PM
 */

#include "cmos.h"

/**CMOS的地址空间没有映射到内存区域上，
 * 要读取CMOS，需要操作端口0x70、0x71
 * 先往0x70发送指定字节的偏移地址，
 * 再向0x71读、写，以读取、写入CMOS。
 */

/**从CMOS中获取信息函数*/
unsigned char read_cmos(unsigned char addr)
{
	io_out8(CMOS_INDEX, addr);		/**写入想要读出的信息在CMOS的偏移*/
	return io_in8(CMOS_DATA);		/**读出相关信息*/
}
