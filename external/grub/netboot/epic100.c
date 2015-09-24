
#define LINUX_OUT_MACROS

#include "etherboot.h"
#include "nic.h"
#include "cards.h"
#include "timer.h"
#include "epic100.h"

#undef	virt_to_bus
#define	virt_to_bus(x)	((unsigned long)x)

#define TX_RING_SIZE	2	
#define RX_RING_SIZE	2

#define PKT_BUF_SZ	1536	


#define EPIC_DEBUG 0	

struct epic_rx_desc {
    unsigned short status;
    unsigned short rxlength;
    unsigned long  bufaddr;
    unsigned short buflength;
    unsigned short control;
    unsigned long  next;
};

#define TD_STDFLAGS	TD_LASTDESC

struct epic_tx_desc {
    unsigned short status;
    unsigned short txlength;
    unsigned long  bufaddr;
    unsigned short buflength;
    unsigned short control;
    unsigned long  next;
};

#define delay(nanosec)   do { int _i = 3; while (--_i > 0) \
                                     { __SLOW_DOWN_IO; }} while (0)

static void	epic100_open(void);
static void	epic100_init_ring(void);
static void	epic100_disable(struct nic *nic);
static int	epic100_poll(struct nic *nic);
static void	epic100_transmit(struct nic *nic, const char *destaddr,
				 unsigned int type, unsigned int len, const char *data);
static int	read_eeprom(int location);
static int	mii_read(int phy_id, int location);

static int	ioaddr;

static int	command;
static int	intstat;
static int	intmask;
static int	genctl ;
static int	eectl  ;
static int	test   ;
static int	mmctl  ;
static int	mmdata ;
static int	lan0   ;
static int	rxcon  ;
static int	txcon  ;
static int	prcdar ;
static int	ptcdar ;
static int	eththr ;

static unsigned int	cur_rx, cur_tx;		
#ifdef	DEBUG_EEPROM
static unsigned short	eeprom[64];
#endif
static signed char	phys[4];		
static struct epic_rx_desc	rx_ring[RX_RING_SIZE];
static struct epic_tx_desc	tx_ring[TX_RING_SIZE];
#ifdef	USE_LOWMEM_BUFFER
#define rx_packet ((char *)0x10000 - PKT_BUF_SZ * RX_RING_SIZE)
#define tx_packet ((char *)0x10000 - PKT_BUF_SZ * RX_RING_SIZE - PKT_BUF_SZ * TX_RING_SIZE)
#else
static char		rx_packet[PKT_BUF_SZ * RX_RING_SIZE];
static char		tx_packet[PKT_BUF_SZ * TX_RING_SIZE];
#endif


    static void
epic100_reset(struct nic *nic)
{
    
    outl(GC_SOFT_RESET, genctl);
}

    struct nic*
