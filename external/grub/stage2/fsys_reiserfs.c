/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000, 2001  Free Software Foundation, Inc.
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

#ifdef FSYS_REISERFS
#include "shared.h"
#include "filesys.h"

#undef REISERDEBUG


typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;
typedef unsigned long long __u64;

typedef long linux_off_t;

#define __cpu_to_le64(x) ((__u64) (x))
#define __le64_to_cpu(x) ((__u64) (x))
#define __cpu_to_le32(x) ((__u32) (x))
#define __le32_to_cpu(x) ((__u32) (x))
#define __cpu_to_le16(x) ((__u16) (x))
#define __le16_to_cpu(x) ((__u16) (x))

struct reiserfs_super_block
{
  __u32 s_block_count;			
  __u32 s_free_blocks;                  
  __u32 s_root_block;           	
  __u32 s_journal_block;           	
  __u32 s_journal_dev;           	
  __u32 s_journal_size; 		
  __u32 s_journal_trans_max;            
  __u32 s_journal_magic;                
  __u32 s_journal_max_batch;            
  __u32 s_journal_max_commit_age;       
  __u32 s_journal_max_trans_age;        
  __u16 s_blocksize;                   	
  __u16 s_oid_maxsize;			
  __u16 s_oid_cursize;			
  __u16 s_state;                       	
  char s_magic[16];                     
  __u16 s_tree_height;                  
  __u16 s_bmap_nr;                      
  __u16 s_version;
  char s_unused[128];			
};

#define REISERFS_MAX_SUPPORTED_VERSION 2
#define REISERFS_SUPER_MAGIC_STRING "ReIsErFs"
#define REISER2FS_SUPER_MAGIC_STRING "ReIsEr2Fs"
#define REISER3FS_SUPER_MAGIC_STRING "ReIsEr3Fs"

#define MAX_HEIGHT 7

#define JOURNAL_TRANS_HALF 1018

/* first block written in a commit.  */
struct reiserfs_journal_desc {
  __u32 j_trans_id;			
  __u32 j_len;				
  __u32 j_mount_id;			
  __u32 j_realblock[JOURNAL_TRANS_HALF]; 
  char j_magic[12];
};

/* last block written in a commit */
struct reiserfs_journal_commit {
  __u32 j_trans_id;			
  __u32 j_len;			
  __u32 j_realblock[JOURNAL_TRANS_HALF]; 
  char j_digest[16];			
};

/* this header block gets written whenever a transaction is considered
   fully flushed, and is more recent than the last fully flushed
   transaction.  
   fully flushed means all the log blocks and all the real blocks are
   on disk, and this transaction does not need to be replayed.  
*/
struct reiserfs_journal_header {
  
  __u32 j_last_flush_trans_id;
  
  __u32 j_first_unflushed_offset;
  
  __u32 j_mount_id;
};

#define JOURNAL_DESC_MAGIC "ReIsErLB" 


struct offset_v1
{
  __u32 k_offset;
  __u32 k_uniqueness;
};

struct offset_v2
{
  __u64 k_offset:60;
  __u64 k_type: 4;
};


struct key
{
  
  __u32 k_dir_id;
  
  __u32 k_objectid;
  
  union
  {
    struct offset_v1 v1;
    struct offset_v2 v2;
  }
  u;
};

#define KEY_SIZE (sizeof (struct key))

struct block_head
{       
  __u16 blk_level;        
  __u16 blk_nr_item;      
  __u16 blk_free_space;   
  struct key  blk_right_delim_key; 
};
#define BLKH_SIZE (sizeof (struct block_head))
#define DISK_LEAF_NODE_LEVEL  1 

struct item_head
{
  struct key ih_key; 	
  
  union
  {
    __u16 ih_free_space; 
    __u16 ih_entry_count; 
  }
  u;
  __u16 ih_item_len;           
  __u16 ih_item_location;      
  __u16 ih_version;	       
};
#define IH_SIZE (sizeof (struct item_head))

#define ITEM_VERSION_1 0
#define ITEM_VERSION_2 1
#define IH_KEY_OFFSET(ih) ((ih)->ih_version == ITEM_VERSION_1 \
			   ? (ih)->ih_key.u.v1.k_offset \
			   : (ih)->ih_key.u.v2.k_offset)

