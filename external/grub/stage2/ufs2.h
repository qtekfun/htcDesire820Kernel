/*
 * Copyright (C) 2004 Free Software Foundation, Inc.
 * Copyright (c) 2002 Networks Associates Technology, Inc.
 * All rights reserved.
 *
 * This software was developed for the FreeBSD Project by Marshall
 * Kirk McKusick and Network Associates Laboratories, the Security
 * Research Division of Network Associates, Inc. under DARPA/SPAWAR
 * contract N66001-01-C-8035 ("CBOSS"), as part of the DARPA CHATS
 * research program
 *
 * Copyright (c) 1982, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the authors may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)dinode.h	8.3 (Berkeley) 1/21/94
 * $FreeBSD: src/sys/ufs/ufs/dinode.h,v 1.11 2002/07/16 22:36:00 mckusick Exp $
 */

#ifndef _GRUB_UFS2_H_
#define _GRUB_UFS2_H_

typedef signed char            grub_int8_t;
typedef signed short           grub_int16_t;
typedef signed int             grub_int32_t;
typedef signed long long int   grub_int64_t;
typedef unsigned char          grub_uint8_t;
typedef unsigned short         grub_uint16_t;
typedef unsigned int           grub_uint32_t;
typedef unsigned long long int grub_uint64_t;

typedef grub_uint8_t                grub_u_char;
typedef grub_uint32_t               grub_u_int;

typedef grub_uint8_t                grub_u_int8_t;
typedef grub_uint16_t               grub_u_int16_t;
typedef grub_uint32_t               grub_u_int32_t;
typedef grub_uint64_t               grub_u_int64_t;

#define i_size di_size


#define DEV_BSIZE 512

#define	ROOTINO	((grub_ino_t)2)

typedef grub_int32_t ufs1_daddr_t;
typedef	grub_int64_t	ufs2_daddr_t;
typedef grub_int64_t ufs_lbn_t;
typedef grub_int64_t ufs_time_t;

typedef grub_uint32_t      grub_ino_t;

#define	IEXEC		0000100		
#define	IWRITE		0000200		
#define	IREAD		0000400		
#define	ISVTX		0001000		
#define	ISGID		0002000		
#define	ISUID		0004000		

#define	IFMT		0170000		
#define	IFIFO		0010000		
#define	IFCHR		0020000		
#define	IFDIR		0040000		
#define	IFBLK		0060000		
#define	IFREG		0100000		
#define	IFLNK		0120000		
#define	IFSOCK		0140000		
#define	IFWHT		0160000		


#define	NXADDR	2			
#define	NDADDR	12			
#define	NIADDR	3			

struct ufs1_dinode {
	grub_u_int16_t       di_mode;        
	grub_int16_t         di_nlink;       
	union {
		grub_u_int16_t oldids[2];    
	} di_u;
	grub_u_int64_t       di_size;        
	grub_int32_t         di_atime;       
	grub_int32_t         di_atimensec;   
	grub_int32_t         di_mtime;       
	grub_int32_t         di_mtimensec;   
	grub_int32_t         di_ctime;       
	grub_int32_t         di_ctimensec;   
	ufs1_daddr_t    di_db[NDADDR];  
	ufs1_daddr_t    di_ib[NIADDR];  
	grub_u_int32_t       di_flags;       
	grub_int32_t         di_blocks;      
	grub_int32_t         di_gen;         
	grub_u_int32_t       di_uid;         
	grub_u_int32_t       di_gid;         
	grub_int32_t         di_spare[2];    
};

struct ufs2_dinode {
	grub_u_int16_t	di_mode;	
	grub_int16_t		di_nlink;	
	grub_u_int32_t	di_uid;		
	grub_u_int32_t	di_gid;		
	grub_u_int32_t	di_blksize;	
	grub_u_int64_t	di_size;	
	grub_u_int64_t	di_blocks;	
	ufs_time_t	di_atime;	
	ufs_time_t	di_mtime;	
	ufs_time_t	di_ctime;	
	ufs_time_t	di_birthtime;	
	grub_int32_t		di_mtimensec;	
	grub_int32_t		di_atimensec;	
	grub_int32_t		di_ctimensec;	
	grub_int32_t		di_birthnsec;	
	grub_int32_t		di_gen;		
	grub_u_int32_t	di_kernflags;	
	grub_u_int32_t	di_flags;	
	grub_int32_t		di_extsize;	
	ufs2_daddr_t	di_extb[NXADDR];
	ufs2_daddr_t	di_db[NDADDR];	
	ufs2_daddr_t	di_ib[NIADDR];	
	grub_int64_t		di_spare[3];	
};

