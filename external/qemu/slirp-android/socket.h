/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */


#ifndef _SLIRP_SOCKET_H_
#define _SLIRP_SOCKET_H_

#define SO_EXPIRE 240000
#define SO_EXPIREFAST 10000


struct socket {
  struct socket *so_next,*so_prev;      

  int s;                           

			
  struct mbuf *so_m;	           
  struct tcpiphdr *so_ti;	   
  int so_urgc;
  uint32_t   so_faddr_ip;
  uint32_t   so_laddr_ip;
  uint16_t   so_faddr_port;
  uint16_t   so_laddr_port;
  uint16_t   so_haddr_port;

  u_int8_t	so_iptos;	
  u_int8_t	so_emu;		

  u_char	so_type;		
  int	so_state;		

  struct 	tcpcb *so_tcpcb;	
  u_int	so_expire;		

  int	so_queued;		
  int	so_nqueued;		

  struct sbuf so_rcv;		
  struct sbuf so_snd;		
  void * extra;			
};


#define SS_NOFDREF		0x001	

#define SS_ISFCONNECTING	0x002	
#define SS_ISFCONNECTED		0x004	
#define SS_FCANTRCVMORE		0x008	
#define SS_FCANTSENDMORE	0x010	
	
#define SS_FWDRAIN		0x040	

#define SS_CTL			0x080
#define SS_FACCEPTCONN		0x100	
#define SS_FACCEPTONCE		0x200	
#define SS_PROXIFIED            0x400   

extern struct socket tcb;

void so_init _P((void));
struct socket * solookup _P((struct socket *, uint32_t, u_int, uint32_t, u_int));
struct socket * socreate _P((void));
void sofree _P((struct socket *));
int soread _P((struct socket *));
void sorecvoob _P((struct socket *));
int sosendoob _P((struct socket *));
int sowrite _P((struct socket *));
void sorecvfrom _P((struct socket *));
int sosendto _P((struct socket *, struct mbuf *));
struct socket * solisten _P((u_int, u_int32_t, u_int, int));
int  sounlisten _P((u_int port));
void soisfconnecting _P((register struct socket *));
void soisfconnected _P((register struct socket *));
void soisfdisconnected _P((struct socket *));
void sofwdrain _P((struct socket *));
struct iovec; 
size_t sopreprbuf(struct socket *so, struct iovec *iov, int *np);
int soreadbuf(struct socket *so, const char *buf, int size);

#endif 