#define IH_KEY_ISTYPE(ih, type) ((ih)->ih_version == ITEM_VERSION_1 \
				 ? (ih)->ih_key.u.v1.k_uniqueness == V1_##type \
				 : (ih)->ih_key.u.v2.k_type == V2_##type)

struct disk_child
{
  unsigned long       dc_block_number;              
  unsigned short      dc_size;		            
};

#define DC_SIZE (sizeof (struct disk_child))

struct stat_data
{
  __u16 sd_mode;	
  __u16 sd_notused1[3]; 
  __u32 sd_size;	
  __u32 sd_size_hi;	
};

struct reiserfs_de_head
{
  __u32 deh_offset;  
  __u32 deh_dir_id;  
  __u32 deh_objectid;
  __u16 deh_location;
  __u16 deh_state;   
};

#define DEH_SIZE (sizeof (struct reiserfs_de_head))

#define DEH_Statdata (1 << 0)			
#define DEH_Visible  (1 << 2)

#define SD_OFFSET  0
#define SD_UNIQUENESS 0
#define DOT_OFFSET 1
#define DOT_DOT_OFFSET 2
#define DIRENTRY_UNIQUENESS 500

#define V1_TYPE_STAT_DATA 0x0
#define V1_TYPE_DIRECT 0xffffffff
#define V1_TYPE_INDIRECT 0xfffffffe
#define V1_TYPE_DIRECTORY_MAX 0xfffffffd
#define V2_TYPE_STAT_DATA 0
#define V2_TYPE_INDIRECT 1
#define V2_TYPE_DIRECT 2
#define V2_TYPE_DIRENTRY 3 

#define REISERFS_ROOT_OBJECTID 2
#define REISERFS_ROOT_PARENT_OBJECTID 1
#define REISERFS_DISK_OFFSET_IN_BYTES (64 * 1024)
#define REISERFS_OLD_DISK_OFFSET_IN_BYTES (8 * 1024)
#define REISERFS_OLD_BLOCKSIZE 4096

#define S_ISREG(mode) (((mode) & 0170000) == 0100000)
#define S_ISDIR(mode) (((mode) & 0170000) == 0040000)
#define S_ISLNK(mode) (((mode) & 0170000) == 0120000)

#define PATH_MAX       1024	
#define MAX_LINK_COUNT    5	

#define FSYSREISER_CACHE_SIZE 24*1024
#define FSYSREISER_MIN_BLOCKSIZE SECTOR_SIZE
#define FSYSREISER_MAX_BLOCKSIZE FSYSREISER_CACHE_SIZE / 3

struct fsys_reiser_fileinfo
{
  __u32 k_dir_id;
  __u32 k_objectid;
};

struct fsys_reiser_info
{
  
  struct item_head *current_ih;
  
  char *current_item;
  
  struct fsys_reiser_fileinfo fileinfo;
  
  __u32 journal_block;
  
  __u32 journal_block_count;
  __u32 journal_first_desc;

  
  __u16 version;
  
  __u16 tree_depth;
  
  __u8  blocksize_shift;
  
  __u8  fullblocksize_shift;
  
  __u16 blocksize;
  
  __u16 cached_slots;
  
  __u16 journal_transactions;
  
  unsigned int blocks[MAX_HEIGHT];
  unsigned int next_key_nr[MAX_HEIGHT];
};

#define ROOT     ((char *) ((int) FSYS_BUF))
#define CACHE(i) (ROOT + ((i) << INFO->fullblocksize_shift))
#define LEAF     CACHE (DISK_LEAF_NODE_LEVEL)

#define BLOCKHEAD(cache) ((struct block_head *) cache)
#define ITEMHEAD         ((struct item_head  *) ((int) LEAF + BLKH_SIZE))
#define KEY(cache)       ((struct key        *) ((int) cache + BLKH_SIZE))
#define DC(cache)        ((struct disk_child *) \
			  ((int) cache + BLKH_SIZE + KEY_SIZE * nr_item))
#define INFO \
    ((struct fsys_reiser_info *) ((int) FSYS_BUF + FSYSREISER_CACHE_SIZE))
