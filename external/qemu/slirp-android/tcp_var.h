/*
 * Copyright (c) 1982, 1986, 1993, 1994
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
 *	@(#)tcp_var.h	8.3 (Berkeley) 4/10/94
 * tcp_var.h,v 1.3 1994/08/21 05:27:39 paul Exp
 */

#ifndef _TCP_VAR_H_
#define _TCP_VAR_H_

#include "mbuf.h"
#include "tcpip.h"
#include "tcp_timer.h"

struct tcpcb {
	struct tcpiphdr  *seg_next;	
	struct tcpiphdr  *seg_prev;
	short	t_state;		
	short	t_timer[TCPT_NTIMERS];	
	short	t_rxtshift;		
	short	t_rxtcur;		
	short	t_dupacks;		
	u_short	t_maxseg;		
	char	t_force;		
	u_short	t_flags;
#define	TF_ACKNOW	0x0001		
#define	TF_DELACK	0x0002		
#define	TF_NODELAY	0x0004		
#define	TF_NOOPT	0x0008		
#define	TF_SENTFIN	0x0010		
#define	TF_REQ_SCALE	0x0020		
#define	TF_RCVD_SCALE	0x0040		
#define	TF_REQ_TSTMP	0x0080		
#define	TF_RCVD_TSTMP	0x0100		
#define	TF_SACK_PERMIT	0x0200		

	
	struct	tcpiphdr t_template;

	struct	socket *t_socket;		
	tcp_seq	snd_una;		
	tcp_seq	snd_nxt;		
	tcp_seq	snd_up;			
	tcp_seq	snd_wl1;		
	tcp_seq	snd_wl2;		
	tcp_seq	iss;			
	u_int32_t snd_wnd;		
	u_int32_t rcv_wnd;		
	tcp_seq	rcv_nxt;		
	tcp_seq	rcv_up;			
	tcp_seq	irs;			
	tcp_seq	rcv_adv;		
	tcp_seq	snd_max;		
	u_int32_t snd_cwnd;		
	u_int32_t snd_ssthresh;		
	short	t_idle;			
	short	t_rtt;			
	tcp_seq	t_rtseq;		
	short	t_srtt;			
	short	t_rttvar;		
	u_short	t_rttmin;		
	u_int32_t max_sndwnd;		

	char	t_oobflags;		
	char	t_iobc;			
#define	TCPOOB_HAVEDATA	0x01
#define	TCPOOB_HADDATA	0x02
	short	t_softerror;		

	u_char	snd_scale;		
	u_char	rcv_scale;		
	u_char	request_r_scale;	
	u_char	requested_s_scale;
	u_int32_t	ts_recent;		
	u_int32_t	ts_recent_age;		
	tcp_seq	last_ack_sent;

};

#define	sototcpcb(so)	((so)->so_tcpcb)

#define	TCP_RTT_SCALE		8	
#define	TCP_RTT_SHIFT		3	
#define	TCP_RTTVAR_SCALE	4	
#define	TCP_RTTVAR_SHIFT	2	

#define	TCP_REXMTVAL(tp) \
	(((tp)->t_srtt >> TCP_RTT_SHIFT) + (tp)->t_rttvar)

struct tcpstat {
	u_long	tcps_connattempt;	
	u_long	tcps_accepts;		
	u_long	tcps_connects;		
	u_long	tcps_drops;		
	u_long	tcps_conndrops;		
	u_long	tcps_closed;		
	u_long	tcps_segstimed;		
	u_long	tcps_rttupdated;	
	u_long	tcps_delack;		
	u_long	tcps_timeoutdrop;	
	u_long	tcps_rexmttimeo;	
	u_long	tcps_persisttimeo;	
	u_long	tcps_keeptimeo;		
	u_long	tcps_keepprobe;		
	u_long	tcps_keepdrops;		

	u_long	tcps_sndtotal;		
	u_long	tcps_sndpack;		
	u_long	tcps_sndbyte;		
	u_long	tcps_sndrexmitpack;	
	u_long	tcps_sndrexmitbyte;	
	u_long	tcps_sndacks;		
	u_long	tcps_sndprobe;		
	u_long	tcps_sndurg;		
	u_long	tcps_sndwinup;		
	u_long	tcps_sndctrl;		

	u_long	tcps_rcvtotal;		
	u_long	tcps_rcvpack;		
	u_long	tcps_rcvbyte;		
	u_long	tcps_rcvbadsum;		
	u_long	tcps_rcvbadoff;		
	
	u_long	tcps_rcvduppack;	
	u_long	tcps_rcvdupbyte;	
	u_long	tcps_rcvpartduppack;	
	u_long	tcps_rcvpartdupbyte;	
	u_long	tcps_rcvoopack;		
	u_long	tcps_rcvoobyte;		
	u_long	tcps_rcvpackafterwin;	
	u_long	tcps_rcvbyteafterwin;	
	u_long	tcps_rcvafterclose;	
	u_long	tcps_rcvwinprobe;	
	u_long	tcps_rcvdupack;		
	u_long	tcps_rcvacktoomuch;	
	u_long	tcps_rcvackpack;	
	u_long	tcps_rcvackbyte;	
	u_long	tcps_rcvwinupd;		
	
	u_long	tcps_predack;		
	u_long	tcps_preddat;		
	u_long	tcps_socachemiss;	
	u_long	tcps_didnuttin;		
};

extern struct	tcpstat tcpstat;	
extern u_int32_t	tcp_now;		

#endif
