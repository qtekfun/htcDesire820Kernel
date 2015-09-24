
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#include "etherboot.h"
#include "nic.h"
#include "cards.h"
#include "timer.h"

#define	udelay(n)	waiton_timer2(((n)*TICKS_PER_MS)/1000)




#define	CUC_START	0x0100
#define	CUC_RESUME	0x0200
#define	CUC_SUSPEND	0x0300
#define	RX_START	0x0010
#define	RX_RESUME	0x0020
#define	RX_SUSPEND	0x0030

/* The Rx unit uses a list of frame descriptors and a list of data buffer
   descriptors.  We use full-sized (1518 byte) data buffers, so there is
   a one-to-one pairing of frame descriptors to buffer descriptors.

   The Tx ("command") unit executes a list of commands that look like:
	Status word	Written by the 82586 when the command is done.
	Command word	Command in lower 3 bits, post-command action in upper 3
	Link word	The address of the next command.
	Parameters	(as needed).

	Some definitions related to the Command Word are:
 */
#define CMD_EOL		0x8000		
#define CMD_SUSP	0x4000		
#define CMD_INTR	0x2000		

enum commands {
	CmdNOp = 0, CmdSASetup = 1, CmdConfigure = 2, CmdMulticastList = 3,
	CmdTx = 4, CmdTDR = 5, CmdDump = 6, CmdDiagnose = 7};



#ifdef	INCLUDE_3C507

#define	SA_DATA		0	
#define MISC_CTRL	6	
#define RESET_IRQ	10	
#define I82586_ATTN	11	
#define ROM_CONFIG	13
#define MEM_CONFIG	14
#define IRQ_CONFIG	15
#define EL16_IO_EXTENT	16

#define ID_PORT		0x100

#endif

#ifdef	INCLUDE_NI5210

#define	NI52_RESET	0  
#define	I82586_ATTN	1  

#endif

#ifdef	INCLUDE_EXOS205

#define	EXOS205_RESET	0  
#define	I82586_ATTN	1  

#endif

#define iSCB_STATUS	0x8
#define iSCB_CMD	0xA
#define iSCB_CBL	0xC	
#define iSCB_RFA	0xE	



#define CONFIG_CMD	0x18
#define SET_SA_CMD	0x24
#define SA_OFFSET	0x2A
#define IDLELOOP	0x30
#define TDR_CMD		0x38
#define TDR_TIME	0x3C
#define DUMP_CMD	0x40
#define DIAG_CMD	0x48
#define SET_MC_CMD	0x4E
#define DUMP_DATA	0x56	

#define TX_BUF_START	0x0100
#define TX_BUF_SIZE	(1518+14+20+16) 

#define RX_BUF_START	0x1000
#define RX_BUF_SIZE	(1518+14+18)	
#define RX_BUF_END	(mem_end - mem_start - 20)


static unsigned short init_words[] = {
	
#if	defined(INCLUDE_3C507)
	0x0000,					
#else
	0x0001,					
#endif
	0,0,					
	0x0000,0x0000,				

	
	0x0001,					
	0x0008,0,0,				

	
	0,0xf000|RX_START|CUC_START,		
	CONFIG_CMD,				
	RX_BUF_START,				
	0,0,0,0,				

	
	0, CmdConfigure,			
	SET_SA_CMD,				
	0x0804,					
	0x2e40,					
	0,					

	
	0, CmdSASetup,
	SET_MC_CMD,				
	0xaa00,0xb000,0x0bad,	

	
	0, CmdNOp, IDLELOOP, 0 ,

	
	0, CmdTDR, IDLELOOP, 0,

	
	0, CmdDump, IDLELOOP, DUMP_DATA,

	
	0, CmdDiagnose, IDLELOOP,

	
	0, CmdMulticastList, IDLELOOP, 0,
};


static unsigned short		ioaddr, irq, scb_base;
static Address			mem_start, mem_end;
static unsigned short		rx_head, rx_tail;

#define	read_mem(m,s)	fmemcpy((char *)s, m, sizeof(s))

