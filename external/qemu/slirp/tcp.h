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
 *	@(#)tcp.h	8.1 (Berkeley) 6/10/93
 * tcp.h,v 1.3 1994/08/21 05:27:34 paul Exp
 */

#ifndef _TCP_H_
#define _TCP_H_

typedef	u_int32_t	tcp_seq;

#define      PR_SLOWHZ       2               
#define      PR_FASTHZ       5               

extern struct socket *tcp_last_so;

#define TCP_SNDSPACE 8192
#define TCP_RCVSPACE 8192

struct tcphdr {
	u_int16_t	th_sport;		
	u_int16_t	th_dport;		
	tcp_seq	th_seq;			
	tcp_seq	th_ack;			
#ifdef HOST_WORDS_BIGENDIAN
	u_int	th_off:4,		
		th_x2:4;		
#else
	u_int	th_x2:4,		
		th_off:4;		
#endif
	u_int8_t	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	u_int16_t	th_win;			
	u_int16_t	th_sum;			
	u_int16_t	th_urp;			
};

#include "tcp_var.h"

#define	TCPOPT_EOL		0
#define	TCPOPT_NOP		1
#define	TCPOPT_MAXSEG		2
#define    TCPOLEN_MAXSEG		4
#define TCPOPT_WINDOW		3
#define    TCPOLEN_WINDOW		3
#define TCPOPT_SACK_PERMITTED	4		
#define    TCPOLEN_SACK_PERMITTED	2
#define TCPOPT_SACK		5		
#define TCPOPT_TIMESTAMP	8
#define    TCPOLEN_TIMESTAMP		10
#define    TCPOLEN_TSTAMP_APPA		(TCPOLEN_TIMESTAMP+2) 

#define TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)

#define	TCP_MSS	1460

#define	TCP_MAXWIN	65535	

#define TCP_MAX_WINSHIFT	14	

#undef TCP_NODELAY
#define	TCP_NODELAY	0x01	
#undef TCP_MAXSEG
	


#define TCP_NSTATES     11

#define TCPS_CLOSED             0       
#define TCPS_LISTEN             1       
#define TCPS_SYN_SENT           2       
#define TCPS_SYN_RECEIVED       3       
#define TCPS_ESTABLISHED        4       
#define TCPS_CLOSE_WAIT         5       
#define TCPS_FIN_WAIT_1         6       
#define TCPS_CLOSING            7       
#define TCPS_LAST_ACK           8       
#define TCPS_FIN_WAIT_2         9       
#define TCPS_TIME_WAIT          10      

#define TCPS_HAVERCVDSYN(s)     ((s) >= TCPS_SYN_RECEIVED)
#define TCPS_HAVEESTABLISHED(s) ((s) >= TCPS_ESTABLISHED)
#define TCPS_HAVERCVDFIN(s)     ((s) >= TCPS_TIME_WAIT)

#define SEQ_LT(a,b)     ((int)((a)-(b)) < 0)
#define SEQ_LEQ(a,b)    ((int)((a)-(b)) <= 0)
#define SEQ_GT(a,b)     ((int)((a)-(b)) > 0)
#define SEQ_GEQ(a,b)    ((int)((a)-(b)) >= 0)

#define tcp_rcvseqinit(tp) \
     (tp)->rcv_adv = (tp)->rcv_nxt = (tp)->irs + 1

#define tcp_sendseqinit(tp) \
    (tp)->snd_una = (tp)->snd_nxt = (tp)->snd_max = (tp)->snd_up = (tp)->iss

#define TCP_ISSINCR     (125*1024)      

extern tcp_seq tcp_iss;                

extern const char * const tcpstates[];

#endif
