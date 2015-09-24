/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2003,2004,2005  Free Software Foundation, Inc.
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

#define _LARGEFILE_SOURCE       1
#define _FILE_OFFSET_BITS       64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>

#ifdef __linux__
# if !defined(__GLIBC__) || \
        ((__GLIBC__ < 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 1)))
#  include <linux/unistd.h>     
# endif 
# include <sys/ioctl.h>		
# ifndef HDIO_GETGEO
#  define HDIO_GETGEO	0x0301	
struct hd_geometry
{
  unsigned char heads;
  unsigned char sectors;
  unsigned short cylinders;
  unsigned long start;
};
# endif 
# ifndef FLOPPY_MAJOR
#  define FLOPPY_MAJOR	2	
# endif 
# ifndef MAJOR
#  define MAJOR(dev)	\
  ({ \
     unsigned long long __dev = (dev); \
     (unsigned) ((__dev >> 8) & 0xfff) \
                 | ((unsigned int) (__dev >> 32) & ~0xfff); \
  })
# endif 
# ifndef CDROM_GET_CAPABILITY
#  define CDROM_GET_CAPABILITY	0x5331	
# endif 
# ifndef BLKGETSIZE
#  define BLKGETSIZE	_IO(0x12,96)	
# endif 
#endif 

#if defined(__FreeBSD__) && ! defined(__FreeBSD_kernel__)
# define __FreeBSD_kernel__
#endif
#ifdef __FreeBSD_kernel__
  
# include <osreldate.h>
# ifndef __FreeBSD_kernel_version
#  define __FreeBSD_kernel_version __FreeBSD_version
# endif

  
# include <sys/utsname.h>
int
get_kfreebsd_version ()
{
  struct utsname uts;
  int major; int minor, v[2];

  uname (&uts);
  sscanf (uts.release, "%d.%d", &major, &minor);

  if (major >= 9)
    major = 9;
  if (major >= 5)
    {
      v[0] = minor/10; v[1] = minor%10;
    }
  else
    {
      v[0] = minor%10; v[1] = minor/10;
    }
  return major*100000+v[0]*10000+v[1]*1000;
}
#endif 

#if defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__)
# include <sys/ioctl.h>		
# include <sys/disklabel.h>
# include <sys/cdio.h>		
# if defined(__FreeBSD_kernel__)
#  include <sys/param.h>
#  if __FreeBSD_kernel_version >= 500040
#   include <sys/disk.h>
#  endif
# endif 
#endif 

#ifdef HAVE_OPENDISK
# include <util.h>
#endif 

#define WITHOUT_LIBC_STUBS	1
#include <shared.h>
#include <device.h>

