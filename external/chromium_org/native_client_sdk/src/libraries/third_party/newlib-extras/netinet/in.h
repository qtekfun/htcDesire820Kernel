/*
 * Copyright (c) 1982, 1986, 1990, 1993
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
 *	@(#)in.h	8.3 (Berkeley) 1/3/94
 * $FreeBSD: src/sys/netinet/in.h,v 1.68 2002/04/24 01:26:11 mike Exp $
 */

#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_

#include <sys/cdefs.h>
#include <sys/config.h>
#include <sys/_types.h>
#include <machine/endian.h>

#define	IPPROTO_IP		0		
#define	IPPROTO_ICMP		1		
#define	IPPROTO_TCP		6		
#define	IPPROTO_UDP		17		

#define	INADDR_ANY		(uint32_t)0x00000000
#define	INADDR_BROADCAST	(uint32_t)0xffffffff	

#ifndef _UINT8_T_DECLARED
typedef	__uint8_t		uint8_t;
#define	_UINT8_T_DECLARED
#endif

#ifndef _UINT16_T_DECLARED
typedef	__uint16_t		uint16_t;
#define	_UINT16_T_DECLARED
#endif

#ifndef _UINT32_T_DECLARED
typedef	__uint32_t		uint32_t;
#define	_UINT32_T_DECLARED
#endif

#ifndef _IN_ADDR_T_DECLARED
typedef	uint32_t		in_addr_t;
#define	_IN_ADDR_T_DECLARED
#endif

#ifndef _IN_PORT_T_DECLARED
typedef	uint16_t		in_port_t;
#define	_IN_PORT_T_DECLARED
#endif

typedef unsigned short sa_family_t;

#ifdef _BSD_SA_FAMILY_T_
typedef	_BSD_SA_FAMILY_T_	sa_family_t;
#undef _BSD_SA_FAMILY_T_ 
#endif

#ifndef	_STRUCT_IN_ADDR_DECLARED
struct in_addr {
	in_addr_t s_addr;
};
#define	_STRUCT_IN_ADDR_DECLARED
#endif

struct sockaddr_in {
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};

#if __POSIX_VISIBLE >= 200112
#define	IPPROTO_RAW		255		
#define	INET_ADDRSTRLEN		16
#endif

#define __BSD_VISIBLE 1
#if __BSD_VISIBLE

