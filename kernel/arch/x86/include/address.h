/*Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.

Explorer 0.01 内存分布
File name:Explorer\Kernel\include\address.h
2014.7.10 2:37 PM
*/

#ifndef ADDRESS_H_
#define ADDRESS_H_

/*对单位定义*/
#define KB  * 1024
#define MB	* 1048576
/*对内存区间图的定义*/
#define mem_map_addr	0x30000			/*起始地址*/
#define mem_map_size	128 KB			/*内存段长度*/
#define mem_map_len		(128 KB)/8		/*表项长度*/
/*对内核内存区间图的定义*/
#define k_mem_map_addr	0x50000
#define k_mem_map_size	64 KB
#define k_mem_map_len	(k_mem_map_size)/8
/*对全局描述符表的定义*/
#define GDT_addr		0x60000
#define GDT_len			8192
#define GDT_size		(GDT_len * 8)

#endif