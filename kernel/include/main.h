/**
 * Copyright 2013-2015 by 2013-2015 by Explorer OS Developers. All rights reserved.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer 0.01 function main definition
 * Explorer/include/main.h
 * 7/24/2014 10:56 AM
 */

#ifndef MAIN_H_
#define MAIN_H_

/**在_start.asm中放置了boot_info_ptr指针指向boot_info*/
extern struct boot_info *boot_info_ptr;

/**地址范围描述符结构体*/
#define ARDS_FREE	1
#define ARDS_RESV	2
struct Address_Range_Descriptor_Structure
{
	unsigned int BaseAddrLow;
	unsigned int BaseAddrHigh;
	unsigned int LengthLow;
	unsigned int LengthHigh;
	unsigned int Type;
};

/**实模式Segment:Offset地址转换成保护模式地址*/
#define real_addr_in_pm(Segment, Offset)	(Segment * 0x10 + Offset)

#pragma pack(push)					//保存当前对齐信息
#pragma pack(1)						//设定结构体以一个字节为单位对齐

/**VBE信息块结构体*/
#define VBE_SIGN_NUM	4
struct VbeInfoBlock
{
	unsigned char VbeSignature[VBE_SIGN_NUM];
	unsigned short VbeVersion;
	unsigned short OemStringPtr_Off;
	unsigned short OemStringPtr_Seg;
	unsigned char Capabilities[4];
	unsigned short VideoModePtr_Off;
	unsigned short VideoModePtr_Seg;
	unsigned short TotalMemory;
	unsigned short OemSoftwareRev;
	unsigned int OemVendorNamePtr;
	unsigned int OemProductNamePtr;
	unsigned int OemProductRevPtr;
	unsigned char Reserved[222];
	unsigned char OemData[256];
};

struct ModeInfoBlock
{
	/**Mandatory information for all VBE revisions*/
	unsigned short ModeAttributes;
	unsigned char WinAAttributes;
	unsigned char WinBAttributes;
	unsigned short WinGranularity;
	unsigned short WinSize;
	unsigned short WinASegment;
	unsigned short WinBSegment;
	unsigned int WinFuncPtr;
	unsigned short BytesPerScanLine;
	
	/**Mandatory information for VBE 1.2 and above*/
	unsigned short XResolution;
	unsigned short YResolution;
	unsigned char XCharSize;
	unsigned char YCharSize;
	unsigned char NumberOfPlanes;
	unsigned char BitsPerPixel;
	unsigned char NumberOfBanks;
	unsigned char MemoryModel;
	unsigned char BankSize;
	unsigned char NUmberOfImagePages;
	unsigned char Reserved1;
	
	/**Direct Color fields(required for direct/6 and YUV/7 memory models)*/
	unsigned char RedMaskSize;
	unsigned char RedFieldPosition;
	unsigned char GreenMaskSize;
	unsigned char GreenFieldPosition;
	unsigned char BlueMaskSize;
	unsigned char BlueFieldPosition;
	unsigned char RsvdMaskSize;
	unsigned char RsvdFieldPosition;
	unsigned char DirectColorModeInfo;
	
	/**Mandatory infomation for VBE 2.0 and above*/
	unsigned int PhysBasePtr;
	unsigned int Reserved2;
	unsigned short Reserved3;
	
	/**Mandatory information for VBE 3.0 and above*/
	unsigned short LinBytesPerScanLine;
	unsigned char BnkNumberOfImagePages;
	unsigned char LinNumberOfImagePages;
	unsigned char LinRedMaskSize;
	unsigned char LinRedFieldPosition;
	unsigned char LinGreenMaskSize;
	unsigned char LinGreenFieldPosition;
	unsigned char LinBlueMaskSize;
	unsigned char LinBlueFieldPosition;
	unsigned char LinRsvdMaskSize;
	unsigned char LinRsvdFieldPosition;
	unsigned int MaxPixelClock;
	unsigned char Reserved4[189];
	unsigned char Reserved5;
};

#define BOOT_FLAG_MAX	4
#define BOOT_ARDS_NUM	8
/**启动信息结构体*/
struct boot_info
{
	/**信息一定要是"EBI"(Explorer Boot Information)*/
	char flag[BOOT_FLAG_MAX];
	/**启动信息长度*/
	unsigned int size;
	/**内存分布信息*/
	struct Address_Range_Descriptor_Structure ARDS[BOOT_ARDS_NUM];
	
	/**VBE信息块结构体*/
	struct VbeInfoBlock VbeInfoBlock;
	/**VBE模式信息结构体*/
	struct ModeInfoBlock ModeInfoBlock;
};

#pragma pack(pop)					//恢复原来的对齐单位

/**内核主函数*/
void main(void);

/**内存管理相关信息输出函数*/
void output_mem_info(void);

/**输出内核信息*/
void output_kernel_info(void);

/**输出VBE信息*/
void output_viedo_info(void);

#endif
