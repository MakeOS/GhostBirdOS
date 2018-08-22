/**
 * Date:2018/8/17 21:08
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * Virtual Hard Disk Image Format support
 *
 * Reference: Virtual Hard Disk Image Format Specification 1.0. Oct 11, 2006.
 * 
 * BSD 2-Clause License
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "../include/return.h"
#include "../include/storage.h"

#pragma pack(push)
#pragma pack(1)

#define VHD_FOOTER_DISKTYPE_NONE		0
#define VHD_FOOTER_DISKTYPE_FIXD		2
#define VHD_FOOTER_DISKTYPE_DNMC	3
#define VHD_FOOTER_DISKTYPE_DFFC		4

struct vhd_footer
{
	// Microsoft uses the "conectix" string to identify
	char Cookie[8];
	
	// Any fields not listed are reserved.
	// No features enabled	0x00000000
	// Temporary				0x00000001
	// Reserved				0x00000002
	unsigned int Features;	
	
	/**This field is divided into a major/minor version and matches
	the version ofthe specification used in creating the file**/
	unsigned int FileFormatVersion;
	
	/**This field holds the absolute byte offset, from the beginning 
	of the file, to the next structure. This field is used for dynamic disks 
	and differencing disks, but not fixed disks. For fixed disks, this field 
	should be set to 0xFFFFFFFF. **/
	unsigned long long DataPffset;
	
	/**This field stores the creation time of a hard disk image. This is the 
	number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT.**/
	unsigned int TimeStamp;
	
	/**This field is used to document which application created the hard disk. 
	The field is a left-justified text field. It uses a single-byte character set. 
	If the hard disk is created by Microsoft Virtual PC, "vpc " is written in this field. 
	If the hard disk image is created by Microsoft Virtual Server, then "vs  " is 
	written in this field. Other applications should use their own unique identifiers.**/
	unsigned int CreatorApplication;
	
	/**This field holds the major/minor version of the 
	application that created the hard disk image.**/
	unsigned int CreatorVersion;
	
	// This field stores the type of host operating system this disk image is created on.
	// Windows				0x5769326B ("Wi2k")
	// Macintosh				0x4D616320 ("Mac ")
	unsigned int CreatorHostOS;
	
	/**This field stores the size of the hard disk in bytes, from the perspective of the 
	virtual machine, at creation time. This field is for informational purposes.**/
	unsigned long long OriginalSize;
	
	/**This field stores the current size of the hard disk, in bytes, from the perspective 
	of the virtual machine.
	This value is same as the original size when the hard disk is created. This value 
	can change depending on whether the hard disk is expanded.**/
	unsigned long long CurrentSize;
	
	// Disk Geometry
	unsigned short Cylinder;
	unsigned char Heads;
	unsigned char Sectors;
	
	// None					0
	// Reserved (deprecated)	1
	// Fixed hard disk			2
	// Dynamic hard disk		3
	// Differencing hard disk	4
	// Reserved (deprecated)	5
	// Reserved (deprecated)	6
	unsigned int DiskType;
	
	/**This field holds a basic checksum of the hard disk footer. It is just a one’s 
	complement of the sum of all the bytes in the footer without the checksum field.
	If the checksum verification fails, the Virtual PC and Virtual Server products will 
	instead use the header. If the checksum in the header also fails, the file should 
	be assumed to be corrupt. The pseudo-code for the algorithm used to 
	determine the checksum can be found in the appendix of this document. **/
	unsigned int Checksum;
	
	/**Every hard disk has a unique ID stored in the hard disk. This is used to identify 
	the hard disk. This is a 128-bit universally unique identifier (UUID). This field is 
	used to associate a parent hard disk image with its differencing hard disk image(s).**/
	unsigned int UniqueId[4];
	
	/**This field holds a one-byte flag that describes whether the system is in saved state. 
	If the hard disk is in the saved state the value is set to 1. 
	Operations such as compaction and expansion cannot be performed on a hard disk in a saved state.**/
	unsigned char SavedState;
	
	char Reserved[427];
};

#pragma pack(pop)

struct vhd_footer *storage_vhd_footer = NULL;

