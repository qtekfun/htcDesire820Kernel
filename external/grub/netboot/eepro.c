
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

#undef	DEBUG		

#define LAN595		0
#define LAN595TX	1
#define LAN595FX	2
#define LAN595FX_10ISA	3

#define	SLOW_DOWN	inb(0x80);

#define SA_ADDR0 0x00	
#define SA_ADDR1 0xaa
#define SA_ADDR2 0x00

#define GetBit(x,y) ((x & (1<<y))>>y)

#define ee_PnP       0  
#define ee_Word1     1  
#define ee_BusWidth  2  
#define ee_FlashAddr 3  
#define ee_FlashMask 0x7   
#define ee_AutoIO    6  
#define ee_reserved0 7  
#define ee_Flash     8  
#define ee_AutoNeg   9  
#define ee_IO0       10 
#define ee_IO0Mask   0x 
#define ee_IO1       15 

#define ee_IntSel    0   
#define ee_IntMask   0x7
#define ee_LI        3   
#define ee_PC        4   
#define ee_TPE_AUI   5   
#define ee_Jabber    6   
#define ee_AutoPort  7   
#define ee_SMOUT     8   
#define ee_PROM      9   
#define ee_reserved1 10  
#define ee_AltReady  13  
#define ee_reserved2 14  
#define ee_Duplex    15

#define ee_IA5       0 
#define ee_IA4       8 
#define ee_IA3       0 
#define ee_IA2       8 
#define ee_IA1       0 
#define ee_IA0       8 

#define ee_BNC_TPE   0 
#define ee_BootType  1 
#define ee_BootTypeMask 0x3 
#define ee_NumConn   3  
#define ee_FlashSock 4  
#define ee_PortTPE   5
#define ee_PortBNC   6
#define ee_PortAUI   7
#define ee_PowerMgt  10 
#define ee_CP        13 
#define ee_CPMask    0x7

#define ee_Stepping  0 
#define ee_StepMask  0x0F
#define ee_BoardID   4 
#define ee_BoardMask 0x0FFF

#define ee_INT_TO_IRQ 0 
#define ee_FX_INT2IRQ 0x1EB8 

#define ee_SIZE 0x40 
#define ee_Checksum 0xBABA 


#define ee_addr_vendor 0x10  
#define ee_addr_id 0x11      
#define ee_addr_SN 0x12      
#define ee_addr_CRC_8 0x14   


#define ee_vendor_intel0 0x25  
#define ee_vendor_intel1 0xD4
#define ee_id_eepro10p0 0x10   
#define ee_id_eepro10p1 0x31

#define	RAM_SIZE	0x8000

#define	RCV_HEADER	8
#define RCV_DEFAULT_RAM	0x6000
#define RCV_RAM 	rcv_ram

static unsigned rcv_ram = RCV_DEFAULT_RAM;

#define XMT_HEADER	8
#define XMT_RAM		(RAM_SIZE - RCV_RAM)

#define XMT_START	((rcv_start + RCV_RAM) % RAM_SIZE)

#define RCV_LOWER_LIMIT	(rcv_start >> 8)
#define RCV_UPPER_LIMIT	(((rcv_start + RCV_RAM) - 2) >> 8)
#define XMT_LOWER_LIMIT	(XMT_START >> 8)
#define XMT_UPPER_LIMIT	(((XMT_START + XMT_RAM) - 2) >> 8)

#define RCV_START_PRO	0x00
#define RCV_START_10	XMT_RAM
					
static unsigned rcv_start = RCV_START_PRO;

#define	RCV_DONE	0x0008
#define	RX_OK		0x2000
#define	RX_ERROR	0x0d81

#define	TX_DONE_BIT	0x0080
#define	CHAIN_BIT	0x8000
#define	XMT_STATUS	0x02
#define	XMT_CHAIN	0x04
#define	XMT_COUNT	0x06

#define	BANK0_SELECT	0x00		
#define	BANK1_SELECT	0x40		
#define	BANK2_SELECT	0x80		

