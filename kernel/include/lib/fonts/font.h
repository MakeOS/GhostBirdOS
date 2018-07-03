/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com>
 * Explorer Font Header
 * Explorer/include/font.h
 * version:Alpha
 * 18/10/2014
 */


#ifndef FONT_H_
#define FONT_H_

/**字库信息描述结构体*/
struct font_info{
	char *name, *addr, *copr;
	unsigned long length, width;
};

/**初始化字库管理函数*/
void init_Font(void);

/**注册字库函数*/
long register_font(struct font_info *font_info);

/**获取字体信息函数*/
struct font_info *font(char *name);

#endif