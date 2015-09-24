/* rtl8139.c - etherboot driver for the Realtek 8139 chipset

  ported from the linux driver written by Donald Becker
  by Rainer Bawidamann (Rainer.Bawidamann@informatik.uni-ulm.de) 1999

  This software may be used and distributed according to the terms
  of the GNU Public License, incorporated herein by reference.

  changes to the original driver:
  - removed support for interrupts, switching to polling mode (yuck!)
  - removed support for the 8129 chip (external MII)

*/



#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"
#include "timer.h"

#define RTL_TIMEOUT (1*TICKS_PER_SEC)

#define TX_FIFO_THRESH 256      
#define RX_FIFO_THRESH  4       
#define RX_DMA_BURST    4       
#define TX_DMA_BURST    4       
#define NUM_TX_DESC     4       
#define TX_BUF_SIZE	ETH_FRAME_LEN	
#define RX_BUF_LEN_IDX 0	
#define RX_BUF_LEN (8192 << RX_BUF_LEN_IDX)

#undef DEBUG_TX
#undef DEBUG_RX

enum RTL8139_registers {
	MAC0=0,			
	MAR0=8,			
	TxStatus0=0x10,		
	TxAddr0=0x20,		
	RxBuf=0x30, RxEarlyCnt=0x34, RxEarlyStatus=0x36,
	ChipCmd=0x37, RxBufPtr=0x38, RxBufAddr=0x3A,
	IntrMask=0x3C, IntrStatus=0x3E,
	TxConfig=0x40, RxConfig=0x44,
	Timer=0x48,		
	RxMissed=0x4C,		
	Cfg9346=0x50, Config0=0x51, Config1=0x52,
	TimerIntrReg=0x54,	
	MediaStatus=0x58,
	Config3=0x59,
	MultiIntr=0x5C,
	RevisionID=0x5E,	
	TxSummary=0x60,
	MII_BMCR=0x62, MII_BMSR=0x64, NWayAdvert=0x66, NWayLPAR=0x68,
	NWayExpansion=0x6A,
	DisconnectCnt=0x6C, FalseCarrierCnt=0x6E,
	NWayTestReg=0x70,
	RxCnt=0x72,		
	CSCR=0x74,		
	PhyParm1=0x78,TwisterParm=0x7c,PhyParm2=0x80,	
};

enum ChipCmdBits {
	CmdReset=0x10, CmdRxEnb=0x08, CmdTxEnb=0x04, RxBufEmpty=0x01, };

enum IntrStatusBits {
	PCIErr=0x8000, PCSTimeout=0x4000, CableLenChange= 0x2000,
	RxFIFOOver=0x40, RxUnderrun=0x20, RxOverflow=0x10,
	TxErr=0x08, TxOK=0x04, RxErr=0x02, RxOK=0x01,
};
enum TxStatusBits {
	TxHostOwns=0x2000, TxUnderrun=0x4000, TxStatOK=0x8000,
	TxOutOfWindow=0x20000000, TxAborted=0x40000000,
	TxCarrierLost=0x80000000,
};
enum RxStatusBits {
	RxMulticast=0x8000, RxPhysical=0x4000, RxBroadcast=0x2000,
	RxBadSymbol=0x0020, RxRunt=0x0010, RxTooLong=0x0008, RxCRCErr=0x0004,
	RxBadAlign=0x0002, RxStatusOK=0x0001,
};

enum MediaStatusBits {
	MSRTxFlowEnable=0x80, MSRRxFlowEnable=0x40, MSRSpeed10=0x08,
	MSRLinkFail=0x04, MSRRxPauseFlag=0x02, MSRTxPauseFlag=0x01,
};

enum MIIBMCRBits {
	BMCRReset=0x8000, BMCRSpeed100=0x2000, BMCRNWayEnable=0x1000,
	BMCRRestartNWay=0x0200, BMCRDuplex=0x0100,
};

enum CSCRBits {
	CSCR_LinkOKBit=0x0400, CSCR_LinkChangeBit=0x0800,
	CSCR_LinkStatusBits=0x0f000, CSCR_LinkDownOffCmd=0x003c0,
	CSCR_LinkDownCmd=0x0f3c0,
};

enum rx_mode_bits {
	RxCfgWrap=0x80,
	AcceptErr=0x20, AcceptRunt=0x10, AcceptBroadcast=0x08,
	AcceptMulticast=0x04, AcceptMyPhys=0x02, AcceptAllPhys=0x01,
};

static int ioaddr;
static unsigned int cur_rx,cur_tx;

static unsigned char tx_buffer[TX_BUF_SIZE] __attribute__((aligned(4)));

