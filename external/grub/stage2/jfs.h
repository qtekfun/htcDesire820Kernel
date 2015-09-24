/*   
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000  International Business Machines  Corp.
 *  Copyright (C) 2001  Free Software Foundation, Inc.
 *
 *  This program is free software;  you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *  the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;  if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _JFS_H_
#define _JFS_H_


#define JFS_AIX		0x80000000	

#define JFS_OS2		0x40000000	

#define JFS_LINUX      	0x10000000	

#define JFS_UNICODE	0x00000001	

#define	JFS_SWAP_BYTES		0x00100000	


#ifdef PSIZE
#undef PSIZE
#endif
#define	PSIZE		4096	

#define	PBSIZE		512	
#define DISIZE		512	
#define L2DISIZE	9
#define	INOSPERIAG	4096	
#define	L2INOSPERIAG	12
#define INOSPEREXT	32	
#define L2INOSPEREXT	5

#define MINJFS			(0x1000000)

#define SUPER1_OFF	0x8000	

#define AITBL_OFF	(SUPER1_OFF + PSIZE + (PSIZE << 1))

#define	AGGREGATE_I	1	
#define	FILESYSTEM_I	16	

#define	ROOT_I		2	

#define JFS_NAME_MAX	255
#define JFS_PATH_MAX	PSIZE

typedef unsigned char u8;
typedef char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;

typedef u16 UniChar;


#define BT_TYPE		0x07	
#define	BT_ROOT		0x01	
#define	BT_LEAF		0x02	
#define	BT_INTERNAL	0x04	
#define	BT_RIGHTMOST	0x10	
#define	BT_LEFTMOST	0x20	


struct timestruc_t {
	u32 tv_sec;
	u32 tv_nsec;
};

typedef struct {
	unsigned len:24;
	unsigned addr1:8;
	u32 addr2;
} pxd_t;

#define	lengthPXD(pxd)	((pxd)->len)
#define	addressPXD(pxd)	(((s64)((pxd)->addr1)) << 32 | ((pxd)->addr2))

typedef struct {
	unsigned flag:8;	
	unsigned rsrvd:24;	
	u32 size;		
	unsigned len:24;	
	unsigned addr1:8;	
	u32 addr2;		
} dxd_t;			

typedef struct dasd {
	u8 thresh;		
	u8 delta;		
	u8 rsrvd1;
	u8 limit_hi;		
	u32 limit_lo;		
	u8 rsrvd2[3];
	u8 used_hi;		
	u32 used_lo;		
} dasd_t;



#define JFS_MAGIC 	0x3153464A	

struct jfs_superblock
{
	u32 s_magic;		
	u32 s_version;		

	s64 s_size;		
	s32 s_bsize;		
	s16 s_l2bsize;		
	s16 s_l2bfactor;	
	s32 s_pbsize;		
	s16 s_l2pbsize;		
	s16 pad;		

	u32 s_agsize;		

	u32 s_flag;		
	u32 s_state;		
	s32 s_compress;		

	pxd_t s_ait2;		

	pxd_t s_aim2;		
	u32 s_logdev;		
	s32 s_logserial;	
	pxd_t s_logpxd;		

	pxd_t s_fsckpxd;	

	struct timestruc_t s_time;	

	s32 s_fsckloglen;	
	s8 s_fscklog;		
	char s_fpack[11];	

	
	s64 s_xsize;		
	pxd_t s_xfsckpxd;	
	pxd_t s_xlogpxd;	
	

	char s_attach;		
	u8 rsrvd4[7];		

	u64 totalUsable;	
	u64 minFree;		

	u64 realFree;		
};


typedef struct {
	s8 next;		
	s8 cnt;			
	UniChar name[15];	
} dtslot_t;			

#define DTSLOTDATALEN	15

typedef struct {
	pxd_t xd;		

	s8 next;		
	u8 namlen;		
	UniChar name[11];	
} idtentry_t;			

typedef struct {
	u32 inumber;		
	s8 next;		
	u8 namlen;		
	UniChar name[11];	
	u32 index;		
} ldtentry_t;			

#define DTLHDRDATALEN	11

 


typedef struct dir_table_slot {
	u8 rsrvd;	
	u8 flag;	
	u8 slot;	
	u8 addr1;	
	u32 addr2;	
} dir_table_slot_t;	

typedef union {
	struct {
		dasd_t DASD;	

		u8 flag;	
		s8 nextindex;	
		s8 freecnt;	
		s8 freelist;	

		u32 idotdot;	

		s8 stbl[8];	
	} header;		

	dtslot_t slot[9];
} dtroot_t;

typedef union {
	struct {
		s64 next;	
		s64 prev;	

		u8 flag;	
		s8 nextindex;	
		s8 freecnt;	
		s8 freelist;	

		u8 maxslot;	
		s8 stblindex;	
		u8 rsrvd[2];	

		pxd_t self;	
	} header;		

	dtslot_t slot[128];
} dtpage_t;


typedef struct xad {
	unsigned flag:8;	
	unsigned rsvrd:16;	
	unsigned off1:8;	
	u32 off2;		
	unsigned len:24;	
	unsigned addr1:8;	
	u32 addr2;		
} xad_t;			

#define offsetXAD(xad)	(((s64)((xad)->off1)) << 32 | ((xad)->off2))
#define addressXAD(xad)	(((s64)((xad)->addr1)) << 32 | ((xad)->addr2))
#define lengthXAD(xad)	((xad)->len)

#define XTPAGEMAXSLOT   256
#define XTENTRYSTART    2

typedef union {
	struct xtheader {
		s64 next;	
		s64 prev;	

		u8 flag;	
		u8 rsrvd1;	
		s16 nextindex;	
		s16 maxentry;	
		s16 rsrvd2;	

		pxd_t self;	
	} header;		

	xad_t xad[XTPAGEMAXSLOT];	
} xtpage_t;


struct dinode {
	u32 di_inostamp;	
	s32 di_fileset;		
	u32 di_number;		
	u32 di_gen;		

	pxd_t di_ixpxd;		

	s64 di_size;		
	s64 di_nblocks;		

	u32 di_nlink;		

	u32 di_uid;		
	u32 di_gid;		

	u32 di_mode;		

	struct timestruc_t di_atime;	
	struct timestruc_t di_ctime;	
	struct timestruc_t di_mtime;	
	struct timestruc_t di_otime;	

	dxd_t di_acl;		

	dxd_t di_ea;		

	s32 di_next_index;  

	s32 di_acltype;		

	union {
		struct {
			dir_table_slot_t _table[12];	

			dtroot_t _dtroot;		
		} _dir;					
#define di_dirtable	u._dir._table
#define di_dtroot	u._dir._dtroot
#define di_parent       di_dtroot.header.idotdot
#define di_DASD		di_dtroot.header.DASD

		struct {
			union {
				u8 _data[96];		
				struct {
					void *_imap;	
					u32 _gengen;	
				} _imap;
			} _u1;				
#define di_gengen	u._file._u1._imap._gengen

			union {
				xtpage_t _xtroot;
				struct {
					u8 unused[16];	
					dxd_t _dxd;	
					union {
						u32 _rdev;	
						u8 _fastsymlink[128];
					} _u;
					u8 _inlineea[128];
				} _special;
			} _u2;
		} _file;
#define di_xtroot	u._file._u2._xtroot
#define di_dxd		u._file._u2._special._dxd
#define di_btroot	di_xtroot
#define di_inlinedata	u._file._u2._special._u
#define di_rdev		u._file._u2._special._u._rdev
#define di_fastsymlink	u._file._u2._special._u._fastsymlink
#define di_inlineea     u._file._u2._special._inlineea
	} u;
};

typedef struct dinode dinode_t;

#define IFMT	0xF000		
#define IFDIR	0x4000		
#define IFREG	0x8000		
#define IFLNK	0xA000		

#define INLINEEA        0x00040000	


#define	EXTSPERIAG	128	
#define SMAPSZ		4	
#define	MAXAG		128	

typedef struct {
	s64 agstart;		
	s32 iagnum;		
	s32 inofreefwd;		
	s32 inofreeback;	
	s32 extfreefwd;		
	s32 extfreeback;	
	s32 iagfree;		

	
	s32 inosmap[SMAPSZ];	
	s32 extsmap[SMAPSZ];	
	s32 nfreeinos;		
	s32 nfreeexts;		
	
	u8 pad[1976];		
	
	u32 wmap[EXTSPERIAG];	
	u32 pmap[EXTSPERIAG];	
	pxd_t inoext[EXTSPERIAG];	
} iag_t;			

#endif 
