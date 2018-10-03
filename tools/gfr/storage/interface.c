/**
 * Date:2018/9/19 15:35
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * Storage support
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"
#include "../include/return.h"
#include "../include/config.h"
#include "../include/storage.h"

RET stg_gets(Storage *sd, dq LBA, void *dst)
{
	
}

RET stg_puts(Storage *sd, dq LBA, void *src)
{
	
}

RET stg_close(Storage *sd)
{
	free(sd);
	return RET_FAIL;
}

Storage *stg_open(const db *location)
{
	Storage *sd = NULL;
	const db *ext_ptr;
	
	while (!sd) sd = malloc(sizeof(Storage));
	memset(sd, 0, sizeof (Storage));
	
	printf("\nOpening storage...\n\n");
	
	// Open this storage file
	sd->file = fopen(location, "rb+");
	if (!sd->file)
	{
		printf("\topen %s error!\n", location);
		return NULL;
	}
	
	// Find out extension name of this file
	ext_ptr = location + strlen(location) - 1;
	while (ext_ptr --) if (*ext_ptr == '.') break;
	ext_ptr ++;
	
	if (!strcmp(ext_ptr, "vhd"))
		return stg_vhd_init(sd);
}