#define JOURNAL_START    ((__u32 *) (INFO + 1))
#define JOURNAL_END      ((__u32 *) (FSYS_BUF + FSYS_BUFLEN))


static __inline__ unsigned long
log2 (unsigned long word)
{
  __asm__ ("bsfl %1,%0"
	   : "=r" (word)
	   : "r" (word));
  return word;
}

static __inline__ int
is_power_of_two (unsigned long word)
{
  return (word & -word) == word;
}

static int 
journal_read (int block, int len, char *buffer) 
{
  return devread ((INFO->journal_block + block) << INFO->blocksize_shift, 
		  0, len, buffer);
}

static int
block_read (int blockNr, int start, int len, char *buffer)
{
  int transactions = INFO->journal_transactions;
  int desc_block = INFO->journal_first_desc;
  int journal_mask = INFO->journal_block_count - 1;
  int translatedNr = blockNr;
  __u32 *journal_table = JOURNAL_START;
  while (transactions-- > 0) 
    {
      int i = 0;
      int j_len;
      if (*journal_table != 0xffffffff)
	{
	  
	  j_len = *journal_table++;
	  while (i++ < j_len)
	    {
	      if (*journal_table++ == blockNr)
		{
		  journal_table += j_len - i;
		  goto found;
		}
	    }
	}
      else
	{
	  struct reiserfs_journal_desc   desc;
	  struct reiserfs_journal_commit commit;

	  if (! journal_read (desc_block, sizeof (desc), (char *) &desc))
	    return 0;

	  j_len = desc.j_len;
	  while (i < j_len && i < JOURNAL_TRANS_HALF)
	    if (desc.j_realblock[i++] == blockNr)
	      goto found;
	  
	  if (j_len >= JOURNAL_TRANS_HALF)
	    {
	      int commit_block = (desc_block + 1 + j_len) & journal_mask;
	      if (! journal_read (commit_block, 
				  sizeof (commit), (char *) &commit))
		return 0;
	      while (i < j_len)
		if (commit.j_realblock[i++ - JOURNAL_TRANS_HALF] == blockNr)
		  goto found;
	    }
	}
      goto not_found;
      
    found:
      translatedNr = INFO->journal_block + ((desc_block + i) & journal_mask);
#ifdef REISERDEBUG
      printf ("block_read: block %d is mapped to journal block %d.\n", 
	      blockNr, translatedNr - INFO->journal_block);
#endif
      /* We must continue the search, as this block may be overwritten
       * in later transactions.
       */
    not_found:
      desc_block = (desc_block + 2 + j_len) & journal_mask;
    }
  return devread (translatedNr << INFO->blocksize_shift, start, len, buffer);
}

static int
journal_init (void)
{
  unsigned int block_count = INFO->journal_block_count;
  unsigned int desc_block;
  unsigned int commit_block;
  unsigned int next_trans_id;
  struct reiserfs_journal_header header;
  struct reiserfs_journal_desc   desc;
  struct reiserfs_journal_commit commit;
  __u32 *journal_table = JOURNAL_START;

  journal_read (block_count, sizeof (header), (char *) &header);
  desc_block = header.j_first_unflushed_offset;
  if (desc_block >= block_count)
    return 0;

  INFO->journal_first_desc = desc_block;
  next_trans_id = header.j_last_flush_trans_id + 1;

#ifdef REISERDEBUG
  printf ("journal_init: last flushed %d\n", 
	  header.j_last_flush_trans_id);
#endif

  while (1) 
    {
      journal_read (desc_block, sizeof (desc), (char *) &desc);
      if (substring (JOURNAL_DESC_MAGIC, desc.j_magic) > 0
	  || desc.j_trans_id != next_trans_id
	  || desc.j_mount_id != header.j_mount_id)
	
	break;
      
      commit_block = (desc_block + desc.j_len + 1) & (block_count - 1);
      journal_read (commit_block, sizeof (commit), (char *) &commit);
      if (desc.j_trans_id != commit.j_trans_id
	  || desc.j_len != commit.j_len)
	
	break;
      
#ifdef REISERDEBUG
      printf ("Found valid transaction %d/%d at %d.\n", 
	      desc.j_trans_id, desc.j_mount_id, desc_block);
#endif

      next_trans_id++;
      if (journal_table < JOURNAL_END)
	{
	  if ((journal_table + 1 + desc.j_len) >= JOURNAL_END)
	    {
	      *journal_table = 0xffffffff;
	      journal_table = JOURNAL_END;
	    }
	  else
	    {
	      int i;
	      *journal_table++ = desc.j_len;
	      for (i = 0; i < desc.j_len && i < JOURNAL_TRANS_HALF; i++)
		{
		  *journal_table++ = desc.j_realblock[i];
#ifdef REISERDEBUG
		  printf ("block %d is in journal %d.\n", 
			  desc.j_realblock[i], desc_block);
#endif
		}
	      for (     ; i < desc.j_len; i++)
		{
		  *journal_table++ = commit.j_realblock[i-JOURNAL_TRANS_HALF];
#ifdef REISERDEBUG
		  printf ("block %d is in journal %d.\n", 
			  commit.j_realblock[i-JOURNAL_TRANS_HALF], 
			  desc_block);
#endif
		}
	    }
	}
      desc_block = (commit_block + 1) & (block_count - 1);
    }
#ifdef REISERDEBUG
  printf ("Transaction %d/%d at %d isn't valid.\n", 
	  desc.j_trans_id, desc.j_mount_id, desc_block);
#endif

  INFO->journal_transactions
    = next_trans_id - header.j_last_flush_trans_id - 1;
  return errnum == 0;
}

