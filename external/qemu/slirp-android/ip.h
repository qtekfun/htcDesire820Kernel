/*
 * Copyright (c) 1982, 1986, 1993
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
 *	@(#)ip.h	8.1 (Berkeley) 6/10/93
 * ip.h,v 1.3 1994/08/21 05:27:30 paul Exp
 */

#ifndef _IP_H_
#define _IP_H_

#include "helper.h"

#ifdef HOST_WORDS_BIGENDIAN
# ifndef NTOHL
#  define NTOHL(d)
# endif
# ifndef NTOHS
#  define NTOHS(d)
# endif
# ifndef HTONL
#  define HTONL(d)
# endif
# ifndef HTONS
#  define HTONS(d)
# endif
#else
# ifndef NTOHL
#  define NTOHL(d) ((d) = ntohl((d)))
# endif
# ifndef NTOHS
#  define NTOHS(d) ((d) = ntohs((u_int16_t)(d)))
# endif
# ifndef HTONL
#  define HTONL(d) ((d) = htonl((d)))
# endif
# ifndef HTONS
#  define HTONS(d) ((d) = htons((u_int16_t)(d)))
# endif
#endif

typedef u_int32_t n_long;                 

#define	IPVERSION	4

struct ip {
#ifdef HOST_WORDS_BIGENDIAN
	u_int ip_v:4;		
	u_int ip_hl:4;		
#else
	u_int ip_hl:4;		
	u_int ip_v:4;		
#endif
	u_int8_t  ip_tos;			
	u_int16_t	ip_len;			
	u_int16_t	ip_id;			
	u_int16_t	ip_off;			
#define	IP_DF 0x4000			
#define	IP_MF 0x2000			
#define	IP_OFFMASK 0x1fff		
	u_int8_t ip_ttl;			
	u_int8_t ip_p;			
	u_int16_t	ip_sum;			
    ipaddr_t        ip_src, ip_dst; 
};

#define	IP_MAXPACKET	65535		

#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04

#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		
#define	IPOPT_NOP		1		

#define	IPOPT_RR		7		
#define	IPOPT_TS		68		
#define	IPOPT_SECURITY		130		
#define	IPOPT_LSRR		131		
#define	IPOPT_SATID		136		
#define	IPOPT_SSRR		137		

#define	IPOPT_OPTVAL		0		
#define	IPOPT_OLEN		1		
#define IPOPT_OFFSET		2		
#define	IPOPT_MINOFF		4		

struct	ip_timestamp {
	u_int8_t	ipt_code;		
	u_int8_t	ipt_len;		
	u_int8_t	ipt_ptr;		
#ifdef HOST_WORDS_BIGENDIAN
	u_int	ipt_oflw:4,		
		ipt_flg:4;		
#else
	u_int	ipt_flg:4,		
		ipt_oflw:4;		
#endif
	union ipt_timestamp {
		n_long	ipt_time[1];
		struct	ipt_ta {
			ipaddr_t ipt_addr;
			n_long   ipt_time;
		} ipt_ta[1];
	} ipt_timestamp;
};

#define	IPOPT_TS_TSONLY		0		
#define	IPOPT_TS_TSANDADDR	1		
#define	IPOPT_TS_PRESPEC	3		

#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

#define	MAXTTL		255		
#define	IPDEFTTL	64		
#define	IPFRAGTTL	60		
#define	IPTTLDEC	1		

#define	IP_MSS		576		

#if SIZEOF_CHAR_P == 4
struct mbuf_ptr {
	struct mbuf *mptr;
	uint32_t dummy;
};
#else
struct mbuf_ptr {
	struct mbuf *mptr;
};
#endif
struct qlink {
	void *next, *prev;
};

struct ipovly {
	struct mbuf_ptr ih_mbuf;	
	u_int8_t	ih_x1;			
	u_int8_t	ih_pr;			
	u_int16_t	ih_len;			
	ipaddr_t        ih_src;		
	ipaddr_t        ih_dst;		
} __attribute__((packed));

struct ipq {
        struct qlink frag_link;			
	struct qlink ip_link;				
	u_int8_t	ipq_ttl;		
	u_int8_t	ipq_p;			
	u_int16_t	ipq_id;			
	ipaddr_t        ipq_src,ipq_dst;
};

struct	ipasfrag {
	struct qlink ipf_link;
	struct ip ipf_ip;
};

#define ipf_off      ipf_ip.ip_off
#define ipf_tos      ipf_ip.ip_tos
#define ipf_len      ipf_ip.ip_len
#define ipf_next     ipf_link.next
#define ipf_prev     ipf_link.prev 

#define MAX_IPOPTLEN	40

struct ipoption {
	u_int32_t ipopt_dst;	
	int8_t	  ipopt_list[MAX_IPOPTLEN];	
};

#ifdef LOG_ENABLED

struct	ipstat {
	u_long	ips_total;		
	u_long	ips_badsum;		
	u_long	ips_tooshort;		
	u_long	ips_toosmall;		
	u_long	ips_badhlen;		
	u_long	ips_badlen;		
	u_long	ips_fragments;		
	u_long	ips_fragdropped;	
	u_long	ips_fragtimeout;	
	u_long	ips_forward;		
	u_long	ips_cantforward;	
	u_long	ips_redirectsent;	
	u_long	ips_noproto;		
	u_long	ips_delivered;		
	u_long	ips_localout;		
	u_long	ips_odropped;		
	u_long	ips_reassembled;	
	u_long	ips_fragmented;		
	u_long	ips_ofragments;		
	u_long	ips_cantfrag;		
	u_long	ips_badoptions;		
	u_long	ips_noroute;		
	u_long	ips_badvers;		
	u_long	ips_rawout;		
	u_long	ips_unaligned;		
};

extern struct	ipstat	ipstat;
#endif

extern struct	ipq	ipq;			
extern u_int16_t	ip_id;				

#endif
