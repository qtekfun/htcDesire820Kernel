/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#ifndef _IF_H_
#define _IF_H_

#define IF_COMPRESS	0x01	
#define IF_NOCOMPRESS	0x02	
#define IF_AUTOCOMP	0x04	
#define IF_NOCIDCOMP	0x08	

#define IF_MTU 1500
#define IF_MRU 1500
#define	IF_COMP IF_AUTOCOMP	

#if 0
#ifdef USE_PPP
#define IF_MAXLINKHDR 48
#else
#define IF_MAXLINKHDR 40
#endif
#else
        
#define IF_MAXLINKHDR (2 + 14 + 40)
#endif

extern int	if_queued;	

extern	struct mbuf if_fastq;                  
extern	struct mbuf if_batchq;                 
extern	struct mbuf *next_m;

#define ifs_init(ifm) ((ifm)->ifs_next = (ifm)->ifs_prev = (ifm))

#ifdef LOG_ENABLED
struct slirp_ifstats {
	u_int out_pkts;		
	u_int out_bytes;		
	u_int out_errpkts;	
	u_int out_errbytes;	
	u_int in_pkts;		
	u_int in_bytes;		
	u_int in_errpkts;		
	u_int in_errbytes;	

	u_int bytes_saved;	

	u_int in_mbad;		
};
#endif

#endif
