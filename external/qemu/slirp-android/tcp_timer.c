/*
 * Copyright (c) 1982, 1986, 1988, 1990, 1993
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
 *	@(#)tcp_timer.c	8.1 (Berkeley) 6/10/93
 * tcp_timer.c,v 1.2 1994/08/02 07:49:10 davidg Exp
 */

#include <slirp.h>

#ifdef LOG_ENABLED
struct   tcpstat tcpstat;        
#endif

u_int32_t        tcp_now;                

static struct tcpcb *tcp_timers(register struct tcpcb *tp, int timer);

void
tcp_fasttimo(void)
{
	register struct socket *so;
	register struct tcpcb *tp;

	DEBUG_CALL("tcp_fasttimo");

	so = tcb.so_next;
	if (so)
	for (; so != &tcb; so = so->so_next)
		if ((tp = (struct tcpcb *)so->so_tcpcb) &&
		    (tp->t_flags & TF_DELACK)) {
			tp->t_flags &= ~TF_DELACK;
			tp->t_flags |= TF_ACKNOW;
			STAT(tcpstat.tcps_delack++);
			(void) tcp_output(tp);
		}
}

void
tcp_slowtimo(void)
{
	register struct socket *ip, *ipnxt;
	register struct tcpcb *tp;
	register int i;

	DEBUG_CALL("tcp_slowtimo");

	ip = tcb.so_next;
	if (ip == 0)
	   return;
	for (; ip != &tcb; ip = ipnxt) {
		ipnxt = ip->so_next;
		tp = sototcpcb(ip);
		if (tp == 0)
			continue;
		for (i = 0; i < TCPT_NTIMERS; i++) {
			if (tp->t_timer[i] && --tp->t_timer[i] == 0) {
				tcp_timers(tp,i);
				if (ipnxt->so_prev != ip)
					goto tpgone;
			}
		}
		tp->t_idle++;
		if (tp->t_rtt)
		   tp->t_rtt++;
tpgone:
		;
	}
	tcp_iss += TCP_ISSINCR/PR_SLOWHZ;		
#ifdef TCP_COMPAT_42
	if ((int)tcp_iss < 0)
		tcp_iss = 0;				
#endif
	tcp_now++;					
}

void
tcp_canceltimers(struct tcpcb *tp)
{
	register int i;

	for (i = 0; i < TCPT_NTIMERS; i++)
		tp->t_timer[i] = 0;
}

const int tcp_backoff[TCP_MAXRXTSHIFT + 1] =
   { 1, 2, 4, 8, 16, 32, 64, 64, 64, 64, 64, 64, 64 };

static struct tcpcb *
tcp_timers(register struct tcpcb *tp, int timer)
{
	register int rexmt;

	DEBUG_CALL("tcp_timers");

	switch (timer) {

	case TCPT_2MSL:
		if (tp->t_state != TCPS_TIME_WAIT &&
		    tp->t_idle <= TCP_MAXIDLE)
			tp->t_timer[TCPT_2MSL] = TCPTV_KEEPINTVL;
		else
			tp = tcp_close(tp);
		break;

	case TCPT_REXMT:


		if (++tp->t_rxtshift > TCP_MAXRXTSHIFT) {

			tp->t_maxseg >>= 1;
			if (tp->t_maxseg < 32) {
				tp->t_rxtshift = TCP_MAXRXTSHIFT;
				STAT(tcpstat.tcps_timeoutdrop++);
				tp = tcp_drop(tp, tp->t_softerror);
				 
				return (tp); 
			}

			tp->t_rxtshift = 6;
		}
		STAT(tcpstat.tcps_rexmttimeo++);
		rexmt = TCP_REXMTVAL(tp) * tcp_backoff[tp->t_rxtshift];
		TCPT_RANGESET(tp->t_rxtcur, rexmt,
		    (short)tp->t_rttmin, TCPTV_REXMTMAX); 
		tp->t_timer[TCPT_REXMT] = tp->t_rxtcur;
		if (tp->t_rxtshift > TCP_MAXRXTSHIFT / 4) {
			tp->t_rttvar += (tp->t_srtt >> TCP_RTT_SHIFT);
			tp->t_srtt = 0;
		}
		tp->snd_nxt = tp->snd_una;
		tp->t_rtt = 0;
		{
		u_int win = min(tp->snd_wnd, tp->snd_cwnd) / 2 / tp->t_maxseg;
		if (win < 2)
			win = 2;
		tp->snd_cwnd = tp->t_maxseg;
		tp->snd_ssthresh = win * tp->t_maxseg;
		tp->t_dupacks = 0;
		}
		(void) tcp_output(tp);
		break;

	case TCPT_PERSIST:
		STAT(tcpstat.tcps_persisttimeo++);
		tcp_setpersist(tp);
		tp->t_force = 1;
		(void) tcp_output(tp);
		tp->t_force = 0;
		break;

	case TCPT_KEEP:
		STAT(tcpstat.tcps_keeptimeo++);
		if (tp->t_state < TCPS_ESTABLISHED)
			goto dropit;

		if ((SO_OPTIONS) && tp->t_state <= TCPS_CLOSE_WAIT) {
		    	if (tp->t_idle >= TCPTV_KEEP_IDLE + TCP_MAXIDLE)
				goto dropit;
			STAT(tcpstat.tcps_keepprobe++);
#ifdef TCP_COMPAT_42
			tcp_respond(tp, &tp->t_template, (struct mbuf *)NULL,
			    tp->rcv_nxt - 1, tp->snd_una - 1, 0);
#else
			tcp_respond(tp, &tp->t_template, (struct mbuf *)NULL,
			    tp->rcv_nxt, tp->snd_una - 1, 0);
#endif
			tp->t_timer[TCPT_KEEP] = TCPTV_KEEPINTVL;
		} else
			tp->t_timer[TCPT_KEEP] = TCPTV_KEEP_IDLE;
		break;

	dropit:
		STAT(tcpstat.tcps_keepdrops++);
		tp = tcp_drop(tp, 0); 
		break;
	}

	return (tp);
}
