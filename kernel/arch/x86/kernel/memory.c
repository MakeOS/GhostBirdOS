/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Memory Management Unit
 * Explorer/arch/x86/kernel/memory.c
 * version:Alpha
 * 5/7/2014
 */
 
#include <memory.h>
#include <main.h>
#include <stddef.h>
#include <stdlib.h>
#include <lib/mem.h>
#include <../include/page.h>

/**内存信息*/
unsigned int all_mem = 0, real_mem = 0;

/**内核区域大小*/
#define RANG_KERNEL_SIZE		268435456

/**内核内存字节页图*/
#define KER_MEM_BYTEMAP_PTR		0x100000
#define KER_MEM_BYTEMAP_SIZE	RANG_KERNEL_SIZE / 4096
unsigned char *ker_mem_bytemap = (unsigned char *) KER_MEM_BYTEMAP_PTR;

/**允许分配回收的内核区域起始位置*/
#define KERNEL_ALLOC_START		KER_MEM_BYTEMAP_PTR + KER_MEM_BYTEMAP_SIZE

/**内核内存字节页图中每项的值的含义*/
#define KER_MEM_BYTEMAP_FREE	0b00000000			// 空闲页
#define KER_MEM_BYTEMAP_USED	0b00000001			// 已经使用的内存块
#define KER_MEM_BYTEMAP_START	0b00000010			// 已经使用的内存块的起始页
#define KER_MEM_BYTEMAP_END		0b00000100			// 已经使用的内存块的末尾页

/**物理内存位页图*/
#define PHY_MEM_ALLOC_START		RANG_KERNEL_SIZE
#define PHY_MEM_BITMAP_PTR		0x30000
#define PHY_MEM_BITMAP_SIZE		(4294967296 / PAGE_SIZE) / 8
unsigned int *phy_mem_bitmap = (unsigned int *) PHY_MEM_BITMAP_PTR;


/**NOTICE：phy_mem_bitmap是个32位数据指针，所以phy_mem_bitmap作为数组，每一个元素都能表示32个页，即2^5*/


/**设置页使用函数*/
void set_phy_page_used(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] | (1 << ((ptr >> 12) & 0b11111));
}

/**设置页自由函数*/
void set_phy_page_free(unsigned long ptr)
{
	phy_mem_bitmap[ptr >> 17] = phy_mem_bitmap[ptr >> 17] & ~(1 << ((ptr >> 12) & 0b11111));
}

/**设置内核内存字节页图函数*/
void set_ker_bytemap(unsigned long ptr, char flag)
{
	ker_mem_bytemap[ptr >> 12] = flag;
}

/**初始化内存管理单元函数*/
void init_MMU(struct boot_info *boot_info)
{
	unsigned long n;
	unsigned int BaseAddr, Length;
	
	/**内核区域大小必须为4MB的整数倍*/
	if ((RANG_KERNEL_SIZE % 4194304) != 0) reset();
	
	/**初始化物理内存位页图*/
	for (n = 0; n < (PHY_MEM_BITMAP_SIZE / sizeof(unsigned int)); n ++)
	{
		/**1代表已经被占用*/
		phy_mem_bitmap[n] = 0xffffffff;
	}
	
	/**初始化内核内存字节页图*/
	for (n = 0; n < KER_MEM_BYTEMAP_SIZE; n ++)
	{
		ker_mem_bytemap[n] == KER_MEM_BYTEMAP_USED | KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_END;
	}
	
	/**循环读取Address Range Descriptor Structure ，
	 * 建立物理内存位页图
	 */
	for (n = 0; n < BOOT_ARDS_NUM; n ++)
	{
		/**总内存计数*/
		all_mem += boot_info->ARDS[n].LengthLow;
		
		/**判断是否是高于4GB的范围*/
		if (boot_info->ARDS[n].BaseAddrHigh != 0) break;
		
		/**4KB对齐*/
		boot_info->ARDS[n].BaseAddrLow = boot_info->ARDS[n].BaseAddrLow & 0xfffff000;
		boot_info->ARDS[n].LengthLow = boot_info->ARDS[n].LengthLow & 0xfffff000;
		
		/**判断该ARDS是否可用*/
		if (boot_info->ARDS[n].Type != ARDS_FREE) continue;
		
		/**判断该ARDS的范围是否为0*/
		if (boot_info->ARDS[n].LengthLow == 0) continue;
		
		/**归纳信息*/
		BaseAddr = boot_info->ARDS[n].BaseAddrLow;
		Length = boot_info->ARDS[n].LengthLow;
		real_mem += Length;
		
		/**打印该范围信息*/
		//printk("available memory:%#x~%#x.\n", BaseAddr, BaseAddr + Length);
		
		/**制作相应的物理内存位页图和内核内存字节页图*/
		while (Length != 0)
		{
			/**设置空闲*/
			set_phy_page_free(BaseAddr);
			
			/**判断是否在内核内存区中*/
			if (BaseAddr < RANG_KERNEL_SIZE)
			{
				set_ker_bytemap(BaseAddr, KER_MEM_BYTEMAP_FREE);
			}
			
			/**计数增加*/
			BaseAddr += PAGE_SIZE;
			Length -= PAGE_SIZE;
		}
	}
	
	/**打印信息*/
	//printk("Register memory done.\n");
	//printk("Installed memory(RAM):%dMB(%dMB is available).\n", all_mem / 1048576, real_mem / 1048576);
	
	/**少于256MB的情况不能下一步初始化*/
	if (all_mem < 268435456)
	{
		printk("Not enough memory!Please make sure the memory more than 256MB.");
		reset();
	}
	
	/**正常返回*/
	return;
}

