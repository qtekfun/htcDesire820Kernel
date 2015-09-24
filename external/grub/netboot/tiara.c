/**************************************************************************
Etherboot -  BOOTP/TFTP Bootstrap Program

TIARA (Fujitsu Etherstar) NIC driver for Etherboot
Copyright (c) Ken Yap 1998

Information gleaned from:

TIARA.ASM Packet driver by Brian Fisher, Queens U, Kingston, Ontario
Fujitsu MB86960 spec sheet (different chip but same family)
***************************************************************************/

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include "etherboot.h"
#include "nic.h"
#include "cards.h"


#define	DLCR_XMIT_STAT	0x00
#define	DLCR_XMIT_MASK	0x01
#define	DLCR_RECV_STAT	0x02
#define	DLCR_RECV_MASK	0x03
#define	DLCR_XMIT_MODE	0x04
#define	DLCR_RECV_MODE	0x05
#define	DLCR_ENABLE	0x06
#define	DLCR_TDR_LOW	0x07
#define	DLCR_NODE_ID	0x08
#define	DLCR_TDR_HIGH	0x0F
#define	BMPR_MEM_PORT	0x10
#define	BMPR_PKT_LEN	0x12
#define	BMPR_DMA_ENABLE	0x14
#define	PROM_ID		0x18

#define	TMST		0x80
#define	TMT_OK		0x80
#define	TMT_16COLL	0x02
#define	BUF_EMPTY	0x40

#define	CARD_DISABLE	0x80	/* written to DLCR_ENABLE to disable card */
#define	CARD_ENABLE	0	/* written to DLCR_ENABLE to enable card */

#define	CLEAR_STATUS	0x0F	

#define	NO_TX_IRQS	0	/* written to clear transmit IRQs */

#define	CLR_RCV_STATUS	0xCF	

#define	EN_RCV_IRQS	0x80	

#define	XMIT_MODE	0x02

#define	RECV_MODE	0x02


static unsigned short	ioaddr;

static void tiara_reset(struct nic *nic)
{
	int		i;

	outb(CARD_DISABLE, ioaddr + DLCR_ENABLE);
	outb(CLEAR_STATUS, ioaddr + DLCR_XMIT_STAT);
	outb(NO_TX_IRQS, ioaddr + DLCR_XMIT_MASK);
	outb(CLR_RCV_STATUS, ioaddr + DLCR_RECV_STAT);
	outb(XMIT_MODE, ioaddr + DLCR_XMIT_MODE);
	outb(RECV_MODE, ioaddr + DLCR_RECV_MODE);
	
	while ((inb(ioaddr + DLCR_RECV_MODE) & BUF_EMPTY) == 0)
		inb(ioaddr + BMPR_MEM_PORT);
	
	for (i = 0; i < ETH_ALEN; ++i)
		outb(nic->node_addr[i], ioaddr + DLCR_NODE_ID + i);
	outb(CLR_RCV_STATUS, ioaddr + DLCR_RECV_STAT);
	outb(CARD_ENABLE, ioaddr + DLCR_ENABLE);
}

static int tiara_poll(struct nic *nic)
{
	unsigned int		len;

	if (inb(ioaddr + DLCR_RECV_MODE) & BUF_EMPTY)
		return (0);
	
	outw(CLR_RCV_STATUS, ioaddr + DLCR_RECV_STAT);
	len = inw(ioaddr + BMPR_MEM_PORT);		
	len = inw(ioaddr + BMPR_MEM_PORT);
	
	if (len > ETH_FRAME_LEN)
		return (0);		
	insw(ioaddr + BMPR_MEM_PORT, nic->packet, len / 2);
	
	if (memcmp(nic->packet + ETH_ALEN, nic->node_addr, ETH_ALEN) == 0)
		return (0);
	nic->packetlen = len;
	return (1);
}

static void tiara_transmit(
struct nic *nic,
const char *d,			
unsigned int t,			
unsigned int s,			
const char *p)			
{
	unsigned int	len;
	unsigned long	time;

	len = s + ETH_HLEN;
	if (len < ETH_ZLEN)
		len = ETH_ZLEN;
	t = htons(t);
	outsw(ioaddr + BMPR_MEM_PORT, d, ETH_ALEN / 2);
	outsw(ioaddr + BMPR_MEM_PORT, nic->node_addr, ETH_ALEN / 2);
	outw(t, ioaddr + BMPR_MEM_PORT);
	outsw(ioaddr + BMPR_MEM_PORT, p, s / 2);
	if (s & 1)					
		outb(p[s-1], ioaddr + BMPR_MEM_PORT);
	while (s++ < ETH_ZLEN - ETH_HLEN)	
		outb(0, ioaddr + BMPR_MEM_PORT);
	outw(len | (TMST << 8), ioaddr + BMPR_PKT_LEN);
	
	time = currticks() + TICKS_PER_SEC;		
	while (currticks() < time && (inb(ioaddr) & (TMT_OK|TMT_16COLL)) == 0)
		;
	if ((inb(ioaddr) & (TMT_OK|TMT_16COLL)) == 0)
		printf("Tiara timed out on transmit\n");
	
}

static void tiara_disable(struct nic *nic)
{
	
	outb(CARD_DISABLE, ioaddr + DLCR_ENABLE);
}

static int tiara_probe1(struct nic *nic)
{
	
	static char	vendor_prefix[] = { 0x08, 0x00, 0x1A };
	static char	all_ones[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	int		i;

	for (i = 0; i < ETH_ALEN; ++i)
		nic->node_addr[i] = inb(ioaddr + PROM_ID + i);
	if (memcmp(nic->node_addr, vendor_prefix, sizeof(vendor_prefix)) != 0)
		return (0);
	if (memcmp(nic->node_addr, all_ones, sizeof(all_ones)) == 0)
		return (0);
	printf("\nTiara ioaddr %#hX, addr %!\n", ioaddr, nic->node_addr);
	return (1);
}

struct nic *tiara_probe(struct nic *nic, unsigned short *probe_addrs)
{
	
	static unsigned short	io_addrs[] = {
		0x100, 0x120, 0x140, 0x160,
		0x180, 0x1A0, 0x1C0, 0x1E0,
		0x200, 0x220, 0x240, 
		0x280, 0x2A0, 0x2C0, 
		0x300, 0x320, 0x340, 
		0x380,  0x3C0, 
		0x0
	};
	unsigned short		*p;

	
	if (probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
		if (tiara_probe1(nic))
			break;
	
	if (ioaddr != 0)
	{
		tiara_reset(nic);
		
		nic->reset = tiara_reset;
		nic->poll = tiara_poll;
		nic->transmit = tiara_transmit;
		nic->disable = tiara_disable;
		return nic;
	}
	else
		return (0);
}
