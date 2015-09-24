/*-
 * Copyright (c) 1980, 1983, 1988, 1993
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
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * -
 * --Copyright--
 */


#ifndef _NETDB_H_
#define _NETDB_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <machine/ansi.h>
#include <stdio.h>

#ifndef __socklen_t_defined
typedef unsigned int socklen_t;
#define __socklen_t_defined 1
#endif

#ifdef	_BSD_SOCKLEN_T_
typedef	_BSD_SOCKLEN_T_	socklen_t;
#undef	_BSD_SOCKLEN_T_
#endif

#ifndef _PATH_HEQUIV
# define	_PATH_HEQUIV	"/etc/hosts.equiv"
#endif
#define	_PATH_HOSTS	"/etc/hosts"
#define	_PATH_NETWORKS	"/etc/networks"
#define	_PATH_PROTOCOLS	"/etc/protocols"
#define	_PATH_SERVICES	"/etc/services"
#define _PATH_NSSWITCH_CONF  "/etc/nsswitch.conf"

__BEGIN_DECLS
extern int *__h_errno_location(void);
__END_DECLS

#define h_errno (*(__h_errno_location()))

#define	MAXALIASES	35
  
#define MAXADDRS         2
struct hostent {
  char	*h_name;	
  char	**h_aliases;	
  int	h_addrtype;	
  int	h_length;	
  char	**h_addr_list;	
  char *h_addr;         
  
  char *__host_addrs[MAXADDRS];
  char *__host_aliases[MAXALIASES];
  unsigned int __host_addr[4];
};

struct netent {
	char		*n_name;	
	char		**n_aliases;	
	int		n_addrtype;	
	unsigned long	n_net;		
};

struct servent {
	char	*s_name;	
	char	**s_aliases;	
	int	s_port;		
	char	*s_proto;	
};

struct protoent {
	char	*p_name;	
	char	**p_aliases;	
	int	p_proto;	
};

struct addrinfo {
	int	ai_flags;	
	int	ai_family;	
	int	ai_socktype;	
	int	ai_protocol;	
	size_t	ai_addrlen;	
	char	*ai_canonname;	
	struct	sockaddr *ai_addr;	
	struct	addrinfo *ai_next;	
};


#define	NETDB_INTERNAL	-1	
#define	NETDB_SUCCESS	0	
#define	HOST_NOT_FOUND	1 
#define	TRY_AGAIN	2 
#define	NO_RECOVERY	3 
#define	NO_DATA		4 
#define	NO_ADDRESS	NO_DATA		

# define EAI_BADFLAGS     -1    
# define EAI_NONAME       -2    
# define EAI_AGAIN        -3    
# define EAI_FAIL         -4    
# define EAI_NODATA       -5    
# define EAI_FAMILY       -6    
# define EAI_SOCKTYPE     -7    
# define EAI_SERVICE      -8    
# define EAI_ADDRFAMILY   -9    
# define EAI_MEMORY       -10   
# define EAI_SYSTEM       -11   
# define EAI_OVERFLOW     -12   
# ifdef __USE_GNU
#  define EAI_INPROGRESS  -100  
#  define EAI_CANCELED    -101  
#  define EAI_NOTCANCELED -102  
#  define EAI_ALLDONE     -103  
#  define EAI_INTR        -104  
#  define EAI_IDN_ENCODE  -105  
# endif

#define	AI_PASSIVE	0x00000001 
#define	AI_CANONNAME	0x00000002 
#define	AI_NUMERICHOST	0x00000004 
#define AI_NUMERICSERV  0x00000008 
#define AI_MASK \
    (AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST | AI_ADDRCONFIG)

#define	AI_ALL		0x00000100 
#define	AI_V4MAPPED_CFG	0x00000200 
#define	AI_ADDRCONFIG	0x00000400 
#define	AI_V4MAPPED	0x00000800 
#define	AI_DEFAULT	(AI_V4MAPPED_CFG | AI_ADDRCONFIG)

#define	NI_MAXHOST	1025
#define	NI_MAXSERV	32

#define	NI_NOFQDN	0x00000001
#define	NI_NUMERICHOST	0x00000002
#define	NI_NAMEREQD	0x00000004
#define	NI_NUMERICSERV	0x00000008
#define	NI_DGRAM	0x00000010
#define NI_WITHSCOPEID	0x00000020

#define	SCOPE_DELIMITER	'%'

__BEGIN_DECLS
void		endhostent(void);
void		endhostent_r(FILE **, int *);
void		endnetent(void);
void		endnetgrent(void);
void		endprotoent(void);
void		endservent(void);
void		freehostent(struct hostent *);
struct hostent	*gethostbyaddr(const void *, socklen_t, int);
struct hostent	*gethostbyname(const char *);
struct hostent	*gethostbyname2(const char *, int);
struct hostent	*gethostent(void);
int             gethostent_r(struct hostent *, char *, int, int *, FILE **);
struct hostent	*getipnodebyaddr(const void *, size_t, int, int *);
struct hostent	*getipnodebyname(const char *, int, int, int *);
struct netent	*getnetbyaddr(uint32_t, int);
struct netent	*getnetbyname(const char *);
struct netent	*getnetent(void);
int		getnetgrent(char **, char **, char **);
struct protoent	*getprotobyname(const char *);
struct protoent	*getprotobynumber(int);
struct protoent	*getprotoent(void);
struct servent	*getservbyname(const char *, const char *);
struct servent	*getservbyport(int, const char *);
struct servent	*getservent(void);
void		herror(const char *);
__const char	*hstrerror(int);
int		innetgr(const char *, const char *, const char *, const char *);
void		sethostent(int);
void		sethostent_r(int, FILE **, int *);
void		setnetent(int);
void		setprotoent(int);
int		getaddrinfo(const char *__restrict, const char *__restrict,
			    const struct addrinfo *__restrict,
			    struct addrinfo **__restrict);
int		getnameinfo(const struct sockaddr *__restrict, socklen_t,
			    char *__restrict, socklen_t, char *__restrict,
			    socklen_t, unsigned int);
void		freeaddrinfo(struct addrinfo *);
char		*gai_strerror(int);
int		setnetgrent(const char *);
void		setservent(int);


void	_sethosthtent(int);
void	_sethosthtent_r(int, FILE **, int *);
void	_endhosthtent(void);
void	_endhosthtent_r(FILE **, int *);
void	_sethostdnsent(int);
void	_endhostdnsent(void);
void	_setnethtent(int);
void	_endnethtent(void);
void	_setnetdnsent(int);
void	_endnetdnsent(void);
struct hostent * _gethostbynisname(const char *, int);
struct hostent * _gethostbynisaddr(const char *, int, int);
void _map_v4v6_address(const char *, char *);
void _map_v4v6_hostent(struct hostent *, char **, int *);
__END_DECLS

#endif 
