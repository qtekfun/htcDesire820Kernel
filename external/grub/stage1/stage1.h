/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2002,2004   Free Software Foundation, Inc.
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

#ifndef STAGE1_HEADER
#define STAGE1_HEADER	1


#define COMPAT_VERSION_MAJOR	3
#define COMPAT_VERSION_MINOR	2
#define COMPAT_VERSION		((COMPAT_VERSION_MINOR << 8) \
					| COMPAT_VERSION_MAJOR)

#define STAGE1_SIGNATURE	0xaa55

#define STAGE1_BPBEND		0x3e

#define STAGE1_VER_MAJ_OFFS	0x3e

#define STAGE1_BOOT_DRIVE	0x40

#define STAGE1_FORCE_LBA	0x41

#define STAGE1_STAGE2_ADDRESS	0x42

#define STAGE1_STAGE2_SECTOR	0x44

#define STAGE1_STAGE2_SEGMENT	0x48

#define STAGE1_BOOT_DRIVE_CHECK	0x4b

#define STAGE1_WINDOWS_NT_MAGIC	0x1b8

#define STAGE1_PARTSTART	0x1be

#define STAGE1_PARTEND		0x1fe

#define STAGE1_STACKSEG		0x2000

#define STAGE1_BUFFERSEG	0x7000

#define STAGE1_DRP_ADDR		0x7f00

#define STAGE1_DRP_SIZE		0x42

#define STAGE1_BIOS_HD_FLAG	0x80

#define GRUB_INVALID_DRIVE	0xFF

#endif 
