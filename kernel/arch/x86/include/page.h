/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Paging Support
 * Explorer/arch/x86/include/page.h
 * version:Alpha
 * 7/9/2014 8:30 AM
 */

#ifndef PAGE_H_
#define PAGE_H_

#define PAGE_SIZE	4096				// 页的大小：4096Byte

/**页故障处理程序*/
void do_page_fault(int error_code);

#endif