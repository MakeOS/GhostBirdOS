/**
 * Date:2018/9/19 15:33
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * Ghost Bird File System - format
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#include <stdlib.h>
#include <stdio.h>
#include "../include/types.h"
#include "../include/return.h"
#include "../include/storage.h"
#include "../include/gbfs.h"

/**
计算FAT、

**/

GBFS *gbfs_fmt(Storage *sd, db partition)
{
	GBFS *gd = NULL;
	
	if(partition >= 4)
	{
		printf("Partition number cannot more than or equal 4.\n");
		return NULL;
	}
	
	printf("\nFormating partition[%d]...\n\n", partition);
	
	while (!gd) gd = malloc(sizeof(GBFS));
	memset(gd, 0, sizeof(GBFS));
	
	sd->mbr->pt[partition];
	
	gd->sd = sd;
	gd->partition = partition;
	
	
	
	return gd;
}
