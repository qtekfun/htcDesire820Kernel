/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999, 2001, 2003  Free Software Foundation, Inc.
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

#ifdef FSYS_EXT2FS

#include "shared.h"
#include "filesys.h"

static int mapblock1, mapblock2;

#define DEV_BSIZE 512

#define BLOCK_SIZE 1024		
#define WHICH_SUPER 1
#define SBLOCK (WHICH_SUPER * BLOCK_SIZE / DEV_BSIZE)	

typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;

#define EXT2_NDIR_BLOCKS                12
#define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1)

struct ext2_super_block
  {
    __u32 s_inodes_count;	
    __u32 s_blocks_count;	
    __u32 s_r_blocks_count;	
    __u32 s_free_blocks_count;	
    __u32 s_free_inodes_count;	
    __u32 s_first_data_block;	
    __u32 s_log_block_size;	
    __s32 s_log_frag_size;	
    __u32 s_blocks_per_group;	
    __u32 s_frags_per_group;	
    __u32 s_inodes_per_group;	
    __u32 s_mtime;		
    __u32 s_wtime;		
    __u16 s_mnt_count;		
    __s16 s_max_mnt_count;	
    __u16 s_magic;		
    __u16 s_state;		
    __u16 s_errors;		
    __u16 s_pad;
    __u32 s_lastcheck;		
    __u32 s_checkinterval;	
    __u32 s_creator_os;		
    __u32 s_rev_level;		
    __u16 s_def_resuid;		
    __u16 s_def_resgid;		
    __u32 s_reserved[235];	
  };

struct ext2_group_desc
  {
    __u32 bg_block_bitmap;	
    __u32 bg_inode_bitmap;	
    __u32 bg_inode_table;	
    __u16 bg_free_blocks_count;	
    __u16 bg_free_inodes_count;	
    __u16 bg_used_dirs_count;	
    __u16 bg_pad;
    __u32 bg_reserved[3];
  };

struct ext2_inode
  {
    __u16 i_mode;		
    __u16 i_uid;		
    __u32 i_size;		
    __u32 i_atime;		
    __u32 i_ctime;		
    __u32 i_mtime;		
    __u32 i_dtime;		
    __u16 i_gid;		
    __u16 i_links_count;	
    __u32 i_blocks;		
    __u32 i_flags;		
    union
      {
	struct
	  {
	    __u32 l_i_reserved1;
	  }
	linux1;
	struct
	  {
	    __u32 h_i_translator;
	  }
	hurd1;
	struct
	  {
	    __u32 m_i_reserved1;
	  }
	masix1;
      }
    osd1;			
    __u32 i_block[EXT2_N_BLOCKS];	
    __u32 i_version;		
    __u32 i_file_acl;		
    __u32 i_dir_acl;		
    __u32 i_faddr;		
    union
      {
	struct
	  {
	    __u8 l_i_frag;	
	    __u8 l_i_fsize;	
	    __u16 i_pad1;
	    __u32 l_i_reserved2[2];
	  }
	linux2;
	struct
	  {
	    __u8 h_i_frag;	
	    __u8 h_i_fsize;	
	    __u16 h_i_mode_high;
	    __u16 h_i_uid_high;
	    __u16 h_i_gid_high;
	    __u32 h_i_author;
	  }
	hurd2;
	struct
	  {
	    __u8 m_i_frag;	
	    __u8 m_i_fsize;	
	    __u16 m_pad1;
	    __u32 m_i_reserved2[2];
	  }
	masix2;
      }
    osd2;			
  };

#define NAME_MAX         255	

typedef long linux_off_t;

#define EXT2_NAME_LEN 255
struct ext2_dir_entry
  {
    __u32 inode;		
    __u16 rec_len;		
    __u8 name_len;		
    __u8 file_type;
    char name[EXT2_NAME_LEN];	
  };

#define EXT2_DIR_PAD                    4
#define EXT2_DIR_ROUND                  (EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(name_len)      (((name_len) + 8 + EXT2_DIR_ROUND) & \
                                         ~EXT2_DIR_ROUND)


#define log2(n) ffz(~(n))

#define EXT2_SUPER_MAGIC      0xEF53	
#define EXT2_ROOT_INO              2	
#define PATH_MAX                1024	
#define MAX_LINK_COUNT             5	

#define SUPERBLOCK \
    ((struct ext2_super_block *)(FSYS_BUF))
#define GROUP_DESC \
    ((struct ext2_group_desc *) \
     ((int)SUPERBLOCK + sizeof(struct ext2_super_block)))
#define INODE \
    ((struct ext2_inode *)((int)GROUP_DESC + EXT2_BLOCK_SIZE(SUPERBLOCK)))
#define DATABLOCK1 \
    ((int)((int)INODE + sizeof(struct ext2_inode)))
