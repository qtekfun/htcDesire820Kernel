/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002  Free Software Foundation, Inc.
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


#ifdef FSYS_MINIX

#include "shared.h"
#include "filesys.h"


static int mapblock1, mapblock2, namelen;

#define DEV_BSIZE 512

#define BLOCK_SIZE_BITS 10
#define BLOCK_SIZE 	(1<<BLOCK_SIZE_BITS)

#define WHICH_SUPER 1
#define SBLOCK (WHICH_SUPER * BLOCK_SIZE / DEV_BSIZE)	

typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;

#define MINIX_ROOT_INO 1

#define MINIX_LINK_MAX  250
#define MINIX2_LINK_MAX 65530

#define MINIX_I_MAP_SLOTS       8
#define MINIX_Z_MAP_SLOTS       64
#define MINIX_SUPER_MAGIC       0x137F          
#define MINIX_SUPER_MAGIC2      0x138F          
#define MINIX2_SUPER_MAGIC      0x2468          
#define MINIX2_SUPER_MAGIC2     0x2478          
#define MINIX_VALID_FS          0x0001          
#define MINIX_ERROR_FS          0x0002          

#define MINIX_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct minix_inode)))
#define MINIX2_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof (struct minix2_inode)))

#define MINIX_V1                0x0001          
#define MINIX_V2                0x0002          

#define INODE_VERSION(inode)	(SUPERBLOCK->s_version)

struct minix_inode {
	__u16 i_mode;
	__u16 i_uid;
	__u32 i_size;
	__u32 i_time;
	__u8  i_gid;
	__u8  i_nlinks;
	__u16 i_zone[9];
};

struct minix2_inode {
	__u16 i_mode;
	__u16 i_nlinks;
	__u16 i_uid;
	__u16 i_gid;
	__u32 i_size;
	__u32 i_atime;
	__u32 i_mtime;
	__u32 i_ctime;
	__u32 i_zone[10];
};

struct minix_super_block {
        __u16 s_ninodes;
        __u16 s_nzones;
        __u16 s_imap_blocks;
        __u16 s_zmap_blocks;
        __u16 s_firstdatazone;
        __u16 s_log_zone_size;
        __u32 s_max_size;
        __u16 s_magic;
        __u16 s_state;
        __u32 s_zones;
};

struct minix_dir_entry {
        __u16 inode;
        char name[0];
};

#define SUPERBLOCK \
    ((struct minix_super_block *)(FSYS_BUF))
#define INODE \
    ((struct minix_inode *)((int) SUPERBLOCK + BLOCK_SIZE))
#define DATABLOCK1 \
    ((int)((int)INODE + sizeof(struct minix_inode)))
#define DATABLOCK2 \
    ((int)((int)DATABLOCK1 + BLOCK_SIZE))

#define S_IFMT  00170000
#define S_IFLNK  0120000
#define S_IFREG  0100000
#define S_IFDIR  0040000
#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)

#define PATH_MAX                1024	
#define MAX_LINK_COUNT             5	

int
minix_mount (void)
{
  if (((current_drive & 0x80) || current_slice != 0)
      && ! IS_PC_SLICE_TYPE_MINIX (current_slice)
      && ! IS_PC_SLICE_TYPE_BSD_WITH_FS (current_slice, FS_OTHER))
    return 0;			
  
  if (part_length < (SBLOCK +
		     (sizeof (struct minix_super_block) / DEV_BSIZE)))
    return 0;			
  
  if (!devread (SBLOCK, 0, sizeof (struct minix_super_block),
		(char *) SUPERBLOCK))
    return 0;			
  
  switch (SUPERBLOCK->s_magic)
    {
    case MINIX_SUPER_MAGIC:
      namelen = 14;
      break;
    case MINIX_SUPER_MAGIC2:
      namelen = 30;
      break;
    default:
      return 0;			
    }

  return 1;
}

static int
minix_rdfsb (int fsblock, int buffer)
{
  return devread (fsblock * (BLOCK_SIZE / DEV_BSIZE), 0,
		  BLOCK_SIZE, (char *) buffer);
}

static int
minix_block_map (int logical_block)
{
  int i;

  if (logical_block < 7)
    return INODE->i_zone[logical_block];

  logical_block -= 7;
  if (logical_block < 512)
    {
      i = INODE->i_zone[7];
      
      if (!i || ((mapblock1 != 1)
		 && !minix_rdfsb (i, DATABLOCK1)))
	{
	  errnum = ERR_FSYS_CORRUPT;
	  return -1;
	}
      mapblock1 = 1;
      return ((__u16 *) DATABLOCK1) [logical_block];
    }

  logical_block -= 512;
  i = INODE->i_zone[8];
  if (!i || ((mapblock1 != 2)
	     && !minix_rdfsb (i, DATABLOCK1)))
    {
      errnum = ERR_FSYS_CORRUPT;
      return -1;
    }
  mapblock1 = 2;
  i = ((__u16 *) DATABLOCK1)[logical_block >> 9];
  if (!i || ((mapblock2 != i)
	     && !minix_rdfsb (i, DATABLOCK2)))
    {
      errnum = ERR_FSYS_CORRUPT;
      return -1;
    }
  mapblock2 = i;
  return ((__u16 *) DATABLOCK2)[logical_block & 511];
}

