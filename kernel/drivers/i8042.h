/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Intel 8042 chip support definition
 * Explorer/drivers/i8042.h
 * version:Alpha
 * 11/29/2014 1:07 AM
 */

#ifndef I8042_H_
#define I8042_H_

	/**some port of Intel 8042 chip*/
	#define PORT_DATA				0x0060
	#define PORT_COMMAND			0x0064
	#define port_keysta				0x0064
	#define keysta_send_notready	0x02
	#define keycmd_write_mode		0x60
	#define kbc_mode				0x47
	
	/**send command to Intel 8042*/
	void cmd_8042(char command);
	
	/**read data from Intel 8042*/
	char read_8042(void);
	
	/**write to Intel 8042*/
	void write_8042(char b);
	
	/**wait for Intel 8042*/
	void wait_8042(void);
	
	char read_8042_cmd_byte(void);
	
	void write_8042_cmd_byte(char b);
	
	/**send command for keyboard*/
	void cmd_keyboard(char command);
	
	/**send command for mouse*/
	void cmd_mouse(char command);
	
#endif