/**页目录表*/
unsigned long *pdt, *pt;

/**初始化分页模式函数*/
void init_paging(void)
{
	unsigned long ptr;
	
	/**分配页目录表*/
	for (pdt = NULL; pdt == NULL; )
		pdt = vmalloc(PAGE_SIZE);
	
	/**分配页表*/
	for (pt = NULL; pt == NULL; )
		pt = vmalloc((RANG_KERNEL_SIZE / PAGE_SIZE) * sizeof(pt));
	
	/**将所有页表都注册到页目录表*/
	for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	{
		pdt[ptr] = (ptr * PAGE_SIZE) + (int)pt + 0x7;
	}
	
	/**将所有内存区域的页都注册到页表中*/
	for (ptr = 0; ptr < RANG_KERNEL_SIZE / 4096; ptr ++)
	{
		pt[ptr] = ptr * PAGE_SIZE + 0x7;
	}
	
	/**进入分页模式*/
	goto_paging(pdt);
}

/**虚拟空间映射函数*/
unsigned int kmap(unsigned int vir_addr, unsigned int phy_addr, unsigned int size)
{
	/**如果虚拟地址和物理地址非4KB对齐，则错误返回*/
	if ((vir_addr & 0xfff) != 0) return 1;
	if ((phy_addr & 0xfff) != 0) return 2;
	
	/**如果映射长度非4KB对齐，也错误返回*/
	if ((size & 0xfff) != 0) return 3;

	for (;size != 0;size -= PAGE_SIZE)
	{
		pt[(vir_addr >> 12)] = phy_addr + 0x7;
		phy_addr += PAGE_SIZE;
		vir_addr += PAGE_SIZE;
	}
	
	/**正常返回*/
	return 0;
}

/**创建新页目录函数*/
unsigned long new_pdt(void)
{
	unsigned long ptr;
	unsigned long *new_pdt;
	
	/**分配内存创建新的页目录表*/
	for (new_pdt = NULL; new_pdt == NULL; )
		new_pdt = vmalloc(PAGE_SIZE);
	
	/**将已经出来的页目录表的所有页表都拷贝到新页目录表中*/
	for (ptr = 0; ptr < (RANG_KERNEL_SIZE / 4194304); ptr ++)
	{
		new_pdt[ptr] = pdt[ptr];
	}
}

/**获取一个物理页函数
 * 返回值：NULL代表获取空闲物理页失败，非NULL代表获取成功。
 */
void *get_free_page(void)
{
	unsigned long n, n2, new_page;
	unsigned int bitmap;
	
	/**先判断phy_mem_bitmap数组中是否有个32位元素不为0xffffffff*/
	// for (n = 0; n < PHY_MEM_BITMAP_SIZE / sizeof(unsigned int); n ++)
	for (n = PHY_MEM_ALLOC_START / (PAGE_SIZE * 32); n < PHY_MEM_BITMAP_SIZE / sizeof(unsigned int); n ++)
	{
		if (phy_mem_bitmap[n] != 0xffffffff)
		{
			/**如果某个元素中表示的页集中有空闲的页，则从中寻找*/
			bitmap = phy_mem_bitmap[n];
			
			/**循环看这个页集中哪个页是空闲的*/
			for (n2 = 0; n2 < 32; n2 ++)
			{
				/**进行判断相应描述的位是否为0*/
				if (((bitmap >> n2) & 1) == 0)
				{
					/**这个页的实际地址计算出来*/
					new_page = (n * 32 * PAGE_SIZE) + (n2 * PAGE_SIZE);
					
					/**设置这个页为占用*/
					set_phy_page_used(new_page);
					
					/**返回这个页*/
					return (void *)new_page;
				}
			}
		}
	}
	
	/**当运行到这里的时候，代表遍历了页位图却无合适的页，暂时只能返回NULL值，以后可以在这里实现页交换*/
	
	/**无效返回*/
	return NULL;
}