int
minix_read (char *buf, int len)
{
  int logical_block;
  int offset;
  int map;
  int ret = 0;
  int size = 0;

  while (len > 0)
    {
      
      logical_block = filepos >> BLOCK_SIZE_BITS;
      offset = filepos & (BLOCK_SIZE - 1);
      map = minix_block_map (logical_block);
#ifdef DEBUG_MINIX
      printf ("map=%d\n", map);
#endif
      if (map < 0)
	break;

      size = BLOCK_SIZE;
      size -= offset;
      if (size > len)
	size = len;

      disk_read_func = disk_read_hook;

      devread (map * (BLOCK_SIZE / DEV_BSIZE),
	       offset, size, buf);

      disk_read_func = NULL;

      buf += size;
      len -= size;
      filepos += size;
      ret += size;
    }

  if (errnum)
    ret = 0;

  return ret;
}

int
minix_dir (char *dirname)
{
  int current_ino = MINIX_ROOT_INO;  
  int updir_ino = current_ino;	     
  int ino_blk;			     

  int str_chk = 0;		     

  struct minix_inode * raw_inode;    

  char linkbuf[PATH_MAX];	     
  int link_count = 0;

  char * rest;
  char ch;

  int off;			     
  int loc;			     
  int blk;			     
  long map;			     
  struct minix_dir_entry * dp;	     


#ifdef DEBUG_MINIX
  printf ("\n");
#endif  

  while (1)
    {
#ifdef DEBUG_MINIX
      printf ("inode %d, dirname %s\n", current_ino, dirname);
#endif

      ino_blk = (2 + SUPERBLOCK->s_imap_blocks + SUPERBLOCK->s_zmap_blocks
		 + (current_ino - 1) / MINIX_INODES_PER_BLOCK);
      if (! minix_rdfsb (ino_blk, (int) INODE))
	return 0;

      
      mapblock2 = mapblock1 = -1;

      raw_inode = INODE + ((current_ino - 1) % MINIX_INODES_PER_BLOCK);

      
      memmove ((void *) INODE, (void *) raw_inode, 
	       sizeof (struct minix_inode));

      
      if (S_ISLNK (INODE->i_mode))
	{
	  int len;

	  if (++link_count > MAX_LINK_COUNT)
	    {
	      errnum = ERR_SYMLINK_LOOP;
	      return 0;
	    }
#ifdef DEBUG_MINIX
	  printf ("S_ISLNK (%s)\n", dirname);
#endif

	  
	  len = 0;
	  while (dirname[len] && !isspace (dirname[len]))
	    len++;

	  
	  filemax = (INODE->i_size);
	  if (filemax + len > sizeof (linkbuf) - 2)
	    {
	      errnum = ERR_FILELENGTH;
	      return 0;
	    }

	  if (len)
	    {
	      memmove (linkbuf + filemax, dirname, len);
	    }
	  linkbuf[filemax + len] = '\0';

	  
	  len = grub_read (linkbuf, filemax);
	  filepos = 0;
	  if (!len)
	    return 0;

#ifdef DEBUG_MINIX
	  printf ("symlink=%s\n", linkbuf);
#endif

	  dirname = linkbuf;
	  if (*dirname == '/')
	    {
	      
	      current_ino = MINIX_ROOT_INO;
	      updir_ino = current_ino;
	    }
	  else
	    {
	      
	      current_ino = updir_ino;
	    }

	  
	  continue;
	}

      
      if (!*dirname || isspace (*dirname))
	{
	  if (!S_ISREG (INODE->i_mode))
	    {
	      errnum = ERR_BAD_FILETYPE;
	      return 0;
	    }

	  filemax = (INODE->i_size);
	  return 1;
	}

      
      updir_ino = current_ino;

      
      while (*dirname == '/')
	dirname++;

      if (!(INODE->i_size) || !S_ISDIR (INODE->i_mode))
	{
	  errnum = ERR_BAD_FILETYPE;
	  return 0;
	}

      
      for (rest = dirname; (ch = *rest) && !isspace (ch) && ch != '/';
	   rest++);

      
      
      *rest = 0;
      loc = 0;

      do
	{
#ifdef DEBUG_MINIX
	  printf ("dirname=`%s', rest=`%s', loc=%d\n", dirname, rest, loc);
#endif

	  if (loc >= INODE->i_size)
	    {
	      if (print_possibilities < 0)
		{
#if 0
		  putchar ('\n');
#endif
		}
	      else
		{
		  errnum = ERR_FILE_NOT_FOUND;
		  *rest = ch;
		}
	      return (print_possibilities < 0);
	    }

	  
	  blk = loc >> BLOCK_SIZE_BITS;

	  map = minix_block_map (blk);
#ifdef DEBUG_MINIX
	  printf ("fs block=%d\n", map);
#endif
	  mapblock2 = -1;
	  if ((map < 0) || !minix_rdfsb (map, DATABLOCK2))
	    {
	      errnum = ERR_FSYS_CORRUPT;
	      *rest = ch;
	      return 0;
	    }
	  off = loc & (BLOCK_SIZE - 1);
	  dp = (struct minix_dir_entry *) (DATABLOCK2 + off);
	  
	  loc += sizeof (dp->inode) + namelen;


#ifdef DEBUG_MINIX
	  printf ("directory entry ino=%d\n", dp->inode);
	  if (dp->inode)
	    printf ("entry=%s\n", dp->name);
#endif

	  if (dp->inode)
	    {
	      int saved_c = dp->name[namelen];

	      dp->name[namelen] = 0;
	      str_chk = substring (dirname, dp->name);

# ifndef STAGE1_5
	      if (print_possibilities && ch != '/'
		  && (!*dirname || str_chk <= 0))
		{
		  if (print_possibilities > 0)
		    print_possibilities = -print_possibilities;
		  print_a_completion (dp->name);
		}
# endif

	      dp->name[namelen] = saved_c;
	    }

	}
      while (!dp->inode || (str_chk || (print_possibilities && ch != '/')));

      current_ino = dp->inode;
      *(dirname = rest) = ch;
    }
  
}

#endif 
