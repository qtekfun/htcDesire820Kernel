
/* 
   natsemi.c: An Etherboot driver for the NatSemi DP8381x series.

   Copyright (C) 2001 Entity Cyber, Inc.
   
   This development of this Etherboot driver was funded by 
   
      Sicom Systems: http://www.sicompos.com/
   
   Author: Marty Connor (mdc@thinguin.org)	   
   Adapted from a Linux driver which was written by Donald Becker
   
   This software may be used and distributed according to the terms
   of the GNU Public License (GPL), incorporated herein by reference.
   
   Original Copyright Notice:
   
   Written/copyright 1999-2001 by Donald Becker.
   
   This software may be used and distributed according to the terms of
   the GNU General Public License (GPL), incorporated herein by reference.
   Drivers based on or derived from this code fall under the GPL and must
   retain the authorship, copyright and license notice.  This file is not
   a complete program and may only be used when the entire operating
   system is licensed under the GPL.  License for under other terms may be
   available.  Contact the original author for details.
   
   The original author may be reached as becker@scyld.com, or at
   Scyld Computing Corporation
   410 Severn Ave., Suite 210
   Annapolis MD 21403
   
   Support information and updates available at
   http://www.scyld.com/network/netsemi.html
   
   References:
   
   http://www.scyld.com/expert/100mbps.html
   http://www.scyld.com/expert/NWay.html
   Datasheet is available from:
   http://www.national.com/pf/DP/DP83815.html

*/




#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"


#define OWN       0x80000000
#define DSIZE     0x00000FFF
#define CRC_SIZE  4

#define TX_TIMEOUT       (4*TICKS_PER_SEC)

#define TX_BUF_SIZE    1536
#define RX_BUF_SIZE    1536

#define NUM_RX_DESC    4              

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned int   u32;
typedef   signed int   s32;

#define le16_to_cpu(val) (val)
#define cpu_to_le32(val) (val)
#define get_unaligned(ptr) (*(ptr))
#define put_unaligned(val, ptr) ((void)( *(ptr) = (val) ))
#define get_u16(ptr) (*(u16 *)(ptr))
#define virt_to_bus(x) ((unsigned long)x)
#define virt_to_le32desc(addr)  virt_to_bus(addr)

enum pcistuff {
    PCI_USES_IO     = 0x01,
    PCI_USES_MEM    = 0x02,
    PCI_USES_MASTER = 0x04,
    PCI_ADDR0       = 0x08,
    PCI_ADDR1       = 0x10,
};

#define PCI_IOTYPE (PCI_USES_MASTER | PCI_USES_MEM | PCI_ADDR1)

enum register_offsets {
    ChipCmd      = 0x00, 
    ChipConfig   = 0x04, 
    EECtrl       = 0x08, 
    PCIBusCfg    = 0x0C,
    IntrStatus   = 0x10, 
    IntrMask     = 0x14, 
    IntrEnable   = 0x18,
    TxRingPtr    = 0x20, 
    TxConfig     = 0x24,
    RxRingPtr    = 0x30,
    RxConfig     = 0x34, 
    ClkRun       = 0x3C,
    WOLCmd       = 0x40, 
    PauseCmd     = 0x44,
    RxFilterAddr = 0x48, 
    RxFilterData = 0x4C,
    BootRomAddr  = 0x50, 
    BootRomData  = 0x54, 
    SiliconRev   = 0x58, 
    StatsCtrl    = 0x5C,
    StatsData    = 0x60, 
    RxPktErrs    = 0x60, 
    RxMissed     = 0x68, 
    RxCRCErrs    = 0x64,
    PCIPM        = 0x44,
    PhyStatus    = 0xC0, 
    MIntrCtrl    = 0xC4, 
    MIntrStatus  = 0xC8,

    
    PGSEL        = 0xCC, 
    PMDCSR       = 0xE4, 
    TSTDAT       = 0xFC, 
    DSPCFG       = 0xF4, 
    SDCFG        = 0x8C
};

enum ChipCmdBits {
    ChipReset = 0x100, 
    RxReset   = 0x20, 
    TxReset   = 0x10, 
    RxOff     = 0x08, 
    RxOn      = 0x04,
    TxOff     = 0x02, 
    TxOn      = 0x01
};

enum rx_mode_bits {
    AcceptErr          = 0x20,
    AcceptRunt         = 0x10,
    AcceptBroadcast    = 0xC0000000,
    AcceptMulticast    = 0x00200000, 
    AcceptAllMulticast = 0x20000000,
    AcceptAllPhys      = 0x10000000, 
    AcceptMyPhys       = 0x08000000
};

typedef struct _BufferDesc {
    u32              link;
    volatile u32     cmdsts;
    u32              bufptr;
    u32				 software_use;
} BufferDesc;

