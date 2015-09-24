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
 *	@(#)udp_usrreq.c	8.4 (Berkeley) 1/21/94
 * udp_usrreq.c,v 1.4 1994/10/02 17:48:45 phk Exp
 */

/*
 * Changes and additions relating to SLiRP
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#include <slirp.h>
#include "ip_icmp.h"
#define SLIRP_COMPILATION  1
#include "sockets.h"

#ifdef LOG_ENABLED
struct udpstat udpstat;
#endif

u_int dns_num_conns;

struct socket udb;

static u_int8_t udp_tos(struct socket *so);
static void udp_emu(struct socket *so, struct mbuf *m);

#ifndef	COMPAT_42
#define UDPCKSUM 1
#else
#define UDPCKSUM 0 
#endif

struct	socket *udp_last_so = &udb;

void
udp_init(void)
{
	udb.so_next = udb.so_prev = &udb;
	dns_num_conns = 0;
}
void
udp_input(register struct mbuf *m, int iphlen)
{
	register struct ip *ip;
	register struct udphdr *uh;
	int len;
	struct ip save_ip;
	struct socket *so;

	DEBUG_CALL("udp_input");
	DEBUG_ARG("m = %lx", (long)m);
	DEBUG_ARG("iphlen = %d", iphlen);

	STAT(udpstat.udps_ipackets++);

	if(iphlen > sizeof(struct ip)) {
		ip_stripoptions(m, (struct mbuf *)0);
		iphlen = sizeof(struct ip);
	}

	ip = mtod(m, struct ip *);
	uh = (struct udphdr *)((caddr_t)ip + iphlen);

	len = ntohs((u_int16_t)uh->uh_ulen);

	if (ip->ip_len != len) {
		if (len > ip->ip_len) {
			STAT(udpstat.udps_badlen++);
			goto bad;
		}
		m_adj(m, len - ip->ip_len);
		ip->ip_len = len;
	}

	
	
	
	time_t timestamp = time(NULL);

	if (slirp_should_drop(ntohl(ip->ip_dst.addr), ntohs(uh->uh_dport.port),
	                      IPPROTO_UDP)) {
	  slirp_drop_log(
	    "Dropped UDP: src: 0x%08lx:0x%04x dst: 0x%08lx:0x%04x %ld\n",
	    ntohl(ip->ip_src.addr),
	    ntohs(uh->uh_sport.port),
	    ntohl(ip->ip_dst.addr),
	    ntohs(uh->uh_dport.port),
	    timestamp
	  );
	  goto bad; 
	}
	else {
	  slirp_drop_log(
	    "Allowed UDP: src: 0x%08lx:0x%04x dst: 0x%08lx:0x%04x %ld\n",
	    ntohl(ip->ip_src.addr),
	    ntohs(uh->uh_sport.port),
	    ntohl(ip->ip_dst.addr),
	    ntohs(uh->uh_dport.port),
	    timestamp
	  );
	}
  



	save_ip = *ip;
	save_ip.ip_len+= iphlen;         

	if (UDPCKSUM && uh->uh_sum) {
      memset(&((struct ipovly *)ip)->ih_mbuf, 0, sizeof(struct mbuf_ptr));
	  ((struct ipovly *)ip)->ih_x1 = 0;
	  ((struct ipovly *)ip)->ih_len = uh->uh_ulen;
	  if(cksum(m, len + sizeof(struct ip))) {
	    STAT(udpstat.udps_badsum++);
	    goto bad;
	  }
	}

        if (port_geth(uh->uh_dport) == BOOTP_SERVER) {
            bootp_input(m);
            goto bad;
        }

        if (slirp_restrict)
            goto bad;

        if (port_geth(uh->uh_dport) == TFTP_SERVER) {
            tftp_input(m);
            goto bad;
        }

        
        if (ntohs(uh->uh_dport.port) == 53) {
            if (!slirp_dump_dns(m)) {
                DEBUG_MISC((dfd,"Error logging DNS packet"));
            }
            dns_num_conns++;
            if (slirp_get_max_dns_conns() != -1 &&
                dns_num_conns > slirp_get_max_dns_conns())
                goto bad;
        }


	so = udp_last_so;
	if (so->so_laddr_port != port_geth(uh->uh_sport) ||
	    so->so_laddr_ip   != ip_geth(ip->ip_src)) {
		struct socket *tmp;

		for (tmp = udb.so_next; tmp != &udb; tmp = tmp->so_next) {
			if (tmp->so_laddr_port == port_geth(uh->uh_sport) &&
			    tmp->so_laddr_ip   == ip_geth(ip->ip_src)) {
				tmp->so_faddr_ip   = ip_geth(ip->ip_dst);
				tmp->so_faddr_port = port_geth(uh->uh_dport);
				so = tmp;
				break;
			}
		}
		if (tmp == &udb) {
		  so = NULL;
		} else {
		  STAT(udpstat.udpps_pcbcachemiss++);
		  udp_last_so = so;
		}
	}

	if (so == NULL) {
	  if ((so = socreate()) == NULL) goto bad;
	  if(udp_attach(so) == -1) {
	    DEBUG_MISC((dfd," udp_attach errno = %d-%s\n",
			errno,errno_str));
	    sofree(so);
	    goto bad;
	  }

	  
	  so->so_laddr_ip   = ip_geth(ip->ip_src);
	  so->so_laddr_port = port_geth(uh->uh_sport);

	  if ((so->so_iptos = udp_tos(so)) == 0)
	    so->so_iptos = ip->ip_tos;

	}

        so->so_faddr_ip   = ip_geth(ip->ip_dst); 
        so->so_faddr_port = port_geth(uh->uh_dport); 

	iphlen += sizeof(struct udphdr);
	m->m_len -= iphlen;
	m->m_data += iphlen;

	if (so->so_emu)
	   udp_emu(so, m);

	if(sosendto(so,m) == -1) {
	  m->m_len += iphlen;
	  m->m_data -= iphlen;
	  *ip=save_ip;
	  DEBUG_MISC((dfd,"udp tx errno = %d-%s\n",errno, errno_str));
	  icmp_error(m, ICMP_UNREACH,ICMP_UNREACH_NET, 0,errno_str);
	}

	m_free(so->so_m);   

	
	m->m_len += iphlen;
	m->m_data -= iphlen;
	*ip=save_ip;
	so->so_m=m;         

	return;
bad:
	m_freem(m);
	
	return;
}

int udp_output2_(struct socket *so, struct mbuf *m,
                 const SockAddress*  saddr, 
                 const SockAddress*  daddr,
                 int iptos)
{
    register struct udpiphdr *ui;
    uint32_t  saddr_ip = sock_address_get_ip(saddr);
    uint32_t  daddr_ip = sock_address_get_ip(daddr);
    int       saddr_port = sock_address_get_port(saddr);
    int       daddr_port = sock_address_get_port(daddr);
    int error = 0;

    DEBUG_CALL("udp_output");
	DEBUG_ARG("so = %lx", (long)so);
	DEBUG_ARG("m = %lx", (long)m);
    DEBUG_ARG("saddr = %lx", (long) saddr_ip);
    DEBUG_ARG("daddr = %lx", (long) daddr_ip);

	m->m_data -= sizeof(struct udpiphdr);
	m->m_len += sizeof(struct udpiphdr);

	ui = mtod(m, struct udpiphdr *);
    memset(&ui->ui_i.ih_mbuf, 0 , sizeof(struct mbuf_ptr));
	ui->ui_x1 = 0;
	ui->ui_pr = IPPROTO_UDP;
	ui->ui_len = htons(m->m_len - sizeof(struct ip)); 
	
    ui->ui_src   = ip_seth(saddr_ip);
    ui->ui_dst   = ip_seth(daddr_ip);
    ui->ui_sport = port_seth(saddr_port);
    ui->ui_dport = port_seth(daddr_port);
	ui->ui_ulen = ui->ui_len;

	ui->ui_sum = 0;
	if (UDPCKSUM) {
	    if ((ui->ui_sum = cksum(m,  m->m_len)) == 0)
		ui->ui_sum = 0xffff;
	}
	((struct ip *)ui)->ip_len = m->m_len;

	((struct ip *)ui)->ip_ttl = IPDEFTTL;
	((struct ip *)ui)->ip_tos = iptos;

	STAT(udpstat.udps_opackets++);

	
	if (so != NULL && so->so_faddr_port == 53) {  
	  if (!slirp_dump_dns(m)) {
	    DEBUG_MISC((dfd,"Error logging DNS packet"));
	  }
	}

	error = ip_output(so, m);

	return (error);
}

int udp_output_(struct socket *so, struct mbuf *m,
               SockAddress* from)
{
    SockAddress  saddr, daddr;
    uint32_t     saddr_ip;
    uint16_t     saddr_port;

    saddr_ip   = sock_address_get_ip(from);
    saddr_port = sock_address_get_port(from);

    if ((so->so_faddr_ip & 0xffffff00) == special_addr_ip) {
        saddr_ip = so->so_faddr_ip;
        if ((so->so_faddr_ip & 0x000000ff) == 0xff)
            saddr_ip = alias_addr_ip;
    }

    sock_address_init_inet( &saddr, saddr_ip, saddr_port );
    sock_address_init_inet( &daddr, so->so_laddr_ip, so->so_laddr_port );

    return udp_output2_(so, m, &saddr, &daddr, so->so_iptos);
}

int
udp_attach(struct socket *so)
{
  so->s = socket_anyaddr_server( 0, SOCKET_DGRAM );
  if (so->s != -1) {
      
      so->so_expire = curtime + SO_EXPIRE;
      insque(so,&udb);
  }
  return(so->s);
}

void
udp_detach(struct socket *so)
{
	socket_close(so->s);
	

	sofree(so);
}

static const struct tos_t udptos[] = {
	{0, 53, IPTOS_LOWDELAY, 0},			
	{517, 517, IPTOS_LOWDELAY, EMU_TALK},	
	{518, 518, IPTOS_LOWDELAY, EMU_NTALK},	
	{0, 7648, IPTOS_LOWDELAY, EMU_CUSEEME},	
	{0, 0, 0, 0}
};

static u_int8_t
udp_tos(struct socket *so)
{
	int i = 0;

	while(udptos[i].tos) {
		if ((udptos[i].fport && so->so_faddr_port == udptos[i].fport) ||
		    (udptos[i].lport && so->so_laddr_port == udptos[i].lport)) {
		    	so->so_emu = udptos[i].emu;
			return udptos[i].tos;
		}
		i++;
	}

	return 0;
}


static void
udp_emu(struct socket *so, struct mbuf *m)
{
        SockAddress  sockaddr;

struct cu_header {
	uint16_t	d_family;		
	uint16_t	d_port;			
	uint32_t	d_addr;			
	uint16_t	s_family;		
	uint16_t	s_port;			
	uint32_t	so_addr;		
	uint32_t	seqn;			
	uint16_t	message;		
	uint16_t	data_type;		
	uint16_t	pkt_len;		
} *cu_head;

	switch(so->so_emu) {

	case EMU_CUSEEME:

		if (m->m_len >= sizeof (*cu_head)) {
			if (socket_get_address(so->s, &sockaddr) < 0)
                            return;

			cu_head = mtod(m, struct cu_header *);
			cu_head->s_port  = htons( sock_address_get_port(&sockaddr));
			cu_head->so_addr = htonl( sock_address_get_ip(&sockaddr));
		}

		return;
	}
}

struct socket *
udp_listen(u_int port, u_int32_t laddr, u_int lport, int flags)
{
	struct socket *so;
	SockAddress    addr;
	uint32_t       addr_ip;

	if ((so = socreate()) == NULL) {
		free(so);
		return NULL;
	}
	so->s = socket_anyaddr_server( port, SOCKET_DGRAM );
	so->so_expire = curtime + SO_EXPIRE;
    so->so_haddr_port = port;
	insque(so,&udb);

	if (so->s < 0) {
		udp_detach(so);
		return NULL;
	}

        socket_get_address(so->s, &addr);

	so->so_faddr_port = sock_address_get_port(&addr);
	addr_ip = sock_address_get_ip(&addr);

	if (addr_ip == 0 || addr_ip == loopback_addr_ip)
	   so->so_faddr_ip = alias_addr_ip;
	else
	   so->so_faddr_ip = addr_ip;

	so->so_laddr_port = lport;
	so->so_laddr_ip   = laddr;
	if (flags != SS_FACCEPTONCE)
	   so->so_expire = 0;

	so->so_state = SS_ISFCONNECTED;

	return so;
}

int udp_unlisten (u_int port)
{
	return slirp_unredir(1, port);
}