#define	IPPROTO_HOPOPTS		0		
#define	IPPROTO_IGMP		2		
#define	IPPROTO_GGP		3		
#define IPPROTO_IPV4		4 		
#define IPPROTO_IPIP		IPPROTO_IPV4	
#define	IPPROTO_ST		7		
#define	IPPROTO_EGP		8		
#define	IPPROTO_PIGP		9		
#define	IPPROTO_RCCMON		10		
#define	IPPROTO_NVPII		11		
#define	IPPROTO_PUP		12		
#define	IPPROTO_ARGUS		13		
#define	IPPROTO_EMCON		14		
#define	IPPROTO_XNET		15		
#define	IPPROTO_CHAOS		16		
#define	IPPROTO_MUX		18		
#define	IPPROTO_MEAS		19		
#define	IPPROTO_HMP		20		
#define	IPPROTO_PRM		21		
#define	IPPROTO_IDP		22		
#define	IPPROTO_TRUNK1		23		
#define	IPPROTO_TRUNK2		24		
#define	IPPROTO_LEAF1		25		
#define	IPPROTO_LEAF2		26		
#define	IPPROTO_RDP		27		
#define	IPPROTO_IRTP		28		
#define	IPPROTO_TP		29 		
#define	IPPROTO_BLT		30		
#define	IPPROTO_NSP		31		
#define	IPPROTO_INP		32		
#define	IPPROTO_SEP		33		
#define	IPPROTO_3PC		34		
#define	IPPROTO_IDPR		35		
#define	IPPROTO_XTP		36		
#define	IPPROTO_DDP		37		
#define	IPPROTO_CMTP		38		
#define	IPPROTO_TPXX		39		
#define	IPPROTO_IL		40		
#define	IPPROTO_IPV6		41		
#define	IPPROTO_SDRP		42		
#define	IPPROTO_ROUTING		43		
#define	IPPROTO_FRAGMENT	44		
#define	IPPROTO_IDRP		45		
#define	IPPROTO_RSVP		46 		
#define	IPPROTO_GRE		47		
#define	IPPROTO_MHRP		48		
#define	IPPROTO_BHA		49		
#define	IPPROTO_ESP		50		
#define	IPPROTO_AH		51		
#define	IPPROTO_INLSP		52		
#define	IPPROTO_SWIPE		53		
#define	IPPROTO_NHRP		54		
#define IPPROTO_MOBILE		55		
#define IPPROTO_TLSP		56		
#define IPPROTO_SKIP		57		
#define	IPPROTO_ICMPV6		58		
#define	IPPROTO_NONE		59		
#define	IPPROTO_DSTOPTS		60		
#define	IPPROTO_AHIP		61		
#define	IPPROTO_CFTP		62		
#define	IPPROTO_HELLO		63		
#define	IPPROTO_SATEXPAK	64		
#define	IPPROTO_KRYPTOLAN	65		
#define	IPPROTO_RVD		66		
#define	IPPROTO_IPPC		67		
#define	IPPROTO_ADFS		68		
#define	IPPROTO_SATMON		69		
#define	IPPROTO_VISA		70		
#define	IPPROTO_IPCV		71		
#define	IPPROTO_CPNX		72		
#define	IPPROTO_CPHB		73		
#define	IPPROTO_WSN		74		
#define	IPPROTO_PVP		75		
#define	IPPROTO_BRSATMON	76		
#define	IPPROTO_ND		77		
#define	IPPROTO_WBMON		78		
#define	IPPROTO_WBEXPAK		79		
#define	IPPROTO_EON		80		
#define	IPPROTO_VMTP		81		
#define	IPPROTO_SVMTP		82		
#define	IPPROTO_VINES		83		
#define	IPPROTO_TTP		84		
#define	IPPROTO_IGP		85		
#define	IPPROTO_DGP		86		
#define	IPPROTO_TCF		87		
#define	IPPROTO_IGRP		88		
#define	IPPROTO_OSPFIGP		89		
#define	IPPROTO_SRPC		90		
#define	IPPROTO_LARP		91		
#define	IPPROTO_MTP		92		
#define	IPPROTO_AX25		93		
#define	IPPROTO_IPEIP		94		
#define	IPPROTO_MICP		95		
#define	IPPROTO_SCCSP		96		
#define	IPPROTO_ETHERIP		97		
#define	IPPROTO_ENCAP		98		
#define	IPPROTO_APES		99		
#define	IPPROTO_GMTP		100		
#define	IPPROTO_IPCOMP		108		
#define	IPPROTO_PIM		103		
#define	IPPROTO_PGM		113		
#define	IPPROTO_DIVERT		254		
#define	IPPROTO_MAX		256

#define	IPPROTO_DONE		257


#define	IPPORT_RESERVED		1024

#define	IPPORT_HIFIRSTAUTO	49152
#define	IPPORT_HILASTAUTO	65535

#define IPPORT_RESERVEDSTART	600

#define	IPPORT_MAX		65535

#ifndef _UINT8_T_DECLARED
typedef	__uint8_t		uint8_t;
#define	_UINT8_T_DECLARED
#endif

#ifndef _UINT16_T_DECLARED
typedef	__uint16_t		uint16_t;
#define	_UINT16_T_DECLARED
#endif

#ifndef _UINT32_T_DECLARED
typedef	__uint32_t		uint32_t;
#define	_UINT32_T_DECLARED
#endif