static void setup_rx_buffers(struct nic *nic)
{
	Address			write_ptr;
	unsigned short		cur_rx_buf;
	static unsigned short	rx_cmd[16] = {
		0x0000,			
		0x0000,			
		RX_BUF_START,		
		RX_BUF_START + 22,	
		0x0000, 0x0000, 0x0000,	
		0x0000, 0x0000, 0x0000,	
		0x0000,			
		0x0000,			
		-1,			
		RX_BUF_START + 0x20,	
		0x0000,			
		0x8000 | (RX_BUF_SIZE - 0x20)
	};

	cur_rx_buf = rx_head = RX_BUF_START;
	do {		
		write_ptr = mem_start + cur_rx_buf;
		
		rx_cmd[1] = 0x0000;
		rx_cmd[2] = cur_rx_buf + RX_BUF_SIZE;
		rx_cmd[3] = cur_rx_buf + 22;
		rx_cmd[13] = cur_rx_buf + 0x20 + scb_base;
		memcpy((char *)write_ptr, (char *)rx_cmd, sizeof(rx_cmd));
		rx_tail = cur_rx_buf;
		cur_rx_buf += RX_BUF_SIZE;
	} while (cur_rx_buf <= RX_BUF_END - RX_BUF_SIZE);
	write_ptr = mem_start + rx_tail;
	rx_cmd[1] = 0xC000;
	rx_cmd[2] = rx_head;
	memcpy((char *)write_ptr, (char *)rx_cmd, sizeof(unsigned short) * 3);
}

static void ack_status(void)
{
	unsigned short	cmd, status;
	unsigned short	*shmem = (short *)mem_start;

	cmd = (status = shmem[iSCB_STATUS>>1]) & 0xf000;
	if (status & 0x100)		
		cmd |= CUC_RESUME;
	if ((status & 0x200) == 0)	
		cmd |= CUC_START;
	if (status & 0x010)		
		cmd |= RX_RESUME;
	else if ((status & 0x040) == 0)	
		cmd |= RX_START;
	if (cmd == 0)			
		return;
	shmem[iSCB_CMD>>1] = cmd;
#if	defined(DEBUG)
	printf("Status %hX Command %hX\n", status, cmd);
#endif
	outb(0, ioaddr + I82586_ATTN);
}


static void i82586_reset(struct nic *nic)
{
	unsigned long	time;
	unsigned short	*shmem = (short *)mem_start;

	

#ifdef	INCLUDE_3C507
	outb(0x20, ioaddr + MISC_CTRL);
#endif

	
	init_words[3] = scb_base;
	init_words[7] = scb_base;

	
	
	
	memcpy((char *)(mem_end - 10), (char *)init_words, 10);
	memcpy((char *)mem_start, (char *)&init_words[5], sizeof(init_words) - 10);
	memcpy((char *)mem_start + SA_OFFSET, nic->node_addr, ETH_ALEN);
	setup_rx_buffers(nic);

#ifdef	INCLUDE_3C507
	
	outb(0xA0, ioaddr + MISC_CTRL);
#endif

	outb(0, ioaddr + I82586_ATTN);
	time = currticks() + TICKS_PER_SEC;	
	while (
			shmem[iSCB_STATUS>>1] == 0)
	{
		if (currticks() > time)
		{
			printf("i82586 initialisation timed out with status %hX, cmd %hX\n",
					shmem[iSCB_STATUS>>1], shmem[iSCB_CMD>>1]);
			break;
		}
	}
	
	outb(0, ioaddr + I82586_ATTN);

#ifdef	INCLUDE_3C507
	
	outb(0x80, ioaddr + MISC_CTRL);
#endif
#if	defined(DEBUG)
	printf("i82586 status %hX, cmd %hX\n",
			shmem[iSCB_STATUS>>1], shmem[iSCB_CMD>>1]);
#endif
}

static int i82586_poll(struct nic *nic)
{
	int		status;
	unsigned short	rfd_cmd, next_rx_frame, data_buffer_addr,
	frame_status, pkt_len;
	unsigned short	*shmem = (short *)mem_start + rx_head;

	
	if (
			((frame_status = shmem[0]) & 0x8000) == 0)
		return (0);		
	rfd_cmd = shmem[1];
	next_rx_frame = shmem[2];
	data_buffer_addr = shmem[3];
	pkt_len = shmem[11];
	status = 0;
	if (rfd_cmd != 0 || data_buffer_addr != rx_head + 22
			|| (pkt_len & 0xC000) != 0xC000)
		printf("\nRx frame corrupt, discarded");
	else if ((frame_status & 0x2000) == 0)
		printf("\nRx frame had error");
	else
	{
		
		pkt_len &= 0x3FFF;
		memcpy(nic->packet, (char *)mem_start + rx_head + 0x20, pkt_len);
		
		if (memcmp(nic->packet + ETH_ALEN, nic->node_addr, ETH_ALEN) != 0)
		{
			nic->packetlen = pkt_len;
			status = 1;
		}
	}
	
	shmem[0] = 0;
	shmem[1] = 0xC000;
	*(short *)(mem_start + rx_tail + 2) = 0;
	rx_tail = rx_head;
	rx_head = next_rx_frame;
	ack_status();
	return (status);
}

