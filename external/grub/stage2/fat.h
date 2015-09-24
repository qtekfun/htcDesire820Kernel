/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2001  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */



typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;

struct fat_bpb {
	__s8	ignored[3];	
	__s8	system_id[8];	
	__u8	bytes_per_sect[2];	
	__u8	sects_per_clust;
	__u8	reserved_sects[2];	
	__u8	num_fats;	
	__u8	dir_entries[2];	
	__u8	short_sectors[2];	
	__u8	media;		
	__u16	fat_length;	
	__u16	secs_track;	
	__u16	heads;		
	__u32	hidden;		
	__u32	long_sectors;	

	
	__u32	fat32_length;	
	__u16	flags;		
	__u8	version[2];	
	__u32	root_cluster;	
	__u16	info_sector;	
	__u16	backup_boot;	
	__u16	reserved2[6];	
};

#define FAT_CVT_U16(bytarr) (* (__u16*)(bytarr))


#define FAT_MAX_12BIT_CLUST       4087	


#define FAT_ATTRIB_OK_MASK        0x37
#define FAT_ATTRIB_NOT_OK_MASK    0xC8
#define FAT_ATTRIB_DIR            0x10
#define FAT_ATTRIB_LONGNAME       0x0F


#define FAT_DIRENTRY_LENGTH       32

#define FAT_DIRENTRY_ATTRIB(entry) \
  (*((unsigned char *) (entry+11)))
#define FAT_DIRENTRY_VALID(entry) \
  ( ((*((unsigned char *) entry)) != 0) \
    && ((*((unsigned char *) entry)) != 0xE5) \
    && !(FAT_DIRENTRY_ATTRIB(entry) & FAT_ATTRIB_NOT_OK_MASK) )
#define FAT_DIRENTRY_FIRST_CLUSTER(entry) \
  ((*((unsigned short *) (entry+26)))+(*((unsigned short *) (entry+20)) << 16))
#define FAT_DIRENTRY_FILELENGTH(entry) \
  (*((unsigned long *) (entry+28)))

#define FAT_LONGDIR_ID(entry) \
  (*((unsigned char *) (entry)))
#define FAT_LONGDIR_ALIASCHECKSUM(entry) \
  (*((unsigned char *) (entry+13)))
