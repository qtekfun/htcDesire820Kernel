/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000,2003  Free Software Foundation, Inc.
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


struct mod_list
{
  
  unsigned long mod_start;
  unsigned long mod_end;
  
  
  unsigned long cmdline;
  
  
  unsigned long pad;
};



struct AddrRangeDesc
{
  unsigned long size;
  unsigned long long BaseAddr;
  unsigned long long Length;
  unsigned long Type;
  
  
} __attribute__ ((packed));

#define MB_ARD_MEMORY		1


struct drive_info
{
  
  unsigned long size;

  
  unsigned char drive_number;

  
  unsigned char drive_mode;

  
  unsigned short drive_cylinders;
  unsigned char drive_heads;
  unsigned char drive_sectors;

  
  unsigned short drive_ports[0];
};

#define MB_DI_CHS_MODE		0
#define MB_DI_LBA_MODE		1


struct apm_info
{
  unsigned short version;
  unsigned short cseg;
  unsigned long offset;
  unsigned short cseg_16;
  unsigned short dseg_16;
  unsigned short cseg_len;
  unsigned short cseg_16_len;
  unsigned short dseg_16_len;
};



struct multiboot_info
{
  
  unsigned long flags;
  
  
  unsigned long mem_lower;
  unsigned long mem_upper;
  
  
  unsigned long boot_device;
  
  
  unsigned long cmdline;
  
  
  unsigned long mods_count;
  unsigned long mods_addr;
  
  union
  {
    struct
    {
      
      unsigned long tabsize;
      unsigned long strsize;
      unsigned long addr;
      unsigned long pad;
    }
    a;
    
    struct
    {
      
      unsigned long num;
      unsigned long size;
      unsigned long addr;
      unsigned long shndx;
    }
    e;
  }
  syms;
  
  
  unsigned long mmap_length;
  unsigned long mmap_addr;
  
  
  unsigned long drives_length;
  unsigned long drives_addr;
  
  
  unsigned long config_table;
  
  
  unsigned long boot_loader_name;

  
  unsigned long apm_table;

  
  unsigned long vbe_control_info;
  unsigned long vbe_mode_info;
  unsigned short vbe_mode;
  unsigned short vbe_interface_seg;
  unsigned short vbe_interface_off;
  unsigned short vbe_interface_len;
};


#define MB_INFO_MEMORY			0x00000001
#define MB_INFO_BOOTDEV			0x00000002
#define MB_INFO_CMDLINE			0x00000004
#define MB_INFO_MODS			0x00000008


#define MB_INFO_AOUT_SYMS		0x00000010
#define MB_INFO_ELF_SHDR		0x00000020

#define MB_INFO_MEM_MAP			0x00000040

#define MB_INFO_DRIVE_INFO		0x00000080

#define MB_INFO_CONFIG_TABLE		0x00000100

#define MB_INFO_BOOT_LOADER_NAME	0x00000200

#define MB_INFO_APM_TABLE		0x00000400

#define MB_INFO_VIDEO_INFO		0x00000800


#define MULTIBOOT_VALID			0x2BADB002
