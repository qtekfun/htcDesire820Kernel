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


#define	DEV_BSIZE	512

#define	btodb(byte_offset)	((byte_offset) >> 9)
#define	dbtob(block_number)	((block_number) << 9)


typedef unsigned char u_char;	
typedef unsigned short u_short;	
typedef unsigned int u_int;	

typedef struct _quad_
  {
    unsigned int val[2];	
  }
quad;				

typedef unsigned int mach_time_t;	
typedef unsigned int mach_daddr_t;	
typedef unsigned int mach_off_t;	

typedef unsigned short mach_uid_t;
typedef unsigned short mach_gid_t;
typedef unsigned int mach_ino_t;

#define	NBBY	8

#define	MAXBSIZE	8192
#define	MAXFRAG		8


#define	MAXPATHLEN	1024
#define	MAXSYMLINKS	8
