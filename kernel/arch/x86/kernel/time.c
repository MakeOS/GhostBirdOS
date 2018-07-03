/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Time
 * Explorer/arch/x86/kernel/time.c
 * version:Aplha
 * 1/17/2014 9:52 PM
 */

#include "cmos.h"
#include <time.h>
#include <types.h>

/**Explorer时间管理*/

/**时钟相关的变量*/
ulong century;					/**世纪*/
ulong year, mon, day;			/**年、月、日*/
ulong hour, min, sec;			/**时、分、秒*/
ulong week_day;					/**一周中当前天*/
ulong ms = 0;					/**微秒*/
ulong system_runtime = 0;		/**系统运行时间，微秒计时，一开始归0*/

/**BCD码转换成16进制方法*/
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf)

void init_time(void)
{
	/**获取CMOS芯片上的年、月、日、时、分、秒*/
	sec = BCD_HEX(read_cmos(CMOS_CUR_SEC));			/**当前秒*/
	min = BCD_HEX(read_cmos(CMOS_CUR_MIN));			/**当前分*/
	hour = BCD_HEX(read_cmos(CMOS_CUR_HOUR));		/**当前时*/
	week_day = BCD_HEX(read_cmos(CMOS_WEEK_DAY));	/**一周中当前天*/
	day = BCD_HEX(read_cmos(CMOS_MON_DAY));			/**一月中当前日*/
	mon = BCD_HEX(read_cmos(CMOS_CUR_MON));			/**当前月*/
	year = BCD_HEX(read_cmos(CMOS_CUR_YEAR));		/**当前年*/
	century = BCD_HEX(read_cmos(CMOS_CUR_CEN));		/**当前世纪*/
	/**CMOS中的年份仅仅是从本世纪开始到现在的绝对值，需要加上世纪*/
	year += century * 100;
	printk("time:century:%d year:%d month:%d day:%d week:%d hour:%d min:%d sec:%d\n", century, year, mon, day, week_day, hour, min, sec);
}