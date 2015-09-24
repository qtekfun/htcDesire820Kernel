/*
 * Copyright (c) 1982, 1985, 1986, 1988, 1993, 1994
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
 *	@(#)socket.h	8.4 (Berkeley) 2/21/94
 * $FreeBSD: src/sys/sys/socket.h,v 1.60 2002/04/20 02:24:35 mike Exp $
 */

#ifndef _SYS_SOCKET_H_
#define	_SYS_SOCKET_H_

#include <sys/types.h>
#include <sys/_types.h>
#include <netinet/in.h>
#include <netdb.h>


# define __SOCKADDR_ARG		struct sockaddr *__restrict
# define __CONST_SOCKADDR_ARG	__const struct sockaddr *


#ifdef _BSD_SA_FAMILY_T_
typedef	_BSD_SA_FAMILY_T_	sa_family_t;
#undef _BSD_SA_FAMILY_T_
#endif

#ifdef	_BSD_SOCKLEN_T_
typedef	_BSD_SOCKLEN_T_	socklen_t;
#undef	_BSD_SOCKLEN_T_
#endif
 
#define	SOCK_STREAM	1		
#define	SOCK_DGRAM	2		
#define	SOCK_RAW	3		
#define	SOCK_RDM	4		
#define	SOCK_SEQPACKET	5		

#define SOCK_CLOEXEC 0x100
#define SOCK_NONBLOCK 0x200

#define	SO_DEBUG	0x0001		
#define	SO_ACCEPTCONN	0x0002		
#define	SO_REUSEADDR	0x0004		
#define	SO_KEEPALIVE	0x0008		
#define	SO_DONTROUTE	0x0010		
#define	SO_BROADCAST	0x0020		
#define	SO_USELOOPBACK	0x0040		
#define	SO_LINGER	0x0080		
#define	SO_OOBINLINE	0x0100		
#define	SO_REUSEPORT	0x0200		
#define	SO_TIMESTAMP	0x0400		
#define	SO_ACCEPTFILTER	0x1000		

#define SO_SNDBUF	0x1001		
#define SO_RCVBUF	0x1002		
#define SO_SNDLOWAT	0x1003		
#define SO_RCVLOWAT	0x1004		
#define SO_SNDTIMEO	0x1005		
#define SO_RCVTIMEO	0x1006		
#define	SO_ERROR	0x1007		
#define	SO_TYPE		0x1008		

struct linger {
	int	l_onoff;		
	int	l_linger;		
};

struct accept_filter_arg {
	char	af_name[16];
	char	af_arg[256-16];
};

#define	SOL_SOCKET	0xffff		

#define	PF_UNSPEC	0	
#define	PF_LOCAL	1	
#define	PF_UNIX		PF_LOCAL 
#define	PF_FILE		PF_LOCAL 
#define	PF_INET		2	
#define	PF_AX25		3	
#define	PF_IPX		4	
#define	PF_APPLETALK	5	
#define	PF_NETROM	6	
#define	PF_BRIDGE	7	
#define	PF_ATMPVC	8	
#define	PF_X25		9	
#define	PF_INET6	10	
#define	PF_ROSE		11	
#define	PF_DECnet	12	
#define	PF_NETBEUI	13	
#define	PF_SECURITY	14	
#define	PF_KEY		15	
#define	PF_NETLINK	16
#define	PF_ROUTE	PF_NETLINK 
#define	PF_PACKET	17	
#define	PF_ASH		18	
#define	PF_ECONET	19	
#define	PF_ATMSVC	20	
#define	PF_SNA		22	
#define	PF_IRDA		23	
#define	PF_PPPOX	24	
#define	PF_WANPIPE	25	
#define	PF_BLUETOOTH	31	
#define	PF_MAX		32	