int
reiserfs_mount (void)
{
  struct reiserfs_super_block super;
  int superblock = REISERFS_DISK_OFFSET_IN_BYTES >> SECTOR_BITS;

  if (part_length < superblock + (sizeof (super) >> SECTOR_BITS)
      || ! devread (superblock, 0, sizeof (struct reiserfs_super_block), 
		(char *) &super)
      || (substring (REISER3FS_SUPER_MAGIC_STRING, super.s_magic) > 0
	  && substring (REISER2FS_SUPER_MAGIC_STRING, super.s_magic) > 0
	  && substring (REISERFS_SUPER_MAGIC_STRING, super.s_magic) > 0)
      || (
	  super.s_journal_block * super.s_blocksize
	  <= REISERFS_DISK_OFFSET_IN_BYTES))
    {
      
      superblock = REISERFS_OLD_DISK_OFFSET_IN_BYTES >> SECTOR_BITS;
      if (part_length < superblock + (sizeof (super) >> SECTOR_BITS)
	  || ! devread (superblock, 0, sizeof (struct reiserfs_super_block), 
			(char *) &super))
	return 0;

      if (substring (REISER3FS_SUPER_MAGIC_STRING, super.s_magic) > 0
	  && substring (REISER2FS_SUPER_MAGIC_STRING, super.s_magic) > 0
	  && substring (REISERFS_SUPER_MAGIC_STRING, super.s_magic) > 0)
	{
	  
	  if (substring (REISERFS_SUPER_MAGIC_STRING, 
			 (char*) ((int) &super + 20)) > 0)
	    return 0;
	  
	  super.s_blocksize = REISERFS_OLD_BLOCKSIZE;
	  super.s_journal_block = 0;
	  super.s_version = 0;
	}
    }

  
  if (super.s_version > REISERFS_MAX_SUPPORTED_VERSION)
    return 0;
  
  INFO->version = super.s_version;
  INFO->blocksize = super.s_blocksize;
  INFO->fullblocksize_shift = log2 (super.s_blocksize);
  INFO->blocksize_shift = INFO->fullblocksize_shift - SECTOR_BITS;
  INFO->cached_slots = 
    (FSYSREISER_CACHE_SIZE >> INFO->fullblocksize_shift) - 1;

#ifdef REISERDEBUG
  printf ("reiserfs_mount: version=%d, blocksize=%d\n", 
	  INFO->version, INFO->blocksize);
#endif 

  
  memset (INFO->blocks, 0, sizeof (INFO->blocks));

  if (super.s_blocksize < FSYSREISER_MIN_BLOCKSIZE
      || super.s_blocksize > FSYSREISER_MAX_BLOCKSIZE
      || (SECTOR_SIZE << INFO->blocksize_shift) != super.s_blocksize)
    return 0;

  INFO->journal_transactions = 0;
  if (super.s_journal_block != 0 && super.s_journal_dev == 0)
    {
      INFO->journal_block = super.s_journal_block;
      INFO->journal_block_count = super.s_journal_size;
      if (is_power_of_two (INFO->journal_block_count))
	journal_init ();

      
      block_read (superblock >> INFO->blocksize_shift, 
		  0, sizeof (struct reiserfs_super_block), (char *) &super);
    }

  if (! block_read (super.s_root_block, 0, INFO->blocksize, (char*) ROOT))
    return 0;
  
  INFO->tree_depth = BLOCKHEAD (ROOT)->blk_level;
  
#ifdef REISERDEBUG
  printf ("root read_in: block=%d, depth=%d\n", 
	  super.s_root_block, INFO->tree_depth);
#endif 

  if (INFO->tree_depth >= MAX_HEIGHT)
    return 0;
  if (INFO->tree_depth == DISK_LEAF_NODE_LEVEL)
    {
      memcpy (LEAF, ROOT, INFO->blocksize);
    }
  return 1;
}



