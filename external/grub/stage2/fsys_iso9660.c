/*
 *  ISO 9660 filesystem backend for GRUB (GRand Unified Bootloader)
 *  including Rock Ridge Extensions support
 *
 *  Copyright (C) 1998, 1999  Kousuke Takai  <tak@kmc.kyoto-u.ac.jp>
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
/*
 *  References:
 *	linux/fs/isofs/rock.[ch]
 *	mkisofs-1.11.1/diag/isoinfo.c
 *	mkisofs-1.11.1/iso9660.h
 *		(all are written by Eric Youngdale)
 *
 *  Modifications by:
 *	Leonid Lisovskiy   <lly@pisem.net>	2003
 */

#ifdef FSYS_ISO9660

#include "shared.h"
#include "filesys.h"
#include "iso9660.h"

struct iso_sb_info {
  unsigned long vol_sector;

};

struct iso_inode_info {
  unsigned long file_start;
};

#define ISO_SUPER	\
    ((struct iso_sb_info *)(FSYS_BUF))
#define INODE		\
    ((struct iso_inode_info *)(FSYS_BUF+sizeof(struct iso_sb_info)))
#define PRIMDESC        ((struct iso_primary_descriptor *)(FSYS_BUF + 2048))
#define DIRREC          ((struct iso_directory_record *)(FSYS_BUF + 4096))
#define RRCONT_BUF      ((unsigned char *)(FSYS_BUF + 6144))
#define NAME_BUF        ((unsigned char *)(FSYS_BUF + 8192))


static inline unsigned long
log2 (unsigned long word)
{
  asm volatile ("bsfl %1,%0"
		:          "=r" (word)
		:          "r" (word));
  return word;
}

static int
iso9660_devread (int sector, int byte_offset, int byte_len, char *buf)
{
  unsigned short sector_size_lg2 = log2(buf_geom.sector_size);

  if (sector < 0)
    {
      errnum = ERR_OUTSIDE_PART;
      return 0;
    }
  if (byte_len <= 0)
    return 1;

  sector += (byte_offset >> sector_size_lg2);
  byte_offset &= (buf_geom.sector_size - 1);
  asm volatile ("shl%L0 %1,%0"
		: "=r"(sector)
		: "Ic"((int8_t)(ISO_SECTOR_BITS - sector_size_lg2)),
		"0"(sector));

#if !defined(STAGE1_5)
  if (disk_read_hook && debug)
    printf ("<%d, %d, %d>", sector, byte_offset, byte_len);
#endif 

  return rawread(current_drive, part_start + sector, byte_offset, byte_len, buf);
}

int
iso9660_mount (void)
{
  unsigned int sector;

  if ((current_partition != 0xFFFFFF)
      && !IS_PC_SLICE_TYPE_BSD_WITH_FS(current_slice, FS_ISO9660))
    return 0;

  for (sector = 16 ; sector < 32 ; sector++)
    {
      if (!iso9660_devread(sector, 0, sizeof(*PRIMDESC), (char *)PRIMDESC)) 
	break;
      
      if (PRIMDESC->type.l == ISO_VD_PRIMARY
	  && !memcmp(PRIMDESC->id, ISO_STANDARD_ID, sizeof(PRIMDESC->id)))
	{
	  ISO_SUPER->vol_sector = sector;
	  INODE->file_start = 0;
	  fsmax = PRIMDESC->volume_space_size.l;
	  return 1;
	}
    }

  return 0;
}