#define sw16(x) \
	((short)( \
		(((short)(x) & (short)0x00ffU) << 8) | \
		(((short)(x) & (short)0xff00U) >> 8) ))

#define sw32(x) \
((long)( \
	(((long)(x) & (long)0x000000ff) << 24) | \
	(((long)(x) & (long)0x0000ff00) << 8) | \
	(((long)(x) & (long)0x00ff0000) >> 8) | \
	(((long)(x) & (long)0xff000000) >> 24) ))
	
#define sw64(x) \
((long)( \
	(((long)(x) & (long)0x00000000000000ff) << 56) | \
	(((long)(x) & (long)0x000000000000ff00) << 40) | \
	(((long)(x) & (long)0x0000000000ff0000) << 24) | \
	(((long)(x) & (long)0x00000000ff000000) << 8) | \
	(((long)(x) & (long)0x000000ff00000000) >> 8) | \
	(((long)(x) & (long)0x0000ff0000000000) >> 24) | \
	(((long)(x) & (long)0x00ff000000000000) >> 40) | \
	(((long)(x) & (long)0xff00000000000000) >> 56) ))

int storage_vhd_write(struct storage_descriptor *sd, unsigned long LBA, union storage_sector *sector)
{
	unsigned long n;
	fseek(sd->file, LBA * STORAGE_SECTOR_SIZE, SEEK_SET);
	
	for (n = 0; n < STORAGE_SECTOR_SIZE; n++)
		putc(sector->byte[n], sd->file);

	return RET_SUCC;
}

int storage_vhd_read(struct storage_descriptor *sd, unsigned long LBA, union storage_sector *sector)
{
	unsigned long n;
	
	fseek(sd->file, LBA * STORAGE_SECTOR_SIZE, SEEK_SET);
	
	for (n = 0; n < STORAGE_SECTOR_SIZE; n++)
		sector->byte[n] = getc(sd->file);
		
	return RET_SUCC;
}

int storage_vhd_deinit(struct storage_descriptor *sd)
{
	free(storage_vhd_footer);
	return RET_SUCC;
}

int storage_vhd_init(struct storage_descriptor *sd)
{
	unsigned int n;
	
	fseek(sd->file, -sizeof(struct vhd_footer), SEEK_END);
	
	sd->info_block = NULL;
	while (!sd->info_block)
		sd->info_block = malloc(sizeof(struct vhd_footer));
	
	for (n = 0; n < sizeof(struct vhd_footer); n ++)
		((char *)sd->info_block)[n] = getc(sd->file);
		
	// Output this vhd information	
	printf("Image format: Virtual Hard Disk");
	switch (((struct vhd_footer *)sd->info_block)->DiskType)
	{
		case sw32(VHD_FOOTER_DISKTYPE_FIXD):
			printf("(Fixed hard disk)");
			break;
		case sw32(VHD_FOOTER_DISKTYPE_DNMC):
			printf("(Dynamic hard disk, unsupported yet)\n");
			goto abort;
		case sw32(VHD_FOOTER_DISKTYPE_DFFC):
			printf("(Differencing hard disk, unsupported yet)\n");
			goto abort;
		default:
			printf("(Reserved Type)");
			goto abort;
	}
	
	printf("\n");
	printf(
		"Cookie: %s, Features:%#X\n"
		"Format version: %#X\n"
		"Size: %lldBytes\n"
		, ((struct vhd_footer *)sd->info_block)->Cookie
		, sw32(((struct vhd_footer *)sd->info_block)->Features)
		, ((struct vhd_footer *)sd->info_block)->FileFormatVersion
		, sw64(((struct vhd_footer *)sd->info_block)->CurrentSize)
	);
	
	// Provide vhd read/write interface
	storage_write = storage_vhd_write;
	storage_read = storage_vhd_read;
	
	// Full the struct 'storage_descriptor'
	while (!sd->MBR)
		sd->MBR = malloc(sizeof(struct storage_master_boot_record));
	sd->size = sw64(((struct vhd_footer *)sd->info_block)->CurrentSize) / STORAGE_SECTOR_SIZE;
	storage_vhd_read(sd, 0, (union storage_sector *)sd->MBR);

	return RET_SUCC;
	
abort:
	free(storage_vhd_footer);
	return RET_FAIL;
}
