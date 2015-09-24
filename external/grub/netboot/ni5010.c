
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include "etherboot.h"
#include "nic.h"
#include "cards.h"

/*
 * Racal-Interlan ni5010 Ethernet definitions
 *
 * This is an extension to the Linux operating system, and is covered by the
 * same Gnu Public License that covers that work.
 *
 * copyrights (c) 1996 by Jan-Pascal van Best (jvbest@wi.leidenuniv.nl)
 *
 * I have done a look in the following sources:
 *   crynwr-packet-driver by Russ Nelson
 */

#define NI5010_BUFSIZE	2048	

#define NI5010_MAGICVAL0 0x00  
#define NI5010_MAGICVAL1 0x55
#define NI5010_MAGICVAL2 0xAA

#define SA_ADDR0 0x02
#define SA_ADDR1 0x07
#define SA_ADDR2 0x01

#define NI5010_IO_EXTENT       32

#define PRINTK(x) if (NI5010_DEBUG) printk x
#define PRINTK2(x) if (NI5010_DEBUG>=2) printk x
#define PRINTK3(x) if (NI5010_DEBUG>=3) printk x

#define EDLC_XSTAT	(ioaddr + 0x00)	
#define EDLC_XCLR	(ioaddr + 0x00)	
#define EDLC_XMASK	(ioaddr + 0x01)	
#define EDLC_RSTAT	(ioaddr + 0x02)	
#define EDLC_RCLR	(ioaddr + 0x02)	
#define EDLC_RMASK	(ioaddr + 0x03)	
#define EDLC_XMODE	(ioaddr + 0x04)	
#define EDLC_RMODE	(ioaddr + 0x05)	
#define EDLC_RESET	(ioaddr + 0x06)	
#define EDLC_TDR1	(ioaddr + 0x07)	
#define EDLC_ADDR	(ioaddr + 0x08)	
	 			
#define EDLC_TDR2	(ioaddr + 0x0f)	
#define IE_GP		(ioaddr + 0x10)	
				
#define IE_RCNT		(ioaddr + 0x10)	
 				
#define IE_MMODE	(ioaddr + 0x12)	
#define IE_DMA_RST	(ioaddr + 0x13)	
#define IE_ISTAT	(ioaddr + 0x13)	
#define IE_RBUF		(ioaddr + 0x14)	
#define IE_XBUF		(ioaddr + 0x15)	
#define IE_SAPROM	(ioaddr + 0x16)	
#define IE_RESET	(ioaddr + 0x17)	

#define XS_TPOK		0x80	
#define XS_CS		0x40	
#define XS_RCVD		0x20	
#define XS_SHORT	0x10	
#define XS_UFLW		0x08	
#define XS_COLL		0x04	
#define XS_16COLL	0x02	
#define XS_PERR		0x01	

#define XS_CLR_UFLW	0x08	
#define XS_CLR_COLL	0x04	
#define XS_CLR_16COLL	0x02	
#define XS_CLR_PERR	0x01	

#define XM_TPOK		0x80	
#define XM_RCVD		0x20	
#define XM_UFLW		0x08	
#define XM_COLL		0x04	
#define XM_COLL16	0x02	
#define XM_PERR		0x01	
 				
#define XM_ALL		(XM_TPOK | XM_RCVD | XM_UFLW | XM_COLL | XM_COLL16)

#define RS_PKT_OK	0x80	
#define RS_RST_PKT	0x10	
#define RS_RUNT		0x08	
#define RS_ALIGN	0x04	
#define RS_CRC_ERR	0x02	
#define RS_OFLW		0x01	
#define RS_VALID_BITS	( RS_PKT_OK | RS_RST_PKT | RS_RUNT | RS_ALIGN | RS_CRC_ERR | RS_OFLW )
 				

#define RS_CLR_PKT_OK	0x80	
#define RS_CLR_RST_PKT	0x10	
#define RS_CLR_RUNT	0x08	
#define RS_CLR_ALIGN	0x04	
#define RS_CLR_CRC_ERR	0x02	
#define RS_CLR_OFLW	0x01	

