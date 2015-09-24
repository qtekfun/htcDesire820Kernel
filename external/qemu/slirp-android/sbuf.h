/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#ifndef _SBUF_H_
#define _SBUF_H_

#define sbflush(sb) sbdrop((sb),(sb)->sb_cc)
#define sbspace(sb) ((sb)->sb_datalen - (sb)->sb_cc)

struct sbuf {
	u_int	sb_cc;		
	u_int	sb_datalen;	
	char	*sb_wptr;	/* write pointer. points to where the next
				 * bytes should be written in the sbuf */
	char	*sb_rptr;	
	char	*sb_data;	
};

void sbfree _P((struct sbuf *));
void sbdrop _P((struct sbuf *, int));
void sbreserve _P((struct sbuf *, int));
void sbappend _P((struct socket *, struct mbuf *));
void sbcopy _P((struct sbuf *, int, int, char *));

#endif