int
iso9660_dir (char *dirname)
{
  struct iso_directory_record *idr;
  RR_ptr_t rr_ptr;
  struct rock_ridge *ce_ptr;
  unsigned int pathlen;
  int size;
  unsigned int extent;
  unsigned char file_type;
  unsigned int rr_len;
  unsigned char rr_flag;

  idr = &PRIMDESC->root_directory_record;
  INODE->file_start = 0;

  do
    {
      while (*dirname == '/')	
	dirname++;
      
      for (pathlen = 0 ;
	   dirname[pathlen]
	     && !isspace(dirname[pathlen]) && dirname[pathlen] != '/' ;
	   pathlen++)
	;

      size = idr->size.l;
      extent = idr->extent.l;

      while (size > 0)
	{
	  if (!iso9660_devread(extent, 0, ISO_SECTOR_SIZE, (char *)DIRREC))
	    {
	      errnum = ERR_FSYS_CORRUPT;
	      return 0;
	    }
	  extent++;

	  idr = (struct iso_directory_record *)DIRREC;
	  for (; idr->length.l > 0;
	       idr = (struct iso_directory_record *)((char *)idr + idr->length.l) )
	    {
	      const char *name = idr->name;
	      unsigned int name_len = idr->name_len.l;

	      file_type = (idr->flags.l & 2) ? ISO_DIRECTORY : ISO_REGULAR;
	      if (name_len == 1)
		{
		  if ((name[0] == 0) ||	
		      (name[0] == 1)) 	
		    continue;
		}
	      if (name_len > 2 && CHECK2(name + name_len - 2, ';', '1'))
		{
		  name_len -= 2;	
		  if (name_len > 1 && name[name_len - 1] == '.')
		    name_len--;		
		}

	      rr_len = (idr->length.l - idr->name_len.l
			- sizeof(struct iso_directory_record)
			+ sizeof(idr->name));
	      rr_ptr.ptr = ((unsigned char *)idr + idr->name_len.l
			    + sizeof(struct iso_directory_record)
			    - sizeof(idr->name));
	      if (rr_ptr.i & 1)
		rr_ptr.i++, rr_len--;
	      ce_ptr = NULL;
	      rr_flag = RR_FLAG_NM | RR_FLAG_PX ;

	      while (rr_len >= 4)
		{
		  if (rr_ptr.rr->version != 1)
		    {
#ifndef STAGE1_5
		      if (debug)
			printf(
			       "Non-supported version (%d) RockRidge chunk "
			       "`%c%c'\n", rr_ptr.rr->version,
			       rr_ptr.rr->signature & 0xFF,
			       rr_ptr.rr->signature >> 8);
#endif
		    }
		  else
		    {
		      switch (rr_ptr.rr->signature)
			{
			case RRMAGIC('R', 'R'):
			  if ( rr_ptr.rr->len >= (4+sizeof(struct RR)))
			    rr_flag &= rr_ptr.rr->u.rr.flags.l;
			  break;
			case RRMAGIC('N', 'M'):
			  name = rr_ptr.rr->u.nm.name;
			  name_len = rr_ptr.rr->len - (4+sizeof(struct NM));
			  rr_flag &= ~RR_FLAG_NM;
			  break;
			case RRMAGIC('P', 'X'):
			  if (rr_ptr.rr->len >= (4+sizeof(struct PX)))
			    {
			      file_type = ((rr_ptr.rr->u.px.mode.l & POSIX_S_IFMT)
					   == POSIX_S_IFREG
					   ? ISO_REGULAR
					   : ((rr_ptr.rr->u.px.mode.l & POSIX_S_IFMT)
					      == POSIX_S_IFDIR
					      ? ISO_DIRECTORY : ISO_OTHER));
			      rr_flag &= ~RR_FLAG_PX;
			    }
			  break;
			case RRMAGIC('C', 'E'):
			  if (rr_ptr.rr->len >= (4+sizeof(struct CE)))
			    ce_ptr = rr_ptr.rr;
			  break;
#if 0		
			case RRMAGIC('S', 'L'):
			  {
			    int slen;
			    unsigned char rootflag, prevflag;
			    char *rpnt = NAME_BUF+1024;
			    struct SL_component *slp;

			    slen = rr_ptr.rr->len - (4+1);
			    slp = &rr_ptr.rr->u.sl.link;
			    while (slen > 1)
			      {
				rootflag = 0;
				switch (slp->flags.l)
				  {
				  case 0:
				    memcpy(rpnt, slp->text, slp->len);
				    rpnt += slp->len;
				    break;
				  case 4:
				    *rpnt++ = '.';
				    
				  case 2:
				    *rpnt++ = '.';
				    break;
				  case 8:
				    rootflag = 1;
				    *rpnt++ = '/';
				    break;
				  default:
				    printf("Symlink component flag not implemented (%d)\n",
					   slp->flags.l);
				    slen = 0;
				    break;
				  }
				slen -= slp->len + 2;
				prevflag = slp->flags.l;
				slp = (struct SL_component *) ((char *) slp + slp->len + 2);

				if (slen < 2)
				  {
				    if ((!rootflag) && (rr_ptr.rr->u.sl.flags.l & 1) && !(prevflag & 1))
				      *rpnt++='/';
				    break;
				  }

				if (!rootflag && !(prevflag & 1))
				  *rpnt++ = '/';
			      }
			    *rpnt++ = '\0';
			    grub_putstr(NAME_BUF+1024);
			  }
			  rr_flag &= ~RR_FLAG_SL;
			  break;
#endif
			default:
			  break;
			}
		    }
		  if (!rr_flag)
		    break;

		  rr_len -= rr_ptr.rr->len;
		  rr_ptr.ptr += rr_ptr.rr->len;
		  if (rr_len < 4 && ce_ptr != NULL)
		    {
		      
		      if( RRCONT_BUF <= (unsigned char *)name
			  && (unsigned char *)name < RRCONT_BUF + ISO_SECTOR_SIZE )
			{
			  memcpy(NAME_BUF, name, name_len);
			  name = NAME_BUF;
			}
		      rr_ptr.ptr = RRCONT_BUF + ce_ptr->u.ce.offset.l;
		      rr_len = ce_ptr->u.ce.size.l;
		      if (!iso9660_devread(ce_ptr->u.ce.extent.l, 0, ISO_SECTOR_SIZE, RRCONT_BUF))
			{
			  errnum = 0;	
			  break;
			}
		      ce_ptr = NULL;
		    }
		} 

	      filemax = MAXINT;
	      if (name_len >= pathlen
		  && !memcmp(name, dirname, pathlen))
		{
		  if (dirname[pathlen] == '/' || !print_possibilities)
		    {
		      if (pathlen == name_len)
			{
			  if (dirname[pathlen] == '/')
			    {
			      if (file_type != ISO_DIRECTORY)
				{
				  errnum = ERR_BAD_FILETYPE;
				  return 0;
				}
			      goto next_dir_level;
			    }
			  if (file_type != ISO_REGULAR)
			    {
			      errnum = ERR_BAD_FILETYPE;
			      return 0;
			    }
			  INODE->file_start = idr->extent.l;
			  filepos = 0;
			  filemax = idr->size.l;
			  return 1;
			}
		    }
		  else	
		    {
#ifndef STAGE1_5
		      if (print_possibilities > 0)
			print_possibilities = -print_possibilities;
		      memcpy(NAME_BUF, name, name_len);
		      NAME_BUF[name_len] = '\0';
		      print_a_completion (NAME_BUF);
#endif
		    }
		}
	    } 

	  size -= ISO_SECTOR_SIZE;
	} 

      if (dirname[pathlen] == '/' || print_possibilities >= 0)
	{
	  errnum = ERR_FILE_NOT_FOUND;
	  return 0;
	}

    next_dir_level:
      dirname += pathlen;

    } while (*dirname == '/');

  return 1;
}

int
iso9660_read (char *buf, int len)
{
  int sector, blkoffset, size, ret;

  if (INODE->file_start == 0)
    return 0;

  ret = 0;
  blkoffset = filepos & (ISO_SECTOR_SIZE - 1);
  sector = filepos >> ISO_SECTOR_BITS;
  while (len > 0)
    {
      size = ISO_SECTOR_SIZE - blkoffset;
      if (size > len)
        size = len;

      disk_read_func = disk_read_hook;

      if (!iso9660_devread(INODE->file_start + sector, blkoffset, size, buf))
	return 0;

      disk_read_func = NULL;

      len -= size;
      buf += size;
      ret += size;
      filepos += size;
      sector++;
      blkoffset = 0;
    }

  return ret;
}

#endif 