#define RM_PKT_OK	0x80	
#define RM_RST_PKT	0x10	
#define RM_RUNT		0x08	
#define RM_ALIGN	0x04	
#define RM_CRC_ERR	0x02	
#define RM_OFLW		0x01	

#define RMD_TEST	0x80	
#define RMD_ADD_SIZ	0x10	
#define RMD_EN_RUNT	0x08	
#define RMD_EN_RST	0x04	

#define RMD_PROMISC	0x03	
#define RMD_MULTICAST	0x02	
#define RMD_BROADCAST	0x01	
#define RMD_NO_PACKETS	0x00	

#define XMD_COLL_CNT	0xf0	
#define XMD_IG_PAR	0x08	
#define XMD_T_MODE	0x04	
#define XMD_LBC		0x02	
#define XMD_DIS_C	0x01	

#define RS_RESET	0x80	

#define MM_EN_DMA	0x80	
#define MM_EN_RCV	0x40	
#define MM_EN_XMT	0x20	
#define MM_BUS_PAGE	0x18	
#define MM_NET_PAGE	0x06	
#define MM_MUX		0x01	
				

#define IS_TDIAG	0x80	
#define IS_EN_RCV	0x20	
#define IS_EN_XMT	0x10	
#define IS_EN_DMA	0x08	
#define IS_DMA_INT	0x04	
#define IS_R_INT	0x02	
#define IS_X_INT	0x01	


static unsigned short		ioaddr = 0;
static unsigned int		bufsize_rcv = 0;

#if	0
static void show_registers(void)
{
	printf("XSTAT %hhX ", inb(EDLC_XSTAT));
	printf("XMASK %hhX ", inb(EDLC_XMASK));
	printf("RSTAT %hhX ", inb(EDLC_RSTAT));
	printf("RMASK %hhX ", inb(EDLC_RMASK));
	printf("RMODE %hhX ", inb(EDLC_RMODE));
	printf("XMODE %hhX ", inb(EDLC_XMODE));
	printf("ISTAT %hhX\n", inb(IE_ISTAT));
}
#endif

static void reset_receiver(void)
{
	outw(0, IE_GP);		
	outb(RS_VALID_BITS, EDLC_RCLR);	
	outb(MM_EN_RCV, IE_MMODE); 
}

static void ni5010_reset(struct nic *nic)
{
	int		i;

	
	outb(RS_RESET, EDLC_RESET);	
	outb(0, IE_RESET);	
	outb(0, EDLC_XMASK);	
	outb(0, EDLC_RMASK);	
	outb(0xFF, EDLC_XCLR);	
	outb(0xFF, EDLC_RCLR);	
	outb(XMD_LBC, EDLC_XMODE);	
	
	for(i = 0; i < ETH_ALEN; i++)
		outb(nic->node_addr[i], EDLC_ADDR + i);
	outb(XMD_IG_PAR | XMD_T_MODE | XMD_LBC, EDLC_XMODE); 
				
	outb(RMD_BROADCAST, EDLC_RMODE);
				
	reset_receiver();
	outb(0x00, EDLC_RESET);	
}

static int ni5010_poll(struct nic *nic)
{
	int		rcv_stat;

	if (((rcv_stat = inb(EDLC_RSTAT)) & RS_VALID_BITS) != RS_PKT_OK) {
		outb(rcv_stat, EDLC_RSTAT);	
		return (0);
	}
        outb(rcv_stat, EDLC_RCLR);	
	nic->packetlen = inw(IE_RCNT);
	
        outb(MM_MUX, IE_MMODE);	
	outw(0, IE_GP);		
	insb(IE_RBUF, nic->packet, nic->packetlen); 
	return (1);
}