#define	MAXNAMLEN	255

struct	direct {
	grub_u_int32_t d_ino;		
	grub_u_int16_t d_reclen;		
	grub_u_int8_t  d_type; 		
	grub_u_int8_t  d_namlen;		
	char	  d_name[MAXNAMLEN + 1];
};

#define DT_UNKNOWN       0
#define DT_FIFO          1
#define DT_CHR           2
#define DT_DIR           4
#define DT_BLK           6
#define DT_REG           8
#define DT_LNK          10
#define DT_SOCK         12
#define DT_WHT          14

#define SBLOCK_FLOPPY        0
#define SBLOCK_UFS1       8192
#define SBLOCK_UFS2      65536
#define SBLOCK_PIGGY    262144
#define SBLOCKSIZE        8192
#define SBLOCKSEARCH \
	{ SBLOCK_UFS2, SBLOCK_UFS1, SBLOCK_FLOPPY, SBLOCK_PIGGY, -1 }

#define MAXMNTLEN	512

#define	NOCSPTRS	((128 / sizeof(void *)) - 4)

#define FSMAXSNAP 20
	
struct csum {
	grub_int32_t	cs_ndir;		
	grub_int32_t	cs_nbfree;		
	grub_int32_t	cs_nifree;		
	grub_int32_t	cs_nffree;		
};

struct csum_total {
	grub_int64_t	cs_ndir;		
	grub_int64_t	cs_nbfree;		
	grub_int64_t	cs_nifree;		
	grub_int64_t	cs_nffree;		
	grub_int64_t	cs_numclusters;		
	grub_int64_t	cs_spare[3];		
};

struct fs {
	grub_int32_t	 fs_firstfield;		
	grub_int32_t	 fs_unused_1;		
	grub_int32_t	 fs_sblkno;		
	grub_int32_t	 fs_cblkno;		
	grub_int32_t	 fs_iblkno;		
	grub_int32_t	 fs_dblkno;		
	grub_int32_t	 fs_old_cgoffset;	
	grub_int32_t	 fs_old_cgmask;		
	grub_int32_t  fs_old_time;		/* last time written */
	grub_int32_t	 fs_old_size;		
	grub_int32_t	 fs_old_dsize;		
	grub_int32_t	 fs_ncg;		
	grub_int32_t	 fs_bsize;		
	grub_int32_t	 fs_fsize;		
	grub_int32_t	 fs_frag;		
	grub_int32_t	 fs_minfree;		
	grub_int32_t	 fs_old_rotdelay;	
	grub_int32_t	 fs_old_rps;		
	grub_int32_t	 fs_bmask;		
	grub_int32_t	 fs_fmask;		
	grub_int32_t	 fs_bshift;		
	grub_int32_t	 fs_fshift;		
	grub_int32_t	 fs_maxcontig;		
	grub_int32_t	 fs_maxbpg;		
	grub_int32_t	 fs_fragshift;		
	grub_int32_t	 fs_fsbtodb;		
	grub_int32_t	 fs_sbsize;		
	grub_int32_t	 fs_spare1[2];		
					
	grub_int32_t	 fs_nindir;		
	grub_int32_t	 fs_inopb;		
	grub_int32_t	 fs_old_nspf;		
	grub_int32_t	 fs_optim;		
	grub_int32_t	 fs_old_npsect;		
	grub_int32_t	 fs_old_interleave;	
	grub_int32_t	 fs_old_trackskew;	
	grub_int32_t	 fs_id[2];		
	grub_int32_t	 fs_old_csaddr;		
	grub_int32_t	 fs_cssize;		
	grub_int32_t	 fs_cgsize;		
	grub_int32_t	 fs_spare2;		
	grub_int32_t	 fs_old_nsect;		
	grub_int32_t  fs_old_spc;		
	grub_int32_t	 fs_old_ncyl;		
	grub_int32_t	 fs_old_cpg;		
	grub_int32_t	 fs_ipg;		
	grub_int32_t	 fs_fpg;		
	struct	csum fs_old_cstotal;	
	grub_int8_t   fs_fmod;		
	grub_int8_t   fs_clean;		
	grub_int8_t 	 fs_ronly;		
	grub_int8_t   fs_old_flags;		
	grub_u_char	 fs_fsmnt[MAXMNTLEN];	
	grub_int32_t	 fs_cgrotor;		
	void 	*fs_ocsp[NOCSPTRS];	
	grub_u_int8_t *fs_contigdirs;	
	struct	csum *fs_csp;		
	grub_int32_t	*fs_maxcluster;		
	grub_u_int	*fs_active;		
	grub_int32_t	 fs_old_cpc;		
	grub_int32_t	 fs_maxbsize;		
	grub_int64_t	 fs_sparecon64[17];	
	grub_int64_t	 fs_sblockloc;		
	struct	csum_total fs_cstotal;	
	ufs_time_t fs_time;		/* last time written */
	grub_int64_t	 fs_size;		
	grub_int64_t	 fs_dsize;		
	ufs2_daddr_t fs_csaddr;		
	grub_int64_t	 fs_pendingblocks;	
	grub_int32_t	 fs_pendinginodes;	
	grub_int32_t	 fs_snapinum[FSMAXSNAP];
	grub_int32_t	 fs_avgfilesize;	
	grub_int32_t	 fs_avgfpdir;		
	grub_int32_t	 fs_save_cgsize;	
	grub_int32_t	 fs_sparecon32[26];	
	grub_int32_t  fs_flags;		
	grub_int32_t	 fs_contigsumsize;	 
	grub_int32_t	 fs_maxsymlinklen;	
	grub_int32_t	 fs_old_inodefmt;	
	grub_u_int64_t fs_maxfilesize;	
	grub_int64_t	 fs_qbmask;		
	grub_int64_t	 fs_qfmask;		
	grub_int32_t	 fs_state;		
	grub_int32_t	 fs_old_postblformat;	
	grub_int32_t	 fs_old_nrpos;		
	grub_int32_t	 fs_spare5[2];		
					
