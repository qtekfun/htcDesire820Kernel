
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include "etherboot.h"
#include "nic.h"
#ifdef	INCLUDE_LANCE
#include "pci.h"
#endif
#include "cards.h"

#if	defined(INCLUDE_NE2100) || defined(INCLUDE_LANCE)
#define	LANCE_ETH_ADDR	0x0
#define	LANCE_DATA	0x10
#define	LANCE_ADDR	0x12
#define	LANCE_RESET	0x14
#define	LANCE_BUS_IF	0x16
#define	LANCE_TOTAL_SIZE	0x18
#endif
#ifdef	INCLUDE_NI6510
#define	LANCE_ETH_ADDR	0x8
#define	LANCE_DATA	0x0
#define	LANCE_ADDR	0x2
#define	LANCE_RESET	0x4
#define	LANCE_BUS_IF	0x6
#define	LANCE_TOTAL_SIZE	0x10
#endif


#define LANCE_LOG_RX_BUFFERS	2		

#define RX_RING_SIZE		(1 << (LANCE_LOG_RX_BUFFERS))
#define RX_RING_MOD_MASK	(RX_RING_SIZE - 1)
#define RX_RING_LEN_BITS	((LANCE_LOG_RX_BUFFERS) << 29)

struct lance_init_block
{
	unsigned short	mode;
	unsigned char	phys_addr[ETH_ALEN];
	unsigned long	filter[2];
	Address		rx_ring;
	Address		tx_ring;
};

struct lance_rx_head
{
	union {
		Address		base;
		unsigned char	addr[4];
	} u;
	short		buf_length;	
	short		msg_length;
};

struct lance_tx_head
{
	union {
		Address		base;
		unsigned char	addr[4];
	} u;
	short		buf_length;	
	short		misc;
};

struct lance_interface
{
	struct lance_init_block	init_block;
	struct lance_rx_head	rx_ring[RX_RING_SIZE];
	struct lance_tx_head	tx_ring;
	unsigned char		rbuf[RX_RING_SIZE][ETH_FRAME_LEN+4];
	unsigned char		tbuf[ETH_FRAME_LEN];
	int			rx_idx;
};

#define	LANCE_MUST_PAD		0x00000001
#define	LANCE_ENABLE_AUTOSELECT	0x00000002
#define	LANCE_SELECT_PHONELINE	0x00000004
#define	LANCE_MUST_UNRESET	0x00000008

static const struct lance_chip_type
{
	int	id_number;
	const char	*name;
	int	flags;
} chip_table[] = {
	{0x0000, "LANCE 7990",			
		LANCE_MUST_PAD + LANCE_MUST_UNRESET},
	{0x0003, "PCnet/ISA 79C960",		
		LANCE_ENABLE_AUTOSELECT},
	{0x2260, "PCnet/ISA+ 79C961",		
		LANCE_ENABLE_AUTOSELECT},
	{0x2420, "PCnet/PCI 79C970",		
		LANCE_ENABLE_AUTOSELECT},
	{0x2430, "PCnet32",			
		LANCE_ENABLE_AUTOSELECT},
        {0x2621, "PCnet/PCI-II 79C970A",        
                LANCE_ENABLE_AUTOSELECT},
	{0x2625, "PCnet-FAST III 79C973",	
		LANCE_ENABLE_AUTOSELECT},
        {0x2626, "PCnet/HomePNA 79C978",        
                LANCE_ENABLE_AUTOSELECT|LANCE_SELECT_PHONELINE},
	{0x0, "PCnet (unknown)",
		LANCE_ENABLE_AUTOSELECT},
};

#undef	virt_to_bus
#define	virt_to_bus(x)		((unsigned long)x)

static int			chip_version;
static int			lance_version;
static unsigned short		ioaddr;
#ifndef	INCLUDE_LANCE
static int			dma;
#endif
static struct lance_interface	*lp;

#ifdef	USE_LOWMEM_BUFFER
#define lance ((char *)0x10000 - (sizeof(struct lance_interface)+8))
#else
static char			lance[sizeof(struct lance_interface)+8];
#endif

#ifndef	INCLUDE_LANCE

#define DMA1_CMD_REG		0x08	
#define DMA1_STAT_REG		0x08	
#define DMA1_REQ_REG            0x09    
#define DMA1_MASK_REG		0x0A	
#define DMA1_MODE_REG		0x0B	
#define DMA1_CLEAR_FF_REG	0x0C	
#define DMA1_TEMP_REG           0x0D    
#define DMA1_RESET_REG		0x0D	
#define DMA1_CLR_MASK_REG       0x0E    
#define DMA1_MASK_ALL_REG       0x0F    