#define	AF_UNSPEC	PF_UNSPEC
#define	AF_LOCAL	PF_LOCAL
#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
#define	AF_INET		PF_INET
#define	AF_AX25		PF_AX25
#define	AF_IPX		PF_IPX
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_NETROM	PF_NETROM
#define	AF_BRIDGE	PF_BRIDGE
#define	AF_ATMPVC	PF_ATMPVC
#define	AF_X25		PF_X25
#define	AF_INET6	PF_INET6
#define	AF_ROSE		PF_ROSE
#define	AF_DECnet	PF_DECnet
#define	AF_NETBEUI	PF_NETBEUI
#define	AF_SECURITY	PF_SECURITY
#define	AF_KEY		PF_KEY
#define	AF_NETLINK	PF_NETLINK
#define	AF_ROUTE	PF_ROUTE
#define	AF_PACKET	PF_PACKET
#define	AF_ASH		PF_ASH
#define	AF_ECONET	PF_ECONET
#define	AF_ATMSVC	PF_ATMSVC
#define	AF_SNA		PF_SNA
#define	AF_IRDA		PF_IRDA
#define	AF_PPPOX	PF_PPPOX
#define	AF_WANPIPE	PF_WANPIPE
#define	AF_BLUETOOTH	PF_BLUETOOTH
#define	AF_MAX		PF_MAX

struct sockaddr {
	sa_family_t	sa_family;	
	char		sa_data[14];	
};
#define	SOCK_MAXADDRLEN	255		

struct sockproto {
	u_short	sp_family;		
	u_short	sp_protocol;		
};

#define	_SS_MAXSIZE	128U
#define	_SS_ALIGNSIZE	(sizeof(int64_t))
#define	_SS_PAD1SIZE	(_SS_ALIGNSIZE - sizeof(unsigned char) - sizeof(sa_family_t))
#define	_SS_PAD2SIZE	(_SS_MAXSIZE - sizeof(unsigned char) - sizeof(sa_family_t) - \
				_SS_PAD1SIZE - _SS_ALIGNSIZE)

struct sockaddr_storage {
	unsigned char		ss_len;		
	sa_family_t	ss_family;	
	char		__ss_pad1[_SS_PAD1SIZE];
	int64_t		__ss_align;	
	char		__ss_pad2[_SS_PAD2SIZE];
};

#define NET_MAXID	AF_MAX

#define CTL_NET_NAMES { \
	{ 0, 0 }, \
	{ "unix", CTLTYPE_NODE }, \
	{ "inet", CTLTYPE_NODE }, \
	{ "implink", CTLTYPE_NODE }, \
	{ "pup", CTLTYPE_NODE }, \
	{ "chaos", CTLTYPE_NODE }, \
	{ "xerox_ns", CTLTYPE_NODE }, \
	{ "iso", CTLTYPE_NODE }, \
	{ "emca", CTLTYPE_NODE }, \
	{ "datakit", CTLTYPE_NODE }, \
	{ "ccitt", CTLTYPE_NODE }, \
	{ "ibm_sna", CTLTYPE_NODE }, \
	{ "decnet", CTLTYPE_NODE }, \
	{ "dec_dli", CTLTYPE_NODE }, \
	{ "lat", CTLTYPE_NODE }, \
	{ "hylink", CTLTYPE_NODE }, \
	{ "appletalk", CTLTYPE_NODE }, \
	{ "route", CTLTYPE_NODE }, \
	{ "link_layer", CTLTYPE_NODE }, \
	{ "xtp", CTLTYPE_NODE }, \
	{ "coip", CTLTYPE_NODE }, \
	{ "cnt", CTLTYPE_NODE }, \
	{ "rtip", CTLTYPE_NODE }, \
	{ "ipx", CTLTYPE_NODE }, \
	{ "sip", CTLTYPE_NODE }, \
	{ "pip", CTLTYPE_NODE }, \
	{ "isdn", CTLTYPE_NODE }, \
	{ "key", CTLTYPE_NODE }, \
	{ "inet6", CTLTYPE_NODE }, \
	{ "natm", CTLTYPE_NODE }, \
	{ "atm", CTLTYPE_NODE }, \
	{ "hdrcomplete", CTLTYPE_NODE }, \
	{ "netgraph", CTLTYPE_NODE }, \
	{ "snp", CTLTYPE_NODE }, \
	{ "scp", CTLTYPE_NODE }, \
}

#define NET_RT_DUMP	1		
#define NET_RT_FLAGS	2		
#define NET_RT_IFLIST	3		
#define	NET_RT_MAXID	4

#define CTL_NET_RT_NAMES { \
	{ 0, 0 }, \
	{ "dump", CTLTYPE_STRUCT }, \
	{ "flags", CTLTYPE_STRUCT }, \
	{ "iflist", CTLTYPE_STRUCT }, \
}