#ifdef	USE_LOWMEM_BUFFER
#define rx_ring ((unsigned char *)(0x10000 - (RX_BUF_LEN + 16)))
#else
static unsigned char rx_ring[RX_BUF_LEN+16] __attribute__((aligned(4)));
#endif

struct nic *rtl8139_probe(struct nic *nic, unsigned short *probeaddrs,
	struct pci_device *pci);
static int read_eeprom(int location);
static void rtl_reset(struct nic *nic);
static void rtl_transmit(struct nic *nic, const char *destaddr,
	unsigned int type, unsigned int len, const char *data);
static int rtl_poll(struct nic *nic);
static void rtl_disable(struct nic*);


struct nic *rtl8139_probe(struct nic *nic, unsigned short *probeaddrs,
	struct pci_device *pci)
{
	int i;
	int speed10, fullduplex;

	printf(" - ");

	
	ioaddr = probeaddrs[0] & ~3;

	adjust_pci_device(pci);

	
	outb(0x00, ioaddr + Config1);

	if (read_eeprom(0) != 0xffff) {
		unsigned short *ap = (unsigned short*)nic->node_addr;
		for (i = 0; i < 3; i++)
			*ap++ = read_eeprom(i + 7);
	} else {
		unsigned char *ap = (unsigned char*)nic->node_addr;
		for (i = 0; i < ETH_ALEN; i++)
			*ap++ = inb(ioaddr + MAC0 + i);
	}

	speed10 = inb(ioaddr + MediaStatus) & MSRSpeed10;
	fullduplex = inw(ioaddr + MII_BMCR) & BMCRDuplex;
	printf("ioaddr %#hX, addr %! %sMbps %s-duplex\n", ioaddr,
		nic->node_addr,  speed10 ? "10" : "100",
		fullduplex ? "full" : "half");

	rtl_reset(nic);

	nic->reset = rtl_reset;
	nic->poll = rtl_poll;
	nic->transmit = rtl_transmit;
	nic->disable = rtl_disable;

	return nic;
}


#define EE_SHIFT_CLK    0x04    
#define EE_CS           0x08    
#define EE_DATA_WRITE   0x02    
#define EE_WRITE_0      0x00
#define EE_WRITE_1      0x02
#define EE_DATA_READ    0x01    
#define EE_ENB          (0x80 | EE_CS)


#define eeprom_delay()  inl(ee_addr)

#define EE_WRITE_CMD    (5 << 6)
#define EE_READ_CMD     (6 << 6)
#define EE_ERASE_CMD    (7 << 6)

static int read_eeprom(int location)
{
	int i;
	unsigned int retval = 0;
	long ee_addr = ioaddr + Cfg9346;
	int read_cmd = location | EE_READ_CMD;

	outb(EE_ENB & ~EE_CS, ee_addr);
	outb(EE_ENB, ee_addr);

	
	for (i = 10; i >= 0; i--) {
		int dataval = (read_cmd & (1 << i)) ? EE_DATA_WRITE : 0;
		outb(EE_ENB | dataval, ee_addr);
		eeprom_delay();
		outb(EE_ENB | dataval | EE_SHIFT_CLK, ee_addr);
		eeprom_delay();
	}
	outb(EE_ENB, ee_addr);
	eeprom_delay();

	for (i = 16; i > 0; i--) {
		outb(EE_ENB | EE_SHIFT_CLK, ee_addr);
		eeprom_delay();
		retval = (retval << 1) | ((inb(ee_addr) & EE_DATA_READ) ? 1 : 0);
		outb(EE_ENB, ee_addr);
		eeprom_delay();
	}

	
	outb(~EE_CS, ee_addr);
	return retval;
}

static void rtl_reset(struct nic* nic)
{
	int i;

	outb(CmdReset, ioaddr + ChipCmd);

	cur_rx = 0;
	cur_tx = 0;

	
	load_timer2(10*TICKS_PER_MS);
	while ((inb(ioaddr + ChipCmd) & CmdReset) != 0 && timer2_running())
		;

	for (i = 0; i < ETH_ALEN; i++)
		outb(nic->node_addr[i], ioaddr + MAC0 + i);

	
	outb(CmdRxEnb | CmdTxEnb, ioaddr + ChipCmd);
	outl((RX_FIFO_THRESH<<13) | (RX_BUF_LEN_IDX<<11) | (RX_DMA_BURST<<8),
		ioaddr + RxConfig);		
	outl((TX_DMA_BURST<<8)|0x03000000, ioaddr + TxConfig);


#ifdef	DEBUG_RX
	printf("rx ring address is %X\n",(unsigned long)rx_ring);
#endif
	outl((unsigned long)rx_ring, ioaddr + RxBuf);

	
	outl(0, ioaddr + RxMissed);
	
	outb(AcceptBroadcast|AcceptMyPhys, ioaddr + RxConfig);
	outb(CmdRxEnb | CmdTxEnb, ioaddr + ChipCmd);

	
	outw(0, ioaddr + IntrMask);
}

