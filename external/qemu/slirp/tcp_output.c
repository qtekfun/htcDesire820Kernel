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
 *	@(#)tcp_output.c	8.3 (Berkeley) 12/30/93
 * tcp_output.c,v 1.3 1994/09/15 10:36:55 davidg Exp
 */

/*
 * Changes and additions relating to SLiRP
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#include <slirp.h>

const char * const tcpstates[] = {
	"REDIRECT",	"LISTEN",	"SYN_SENT",     "SYN_RCVD",
	"ESTABLISHED",  "CLOSE_WAIT",   "FIN_WAIT_1",   "CLOSING",
	"LAST_ACK",     "FIN_WAIT_2",   "TIME_WAIT",
};

static const u_char  tcp_outflags[TCP_NSTATES] = {
	TH_RST|TH_ACK, 0,      TH_SYN,        TH_SYN|TH_ACK,
	TH_ACK,        TH_ACK, TH_FIN|TH_ACK, TH_FIN|TH_ACK,
	TH_FIN|TH_ACK, TH_ACK, TH_ACK,
};


#define MAX_TCPOPTLEN	32	

int
tcp_output(struct tcpcb *tp)
{
	register struct socket *so = tp->t_socket;
	register long len, win;
	int off, flags, error;
	register struct mbuf *m;
	register struct tcpiphdr *ti;
	u_char opt[MAX_TCPOPTLEN];
	unsigned optlen, hdrlen;
	int idle, sendalot;

	DEBUG_CALL("tcp_output");
	DEBUG_ARG("tp = %lx", (long )tp);

	idle = (tp->snd_max == tp->snd_una);
	if (idle && tp->t_idle >= tp->t_rxtcur)
		tp->snd_cwnd = tp->t_maxseg;
again:
	sendalot = 0;
	off = tp->snd_nxt - tp->snd_una;
	win = min(tp->snd_wnd, tp->snd_cwnd);

	flags = tcp_outflags[tp->t_state];

	DEBUG_MISC((dfd, " --- tcp_output flags = 0x%x\n",flags));

	if (tp->t_force) {
		if (win == 0) {
			if (off < so->so_snd.sb_cc)
				flags &= ~TH_FIN;
			win = 1;
		} else {
			tp->t_timer[TCPT_PERSIST] = 0;
			tp->t_rxtshift = 0;
		}
	}

	len = min(so->so_snd.sb_cc, win) - off;

	if (len < 0) {
		len = 0;
		if (win == 0) {
			tp->t_timer[TCPT_REXMT] = 0;
			tp->snd_nxt = tp->snd_una;
		}
	}

	if (len > tp->t_maxseg) {
		len = tp->t_maxseg;
		sendalot = 1;
	}
	if (SEQ_LT(tp->snd_nxt + len, tp->snd_una + so->so_snd.sb_cc))
		flags &= ~TH_FIN;

	win = sbspace(&so->so_rcv);

	if (len) {
		if (len == tp->t_maxseg)
			goto send;
		if ((1 || idle || tp->t_flags & TF_NODELAY) &&
		    len + off >= so->so_snd.sb_cc)
			goto send;
		if (tp->t_force)
			goto send;
		if (len >= tp->max_sndwnd / 2 && tp->max_sndwnd > 0)
			goto send;
		if (SEQ_LT(tp->snd_nxt, tp->snd_max))
			goto send;
	}

	if (win > 0) {
		long adv = min(win, (long)TCP_MAXWIN << tp->rcv_scale) -
			(tp->rcv_adv - tp->rcv_nxt);

		if (adv >= (long) (2 * tp->t_maxseg))
			goto send;
		if (2 * adv >= (long) so->so_rcv.sb_datalen)
			goto send;
	}

	if (tp->t_flags & TF_ACKNOW)
		goto send;
	if (flags & (TH_SYN|TH_RST))
		goto send;
	if (SEQ_GT(tp->snd_up, tp->snd_una))
		goto send;
	if (flags & TH_FIN &&
	    ((tp->t_flags & TF_SENTFIN) == 0 || tp->snd_nxt == tp->snd_una))
		goto send;

	if (so->so_snd.sb_cc && tp->t_timer[TCPT_REXMT] == 0 &&
	    tp->t_timer[TCPT_PERSIST] == 0) {
		tp->t_rxtshift = 0;
		tcp_setpersist(tp);
	}

	STAT(tcpstat.tcps_didnuttin++);

	return (0);

send:
	optlen = 0;
	hdrlen = sizeof (struct tcpiphdr);
	if (flags & TH_SYN) {
		tp->snd_nxt = tp->iss;
		if ((tp->t_flags & TF_NOOPT) == 0) {
			u_int16_t mss;

			opt[0] = TCPOPT_MAXSEG;
			opt[1] = 4;
			mss = htons((u_int16_t) tcp_mss(tp, 0));
			memcpy((caddr_t)(opt + 2), (caddr_t)&mss, sizeof(mss));
			optlen = 4;

		}
 	}

 	hdrlen += optlen;

	 if (len > tp->t_maxseg - optlen) {
		len = tp->t_maxseg - optlen;
		sendalot = 1;
	 }

	if (len) {
		if (tp->t_force && len == 1)
			STAT(tcpstat.tcps_sndprobe++);
		else if (SEQ_LT(tp->snd_nxt, tp->snd_max)) {
			STAT(tcpstat.tcps_sndrexmitpack++);
			STAT(tcpstat.tcps_sndrexmitbyte += len);
		} else {
			STAT(tcpstat.tcps_sndpack++);
			STAT(tcpstat.tcps_sndbyte += len);
		}

		m = m_get();
		if (m == NULL) {
			error = 1;
			goto out;
		}
		m->m_data += IF_MAXLINKHDR;
		m->m_len = hdrlen;


			sbcopy(&so->so_snd, off, (int) len, mtod(m, caddr_t) + hdrlen);
			m->m_len += len;

		if (off + len == so->so_snd.sb_cc)
			flags |= TH_PUSH;
	} else {
		if (tp->t_flags & TF_ACKNOW)
			STAT(tcpstat.tcps_sndacks++);
		else if (flags & (TH_SYN|TH_FIN|TH_RST))
			STAT(tcpstat.tcps_sndctrl++);
		else if (SEQ_GT(tp->snd_up, tp->snd_una))
			STAT(tcpstat.tcps_sndurg++);
		else
			STAT(tcpstat.tcps_sndwinup++);

		m = m_get();
		if (m == NULL) {
			error = 1;
			goto out;
		}
		m->m_data += IF_MAXLINKHDR;
		m->m_len = hdrlen;
	}

	ti = mtod(m, struct tcpiphdr *);

	memcpy((caddr_t)ti, &tp->t_template, sizeof (struct tcpiphdr));

	if (flags & TH_FIN && tp->t_flags & TF_SENTFIN &&
	    tp->snd_nxt == tp->snd_max)
		tp->snd_nxt--;
	if (len || (flags & (TH_SYN|TH_FIN)) || tp->t_timer[TCPT_PERSIST])
		ti->ti_seq = htonl(tp->snd_nxt);
	else
		ti->ti_seq = htonl(tp->snd_max);
	ti->ti_ack = htonl(tp->rcv_nxt);
	if (optlen) {
		memcpy((caddr_t)(ti + 1), (caddr_t)opt, optlen);
		ti->ti_off = (sizeof (struct tcphdr) + optlen) >> 2;
	}
	ti->ti_flags = flags;
	if (win < (long)(so->so_rcv.sb_datalen / 4) && win < (long)tp->t_maxseg)
		win = 0;
	if (win > (long)TCP_MAXWIN << tp->rcv_scale)
		win = (long)TCP_MAXWIN << tp->rcv_scale;
	if (win < (long)(tp->rcv_adv - tp->rcv_nxt))
		win = (long)(tp->rcv_adv - tp->rcv_nxt);
	ti->ti_win = htons((u_int16_t) (win>>tp->rcv_scale));

	if (SEQ_GT(tp->snd_up, tp->snd_una)) {
		ti->ti_urp = htons((u_int16_t)(tp->snd_up - ntohl(ti->ti_seq)));
#ifdef notdef
	if (SEQ_GT(tp->snd_up, tp->snd_nxt)) {
		ti->ti_urp = htons((u_int16_t)(tp->snd_up - tp->snd_nxt));
#endif
		ti->ti_flags |= TH_URG;
	} else
		tp->snd_up = tp->snd_una;		

	if (len + optlen)
		ti->ti_len = htons((u_int16_t)(sizeof (struct tcphdr) +
		    optlen + len));
	ti->ti_sum = cksum(m, (int)(hdrlen + len));

	if (tp->t_force == 0 || tp->t_timer[TCPT_PERSIST] == 0) {
		tcp_seq startseq = tp->snd_nxt;

		if (flags & (TH_SYN|TH_FIN)) {
			if (flags & TH_SYN)
				tp->snd_nxt++;
			if (flags & TH_FIN) {
				tp->snd_nxt++;
				tp->t_flags |= TF_SENTFIN;
			}
		}
		tp->snd_nxt += len;
		if (SEQ_GT(tp->snd_nxt, tp->snd_max)) {
			tp->snd_max = tp->snd_nxt;
			if (tp->t_rtt == 0) {
				tp->t_rtt = 1;
				tp->t_rtseq = startseq;
				STAT(tcpstat.tcps_segstimed++);
			}
		}

		if (tp->t_timer[TCPT_REXMT] == 0 &&
		    tp->snd_nxt != tp->snd_una) {
			tp->t_timer[TCPT_REXMT] = tp->t_rxtcur;
			if (tp->t_timer[TCPT_PERSIST]) {
				tp->t_timer[TCPT_PERSIST] = 0;
				tp->t_rxtshift = 0;
			}
		}
	} else
		if (SEQ_GT(tp->snd_nxt + len, tp->snd_max))
			tp->snd_max = tp->snd_nxt + len;

	m->m_len = hdrlen + len; 

    {

	((struct ip *)ti)->ip_len = m->m_len;

	((struct ip *)ti)->ip_ttl = IPDEFTTL;
	((struct ip *)ti)->ip_tos = so->so_iptos;

	
	error = ip_output(so, m);

    }
	if (error) {
out:
		return (error);
	}
	STAT(tcpstat.tcps_sndtotal++);

	if (win > 0 && SEQ_GT(tp->rcv_nxt+win, tp->rcv_adv))
		tp->rcv_adv = tp->rcv_nxt + win;
	tp->last_ack_sent = tp->rcv_nxt;
	tp->t_flags &= ~(TF_ACKNOW|TF_DELACK);
	if (sendalot)
		goto again;

	return (0);
}

void
tcp_setpersist(struct tcpcb *tp)
{
    int t = ((tp->t_srtt >> 2) + tp->t_rttvar) >> 1;

	TCPT_RANGESET(tp->t_timer[TCPT_PERSIST],
	    t * tcp_backoff[tp->t_rxtshift],
	    TCPTV_PERSMIN, TCPTV_PERSMAX);
	if (tp->t_rxtshift < TCP_MAXRXTSHIFT)
		tp->t_rxtshift++;
}