void
get_drive_geometry (struct geometry *geom, char **map, int drive)
{
  int fd;

  if (geom->flags == -1)
    {
      fd = open (map[drive], O_RDONLY);
      assert (fd >= 0);
    }
  else
    fd = geom->flags;

  
  geom->sector_size = SECTOR_SIZE;
  
#if defined(__linux__)
  
  {
    struct hd_geometry hdg;
    unsigned long nr;
    
    if (ioctl (fd, HDIO_GETGEO, &hdg))
      goto fail;

    if (ioctl (fd, BLKGETSIZE, &nr))
      goto fail;
    
    
    geom->cylinders = hdg.cylinders;
    geom->heads = hdg.heads;
    geom->sectors = hdg.sectors;
    geom->total_sectors = nr;
    
    goto success;
  }

#elif defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__)
# if defined(__FreeBSD_kernel__) && __FreeBSD_kernel_version >= 500040
  
  if (get_kfreebsd_version () >= 500040)
  {
    unsigned int sector_size;
    off_t media_size;
    unsigned int tmp;
    
    if(ioctl (fd, DIOCGSECTORSIZE, &sector_size) != 0)
      sector_size = 512;
    
    if (ioctl (fd, DIOCGMEDIASIZE, &media_size) != 0)
      goto fail;

    geom->total_sectors = media_size / sector_size;
    
    if (ioctl (fd, DIOCGFWSECTORS, &tmp) == 0)
      geom->sectors = tmp;
    else
      geom->sectors = 63;
    if (ioctl (fd, DIOCGFWHEADS, &tmp) == 0)
      geom->heads = tmp;
    else if (geom->total_sectors <= 63 * 1 * 1024)
      geom->heads = 1;
    else if (geom->total_sectors <= 63 * 16 * 1024)
      geom->heads = 16;
    else
      geom->heads = 255;

    geom->cylinders = (geom->total_sectors
			   / geom->heads
			   / geom->sectors);
    
    goto success;
  }
  else
#endif 

  
  {
    struct disklabel hdg;
    if (ioctl (fd, DIOCGDINFO, &hdg))
      goto fail;
    
    geom->cylinders = hdg.d_ncylinders;
    geom->heads = hdg.d_ntracks;
    geom->sectors = hdg.d_nsectors;
    geom->total_sectors = hdg.d_secperunit;

    goto success;
  }
  
#else
  
# warning "Automatic detection of geometries will be performed only \
partially. This is not fatal."
#endif

 fail:
  {
    struct stat st;

    if (drive & 0x80)
      {
	geom->cylinders = DEFAULT_HD_CYLINDERS;
	geom->heads = DEFAULT_HD_HEADS;
	geom->sectors = DEFAULT_HD_SECTORS;
      }
    else
      {
	geom->cylinders = DEFAULT_FD_CYLINDERS;
	geom->heads = DEFAULT_FD_HEADS;
	geom->sectors = DEFAULT_FD_SECTORS;
      }

    
    if (! fstat (fd, &st) && st.st_size)
      geom->total_sectors = st.st_size >> SECTOR_BITS;
    else
      geom->total_sectors = geom->cylinders * geom->heads * geom->sectors;
  }

 success:
  if (geom->flags == -1)
    close (fd);
}

#ifdef __linux__
static int
have_devfs (void)
{
  static int dev_devfsd_exists = -1;
  
  if (dev_devfsd_exists < 0)
    {
      struct stat st;
      
      dev_devfsd_exists = stat ("/dev/.devfsd", &st) == 0;
    }
  
  return dev_devfsd_exists;
}
#endif 

static void
get_floppy_disk_name (char *name, int unit)
{
#if defined(__linux__)
  
  if (have_devfs ())
    sprintf (name, "/dev/floppy/%d", unit);
  else
    sprintf (name, "/dev/fd%d", unit);
#elif defined(__GNU__)
  
  sprintf (name, "/dev/fd%d", unit);
#elif defined(__FreeBSD_kernel__)
  
  if (get_kfreebsd_version () >= 400000)
    sprintf (name, "/dev/fd%d", unit);
  else
    sprintf (name, "/dev/rfd%d", unit);
#elif defined(__NetBSD__)
  
  
  sprintf (name, "/dev/rfd%da", unit);
#elif defined(__OpenBSD__)
  
  sprintf (name, "/dev/rfd%dc", unit);
#elif defined(__QNXNTO__)
  
  sprintf (name, "/dev/fd%d", unit);
#else
# warning "BIOS floppy drives cannot be guessed in your operating system."
  
  *name = 0;
#endif
}

static void
get_ide_disk_name (char *name, int unit)
{
#if defined(__linux__)
  
  sprintf (name, "/dev/hd%c", unit + 'a');
#elif defined(__GNU__)
  
  sprintf (name, "/dev/hd%d", unit);
#elif defined(__FreeBSD_kernel__)
  
  if (get_kfreebsd_version () >= 400000)
    sprintf (name, "/dev/ad%d", unit);
  else
    sprintf (name, "/dev/rwd%d", unit);
#elif defined(__NetBSD__) && defined(HAVE_OPENDISK)
  
  char shortname[16];
  int fd;
  
  sprintf (shortname, "wd%d", unit);
  fd = opendisk (shortname, O_RDONLY, name,
		 16,	
		 0	
		 );
  close (fd);
#elif defined(__OpenBSD__)
  
  sprintf (name, "/dev/rwd%dc", unit);
#elif defined(__QNXNTO__)
  
  sprintf (name, "/dev/hd%d", unit);
#else
# warning "BIOS IDE drives cannot be guessed in your operating system."
  
  *name = 0;
#endif
}

