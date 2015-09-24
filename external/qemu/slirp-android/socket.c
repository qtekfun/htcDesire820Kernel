/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#define WANT_SYS_IOCTL_H
#include "qemu-common.h"
#include <slirp.h>
#include "ip_icmp.h"
#include "main.h"
#ifdef __sun__
#include <sys/filio.h>
#endif
#define  SLIRP_COMPILATION 1
#include "sockets.h"
#include "proxy_common.h"

static void sofcantrcvmore(struct socket *so);
static void sofcantsendmore(struct socket *so);

#if 0
static void
so_init()
{
	
}
#endif

struct socket *
solookup(struct socket *head, uint32_t laddr, u_int lport,
         uint32_t faddr, u_int fport)
{
	struct socket *so;

	for (so = head->so_next; so != head; so = so->so_next) {
		if (so->so_laddr_port == lport &&
		    so->so_laddr_ip   == laddr &&
		    so->so_faddr_ip   == faddr &&
		    so->so_faddr_port == fport)
		   break;
	}

	if (so == head)
	   return (struct socket *)NULL;
	return so;

}

struct socket *
socreate(void)
{
  struct socket *so;

  so = (struct socket *)malloc(sizeof(struct socket));
  if(so) {
    memset(so, 0, sizeof(struct socket));
    so->so_state = SS_NOFDREF;
    so->s = -1;
  }
  return(so);
}

void
sofree(struct socket *so)
{
  if (so->so_state & SS_PROXIFIED)
    proxy_manager_del(so);

  if (so->so_emu==EMU_RSH && so->extra) {
	sofree(so->extra);
	so->extra=NULL;
  }
  if (so == tcp_last_so)
    tcp_last_so = &tcb;
  else if (so == udp_last_so)
    udp_last_so = &udb;

  m_free(so->so_m);

  if(so->so_next && so->so_prev)
    remque(so);  

  free(so);
}

size_t sopreprbuf(struct socket *so, struct iovec *iov, int *np)
{
	int n, lss, total;
	struct sbuf *sb = &so->so_snd;
	int len = sb->sb_datalen - sb->sb_cc;
	int mss = so->so_tcpcb->t_maxseg;

	DEBUG_CALL("sopreprbuf");
	DEBUG_ARG("so = %lx", (long )so);

	len = sb->sb_datalen - sb->sb_cc;

	if (len <= 0)
		return 0;

	iov[0].iov_base = sb->sb_wptr;
        iov[1].iov_base = NULL;
        iov[1].iov_len = 0;
	if (sb->sb_wptr < sb->sb_rptr) {
		iov[0].iov_len = sb->sb_rptr - sb->sb_wptr;
		
		if (iov[0].iov_len > len)
		   iov[0].iov_len = len;
		if (iov[0].iov_len > mss)
		   iov[0].iov_len -= iov[0].iov_len%mss;
		n = 1;
	} else {
		iov[0].iov_len = (sb->sb_data + sb->sb_datalen) - sb->sb_wptr;
		
		if (iov[0].iov_len > len) iov[0].iov_len = len;
		len -= iov[0].iov_len;
		if (len) {
			iov[1].iov_base = sb->sb_data;
			iov[1].iov_len = sb->sb_rptr - sb->sb_data;
			if(iov[1].iov_len > len)
			   iov[1].iov_len = len;
			total = iov[0].iov_len + iov[1].iov_len;
			if (total > mss) {
				lss = total%mss;
				if (iov[1].iov_len > lss) {
					iov[1].iov_len -= lss;
					n = 2;
				} else {
					lss -= iov[1].iov_len;
					iov[0].iov_len -= lss;
					n = 1;
				}
			} else
				n = 2;
		} else {
			if (iov[0].iov_len > mss)
			   iov[0].iov_len -= iov[0].iov_len%mss;
			n = 1;
		}
	}
	if (np)
		*np = n;

	return iov[0].iov_len + (n - 1) * iov[1].iov_len;
}

