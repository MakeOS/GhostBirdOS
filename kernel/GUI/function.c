/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Function Library
 * Explorer/GUI/GUI.c
 * version:Alpha
 * 8/16/2014 9:08 AM
 */

#include <GUI.h>
#include <lib/fonts/font.h>
#include <lib/math.h>
#include "layer.h"

/**在指定图层的指定位置显示一个字符串*/
void GUI_put_string(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width, struct font_info *font_info, const char *string)
{
	while (*string != 0x00)
	{
		GUI_put_word(layer, color, x, y, length, width, font_info, *string);
		string ++;
		x += font_info->length;
	}
}

/**put word to layer*/
void GUI_put_word(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width, struct font_info *font_info, unsigned char ascii)
{
	unsigned long p, i, font_offset;/*offset in font*/
	unsigned char d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		/**对准一行点阵*/
		d = font_info->addr[font_offset + i];
		
		/**对每行点阵进行逻辑计算后判断*/
		if ((d & 0x80) != 0) { GUI_put_pixel(layer, color, x + 0, y + i, length, width); }
		if ((d & 0x40) != 0) { GUI_put_pixel(layer, color, x + 1, y + i, length, width); }
		if ((d & 0x20) != 0) { GUI_put_pixel(layer, color, x + 2, y + i, length, width); }
		if ((d & 0x10) != 0) { GUI_put_pixel(layer, color, x + 3, y + i, length, width); }
		if ((d & 0x08) != 0) { GUI_put_pixel(layer, color, x + 4, y + i, length, width); }
		if ((d & 0x04) != 0) { GUI_put_pixel(layer, color, x + 5, y + i, length, width); }
		if ((d & 0x02) != 0) { GUI_put_pixel(layer, color, x + 6, y + i, length, width); }
		if ((d & 0x01) != 0) { GUI_put_pixel(layer, color, x + 7, y + i, length, width); }
	}
}

/**draw a line in the layer*/
void GUI_line(struct layer *layer, unsigned int color, unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1)
{
	int dx,dy,n,k,i,f;
	int x,y;
	dx=abs(x1-x0);
	dy=abs(y1-y0);
	n=dx+dy;
	
	/**根据x1和x0的关系，选择合适的k运算方法*/
	if (x1 == x0)
	{
		k=2;
		x=x0;
		y=y0;
	}else if(x1>=x0)
	{
		k=y1>=y0?1:4;
		x=x0;
		y=y0;
	}else{
		k=y1>=y0?2:4;
		x=x0;
		y=y0;
	}

	for(i=0,f=0;i<n;i++)
		if(f>=0)
		switch(k)
		{
			case 1:GUI_put_pixel(layer, color, x++, y, 0, 0);f -= dy;break;
			case 2:GUI_put_pixel(layer, color, x, y++, 0, 0);f -= dx;break;
			case 3:GUI_put_pixel(layer, color, x--, y, 0, 0);f -= dy;break;
			case 4:GUI_put_pixel(layer, color, x, y--, 0, 0);f -= dx;break;
		}else
		switch(k)
		{
			case 1:GUI_put_pixel(layer, color, x, y++, 0, 0);f += dx;break;
			case 2:GUI_put_pixel(layer, color, x--, y, 0, 0);f += dy;break;
			case 3:GUI_put_pixel(layer, color, x, y--, 0, 0);f += dx;break;
			case 4:GUI_put_pixel(layer, color, x++, y, 0, 0);f += dy;break;
		}
}

/**draw a square in the layer*/
long int GUI_put_square(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/**Two pointers to determine coordinates*/
	unsigned long int x_point, y_point;
	/**
	 * Don't worried about the square overflow the buffer
	 * because every pixel check in the GUI_put_pixel function
	 */
	for (y_point = 0; y_point < width; y_point++)
	{
		for (x_point = 0; x_point < length; x_point++)
		{
			GUI_put_pixel(layer, color, (x + x_point), (y + y_point), 0, 0);
		}
	}
}

/**
 * 画一个像素至图层上
 * made by Lab Explorer<2322869088@qq.com>
 * layer为要画的目标图层，color为要绘制的像素颜色(32-bit)
 * (x,y)是像素的坐标，length和width是用于人为边界判断，0代表仅作图层边框判断
 * 
 * 人为边界判断：窗口作为加入窗口元素的图层，在被画上数据时，容易因为无有效判断而
 * 绘制到窗口本身的部分。程序绘制控件的时候也容易画到其他控件的区域。为了解决这个
 * 问题，加入人为边界判断方法，对像素是否超出范围进行有效的判断。从防止绘制过界的
 * 角度出发，不光要有边界判断(因为边界判断仅仅判断是否会溢出到右边或下边)，还要
 * 在一开始就对整体绘制元素的坐标进行纠正，以不会溢出到左边和上边。
 */
void GUI_put_pixel(struct layer *layer, unsigned int color, unsigned long x, unsigned long y, unsigned long length, unsigned long width)
{
	/**判断是否需要人为边界判断*/
	if ((length != 0) && (width != 0))
	{
		/**进行人为边界判断*/
		if ((x >= length) | (y >= width)) return;
	}
		
	/**判断这个像素是否超出图层边界，如果超出，会覆盖其它内存数据*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return;
	
	/**绘制*/
	(*layer).buf[(y * (*layer).length) + x] = color;
	
	/**刷新像素*/
	GUI_refresh_pix((*layer).x + x, (*layer).y + y);
	
	/**正常返回*/
	return;
}

/**get pixel from layer*/
unsigned int GUI_get_pix(struct layer *layer, unsigned long x, unsigned long y)
{
	/**There is a judgement about if the pixel in the layer*/
	/**If there isn't a judgement,the pixel may be overflow the buffer*/
	if ((x >= (*layer).length) | (y >= (*layer).width)) return 0;
	return (*layer).buf[(y * (*layer).length) + x];
}