epic100_probe(struct nic *nic, unsigned short *probeaddrs)
{
    unsigned short sum = 0;
    unsigned short value;
    int i;
    unsigned short* ap;
    unsigned int phy, phy_idx;

    if (probeaddrs == 0 || probeaddrs[0] == 0)
	return 0;


    ioaddr = probeaddrs[0] & ~3; 

    
    command = ioaddr + COMMAND;		
    intstat = ioaddr + INTSTAT;		
    intmask = ioaddr + INTMASK;		
    genctl  = ioaddr + GENCTL;		
    eectl   = ioaddr + EECTL;		
    test    = ioaddr + TEST;		
    mmctl   = ioaddr + MMCTL;		
    mmdata  = ioaddr + MMDATA;		
    lan0    = ioaddr + LAN0;		
    rxcon   = ioaddr + RXCON;		
    txcon   = ioaddr + TXCON;		
    prcdar  = ioaddr + PRCDAR;		
    ptcdar  = ioaddr + PTCDAR;		
    eththr  = ioaddr + ETHTHR;		

    
    outl(GC_SOFT_RESET, genctl);

    
    outl(INTR_DISABLE, intmask);


    for (i = 0; i < 16; i++) {
	outl(0x00000008, test);
    }

#ifdef	DEBUG_EEPROM
    for (i = 0; i < 64; i++) {
	value = read_eeprom(i);
	eeprom[i] = value;
	sum += value;
    }

#if	(EPIC_DEBUG > 1)
    printf("EEPROM contents\n");
    for (i = 0; i < 64; i++) {
	printf(" %hhX%s", eeprom[i], i % 16 == 15 ? "\n" : "");
    }
#endif
#endif

    
    ap = (unsigned short*)nic->node_addr;
    for (i = 0; i < 3; i++)
	*ap++ = inw(lan0 + i*4);

    printf(" I/O %#hX %! ", ioaddr, nic->node_addr);

    
    for (phy = 0, phy_idx = 0; phy < 32 && phy_idx < sizeof(phys); phy++) {
	int mii_status = mii_read(phy, 0);

	if (mii_status != 0xffff  && mii_status != 0x0000) {
	    phys[phy_idx++] = phy;
#if	(EPIC_DEBUG > 1)
	    printf("MII transceiver found at address %d.\n", phy);
#endif
	}
    }
    if (phy_idx == 0) {
#if	(EPIC_DEBUG > 1)
	printf("***WARNING***: No MII transceiver found!\n");
#endif
	
	phys[0] = 3;
    }

    epic100_open();

    nic->reset    = epic100_reset;
    nic->poll     = epic100_poll;
    nic->transmit = epic100_transmit;
    nic->disable  = epic100_disable;

    return nic;
}

    static void
epic100_open(void)
{
    int mii_reg5;
    int full_duplex = 0;
    unsigned long tmp;

    epic100_init_ring();

    
    outl(GC_RX_FIFO_THR_64 | GC_MRC_READ_MULT | GC_ONE_COPY, genctl);

    outl(TX_FIFO_THRESH, eththr);

    tmp = TC_EARLY_TX_ENABLE | TX_SLOT_TIME;

    mii_reg5 = mii_read(phys[0], 5);
    if (mii_reg5 != 0xffff && (mii_reg5 & 0x0100)) {
	full_duplex = 1;
	printf(" full-duplex mode");
	tmp |= TC_LM_FULL_DPX;
    } else
	tmp |= TC_LM_NORMAL;

    outl(tmp, txcon);

    
    outl(virt_to_bus(&rx_ring), prcdar);
    outl(virt_to_bus(&tx_ring), ptcdar);

    
    outl(0x04, rxcon);
    outl(CR_START_RX | CR_QUEUE_RX, command);

    putchar('\n');
}

    static void
epic100_init_ring(void)
{
    int i;
    char* p;

    cur_rx = cur_tx = 0;

    p = &rx_packet[0];
    for (i = 0; i < RX_RING_SIZE; i++) {
	rx_ring[i].status    = RRING_OWN;	
	rx_ring[i].buflength = PKT_BUF_SZ;
	rx_ring[i].bufaddr   = virt_to_bus(p + (PKT_BUF_SZ * i));
	rx_ring[i].control   = 0;
	rx_ring[i].next      = virt_to_bus(&(rx_ring[i + 1]) );
    }
    
    rx_ring[i-1].next = virt_to_bus(&rx_ring[0]);

    p = &tx_packet[0];

    for (i = 0; i < TX_RING_SIZE; i++) {
	tx_ring[i].status  = 0;			
	tx_ring[i].bufaddr = virt_to_bus(p + (PKT_BUF_SZ * i));
	tx_ring[i].control = TD_STDFLAGS;
	tx_ring[i].next    = virt_to_bus(&(tx_ring[i + 1]) );
    }
    tx_ring[i-1].next = virt_to_bus(&tx_ring[0]);
}

    static void