int
soread(struct socket *so)
{
	int n, nn;
	struct sbuf *sb = &so->so_snd;
	struct iovec iov[2];

	DEBUG_CALL("soread");
	DEBUG_ARG("so = %lx", (long )so);

	sopreprbuf(so, iov, &n);

#ifdef HAVE_READV
	nn = readv(so->s, (struct iovec *)iov, n);
	DEBUG_MISC((dfd, " ... read nn = %d bytes\n", nn));
#else
	nn = socket_recv(so->s, iov[0].iov_base, iov[0].iov_len);
#endif
	if (nn <= 0) {
		if (nn < 0 && (errno == EINTR || errno == EAGAIN))
			return 0;
		else {
			DEBUG_MISC((dfd, " --- soread() disconnected, nn = %d, errno = %d-%s\n", nn, errno,errno_str));
			sofcantrcvmore(so);
			tcp_sockclosed(sototcpcb(so));
			return -1;
		}
	}

#ifndef HAVE_READV
	if (n == 2 && nn == iov[0].iov_len) {
            int ret;
            ret = socket_recv(so->s, iov[1].iov_base, iov[1].iov_len);
            if (ret > 0)
                nn += ret;
        }

	DEBUG_MISC((dfd, " ... read nn = %d bytes\n", nn));
#endif

	
	sb->sb_cc += nn;
	sb->sb_wptr += nn;
	if (sb->sb_wptr >= (sb->sb_data + sb->sb_datalen))
		sb->sb_wptr -= sb->sb_datalen;
	return nn;
}

int soreadbuf(struct socket *so, const char *buf, int size)
{
    int n, nn, copy = size;
	struct sbuf *sb = &so->so_snd;
	struct iovec iov[2];

	DEBUG_CALL("soreadbuf");
	DEBUG_ARG("so = %lx", (long )so);

	if (sopreprbuf(so, iov, &n) < size)
        goto err;

    nn = MIN(iov[0].iov_len, copy);
    memcpy(iov[0].iov_base, buf, nn);

    copy -= nn;
    buf += nn;

    if (copy == 0)
        goto done;

    memcpy(iov[1].iov_base, buf, copy);

done:
    
	sb->sb_cc += size;
	sb->sb_wptr += size;
	if (sb->sb_wptr >= (sb->sb_data + sb->sb_datalen))
		sb->sb_wptr -= sb->sb_datalen;
    return size;
err:

    sofcantrcvmore(so);
    tcp_sockclosed(sototcpcb(so));
    fprintf(stderr, "soreadbuf buffer to small");
    return -1;
}

void
sorecvoob(struct socket *so)
{
	struct tcpcb *tp = sototcpcb(so);

	DEBUG_CALL("sorecvoob");
	DEBUG_ARG("so = %lx", (long)so);

	soread(so);
	tp->snd_up = tp->snd_una + so->so_snd.sb_cc;
	tp->t_force = 1;
	tcp_output(tp);
	tp->t_force = 0;
}

int
sosendoob(struct socket *so)
{
	struct sbuf *sb = &so->so_rcv;
	char buff[2048]; 

	int n, len;

	DEBUG_CALL("sosendoob");
	DEBUG_ARG("so = %lx", (long)so);
	DEBUG_ARG("sb->sb_cc = %d", sb->sb_cc);

	if (so->so_urgc > 2048)
	   so->so_urgc = 2048; 

	if (sb->sb_rptr < sb->sb_wptr) {
		
		n = socket_send_oob(so->s, sb->sb_rptr, so->so_urgc); 
		so->so_urgc -= n;

		DEBUG_MISC((dfd, " --- sent %d bytes urgent data, %d urgent bytes left\n", n, so->so_urgc));
	} else {
		len = (sb->sb_data + sb->sb_datalen) - sb->sb_rptr;
		if (len > so->so_urgc) len = so->so_urgc;
		memcpy(buff, sb->sb_rptr, len);
		so->so_urgc -= len;
		if (so->so_urgc) {
			n = sb->sb_wptr - sb->sb_data;
			if (n > so->so_urgc) n = so->so_urgc;
			memcpy((buff + len), sb->sb_data, n);
			so->so_urgc -= n;
			len += n;
		}
		n = socket_send_oob(so->s, buff, len); 
#ifdef DEBUG
		if (n != len)
		   DEBUG_ERROR((dfd, "Didn't send all data urgently XXXXX\n"));
#endif
		DEBUG_MISC((dfd, " ---2 sent %d bytes urgent data, %d urgent bytes left\n", n, so->so_urgc));
	}

	sb->sb_cc -= n;
	sb->sb_rptr += n;
	if (sb->sb_rptr >= (sb->sb_data + sb->sb_datalen))
		sb->sb_rptr -= sb->sb_datalen;

	return n;
}

