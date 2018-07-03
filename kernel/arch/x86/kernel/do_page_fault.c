/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 页故障处理程序
 * Explorer/arch/x86/kernel/do_page_fault.c
 * version:Alpha
 * 8/7/2014 10:46 AM
 */

#include <lib/mem.h>
#include "../include/page.h"
#include <stdlib.h>
#include <memory.h>
#include <types.h>

int rentry = 0;

/**页故障处理函数*/
void do_page_fault(int error_code)
{
	u32 cr2, *pdt, *pt, *new_page;
	
	/**判断是否重入*/
	if (rentry != 0)
	{
		printk("rentry!!!");
		fin:goto fin;
	}else{
		rentry = 1;
	}
	
	/**读取CR2信息*/
	cr2 = read_CR2();
	pdt = (u32 *) (read_CR3() & 0xfffff000);
	
		
	/**判断是否是缺页引发的中断*/
	if ((error_code == 2) | (error_code == 0))
	{
		printk("do page fault address:%#X.\n", cr2);
		
		/**如果故障原因是没有页表*/
		if ((pdt[(cr2 >> 22)] & 1) == 0)
		{
			/**获取一个新页*/
			for (new_page = NULL; new_page == NULL; )
				new_page = vmalloc(PAGE_SIZE);
			
			/**放置新页作为页表（页目录表和页表均在内核态空间中）*/
			pdt[(cr2 >> 22)] = ((unsigned int)new_page | 0x7);
			
			/**清空这个页，保证不会出现干扰*/
			memset((u8 *) (pdt[(cr2 >> 22)]), 0x00, ((4096 - 256) / 4));
		}
		
		/**先获取页表地址*/
		pt = (u32 *)(pdt[(cr2 >> 22)] & 0xfffff000);
		
		/**无论是否出现缺页表的情况，一定缺少页*/
		pt[(cr2 & 0x3FF000) >> 12] = (get_free_page() | 0x7);
		
		/*显示信息及返回*/
		//printk("Page fault:allocated.%X",pdt[(cr2 >> 22)]);
		
		goto finish;
	}else{
		/**开启shell*/
		enable_shell();
		
		/**关闭shell*/
		printk("Page fault:(Unknown)error code:0x%X", error_code);
		
		/**停机指令*/
		io_hlt();
	}
	
finish:
	rentry = 0;
	return;
}