#define	COMMAND_REG	0x00	
#define	MC_SETUP	0x03
#define	XMT_CMD		0x04
#define	DIAGNOSE_CMD	0x07
#define	RCV_ENABLE_CMD	0x08
#define	RCV_DISABLE_CMD	0x0a
#define	STOP_RCV_CMD	0x0b
#define	RESET_CMD	0x0e
#define	POWER_DOWN_CMD	0x18
#define	RESUME_XMT_CMD	0x1c
#define	SEL_RESET_CMD	0x1e
#define	STATUS_REG	0x01	
#define	RX_INT		0x02
#define	TX_INT		0x04
#define	EXEC_STATUS	0x30
#define	ID_REG		0x02	
#define	R_ROBIN_BITS	0xc0	
#define	ID_REG_MASK	0x2c
#define	ID_REG_SIG	0x24
#define	AUTO_ENABLE	0x10
#define	INT_MASK_REG	0x03	
#define	RX_STOP_MASK	0x01
#define	RX_MASK		0x02
#define	TX_MASK		0x04
#define	EXEC_MASK	0x08
#define	ALL_MASK	0x0f
#define	IO_32_BIT	0x10
#define	RCV_BAR		0x04	
#define	RCV_STOP	0x06

#define	XMT_BAR_PRO	0x0a
#define	XMT_BAR_10	0x0b
static unsigned xmt_bar = XMT_BAR_PRO;

#define	HOST_ADDRESS_REG	0x0c
#define	IO_PORT		0x0e
#define	IO_PORT_32_BIT	0x0c

#define	REG1	0x01
#define	WORD_WIDTH	0x02
#define	INT_ENABLE	0x80
#define INT_NO_REG	0x02
#define	RCV_LOWER_LIMIT_REG	0x08
#define	RCV_UPPER_LIMIT_REG	0x09

#define	XMT_LOWER_LIMIT_REG_PRO	0x0a
#define	XMT_UPPER_LIMIT_REG_PRO	0x0b
#define	XMT_LOWER_LIMIT_REG_10	0x0b
#define	XMT_UPPER_LIMIT_REG_10	0x0a
static unsigned xmt_lower_limit_reg = XMT_LOWER_LIMIT_REG_PRO;
static unsigned xmt_upper_limit_reg = XMT_UPPER_LIMIT_REG_PRO;

#define	XMT_Chain_Int	0x20	
#define	XMT_Chain_ErrStop	0x40 
#define	RCV_Discard_BadFrame	0x80 
#define	REG2		0x02
#define	PRMSC_Mode	0x01
#define	Multi_IA	0x20
#define	REG3		0x03
#define	TPE_BIT		0x04
#define	BNC_BIT		0x20
#define	REG13		0x0d
#define	FDX		0x00
#define	A_N_ENABLE	0x02
	
#define	I_ADD_REG0	0x04
#define	I_ADD_REG1	0x05
#define	I_ADD_REG2	0x06
#define	I_ADD_REG3	0x07
#define	I_ADD_REG4	0x08
#define	I_ADD_REG5	0x09

#define EEPROM_REG_PRO	0x0a
#define EEPROM_REG_10	0x0b
static unsigned eeprom_reg = EEPROM_REG_PRO;

#define EESK 0x01
#define EECS 0x02
#define EEDI 0x04
#define EEDO 0x08


#define eeprom_delay() { udelay(40); }
#define EE_READ_CMD (6 << 6)

#define eepro_full_reset(ioaddr)	outb(RESET_CMD, ioaddr); udelay(40);

#define eepro_sel_reset(ioaddr) 	{ \
					outb(SEL_RESET_CMD, ioaddr); \
					SLOW_DOWN; \
					SLOW_DOWN; \
					}

#define	eepro_clear_int(ioaddr)	outb(ALL_MASK, ioaddr + STATUS_REG)

#define	eepro_en_rx(ioaddr)	outb(RCV_ENABLE_CMD, ioaddr)

#define	eepro_dis_rx(ioaddr)	outb(RCV_DISABLE_CMD, ioaddr)

