/*
 * Copyright (c) 1982, 1986, 1988, 1993
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
 * 3. Neither the name of the University nor the names of its contributors
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
 *	@(#)mbuf.h	8.3 (Berkeley) 1/21/94
 * mbuf.h,v 1.9 1994/11/14 13:54:20 bde Exp
 */

#ifndef _MBUF_H_
#define _MBUF_H_

#define m_freem m_free


#define MINCSIZE 4096	

#define mtod(m,t)	((t)(m)->m_data)


struct m_hdr {
	struct	mbuf *mh_next;		
	struct	mbuf *mh_prev;
	struct	mbuf *mh_nextpkt;	
	struct	mbuf *mh_prevpkt; 
	int	mh_flags;	  

	int	mh_size;		
	struct	socket *mh_so;

	caddr_t	mh_data;		
	int	mh_len;			
};

#define M_ROOM(m) ((m->m_flags & M_EXT)? \
			(((m)->m_ext + (m)->m_size) - (m)->m_data) \
		   : \
			(((m)->m_dat + (m)->m_size) - (m)->m_data))

#define M_FREEROOM(m) (M_ROOM(m) - (m)->m_len)
#define M_TRAILINGSPACE M_FREEROOM

struct mbuf {
	struct	m_hdr m_hdr;
	union M_dat {
		char	m_dat_[1]; 
		char	*m_ext_;
	} M_dat;
};

#define m_next		m_hdr.mh_next
#define m_prev		m_hdr.mh_prev
#define m_nextpkt	m_hdr.mh_nextpkt
#define m_prevpkt	m_hdr.mh_prevpkt
#define m_flags		m_hdr.mh_flags
#define	m_len		m_hdr.mh_len
#define	m_data		m_hdr.mh_data
#define m_size		m_hdr.mh_size
#define m_dat		M_dat.m_dat_
#define m_ext		M_dat.m_ext_
#define m_so		m_hdr.mh_so

#define ifq_prev m_prev
#define ifq_next m_next
#define ifs_prev m_prevpkt
#define ifs_next m_nextpkt
#define ifq_so m_so

#define M_EXT			0x01	
#define M_FREELIST		0x02	
#define M_USEDLIST		0x04	
#define M_DOFREE		0x08	


struct mbstat {
	int mbs_alloced;		

};

extern struct	mbstat mbstat;
extern int mbuf_alloced;
extern struct mbuf m_freelist, m_usedlist;
extern int mbuf_max;

void m_init _P((void));
struct mbuf * m_get _P((void));
void m_free _P((struct mbuf *));
void m_cat _P((register struct mbuf *, register struct mbuf *));
void m_inc _P((struct mbuf *, int));
void m_adj _P((struct mbuf *, int));
int m_copy _P((struct mbuf *, struct mbuf *, int, int));
struct mbuf * dtom _P((void *));

#endif