#define DMA2_CMD_REG		0xD0	
#define DMA2_STAT_REG		0xD0	
#define DMA2_REQ_REG            0xD2    
#define DMA2_MASK_REG		0xD4	
#define DMA2_MODE_REG		0xD6	
#define DMA2_CLEAR_FF_REG	0xD8	
#define DMA2_TEMP_REG           0xDA    
#define DMA2_RESET_REG		0xDA	
#define DMA2_CLR_MASK_REG       0xDC    
#define DMA2_MASK_ALL_REG       0xDE    


#define DMA_MODE_READ	0x44	
#define DMA_MODE_WRITE	0x48	
#define DMA_MODE_CASCADE 0xC0   

static void enable_dma(unsigned int dmanr)
{
	if (dmanr <= 3)
		outb_p(dmanr, DMA1_MASK_REG);
	else
		outb_p(dmanr & 3, DMA2_MASK_REG);
}

static void disable_dma(unsigned int dmanr)
{
	if (dmanr <= 3)
		outb_p(dmanr | 4, DMA1_MASK_REG);
	else
		outb_p((dmanr & 3) | 4, DMA2_MASK_REG);
}

static void set_dma_mode(unsigned int dmanr, char mode)
{
	if (dmanr <= 3)
		outb_p(mode | dmanr, DMA1_MODE_REG);
	else
		outb_p(mode | (dmanr&3), DMA2_MODE_REG);
}
#endif	