#ifndef _IN_ADDR_T_DECLARED
typedef	uint32_t		in_addr_t;
#define	_IN_ADDR_T_DECLARED
#endif

#ifndef _IN_PORT_T_DECLARED
typedef	uint16_t		in_port_t;
#define	_IN_PORT_T_DECLARED
#endif

#ifdef _BSD_SA_FAMILY_T_
typedef	_BSD_SA_FAMILY_T_	sa_family_t;
#undef _BSD_SA_FAMILY_T_
#endif

#ifndef	_STRUCT_IN_ADDR_DECLARED
struct in_addr {
	in_addr_t s_addr;
};
#define	_STRUCT_IN_ADDR_DECLARED
#endif

#define	IN_CLASSA(i)		(((uint32_t)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET		0xff000000
#define	IN_CLASSA_NSHIFT	24
#define	IN_CLASSA_HOST		0x00ffffff
#define	IN_CLASSA_MAX		128

#define	IN_CLASSB(i)		(((uint32_t)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET		0xffff0000
#define	IN_CLASSB_NSHIFT	16
#define	IN_CLASSB_HOST		0x0000ffff
#define	IN_CLASSB_MAX		65536

#define	IN_CLASSC(i)		(((uint32_t)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET		0xffffff00
#define	IN_CLASSC_NSHIFT	8
#define	IN_CLASSC_HOST		0x000000ff

#define	IN_CLASSD(i)		(((uint32_t)(i) & 0xf0000000) == 0xe0000000)
#define	IN_CLASSD_NET		0xf0000000	
#define	IN_CLASSD_NSHIFT	28		
#define	IN_CLASSD_HOST		0x0fffffff	
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	IN_EXPERIMENTAL(i)	(((uint32_t)(i) & 0xf0000000) == 0xf0000000)
#define	IN_BADCLASS(i)		(((uint32_t)(i) & 0xf0000000) == 0xf0000000)

#define	INADDR_LOOPBACK		(uint32_t)0x7f000001
#ifndef _KERNEL
#define	INADDR_NONE		0xffffffff		
#endif

#define	INADDR_UNSPEC_GROUP	(uint32_t)0xe0000000	
#define	INADDR_ALLHOSTS_GROUP	(uint32_t)0xe0000001	
#define	INADDR_ALLRTRS_GROUP	(uint32_t)0xe0000002	
#define	INADDR_MAX_LOCAL_GROUP	(uint32_t)0xe00000ff	

#define	IN_LOOPBACKNET		127			

#define	IP_OPTIONS		1    
#define	IP_HDRINCL		2    
#define	IP_TOS			3    
#define	IP_TTL			4    
#define	IP_RECVOPTS		5    
#define	IP_RECVRETOPTS		6    
#define	IP_RECVDSTADDR		7    
#define	IP_RETOPTS		8    
#define	IP_MULTICAST_IF		9    
#define	IP_MULTICAST_TTL	10   
#define	IP_MULTICAST_LOOP	11   
#define	IP_ADD_MEMBERSHIP	12   
#define	IP_DROP_MEMBERSHIP	13   
#define IP_MULTICAST_VIF	14   
#define IP_RSVP_ON		15   
#define IP_RSVP_OFF		16   
#define IP_RSVP_VIF_ON		17   
#define IP_RSVP_VIF_OFF		18   
#define IP_PORTRANGE		19   
#define	IP_RECVIF		20   
#define	IP_IPSEC_POLICY		21   
#define	IP_FAITH		22   

#define	IP_FW_ADD     		50   
#define	IP_FW_DEL    		51   
#define	IP_FW_FLUSH   		52   
#define	IP_FW_ZERO    		53   
#define	IP_FW_GET     		54   
#define	IP_FW_RESETLOG		55   

#define	IP_DUMMYNET_CONFIGURE	60   
#define	IP_DUMMYNET_DEL		61   
#define	IP_DUMMYNET_FLUSH	62   
#define	IP_DUMMYNET_GET		64   

