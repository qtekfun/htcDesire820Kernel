/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000   Free Software Foundation, Inc.
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


struct multiboot_header
{
  
  unsigned magic;
  
  
  unsigned flags;
  
  unsigned checksum;
  
  
  unsigned header_addr;
  unsigned load_addr;
  unsigned load_end_addr;
  unsigned bss_end_addr;
  unsigned entry_addr;

  
  unsigned mode_type;
  unsigned width;
  unsigned height;
  unsigned depth;
};

#define MULTIBOOT_SEARCH		8192
#define MULTIBOOT_FOUND(addr, len) \
  (! ((addr) & 0x3) \
   && (len) >= 12 \
   && *((int *) (addr)) == MULTIBOOT_MAGIC \
   && ! (*((unsigned *) (addr)) + *((unsigned *) (addr + 4)) \
	 + *((unsigned *) (addr + 8))) \
   && (! (MULTIBOOT_AOUT_KLUDGE & *((int *) (addr + 4))) || (len) >= 32) \
   && (! (MULTIBOOT_VIDEO_MODE & *((int *) (addr + 4))) || (len) >= 48))

#define MULTIBOOT_MAGIC			0x1BADB002

#define MULTIBOOT_MUSTKNOW		0x0000FFFF

#define MULTIBOOT_UNSUPPORTED		0x0000FFF8

#define MULTIBOOT_PAGE_ALIGN		0x00000001

#define MULTIBOOT_MEMORY_INFO		0x00000002

#define MULTIBOOT_VIDEO_MODE		0x00000004

#define MULTIBOOT_AOUT_KLUDGE		0x00010000
