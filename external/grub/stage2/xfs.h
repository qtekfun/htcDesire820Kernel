/* 
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000  Silicon Graphics, Inc.  All Rights Reserved.
 *  Copyright (C) 2001,2004  Free Software Foundation, Inc.
 * 
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it would be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 * 
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write the Free Software Foundation, Inc., 59
 *  Temple Place - Suite 330, Boston MA 02111-1307, USA.
 * 
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 * 
 *  http://www.sgi.com 
 * 
 *  For further information regarding this notice, see: 
 * 
 *  http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
 */

typedef signed char	xfs_int8_t;
typedef unsigned char	xfs_uint8_t;
typedef short		xfs_int16_t;
typedef unsigned short	xfs_uint16_t;
typedef int		xfs_int32_t;
typedef unsigned int	xfs_uint32_t;
typedef long long	xfs_int64_t;
typedef unsigned long long xfs_uint64_t;

typedef xfs_uint64_t	xfs_ino_t;
typedef	xfs_uint32_t	xfs_agino_t;
typedef xfs_int64_t	xfs_daddr_t;
typedef xfs_int64_t	xfs_off_t;
typedef xfs_uint8_t	uuid_t[16];



typedef xfs_uint32_t	xfs_agblock_t;	
typedef	xfs_uint32_t	xfs_extlen_t;	
typedef	xfs_uint32_t	xfs_agnumber_t;	
typedef xfs_int32_t	xfs_extnum_t;	
typedef xfs_int16_t	xfs_aextnum_t;	
typedef	xfs_int64_t	xfs_fsize_t;	

typedef	xfs_uint32_t	xfs_dablk_t;	
typedef	xfs_uint32_t	xfs_dahash_t;	

typedef xfs_uint64_t	xfs_dfsbno_t;	
typedef xfs_uint64_t	xfs_drfsbno_t;	
typedef	xfs_uint64_t	xfs_drtbno_t;	
typedef	xfs_uint64_t	xfs_dfiloff_t;	

typedef	xfs_uint64_t	xfs_fsblock_t;	
typedef	xfs_uint64_t	xfs_fileoff_t;	
typedef	xfs_uint64_t	xfs_filblks_t;	



#define	XFS_SB_MAGIC		0x58465342	
#define	XFS_SB_VERSION_4	4		
#define	XFS_SB_VERSION_NUMBITS	0x000f

typedef struct xfs_sb
{
	xfs_uint32_t	sb_magicnum;	
	xfs_uint32_t	sb_blocksize;	
	xfs_drfsbno_t	sb_dblocks;	
	xfs_drfsbno_t	sb_rblocks;	
	xfs_drtbno_t	sb_rextents;	
	uuid_t		sb_uuid;	
	xfs_dfsbno_t	sb_logstart;	
	xfs_ino_t	sb_rootino;	
	xfs_ino_t	sb_rbmino;	
	xfs_ino_t	sb_rsumino;	
	xfs_agblock_t	sb_rextsize;	
	xfs_agblock_t	sb_agblocks;	
	xfs_agnumber_t	sb_agcount;	
	xfs_extlen_t	sb_rbmblocks;	
	xfs_extlen_t	sb_logblocks;	
	xfs_uint16_t	sb_versionnum;	
	xfs_uint16_t	sb_sectsize;	
	xfs_uint16_t	sb_inodesize;	
	xfs_uint16_t	sb_inopblock;	
	char		sb_fname[12];	
	xfs_uint8_t	sb_blocklog;	
	xfs_uint8_t	sb_sectlog;	
	xfs_uint8_t	sb_inodelog;	
	xfs_uint8_t	sb_inopblog;	
	xfs_uint8_t	sb_agblklog;	
	xfs_uint8_t	sb_rextslog;	
	xfs_uint8_t	sb_inprogress;	
	xfs_uint8_t	sb_imax_pct;	
					
	xfs_uint64_t	sb_icount;	
	xfs_uint64_t	sb_ifree;	
	xfs_uint64_t	sb_fdblocks;	
	xfs_uint64_t	sb_frextents;	
	xfs_ino_t	sb_uquotino;	
	xfs_ino_t	sb_gquotino;	
	xfs_uint16_t	sb_qflags;	
	xfs_uint8_t	sb_flags;	
	xfs_uint8_t	sb_shared_vn;	
	xfs_extlen_t	sb_inoalignmt;	
	xfs_uint32_t	sb_unit;	
	xfs_uint32_t	sb_width;		
	xfs_uint8_t	sb_dirblklog;	
        xfs_uint8_t       sb_dummy[7];    
} xfs_sb_t;