#define eepro_sw2bank0(ioaddr) outb(BANK0_SELECT, ioaddr)
#define eepro_sw2bank1(ioaddr) outb(BANK1_SELECT, ioaddr)
#define eepro_sw2bank2(ioaddr) outb(BANK2_SELECT, ioaddr)

static unsigned int	rx_start, tx_start;
static int		tx_last;
static unsigned		tx_end;
static int		eepro = 0;
static unsigned short	ioaddr = 0;
static unsigned int	mem_start, mem_end = RCV_DEFAULT_RAM / 1024;

#define	udelay(n)	waiton_timer2(((n)*TICKS_PER_MS)/1000)

static void eepro_reset(struct nic *nic)
{
	int		temp_reg, i;

	
	eepro_sw2bank2(ioaddr);	
	temp_reg = inb(ioaddr + eeprom_reg);
#ifdef	DEBUG
	printf("Stepping %d\n", temp_reg >> 5);
#endif
	if (temp_reg & 0x10)	
		outb(temp_reg & 0xEF, ioaddr + eeprom_reg);
	for (i = 0; i < ETH_ALEN; i++)	
		outb(nic->node_addr[i], ioaddr + I_ADD_REG0 + i);
	temp_reg = inb(ioaddr + REG1);
	
	outb(temp_reg | XMT_Chain_Int | XMT_Chain_ErrStop
		| RCV_Discard_BadFrame, ioaddr + REG1);
	temp_reg = inb(ioaddr + REG2);		
	outb(temp_reg | 0x14, ioaddr + REG2);
	temp_reg = inb(ioaddr + REG3);
	outb(temp_reg & 0x3F, ioaddr + REG3);	
	
	eepro_sw2bank1(ioaddr);	
	
	outb(RCV_LOWER_LIMIT, ioaddr + RCV_LOWER_LIMIT_REG);
	outb(RCV_UPPER_LIMIT, ioaddr + RCV_UPPER_LIMIT_REG);
	outb(XMT_LOWER_LIMIT, ioaddr + xmt_lower_limit_reg);
	outb(XMT_UPPER_LIMIT, ioaddr + xmt_upper_limit_reg);
	eepro_sw2bank0(ioaddr);	
	eepro_clear_int(ioaddr);
	
	outw(rx_start = (RCV_LOWER_LIMIT << 8), ioaddr + RCV_BAR);
	outw(((RCV_UPPER_LIMIT << 8) | 0xFE), ioaddr + RCV_STOP);
	
	outw((XMT_LOWER_LIMIT << 8), ioaddr + xmt_bar);
	eepro_sel_reset(ioaddr);
	tx_start = tx_end = (XMT_LOWER_LIMIT << 8);
	tx_last = 0;
	eepro_en_rx(ioaddr);
}

static int eepro_poll(struct nic *nic)
{
	int		i;
	unsigned int	rcv_car = rx_start;
	unsigned int	rcv_event, rcv_status, rcv_next_frame, rcv_size;

	
	
	
#if	0
	if ((inb(ioaddr + STATUS_REG) & 0x40) == 0)
		return (0);
	outb(0x40, ioaddr + STATUS_REG);
#endif
	outw(rcv_car, ioaddr + HOST_ADDRESS_REG);
	rcv_event = inw(ioaddr + IO_PORT);
	if (rcv_event != RCV_DONE)
		return (0);
	rcv_status = inw(ioaddr + IO_PORT);
	rcv_next_frame = inw(ioaddr + IO_PORT);
	rcv_size = inw(ioaddr + IO_PORT);
#if	0
	printf("%hX %hX %d %hhX\n", rcv_status, rcv_next_frame, rcv_size,
		inb(ioaddr + STATUS_REG));
#endif
	if ((rcv_status & (RX_OK|RX_ERROR)) != RX_OK) {
		printf("Receive error %hX\n", rcv_status);
		return (0);
	}
	rcv_size &= 0x3FFF;
	insw(ioaddr + IO_PORT, nic->packet, ((rcv_size + 3) >> 1));
#if	0
	for (i = 0; i < 48; i++) {
		printf("%hhX", nic->packet[i]);
		putchar(i % 16 == 15 ? '\n' : ' ');
	}
#endif
	nic->packetlen = rcv_size;
	rcv_car = rx_start + RCV_HEADER + rcv_size;
	rx_start = rcv_next_frame;
	if (rcv_car == 0)
		rcv_car = ((RCV_UPPER_LIMIT << 8) | 0xff);
	outw(rcv_car - 1, ioaddr + RCV_STOP);
	return (1);
}

