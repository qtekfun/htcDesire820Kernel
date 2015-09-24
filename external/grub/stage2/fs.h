/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)fs.h	7.7 (Berkeley) 5/9/89
 */

#define BBSIZE		8192
#define SBSIZE		8192
#define	BBOFF		((mach_off_t)(0))
#define	SBOFF		((mach_off_t)(BBOFF + BBSIZE))
#define	BBLOCK		((mach_daddr_t)(0))
#define	SBLOCK		((mach_daddr_t)(BBLOCK + BBSIZE / DEV_BSIZE))

#define	ROOTINO		((mach_ino_t)2)	

#define MINBSIZE	4096

#define MAXMNTLEN 512
#define MAXCSBUFS 32

struct csum
  {
    int cs_ndir;		
    int cs_nbfree;		
    int cs_nifree;		
    int cs_nffree;		
  };

#define	FS_MAGIC	0x011954
struct fs
  {
    int xxx1;			
    int xxx2;			
    mach_daddr_t fs_sblkno;	
    mach_daddr_t fs_cblkno;	
    mach_daddr_t fs_iblkno;	
    mach_daddr_t fs_dblkno;	
    int fs_cgoffset;		
    int fs_cgmask;		
    mach_time_t fs_time;	/* last time written */
    int fs_size;		
    int fs_dsize;		
    int fs_ncg;			
    int fs_bsize;		
    int fs_fsize;		
    int fs_frag;		
    int fs_minfree;		
    int fs_rotdelay;		
    int fs_rps;			
    int fs_bmask;		
    int fs_fmask;		
    int fs_bshift;		
    int fs_fshift;		
    int fs_maxcontig;		
    int fs_maxbpg;		
    int fs_fragshift;		
    int fs_fsbtodb;		
    int fs_sbsize;		
    int fs_csmask;		
    int fs_csshift;		
    int fs_nindir;		
    int fs_inopb;		
    int fs_nspf;		
    int fs_optim;		
    int fs_npsect;		
    int fs_interleave;		
    int fs_trackskew;		
    int fs_headswitch;		
    int fs_trkseek;		
    mach_daddr_t fs_csaddr;	
    int fs_cssize;		
    int fs_cgsize;		
    int fs_ntrak;		
    int fs_nsect;		
    int fs_spc;			
    int fs_ncyl;		
    int fs_cpg;			
    int fs_ipg;			
    int fs_fpg;			
    struct csum fs_cstotal;	
    char fs_fmod;		
    char fs_clean;		
    char fs_ronly;		
    char fs_flags;		
    char fs_fsmnt[MAXMNTLEN];	
    int fs_cgrotor;		
#if 1
    int was_fs_csp[MAXCSBUFS];
#else
    struct csum *fs_csp[MAXCSBUFS];	
#endif
    int fs_cpc;			
    short fs_opostbl[16][8];	
    long fs_sparecon[50];	
    long fs_contigsumsize;	
    long fs_maxsymlinklen;	
    long fs_inodefmt;		
    quad fs_maxfilesize;	
    quad fs_qbmask;		
    quad fs_qfmask;		
    long fs_state;		
    int fs_postblformat;	
    int fs_nrpos;		
    int fs_postbloff;		
    int fs_rotbloff;		
    int fs_magic;		
    u_char fs_space[1];		
  };
#define FS_OPTTIME	0	
#define FS_OPTSPACE	1	

#define FS_42POSTBLFMT		-1	
#define FS_DYNAMICPOSTBLFMT	1	
#define fs_postbl(fs, cylno) \
    (((fs)->fs_postblformat == FS_42POSTBLFMT) \
    ? ((fs)->fs_opostbl[cylno]) \
    : ((short *)((char *)(fs) + (fs)->fs_postbloff) + (cylno) * (fs)->fs_nrpos))
#define fs_rotbl(fs) \
    (((fs)->fs_postblformat == FS_42POSTBLFMT) \
    ? ((fs)->fs_space) \
    : ((u_char *)((char *)(fs) + (fs)->fs_rotbloff)))

#define fs_cs(fs, indx) \
	fs_csp[(indx) >> (fs)->fs_csshift][(indx) & ~(fs)->fs_csmask]

#define	CG_MAGIC	0x090255
struct cg
  {
    int xxx1;			
    int cg_magic;		
    mach_time_t cg_time;		/* time last written */
    int cg_cgx;			
    short cg_ncyl;		
    short cg_niblk;		
    int cg_ndblk;		
    struct csum cg_cs;		
    int cg_rotor;		
    int cg_frotor;		
    int cg_irotor;		
    int cg_frsum[MAXFRAG];	
    int cg_btotoff;		
    int cg_boff;		
    int cg_iusedoff;		
    int cg_freeoff;		
    int cg_nextfreeoff;		
    int cg_sparecon[16];	
    u_char cg_space[1];		
  };
#define cg_blktot(cgp) \
    (((cgp)->cg_magic != CG_MAGIC) \
    ? (((struct ocg *)(cgp))->cg_btot) \
    : ((int *)((char *)(cgp) + (cgp)->cg_btotoff)))
#define cg_blks(fs, cgp, cylno) \
    (((cgp)->cg_magic != CG_MAGIC) \
    ? (((struct ocg *)(cgp))->cg_b[cylno]) \
    : ((short *)((char *)(cgp) + (cgp)->cg_boff) + (cylno) * (fs)->fs_nrpos))