#define DATABLOCK2 \
    ((int)((int)DATABLOCK1 + EXT2_BLOCK_SIZE(SUPERBLOCK)))

#define EXT2_ADDR_PER_BLOCK(s)          (EXT2_BLOCK_SIZE(s) / sizeof (__u32))
#define EXT2_ADDR_PER_BLOCK_BITS(s)		(log2(EXT2_ADDR_PER_BLOCK(s)))

#define EXT2_BLOCK_SIZE_BITS(s)        ((s)->s_log_block_size + 10)
#define EXT2_BLOCK_SIZE(s)	(1 << EXT2_BLOCK_SIZE_BITS(s))
#define EXT2_DESC_PER_BLOCK(s) \
     (EXT2_BLOCK_SIZE(s) / sizeof (struct ext2_group_desc))
#define S_IFMT  00170000
#define S_IFLNK  0120000
#define S_IFREG  0100000
#define S_IFDIR  0040000
#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)

static __inline__ unsigned long
ffz (unsigned long word)
{
  __asm__ ("bsfl %1,%0"
:	   "=r" (word)
:	   "r" (~word));
  return word;
}

int
ext2fs_mount (void)
{
  int retval = 1;

  if ((((current_drive & 0x80) || (current_slice != 0))
       && (current_slice != PC_SLICE_TYPE_EXT2FS)
       && (current_slice != PC_SLICE_TYPE_LINUX_RAID)
       && (! IS_PC_SLICE_TYPE_BSD_WITH_FS (current_slice, FS_EXT2FS))
       && (! IS_PC_SLICE_TYPE_BSD_WITH_FS (current_slice, FS_OTHER)))
      || part_length < (SBLOCK + (sizeof (struct ext2_super_block) / DEV_BSIZE))
      || !devread (SBLOCK, 0, sizeof (struct ext2_super_block),
		   (char *) SUPERBLOCK)
      || SUPERBLOCK->s_magic != EXT2_SUPER_MAGIC)
      retval = 0;

  return retval;
}

static int
ext2_rdfsb (int fsblock, int buffer)
{
#ifdef E2DEBUG
  printf ("fsblock %d buffer %d\n", fsblock, buffer);
#endif 
  return devread (fsblock * (EXT2_BLOCK_SIZE (SUPERBLOCK) / DEV_BSIZE), 0,
		  EXT2_BLOCK_SIZE (SUPERBLOCK), (char *) buffer);
}

static int
ext2fs_block_map (int logical_block)
{

#ifdef E2DEBUG
  unsigned char *i;
  for (i = (unsigned char *) INODE;
       i < ((unsigned char *) INODE + sizeof (struct ext2_inode));
       i++)
    {
      printf ("%c", "0123456789abcdef"[*i >> 4]);
      printf ("%c", "0123456789abcdef"[*i % 16]);
      if (!((i + 1 - (unsigned char *) INODE) % 16))
	{
	  printf ("\n");
	}
      else
	{
	  printf (" ");
	}
    }
  printf ("logical block %d\n", logical_block);
#endif 

  
  if (logical_block < EXT2_NDIR_BLOCKS)
    {
#ifdef E2DEBUG
      printf ("returning %d\n", (unsigned char *) (INODE->i_block[logical_block]));
      printf ("returning %d\n", INODE->i_block[logical_block]);
#endif 
      return INODE->i_block[logical_block];
    }
  
  logical_block -= EXT2_NDIR_BLOCKS;
  
  if (logical_block < EXT2_ADDR_PER_BLOCK (SUPERBLOCK))
    {
      if (mapblock1 != 1
	  && !ext2_rdfsb (INODE->i_block[EXT2_IND_BLOCK], DATABLOCK1))
	{
	  errnum = ERR_FSYS_CORRUPT;
	  return -1;
	}
      mapblock1 = 1;
      return ((__u32 *) DATABLOCK1)[logical_block];
    }
  
  logical_block -= EXT2_ADDR_PER_BLOCK (SUPERBLOCK);
  
  if (logical_block < (1 << (EXT2_ADDR_PER_BLOCK_BITS (SUPERBLOCK) * 2)))
    {
      int bnum;
      if (mapblock1 != 2
	  && !ext2_rdfsb (INODE->i_block[EXT2_DIND_BLOCK], DATABLOCK1))
	{
	  errnum = ERR_FSYS_CORRUPT;
	  return -1;
	}
      mapblock1 = 2;
      if ((bnum = (((__u32 *) DATABLOCK1)
		   [logical_block >> EXT2_ADDR_PER_BLOCK_BITS (SUPERBLOCK)]))
	  != mapblock2
	  && !ext2_rdfsb (bnum, DATABLOCK2))
	{
	  errnum = ERR_FSYS_CORRUPT;
	  return -1;
	}
      mapblock2 = bnum;
      return ((__u32 *) DATABLOCK2)
	[logical_block & (EXT2_ADDR_PER_BLOCK (SUPERBLOCK) - 1)];
    }
  
  mapblock2 = -1;
  logical_block -= (1 << (EXT2_ADDR_PER_BLOCK_BITS (SUPERBLOCK) * 2));
  if (mapblock1 != 3
      && !ext2_rdfsb (INODE->i_block[EXT2_TIND_BLOCK], DATABLOCK1))
    {
      errnum = ERR_FSYS_CORRUPT;
      return -1;
    }
  mapblock1 = 3;
  if (!ext2_rdfsb (((__u32 *) DATABLOCK1)
		   [logical_block >> (EXT2_ADDR_PER_BLOCK_BITS (SUPERBLOCK)
				      * 2)],
		   DATABLOCK2))
    {
      errnum = ERR_FSYS_CORRUPT;
      return -1;
    }
  if (!ext2_rdfsb (((__u32 *) DATABLOCK2)
		   [(logical_block >> EXT2_ADDR_PER_BLOCK_BITS (SUPERBLOCK))
		    & (EXT2_ADDR_PER_BLOCK (SUPERBLOCK) - 1)],
		   DATABLOCK2))
    {
      errnum = ERR_FSYS_CORRUPT;
      return -1;
    }
  return ((__u32 *) DATABLOCK2)
    [logical_block & (EXT2_ADDR_PER_BLOCK (SUPERBLOCK) - 1)];
}

