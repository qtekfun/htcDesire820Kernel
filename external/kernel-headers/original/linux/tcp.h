/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the TCP protocol.
 *
 * Version:	@(#)tcp.h	1.0.2	04/28/93
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _LINUX_TCP_H
#define _LINUX_TCP_H

#include <linux/types.h>
#include <asm/byteorder.h>

struct tcphdr {
	__u16	source;
	__u16	dest;
	__u32	seq;
	__u32	ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	res1:4,
		doff:4,
		fin:1,
		syn:1,
		rst:1,
		psh:1,
		ack:1,
		urg:1,
		ece:1,
		cwr:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	doff:4,
		res1:4,
		cwr:1,
		ece:1,
		urg:1,
		ack:1,
		psh:1,
		rst:1,
		syn:1,
		fin:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif	
	__u16	window;
	__u16	check;
	__u16	urg_ptr;
};

union tcp_word_hdr { 
	struct tcphdr hdr;
	__u32 		  words[5];
}; 

#define tcp_flag_word(tp) ( ((union tcp_word_hdr *)(tp))->words [3]) 

enum { 
	TCP_FLAG_CWR = __constant_htonl(0x00800000), 
	TCP_FLAG_ECE = __constant_htonl(0x00400000), 
	TCP_FLAG_URG = __constant_htonl(0x00200000), 
	TCP_FLAG_ACK = __constant_htonl(0x00100000), 
	TCP_FLAG_PSH = __constant_htonl(0x00080000), 
	TCP_FLAG_RST = __constant_htonl(0x00040000), 
	TCP_FLAG_SYN = __constant_htonl(0x00020000), 
	TCP_FLAG_FIN = __constant_htonl(0x00010000),
	TCP_RESERVED_BITS = __constant_htonl(0x0F000000),
	TCP_DATA_OFFSET = __constant_htonl(0xF0000000)
}; 

#define TCP_NODELAY		1	
#define TCP_MAXSEG		2	
#define TCP_CORK		3	
#define TCP_KEEPIDLE		4	
#define TCP_KEEPINTVL		5	
#define TCP_KEEPCNT		6	
#define TCP_SYNCNT		7	
#define TCP_LINGER2		8	
#define TCP_DEFER_ACCEPT	9	
#define TCP_WINDOW_CLAMP	10	
#define TCP_INFO		11	
#define TCP_QUICKACK		12	
#define TCP_CONGESTION		13	

#define TCPI_OPT_TIMESTAMPS	1
#define TCPI_OPT_SACK		2
#define TCPI_OPT_WSCALE		4
#define TCPI_OPT_ECN		8

enum tcp_ca_state
{
	TCP_CA_Open = 0,
#define TCPF_CA_Open	(1<<TCP_CA_Open)
	TCP_CA_Disorder = 1,
#define TCPF_CA_Disorder (1<<TCP_CA_Disorder)
	TCP_CA_CWR = 2,
#define TCPF_CA_CWR	(1<<TCP_CA_CWR)
	TCP_CA_Recovery = 3,
#define TCPF_CA_Recovery (1<<TCP_CA_Recovery)
	TCP_CA_Loss = 4
#define TCPF_CA_Loss	(1<<TCP_CA_Loss)
};

struct tcp_info
{
	__u8	tcpi_state;
	__u8	tcpi_ca_state;
	__u8	tcpi_retransmits;
	__u8	tcpi_probes;
	__u8	tcpi_backoff;
	__u8	tcpi_options;
	__u8	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

	__u32	tcpi_rto;
	__u32	tcpi_ato;
	__u32	tcpi_snd_mss;
	__u32	tcpi_rcv_mss;

	__u32	tcpi_unacked;
	__u32	tcpi_sacked;
	__u32	tcpi_lost;
	__u32	tcpi_retrans;
	__u32	tcpi_fackets;

	
	__u32	tcpi_last_data_sent;
	__u32	tcpi_last_ack_sent;     
	__u32	tcpi_last_data_recv;
	__u32	tcpi_last_ack_recv;

	
	__u32	tcpi_pmtu;
	__u32	tcpi_rcv_ssthresh;
	__u32	tcpi_rtt;
	__u32	tcpi_rttvar;
	__u32	tcpi_snd_ssthresh;
	__u32	tcpi_snd_cwnd;
	__u32	tcpi_advmss;
	__u32	tcpi_reordering;

	__u32	tcpi_rcv_rtt;
	__u32	tcpi_rcv_space;

	__u32	tcpi_total_retrans;
};

#ifdef __KERNEL__

#include <linux/skbuff.h>
#include <linux/dmaengine.h>
#include <net/sock.h>
#include <net/inet_connection_sock.h>
#include <net/inet_timewait_sock.h>

struct tcp_sack_block {
	__u32	start_seq;
	__u32	end_seq;
};

struct tcp_options_received {
	long	ts_recent_stamp;
	__u32	ts_recent;	
	__u32	rcv_tsval;	
	__u32	rcv_tsecr;	
	__u16 	saw_tstamp : 1,	
		tstamp_ok : 1,	
		dsack : 1,	
		wscale_ok : 1,	
		sack_ok : 4,	
		snd_wscale : 4,	
		rcv_wscale : 4;	
	__u8	eff_sacks;	
	__u8	num_sacks;	
	__u16	user_mss;  	
	__u16	mss_clamp;	
};

struct tcp_request_sock {
	struct inet_request_sock req;
	__u32			 rcv_isn;
	__u32			 snt_isn;
};

static inline struct tcp_request_sock *tcp_rsk(const struct request_sock *req)
{
	return (struct tcp_request_sock *)req;
}

struct tcp_sock {
	
	struct inet_connection_sock	inet_conn;
	int	tcp_header_len;	

	__u32	pred_flags;

 	__u32	rcv_nxt;	
 	__u32	snd_nxt;	

 	__u32	snd_una;	
 	__u32	snd_sml;	
	__u32	rcv_tstamp;	
	__u32	lsndtime;	

	
	struct {
		struct sk_buff_head	prequeue;
		struct task_struct	*task;
		struct iovec		*iov;
		int			memory;
		int			len;
#ifdef CONFIG_NET_DMA
		
		struct dma_chan		*dma_chan;
		int			wakeup;
		struct dma_pinned_list	*pinned_list;
		dma_cookie_t		dma_cookie;
#endif
	} ucopy;

	__u32	snd_wl1;	
	__u32	snd_wnd;	
	__u32	max_window;	
	__u32	mss_cache;	
	__u16	xmit_size_goal;	
	

	__u32	window_clamp;	
	__u32	rcv_ssthresh;	

	__u32	frto_highmark;	
	__u8	reordering;	
	__u8	frto_counter;	
	__u8	nonagle;	
	__u8	keepalive_probes; 

	__u32	srtt;		
	__u32	mdev;		
	__u32	mdev_max;	
	__u32	rttvar;		
	__u32	rtt_seq;	

	__u32	packets_out;	
	__u32	left_out;	
	__u32	retrans_out;	
	struct tcp_options_received rx_opt;

 	__u32	snd_ssthresh;	
 	__u32	snd_cwnd;	
 	__u16	snd_cwnd_cnt;	
	__u16	snd_cwnd_clamp; 
	__u32	snd_cwnd_used;
	__u32	snd_cwnd_stamp;

	struct sk_buff_head	out_of_order_queue; 

 	__u32	rcv_wnd;	
	__u32	rcv_wup;	
	__u32	write_seq;	
	__u32	pushed_seq;	
	__u32	copied_seq;	

	struct tcp_sack_block duplicate_sack[1]; 
	struct tcp_sack_block selective_acks[4]; 

	struct tcp_sack_block recv_sack_cache[4];

	
	struct sk_buff* lost_skb_hint;

	struct sk_buff *scoreboard_skb_hint;
	struct sk_buff *retransmit_skb_hint;
	struct sk_buff *forward_skb_hint;
	struct sk_buff *fastpath_skb_hint;

	int     fastpath_cnt_hint;
	int     lost_cnt_hint;
	int     retransmit_cnt_hint;
	int     forward_cnt_hint;

	__u16	advmss;		
	__u16	prior_ssthresh; 
	__u32	lost_out;	
	__u32	sacked_out;	
	__u32	fackets_out;	
	__u32	high_seq;	

	__u32	retrans_stamp;	
	__u32	undo_marker;	
	int	undo_retrans;	
	__u32	urg_seq;	
	__u16	urg_data;	
	__u8	urg_mode;	
	__u8	ecn_flags;	
	__u32	snd_up;		

	__u32	total_retrans;	
	__u32	bytes_acked;	

	unsigned int		keepalive_time;	  
	unsigned int		keepalive_intvl;  
	int			linger2;

	unsigned long last_synq_overflow; 

	struct {
		__u32	rtt;
		__u32	seq;
		__u32	time;
	} rcv_rtt_est;

	struct {
		int	space;
		__u32	seq;
		__u32	time;
	} rcvq_space;

	struct {
		__u32		  probe_seq_start;
		__u32		  probe_seq_end;
	} mtu_probe;
};

static inline struct tcp_sock *tcp_sk(const struct sock *sk)
{
	return (struct tcp_sock *)sk;
}

struct tcp_timewait_sock {
	struct inet_timewait_sock tw_sk;
	__u32			  tw_rcv_nxt;
	__u32			  tw_snd_nxt;
	__u32			  tw_rcv_wnd;
	__u32			  tw_ts_recent;
	long			  tw_ts_recent_stamp;
};

static inline struct tcp_timewait_sock *tcp_twsk(const struct sock *sk)
{
	return (struct tcp_timewait_sock *)sk;
}

#endif

#endif	
