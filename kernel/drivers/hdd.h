/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer IDE Disk Support Definition
 * Explorer/drivers/hdd.h
 * version:Alpha
 * 7/18/2014 5:48 PM
 */

#ifndef HDD_H_
#define HDD_H_

#include "types.h"

struct HD_info
{
	unsigned short wGenConfig;				// WORD 0: 基本信息字
	unsigned short wNumCyls;				// WORD 1: 柱面数
	unsigned short wReserved2;				// WORD 2: 保留
	unsigned short wNumHeads;				// WORD 3: 磁头数
	unsigned short wReserved4;				// WORD 4: 保留
	unsigned short wReserved5;				// WORD 5: 保留
	unsigned short wNumSectorsPerTrack;		// WORD 6: 每磁道扇区数
	unsigned short wVendorUnique[3];		// WORD 7-9: 厂家设定值
	char sSerialNumber[20];					// WORD 10-19:序列号
	unsigned short  wBufferType;			// WORD 20: 缓冲类型
	unsigned short  wBufferSize;			// WORD 21: 缓冲大小
	unsigned short  wECCSize;				// WORD 22: ECC校验大小
	char sFirmwareRev[8];					// WORD 23-26: 固件版本
	char sModelNumber[40];					// WORD 27-46: 内部型号
	unsigned short  wMoreVendorUnique;		// WORD 47: 厂家设定值
	unsigned short  wReserved48;			// WORD 48: 保留
	struct {
		unsigned short  reserved1:8;
		unsigned short  DMA:1;				// 1=支持DMA
		unsigned short  LBA:1;				// 1=支持LBA
		unsigned short  DisIORDY:1;			// 1=可不使用IORDY
		unsigned short  IORDY:1;			// 1=支持IORDY
		unsigned short  SoftReset:1;		// 1=需要ATA软启动
		unsigned short  Overlap:1;			// 1=支持重叠操作
		unsigned short  Queue:1;			// 1=支持命令队列
		unsigned short  InlDMA:1;			// 1=支持交叉存取DMA
	}wCapabilities;							// WORD 49: 一般能力
	unsigned short  wReserved1;				// WORD 50: 保留
	unsigned short  wPIOTiming;				// WORD 51: PIO时序
	unsigned short  wDMATiming;				// WORD 52: DMA时序
	struct {
		unsigned short  CHSNumber:1;		// 1=WORD 54-58有效
		unsigned short  CycleNumber:1;		// 1=WORD 64-70有效
		unsigned short  UnltraDMA:1;		// 1=WORD 88有效
		unsigned short  reserved:13;
	}wFieldValidity;						// WORD 53: 后续字段有效性标志
	unsigned short  wNumCurCyls;			// WORD 54: CHS可寻址的柱面数
	unsigned short  wNumCurHeads;			// WORD 55: CHS可寻址的磁头数
	unsigned short  wNumCurSectorsPerTrack;	// WORD 56: CHS可寻址每磁道扇区数
	unsigned short  wCurSectorsLow;			// WORD 57: CHS可寻址的扇区数低位字
	unsigned short  wCurSectorsHigh;		// WORD 58: CHS可寻址的扇区数高位字
	struct {
		unsigned short  CurNumber:8;		// 当前一次性可读写扇区数
		unsigned short  Multi:1;			// 1=已选择多扇区读写
		unsigned short  reserved1:7;
	}wMultSectorStuff;						// WORD 59: 多扇区读写设定
	unsigned long  dwTotalSectors;			// WORD 60-61: LBA可寻址的扇区数
	unsigned short  wSingleWordDMA;			// WORD 62: 单字节DMA支持能力
	struct {
		unsigned short Mode0:1;				// 1=支持模式0 (4.17Mb/s)
		unsigned short Mode1:1;				// 1=支持模式1 (13.3Mb/s)
		unsigned short Mode2:1;				// 1=支持模式2 (16.7Mb/s)
		unsigned short  Reserved1:5;
		unsigned short Mode0Sel:1;			// 1=已选择模式0
		unsigned short Mode1Sel:1;			// 1=已选择模式1
		unsigned short Mode2Sel:1;			// 1=已选择模式2
		unsigned short  Reserved2:5;
	}wMultiWordDMA;							// WORD 63: 多字节DMA支持能力
	struct {
		unsigned short  AdvPOIModes:8;		// 支持高级POI模式数
		unsigned short  reserved:8;
	}wPIOCapacity;							// WORD 64: 高级PIO支持能力
	unsigned short wMinMultiWordDMACycle;	// WORD 65: 多字节DMA传输周期的最小值
	unsigned short wRecMultiWordDMACycle;	// WORD 66: 多字节DMA传输周期的建议值
	unsigned short wMinPIONoFlowCycle;		// WORD 67: 无流控制时PIO传输周期的最小值
	unsigned short wMinPOIFlowCycle;		// WORD 68: 有流控制时PIO传输周期的最小值
	unsigned short wReserved69[11];			// WORD 69-79: 保留
	struct {
		unsigned short Reserved1:1;
		unsigned short ATA1:1;				// 1=支持ATA-1
		unsigned short ATA2:1;				// 1=支持ATA-2
		unsigned short ATA3:1;				// 1=支持ATA-3
		unsigned short ATA4:1;				// 1=支持ATA/ATAPI-4
		unsigned short ATA5:1;				// 1=支持ATA/ATAPI-5
		unsigned short ATA6:1;				// 1=支持ATA/ATAPI-6
		unsigned short ATA7:1;				// 1=支持ATA/ATAPI-7
		unsigned short ATA8:1;				// 1=支持ATA/ATAPI-8
		unsigned short ATA9:1;				// 1=支持ATA/ATAPI-9
		unsigned short ATA10:1;				// 1=支持ATA/ATAPI-10
		unsigned short ATA11:1;				// 1=支持ATA/ATAPI-11
		unsigned short ATA12:1;				// 1=支持ATA/ATAPI-12
		unsigned short ATA13:1;				// 1=支持ATA/ATAPI-13
		unsigned short ATA14:1;				// 1=支持ATA/ATAPI-14
		unsigned short Reserved2:1;
	}wMajorVersion;							// WORD 80: 主版本
	unsigned short wMinorVersion;			// WORD 81: 副版本
	unsigned short wReserved82[6];			// WORD 82-87: 保留
	struct {
		unsigned short Mode0:1;				// 1=支持模式0 (16.7Mb/s)
		unsigned short Mode1:1;				// 1=支持模式1 (25Mb/s)
		unsigned short Mode2:1;				// 1=支持模式2 (33Mb/s)
		unsigned short Mode3:1;				// 1=支持模式3 (44Mb/s)
		unsigned short Mode4:1;				// 1=支持模式4 (66Mb/s)
		unsigned short Mode5:1;				// 1=支持模式5 (100Mb/s)
		unsigned short Mode6:1;				// 1=支持模式6 (133Mb/s)
		unsigned short Mode7:1;				// 1=支持模式7 (166Mb/s) ???
		unsigned short Mode0Sel:1;			// 1=已选择模式0
		unsigned short Mode1Sel:1;			// 1=已选择模式1
		unsigned short Mode2Sel:1;			// 1=已选择模式2
		unsigned short Mode3Sel:1;			// 1=已选择模式3
		unsigned short Mode4Sel:1;			// 1=已选择模式4
		unsigned short Mode5Sel:1;			// 1=已选择模式5
		unsigned short Mode6Sel:1;			// 1=已选择模式6
		unsigned short Mode7Sel:1;			// 1=已选择模式7
	}wUltraDMA;								// WORD 88:  Ultra DMA支持能力
	unsigned short wReserved89[167];		// WORD 89-255
};

struct Partition_Table{
	u8 active;
	char code[15];
};

struct Master_Boot_Record{
	char code[446];
	struct Partition_Table PT_1, PT_2, PT_3, PT_4;
	short int valid;
};

u32 LBA_start;
void inti_hdd(void);
void read_disk(u32 LBA, u16 *buffer, u32 number);
void hdd_wait(void);

/**磁盘参数获取函数*/
static void HD_get_phy_info(struct HD_info *info);

void int_HDC_handle(void);

#endif

