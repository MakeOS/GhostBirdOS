/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Lab Explorer Developers<1@GhostBirdOS.org>
 * Intel 8042 chip support
 * Explorer/drivers/i8042.c
 * version:Alpha
 * 11/29/2014 12:52 AM
 */

#include "i8042.h"

#define PORT_8042_DATA		0x60
#define PORT_8042_COMMAND	0x64

/**send command to Intel 8042*/
void cmd_8042(char command)
{
	wait_8042();
	io_out8(PORT_8042_COMMAND, command);
}

/**read data from Intel 8042*/
char read_8042(void)
{
	wait_8042();
	return io_in8(PORT_8042_DATA);
}

/**write to Intel 8042*/
void write_8042(char b)
{
	wait_8042();
	io_out8(PORT_8042_DATA, b);
}

/**wait for Intel 8042*/
void wait_8042(void)
{
	for(;;)
	{
		if ((io_in8(port_keysta) & keysta_send_notready) == 0)
		{
			break;
		}
	}
	return;
}

char read_8042_cmd_byte(void)
{
	cmd_8042(0x20);
	return read_8042();
}

void write_8042_cmd_byte(char b)
{
	cmd_8042(0x60);
	write_8042(b);
	return ;
}

/**send command for keyboard*/
void cmd_keyboard(char command)
{
	write_8042(command);
	wait_8042();
}

/**send command for mouse*/
void cmd_mouse(char command)
{
	cmd_8042(0xD4);/**8042 knows that the command is send for mouse*/
	write_8042(command);
}

void enable_mouse(void)
{
	char b;
	b = read_8042_cmd_byte();
	b = (b & 0b11011111);
	b = (b | 0b10);
	write_8042_cmd_byte(b);
	cmd_8042(0xA8);
}

void disable_mouse(void)
{
	write_8042_cmd_byte(read_8042_cmd_byte() | 0b100000);
}

void enable_keyboard(void)
{
	write_8042_cmd_byte(read_8042_cmd_byte() & 0b11101111);
}