typedef struct xfs_btree_lblock
{
	xfs_uint32_t	bb_magic;	
	xfs_uint16_t	bb_level;	
	xfs_uint16_t	bb_numrecs;	
	xfs_dfsbno_t	bb_leftsib;	
	xfs_dfsbno_t	bb_rightsib;	
} xfs_btree_lblock_t;

typedef struct xfs_btree_hdr
{
	xfs_uint32_t	bb_magic;	
	xfs_uint16_t	bb_level;	
	xfs_uint16_t	bb_numrecs;	
} xfs_btree_hdr_t;

typedef struct xfs_btree_block
{
	xfs_btree_hdr_t	bb_h;		
	union		{
		struct	{
			xfs_agblock_t	bb_leftsib;
			xfs_agblock_t	bb_rightsib;
		}	s;		
		struct	{
			xfs_dfsbno_t	bb_leftsib;
			xfs_dfsbno_t	bb_rightsib;
		}	l;		
	}		bb_u;		
} xfs_btree_block_t;


typedef struct xfs_bmdr_block
{
	xfs_uint16_t	bb_level;	
	xfs_uint16_t	bb_numrecs;	
} xfs_bmdr_block_t;


#define	BMBT_USE_64	1

typedef struct xfs_bmbt_rec_32
{
	xfs_uint32_t		l0, l1, l2, l3;
} xfs_bmbt_rec_32_t;
typedef struct xfs_bmbt_rec_64
{
	xfs_uint64_t		l0, l1;
} xfs_bmbt_rec_64_t;

#if BMBT_USE_64
typedef	xfs_uint64_t	xfs_bmbt_rec_base_t;	
typedef xfs_bmbt_rec_64_t xfs_bmbt_rec_t, xfs_bmdr_rec_t;
#else	
typedef	xfs_uint32_t	xfs_bmbt_rec_base_t;	
typedef xfs_bmbt_rec_32_t xfs_bmbt_rec_t, xfs_bmdr_rec_t;
#endif	

typedef struct xfs_bmbt_key
{
	xfs_dfiloff_t	br_startoff;	
} xfs_bmbt_key_t, xfs_bmdr_key_t;

typedef xfs_dfsbno_t xfs_bmbt_ptr_t, xfs_bmdr_ptr_t;	
					
typedef	struct xfs_btree_lblock xfs_bmbt_block_t;



typedef	xfs_uint16_t	xfs_dir2_data_off_t;

typedef	xfs_off_t		xfs_dir2_off_t;


#define	XFS_DIR2_LEAF1_MAGIC	0xd2f1	
#define	XFS_DIR2_LEAFN_MAGIC	0xd2ff	

typedef struct xfs_da_blkinfo {
	xfs_dablk_t forw;			
	xfs_dablk_t back;			
	xfs_uint16_t magic;			
	xfs_uint16_t pad;				
} xfs_da_blkinfo_t;


typedef struct xfs_da_intnode {
	struct xfs_da_node_hdr {	
		xfs_da_blkinfo_t info;	
		xfs_uint16_t count;	
		xfs_uint16_t level;	
	} hdr;
	struct xfs_da_node_entry {
		xfs_dahash_t hashval;	
		xfs_dablk_t before;	
	} btree[1];			
} xfs_da_intnode_t;



#define	XFS_DIR2_DATA_FREE_TAG	0xffff
#define	XFS_DIR2_DATA_FD_COUNT	3


typedef struct xfs_dir2_data_free {
	xfs_dir2_data_off_t	offset;		
	xfs_dir2_data_off_t	length;		
} xfs_dir2_data_free_t;

typedef struct xfs_dir2_data_hdr {
	xfs_uint32_t		magic;		
						
	xfs_dir2_data_free_t	bestfree[XFS_DIR2_DATA_FD_COUNT];
} xfs_dir2_data_hdr_t;

typedef struct xfs_dir2_data_entry {
	xfs_ino_t		inumber;	
	xfs_uint8_t		namelen;	
	xfs_uint8_t		name[1];	
						
	xfs_dir2_data_off_t	tag;		
} xfs_dir2_data_entry_t;