int
sowrite(struct socket *so)
{
	int  n,nn;
	struct sbuf *sb = &so->so_rcv;
	int len = sb->sb_cc;
	struct iovec iov[2];

	DEBUG_CALL("sowrite");
	DEBUG_ARG("so = %lx", (long)so);

	if (so->so_urgc) {
		sosendoob(so);
		if (sb->sb_cc == 0)
			return 0;
	}


        len = sb->sb_cc;

	iov[0].iov_base = sb->sb_rptr;
        iov[1].iov_base = NULL;
        iov[1].iov_len = 0;
	if (sb->sb_rptr < sb->sb_wptr) {
		iov[0].iov_len = sb->sb_wptr - sb->sb_rptr;
		
		if (iov[0].iov_len > len) iov[0].iov_len = len;
		n = 1;
	} else {
		iov[0].iov_len = (sb->sb_data + sb->sb_datalen) - sb->sb_rptr;
		if (iov[0].iov_len > len) iov[0].iov_len = len;
		len -= iov[0].iov_len;
		if (len) {
			iov[1].iov_base = sb->sb_data;
			iov[1].iov_len = sb->sb_wptr - sb->sb_data;
			if (iov[1].iov_len > len) iov[1].iov_len = len;
			n = 2;
		} else
			n = 1;
	}
	

#ifdef HAVE_READV
	nn = writev(so->s, (const struct iovec *)iov, n);

	DEBUG_MISC((dfd, "  ... wrote nn = %d bytes\n", nn));
#else
	nn = socket_send(so->s, iov[0].iov_base, iov[0].iov_len);
#endif
	
	if (nn < 0 && (errno == EAGAIN || errno == EINTR))
		return 0;

	if (nn <= 0) {
		DEBUG_MISC((dfd, " --- sowrite disconnected, so->so_state = %x, errno = %d\n",
			so->so_state, errno));
		sofcantsendmore(so);
		tcp_sockclosed(sototcpcb(so));
		return -1;
	}

#ifndef HAVE_READV
	if (n == 2 && nn == iov[0].iov_len) {
            int ret;
            ret = socket_send(so->s, iov[1].iov_base, iov[1].iov_len);
            if (ret > 0)
                nn += ret;
        }
        DEBUG_MISC((dfd, "  ... wrote nn = %d bytes\n", nn));
#endif

	
	sb->sb_cc -= nn;
	sb->sb_rptr += nn;
	if (sb->sb_rptr >= (sb->sb_data + sb->sb_datalen))
		sb->sb_rptr -= sb->sb_datalen;

	if ((so->so_state & SS_FWDRAIN) && sb->sb_cc == 0)
		sofcantsendmore(so);

	return nn;
}

