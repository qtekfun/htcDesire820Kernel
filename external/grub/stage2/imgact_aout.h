
/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)exec.h	8.1 (Berkeley) 6/11/93
 *	$Id: imgact_aout.h,v 1.1 1999/06/24 00:03:22 okuji Exp $
 */

#ifndef	_IMGACT_AOUT_H_
#define	_IMGACT_AOUT_H_

#define ntohl(x) ((x << 24) | ((x & 0xFF00) << 8) \
		  | ((x >> 8) & 0xFF00) | (x >> 24))
#define htonl(x) ntohl(x)
#define __LDPGSZ 0x1000

#define N_GETMAGIC(ex) \
	( (ex).a_midmag & 0xffff )
#define N_GETMID(ex) \
	( (N_GETMAGIC_NET(ex) == ZMAGIC) ? N_GETMID_NET(ex) : \
	((ex).a_midmag >> 16) & 0x03ff )
#define N_GETFLAG(ex) \
	( (N_GETMAGIC_NET(ex) == ZMAGIC) ? N_GETFLAG_NET(ex) : \
	((ex).a_midmag >> 26) & 0x3f )
#define N_SETMAGIC(ex,mag,mid,flag) \
	( (ex).a_midmag = (((flag) & 0x3f) <<26) | (((mid) & 0x03ff) << 16) | \
	((mag) & 0xffff) )

#define N_GETMAGIC_NET(ex) \
	(ntohl((ex).a_midmag) & 0xffff)
#define N_GETMID_NET(ex) \
	((ntohl((ex).a_midmag) >> 16) & 0x03ff)
#define N_GETFLAG_NET(ex) \
	((ntohl((ex).a_midmag) >> 26) & 0x3f)
#define N_SETMAGIC_NET(ex,mag,mid,flag) \
	( (ex).a_midmag = htonl( (((flag)&0x3f)<<26) | (((mid)&0x03ff)<<16) | \
	(((mag)&0xffff)) ) )

#define N_ALIGN(ex,x) \
	(N_GETMAGIC(ex) == ZMAGIC || N_GETMAGIC(ex) == QMAGIC || \
	 N_GETMAGIC_NET(ex) == ZMAGIC || N_GETMAGIC_NET(ex) == QMAGIC ? \
	 ((x) + __LDPGSZ - 1) & ~(unsigned long)(__LDPGSZ - 1) : (x))

#define	N_BADMAG(ex) \
	(N_GETMAGIC(ex) != OMAGIC && N_GETMAGIC(ex) != NMAGIC && \
	 N_GETMAGIC(ex) != ZMAGIC && N_GETMAGIC(ex) != QMAGIC && \
	 N_GETMAGIC_NET(ex) != OMAGIC && N_GETMAGIC_NET(ex) != NMAGIC && \
	 N_GETMAGIC_NET(ex) != ZMAGIC && N_GETMAGIC_NET(ex) != QMAGIC)


#define N_TXTADDR(ex) \
	((N_GETMAGIC(ex) == OMAGIC || N_GETMAGIC(ex) == NMAGIC || \
	N_GETMAGIC(ex) == ZMAGIC) ? 0 : __LDPGSZ)

#define N_DATADDR(ex) \
	N_ALIGN(ex, N_TXTADDR(ex) + (ex).a_text)

#define	N_TXTOFF(ex) \
	(N_GETMAGIC(ex) == ZMAGIC ? __LDPGSZ : (N_GETMAGIC(ex) == QMAGIC || \
	N_GETMAGIC_NET(ex) == ZMAGIC) ? 0 : sizeof(struct exec))

#define	N_DATOFF(ex) \
	N_ALIGN(ex, N_TXTOFF(ex) + (ex).a_text)

#define N_RELOFF(ex) \
	N_ALIGN(ex, N_DATOFF(ex) + (ex).a_data)

#define N_SYMOFF(ex) \
	(N_RELOFF(ex) + (ex).a_trsize + (ex).a_drsize)

#define	N_STROFF(ex) 	(N_SYMOFF(ex) + (ex).a_syms)


struct exec
  {
    unsigned long a_midmag;	
    unsigned long a_text;	
    unsigned long a_data;	
    unsigned long a_bss;	
    unsigned long a_syms;	
    unsigned long a_entry;	
    unsigned long a_trsize;	
    unsigned long a_drsize;	
  };
#define a_magic a_midmag	

#define	OMAGIC          0x107	
#define	NMAGIC          0x108	
#define	ZMAGIC          0x10b	
#define QMAGIC          0xcc	

#define	MID_ZERO	0	
#define	MID_SUN010	1	
#define	MID_SUN020	2	
#define MID_I386	134	
#define MID_SPARC	138	
#define	MID_HP200	200	
#define	MID_HP300	300	
#define	MID_HPUX	0x20C	
#define	MID_HPUX800     0x20B	

#define EX_PIC		0x10	
#define EX_DYNAMIC	0x20	
#define EX_DPMASK	0x30	

#endif 
