
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
 * Copyright (c) 1982, 1986, 1989 The Regents of the University of California.
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
 *	@(#)dir.h	7.6 (Berkeley) 5/9/89
 */

#ifndef _BOOT_UFS_DIR_H_
#define	_BOOT_UFS_DIR_H_

#define DIRBLKSIZ	DEV_BSIZE
#define	MAXNAMLEN	255

struct direct
  {
    u_int d_ino;		
    u_short d_reclen;		
    u_short d_namlen;		
    char d_name[MAXNAMLEN + 1];	
  };

#undef DIRSIZ
#define DIRSIZ(dp) \
    ((sizeof (struct direct) - (MAXNAMLEN+1)) + (((dp)->d_namlen+1 + 3) &~ 3))

#ifdef KERNEL
struct dirtemplate
  {
    u_int dot_ino;
    short dot_reclen;
    short dot_namlen;
    char dot_name[4];		
    u_int dotdot_ino;
    short dotdot_reclen;
    short dotdot_namlen;
    char dotdot_name[4];	
  };
#endif

#ifndef KERNEL
#define d_fileno d_ino		
#ifndef DEV_BSIZE
#define	DEV_BSIZE	512
#endif
typedef struct _dirdesc
  {
    int dd_fd;
    int dd_loc;
    int dd_size;
    char dd_buf[DIRBLKSIZ];
  }
DIR;

#define dirfd(dirp)	((dirp)->dd_fd)

#ifndef NULL
#define NULL 0
#endif
extern DIR *opendir ();
extern struct direct *readdir ();
extern int telldir ();
extern void seekdir ();
#define rewinddir(dirp)	seekdir((dirp), (long)0)
extern void closedir ();
#endif 
#endif 