static void
get_scsi_disk_name (char *name, int unit)
{
#if defined(__linux__)
  
  sprintf (name, "/dev/sd%c", unit + 'a');
#elif defined(__GNU__)
  
  sprintf (name, "/dev/sd%d", unit);
#elif defined(__FreeBSD_kernel__)
  
  if (get_kfreebsd_version () >= 400000)
    sprintf (name, "/dev/da%d", unit);
  else
    sprintf (name, "/dev/rda%d", unit);
#elif defined(__NetBSD__) && defined(HAVE_OPENDISK)
  
  char shortname[16];
  int fd;
  
  sprintf (shortname, "sd%d", unit);
  fd = opendisk (shortname, O_RDONLY, name,
		 16,	
		 0	
		 );
  close (fd);
#elif defined(__OpenBSD__)
  
  sprintf (name, "/dev/rsd%dc", unit);
#elif defined(__QNXNTO__)
  
  *name = 0;
#else
# warning "BIOS SCSI drives cannot be guessed in your operating system."
  
  *name = 0;
#endif
}

#ifdef __linux__
static void
get_dac960_disk_name (char *name, int controller, int drive)
{
  sprintf (name, "/dev/rd/c%dd%d", controller, drive);
}

static void
get_ataraid_disk_name (char *name, int unit)
{
  sprintf (name, "/dev/ataraid/d%c", unit + '0');
}
#endif

int
check_device (const char *device)
{
  char buf[512];
  FILE *fp;

  
  if (*device == 0)
    return 1;
  
  fp = fopen (device, "r");
  if (! fp)
    {
      switch (errno)
	{
#ifdef ENOMEDIUM
	case ENOMEDIUM:
# if 0
	  
	  return 1;
# endif
	  break;
#endif 
	default:
	  
	  break;
	}
      
      return 0;
    }
  
#ifdef __linux__
# ifdef CDROM_GET_CAPABILITY
  if (ioctl (fileno (fp), CDROM_GET_CAPABILITY, 0) >= 0)
    return 0;
# else 
  
  {
    struct hd_geometry hdg;
    struct stat st;

    if (fstat (fileno (fp), &st))
      return 0;

    if (S_ISBLK (st.st_mode)
	&& MAJOR (st.st_rdev) != FLOPPY_MAJOR
	&& ioctl (fileno (fp), HDIO_GETGEO, &hdg))
      return 0;
  }
# endif 
#endif 

#if defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(__OpenBSD__)
# ifdef CDIOCCLRDEBUG
  if (ioctl (fileno (fp), CDIOCCLRDEBUG, 0) >= 0)
    return 0;
# endif 
#endif 
  
  
  if (fread (buf, 1, 512, fp) != 512)
    {
      fclose (fp);
      return 0;
    }
  
  fclose (fp);
  return 1;
}

