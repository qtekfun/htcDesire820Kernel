
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

#define RB_ASKNAME      0x01	
#define RB_SINGLE       0x02	
#define RB_NOSYNC       0x04	
#define RB_HALT         0x08	
#define RB_INITNAME     0x10	
#define RB_DFLTROOT     0x20	
#define RB_KDB          0x40	
#define RB_RDONLY       0x80	
#define RB_DUMP         0x100	
#define RB_MINIROOT     0x200	
#define RB_CONFIG       0x400	
#define RB_VERBOSE      0x800	
#define RB_SERIAL       0x1000	
#define RB_CDROM        0x2000	
#define RB_GDB		0x8000	
#define RB_MUTE		0x10000	
#define RB_MULTIPLE	0x20000000	

#define RB_BOOTINFO     0x80000000	

#define B_ADAPTORSHIFT          24
#define B_CONTROLLERSHIFT       20
#define B_UNITSHIFT             16
#define B_PARTITIONSHIFT        8
#define B_TYPESHIFT             0

#define B_DEVMAGIC      ((unsigned long)0xa0000000)

#define MAKEBOOTDEV(type, adaptor, controller, unit, partition) \
        (((type) << B_TYPESHIFT) | ((adaptor) << B_ADAPTORSHIFT) | \
        ((controller) << B_CONTROLLERSHIFT) | ((unit) << B_UNITSHIFT) | \
        ((partition) << B_PARTITIONSHIFT) | B_DEVMAGIC)


#define BOOTINFO_VERSION        1

#define N_BIOS_GEOM             8

struct bootinfo
  {
    unsigned int bi_version;
    unsigned char *bi_kernelname;
    struct nfs_diskless *bi_nfs_diskless;
    
#define bi_endcommon            bi_n_bios_used
    unsigned int bi_n_bios_used;
    unsigned long bi_bios_geom[N_BIOS_GEOM];
    unsigned int bi_size;
    unsigned char bi_memsizes_valid;
    unsigned char bi_bios_dev;
    unsigned char bi_pad[2];
    unsigned long bi_basemem;
    unsigned long bi_extmem;
    unsigned long bi_symtab;
    unsigned long bi_esymtab;
  };