static void i82586_transmit(
		struct nic *nic,
		const char *d,			
		unsigned int t,			
		unsigned int s,			
		const char *p)			
{
	Address			bptr;
	unsigned short		type, z;
	static unsigned short	tx_cmd[11] = {
		0x0,			
		CmdTx,			
		TX_BUF_START+16,	
		TX_BUF_START+8,		
		0x8000,			
		0xffff,			
		TX_BUF_START+22,	
		0x0,			
		0x0,			
		CmdNOp,			
		TX_BUF_START+16		
	};
	unsigned short	*shmem = (short *)mem_start + TX_BUF_START;

	
	
	type = htons(t);
	if (s < ETH_ZLEN)
		s = ETH_ZLEN;
	tx_cmd[4] = (s + ETH_HLEN) | 0x8000;
	tx_cmd[6] = TX_BUF_START + 22 + scb_base;
	bptr = mem_start + TX_BUF_START;
	memcpy((char *)bptr, (char *)tx_cmd, sizeof(tx_cmd));
	bptr += sizeof(tx_cmd);
	memcpy((char *)bptr, d, ETH_ALEN);
	bptr += ETH_ALEN;
	memcpy((char *)bptr, nic->node_addr, ETH_ALEN);
	bptr += ETH_ALEN;
	memcpy((char *)bptr, (char *)&type, sizeof(type));
	bptr += sizeof(type);
	memcpy((char *)bptr, p, s);
	
	*(unsigned short *)(mem_start + IDLELOOP + 4) = TX_BUF_START;
	
	while (
			(shmem[0] & 0x2000) == 0)
		;
	*(unsigned short *)(mem_start + IDLELOOP + 4) = IDLELOOP;
	*(unsigned short *)(mem_start + TX_BUF_START + 20) = IDLELOOP;
	ack_status();
}

static void i82586_disable(struct nic *nic)
{
	unsigned short	*shmem = (short *)mem_start;

#if	0
	
	shmem[iSCB_CMD>>1] = RX_SUSPEND | CUC_SUSPEND;
	outb(0, ioaddr + I82586_ATTN);
#ifdef	INCLUDE_NI5210
	outb(0, ioaddr + NI52_RESET);
#endif
#endif	
}

#ifdef	INCLUDE_3C507

static int t507_probe1(struct nic *nic, unsigned short ioaddr)
{
	int			i;
	Address			size;
	char			mem_config;
	char			if_port;

	if (inb(ioaddr) != '*' || inb(ioaddr+1) != '3'
		|| inb(ioaddr+2) != 'C' || inb(ioaddr+3) != 'O')
		return (0);
	irq = inb(ioaddr + IRQ_CONFIG) & 0x0f;
	mem_config = inb(ioaddr + MEM_CONFIG);
	if (mem_config & 0x20)
	{
		size = 65536L;
		mem_start = 0xf00000L + (mem_config & 0x08 ? 0x080000L
			: (((Address)mem_config & 0x3) << 17));
	}
	else
	{
		size = ((((Address)mem_config & 0x3) + 1) << 14);
		mem_start = 0x0c0000L + (((Address)mem_config & 0x18) << 12);
	}
	mem_end = mem_start + size;
	scb_base = 65536L - size;
	if_port = inb(ioaddr + ROM_CONFIG) & 0x80;
	
	outb(0x01, ioaddr + MISC_CTRL);
	for (i = 0; i < ETH_ALEN; ++i)
	{
		nic->node_addr[i] = inb(ioaddr+i);
	}
	printf("\n3c507 ioaddr %#hX, IRQ %d, mem [%#X-%#X], %sternal xcvr, addr %!\n",
		ioaddr, irq, mem_start, mem_end, if_port ? "in" : "ex", nic->node_addr);
	return (1);
}


struct nic *t507_probe(struct nic *nic, unsigned short *probe_addrs)
{
	static unsigned char	init_ID_done = 0;
	unsigned short		lrs_state = 0xff;
	static unsigned short	io_addrs[] = { 0x300, 0x320, 0x340, 0x280, 0 };
	unsigned short		*p;
	int			i;

	if (init_ID_done == 0)
	{
		
		outb(0x00, ID_PORT);
		for (i = 0; i < 255; ++i)
		{
			outb(lrs_state, ID_PORT);
			lrs_state <<= 1;
			if (lrs_state & 0x100)
				lrs_state ^= 0xe7;
		}
		outb(0x00, ID_PORT);
		init_ID_done = 1;
	}
	