enum desc_status_bits {
    DescOwn   = 0x80000000, 
    DescMore  = 0x40000000, 
    DescIntr  = 0x20000000,
    DescNoCRC = 0x10000000,
    DescPktOK = 0x08000000, 
    RxTooLong = 0x00400000
};


static int natsemi_debug = 1;			

const char *nic_name;

static u32 SavedClkRun;


static unsigned short vendor, dev_id;
static unsigned long ioaddr;

static unsigned int cur_rx;

static unsigned int advertising;

static unsigned int rx_config;
static unsigned int tx_config;


static BufferDesc txd              __attribute__ ((aligned(4)));
static BufferDesc rxd[NUM_RX_DESC] __attribute__ ((aligned(4)));

#ifdef USE_LOWMEM_BUFFER
#define txb ((char *)0x10000 - TX_BUF_SIZE)
#define rxb ((char *)0x10000 - NUM_RX_DESC*RX_BUF_SIZE - TX_BUF_SIZE)
#else
static unsigned char txb[TX_BUF_SIZE] __attribute__ ((aligned(4)));
static unsigned char rxb[NUM_RX_DESC * RX_BUF_SIZE] __attribute__ ((aligned(4)));
#endif


struct nic *natsemi_probe(struct nic *nic, unsigned short *io_addrs, struct pci_device *pci);
static int eeprom_read(long addr, int location);
static int mdio_read(int phy_id, int location);
static void natsemi_init(struct nic *nic);
static void natsemi_reset(struct nic *nic);
static void natsemi_init_rxfilter(struct nic *nic);
static void natsemi_init_txd(struct nic *nic);
static void natsemi_init_rxd(struct nic *nic);
static void natsemi_set_rx_mode(struct nic *nic);
static void natsemi_check_duplex(struct nic *nic);
static void natsemi_transmit(struct nic *nic, const char *d, unsigned int t, unsigned int s, const char *p);
static int  natsemi_poll(struct nic *nic);
static void natsemi_disable(struct nic *nic);


struct nic *
natsemi_probe(struct nic *nic, unsigned short *io_addrs, struct pci_device *pci)
{
    int i;
    int prev_eedata;
    u32 tmp;

    if (io_addrs == 0 || *io_addrs == 0)
        return NULL;

    
	
    ioaddr     = *io_addrs & ~3;
    vendor     = pci->vendor;
    dev_id     = pci->dev_id;
    nic_name   = pci->name;
    
    adjust_pci_device(pci);

    pcibios_read_config_dword(pci->bus, pci->devfn, PCIPM, &tmp);
    if (tmp & (0x03|0x100)) {
	
	u32 newtmp = tmp & ~(0x03|0x100);
	pcibios_write_config_dword(pci->bus, pci->devfn, PCIPM, newtmp);
    }

    

    prev_eedata = eeprom_read(ioaddr, 6);
    for (i = 0; i < 3; i++) {
	int eedata = eeprom_read(ioaddr, i + 7);
	nic->node_addr[i*2] = (eedata << 1) + (prev_eedata >> 15);
	nic->node_addr[i*2+1] = eedata >> 7;
	prev_eedata = eedata;
    }

    printf("\nnatsemi_probe: MAC addr %! at ioaddr %#hX\n",
           nic->node_addr, ioaddr);
    printf("natsemi_probe: Vendor:%#hX Device:%#hX\n", vendor, dev_id);
    
    
    outl(ChipReset, ioaddr + ChipCmd);

    advertising = mdio_read(1, 4);
    {
	u32 chip_config = inl(ioaddr + ChipConfig);
	printf("%s: Transceiver default autoneg. %s "
	       "10%s %s duplex.\n",
	       nic_name,
	       chip_config & 0x2000 ? "enabled, advertise" : "disabled, force",
	       chip_config & 0x4000 ? "0" : "",
	       chip_config & 0x8000 ? "full" : "half");
    }
    printf("%s: Transceiver status %hX advertising %hX\n",
	   nic_name, (int)inl(ioaddr + 0x84), advertising);

    /* Disable PME:
     * The PME bit is initialized from the EEPROM contents.
     * PCI cards probably have PME disabled, but motherboard
     * implementations may have PME set to enable WakeOnLan. 
     * With PME set the chip will scan incoming packets but
     * nothing will be written to memory. */
    SavedClkRun = inl(ioaddr + ClkRun);
    outl(SavedClkRun & ~0x100, ioaddr + ClkRun);

    
    natsemi_init(nic);

    nic->reset    = natsemi_init;
    nic->poll     = natsemi_poll;
    nic->transmit = natsemi_transmit;
    nic->disable  = natsemi_disable;

    return nic;
}


#define eeprom_delay(ee_addr)	inl(ee_addr)