#ifndef	SOMAXCONN
#define	SOMAXCONN	128
#endif

struct msghdr {
	void		*msg_name;		
	socklen_t	 msg_namelen;		
	struct iovec	*msg_iov;		
	int		 msg_iovlen;		
	void		*msg_control;		
	socklen_t	 msg_controllen;	
	int		 msg_flags;		
};

#define	MSG_OOB		0x1		
#define	MSG_PEEK	0x2		
#define	MSG_DONTROUTE	0x4		
#define	MSG_EOR		0x8		
#define	MSG_TRUNC	0x10		
#define	MSG_CTRUNC	0x20		
#define	MSG_WAITALL	0x40		
#define	MSG_DONTWAIT	0x80		
#define	MSG_EOF		0x100		
#define MSG_COMPAT      0x8000		

struct cmsghdr {
	socklen_t	cmsg_len;		
	int		cmsg_level;		
	int		cmsg_type;		
};

#define CMGROUP_MAX 16

struct cmsgcred {
	pid_t	cmcred_pid;		
	uid_t	cmcred_uid;		
	uid_t	cmcred_euid;		
	gid_t	cmcred_gid;		
	short	cmcred_ngroups;		
	gid_t	cmcred_groups[CMGROUP_MAX];	
};

#define	CMSG_DATA(cmsg)		((unsigned char *)(cmsg) + \
				 _ALIGN(sizeof(struct cmsghdr)))

#define	CMSG_NXTHDR(mhdr, cmsg)	\
	(((caddr_t)(cmsg) + _ALIGN((cmsg)->cmsg_len) + \
	  _ALIGN(sizeof(struct cmsghdr)) > \
	    (caddr_t)(mhdr)->msg_control + (mhdr)->msg_controllen) ? \
	    (struct cmsghdr *)NULL : \
	    (struct cmsghdr *)((caddr_t)(cmsg) + _ALIGN((cmsg)->cmsg_len)))

#define	CMSG_FIRSTHDR(mhdr)	((struct cmsghdr *)(mhdr)->msg_control)

	
#define	CMSG_SPACE(l)		(_ALIGN(sizeof(struct cmsghdr)) + _ALIGN(l))
#define	CMSG_LEN(l)		(_ALIGN(sizeof(struct cmsghdr)) + (l))

#ifdef _KERNEL
#define	CMSG_ALIGN(n)	_ALIGN(n)
#endif

#define	SCM_RIGHTS	0x01		
#define	SCM_TIMESTAMP	0x02		
#define	SCM_CREDS	0x03		

struct osockaddr {
	u_short	sa_family;		
	char	sa_data[14];		
};

struct omsghdr {
	caddr_t	msg_name;		
	int	msg_namelen;		
	struct	iovec *msg_iov;		
	int	msg_iovlen;		
	caddr_t	msg_accrights;		
	int	msg_accrightslen;
};

#define	SHUT_RD		0		
#define	SHUT_WR		1		
#define	SHUT_RDWR	2		

struct sf_hdtr {
	struct iovec *headers;	
	int hdr_cnt;		
	struct iovec *trailers;	
	int trl_cnt;		
};

#include <sys/cdefs.h>

__BEGIN_DECLS
int	accept(int, struct sockaddr *, socklen_t *);
int	bind(int, const struct sockaddr *, socklen_t);
int	connect(int, const struct sockaddr *, socklen_t);
int	getpeername(int, struct sockaddr *, socklen_t *);
int	getsockname(int, struct sockaddr *, socklen_t *);
int	getsockopt(int, int, int, void *, socklen_t *);
int	listen(int, int);
ssize_t	recv(int, void *, size_t, int);
ssize_t	recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
ssize_t	recvmsg(int, struct msghdr *, int);
ssize_t	send(int, const void *, size_t, int);
ssize_t	sendto(int, const void *,
	    size_t, int, const struct sockaddr *, socklen_t);
ssize_t	sendmsg(int, const struct msghdr *, int);
int	sendfile(int, int, off_t, size_t, struct sf_hdtr *, off_t *, int);
int	setsockopt(int, int, int, const void *, socklen_t);
int	shutdown(int, int);
int	socket(int, int, int);
int	socketpair(int, int, int, int *);
__END_DECLS

#endif 
