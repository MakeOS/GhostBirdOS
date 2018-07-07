/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer CMOS support Definition
 * Explorer/arch/x86/kernel/cmos.h
 * version:Aplha
 * 12/6/2014 8:58 PM
 */

#ifndef CMOS_H_
#define CMOS_H_

/**CMOS的读写端口*/
#define CMOS_INDEX		0x70
#define CMOS_DATA		0x71

/**CMOS中相关信息偏移*/
#define CMOS_CUR_SEC	0x0			//CMOS中当前秒值(BCD)
#define CMOS_ALA_SEC	0x1			//CMOS中报警秒值(BCD)
#define CMOS_CUR_MIN	0x2			//CMOS中当前分钟(BCD)
#define CMOS_ALA_MIN	0x3			//CMOS中报警分钟(BCD)
#define CMOS_CUR_HOUR	0x4			//CMOS中当前小时(BCD)
#define CMOS_ALA_HOUR	0x5			//CMOS中报警小时(BCD)
#define CMOS_WEEK_DAY	0x6			//CMOS中一周中当前天(BCD)
#define CMOS_MON_DAY	0x7			//CMOS中一月中当前日(BCD)
#define CMOS_CUR_MON	0x8			//CMOS中当前月份(BCD)
#define CMOS_CUR_YEAR	0x9			//CMOS中当前年份(BCD)
#define CMOS_DEV_TYPE	0x12		//CMOS中驱动器格式
#define CMOS_CUR_CEN	0x32		//CMOS中当前世纪(BCD)

/**内容结构体*/
struct CMOS
{
	unsigned char current_second;
	unsigned char alarm_second;
	unsigned char current_minute;
	unsigned char alarm_minute;
	unsigned char current_hour;
	unsigned char alarm_hour;
	unsigned char current_day_of_week;
	unsigned char current_day;
	unsigned char current_month;
	unsigned char current_year;
	unsigned char status_registers[4];
	unsigned char diagnostic_status;
	unsigned char shutdown_code;
	unsigned char drive_types;
	unsigned char reserved_x;
	unsigned char disk_1_type;
	unsigned char reserved;
	unsigned char equipment;
	unsigned char lo_mem_base;
	unsigned char hi_mem_base;
	unsigned char hi_exp_base;
	unsigned char lo_exp_base;
	unsigned char fdisk_0_type;
	unsigned char fdisk_1_type;
	unsigned char reserved_2[19];
	unsigned char hi_check_sum;
	unsigned char lo_check_sum;
	unsigned char lo_actual_exp;
	unsigned char hi_actual_exp;
	unsigned char century;
	unsigned char information;
	unsigned char reserved_3[12];
};

#endif