static int
read_device_map (FILE *fp, char **map, const char *map_file)
{
  auto void show_error (int no, const char *msg);
  auto void show_warning (int no, const char *msg, ...);
  
  auto void show_error (int no, const char *msg)
    {
      fprintf (stderr, "%s:%d: error: %s\n", map_file, no, msg);
    }
  
  auto void show_warning (int no, const char *msg, ...)
    {
      va_list ap;
      
      va_start (ap, msg);
      fprintf (stderr, "%s:%d: warning: ", map_file, no);
      vfprintf (stderr, msg, ap);
      va_end (ap);
    }
  
  char buf[1024];		
  int line_number = 0;
  
  while (fgets (buf, sizeof (buf), fp))
    {
      char *ptr, *eptr;
      int drive;
      int is_floppy = 0;
      
      
      line_number++;
      
      
      if (buf[0] == '#')
	continue;
      
      ptr = buf;
      
      while (*ptr && isspace (*ptr))
	ptr++;

      
      if (! *ptr)
	continue;
      
      if (*ptr != '(')
	{
	  show_error (line_number, "No open parenthesis found");
	  return 0;
	}
      
      ptr++;
      if ((*ptr != 'f' && *ptr != 'h') || *(ptr + 1) != 'd')
	{
	  show_error (line_number, "Bad drive name");
	  return 0;
	}
      
      if (*ptr == 'f')
	is_floppy = 1;
      
      ptr += 2;
      drive = strtoul (ptr, &ptr, 10);
      if (drive < 0)
	{
	  show_error (line_number, "Bad device number");
	  return 0;
	}
      else if (drive > 127)
	{
	  show_warning (line_number,
			"Ignoring %cd%d due to a BIOS limitation",
			is_floppy ? 'f' : 'h', drive);
	  continue;
	}
      
      if (! is_floppy)
	drive += 0x80;
      
      if (*ptr != ')')
	{
	  show_error (line_number, "No close parenthesis found");
	  return 0;
	}
      
      ptr++;
      
      while (*ptr && isspace (*ptr))
	ptr++;
      
      if (! *ptr)
	{
	  show_error (line_number, "No filename found");
	  return 0;
	}
      
      
      eptr = ptr;
      while (*eptr && ! isspace (*eptr))
	eptr++;
      *eptr = 0;

      
      if (map[drive])
	{
	  show_error (line_number, "Duplicated entry found");
	  return 0;
	}
      
      map[drive] = strdup (ptr);
      assert (map[drive]);
    }
  
  return 1;
}

int
init_device_map (char ***map, const char *map_file, int floppy_disks)
{
  int i;
  int num_hd = 0;
  FILE *fp = 0;

  assert (map);
  assert (*map == 0);
  *map = malloc (NUM_DISKS * sizeof (char *));
  assert (*map);
  
  
  
  
  for (i = 0; i < NUM_DISKS; i++)
    (*map)[i] = 0;
  
  if (map_file)
    {
      
      fp = fopen (map_file, "r");
      if (fp)
	{
	  int ret;

	  ret = read_device_map (fp, *map, map_file);
	  fclose (fp);
	  return ret;
	}
    }
  
  fprintf (stderr,
	   "Probing devices to guess BIOS drives. "
	   "This may take a long time.\n");
  
  if (map_file)
    
    fp = fopen (map_file, "w");
  
  
  for (i = 0; i < floppy_disks; i++)
    {
      char name[16];
      
      get_floppy_disk_name (name, i);
      if (fp)
	fprintf (fp, "(fd%d)\t%s\n", i, name);
      
      if (check_device (name))
	{
	  (*map)[i] = strdup (name);
	  assert ((*map)[i]);
	}
    }
  
#ifdef __linux__
  if (have_devfs ())
    {
      while (1)
	{
	  char discn[32];
	  char name[PATH_MAX];
	  struct stat st;

	  sprintf (discn, "/dev/discs/disc%d", num_hd);
	  if (stat (discn, &st) < 0)
	    break;
	  
	  if (realpath (discn, name))
	    {
	      strcat (name, "/disc");
	      (*map)[num_hd + 0x80] = strdup (name);
	      assert ((*map)[num_hd + 0x80]);
	      
	      
	      if (fp)
		fprintf (fp, "(hd%d)\t%s\n", num_hd, name);
	    }
	  
	  num_hd++;
	}
      
      
      if (fp)
	fclose (fp);
      
      return 1;
    }
#endif 
    
  
  for (i = 0; i < 8; i++)
    {
      char name[16];
      
      get_ide_disk_name (name, i);
      if (check_device (name))
	{
	  (*map)[num_hd + 0x80] = strdup (name);
	  assert ((*map)[num_hd + 0x80]);
	  
	  
	  if (fp)
	    fprintf (fp, "(hd%d)\t%s\n", num_hd, name);
	  
	  num_hd++;
	}
    }
  
#ifdef __linux__
  
  for (i = 0; i < 8; i++)
    {
      char name[20];

      get_ataraid_disk_name (name, i);
      if (check_device (name))
        {
          (*map)[num_hd + 0x80] = strdup (name);
          assert ((*map)[num_hd + 0x80]);

          
          if (fp)
            fprintf (fp, "(hd%d)\t%s\n", num_hd, name);

          num_hd++;
        }
    }
#endif 

  
  for (i = 0; i < 16; i++)
    {
      char name[16];
      
      get_scsi_disk_name (name, i);
      if (check_device (name))
	{
	  (*map)[num_hd + 0x80] = strdup (name);
	  assert ((*map)[num_hd + 0x80]);
	  
	  
	  if (fp)
	    fprintf (fp, "(hd%d)\t%s\n", num_hd, name);
	  
	  num_hd++;
	}
    }
  
#ifdef __linux__
  {
    int controller, drive;
    
    for (controller = 0; controller < 8; controller++)
      {
	for (drive = 0; drive < 15; drive++)
	  {
	    char name[24];
	    
	    get_dac960_disk_name (name, controller, drive);
	    if (check_device (name))
	      {
		(*map)[num_hd + 0x80] = strdup (name);
		assert ((*map)[num_hd + 0x80]);
		
		
		if (fp)
		  fprintf (fp, "(hd%d)\t%s\n", num_hd, name);
		
		num_hd++;
	      }
	  }
      }
  }
#endif 
  
  
  if (fp)
    fclose (fp);

  return 1;
}