static char *
read_tree_node (unsigned int blockNr, int depth)
{
  char* cache = CACHE(depth);
  int num_cached = INFO->cached_slots;
  if (depth < num_cached)
    {
      if (blockNr == INFO->blocks[depth])
	return cache;
    }
  else
    cache = CACHE(num_cached);

#ifdef REISERDEBUG
  printf ("  next read_in: block=%d (depth=%d)\n",
	  blockNr, depth);
#endif 
  if (! block_read (blockNr, 0, INFO->blocksize, cache))
    return 0;
  
  if (BLOCKHEAD (cache)->blk_level != depth)
    {
      errnum = ERR_FSYS_CORRUPT;
      return 0;
    }

  INFO->blocks[depth] = blockNr;
  return cache;
}

static int
next_key (void)
{
  int depth;
  struct item_head *ih = INFO->current_ih + 1;
  char *cache;
  
#ifdef REISERDEBUG
  printf ("next_key:\n  old ih: key %d:%d:%d:%d version:%d\n", 
	  INFO->current_ih->ih_key.k_dir_id, 
	  INFO->current_ih->ih_key.k_objectid, 
	  INFO->current_ih->ih_key.u.v1.k_offset,
	  INFO->current_ih->ih_key.u.v1.k_uniqueness,
	  INFO->current_ih->ih_version);
#endif 
  
  if (ih == &ITEMHEAD[BLOCKHEAD (LEAF)->blk_nr_item])
    {
      depth = DISK_LEAF_NODE_LEVEL;
      do
	{
	  if (depth == INFO->tree_depth)
	    {
	      ih = (struct item_head *) &BLOCKHEAD (LEAF)->blk_right_delim_key;
	      goto found;
	    }
	  depth++;
#ifdef REISERDEBUG
	  printf ("  depth=%d, i=%d\n", depth, INFO->next_key_nr[depth]);
#endif 
	}
      while (INFO->next_key_nr[depth] == 0);
      
      if (depth == INFO->tree_depth)
	cache = ROOT;
      else if (depth <= INFO->cached_slots)
	cache = CACHE (depth);
      else 
	{
	  cache = read_tree_node (INFO->blocks[depth], depth);
	  if (! cache)
	    return 0;
	}
      
      do
	{
	  int nr_item = BLOCKHEAD (cache)->blk_nr_item;
	  int key_nr = INFO->next_key_nr[depth]++;
#ifdef REISERDEBUG
	  printf ("  depth=%d, i=%d/%d\n", depth, key_nr, nr_item);
#endif 
	  if (key_nr == nr_item)
	    
	    INFO->next_key_nr[depth] = 0;

	  cache = read_tree_node (DC (cache)[key_nr].dc_block_number, --depth);
	  if (! cache)
	    return 0;
	}
      while (depth > DISK_LEAF_NODE_LEVEL);
      
      ih = ITEMHEAD;
    }
 found:
  INFO->current_ih   = ih;
  INFO->current_item = &LEAF[ih->ih_item_location];
#ifdef REISERDEBUG
  printf ("  new ih: key %d:%d:%d:%d version:%d\n", 
	  INFO->current_ih->ih_key.k_dir_id, 
	  INFO->current_ih->ih_key.k_objectid, 
	  INFO->current_ih->ih_key.u.v1.k_offset,
	  INFO->current_ih->ih_key.u.v1.k_uniqueness,
	  INFO->current_ih->ih_version);
#endif 
  return 1;
}

