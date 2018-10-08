/**
 * Date:2018/9/19 15:33
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * Ghost Bird File System support
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#ifndef GBFS_H_
#define GBFS_H_

#include "types.h"
#include "return.h"
#include "storage.h"

#define GBFS_FAT_NUL 0xffffffff
#define GBFS_FAT_END 0xfffffffe

#pragma pack(push)
#pragma pack(1)

#define GBFS_PBR_CODE_SIZE 496
typedef struct gbfs_pbr
{
	db		code[GBFS_PBR_CODE_SIZE];
	dd		fat_sctor;
	dd		free_page;
	dd		first_page;
	dw		resv;
	dw		sign;
}PBR;

typedef Sector Page;

#pragma pack(pop)

typedef struct gbfs_desc
{
	Storage *sd;
	db		partition;
	PBR		*pbr;
	Sector	*fat;
	Sector	*page;
}GBFS;

typedef struct gbfs_dir
{
	GBFS *gd;
	
}DIR;

#define GBFS_FAT_LBA(gd, index)STG_PBR_LBA(gd->sd, gd->partition) + index + 1
#define GBFS_PAGE_LBA(gd, number)STG_PBR_LBA(gd->sd, gd->partition) + gd->pbr->fat_sctor + 1

RET		gbfs_putf();
RET		gbfs_getf();

GBFS	*gbfs_open(const db *location);
RET		gbfs_close(GBFS *gd);

GBFS	*gbfs_fmt(Storage *sd, db partition);

#endif
