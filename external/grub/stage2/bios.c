/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2003,2004  Free Software Foundation, Inc.
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

#include "shared.h"


extern int biosdisk_int13_extensions (int ax, int drive, void *dap);
extern int biosdisk_standard (int ah, int drive,
			      int coff, int hoff, int soff,
			      int nsec, int segment);
extern int check_int13_extensions (int drive);
extern int get_diskinfo_standard (int drive,
				  unsigned long *cylinders,
				  unsigned long *heads,
				  unsigned long *sectors);
#if 0
extern int get_diskinfo_floppy (int drive,
				unsigned long *cylinders,
				unsigned long *heads,
				unsigned long *sectors);
#endif


int
biosdisk (int read, int drive, struct geometry *geometry,
	  int sector, int nsec, int segment)
{
  int err;
  
  if (geometry->flags & BIOSDISK_FLAG_LBA_EXTENSION)
    {
      struct disk_address_packet
      {
	unsigned char length;
	unsigned char reserved;
	unsigned short blocks;
	unsigned long buffer;
	unsigned long long block;
      } __attribute__ ((packed)) dap;

#ifdef NO_BUGGY_BIOS_IN_THE_WORLD
      if (sector >= geometry->total_sectors)
	return BIOSDISK_ERROR_GEOMETRY;
#endif 

      dap.length = sizeof (dap);
      dap.block = sector;
      dap.blocks = nsec;
      dap.reserved = 0;
      dap.buffer = segment << 16;
      
      err = biosdisk_int13_extensions ((read + 0x42) << 8, drive, &dap);

#ifndef NO_INT13_FALLBACK
      if (err)
	{
	  if (geometry->flags & BIOSDISK_FLAG_CDROM)
	    return err;
	  
	  geometry->flags &= ~BIOSDISK_FLAG_LBA_EXTENSION;
	  geometry->total_sectors = (geometry->cylinders
				     * geometry->heads
				     * geometry->sectors);
	  return biosdisk (read, drive, geometry, sector, nsec, segment);
	}
#endif 
      
    }
  else
    {
      int cylinder_offset, head_offset, sector_offset;
      int head;

      sector_offset = sector % geometry->sectors + 1;
      head = sector / geometry->sectors;
      head_offset = head % geometry->heads;
      cylinder_offset = head / geometry->heads;
      
      if (cylinder_offset >= geometry->cylinders)
	return BIOSDISK_ERROR_GEOMETRY;

      err = biosdisk_standard (read + 0x02, drive,
			       cylinder_offset, head_offset, sector_offset,
			       nsec, segment);
    }

  return err;
}

static int
get_cdinfo (int drive, struct geometry *geometry)
{
  int err;
  struct iso_spec_packet
  {
    unsigned char size;
    unsigned char media_type;
    unsigned char drive_no;
    unsigned char controller_no;
    unsigned long image_lba;
    unsigned short device_spec;
    unsigned short cache_seg;
    unsigned short load_seg;
    unsigned short length_sec512;
    unsigned char cylinders;
    unsigned char sectors;
    unsigned char heads;
    
    unsigned char dummy[16];
  } __attribute__ ((packed)) cdrp;
  
  grub_memset (&cdrp, 0, sizeof (cdrp));
  cdrp.size = sizeof (cdrp) - sizeof (cdrp.dummy);
  err = biosdisk_int13_extensions (0x4B01, drive, &cdrp);
  if (! err && cdrp.drive_no == drive)
    {
      if ((cdrp.media_type & 0x0F) == 0)
        {
          
          geometry->flags = BIOSDISK_FLAG_LBA_EXTENSION | BIOSDISK_FLAG_CDROM;
          geometry->cylinders = 0;
          geometry->heads = 1;
          geometry->sectors = 15;
          geometry->sector_size = 2048;
          geometry->total_sectors = MAXINT;
          return 1;
        }
      else
        {
	  
          geometry->cylinders
	    = ((unsigned int) cdrp.cylinders
	       + (((unsigned int) (cdrp.sectors & 0xC0)) << 2));
          geometry->heads = cdrp.heads;
          geometry->sectors = cdrp.sectors & 0x3F;
          geometry->sector_size = SECTOR_SIZE;
          geometry->total_sectors = (geometry->cylinders
				     * geometry->heads
				     * geometry->sectors);
          return -1;
        }
    }
  return 0;
}

int
get_diskinfo (int drive, struct geometry *geometry)
{
  int err;

  
  geometry->flags = 0;
  
  if (drive & 0x80)
    {
      
      int version;
      unsigned long total_sectors = 0;
      
      version = check_int13_extensions (drive);

      if (drive >= 0x88 || version)
	{
	  
	  if (get_cdinfo (drive, geometry))
	    return 0;
	}
      
      if (version)
	{
	  struct drive_parameters
	  {
	    unsigned short size;
	    unsigned short flags;
	    unsigned long cylinders;
	    unsigned long heads;
	    unsigned long sectors;
	    unsigned long long total_sectors;
	    unsigned short bytes_per_sector;
	    
	    unsigned long EDD_configuration_parameters;
	    
	    unsigned short signature_dpi;
	    unsigned char length_dpi;
	    unsigned char reserved[3];
	    unsigned char name_of_host_bus[4];
	    unsigned char name_of_interface_type[8];
	    unsigned char interface_path[8];
	    unsigned char device_path[8];
	    unsigned char reserved2;
	    unsigned char checksum;

	    unsigned char dummy[16];
	  } __attribute__ ((packed)) drp;

	  
	  grub_memset (&drp, 0, sizeof (drp));
	  
	  drp.size = sizeof (drp) - sizeof (drp.dummy);
	  
	  err = biosdisk_int13_extensions (0x4800, drive, &drp);
	  if (! err)
	    {
	      
	      geometry->flags = BIOSDISK_FLAG_LBA_EXTENSION;

	      
	       
	      if (drp.total_sectors)
		total_sectors = drp.total_sectors & ~0L;
	      else
		total_sectors = drp.cylinders * drp.heads * drp.sectors;
	    }
	}

      err = get_diskinfo_standard (drive,
				   &geometry->cylinders,
				   &geometry->heads,
				   &geometry->sectors);
      if (err)
	return err;

      if (! total_sectors)
	{
	  total_sectors = (geometry->cylinders
			   * geometry->heads
			   * geometry->sectors);
	}
      geometry->total_sectors = total_sectors;
      geometry->sector_size = SECTOR_SIZE;
    }
  else
    {
      

      
      err = get_diskinfo_standard (drive,
				   &geometry->cylinders,
				   &geometry->heads,
				   &geometry->sectors);

#if 0
      
      if (err)
	err = get_diskinfo_floppy (drive,
				   &geometry->cylinders,
				   &geometry->heads,
				   &geometry->sectors);
#endif
      
      if (err)
	return err;

      geometry->total_sectors = (geometry->cylinders
				 * geometry->heads
				 * geometry->sectors);
      geometry->sector_size = SECTOR_SIZE;
    }

  return 0;
}