enum EEPROM_Ctrl_Bits {
    EE_ShiftClk   = 0x04, 
    EE_DataIn     = 0x01, 
    EE_ChipSelect = 0x08, 
    EE_DataOut    = 0x02
};

#define EE_Write0 (EE_ChipSelect)
#define EE_Write1 (EE_ChipSelect | EE_DataIn)

enum EEPROM_Cmds {
    EE_WriteCmd=(5 << 6), EE_ReadCmd=(6 << 6), EE_EraseCmd=(7 << 6),
};

static int eeprom_read(long addr, int location)
{
    int i;
    int retval = 0;
    int ee_addr = addr + EECtrl;
    int read_cmd = location | EE_ReadCmd;
    outl(EE_Write0, ee_addr);

    
    for (i = 10; i >= 0; i--) {
	short dataval = (read_cmd & (1 << i)) ? EE_Write1 : EE_Write0;
	outl(dataval, ee_addr);
	eeprom_delay(ee_addr);
	outl(dataval | EE_ShiftClk, ee_addr);
	eeprom_delay(ee_addr);
    }
    outl(EE_ChipSelect, ee_addr);
    eeprom_delay(ee_addr);

    for (i = 0; i < 16; i++) {
	outl(EE_ChipSelect | EE_ShiftClk, ee_addr);
	eeprom_delay(ee_addr);
	retval |= (inl(ee_addr) & EE_DataOut) ? 1 << i : 0;
	outl(EE_ChipSelect, ee_addr);
	eeprom_delay(ee_addr);
    }

    
    outl(EE_Write0, ee_addr);
    outl(0, ee_addr);

    return retval;
}


static int mdio_read(int phy_id, int location)
{
    if (phy_id == 1 && location < 32)
	return inl(ioaddr + 0x80 + (location<<2)) & 0xffff;
    else
	return 0xffff;
}


static void
natsemi_init(struct nic *nic)
{
    natsemi_reset(nic);
		
    /* Disable PME:
     * The PME bit is initialized from the EEPROM contents.
     * PCI cards probably have PME disabled, but motherboard
     * implementations may have PME set to enable WakeOnLan. 
     * With PME set the chip will scan incoming packets but
     * nothing will be written to memory. */
    outl(SavedClkRun & ~0x100, ioaddr + ClkRun);

    natsemi_init_rxfilter(nic);

    natsemi_init_txd(nic);
    natsemi_init_rxd(nic);

    
    
    
    if (inl(ioaddr + ChipConfig) & 0x20000000) {	
	tx_config = 0xD0801002;
	rx_config = 0x10000020;
    } else {
	tx_config = 0x10801002;
	rx_config = 0x0020;
    }
    outl(tx_config, ioaddr + TxConfig);
    outl(rx_config, ioaddr + RxConfig);

    natsemi_check_duplex(nic);
    natsemi_set_rx_mode(nic);

    outl(RxOn, ioaddr + ChipCmd);
}

static void 
natsemi_reset(struct nic *nic)
{
    outl(ChipReset, ioaddr + ChipCmd);
	
    if (inl(ioaddr + SiliconRev) == 0x302) {
	outw(0x0001, ioaddr + PGSEL);
	outw(0x189C, ioaddr + PMDCSR);
	outw(0x0000, ioaddr + TSTDAT);
	outw(0x5040, ioaddr + DSPCFG);
	outw(0x008C, ioaddr + SDCFG);
    }
    
    outl(0, ioaddr + IntrMask);
    outl(0, ioaddr + IntrEnable);
}


static void
natsemi_init_rxfilter(struct nic *nic)
{
    int i;

    for (i = 0; i < ETH_ALEN; i += 2) {
	outl(i, ioaddr + RxFilterAddr);
	outw(nic->node_addr[i] + (nic->node_addr[i+1] << 8), ioaddr + RxFilterData);
    }
}


static void
natsemi_init_txd(struct nic *nic)
{
    txd.link   = (u32) 0;
    txd.cmdsts = (u32) 0;
    txd.bufptr = (u32) &txb[0];

    
    outl((u32) &txd, ioaddr + TxRingPtr); 
    if (natsemi_debug > 1)
        printf("natsemi_init_txd: TX descriptor register loaded with: %X\n", 
               inl(ioaddr + TxRingPtr));
}

 
static void 
natsemi_init_rxd(struct nic *nic) 
{ 
    int i;

    cur_rx = 0; 

    
    for (i = 0; i < NUM_RX_DESC; i++) {
        rxd[i].link   = (i+1 < NUM_RX_DESC) ? (u32) &rxd[i+1] : (u32) &rxd[0];
        rxd[i].cmdsts = (u32) RX_BUF_SIZE;
        rxd[i].bufptr = (u32) &rxb[i*RX_BUF_SIZE];
        if (natsemi_debug > 1)
            printf("natsemi_init_rxd: rxd[%d]=%X link=%X cmdsts=%X bufptr=%X\n", 
                   i, &rxd[i], rxd[i].link, rxd[i].cmdsts, rxd[i].bufptr);
    }

    
    outl((u32) &rxd[0], ioaddr + RxRingPtr);

    if (natsemi_debug > 1)
        printf("natsemi_init_rxd: RX descriptor register loaded with: %X\n", 
               inl(ioaddr + RxRingPtr));
}