/**释放一个物理页函数*/
void free_page(void *addr)
{
	/**将该页设置为自由*/
	set_phy_page_free((unsigned long)addr);
}

/**大块内存分配函数*/
void *vmalloc(size_t size)
{
	/**停止调度*/
	disable_schedule();
	
	/**判断长度是否为0*/
	if (size == 0)
	{
		/**允许调度*/
		enable_schedule();
		return NULL;
	}
	
	unsigned long n = (KERNEL_ALLOC_START >> 12), l;
	
	/**对参数化整*/
	if ((size & 0xfff) != 0) size += 0x1000;
	size = size & 0xfffff000;
	
/**n为正在判断的空闲块的首页，l为正在判断的页*/
	/**寻找足够长的内存区域*/
	for (;;)
	{
		/**寻找到空闲区域*/
		if (ker_mem_bytemap[n] == KER_MEM_BYTEMAP_FREE)
		{
			/**循环检查该空闲块长度*/
			for (l = n;;l ++)
			{
				/**如果这个空闲块到这里为止且不够大小*/
				if (ker_mem_bytemap[l] != KER_MEM_BYTEMAP_FREE)
				{
					/**返回并且从下一个页开始找*/
					n = l + 1;
					break;
				}
				
				/**如果长度符合要求*/
				if ((l - n + 1) == (size >> 12))
				{
					/**长度合适跳转到分配模块*/
					goto allocate;
				}
			}
			
		}
		
		/**寻找下一个内存块*/
		n ++;
		
		/**判断寻找是否超过内核内存区域*/
		if (n >= KER_MEM_BYTEMAP_SIZE)
		{
			/**允许调度*/
			enable_schedule();
			/**超过了则分配失败*/
			return NULL;
		}
		
		/**判断剩余内核内存块长度是否满足要求*/
		if ((KER_MEM_BYTEMAP_SIZE - n) < (size >> 12))
		{
			/**允许调度*/
			enable_schedule();
			return NULL;
		}
	}
	
/**分配模块，该模块将相关内存块设置为已经使用*/
/**n为可用的自由内存块首地址，l为偏移*/
allocate:
	/**首先对首位进行特别标记*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_START | KER_MEM_BYTEMAP_USED;
	ker_mem_bytemap[n + (size >> 12) - 1] = ker_mem_bytemap[n + (size >> 12) - 1] | KER_MEM_BYTEMAP_END | KER_MEM_BYTEMAP_USED;
	
	/**将相关内存块都标记为占用*/
	for (l = n; (l - n + 1) != (size >> 12); l ++)
	{
		ker_mem_bytemap[l] = ker_mem_bytemap[l] | KER_MEM_BYTEMAP_USED;
	}
	
	/**允许调度*/
	enable_schedule();
	
	/**正常返回*/
	return (void *) (n << 12);
}


/**大块内存释放函数*/
void vfree(void *addr)
{
	/**判断该地址是否为这个被分配的内存块的首地址*/
	if ((ker_mem_bytemap[(unsigned long) addr >> 12] & KER_MEM_BYTEMAP_START) != KER_MEM_BYTEMAP_START)
	{
		/**输出错误信息*/
		printk("free the memory is error: this address is not the head of the block\n");
	}
	unsigned long n;
	
	/**循环回收内存块*/
	for (n = ((unsigned long) addr >> 12); ((ker_mem_bytemap[n] & KER_MEM_BYTEMAP_END) != KER_MEM_BYTEMAP_END); n ++)
	{
		/**回收内存块*/
		ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	}
	
	/**回收末尾的内存块*/
	ker_mem_bytemap[n] = KER_MEM_BYTEMAP_FREE;
	
	/**正常退出*/
	return;
}