void
sorecvfrom(struct socket *so)
{
        SockAddress  addr;

	DEBUG_CALL("sorecvfrom");
	DEBUG_ARG("so = %lx", (long)so);

	if (so->so_type == IPPROTO_ICMP) {   
	  char buff[256];
	  int len;

	  len = socket_recvfrom(so->s, buff, 256, &addr);
	  

	  if(len == -1 || len == 0) {
	    u_char code=ICMP_UNREACH_PORT;

	    if(errno == EHOSTUNREACH) code=ICMP_UNREACH_HOST;
	    else if(errno == ENETUNREACH) code=ICMP_UNREACH_NET;

	    DEBUG_MISC((dfd," udp icmp rx errno = %d-%s\n",
			errno,errno_str));
	    icmp_error(so->so_m, ICMP_UNREACH,code, 0,errno_str);
	  } else {
	    icmp_reflect(so->so_m);
	    so->so_m = 0; 
	  }
	  
	  udp_detach(so);
	} else {                            	
	  struct mbuf *m;
          int len;
		  int n;

	  if (!(m = m_get())) return;
	  m->m_data += IF_MAXLINKHDR;

	  len = M_FREEROOM(m);
	  
	  n = socket_can_read(so->s);

	  if (n > len) {
	    n = (m->m_data - m->m_dat) + m->m_len + n + 1;
	    m_inc(m, n);
	    len = M_FREEROOM(m);
	  }
	  

	  m->m_len = socket_recvfrom(so->s, m->m_data, len, &addr);
	  DEBUG_MISC((dfd, " did recvfrom %d, errno = %d-%s\n",
		      m->m_len, errno,errno_str));
	  if(m->m_len<0) {
	    u_char code=ICMP_UNREACH_PORT;

	    if(errno == EHOSTUNREACH) code=ICMP_UNREACH_HOST;
	    else if(errno == ENETUNREACH) code=ICMP_UNREACH_NET;

	    DEBUG_MISC((dfd," rx error, tx icmp ICMP_UNREACH:%i\n", code));
	    icmp_error(so->so_m, ICMP_UNREACH,code, 0,errno_str);
	    m_free(m);
	  } else {
	    if (so->so_expire) {
	      if (so->so_faddr_port == 53)
		so->so_expire = curtime + SO_EXPIREFAST;
	      else
		so->so_expire = curtime + SO_EXPIRE;
	    }


	    udp_output_(so, m, &addr);
	  } 
	} 
}

int
sosendto(struct socket *so, struct mbuf *m)
{
	int ret;
    SockAddress   addr;
    uint32_t      addr_ip;
    uint16_t      addr_port;

	DEBUG_CALL("sosendto");
	DEBUG_ARG("so = %lx", (long)so);
	DEBUG_ARG("m = %lx", (long)m);

	if ((so->so_faddr_ip & 0xffffff00) == special_addr_ip) {
        
      	int  low = so->so_faddr_ip & 0xff;

        if ( CTL_IS_DNS(low) )
            addr_ip = dns_addr[low - CTL_DNS];
        else
            addr_ip = loopback_addr_ip;
	} else
	    addr_ip = so->so_faddr_ip;

	addr_port = so->so_faddr_port;

	unsigned long faddr = addr_ip;
        int fport = addr_port;

	if (slirp_should_net_forward(faddr, fport, &faddr, &fport)) {
      time_t timestamp = time(NULL);
      slirp_drop_log(
	       "Redirected UDP: src: 0x%08lx:0x%04x org dst: 0x%08lx:0x%04x "
	       "new dst: 0x%08lx:0x%04x %ld\n",
	        so->so_laddr_ip, so->so_laddr_port,
	        addr_ip, addr_port,
	        faddr, fport, timestamp
	    );
	}
	addr_ip = faddr;
	addr_port = fport;


        sock_address_init_inet(&addr, addr_ip, addr_port);

	DEBUG_MISC((dfd, " sendto()ing, addr.sin_port=%d, addr.sin_addr.s_addr=%08x\n", addr_port, addr_ip));

	
	ret = socket_sendto(so->s, m->m_data, m->m_len,&addr);
	if (ret < 0)
		return -1;

	if (so->so_expire)
		so->so_expire = curtime + SO_EXPIRE;
	so->so_state = SS_ISFCONNECTED; 
	return 0;
}

