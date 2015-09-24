/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#include <slirp.h>

int     if_queued = 0;                  

struct  mbuf if_fastq;                  
struct  mbuf if_batchq;                 
struct	mbuf *next_m;			

#define ifs_init(ifm) ((ifm)->ifs_next = (ifm)->ifs_prev = (ifm))

static void
ifs_insque(struct mbuf *ifm, struct mbuf *ifmhead)
{
	ifm->ifs_next = ifmhead->ifs_next;
	ifmhead->ifs_next = ifm;
	ifm->ifs_prev = ifmhead;
	ifm->ifs_next->ifs_prev = ifm;
}

static void
ifs_remque(struct mbuf *ifm)
{
	ifm->ifs_prev->ifs_next = ifm->ifs_next;
	ifm->ifs_next->ifs_prev = ifm->ifs_prev;
}

void
if_init(void)
{
	if_fastq.ifq_next = if_fastq.ifq_prev = &if_fastq;
	if_batchq.ifq_next = if_batchq.ifq_prev = &if_batchq;
        
	next_m = &if_batchq;
}

#if 0
inline int
writen(fd, bptr, n)
	int fd;
	char *bptr;
	int n;
{
	int ret;
	int total;

	
	ret = send(fd, bptr, n,0);
	if (ret == n || ret <= 0)
	   return ret;

	
	total = ret;
	while (n > total) {
		ret = send(fd, bptr+total, n-total,0);
		if (ret <= 0)
		   return ret;
		total += ret;
	}
	return total;
}

#define INBUFF_SIZE 2048 
void
if_input(ttyp)
	struct ttys *ttyp;
{
	u_char if_inbuff[INBUFF_SIZE];
	int if_n;

	DEBUG_CALL("if_input");
	DEBUG_ARG("ttyp = %lx", (long)ttyp);

	if_n = recv(ttyp->fd, (char *)if_inbuff, INBUFF_SIZE,0);

	DEBUG_MISC((dfd, " read %d bytes\n", if_n));

	if (if_n <= 0) {
		if (if_n == 0 || (errno != EINTR && errno != EAGAIN)) {
			if (ttyp->up)
			   link_up--;
			tty_detached(ttyp, 0);
		}
		return;
	}
	if (if_n == 1) {
		if (*if_inbuff == '0') {
			ttyp->ones = 0;
			if (++ttyp->zeros >= 5)
			   slirp_exit(0);
			return;
		}
		if (*if_inbuff == '1') {
			ttyp->zeros = 0;
			if (++ttyp->ones >= 5)
			   tty_detached(ttyp, 0);
			return;
		}
	}
	ttyp->ones = ttyp->zeros = 0;

	(*ttyp->if_input)(ttyp, if_inbuff, if_n);
}
#endif

void
if_output(struct socket *so, struct mbuf *ifm)
{
	struct mbuf *ifq;
	int on_fastq = 1;

	DEBUG_CALL("if_output");
	DEBUG_ARG("so = %lx", (long)so);
	DEBUG_ARG("ifm = %lx", (long)ifm);

	if (ifm->m_flags & M_USEDLIST) {
		remque(ifm);
		ifm->m_flags &= ~M_USEDLIST;
	}

	for (ifq = if_batchq.ifq_prev; ifq != &if_batchq; ifq = ifq->ifq_prev) {
		if (so == ifq->ifq_so) {
			
			ifm->ifq_so = so;
			ifs_insque(ifm, ifq->ifs_prev);
			goto diddit;
		}
	}

	
	if (so && (so->so_iptos & IPTOS_LOWDELAY)) {
		ifq = if_fastq.ifq_prev;
		on_fastq = 1;
		if (ifq->ifq_so == so) {
			ifm->ifq_so = so;
			ifs_insque(ifm, ifq->ifs_prev);
			goto diddit;
		}
	} else
		ifq = if_batchq.ifq_prev;

	
	ifm->ifq_so = so;
	ifs_init(ifm);
	insque(ifm, ifq);

diddit:
	++if_queued;

	if (so) {
		
		so->so_queued++;
		so->so_nqueued++;
		if (on_fastq && ((so->so_nqueued >= 6) &&
				 (so->so_nqueued - so->so_queued) >= 3)) {

			
			remque(ifm->ifs_next);

			
			insque(ifm->ifs_next, &if_batchq);
		}
	}

#ifndef FULL_BOLT
	if (link_up) {
		
		if_start();
	}
#endif
}

void
if_start(void)
{
	struct mbuf *ifm, *ifqt;

	DEBUG_CALL("if_start");

	if (if_queued == 0)
	   return; 

 again:
        
        if (!slirp_can_output())
            return;

	if (if_fastq.ifq_next != &if_fastq) {
		ifm = if_fastq.ifq_next;
	} else {
		
		if (next_m != &if_batchq)
		   ifm = next_m;
		else
		   ifm = if_batchq.ifq_next;

		
		next_m = ifm->ifq_next;
	}
	
	ifqt = ifm->ifq_prev;
	remque(ifm);
	--if_queued;

	
	if (ifm->ifs_next !=  ifm) {
		insque(ifm->ifs_next, ifqt);
		ifs_remque(ifm);
	}

	
	if (ifm->ifq_so) {
		if (--ifm->ifq_so->so_queued == 0)
		   
		   ifm->ifq_so->so_nqueued = 0;
	}

	
        if_encap((uint8_t *)ifm->m_data, ifm->m_len);

        m_free(ifm);

	if (if_queued)
	   goto again;
}