static void rtl_transmit(struct nic *nic, const char *destaddr,
	unsigned int type, unsigned int len, const char *data)
{
	unsigned int status, to, nstype;
	unsigned long txstatus;

	memcpy(tx_buffer, destaddr, ETH_ALEN);
	memcpy(tx_buffer + ETH_ALEN, nic->node_addr, ETH_ALEN);
	nstype = htons(type);
	memcpy(tx_buffer + 2 * ETH_ALEN, (char*)&nstype, 2);
	memcpy(tx_buffer + ETH_HLEN, data, len);

	len += ETH_HLEN;
#ifdef	DEBUG_TX
	printf("sending %d bytes ethtype %hX\n", len, type);
#endif

	while (len < ETH_ZLEN) {
		tx_buffer[len++] = '\0';
	}

	outl((unsigned long)tx_buffer, ioaddr + TxAddr0 + cur_tx*4);
	outl(((TX_FIFO_THRESH<<11) & 0x003f0000) | len,
		ioaddr + TxStatus0 + cur_tx*4);

	to = currticks() + RTL_TIMEOUT;

	do {
		status = inw(ioaddr + IntrStatus);
		outw(status & (TxOK | TxErr | PCIErr), ioaddr + IntrStatus);
		if ((status & (TxOK | TxErr | PCIErr)) != 0) break;
	} while (currticks() < to);

	txstatus = inl(ioaddr+ TxStatus0 + cur_tx*4);

	if (status & TxOK) {
		cur_tx = (cur_tx + 1) % NUM_TX_DESC;
#ifdef	DEBUG_TX
		printf("tx done (%d ticks), status %hX txstatus %X\n",
			to-currticks(), status, txstatus);
#endif
	} else {
#ifdef	DEBUG_TX
		printf("tx timeout/error (%d ticks), status %hX txstatus %X\n",
			currticks()-to, status, txstatus);
#endif
		rtl_reset(nic);
	}
}

static int rtl_poll(struct nic *nic)
{
	unsigned int status;
	unsigned int ring_offs;
	unsigned int rx_size, rx_status;

	if (inb(ioaddr + ChipCmd) & RxBufEmpty) {
		return 0;
	}

	status = inw(ioaddr + IntrStatus);
	
	outw(status & ~(RxFIFOOver | RxOverflow | RxOK), ioaddr + IntrStatus);

#ifdef	DEBUG_RX
	printf("rtl_poll: int %hX ", status);
#endif

	ring_offs = cur_rx % RX_BUF_LEN;
	rx_status = *(unsigned int*)(rx_ring + ring_offs);
	rx_size = rx_status >> 16;
	rx_status &= 0xffff;

	if ((rx_status & (RxBadSymbol|RxRunt|RxTooLong|RxCRCErr|RxBadAlign)) ||
	    (rx_size < ETH_ZLEN) || (rx_size > ETH_FRAME_LEN + 4)) {
		printf("rx error %hX\n", rx_status);
		rtl_reset(nic);	
		return 0;
	}

	
	nic->packetlen = rx_size - 4;	
	if (ring_offs+4+rx_size-4 > RX_BUF_LEN) {
		int semi_count = RX_BUF_LEN - ring_offs - 4;

		memcpy(nic->packet, rx_ring + ring_offs + 4, semi_count);
		memcpy(nic->packet+semi_count, rx_ring, rx_size-4-semi_count);
#ifdef	DEBUG_RX
		printf("rx packet %d+%d bytes", semi_count,rx_size-4-semi_count);
#endif
	} else {
		memcpy(nic->packet, rx_ring + ring_offs + 4, nic->packetlen);
#ifdef	DEBUG_RX
		printf("rx packet %d bytes", rx_size-4);
#endif
	}
#ifdef	DEBUG_RX
	printf(" at %X type %hhX%hhX rxstatus %hX\n",
		(unsigned long)(rx_ring+ring_offs+4),
		nic->packet[12], nic->packet[13], rx_status);
#endif
	cur_rx = (cur_rx + rx_size + 4 + 3) & ~3;
	outw(cur_rx - 16, ioaddr + RxBufPtr);
	outw(status & (RxFIFOOver | RxOverflow | RxOK), ioaddr + IntrStatus);
	return 1;
}

static void rtl_disable(struct nic *nic)
{
	
	outb(CmdReset, ioaddr + ChipCmd);

	
	load_timer2(10*TICKS_PER_MS);
	while ((inb(ioaddr + ChipCmd) & CmdReset) != 0 && timer2_running())
		;
}