static void eepro_transmit(
	struct nic *nic,
	const char *d,			
	unsigned int t,			
	unsigned int s,			
	const char *p)			
{
	unsigned int	status, tx_available, last, end, length;
	unsigned short	type;
	int		boguscount = 20;

	length = s + ETH_HLEN;
	if (tx_end > tx_start)
		tx_available = XMT_RAM - (tx_end - tx_start);
	else if (tx_end < tx_start)
		tx_available = tx_start - tx_end;
	else
		tx_available = XMT_RAM;
	last = tx_end;
	end = last + (((length + 3) >> 1) << 1) + XMT_HEADER;
	if (end >= (XMT_UPPER_LIMIT << 8)) {
		last = (XMT_LOWER_LIMIT << 8);
		end = last + (((length + 3) >> 1) << 1) + XMT_HEADER;
	}
	outw(last, ioaddr + HOST_ADDRESS_REG);
	outw(XMT_CMD, ioaddr + IO_PORT);
	outw(0, ioaddr + IO_PORT);
	outw(end, ioaddr + IO_PORT);
	outw(length, ioaddr + IO_PORT);
	outsw(ioaddr + IO_PORT, d, ETH_ALEN / 2);
	outsw(ioaddr + IO_PORT, nic->node_addr, ETH_ALEN / 2);
	type = htons(t);
	outsw(ioaddr + IO_PORT, &type, sizeof(type) / 2);
	outsw(ioaddr + IO_PORT, p, (s + 3) >> 1);
	
	status = inw(ioaddr + IO_PORT);
	outw(last, ioaddr + xmt_bar);
	outb(XMT_CMD, ioaddr);
	tx_start = last;
	tx_last = last;
	tx_end = end;
#if	0
	printf("%d %d\n", tx_start, tx_end);
#endif
	while (boguscount > 0) {
		if (((status = inw(ioaddr + IO_PORT)) & TX_DONE_BIT) == 0) {
			udelay(40);
			boguscount--;
			continue;
		}
#if	DEBUG
		if ((status & 0x2000) == 0)
			printf("Transmit status %hX\n", status);
#endif
	}
}

static void eepro_disable(struct nic *nic)
{
	eepro_sw2bank0(ioaddr);	
	
	outb(STOP_RCV_CMD, ioaddr);
	tx_start = tx_end = (XMT_LOWER_LIMIT << 8);
	tx_last = 0;
	
	eepro_full_reset(ioaddr);
}

static int read_eeprom(int location)
{
	int		i;
	unsigned short	retval = 0;
	int		ee_addr = ioaddr + eeprom_reg;
	int		read_cmd = location | EE_READ_CMD;
	int		ctrl_val = EECS;

	if (eepro == LAN595FX_10ISA) {
		eepro_sw2bank1(ioaddr);
		outb(0x00, ioaddr + STATUS_REG);
	}
	eepro_sw2bank2(ioaddr);
	outb(ctrl_val, ee_addr);
	
	for (i = 8; i >= 0; i--) {
		short outval = (read_cmd & (1 << i)) ? ctrl_val | EEDI : ctrl_val;
		outb(outval, ee_addr);
		outb(outval | EESK, ee_addr);	
		eeprom_delay();
		outb(outval, ee_addr);		
		eeprom_delay();
	}
	outb(ctrl_val, ee_addr);
	for (i = 16; i > 0; i--) {
		outb(ctrl_val | EESK, ee_addr);
		eeprom_delay();
		retval = (retval << 1) | ((inb(ee_addr) & EEDO) ? 1 : 0);
		outb(ctrl_val, ee_addr);
		eeprom_delay();
	}
	
	ctrl_val &= ~EECS;
	outb(ctrl_val | EESK, ee_addr);
	eeprom_delay();
	outb(ctrl_val, ee_addr);
	eeprom_delay();
	eepro_sw2bank0(ioaddr);
	return (retval);
}