#define cg_inosused(cgp) \
    (((cgp)->cg_magic != CG_MAGIC) \
    ? (((struct ocg *)(cgp))->cg_iused) \
    : ((char *)((char *)(cgp) + (cgp)->cg_iusedoff)))
#define cg_blksfree(cgp) \
    (((cgp)->cg_magic != CG_MAGIC) \
    ? (((struct ocg *)(cgp))->cg_free) \
    : ((u_char *)((char *)(cgp) + (cgp)->cg_freeoff)))
#define cg_chkmagic(cgp) \
    ((cgp)->cg_magic == CG_MAGIC || ((struct ocg *)(cgp))->cg_magic == CG_MAGIC)

struct ocg
  {
    int xxx1;			
    int xxx2;			
    mach_time_t cg_time;	/* time last written */
    int cg_cgx;			
    short cg_ncyl;		
    short cg_niblk;		
    int cg_ndblk;		
    struct csum cg_cs;		
    int cg_rotor;		
    int cg_frotor;		
    int cg_irotor;		
    int cg_frsum[8];		
    int cg_btot[32];		
    short cg_b[32][8];		
    char cg_iused[256];		
    int cg_magic;		
    u_char cg_free[1];		
  };

#define fsbtodb(fs, b)	((b) << (fs)->fs_fsbtodb)
#define	dbtofsb(fs, b)	((b) >> (fs)->fs_fsbtodb)

#define	cgbase(fs, c)	((mach_daddr_t)((fs)->fs_fpg * (c)))
#define cgstart(fs, c) \
	(cgbase(fs, c) + (fs)->fs_cgoffset * ((c) & ~((fs)->fs_cgmask)))
#define	cgsblock(fs, c)	(cgstart(fs, c) + (fs)->fs_sblkno)	
#define	cgtod(fs, c)	(cgstart(fs, c) + (fs)->fs_cblkno)	
#define	cgimin(fs, c)	(cgstart(fs, c) + (fs)->fs_iblkno)	
#define	cgdmin(fs, c)	(cgstart(fs, c) + (fs)->fs_dblkno)	

#define	itoo(fs, x)	((x) % INOPB(fs))
#define	itog(fs, x)	((x) / (fs)->fs_ipg)
#define	itod(fs, x) \
	((mach_daddr_t)(cgimin(fs, itog(fs, x)) + \
	(blkstofrags((fs), (((x) % (fs)->fs_ipg) / INOPB(fs))))))

#define	dtog(fs, d)	((d) / (fs)->fs_fpg)
#define	dtogd(fs, d)	((d) % (fs)->fs_fpg)

#define blkmap(fs, map, loc) \
    (((map)[(loc) / NBBY] >> ((loc) % NBBY)) & (0xff >> (NBBY - (fs)->fs_frag)))
#define cbtocylno(fs, bno) \
    ((bno) * NSPF(fs) / (fs)->fs_spc)
#define cbtorpos(fs, bno) \
    (((bno) * NSPF(fs) % (fs)->fs_spc / (fs)->fs_nsect * (fs)->fs_trackskew + \
     (bno) * NSPF(fs) % (fs)->fs_spc % (fs)->fs_nsect * (fs)->fs_interleave) % \
     (fs)->fs_nsect * (fs)->fs_nrpos / (fs)->fs_npsect)

#define blkoff(fs, loc)		 \
	((loc) & ~(fs)->fs_bmask)
#define fragoff(fs, loc)	 \
	((loc) & ~(fs)->fs_fmask)
#define lblkno(fs, loc)		 \
	((loc) >> (fs)->fs_bshift)
#define numfrags(fs, loc)	 \
	((loc) >> (fs)->fs_fshift)
#define blkroundup(fs, size)	 \
	(((size) + (fs)->fs_bsize - 1) & (fs)->fs_bmask)
#define fragroundup(fs, size)	 \
	(((size) + (fs)->fs_fsize - 1) & (fs)->fs_fmask)
#define fragstoblks(fs, frags)	 \
	((frags) >> (fs)->fs_fragshift)
#define blkstofrags(fs, blks)	 \
	((blks) << (fs)->fs_fragshift)
#define fragnum(fs, fsb)	 \
	((fsb) & ((fs)->fs_frag - 1))
#define blknum(fs, fsb)		 \
	((fsb) &~ ((fs)->fs_frag - 1))

#define freespace(fs, percentreserved) \
	(blkstofrags((fs), (fs)->fs_cstotal.cs_nbfree) + \
	(fs)->fs_cstotal.cs_nffree - ((fs)->fs_dsize * (percentreserved) / 100))

#define blksize(fs, ip, lbn) \
	(((lbn) >= NDADDR || (ip)->i_size >= ((lbn) + 1) << (fs)->fs_bshift) \
	    ? (fs)->fs_bsize \
	    : (fragroundup(fs, blkoff(fs, (ip)->i_size))))
#define dblksize(fs, dip, lbn) \
	(((lbn) >= NDADDR || (dip)->di_size >= ((lbn) + 1) << (fs)->fs_bshift) \
	    ? (fs)->fs_bsize \
	    : (fragroundup(fs, blkoff(fs, (dip)->di_size))))

#define	NSPB(fs)	((fs)->fs_nspf << (fs)->fs_fragshift)
#define	NSPF(fs)	((fs)->fs_nspf)

#define	INOPB(fs)	((fs)->fs_inopb)
#define	INOPF(fs)	((fs)->fs_inopb >> (fs)->fs_fragshift)

#define	NINDIR(fs)	((fs)->fs_nindir)