struct socket *
solisten(u_int port, u_int32_t laddr, u_int lport, int flags)
{
	SockAddress  addr;
	uint32_t     addr_ip;
	struct socket *so;
	int s;

	DEBUG_CALL("solisten");
	DEBUG_ARG("port = %d", port);
	DEBUG_ARG("laddr = %x", laddr);
	DEBUG_ARG("lport = %d", lport);
	DEBUG_ARG("flags = %x", flags);

	if ((so = socreate()) == NULL) {
	  
	  return NULL;
	}

	
	if ((so->so_tcpcb = tcp_newtcpcb(so)) == NULL) {
		free(so);
		return NULL;
	}
	insque(so,&tcb);

	if (flags & SS_FACCEPTONCE)
	   so->so_tcpcb->t_timer[TCPT_KEEP] = TCPTV_KEEP_INIT*2;

	so->so_state      = (SS_FACCEPTCONN|flags);
	so->so_laddr_port = lport; 
    so->so_laddr_ip   = laddr; 
    so->so_haddr_port = port;

    s = socket_loopback_server( port, SOCKET_STREAM );
    if (s < 0)
        return NULL;

    socket_get_address(s, &addr);

	so->so_faddr_port = sock_address_get_port(&addr);

    addr_ip = (uint32_t) sock_address_get_ip(&addr);

    if (addr_ip == 0 || addr_ip == loopback_addr_ip)
        so->so_faddr_ip = alias_addr_ip;
    else
        so->so_faddr_ip = addr_ip;

	so->s = s;
	return so;
}


int
sounlisten(u_int  port)
{
    struct socket *so;

    for (so = tcb.so_next; so != &tcb; so = so->so_next) {
        if (so->so_haddr_port == port) {
            break;
        }
    }

    if (so == &tcb) {
        return -1;
    }

    sofcantrcvmore( so );
    sofcantsendmore( so );
    close( so->s );
    so->s = -1;
    sofree( so );
    return 0;
}


#if 0
static void
sorwakeup(so)
	struct socket *so;
{
}

static void
sowwakeup(so)
	struct socket *so;
{
	
}
#endif

void
soisfconnecting(struct socket *so)
{
	so->so_state &= ~(SS_NOFDREF|SS_ISFCONNECTED|SS_FCANTRCVMORE|
			  SS_FCANTSENDMORE|SS_FWDRAIN);
	so->so_state |= SS_ISFCONNECTING; 
}

void
soisfconnected(struct socket *so)
{
	so->so_state &= ~(SS_ISFCONNECTING|SS_FWDRAIN|SS_NOFDREF);
	so->so_state |= SS_ISFCONNECTED; 
}

static void
sofcantrcvmore(struct socket *so)
{
	if ((so->so_state & SS_NOFDREF) == 0) {
		shutdown(so->s,0);
		if(global_writefds) {
		  FD_CLR(so->s,global_writefds);
		}
	}
	so->so_state &= ~(SS_ISFCONNECTING);
	if (so->so_state & SS_FCANTSENDMORE)
	   so->so_state = SS_NOFDREF;  
	else
	   so->so_state |= SS_FCANTRCVMORE;
}

static void
sofcantsendmore(struct socket *so)
{
	if ((so->so_state & SS_NOFDREF) == 0) {
            shutdown(so->s,1);           
            if (global_readfds) {
                FD_CLR(so->s,global_readfds);
            }
            if (global_xfds) {
                FD_CLR(so->s,global_xfds);
            }
	}
	so->so_state &= ~(SS_ISFCONNECTING);
	if (so->so_state & SS_FCANTRCVMORE)
	   so->so_state = SS_NOFDREF; 
	else
	   so->so_state |= SS_FCANTSENDMORE;
}

void
soisfdisconnected(struct socket *so)
{
}

void
sofwdrain(struct socket *so)
{
	if (so->so_rcv.sb_cc)
		so->so_state |= SS_FWDRAIN;
	else
		sofcantsendmore(so);
}