static void natsemi_set_rx_mode(struct nic *nic)
{
    u32 rx_mode = AcceptBroadcast | AcceptMyPhys;
	
    outl(rx_mode, ioaddr + RxFilterAddr);
}

static void natsemi_check_duplex(struct nic *nic)
{
    int duplex = inl(ioaddr + ChipConfig) & 0x20000000 ? 1 : 0;
	
    if (natsemi_debug)
	printf("%s: Setting %s-duplex based on negotiated link"
	       " capability.\n", nic_name,
	       duplex ? "full" : "half");
    if (duplex) {
	rx_config |= 0x10000000;
	tx_config |= 0xC0000000;
    } else {
	rx_config &= ~0x10000000;
	tx_config &= ~0xC0000000;
    }
    outl(tx_config, ioaddr + TxConfig);
    outl(rx_config, ioaddr + RxConfig);
}


static void
natsemi_transmit(struct nic  *nic,
		 const char  *d,     
		 unsigned int t,     
		 unsigned int s,     
		 const char  *p)     
{
    u32 status, to, nstype;
    u32 tx_status;
    
    
    outl(TxOff, ioaddr + ChipCmd);

    
    outl((u32) &txd, ioaddr + TxRingPtr);
    if (natsemi_debug > 1)
        printf("natsemi_transmit: TX descriptor register loaded with: %X\n", 
               inl(ioaddr + TxRingPtr));

    memcpy(txb, d, ETH_ALEN);
    memcpy(txb + ETH_ALEN, nic->node_addr, ETH_ALEN);
    nstype = htons(t);
    memcpy(txb + 2 * ETH_ALEN, (char*)&nstype, 2);
    memcpy(txb + ETH_HLEN, p, s);

    s += ETH_HLEN;
    s &= DSIZE;

    if (natsemi_debug > 1)
        printf("natsemi_transmit: sending %d bytes ethtype %hX\n", (int) s, t);

    
    while (s < ETH_ZLEN)  
        txb[s++] = '\0';

    
    txd.bufptr = (u32) &txb[0];
    txd.cmdsts = (u32) OWN | s;

    
    outl(TxOn, ioaddr + ChipCmd);

    if (natsemi_debug > 1)
        printf("natsemi_transmit: Queued Tx packet size %d.\n", (int) s);

    to = currticks() + TX_TIMEOUT;

    while ((((volatile u32) tx_status=txd.cmdsts) & OWN) && (currticks() < to))
         ;

    if (currticks() >= to) {
        printf("natsemi_transmit: TX Timeout! Tx status %X.\n", tx_status);
    }

    if (!(tx_status & 0x08000000)) {
	printf("natsemi_transmit: Transmit error, Tx status %X.\n", tx_status);
    }
}


static int
natsemi_poll(struct nic *nic)
{
    u32 rx_status = rxd[cur_rx].cmdsts;
    int retstat = 0;

    if (natsemi_debug > 2)
        printf("natsemi_poll: cur_rx:%d, status:%X\n", cur_rx, rx_status);

    if (!(rx_status & OWN))
        return retstat;

    if (natsemi_debug > 1)
        printf("natsemi_poll: got a packet: cur_rx:%d, status:%X\n",
               cur_rx, rx_status);

    nic->packetlen = (rx_status & DSIZE) - CRC_SIZE;

    if ((rx_status & (DescMore|DescPktOK|RxTooLong)) != DescPktOK) {
        
        printf("natsemi_poll: Corrupted packet received, buffer status = %X\n",
               rx_status);
        retstat = 0;
    } else {
        
        memcpy(nic->packet, (rxb + cur_rx*RX_BUF_SIZE), nic->packetlen);
        retstat = 1;
    }

    
    rxd[cur_rx].cmdsts = RX_BUF_SIZE;
    rxd[cur_rx].bufptr = (u32) &rxb[cur_rx*RX_BUF_SIZE];
        
    if (++cur_rx == NUM_RX_DESC)
        cur_rx = 0;

    
    outl(RxOn, ioaddr + ChipCmd);

    return retstat;
}


static void
natsemi_disable(struct nic *nic)
{
    
    outl(0, ioaddr + IntrMask);
    outl(0, ioaddr + IntrEnable);

    
    outl(RxOff | TxOff, ioaddr + ChipCmd);
	
    
    outl(SavedClkRun, ioaddr + ClkRun);
}