	if (probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
		if (t507_probe1(nic, ioaddr))
			break;
	if (ioaddr != 0)
	{
		
		i82586_reset(nic);
		nic->reset = i82586_reset;
		nic->poll = i82586_poll;
		nic->transmit = i82586_transmit;
		nic->disable = i82586_disable;
		return nic;
	}
	
	{
		return 0;
	}
}

#endif

#ifdef	INCLUDE_NI5210

static int ni5210_probe2(void)
{
	unsigned short		i;
	unsigned short		shmem[10];

	
	init_words[3] = scb_base;
	init_words[7] = scb_base;

	
	
	memcpy((char *)(mem_end - 10), (char *)init_words, 10);
	memcpy((char *)mem_start, (char *)&init_words[5], sizeof(init_words) - 10);
	if (*(unsigned short *)mem_start != 1)
		return (0);
	outb(0, ioaddr + NI52_RESET);
	outb(0, ioaddr + I82586_ATTN);
	udelay(32);
	i = 50;
	while (
		shmem[iSCB_STATUS>>1] == 0)
	{
		if (--i == 0)
		{
			printf("i82586 initialisation timed out with status %hX, cmd %hX\n",
				shmem[iSCB_STATUS>>1], shmem[iSCB_CMD>>1]);
			break;
		}
	}
	
	outb(0, ioaddr + I82586_ATTN);
	if (*(unsigned short *)mem_start != 0)
		return (0);
	return (1);
}

static int ni5210_probe1(struct nic *nic)
{
	int			i;
	static Address		mem_addrs[] = {
		0xc0000, 0xc4000, 0xc8000, 0xcc000,
		0xd0000, 0xd4000, 0xd8000, 0xdc000,
		0xe0000, 0xe4000, 0xe8000, 0xec000,
		0 };
	Address			*p;

	if (inb(ioaddr + 6) != 0x0 || inb(ioaddr + 7) != 0x55)
		return (0);
	scb_base = -8192;		
	for (p = mem_addrs; (mem_start = *p) != 0; ++p)
		if (mem_end = mem_start + 8192, ni5210_probe2())
			break;
	if (mem_start == 0)
		return (0);
	
	for (i = 0; i < ETH_ALEN; ++i)
	{
		nic->node_addr[i] = inb(ioaddr+i);
	}
	printf("\nNI5210 ioaddr %#hX, mem [%#X-%#X], addr %!\n",
		ioaddr, mem_start, mem_end, nic->node_addr);
	return (1);
}

struct nic *ni5210_probe(struct nic *nic, unsigned short *probe_addrs)
{
	