#define	IP_DEFAULT_MULTICAST_TTL  1	
#define	IP_DEFAULT_MULTICAST_LOOP 1	
#define	IP_MAX_MEMBERSHIPS	20	

struct ip_mreq {
	struct	in_addr imr_multiaddr;	
	struct	in_addr imr_interface;	
};

#define	IP_PORTRANGE_DEFAULT	0	
#define	IP_PORTRANGE_HIGH	1	
#define	IP_PORTRANGE_LOW	2	

#define	IPPROTO_MAXID	(IPPROTO_AH + 1)	

#define	CTL_IPPROTO_NAMES { \
	{ "ip", CTLTYPE_NODE }, \
	{ "icmp", CTLTYPE_NODE }, \
	{ "igmp", CTLTYPE_NODE }, \
	{ "ggp", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "tcp", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ "egp", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "pup", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "udp", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "idp", CTLTYPE_NODE }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "ipsec", CTLTYPE_NODE }, \
}

#define	IPCTL_FORWARDING	1	
#define	IPCTL_SENDREDIRECTS	2	
#define	IPCTL_DEFTTL		3	
#ifdef notyet
#define	IPCTL_DEFMTU		4	
#endif
#define IPCTL_RTEXPIRE		5	
#define IPCTL_RTMINEXPIRE	6	
#define IPCTL_RTMAXCACHE	7	
#define	IPCTL_SOURCEROUTE	8	
#define	IPCTL_DIRECTEDBROADCAST	9	
#define IPCTL_INTRQMAXLEN	10	
#define	IPCTL_INTRQDROPS	11	
#define	IPCTL_STATS		12	
#define	IPCTL_ACCEPTSOURCEROUTE	13	
#define	IPCTL_FASTFORWARDING	14	
#define	IPCTL_KEEPFAITH		15	
#define	IPCTL_GIF_TTL		16	
#define	IPCTL_MAXID		17

#define	IPCTL_NAMES { \
	{ 0, 0 }, \
	{ "forwarding", CTLTYPE_INT }, \
	{ "redirect", CTLTYPE_INT }, \
	{ "ttl", CTLTYPE_INT }, \
	{ "mtu", CTLTYPE_INT }, \
	{ "rtexpire", CTLTYPE_INT }, \
	{ "rtminexpire", CTLTYPE_INT }, \
	{ "rtmaxcache", CTLTYPE_INT }, \
	{ "sourceroute", CTLTYPE_INT }, \
 	{ "directed-broadcast", CTLTYPE_INT }, \
	{ "intr-queue-maxlen", CTLTYPE_INT }, \
	{ "intr-queue-drops", CTLTYPE_INT }, \
	{ "stats", CTLTYPE_STRUCT }, \
	{ "accept_sourceroute", CTLTYPE_INT }, \
	{ "fastforwarding", CTLTYPE_INT }, \
}

#endif 

#ifdef _KERNEL

struct ifnet; struct mbuf;	

int	 in_broadcast(struct in_addr, struct ifnet *);
int	 in_canforward(struct in_addr);
int	 in_localaddr(struct in_addr);
char 	*inet_ntoa(struct in_addr); 
char	*inet_ntoa_r(struct in_addr ina, char *buf); 

#define satosin(sa)	((struct sockaddr_in *)(sa))
#define sintosa(sin)	((struct sockaddr *)(sin))
#define ifatoia(ifa)	((struct in_ifaddr *)(ifa))

#endif 

#define	__KAME_NETINET_IN_H_INCLUDED_
#include <netinet6/in6.h>
#undef __KAME_NETINET_IN_H_INCLUDED_

__BEGIN_DECLS
uint32_t         htonl(uint32_t hostlong);
uint16_t         htons(uint16_t hostshort);
uint32_t         ntohl(uint32_t networklong);
uint16_t         ntohs(uint16_t networkshort);
__END_DECLS

#endif 