int
ext2fs_read (char *buf, int len)
{
  int logical_block;
  int offset;
  int map;
  int ret = 0;
  int size = 0;

#ifdef E2DEBUG
  static char hexdigit[] = "0123456789abcdef";
  unsigned char *i;
  for (i = (unsigned char *) INODE;
       i < ((unsigned char *) INODE + sizeof (struct ext2_inode));
       i++)
    {
      printf ("%c", hexdigit[*i >> 4]);
      printf ("%c", hexdigit[*i % 16]);
      if (!((i + 1 - (unsigned char *) INODE) % 16))
	{
	  printf ("\n");
	}
      else
	{
	  printf (" ");
	}
    }
#endif 
  while (len > 0)
    {
      
      logical_block = filepos >> EXT2_BLOCK_SIZE_BITS (SUPERBLOCK);
      offset = filepos & (EXT2_BLOCK_SIZE (SUPERBLOCK) - 1);
      map = ext2fs_block_map (logical_block);
#ifdef E2DEBUG
      printf ("map=%d\n", map);
#endif 
      if (map < 0)
	break;

      size = EXT2_BLOCK_SIZE (SUPERBLOCK);
      size -= offset;
      if (size > len)
	size = len;

      if (map == 0) {
        memset ((char *) buf, 0, size);
      } else {
        disk_read_func = disk_read_hook;

        devread (map * (EXT2_BLOCK_SIZE (SUPERBLOCK) / DEV_BSIZE),
	         offset, size, buf);

        disk_read_func = NULL;
      }

      buf += size;
      len -= size;
      filepos += size;
      ret += size;
    }

  if (errnum)
    ret = 0;

  return ret;
}



static inline
int ext2_is_fast_symlink (void)
{
  int ea_blocks;
  ea_blocks = INODE->i_file_acl ? EXT2_BLOCK_SIZE (SUPERBLOCK) / DEV_BSIZE : 0;
  return INODE->i_blocks == ea_blocks;
}

