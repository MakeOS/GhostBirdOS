/**
 * Copyright 2013-2015 by Explorer Developers.
 * Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Graphical User Interface Window System Picture
 * Explorer/GUI/window/picture.c
 * version:Alpha
 * 2/16/2015 7:55 PM
 */

#include <GUI.h>
#include <time.h>
#include <studio.h>
#include <stdlib.h>
#include <cfs.h>
#include <memory.h>
#include <lib/graphics.h>
#include "window.h"
#include "../layer.h"

#pragma pack(push)					//保存当前对齐信息
#pragma pack(1)						//设定结构体以一个字节为单位对齐

/**BMP位图文件头结构*/
#define NUM_OF_TYPE		2
struct bmp_file_header
{
	char type[NUM_OF_TYPE];			//位图类别，在Windows中，这个字段为"BM"
	unsigned int size;				//长度
	unsigned int reserved;			//保留，总为0
	unsigned int bmp_data_offset;	//从文件开始到位图数据开始之间的数据(bitmap data)之间的偏移量
	unsigned int bmp_header_size;	//位图信息头(bitmap info header)的长度
	unsigned int length;			//位图长度
	unsigned int width;				//位图宽度
	unsigned short planes;			//位图的位面数(总是1)
};

/**BMP图像信息头
 * 压缩说明
 * 0 - 不压缩 (使用BI_RGB表示)
 * 1 - RLE 8-使用8位RLE压缩方式(用BI_RLE8表示)
 * 2 - RLE 4-使用4位RLE压缩方式(用BI_RLE4表示)
 * 3 - Bitfields-位域存放方式(用BI_BITFIELDS表示)
 */
struct bmp_info_header
{
	unsigned short bpp;				//每个像素位数(1/4/8/16/24/32分别代表单色/16色/256色/16bit/24bit/32bit)
	unsigned int compression;		//压缩说明
	unsigned int bmp_data_size;		//字节表示的位图数据大小(4的倍数)
	unsigned int HResolution;		//用像素/米表示水平分辨率
	unsigned int VResolution;		//用像素/米表示垂直分辨率
	unsigned int colors;			//位图使用的颜色数
	unsigned int impt_colors;		//指定重要的颜色数
};

#pragma pack(pop)					//恢复原来的对齐单位

/**
 * BMP格式图片加载函数
 * 成功返回0，不成功返回1
 */
int *window_load_bmp(struct GUI_image *image, char *buffer)
{	
	/**写指针和读指针*/
	unsigned long wptr, rptr;
	
	unsigned char *data;
	
	/**BMP数据区指针*/
	unsigned char *bmp_data;
	
	/**BMP位图文件头指针*/
	struct bmp_file_header *head = (struct bmp_file_header *) buffer;
	
	/**BMP位图信息头指针*/
	struct bmp_info_header *info_head = (struct bmp_info_header *) (buffer + sizeof(struct bmp_file_header));
	
	/**输出信息*/
	// window_print(GUI_control, "BMP size:%dByte,length:%d,width:%d,bpp:%d.", head->size, head->length, head->width, info_head->bpp);
	
	/**获取数据区起始地址*/
	bmp_data = buffer + head->bmp_data_offset;
	
	/**准备数据区放置抽象过的数据*/
	for (data = NULL; data == NULL; )
		data = vmalloc(head->length * head->width * 4);
	
	/**根据每个像素位数使用不同拷贝方法*/
	switch(info_head->bpp)
	{
		case 24: goto copy_24;
		case 32: goto copy_32;
	}
	
/**24位位图拷贝数据*/
copy_24:
	/**循环拷贝*/
	wptr = 0;
	rptr = (head->width - 1) * head->length * 3;
	for (; wptr < head->length * head->width * 4;)
	{
		data[wptr + 0] = bmp_data[rptr + 0];
		data[wptr + 1] = bmp_data[rptr + 1];
		data[wptr + 2] = bmp_data[rptr + 2];
		data[wptr + 3] = 0xff;
		wptr += 4;
		rptr += 3;
		
		/**向上填充*/
		if (rptr % (head->length * 3) == 0)
		{
			rptr -= head->length * 3 * 2;
		}
	}
	goto finish;
	
/**32位位图拷贝数据*/
copy_32:	
	/**循环拷贝*/
	wptr = 0;
	rptr = (head->width - 1) * head->length;
	for (; wptr < head->length * head->width;)
	{
		((unsigned int *)data)[wptr] = ((unsigned int *)bmp_data)[rptr];
		wptr ++;
		rptr ++;
		
		/**向上填充*/
		if (rptr % (head->length) == 0)
		{
			rptr -= head->length * 2;
		}
	}
	goto finish;
	
finish:
	/**填充*image*/
	image->data = (unsigned int *)data;
	image->length = head->length;
	image->width = head->width;
	
	/**正常返回*/
	return 0;
}

/**所有支持的文件的头结构联合体*/
union image_head_union
{
	struct bmp_file_header bmp_file_header;
};

/**加载图片函数*/
struct GUI_image *window_load_image(char *filename)
{
	/**文件信息结构体*/
	file_info image_info;
	
	/**文件缓冲区指针*/
	char *buffer;
	
	/**文件头结构联合体*/
	union image_head_union *head_union;
	
	/**返回的抽象图形结构指针*/
	struct GUI_image *retval;
	
	/**获取文件信息*/
	image_info = get_file_info(filename);
	
	/**申请符合大小的缓冲区*/
	for (buffer = NULL; buffer == NULL; )
		buffer = vmalloc(image_info.size);
	
	/**申请符合大小的抽象图形结构使用的内存*/
	for (retval = NULL; retval == NULL; )
		retval = kmalloc(sizeof(struct GUI_image), 0);
	
	
	/**加载图片文件*/
	file_open(filename, buffer);
	
	/**头结构体指针指向文件头部*/
	head_union = (union image_head_union *) buffer;
	
/**判断图片格式，分别使用不同的转换函数转换*/
	
	/**判断是否是BMP图片*/
	if (head_union->bmp_file_header.type[0] == 'B' &
		head_union->bmp_file_header.type[1] == 'M' )
	{
		/**是BMP图片*/
		// window_print(GUI_control, "image type:Microsoft Windows BMP Image");
		
		/**调用BMP图片加载函数*/
		window_load_bmp(retval, buffer);
		
		/**释放缓冲区*/
		vfree(buffer);
		// window_print(GUI_control, "GUI image length:%d,width:%d,data:%#X", retval->length, retval->width, retval->data);
	}else{
		/**其他格式文件*/
		// window_print(GUI_control, "Unkown Format.");
		
		/**释放相关结构占用的内存、文件占用的缓存*/
		vfree(buffer);
		kfree(retval);
		
		/**失败返回*/
		return NULL;
	}
	
	/**正常返回*/
	return retval;
}

/**释放图片资源函数*/
void free_image(struct GUI_image *image)
{
	/**判断是否需要释放*/
	if (image->data == NULL) return;
	
	/**释放image的数据区*/
	vfree(image->data);
	
	/**释放image的结构体*/
	kfree(image);
	
	/**置空指针*/
	image = NULL;
	
	/**正常返回*/
}