void
restore_device_map (char **map)
{
  int i;

  for (i = 0; i < NUM_DISKS; i++)
    if (map[i])
      free (map[i]);

  free (map);
}

#ifdef __linux__
int
is_disk_device (char **map, int drive)
{
  struct stat st;
  
  assert (map[drive] != 0);
  assert (stat (map[drive], &st) == 0);
  
  return S_ISBLK (st.st_mode);
}

int
write_to_partition (char **map, int drive, int partition,
		    int sector, int size, const char *buf)
{
  char dev[PATH_MAX];	
  int fd;
  
  if ((partition & 0x00FF00) != 0x00FF00)
    {
      errnum = ERR_DEV_VALUES;
      return 1;
    }
  
  assert (map[drive] != 0);
  
  strcpy (dev, map[drive]);
  if (have_devfs ())
    {
      if (strcmp (dev + strlen(dev) - 5, "/disc") == 0)
	strcpy (dev + strlen(dev) - 5, "/part");
    }
  sprintf (dev + strlen(dev), "%d", ((partition >> 16) & 0xFF) + 1);
  
  
  fd = open (dev, O_RDWR);
  if (fd < 0)
    {
      errnum = ERR_NO_PART;
      return 0;
    }
  
#if defined(__linux__) && (!defined(__GLIBC__) || \
        ((__GLIBC__ < 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 1))))
  
  {
    loff_t offset, result;
    static int _llseek (uint filedes, ulong hi, ulong lo,
                        loff_t *res, uint wh);
    _syscall5 (int, _llseek, uint, filedes, ulong, hi, ulong, lo,
               loff_t *, res, uint, wh);

    offset = (loff_t) sector * (loff_t) SECTOR_SIZE;
    if (_llseek (fd, offset >> 32, offset & 0xffffffff, &result, SEEK_SET))
      {
	errnum = ERR_DEV_VALUES;
	return 0;
      }
  }
#else
  {
    off_t offset = (off_t) sector * (off_t) SECTOR_SIZE;

    if (lseek (fd, offset, SEEK_SET) != offset)
      {
	errnum = ERR_DEV_VALUES;
	return 0;
      }
  }
#endif
  
  if (write (fd, buf, size * SECTOR_SIZE) != (size * SECTOR_SIZE))
    {
      close (fd);
      errnum = ERR_WRITE;
      return 0;
    }
  
  sync ();	
  close (fd);
  
  return 1;
}
#endif 