int
ext2fs_dir (char *dirname)
{
  int current_ino = EXT2_ROOT_INO;	
  int updir_ino = current_ino;	
  int group_id;			
  int group_desc;		
  int desc;			
  int ino_blk;			
  int str_chk = 0;		
  struct ext2_group_desc *gdp;
  struct ext2_inode *raw_inode;	

  char linkbuf[PATH_MAX];	
  int link_count = 0;

  char *rest;
  char ch;			

  int off;			
  int loc;			
  int blk;			
  long map;			
  struct ext2_dir_entry *dp;	
#ifdef E2DEBUG
  unsigned char *i;
#endif	


  while (1)
    {
#ifdef E2DEBUG
      printf ("inode %d\n", current_ino);
      printf ("dirname=%s\n", dirname);
#endif 

      
      group_id = (current_ino - 1) / (SUPERBLOCK->s_inodes_per_group);
      group_desc = group_id >> log2 (EXT2_DESC_PER_BLOCK (SUPERBLOCK));
      desc = group_id & (EXT2_DESC_PER_BLOCK (SUPERBLOCK) - 1);
#ifdef E2DEBUG
      printf ("ipg=%d, dpb=%d\n", SUPERBLOCK->s_inodes_per_group,
	      EXT2_DESC_PER_BLOCK (SUPERBLOCK));
      printf ("group_id=%d group_desc=%d desc=%d\n", group_id, group_desc, desc);
#endif 
      if (!ext2_rdfsb (
			(WHICH_SUPER + group_desc + SUPERBLOCK->s_first_data_block),
			(int) GROUP_DESC))
	{
	  return 0;
	}
      gdp = GROUP_DESC;
      ino_blk = gdp[desc].bg_inode_table +
	(((current_ino - 1) % (SUPERBLOCK->s_inodes_per_group))
	 >> log2 (EXT2_BLOCK_SIZE (SUPERBLOCK) / sizeof (struct ext2_inode)));
#ifdef E2DEBUG
      printf ("inode table fsblock=%d\n", ino_blk);
#endif 
      if (!ext2_rdfsb (ino_blk, (int) INODE))
	{
	  return 0;
	}

      
      mapblock2 = mapblock1 = -1;

      raw_inode = INODE +
	((current_ino - 1)
	 & (EXT2_BLOCK_SIZE (SUPERBLOCK) / sizeof (struct ext2_inode) - 1));
#ifdef E2DEBUG
      printf ("ipb=%d, sizeof(inode)=%d\n",
	      (EXT2_BLOCK_SIZE (SUPERBLOCK) / sizeof (struct ext2_inode)),
	      sizeof (struct ext2_inode));
      printf ("inode=%x, raw_inode=%x\n", INODE, raw_inode);
      printf ("offset into inode table block=%d\n", (int) raw_inode - (int) INODE);
      for (i = (unsigned char *) INODE; i <= (unsigned char *) raw_inode;
	   i++)
	{
	  printf ("%c", "0123456789abcdef"[*i >> 4]);
	  printf ("%c", "0123456789abcdef"[*i % 16]);
	  if (!((i + 1 - (unsigned char *) INODE) % 16))
	    {
	      printf ("\n");
	    }
	  else
	    {
	      printf (" ");
	    }
	}
      printf ("first word=%x\n", *((int *) raw_inode));
#endif 

      
      memmove ((void *) INODE, (void *) raw_inode, sizeof (struct ext2_inode));

#ifdef E2DEBUG
      printf ("first word=%x\n", *((int *) INODE));
#endif 

      
      if (S_ISLNK (INODE->i_mode))
	{
	  int len;
	  if (++link_count > MAX_LINK_COUNT)
	    {
	      errnum = ERR_SYMLINK_LOOP;
	      return 0;
	    }

	  
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

	  
	  if (! ext2_is_fast_symlink ())
	    {
	      
	      len = grub_read (linkbuf, filemax);
	      filepos = 0;
	      if (!len)
		return 0;
	    }
	  else
	    {
	      
	      len = filemax;
	      memmove (linkbuf, (char *) INODE->i_block, len);
	    }

#ifdef E2DEBUG
	  printf ("symlink=%s\n", linkbuf);
#endif

	  dirname = linkbuf;
	  if (*dirname == '/')
	    {
	      
	      current_ino = EXT2_ROOT_INO;
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

#ifdef E2DEBUG
	  printf ("dirname=%s, rest=%s, loc=%d\n", dirname, rest, loc);
#endif 

	  if (loc >= INODE->i_size)
	    {
	      if (print_possibilities < 0)
		{
# if 0
		  putchar ('\n');
# endif
		}
	      else
		{
		  errnum = ERR_FILE_NOT_FOUND;
		  *rest = ch;
		}
	      return (print_possibilities < 0);
	    }

	  
	  blk = loc >> EXT2_BLOCK_SIZE_BITS (SUPERBLOCK);

	  map = ext2fs_block_map (blk);
#ifdef E2DEBUG
	  printf ("fs block=%d\n", map);
#endif 
	  mapblock2 = -1;
	  if ((map < 0) || !ext2_rdfsb (map, DATABLOCK2))
	    {
	      errnum = ERR_FSYS_CORRUPT;
	      *rest = ch;
	      return 0;
	    }
	  off = loc & (EXT2_BLOCK_SIZE (SUPERBLOCK) - 1);
	  dp = (struct ext2_dir_entry *) (DATABLOCK2 + off);
	  
	  loc += dp->rec_len;

	  

#ifdef E2DEBUG
	  printf ("directory entry ino=%d\n", dp->inode);
	  if (dp->inode)
	    printf ("entry=%s\n", dp->name);
#endif 

	  if (dp->inode)
	    {
	      int saved_c = dp->name[dp->name_len];

	      dp->name[dp->name_len] = 0;
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

	      dp->name[dp->name_len] = saved_c;
	    }

	}
      while (!dp->inode || (str_chk || (print_possibilities && ch != '/')));

      current_ino = dp->inode;
      *(dirname = rest) = ch;
    }
  
}

#endif 
