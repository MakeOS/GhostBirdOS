/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com>
 * Explorer Font Register
 * Explorer/lib/fonts/font.c
 * version:Alpha
 * 18/10/2014
 */

#include <lib/fonts/font.h>
#include <lib/string.h>
#include <stdlib.h>

/**字库列表*/
#define SIZE_OF_FONT 10
struct font_info font_lib[SIZE_OF_FONT];

void init_Font(void)
{
	/**empty the list*/
	unsigned long n;
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		font_lib[n].name = NULL;
	}
	
	/**register font*/
	Register_standard_font();
	Register_simsun();
}

/**注册字库函数*/
long register_font(struct font_info *font_info)
{
	unsigned long n;
	
	/**循环判断字库信息库数组中是否存在空元素*/
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		/**如果寻找到了空元素则跳转到相应执行函数*/
		if (font_lib[n].name == NULL) goto empty_font;
	}
	
	/**如果没有找到则失败返回*/
	return -1;
	
empty_font:
	/**放置字库信息*/
	font_lib[n] = *font_info;
	
	/**正常返回*/
	return 0;
}

/**获得字体信息函数*/
struct font_info *font(char *name)
{
	unsigned long n;
	
	/**遍历寻找字库中的相应函数*/
	for (n = 0; n < SIZE_OF_FONT; n++)
	{
		/**判断字符名称是否相同*/
		if (strncmp(font_lib[n].name, name, strlen(font_lib[n].name)) == 0)
		{
			/**相同则返回相应的字体信息*/
			return &font_lib[n];
		}
	}
	
	/**运行到这里就是没有寻找到，返回空指针*/
	return NULL;
}
