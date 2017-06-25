// Explorer Boot
// /boot/bootc.c
// C file of the Explorer Boot

#include "lib/stdlib.h"
#include "../configure.h"
#include "../problem.h"
#include "head.h"
#include "MMU.h"
#include "lib/mem.h"
#include "storage.h"
#include "fs.h"
#include "VI.h"

// A pointer that push argument, eip and jump to the start of kernel
void (*kernel_start)(const struct boot_info *boot_info) = KERNEL_ADDR;

// 视频模式信息
struct Video_Info Video_Info;

/**显示像素函数*/
void (*putpixel)(unsigned int x, unsigned int y, unsigned int color);

/**获取像素函数*/
unsigned int (*getpixel)(unsigned int x, unsigned int y);

/**24位色彩模式获取像素函数*/
unsigned int getpixel24(unsigned int x, unsigned int y)
{
	unsigned int i;
	
	/**先判断该像素是否在屏幕上*/
	if (x < Video_Info.xres & y < Video_Info.yres)
	{
		i = ((y * Video_Info.xres) + x) * 3;
		return (Video_Info.vram[i] + (Video_Info.vram[i+1] << 8) + (Video_Info.vram[i+2] << 16));
	}
}

/**32位色彩模式获取像素函数*/
unsigned int getpixel32(unsigned int x, unsigned int y)
{
	/**先判断该像素是否在屏幕上*/
	if (x < Video_Info.xres & y < Video_Info.yres)
	{
		return ((unsigned int *)Video_Info.vram)[(y * Video_Info.xres) + x];
	}
}

/**24位色彩模式绘制像素函数*/
void putpixel24(unsigned int x, unsigned int y, unsigned int color)
{
	int i;
	unsigned char *vram = Video_Info.vram;
	
	/**先判断该像素是否在屏幕上*/
	if ((x < Video_Info.xres) & (y < Video_Info.yres))
	{
		i = ((y * Video_Info.xres) + x)*3;
		vram[i] = color;
		vram[i+1] = color >> 8;
		vram[i+2] = color >> 16;
	}
}

/**32位色彩模式绘制像素函数*/
void putpixel32(unsigned int x, unsigned int y, unsigned int color)
{
	/**先判断该像素是否在屏幕上*/
	if ((x < Video_Info.xres) & (y < Video_Info.yres))
	{
		((unsigned int *)Video_Info.vram)[(y * Video_Info.xres) + x] = color;
	}
}

/*在屏幕指定地方画方块*/
void rectangle(unsigned long x, unsigned long y, unsigned long height, unsigned long width, unsigned int color)
{
	unsigned long m, n;
	for (n = 0; n != width; n ++)
	{
		for (m = 0; m != height; m ++)
		{
			putpixel(x + m, y + n, color);
		}
	}
}

// 绘字函数
void draw_font(unsigned long x, unsigned long y, unsigned int color, unsigned char ascii)
{
	unsigned long i, font_offset;/*字库偏移量*/
	unsigned char d;
	font_offset = ascii * 16;
	for (i = 0; i < 16; i++)
	{
		d = font[font_offset + i];
		if ((d & 0x80) != 0) { putpixel(x, y + i, color); }
		if ((d & 0x40) != 0) { putpixel(x + 1, y + i, color); }
		if ((d & 0x20) != 0) { putpixel(x + 2, y + i, color); }
		if ((d & 0x10) != 0) { putpixel(x + 3, y + i, color); }
		if ((d & 0x08) != 0) { putpixel(x + 4, y + i, color); }
		if ((d & 0x04) != 0) { putpixel(x + 5, y + i, color); }
		if ((d & 0x02) != 0) { putpixel(x + 6, y + i, color); }
		if ((d & 0x01) != 0) { putpixel(x + 7, y + i, color); }
	}
}

/**屏幕上指定位置输出一行字符串*/
void outtextxy(unsigned long x, unsigned long y, unsigned int color, unsigned char *string)
{
	unsigned long point;
	for (point = 0; string[point] != 0x00; point ++)
	{
		draw_font(x, y, color, string[point]);
		x += 8;
	}
}