static void lance_reset(struct nic *nic)
{
	int		i;
	Address		l;

	
	(void)inw(ioaddr+LANCE_RESET);
	
	if (chip_table[lance_version].flags & LANCE_MUST_UNRESET)
		outw(0, ioaddr+LANCE_RESET);
	if (chip_table[lance_version].flags & LANCE_ENABLE_AUTOSELECT)
	{
		outw(0x2, ioaddr+LANCE_ADDR);
		
		outw(inw(ioaddr+LANCE_BUS_IF) | 0x2, ioaddr+LANCE_BUS_IF);
	}
        if (chip_table[lance_version].flags & LANCE_SELECT_PHONELINE) {
                short media, check ;
                
                outw(49, ioaddr+0x12) ;
                media = inw(ioaddr+0x16) ;
#ifdef DEBUG
                printf("media was %d\n", media) ;
#endif
                media &= ~3 ;
                media |= 1 ;
#ifdef DEBUG
                printf("media changed to %d\n", media) ;
#endif
                media &= ~3 ;
                media |= 1 ;
                outw(49, ioaddr+0x12) ;
                outw(media, ioaddr+0x16) ;
                outw(49, ioaddr+0x12) ;
                check = inw(ioaddr+0x16) ;
#ifdef DEBUG
                printf("check %s, media was set properly\n", 
			check ==  media ? "passed" : "FAILED" ) ; 
#endif
	}
 
	
	
	for (i = 0; i < ETH_ALEN; ++i)
		lp->init_block.phys_addr[i] = nic->node_addr[i];
	
	for (i=0; i<RX_RING_SIZE; i++) {
		lp->rx_ring[i].buf_length = -ETH_FRAME_LEN-4;
		
		lp->rx_ring[i].u.base = virt_to_bus(lp->rbuf[i]) & 0xffffff;
		
		lp->rx_ring[i].u.addr[3] = 0x80;
	}
	lp->rx_idx = 0;
	lp->init_block.mode = 0x0;	
	l = (Address)virt_to_bus(&lp->init_block);
	outw(0x1, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw((short)l, ioaddr+LANCE_DATA);
	outw(0x2, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw((short)(l >> 16), ioaddr+LANCE_DATA);
	outw(0x4, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw(0x915, ioaddr+LANCE_DATA);
	outw(0x0, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw(0x4, ioaddr+LANCE_DATA);		
	outw(0x1, ioaddr+LANCE_DATA);		
	for (i = 10000; i > 0; --i)
		if (inw(ioaddr+LANCE_DATA) & 0x100)
			break;
#ifdef	DEBUG
	if (i <= 0)
		printf("Init timed out\n");
#endif
	outw(0x2, ioaddr+LANCE_DATA);		
}

static int lance_poll(struct nic *nic)
{
	int		status;

	status = lp->rx_ring[lp->rx_idx].u.base >> 24;
	if (status & 0x80)
		return (0);
#ifdef	DEBUG
	printf("LANCE packet received rx_ring.u.base %X mcnt %hX csr0 %hX\n",
		lp->rx_ring[lp->rx_idx].u.base, lp->rx_ring[lp->rx_idx].msg_length,
		inw(ioaddr+LANCE_DATA));
#endif
	if (status == 0x3)
		memcpy(nic->packet, lp->rbuf[lp->rx_idx], nic->packetlen = lp->rx_ring[lp->rx_idx].msg_length);
	lp->rx_ring[lp->rx_idx].buf_length = -ETH_FRAME_LEN-4;
	lp->rx_ring[lp->rx_idx].u.addr[3] |= 0x80;	

	
	outw(0x0, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw(0x500, ioaddr+LANCE_DATA);		

	
	lp->rx_idx = (lp->rx_idx + 1) & RX_RING_MOD_MASK;

	return (status == 0x3);
}

static void lance_transmit(
	struct nic *nic,
	const char *d,			
	unsigned int t,			
	unsigned int s,			
	const char *p)			
{
	unsigned long		time;

	
	memcpy(lp->tbuf, d, ETH_ALEN);	
	memcpy(&lp->tbuf[ETH_ALEN], nic->node_addr, ETH_ALEN); 
	lp->tbuf[ETH_ALEN+ETH_ALEN] = t >> 8;	
	lp->tbuf[ETH_ALEN+ETH_ALEN+1] = t;	
	memcpy(&lp->tbuf[ETH_HLEN], p, s);
	s += ETH_HLEN;
	if (chip_table[chip_version].flags & LANCE_MUST_PAD)
		while (s < ETH_ZLEN)	
			lp->tbuf[s++] = 0;
	lp->tx_ring.buf_length = -s;
	lp->tx_ring.misc = 0x0;
	
	lp->tx_ring.u.base = virt_to_bus(lp->tbuf) & 0xffffff;
	
	lp->tx_ring.u.addr[3] = 0x83;
	
	outw(0x0, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);	
	outw(0x08, ioaddr+LANCE_DATA);
	
	time = currticks() + TICKS_PER_SEC;		
	while (currticks() < time && (lp->tx_ring.u.base & 0x80000000) != 0)
		;
	if ((lp->tx_ring.u.base & 0x80000000) != 0)
		printf("LANCE timed out on transmit\n");
	(void)inw(ioaddr+LANCE_ADDR);
	outw(0x200, ioaddr+LANCE_DATA);		
#ifdef	DEBUG
	printf("tx_ring.u.base %X tx_ring.buf_length %hX tx_ring.misc %hX csr0 %hX\n",
		lp->tx_ring.u.base, lp->tx_ring.buf_length, lp->tx_ring.misc,
		inw(ioaddr+LANCE_DATA));
#endif
}

static void lance_disable(struct nic *nic)
{
	(void)inw(ioaddr+LANCE_RESET);
	if (chip_table[lance_version].flags & LANCE_MUST_UNRESET)
		outw(0, ioaddr+LANCE_RESET);

	outw(0, ioaddr+LANCE_ADDR);
	outw(0x0004, ioaddr+LANCE_DATA);	

#ifndef	INCLUDE_LANCE
	disable_dma(dma);
#endif
}

#ifdef	INCLUDE_LANCE
static int lance_probe1(struct nic *nic, struct pci_device *pci)
#else
static int lance_probe1(struct nic *nic)
#endif
{
	int			reset_val ;
	unsigned int		i;
	Address			l;
	short			dma_channels;
#ifndef	INCLUDE_LANCE
	static const char	dmas[] = { 5, 6, 7, 3 };
#endif

	reset_val = inw(ioaddr+LANCE_RESET);
	outw(reset_val, ioaddr+LANCE_RESET);
#if	1  
	outw(0x0, ioaddr+LANCE_ADDR);	
	if (inw(ioaddr+LANCE_DATA) != 0x4)
		return (-1);
#endif
	outw(88, ioaddr+LANCE_ADDR);	
	if (inw(ioaddr+LANCE_ADDR) != 88)
		lance_version = 0;
	else
	{
		chip_version = inw(ioaddr+LANCE_DATA);
		outw(89, ioaddr+LANCE_ADDR);
		chip_version |= inw(ioaddr+LANCE_DATA) << 16;
		if ((chip_version & 0xfff) != 0x3)
			return (-1);
		chip_version = (chip_version >> 12) & 0xffff;
		for (lance_version = 1; chip_table[lance_version].id_number != 0; ++lance_version)
			if (chip_table[lance_version].id_number == chip_version)
				break;
	}
	
	l = ((Address)lance + 7) & ~7;
	lp = (struct lance_interface *)l;
	lp->init_block.mode = 0x3;	
	lp->init_block.filter[0] = lp->init_block.filter[1] = 0x0;
	
	lp->init_block.rx_ring = (virt_to_bus(&lp->rx_ring) & 0xffffff) | RX_RING_LEN_BITS;
	lp->init_block.tx_ring = virt_to_bus(&lp->tx_ring) & 0xffffff;
	l = virt_to_bus(&lp->init_block);
	outw(0x1, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw((unsigned short)l, ioaddr+LANCE_DATA);
	outw(0x2, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw((unsigned short)(l >> 16), ioaddr+LANCE_DATA);
	outw(0x4, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	outw(0x915, ioaddr+LANCE_DATA);
	outw(0x0, ioaddr+LANCE_ADDR);
	(void)inw(ioaddr+LANCE_ADDR);
	
	for (i = 0; i < ETH_ALEN; ++i) {
		nic->node_addr[i] = inb(ioaddr+LANCE_ETH_ADDR+i);
	}
#ifndef	INCLUDE_LANCE
	
	dma_channels = ((inb(DMA1_STAT_REG) >> 4) & 0xf) |
		(inb(DMA2_STAT_REG) & 0xf0);
	
	for (i = 0; i < (sizeof(dmas)/sizeof(dmas[0])); ++i)
	{
		int		j;

		dma = dmas[i];
		if (dma_channels & (1 << dma))
			continue;
		outw(0x7f04, ioaddr+LANCE_DATA);	
		set_dma_mode(dma, DMA_MODE_CASCADE);
		enable_dma(dma);
		outw(0x1, ioaddr+LANCE_DATA);		
		for (j = 100; j > 0; --j)
			if (inw(ioaddr+LANCE_DATA) & 0x900)
				break;
		if (inw(ioaddr+LANCE_DATA) & 0x100)
			break;
		else
			disable_dma(dma);
	}
	if (i >= (sizeof(dmas)/sizeof(dmas[0])))
		dma = 0;
	printf("\n%s base %#X, DMA %d, addr %!\n",
		chip_table[lance_version].name, ioaddr, dma, nic->node_addr);
#else
	printf(" %s base %#hX, addr %!\n", chip_table[lance_version].name, ioaddr, nic->node_addr);
#endif
	if (chip_table[chip_version].flags & LANCE_ENABLE_AUTOSELECT) {
		outw(0x0002, ioaddr+LANCE_ADDR);
		
		outw(inw(ioaddr+LANCE_BUS_IF) | 0x0002, ioaddr+LANCE_BUS_IF);
	}
	return (lance_version);
}


#ifdef	INCLUDE_LANCE
struct nic *lancepci_probe(struct nic *nic, unsigned short *probe_addrs, struct pci_device *pci)
#endif
#ifdef	INCLUDE_NE2100
struct nic *ne2100_probe(struct nic *nic, unsigned short *probe_addrs)
#endif
#ifdef	INCLUDE_NI6510
struct nic *ni6510_probe(struct nic *nic, unsigned short *probe_addrs)
#endif
{
	unsigned short		*p;
#ifndef	INCLUDE_LANCE
	static unsigned short	io_addrs[] = { 0x300, 0x320, 0x340, 0x360, 0 };
#endif

	
	if (probe_addrs == 0) {
#ifdef	INCLUDE_LANCE
		return 0;
#else
		probe_addrs = io_addrs;
#endif
	}
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
	{
		char	offset15, offset14 = inb(ioaddr + 14);
		unsigned short	pci_cmd;

#ifdef	INCLUDE_NE2100
		if ((offset14 == 0x52 || offset14 == 0x57) &&
		 ((offset15 = inb(ioaddr + 15)) == 0x57 || offset15 == 0x44))
			if (lance_probe1(nic) >= 0)
				break;
#endif
#ifdef	INCLUDE_NI6510
		if ((offset14 == 0x00 || offset14 == 0x52) &&
		 ((offset15 = inb(ioaddr + 15)) == 0x55 || offset15 == 0x44))
			if (lance_probe1(nic) >= 0)
				break;
#endif
#ifdef	INCLUDE_LANCE
		adjust_pci_device(pci);
		if (lance_probe1(nic, pci) >= 0)
			break;
#endif
	}
	
	if (ioaddr != 0)
	{
		
		lance_reset(nic);
		nic->reset = lance_reset;
		nic->poll = lance_poll;
		nic->transmit = lance_transmit;
		nic->disable = lance_disable;
		return nic;
	}

	
	return 0;
}