static int
search_stat (__u32 dir_id, __u32 objectid) 
{
  char *cache;
  int depth;
  int nr_item;
  int i;
  struct item_head *ih;
#ifdef REISERDEBUG
  printf ("search_stat:\n  key %d:%d:0:0\n", dir_id, objectid);
#endif 
  
  depth = INFO->tree_depth;
  cache = ROOT;
  
  while (depth > DISK_LEAF_NODE_LEVEL)
    {
      struct key *key;
      nr_item = BLOCKHEAD (cache)->blk_nr_item;
      
      key = KEY (cache);
      
      for (i = 0; i < nr_item; i++) 
	{
	  if (key->k_dir_id > dir_id
	      || (key->k_dir_id == dir_id 
		  && (key->k_objectid > objectid
		      || (key->k_objectid == objectid
			  && (key->u.v1.k_offset
			      | key->u.v1.k_uniqueness) > 0))))
	    break;
	  key++;
	}
      
#ifdef REISERDEBUG
      printf ("  depth=%d, i=%d/%d\n", depth, i, nr_item);
#endif 
      INFO->next_key_nr[depth] = (i == nr_item) ? 0 : i+1;
      cache = read_tree_node (DC (cache)[i].dc_block_number, --depth);
      if (! cache)
	return 0;
    }
  
  
  nr_item = BLOCKHEAD (LEAF)->blk_nr_item;
  ih = ITEMHEAD;
  for (i = 0; i < nr_item; i++) 
    {
      if (ih->ih_key.k_dir_id == dir_id 
	  && ih->ih_key.k_objectid == objectid
	  && ih->ih_key.u.v1.k_offset == 0
	  && ih->ih_key.u.v1.k_uniqueness == 0)
	{
#ifdef REISERDEBUG
	  printf ("  depth=%d, i=%d/%d\n", depth, i, nr_item);
#endif 
	  INFO->current_ih   = ih;
	  INFO->current_item = &LEAF[ih->ih_item_location];
	  return 1;
	}
      ih++;
    }
  errnum = ERR_FSYS_CORRUPT;
  return 0;
}

int
reiserfs_read (char *buf, int len)
{
  unsigned int blocksize;
  unsigned int offset;
  unsigned int to_read;
  char *prev_buf = buf;
  
#ifdef REISERDEBUG
  printf ("reiserfs_read: filepos=%d len=%d, offset=%x:%x\n",
	  filepos, len, (__u64) IH_KEY_OFFSET (INFO->current_ih) - 1);
#endif 
  
  if (INFO->current_ih->ih_key.k_objectid != INFO->fileinfo.k_objectid
      || IH_KEY_OFFSET (INFO->current_ih) > filepos + 1)
    {
      search_stat (INFO->fileinfo.k_dir_id, INFO->fileinfo.k_objectid);
      goto get_next_key;
    }
  
  while (! errnum)
    {
      if (INFO->current_ih->ih_key.k_objectid != INFO->fileinfo.k_objectid)
	break;
      
      offset = filepos - IH_KEY_OFFSET (INFO->current_ih) + 1;
      blocksize = INFO->current_ih->ih_item_len;
      
#ifdef REISERDEBUG
      printf ("  loop: filepos=%d len=%d, offset=%d blocksize=%d\n",
	      filepos, len, offset, blocksize);
#endif 
      
      if (IH_KEY_ISTYPE(INFO->current_ih, TYPE_DIRECT)
	  && offset < blocksize)
	{
#ifdef REISERDEBUG
	  printf ("direct_read: offset=%d, blocksize=%d\n",
		  offset, blocksize);
#endif 
	  to_read = blocksize - offset;
	  if (to_read > len)
	    to_read = len;
	  
	  if (disk_read_hook != NULL)
	    {
	      disk_read_func = disk_read_hook;
	      
	      block_read (INFO->blocks[DISK_LEAF_NODE_LEVEL],
			  (INFO->current_item - LEAF + offset), to_read, buf);
	      
	      disk_read_func = NULL;
	    }
	  else
	    memcpy (buf, INFO->current_item + offset, to_read);
	  goto update_buf_len;
	}
      else if (IH_KEY_ISTYPE(INFO->current_ih, TYPE_INDIRECT))
	{
	  blocksize = (blocksize >> 2) << INFO->fullblocksize_shift;
#ifdef REISERDEBUG
	  printf ("indirect_read: offset=%d, blocksize=%d\n",
		  offset, blocksize);
#endif 
	  
	  while (offset < blocksize)
	    {
	      __u32 blocknr = ((__u32 *) INFO->current_item)
		[offset >> INFO->fullblocksize_shift];
	      int blk_offset = offset & (INFO->blocksize-1);
	      
	      to_read = INFO->blocksize - blk_offset;
	      if (to_read > len)
		to_read = len;
	      
	      disk_read_func = disk_read_hook;
	      
	      devread (blocknr << INFO->blocksize_shift,
		       blk_offset, to_read, buf);
	      
	      disk_read_func = NULL;
	    update_buf_len:
	      len -= to_read;
	      buf += to_read;
	      offset += to_read;
	      filepos += to_read;
	      if (len == 0)
		goto done;
	    }
	}
    get_next_key:
      next_key ();
    }
 done:
  return errnum ? 0 : buf - prev_buf;
}