typedef struct xfs_dir2_data_unused {
	xfs_uint16_t		freetag;	
	xfs_dir2_data_off_t	length;		
						
	xfs_dir2_data_off_t	tag;		
} xfs_dir2_data_unused_t;

typedef union {
	xfs_dir2_data_entry_t	entry;
	xfs_dir2_data_unused_t	unused;
} xfs_dir2_data_union_t;



typedef struct xfs_dir2_leaf_hdr {
	xfs_da_blkinfo_t	info;		
	xfs_uint16_t		count;		
	xfs_uint16_t		stale;		
} xfs_dir2_leaf_hdr_t;




#define	XFS_DIR2_BLOCK_MAGIC	0x58443242	

typedef struct xfs_dir2_block_tail {
	xfs_uint32_t	count;			
	xfs_uint32_t	stale;			
} xfs_dir2_block_tail_t;




typedef	struct { xfs_uint8_t i[8]; } xfs_dir2_ino8_t;

typedef struct { xfs_uint8_t i[4]; } xfs_dir2_ino4_t;

typedef union {
	xfs_dir2_ino8_t	i8;
	xfs_dir2_ino4_t	i4;
} xfs_dir2_inou_t;

typedef struct { xfs_uint8_t i[2]; } xfs_dir2_sf_off_t;

typedef struct xfs_dir2_sf_hdr {
	xfs_uint8_t		count;		
	xfs_uint8_t		i8count;	
	xfs_dir2_inou_t		parent;		
} xfs_dir2_sf_hdr_t;

typedef struct xfs_dir2_sf_entry {
	xfs_uint8_t		namelen;	
	xfs_dir2_sf_off_t	offset;		
	xfs_uint8_t		name[1];	
	xfs_dir2_inou_t		inumber;	
} xfs_dir2_sf_entry_t;

typedef struct xfs_dir2_sf {
	xfs_dir2_sf_hdr_t	hdr;		
	xfs_dir2_sf_entry_t	list[1];	
} xfs_dir2_sf_t;


#define	XFS_DINODE_VERSION_1	1
#define	XFS_DINODE_VERSION_2	2
#define	XFS_DINODE_MAGIC	0x494e	

typedef struct xfs_timestamp {
	xfs_int32_t	t_sec;		
	xfs_int32_t	t_nsec;		
} xfs_timestamp_t;

typedef struct xfs_dinode_core
{
	xfs_uint16_t	di_magic;	
	xfs_uint16_t	di_mode;	
	xfs_int8_t	di_version;	
	xfs_int8_t	di_format;	
	xfs_uint16_t	di_onlink;	
	xfs_uint32_t	di_uid;		
	xfs_uint32_t	di_gid;		
	xfs_uint32_t	di_nlink;	
	xfs_uint16_t	di_projid;	
	xfs_uint8_t	di_pad[10];	
	xfs_timestamp_t	di_atime;	
	xfs_timestamp_t	di_mtime;	
	xfs_timestamp_t	di_ctime;	
	xfs_fsize_t	di_size;	
	xfs_drfsbno_t	di_nblocks;	
	xfs_extlen_t	di_extsize;	
	xfs_extnum_t	di_nextents;	
	xfs_aextnum_t	di_anextents;	
	xfs_uint8_t	di_forkoff;	
	xfs_int8_t	di_aformat;	
	xfs_uint32_t	di_dmevmask;	
	xfs_uint16_t	di_dmstate;	
	xfs_uint16_t	di_flags;	
	xfs_uint32_t	di_gen;		
} xfs_dinode_core_t;

typedef struct xfs_dinode
{
	xfs_dinode_core_t	di_core;
	xfs_agino_t		di_next_unlinked;
	union {
		xfs_bmdr_block_t di_bmbt;	
		xfs_bmbt_rec_32_t di_bmx[1];	
		xfs_dir2_sf_t	di_dir2sf;	
		char		di_c[1];	
	} di_u;
} xfs_dinode_t;

typedef enum xfs_dinode_fmt
{
	XFS_DINODE_FMT_DEV,		
	XFS_DINODE_FMT_LOCAL,		
					
	XFS_DINODE_FMT_EXTENTS,		
	XFS_DINODE_FMT_BTREE,		
	XFS_DINODE_FMT_UUID 		
} xfs_dinode_fmt_t;

#define	IFMT		0170000		
#define	IFDIR		0040000		
#define	IFREG		0100000		
#define	IFLNK		0120000		
