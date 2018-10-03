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


typedef struct gbfs_pbr
{
	
}PBR;

typedef struct gbfs_desc
{
	Storage *sd;
	db		partition;
}GBFS;

RET		gbfs_puts();
RET		gbfs_gets();

GBFS	*gbfs_open(const db *location);
RET		gbfs_close(GBFS *gd);

RET		gbfs_fmt(Storage *sd, db partition);

#endif
