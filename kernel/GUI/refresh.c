/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Refresh
 * Explorer/GUI/refresh.c
 * version:Alpha
 * 12/14/2014 1:59 PM: created
 * 12/22/2014 3:37 PM: previous file code about refresh add to this file
 */

#include <stdlib.h>
#include <GUI.h>
#include <lib/graphics.h>
#include "layer.h"

/**刷新像素函数
 * (x,y)是在屏幕上需要刷新的像素坐标
 * 该函数为通用函数，仅为兼容性考虑。
 * 在具体平台上建议用相应的汇编语言实现该函数以获得最大效能
 */
long inline int GUI_refresh_pix(unsigned long int x, unsigned long int y)
{
	
	unsigned int new_color;
	unsigned char alpha = 0xff, alpha_cal = 0xff, red = 0, green = 0, blue = 0;
	struct layer *layer = layer_root;
	
	
	/**层叠像素*/
	do
	{
		/**判断该图层是否被隐藏*/
		if (layer->visiable == true)
		{
			/**获取图层的该位置像素颜色*/
			new_color = GUI_get_pix(layer, x - (*layer).x, y - (*layer).y);
			
			/**获取透明度*/
			alpha = (alpha_cal * (new_color >> 24)) / 0xff;
			alpha_cal = (alpha_cal * (0xff - (new_color >> 24))) / 0xff;
			
			/**对透明度判断，如果不透明则直接返回*/
			if (alpha == 0xff)
			{
				/**写入显存中*/
				putpixel(x, y, new_color);
				return;
			}
			
			/**颜色堆加*/
			red		+= ((((new_color >> 16) & 0xff) * alpha) / 0xff);
			green	+= ((((new_color >> 8)  & 0xff) * alpha) / 0xff);
			blue	+= ((( new_color        & 0xff) * alpha) / 0xff);
		}
		
		/**指向下一个图层*/
		layer = layer->bottom;
		
	/**当轮完全部图层时结束*/
	}while (layer != layer_root);

	/**写入显存中*/
	putpixel(x, y, (red << 16) + (green << 8) + (blue));
}

/**GUI块刷新函数*/
void GUI_refresh_block(long x, long y, unsigned long length, unsigned long width)
{
	unsigned long offset_x, offset_y;
	
	/**循环向下刷新*/
	for (offset_y = 0; offset_y < width; offset_y ++)
	{
		/**循环向右刷新*/
		for (offset_x = 0; offset_x < length; offset_x ++)
		{
			/**刷新像素*/
			GUI_refresh_pix(x + offset_x, y + offset_y);
		}
	}
}

/*refresh all the layer_control*/
long int GUI_refresh(void)
{
	unsigned long x, y;
	for (x = 0; x < Video_Info.xres; x ++)
	{
		for (y = 0; y < Video_Info.yres; y ++)
		{
			GUI_refresh_pix(x, y);
		}
	}
}