static int eepro_probe1(struct nic *nic)
{
	int		i, id, counter, l_eepro = 0;
	union {
		unsigned char	caddr[ETH_ALEN];
		unsigned short	saddr[ETH_ALEN/2];
	} station_addr;
	char		*name;

	id = inb(ioaddr + ID_REG);
	if ((id & ID_REG_MASK) != ID_REG_SIG)
		return (0);
	counter = id & R_ROBIN_BITS;
	if (((id = inb(ioaddr + ID_REG)) & R_ROBIN_BITS) != (counter + 0x40))
		return (0);
	
	station_addr.saddr[2] = read_eeprom(2);
	if (station_addr.saddr[2] == 0x0000 || station_addr.saddr[2] == 0xFFFF) {
		l_eepro = 3;
		eepro = LAN595FX_10ISA;
		eeprom_reg= EEPROM_REG_10;
		rcv_start = RCV_START_10;
		xmt_lower_limit_reg = XMT_LOWER_LIMIT_REG_10;
		xmt_upper_limit_reg = XMT_UPPER_LIMIT_REG_10;
		station_addr.saddr[2] = read_eeprom(2);
	}
	station_addr.saddr[1] = read_eeprom(3);
	station_addr.saddr[0] = read_eeprom(4);
	if (l_eepro)
		name = "Intel EtherExpress 10 ISA";
	else if (read_eeprom(7) == ee_FX_INT2IRQ) {
		name = "Intel EtherExpress Pro/10+ ISA";
		l_eepro = 2;
	} else if (station_addr.saddr[0] == SA_ADDR1) {
		name = "Intel EtherExpress Pro/10 ISA";
		l_eepro = 1;
	} else {
		l_eepro = 0;
		name = "Intel 82595-based LAN card";
	}
	station_addr.saddr[0] = swap16(station_addr.saddr[0]);
	station_addr.saddr[1] = swap16(station_addr.saddr[1]);
	station_addr.saddr[2] = swap16(station_addr.saddr[2]);
	for (i = 0; i < ETH_ALEN; i++) {
		nic->node_addr[i] = station_addr.caddr[i];
	}
	printf("\n%s ioaddr %#hX, addr %!", name, ioaddr, nic->node_addr);
	mem_start = RCV_LOWER_LIMIT << 8;
	if ((mem_end & 0x3F) < 3 || (mem_end & 0x3F) > 29)
		mem_end = RCV_UPPER_LIMIT << 8;
	else {
		mem_end = mem_end * 1024 + (RCV_LOWER_LIMIT << 8);
		rcv_ram = mem_end - (RCV_LOWER_LIMIT << 8);
	}
	printf(", Rx mem %dK, if %s\n", (mem_end - mem_start) >> 10,
		GetBit(read_eeprom(5), ee_BNC_TPE) ? "BNC" : "TP");
	return (1);
}

struct nic *eepro_probe(struct nic *nic, unsigned short *probe_addrs)
{
	unsigned short		*p;
	
	static unsigned short	ioaddrs[] = {
		0x300, 0x210, 0x240, 0x280, 0x2C0, 0x200, 0x320, 0x340, 0x360, 0};

	if (probe_addrs == 0 || probe_addrs[0] == 0)
		probe_addrs = ioaddrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; p++) {
		if (eepro_probe1(nic))
			break;
	}
	if (*p == 0)
		return (0);
	eepro_reset(nic);
	
	nic->reset = eepro_reset;
	nic->poll = eepro_poll;
	nic->transmit = eepro_transmit;
	nic->disable = eepro_disable;
	return (nic);
}
