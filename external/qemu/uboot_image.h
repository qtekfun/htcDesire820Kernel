/*
 * (C) Copyright 2000-2005
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 ********************************************************************
 * NOTE: This header file defines an interface to U-Boot. Including
 * this (unmodified) header file in another file is considered normal
 * use of U-Boot, and does *not* fall under the heading of "derived
 * work".
 ********************************************************************
 */

#ifndef __UBOOT_IMAGE_H__
#define __UBOOT_IMAGE_H__

#define IH_OS_INVALID		0	
#define IH_OS_OPENBSD		1	
#define IH_OS_NETBSD		2	
#define IH_OS_FREEBSD		3	
#define IH_OS_4_4BSD		4	
#define IH_OS_LINUX		5	
#define IH_OS_SVR4		6	
#define IH_OS_ESIX		7	
#define IH_OS_SOLARIS		8	
#define IH_OS_IRIX		9	
#define IH_OS_SCO		10	
#define IH_OS_DELL		11	
#define IH_OS_NCR		12	
#define IH_OS_LYNXOS		13	
#define IH_OS_VXWORKS		14	
#define IH_OS_PSOS		15	
#define IH_OS_QNX		16	
#define IH_OS_U_BOOT		17	
#define IH_OS_RTEMS		18	
#define IH_OS_ARTOS		19	
#define IH_OS_UNITY		20	

#define IH_CPU_INVALID		0	
#define IH_CPU_ALPHA		1	
#define IH_CPU_ARM		2	
#define IH_CPU_I386		3	
#define IH_CPU_IA64		4	
#define IH_CPU_MIPS		5	
#define IH_CPU_MIPS64		6	
#define IH_CPU_PPC		7	
#define IH_CPU_S390		8	
#define IH_CPU_SH		9	
#define IH_CPU_SPARC		10	
#define IH_CPU_SPARC64		11	
#define IH_CPU_M68K		12	
#define IH_CPU_NIOS		13	
#define IH_CPU_MICROBLAZE	14	
#define IH_CPU_NIOS2		15	
#define IH_CPU_BLACKFIN		16	
#define IH_CPU_AVR32		17	


#define IH_TYPE_INVALID		0	
#define IH_TYPE_STANDALONE	1	
#define IH_TYPE_KERNEL		2	
#define IH_TYPE_RAMDISK		3	
#define IH_TYPE_MULTI		4	
#define IH_TYPE_FIRMWARE	5	
#define IH_TYPE_SCRIPT		6	
#define IH_TYPE_FILESYSTEM	7	
#define IH_TYPE_FLATDT		8	

#define IH_COMP_NONE		0	
#define IH_COMP_GZIP		1	
#define IH_COMP_BZIP2		2	

#define IH_MAGIC	0x27051956	
#define IH_NMLEN		32	


typedef struct uboot_image_header {
	uint32_t	ih_magic;	
	uint32_t	ih_hcrc;	
	uint32_t	ih_time;	
	uint32_t	ih_size;	
	uint32_t	ih_load;	
	uint32_t	ih_ep;		
	uint32_t	ih_dcrc;	
	uint8_t		ih_os;		
	uint8_t		ih_arch;	
	uint8_t		ih_type;	
	uint8_t		ih_comp;	
	uint8_t		ih_name[IH_NMLEN];	
} uboot_image_header_t;


#endif	
