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
#include <string.h>
#include <stdio.h>
#include "../include/types.h"
#include "../include/return.h"
#include "../include/storage.h"
#include "../include/gbfs.h"
#include "../include/ui.h"

GBFS *gbfs_fmt(Storage *sd, db partition)
{
	GBFS *gd = NULL;
	dd page_sctor;
	dd b, n, m;
	Page *root = NULL;
	
	printf("\nFormating partition[%d]...\n\n", partition);
	
	if (partition >= 4)
	{
		printf("\tPartition number cannot more than or equal 4.\n");
		return NULL;
	}
	
	if (!sd->mbr->pt[partition].size_s)
	{
		printf("\tThis partition does not exist.\n");
		return NULL;
	}
	
	while (!gd) gd = malloc(sizeof(GBFS));
	memset(gd, 0, sizeof(GBFS));	
	while (!gd->pbr) gd->pbr = malloc(sizeof(PBR));
	while (!gd->fat) gd->fat = malloc(sizeof(Sector));
	while (!root) root = malloc(sizeof(Page));
	
	gd->sd = sd;
	gd->partition = partition;
	
	stg_gets(sd, STG_PBR_LBA(sd, partition), gd->pbr);
	page_sctor = (sd->mbr->pt[partition].size_s - 1) \
		* sizeof(Sector) / (sizeof(dd) + sizeof(Sector));
	gd->pbr->fat_sctor = page_sctor \
		* sizeof(dd) / sizeof(Sector) + 1;
	gd->pbr->free_page = page_sctor - 2;
	gd->pbr->first_page = 1;
	sd->puts(sd, STG_PBR_LBA(sd, partition), gd->pbr);
	
	printf(
		"\tpartition sector:%d\n"
		"\tFAT sector:%d, page sector:%d\n"
		, sd->mbr->pt[partition].size_s
		, gd->pbr->fat_sctor, page_sctor
	);
	
	ui_prog_start("Formating");
	ui_prog_update(2, gd->pbr->fat_sctor + 3);
	
	// Build FAT
	gd->fat->dword[0] = GBFS_FAT_END;
	for (n = 1, m = 1; n < page_sctor;)
	{
		if (page_sctor - n > sizeof(Sector) / sizeof(dd))
		{
			b = sizeof(Sector) / sizeof(dd);
			for (; m < b; m ++) gd->fat->dword[m] = ++ n;
			m = 0;
		}
		else
		{
			b = page_sctor - n;
			for (m = 0; m < b; m ++) gd->fat->dword[m] = ++ n;
			gd->fat->dword[m - 1] = GBFS_FAT_END;
			for (; m < sizeof(Sector) / sizeof(dd); m ++)
				gd->fat->dword[m] = GBFS_FAT_NUL;
		}
		sd->puts(sd, GBFS_FAT_LBA(gd, (n - 1) / (sizeof(Sector) / sizeof(dd))), gd->fat);
		ui_prog_update(1, 0);
	}
	
	// Build root directory
	for (n = 0; n < SECTOR_SIZE / sizeof(dq); n ++)
		root->qword[n] = 0;
	sd->puts(sd, GBFS_PAGE_LBA(gd, 0), root);
	ui_prog_update(1, 0);
	ui_prog_finish();
	
	return gd;
}