static void ni5010_transmit(struct nic *nic,
	const char *d,	
	unsigned int t,	
	unsigned int s,	
	const char *p)	
{
	unsigned int	len;
	int		buf_offs, xmt_stat;
	unsigned long	time;

	len = s + ETH_HLEN;
	if (len < ETH_ZLEN)
		len = ETH_ZLEN;
	buf_offs = NI5010_BUFSIZE - len;
	outb(0, EDLC_RMASK);	
	outb(0, IE_MMODE);	
	outb(0xFF, EDLC_RCLR);	
	outw(buf_offs, IE_GP);	
	outsb(IE_XBUF, d, ETH_ALEN);	
	outsb(IE_XBUF, nic->node_addr, ETH_ALEN);
	outb(t >> 8, IE_XBUF);
	outb(t, IE_XBUF);
	outsb(IE_XBUF, p, s);	
	while (s++ < ETH_ZLEN - ETH_HLEN)	
		outb(0, IE_XBUF);
	outw(buf_offs, IE_GP);	
	
	
	
	outb(MM_EN_XMT | MM_MUX, IE_MMODE);	
	
	while (((xmt_stat = inb(IE_ISTAT)) & IS_EN_XMT) != 0)
		;
	reset_receiver();	
}

static void ni5010_disable(struct nic *nic)
{
	outb(0, IE_MMODE);
	outb(RS_RESET, EDLC_RESET);
}

static inline int rd_port(void)
{
	inb(IE_RBUF);
	return inb(IE_SAPROM);
}

static int ni5010_probe1(struct nic *nic)
{
	int		i, boguscount = 40, data;

	if (inb(ioaddr) == 0xFF)
		return (0);
	while ((rd_port() & rd_port() & rd_port()
		& rd_port() & rd_port() & rd_port()) != 0xFF)
	{
		if (boguscount-- <= 0)
			return (0);
	}
	for (i = 0; i < 32; i++)
		if ((data = rd_port()) != 0xFF)
			break;
	if (data == 0xFF)
		return (0);
	if (data == SA_ADDR0 && rd_port() == SA_ADDR1 && rd_port() == SA_ADDR2) {
		for (i = 0; i < 4; i++)
			rd_port();
		if (rd_port() != NI5010_MAGICVAL1 || rd_port() != NI5010_MAGICVAL2)
			return (0);
	} else
		return (0);
	for (i = 0; i < ETH_ALEN; i++) {
		outw(i, IE_GP);
		nic->node_addr[i] = inb(IE_SAPROM);
	}
	printf("\nNI5010 ioaddr %#hX, addr %!\n", ioaddr, nic->node_addr);
/* get the size of the onboard receive buffer
 * higher addresses than bufsize are wrapped into real buffer
 * i.e. data for offs. 0x801 is written to 0x1 with a 2K onboard buffer
 */
	if (bufsize_rcv == 0) {
        	outb(1, IE_MMODE);      
        	outw(0, IE_GP);		
        	outb(0, IE_RBUF);	
        	for (i = 1; i < 0xFF; i++) {
                	outw(i << 8, IE_GP); 
                	outb(i, IE_RBUF);
                	outw(0x0, IE_GP); 
                	data = inb(IE_RBUF);
                	if (data == i) break;
        	}
		bufsize_rcv = i << 8;
        	outw(0, IE_GP);		
        	outb(0, IE_RBUF);	
	}
	printf("Bufsize rcv/xmt=%d/%d\n", bufsize_rcv, NI5010_BUFSIZE);
	return (1);
}

struct nic *ni5010_probe(struct nic *nic, unsigned short *probe_addrs)
{
	static unsigned short	io_addrs[] = {
		0x300, 0x320, 0x340, 0x360, 0x380, 0x3a0, 0 };
	unsigned short		*p;

	
	if (probe_addrs == 0 || *probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; p++) {
		if (ni5010_probe1(nic))
			break;
	}
	if (ioaddr == 0)
		return (0);
	ni5010_reset(nic);
	
	nic->reset = ni5010_reset;
	nic->poll = ni5010_poll;
	nic->transmit = ni5010_transmit;
	nic->disable = ni5010_disable;
	return (nic);
}