epic100_transmit(struct nic *nic, const char *destaddr, unsigned int type,
		 unsigned int len, const char *data)
{
    unsigned short nstype;
    char* txp;
    int entry;

    
    entry = cur_tx % TX_RING_SIZE;

    if ((tx_ring[entry].status & TRING_OWN) == TRING_OWN) {
	printf("eth_transmit: Unable to transmit. status=%hX. Resetting...\n",
	       tx_ring[entry].status);

	epic100_open();
	return;
    }

    txp = (char*)tx_ring[entry].bufaddr;

    memcpy(txp, destaddr, ETH_ALEN);
    memcpy(txp + ETH_ALEN, nic->node_addr, ETH_ALEN);
    nstype = htons(type);
    memcpy(txp + 12, (char*)&nstype, 2);
    memcpy(txp + ETH_HLEN, data, len);

    len += ETH_HLEN;

    tx_ring[entry].txlength  = (len >= 60 ? len : 60);
    tx_ring[entry].buflength = len;
    tx_ring[entry].status    = TRING_OWN;	

    cur_tx++;

    
    outl(CR_QUEUE_TX, command);

    load_timer2(10*TICKS_PER_MS);         
    while ((tx_ring[entry].status & TRING_OWN) && timer2_running())
	;

    if ((tx_ring[entry].status & TRING_OWN) != 0)
	printf("Oops, transmitter timeout, status=%hX\n",
	    tx_ring[entry].status);
}


    static int
epic100_poll(struct nic *nic)
{
    int entry;
    int status;
    int retcode;

    entry = cur_rx % RX_RING_SIZE;

    if ((status = rx_ring[entry].status & RRING_OWN) == RRING_OWN)
	return (0);

    

#if	(EPIC_DEBUG > 4)
    printf("epic_poll: entry %d status %hX\n", entry, status);
#endif

    cur_rx++;
    if (status & 0x2000) {
	printf("epic_poll: Giant packet\n");
	retcode = 0;
    } else if (status & 0x0006) {
	
	printf("epic_poll: Frame received with errors\n");
	retcode = 0;
    } else {
	
	nic->packetlen = rx_ring[entry].rxlength - 4;
	memcpy(nic->packet, (char*)rx_ring[entry].bufaddr, nic->packetlen);
	retcode = 1;
    }

    
    outl(status & INTR_CLEARERRS, intstat);

    
    rx_ring[entry].status = RRING_OWN;

    
    outl(CR_START_RX | CR_QUEUE_RX, command);

    return retcode;
}


    static void
epic100_disable(struct nic *nic)
{
}


#ifdef	DEBUG_EEPROM

#define EE_SHIFT_CLK	0x04	
#define EE_CS		0x02	
#define EE_DATA_WRITE	0x08	
#define EE_WRITE_0	0x01
#define EE_WRITE_1	0x09
#define EE_DATA_READ	0x10	
#define EE_ENB		(0x0001 | EE_CS)

#define EE_WRITE_CMD	(5 << 6)
#define EE_READ_CMD	(6 << 6)
#define EE_ERASE_CMD	(7 << 6)

#define eeprom_delay(n)	delay(n)

    static int
read_eeprom(int location)
{
    int i;
    int retval = 0;
    int read_cmd = location | EE_READ_CMD;

    outl(EE_ENB & ~EE_CS, eectl);
    outl(EE_ENB, eectl);

    
    for (i = 10; i >= 0; i--) {
	short dataval = (read_cmd & (1 << i)) ? EE_DATA_WRITE : 0;
	outl(EE_ENB | dataval, eectl);
	eeprom_delay(100);
	outl(EE_ENB | dataval | EE_SHIFT_CLK, eectl);
	eeprom_delay(150);
	outl(EE_ENB | dataval, eectl);	
	eeprom_delay(250);
    }
    outl(EE_ENB, eectl);

    for (i = 16; i > 0; i--) {
	outl(EE_ENB | EE_SHIFT_CLK, eectl);
	eeprom_delay(100);
	retval = (retval << 1) | ((inl(eectl) & EE_DATA_READ) ? 1 : 0);
	outl(EE_ENB, eectl);
	eeprom_delay(100);
    }

    
    outl(EE_ENB & ~EE_CS, eectl);
    return retval;
}
#endif


#define MII_READOP	1
#define MII_WRITEOP	2

    static int
mii_read(int phy_id, int location)
{
    int i;

    outl((phy_id << 9) | (location << 4) | MII_READOP, mmctl);
    

    for (i = 4000; i > 0; i--)
	if ((inl(mmctl) & MII_READOP) == 0)
	    break;
    return inw(mmdata);
}