int
reiserfs_dir (char *dirname)
{
  struct reiserfs_de_head *de_head;
  char *rest, ch;
  __u32 dir_id, objectid, parent_dir_id = 0, parent_objectid = 0;
#ifndef STAGE1_5
  int do_possibilities = 0;
#endif 
  char linkbuf[PATH_MAX];	
  int link_count = 0;
  int mode;

  dir_id = REISERFS_ROOT_PARENT_OBJECTID;
  objectid = REISERFS_ROOT_OBJECTID;
  
  while (1)
    {
#ifdef REISERDEBUG
      printf ("dirname=%s\n", dirname);
#endif 
      
      
      if (! search_stat (dir_id, objectid))
	return 0;
      
#ifdef REISERDEBUG
      printf ("sd_mode=%x sd_size=%d\n", 
	      ((struct stat_data *) INFO->current_item)->sd_mode,
	      ((struct stat_data *) INFO->current_item)->sd_size);
#endif 
      
      mode = ((struct stat_data *) INFO->current_item)->sd_mode;

      
      if (S_ISLNK (mode))
	{
	  int len;
	  if (++link_count > MAX_LINK_COUNT)
	    {
	      errnum = ERR_SYMLINK_LOOP;
	      return 0;
	    }

	  
	  filemax = ((struct stat_data *) INFO->current_item)->sd_size;

	  
	  len = 0;
	  while (dirname[len] && !isspace (dirname[len]))
	    len++;

	  if (filemax + len > sizeof (linkbuf) - 1)
	    {
	      errnum = ERR_FILELENGTH;
	      return 0;
	    }
 	  
	  grub_memmove (linkbuf + filemax, dirname, len+1);

	  INFO->fileinfo.k_dir_id = dir_id;
	  INFO->fileinfo.k_objectid = objectid;
  	  filepos = 0;
	  if (! next_key ()
	      || reiserfs_read (linkbuf, filemax) != filemax)
	    {
	      if (! errnum)
		errnum = ERR_FSYS_CORRUPT;
	      return 0;
	    }

#ifdef REISERDEBUG
	  printf ("symlink=%s\n", linkbuf);
#endif 

	  dirname = linkbuf;
	  if (*dirname == '/')
	    {
	      
	      dir_id = REISERFS_ROOT_PARENT_OBJECTID;
	      objectid = REISERFS_ROOT_OBJECTID;
	    }
	  else
	    {
	      
	      dir_id   = parent_dir_id;
	      objectid = parent_objectid;
	    }

	  
	  continue;
	}

      
      if (! *dirname || isspace (*dirname))
	{
	  if (! S_ISREG (mode))
	    {
	      errnum = ERR_BAD_FILETYPE;
	      return 0;
	    }
	  
	  filepos = 0;
	  filemax = ((struct stat_data *) INFO->current_item)->sd_size;
	  
	  if (INFO->current_ih->ih_version == ITEM_VERSION_2
	      && ((struct stat_data *) INFO->current_item)->sd_size_hi > 0)
	    filemax = 0xffffffff;
	  
	  INFO->fileinfo.k_dir_id = dir_id;
	  INFO->fileinfo.k_objectid = objectid;
	  return next_key ();
	}
      
      
      while (*dirname == '/')
	dirname++;
      if (! S_ISDIR (mode))
	{
	  errnum = ERR_BAD_FILETYPE;
	  return 0;
	}
      for (rest = dirname; (ch = *rest) && ! isspace (ch) && ch != '/'; rest++);
      *rest = 0;
      
# ifndef STAGE1_5
      if (print_possibilities && ch != '/')
	do_possibilities = 1;
# endif 
      
      while (1)
	{
	  char *name_end;
	  int num_entries;
	  
	  if (! next_key ())
	    return 0;
#ifdef REISERDEBUG
	  printf ("ih: key %d:%d:%d:%d version:%d\n", 
		  INFO->current_ih->ih_key.k_dir_id, 
		  INFO->current_ih->ih_key.k_objectid, 
		  INFO->current_ih->ih_key.u.v1.k_offset,
		  INFO->current_ih->ih_key.u.v1.k_uniqueness,
		  INFO->current_ih->ih_version);
#endif 
	  
	  if (INFO->current_ih->ih_key.k_objectid != objectid)
	    break;
	  
	  name_end = INFO->current_item + INFO->current_ih->ih_item_len;
	  de_head = (struct reiserfs_de_head *) INFO->current_item;
	  num_entries = INFO->current_ih->u.ih_entry_count;
	  while (num_entries > 0)
	    {
	      char *filename = INFO->current_item + de_head->deh_location;
	      char  tmp = *name_end;
	      if ((de_head->deh_state & DEH_Visible))
		{
		  int cmp;
		  *name_end = 0;
		  cmp = substring (dirname, filename);
		  *name_end = tmp;
# ifndef STAGE1_5
		  if (do_possibilities)
		    {
		      if (cmp <= 0)
			{
			  if (print_possibilities > 0)
			    print_possibilities = -print_possibilities;
			  *name_end = 0;
			  print_a_completion (filename);
			  *name_end = tmp;
			}
		    }
		  else
# endif 
		    if (cmp == 0)
		      goto found;
		}
	      name_end = filename;
	      de_head++;
	      num_entries--;
	    }
	}
      
# ifndef STAGE1_5
      if (print_possibilities < 0)
	return 1;
# endif 
      
      errnum = ERR_FILE_NOT_FOUND;
      *rest = ch;
      return 0;
      
    found:
      
      *rest = ch;
      dirname = rest;

      parent_dir_id = dir_id;
      parent_objectid = objectid;
      dir_id = de_head->deh_dir_id;
      objectid = de_head->deh_objectid;
    }
}

int
reiserfs_embed (int *start_sector, int needed_sectors)
{
  struct reiserfs_super_block super;
  int num_sectors;
  
  if (! devread (REISERFS_DISK_OFFSET_IN_BYTES >> SECTOR_BITS, 0, 
		 sizeof (struct reiserfs_super_block), (char *) &super))
    return 0;
  
  *start_sector = 1; 
  if ((substring (REISERFS_SUPER_MAGIC_STRING, super.s_magic) <= 0
       || substring (REISER2FS_SUPER_MAGIC_STRING, super.s_magic) <= 0
       || substring (REISER3FS_SUPER_MAGIC_STRING, super.s_magic) <= 0)
      && (
	  super.s_journal_block * super.s_blocksize 
	  > REISERFS_DISK_OFFSET_IN_BYTES))
    num_sectors = (REISERFS_DISK_OFFSET_IN_BYTES >> SECTOR_BITS) - 1;
  else
    num_sectors = (REISERFS_OLD_DISK_OFFSET_IN_BYTES >> SECTOR_BITS) - 1;
  
  return (needed_sectors <= num_sectors);
}
#endif 
