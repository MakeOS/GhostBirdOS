/**
 * Date:2018/9/19 15:30
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * Storage support
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/

#ifndef STORAGE_H_
#define STORAGE_H_

#include "return.h"
#include "types.h"

#pragma pack(push)
#pragma pack(1)

typedef struct stg_partition_table
{
	db active;
	db chs_strt[3];
	db fs;
	db chs_end[3];
	dd lba_strt;
	dd size_s;
}PT;

typedef struct stg_master_boot_record
{
	db	code[446];
	PT	pt[4];
}MBR;



#define SECTOR_SIZE 512
typedef union stg_sector
{
	db	byte [SECTOR_SIZE / sizeof(db)];
	dw	word [SECTOR_SIZE / sizeof(dw)];
	dd	dword[SECTOR_SIZE / sizeof(dd)];
	dq	qword[SECTOR_SIZE / sizeof(dq)];
}Sector;

#pragma pack(pop)

typedef struct stg_desc
{
	FILE	*file;
	void	*info;
	MBR		*mbr;
	dq		size_s;
	RET		(*puts)(struct stg_desc *sd, dq LBA, void *src);
	RET		(*gets)(struct stg_desc *sd, dq LBA, void *dst);
}Storage;

RET		stg_gets(Storage *sd, dq LBA, void *dst);
RET		stg_puts(Storage *sd, dq LBA, void *src);
RET		stg_close(Storage *sd);
Storage *stg_open(const db *location);

RET		stg_vhd_deinit(Storage *sd);
Storage *stg_vhd_init(Storage *sd);

#endif