	grub_int32_t	 fs_magic;		
};

#define FS_UFS1_MAGIC   0x011954        
#define	FS_UFS2_MAGIC	0x19540119	

#define fsbtodb(fs, b)	((b) << (fs)->fs_fsbtodb)
#define	dbtofsb(fs, b)	((b) >> (fs)->fs_fsbtodb)

#define	cgbase(fs, c)	((ufs2_daddr_t)((fs)->fs_fpg * (c)))
#define	cgimin(fs, c)	(cgstart(fs, c) + (fs)->fs_iblkno)	
#define cgstart(fs, c)							\
       ((fs)->fs_magic == FS_UFS2_MAGIC ? cgbase(fs, c) :		\
       (cgbase(fs, c) + (fs)->fs_old_cgoffset * ((c) & ~((fs)->fs_old_cgmask))))

#define	ino_to_cg(fs, x)	((x) / (fs)->fs_ipg)
#define	ino_to_fsba(fs, x)						\
	((ufs2_daddr_t)(cgimin(fs, ino_to_cg(fs, x)) +			\
	    (blkstofrags((fs), (((x) % (fs)->fs_ipg) / INOPB(fs))))))
#define	ino_to_fsbo(fs, x)	((x) % INOPB(fs))

#define blkoff(fs, loc)		 \
	((loc) & (fs)->fs_qbmask)

#define smalllblktosize(fs, blk)     \
	((blk) << (fs)->fs_bshift)


#define lblkno(fs, loc)		 \
	((loc) >> (fs)->fs_bshift)

#define fragroundup(fs, size)	 \
	(((size) + (fs)->fs_qfmask) & (fs)->fs_fmask)

#define fragstoblks(fs, frags)	 \
	((frags) >> (fs)->fs_fragshift)
#define blkstofrags(fs, blks)	 \
	((blks) << (fs)->fs_fragshift)
#define fragnum(fs, fsb)	 \
	((fsb) & ((fs)->fs_frag - 1))
#define blknum(fs, fsb)		 \
	((fsb) &~ ((fs)->fs_frag - 1))

#define blksize(fs, ip, lbn) \
	(((lbn) >= NDADDR || (ip)->i_size >= smalllblktosize(fs, (lbn) + 1)) \
	    ? (fs)->fs_bsize \
	    : (fragroundup(fs, blkoff(fs, (ip)->i_size))))
#define sblksize(fs, size, lbn) \
	(((lbn) >= NDADDR || (size) >= ((lbn) + 1) << (fs)->fs_bshift) \
	  ? (fs)->fs_bsize \
	  : (fragroundup(fs, blkoff(fs, (size)))))


#define	INOPB(fs)	((fs)->fs_inopb)
#define	INOPF(fs)	((fs)->fs_inopb >> (fs)->fs_fragshift)

#define	NINDIR(fs)	((fs)->fs_nindir)

#define FS_UNCLEAN    0x01      
#define FS_DOSOFTDEP  0x02      
#define FS_NEEDSFSCK  0x04      
#define FS_INDEXDIRS  0x08      
#define FS_ACLS       0x10      
#define FS_MULTILABEL 0x20      
#define FS_FLAGS_UPDATED 0x80   

#endif 