	static unsigned short	io_addrs[] = {
		0x200, 0x208, 0x210, 0x218, 0x220, 0x228, 0x230, 0x238,
		0x240, 0x248, 0x250, 0x258, 0x260, 0x268, 0x270, 
		0x280, 0x288, 0x290, 0x298, 0x2A0, 0x2A8, 0x2B0, 0x2B8,
		0x2C0, 0x2C8, 0x2D0, 0x2D8, 0x2E0, 0x2E8, 0x2F0, 
		0x300, 0x308, 0x310, 0x318, 0x320, 0x328, 0x330, 0x338,
		0x340, 0x348, 0x350, 0x358, 0x360, 0x368, 0x370, 
		0x380, 0x388, 0x390, 0x398, 0x3A0, 0x3A8, 
		0x3C0, 0x3C8, 0x3D0, 0x3D8, 0x3E0, 0x3E8, 
		0x0
	};
	unsigned short		*p;
	int			i;

	
	if (probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
		if (ni5210_probe1(nic))
			break;
	if (ioaddr != 0)
	{
		
		i82586_reset(nic);
		nic->reset = i82586_reset;
		nic->poll = i82586_poll;
		nic->transmit = i82586_transmit;
		nic->disable = i82586_disable;
		return nic;
	}
	
	{
		return 0;
	}
}
#endif

#ifdef	INCLUDE_EXOS205


static unsigned char exos_i186_init[] =
{
0x08,0x00,0x14,0x00,0x00,0x00,0xaa,0xfa,0x33,0xc0,0xba,0xfe,0xff,0xef,0xb8,0xf8,
0xff,0xe7,0xa0,0xb8,0x7c,0x00,0xe7,0xa4,0xb8,0xbc,0x80,0xe7,0xa8,0x8c,0xc8,0x8e,
0xd8,0xbb,0x2f,0x0e,0xc6,0x07,0xa5,0x33,0xc9,0xeb,0x00,0xeb,0x00,0xeb,0x00,0xe2,
0xf8,0xbe,0x2c,0x0e,0xba,0x02,0x05,0x33,0xdb,0xb9,0x03,0x00,0xec,0x24,0x0f,0x8a,
0xe0,0x02,0xd8,0x42,0x42,0xec,0x02,0xd8,0xd0,0xe0,0xd0,0xe0,0xd0,0xe0,0xd0,0xe0,
0x0a,0xc4,0x88,0x04,0x42,0x42,0x46,0xe2,0xe3,0x8a,0xe3,0xd0,0xec,0xd0,0xec,0xd0,
0xec,0xd0,0xec,0x80,0xe3,0x0f,0x02,0xe3,0x80,0xf4,0x05,0xec,0x3a,0xe0,0x74,0x05,
0xc6,0x04,0x5a,0xeb,0xfe,0xc6,0x04,0x55,0x33,0xc0,0x8e,0xd8,0xbe,0x38,0x00,0xc7,
0x04,0xce,0x0e,0x46,0x46,0xc7,0x04,0x00,0xff,0xfb,0xba,0x3c,0x00,0xb8,0x03,0x00,
0xef,0x33,0xdb,0x33,0xc9,0xbd,0x04,0x0f,0x90,0x90,0x90,0x90,0xe2,0xfa,0x43,0x2e,
0x89,0x5e,0x00,0xeb,0xf3,0x52,0xba,0x00,0x06,0xef,0x50,0x53,0x55,0xbd,0xf8,0x0e,
0x2e,0x8b,0x5e,0x00,0x43,0x2e,0x89,0x5e,0x00,0xba,0x22,0x00,0xb8,0x00,0x80,0xef,
0x5d,0x5b,0x58,0x5a,0xcf,0x49,0x4e,0x54,0x52,0x20,0x63,0x6e,0x74,0x2d,0x3e,0x00,
0x00,0x4c,0x4f,0x4f,0x50,0x20,0x63,0x6e,0x74,0x2d,0x3e,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xea,0x30,0x0e,0x00,0xff,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,00
};


#define	OFFSET_SEMA		0x1D1
#define	OFFSET_ADDR		0x1D7

static int exos205_probe2(void)
{
	unsigned short		i;
	unsigned short		shmem[10];

	
	init_words[3] = scb_base;
	init_words[7] = scb_base;

	
	
	memcpy((char *)(mem_end - 10), (char *)init_words, 10);
	memcpy((char *)mem_start, (char *)&init_words[5], sizeof(init_words) - 10);
	if (*(unsigned short *)mem_start != 1)
		return (0);
	outb(0, ioaddr + EXOS205_RESET);
	outb(0, ioaddr + I82586_ATTN);
	i = 50;
	while (
		shmem[iSCB_STATUS>>1] == 0)
	{
		if (--i == 0)
		{
			printf("i82586 initialisation timed out with status %hX, cmd %hX\n",
				shmem[iSCB_STATUS>>1], shmem[iSCB_CMD>>1]);
			break;
		}
	}
	
	outb(0, ioaddr + I82586_ATTN);
	if (*(unsigned short *)mem_start != 0)
		return (0);
	return (1);
}

static int exos205_probe1(struct nic *nic)
{
	int			i;
	
	static Address		mem_addrs[] = {
		0xcc000, 0 };
	Address			*p;

	scb_base = -16384;		
	for (p = mem_addrs; (mem_start = *p) != 0; ++p)
		if (mem_end = mem_start + 16384, exos205_probe2())
			break;
	if (mem_start == 0)
		return (0);
	
	for (i = 0; i < ETH_ALEN; ++i)
	{
		nic->node_addr[i] = inb(ioaddr+i);
	}
	printf("\nEXOS205 ioaddr %#hX, mem [%#X-%#X], addr %!\n",
		ioaddr, mem_start, mem_end, nic->node_addr);
	return (1);
}

struct nic *exos205_probe(struct nic *nic, unsigned short *probe_addrs)
{
	
	static unsigned short	io_addrs[] = {
		0x310, 0x0
	};
	unsigned short		*p;
	int			i;

	
	if (probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
		if (exos205_probe1(nic))
			break;
	if (ioaddr != 0)
	{
		
		i82586_reset(nic);
		nic->reset = i82586_reset;
		nic->poll = i82586_poll;
		nic->transmit = i82586_transmit;
		nic->disable = i82586_disable;
		return nic;
	}
	
	{
		return 0;
	}
}

#endif