// 绘线函数
void line(unsigned long x0, unsigned long y0, unsigned long x1, unsigned long y1, unsigned int color)
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
			case 1:putpixel(x++, y, color);f-=dy;break;
			case 2:putpixel(x, y++, color);f-=dx;break;
			case 3:putpixel(x--, y, color);f-=dy;break;
			case 4:putpixel(x, y--, color);f-=dx;break;
		}else
		switch(k)
		{
			case 1:putpixel(x, y++, color);f+=dx;break;
			case 2:putpixel(x--, y, color);f+=dy;break;
			case 3:putpixel(x, y--, color);f+=dx;break;
			case 4:putpixel(x++, y, color);f+=dy;break;
		}
}

/**重置函数*/
void reset(void)
{
	io_out8(0x64,0xfe);
}

/**停机函数*/
void halt(void)
{
	io_hlt();
}

/**持续停机函数*/
void stillhalt(void)
{
	loop:
		io_hlt();
	goto loop;
}

/**调试函数，用于显示各种内存内容*/
void debug(void * address, unsigned long size)
{
	unsigned char count;
	unsigned char *data_ptr = address;
	
	/**如果size大小不对，则直接输出信息后直接返回*/
	if (size == 0)
	{
		printk("debug invalid.\n");
		return;
	}
	
	/**如果size不是16字节对齐，就按16字节对齐*/
	if ((size % 16) != 0)
	{
		size = size & 0xfffffff0;
		size + 16;
	}
	/**绘制表头*/
	printk ("Memory %#x to %#x:\n", address, address + size);
	printk("Offset(h)  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	/**输出结果*/
	for (; size != 0;)
	{
		printk("%08X   %02X %02X %02X %02X ", (unsigned long)data_ptr & 0xfffffff0, *(data_ptr), *(data_ptr + 1), *(data_ptr + 2), *(data_ptr + 3));
		printk("%02X %02X %02X %02X ", *(data_ptr + 4), *(data_ptr + 5), *(data_ptr + 6), *(data_ptr + 7));
		printk("%02X %02X %02X %02X ", *(data_ptr + 8), *(data_ptr + 9), *(data_ptr + 10), *(data_ptr + 11));
		printk("%02X %02X %02X %02X  ", *(data_ptr + 12), *(data_ptr + 13), *(data_ptr + 14), *(data_ptr + 15));
		
		for (count = 0; count < 16; count++)
		{
			if (*(data_ptr + count) == 0)
			{
				put_char(46);
			}else if (*(data_ptr + count) == 0x0A){
				put_char(46);
			}else{
				put_char(*(data_ptr + count));
			}
		}
		
		printk("\n");
		
		size -= 16;
		data_ptr += 16;
	}
}

extern int_0x20, int_0x21, int_0x22, int_0x23;
extern int_0x24, int_0x25, int_0x26, int_0x27;
extern int_0x28, int_0x29, int_0x2A, int_0x2B;
extern int_0x2C, int_0x2D, int_0x2E, int_0x2F;

// IDT地址及大小
void *IDT_base;
#define		IDT_size	8 * 256

/**清空中断描述符表函数*/
void clean_IDT(void)
{
	/**进行判断*/
	if (IDT_base == NULL) error (ERR_NO_MEM_FOR_ID, "IDT_base = NULL when clean the IDT");
	/**清空中断描述符表*/
	memset(IDT_base, 0, IDT_size);
}

/**中断初始化函数*/
void init_interrupt(void)
{
	extern void write_IDTR(void *base, unsigned short size);
	/**若分配内存不成功则输出错误信息*/
	IDT_base = bmalloc(IDT_size);
	if (IDT_base == NULL) error (ERR_NO_MEM_FOR_ID, "No memory for Interrupt Description Table.");
	/**初始化中断描述符表*/
	write_IDTR(IDT_base, IDT_size - 1);
	
	clean_IDT();
	/**创建中断描述符*/
	create_ID(0x20 , 0x08, &int_0x20 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x21 , 0x08, &int_0x21 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x22 , 0x08, &int_0x22 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x23 , 0x08, &int_0x23 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x24 , 0x08, &int_0x24 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x25 , 0x08, &int_0x25 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x26 , 0x08, &int_0x26 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x27 , 0x08, &int_0x27 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x28 , 0x08, &int_0x28 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x29 , 0x08, &int_0x29 , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2A , 0x08, &int_0x2A , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2B , 0x08, &int_0x2B , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2C , 0x08, &int_0x2C , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2D , 0x08, &int_0x2D , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2E , 0x08, &int_0x2E , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
	create_ID(0x2F , 0x08, &int_0x2F , interrupt_gate + IDT_32 + IDT_DPL_0 + IDT_P);
}

/**允许中断*/
void allow_interrupt(void)
{
	io_sti();
}

/**不允许中断*/
void disallow_interrupt(void)
{
	io_cli();
}

#define PORT_8042_DATA		0x60
#define PORT_8042_COMMAND	0x64

/**some port of Intel 8042 chip*/
#define PORT_DATA				0x0060
#define PORT_COMMAND			0x0064
#define port_keysta				0x0064
#define keysta_send_notready	0x02
#define keycmd_write_mode		0x60
#define kbc_mode				0x47
	
/**wait for Intel 8042*/
void wait_8042(void)
{
	for(;;)
	{
		if ((io_in8(port_keysta) & keysta_send_notready) == 0) break;
	}
	return;
}

/**write to Intel 8042*/
void write_8042(char b)
{
	wait_8042();
	io_out8(PORT_8042_DATA, b);
}

/**send command for keyboard*/
void cmd_keyboard(char command)
{
	write_8042(command);
	wait_8042();
}

/**
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * 开放8259特定中断的函数
 * 入口：IRQ号
 * 出口：(-1):错误 0:正确
 */
int open_PIC(unsigned char IRQ)
{
	if (IRQ > 15)
	{
		return (-1);
	}else if (IRQ <= 7)/*主8259*/
	{
		io_out8(PIC0_IMR, (io_in8(PIC0_IMR) & (~(1 << IRQ))));
	}else{/*从8259*/
		io_out8(PIC1_IMR, (io_in8(PIC1_IMR) & (~(1 << IRQ - 8))));
	}	
	return 0;
}

/**初始化键盘*/
void init_keyboard(void)
{
	/**初始化中断系统*/
	
	/**初始化8259A芯片*/
	/**首先将主、从8259A的中断关闭，防止出现错误*/
	io_out8(PIC0_IMR, 0xff);				/*禁止主8259的所有中断*/
	io_out8(PIC1_IMR, 0xff);				/*禁止从8259的所有中断*/

	io_out8(PIC0_ICW1, 0x11);				/*边沿触发模式*/
	io_out8(PIC0_ICW2, PIC0_intr_offset);	/*PIC 0~7 由PIC0_intr_offset ~ PIC0_intr_offset+8负责*/
	io_out8(PIC0_ICW3, 1 << 2);				/*从PIC跟主PIC的第2号中断线相连*/
	io_out8(PIC0_ICW4, 0x01);				/*无缓冲模式*/
	
	io_out8(PIC1_ICW1, 0x11);				/*边沿触发*/
	io_out8(PIC1_ICW2, PIC1_intr_offset);	/*PIC 8~15 由int 0x28~0x2f负责*/
	io_out8(PIC1_ICW3, 2);					/*从8259跟PIC 2 相连*/
	io_out8(PIC1_ICW4, 0x01);				/*无缓冲模式*/

	io_out8(PIC0_IMR, 0xfb);				/*禁止所有中断*/
	io_out8(PIC1_IMR, 0xff);				/*禁止所有中断*/
	
	/**初始化键盘*/
	cmd_keyboard(0xED);	/*设置LED灯命令*/
	cmd_keyboard(0x00);	/*设置LED状态全部关闭*/
	cmd_keyboard(0xF4);	/*清空键盘缓冲*/
	
	open_PIC(1);
	
	/**返回*/
	return;
}

/**键盘中断处理函数*/
unsigned long r = 0;
unsigned int color = 0x333333;
void keyboard_handle(void)
{
	unsigned char key_val;
	key_val = io_in8(keycmd_write_mode);
	
	/**
	 * 键盘上键是0x48，下键是0x50，
	 * F1键是0x3B，F2是0x3C
	 * Esc键是0x1
	 */
	
	/**向上键触发*/
	if (key_val == 0x48) select_up();
	
	/**向下键触发*/
	if (key_val == 0x50) select_down();
	
	/**F1键触发*/
	if (key_val == 0x3B) VI_active(VI_page_output);
	
	/**F2键触发*/
	if (key_val == 0x3C) VI_active(VI_page_select);
	
	/**Esc键触发*/
	if (key_val == 0x1) reset();
	
	/**EOI*/
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

void i8259A_auto_return(void)
{
	printk("Unexpected interrupt from Intel 8259A happened.");
	/**EOI*/
	io_out8(0x20, 0x20);
	io_out8(0xA0, 0x20);
}

/**其他中断的自动返回*/
void int_auto_return(void)
{
	printk("Unexpected interrupt happened.");
}

int callback(int n, int type)
{
	if (n == 0)
	{
		
	}else{
		
	}
}

/**configure文件储存内存指针*/
static void *config_buf;
/**保留扇区引导程序的主函数*/
void BOOT_main(const struct boot_info *boot_info)
{
	/**判断是否成功获取启动信息*/
	if (boot_info->flag[0] != 'E' |
		boot_info->flag[1] != 'B' |
		boot_info->flag[2] != 'I' )
	{
		reset();		/**系统重置*/
	}
	
	/**判断启动信息的长度是否符合要求*/
	if (boot_info->size != sizeof(struct boot_info))
	{
		reset();		/**系统重置*/
	}
	/**初始化图形模式*/
	Video_Info.xres = boot_info->ModeInfoBlock.XResolution;
	Video_Info.yres = boot_info->ModeInfoBlock.YResolution;
	Video_Info.bit_per_pixel = boot_info->ModeInfoBlock.BitsPerPixel;
	Video_Info.vram_length = (((Video_Info.xres * Video_Info.yres) * (Video_Info.bit_per_pixel / 8)) & 0xfffff000) + 0x1000;
	Video_Info.vram = (unsigned char*)boot_info->ModeInfoBlock.PhysBasePtr;
	
	/**根据BPP判断所需要的相应显示像素函数*/
	if (Video_Info.bit_per_pixel == 24)
	{
		putpixel = putpixel24;
		getpixel = getpixel24;
	}else if (Video_Info.bit_per_pixel == 32)
	{
		putpixel = putpixel32;
		getpixel = getpixel32;
	}
	
	/**初始化可视化界面*/
	init_VI();
	
	/**初始化中断描述符表*/
	init_interrupt();
	
	/**初始化键盘*/
	init_keyboard();
	
	/**允许中断*/
	allow_interrupt();	
	
	/**初始化储存器管理*/
	init_storage();
	
	/**检测活动分区的数量*/
	printk("Active storage partition:%d\n", storage_active_partition());
	
	/**初始化文件系统*/
	init_FS();
	
	/**分配放置configure的内存*/
	config_buf = bmalloc(CONFIG_MAX);
	if (config_buf == NULL) error(ERR_NO_MEM_FOR_CONFIG, "No memory for loader's configure file.");
	
	// Example
	printak("<0xaaaaff>Hello, This is Explorer loader!\n</>");
	select_register(0, callback, "show 'Hello, world!'");
	select_register(1, callback, "show 'Explorer loader'");
	io_hlt();
	
	/**加载内核*/
	read_file(0, 0, KERNEL_NAME, KERNEL_ADDR, 1);
	
	printk("Kernel address:%#X", kernel_start);
	
	/**运行内核*/
	kernel_start(boot_info);
}

