

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *              date       version  by   what
 *  Written:    Aug 20 2000  V0.10  iko  Initial revision.
 * changes:     Aug 22 2000  V0.90  iko  Works!
 *              Aug 23 2000  V0.91  iko  Cleanup, posted to etherboot
 *                                       maintainer.
 *              Aug 26 2000  V0.92  iko  Fixed Rx ring handling.
 *                                       First Linux Kernel (TM)
 *                                       successfully loaded using
 *                                       this driver.
 *              Jan 07 2001  V0.93  iko  Transmitter timeouts are handled
 *                                       using timer2 routines. Proposed
 *                                       by Ken Yap to eliminate CPU speed
 *                                       dependency.
 *
 * This is the etherboot driver for cards based on Winbond W89c840F chip.
 *
 * It was written from skeleton source, with Donald Becker's winbond-840.c
 * kernel driver as a guideline. Mostly the w89c840 related definitions
 * and the lower level routines have been cut-and-pasted into this source.
 *
 * Frankly speaking, about 90% of the code was obtained using cut'n'paste
 * sequence :) while the remainder appeared while brainstorming
 * Linux Kernel 2.4.0-testX source code. Thanks, Donald and Linus!
 *
 * There was a demand for using this card in a rather large
 * remote boot environment at MSKP OVTI Lab of
 * Moscow Institute for Physics and Technology (MIPT) -- http://www.mipt.ru/
 * so you may count that for motivation.
 *
 */



#define USE_IO_OPS

#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"
#include "timer.h"

static const char *w89c840_version = "diver Version 0.92 - August 27, 2000";

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned int   u32;
typedef   signed int   s32;

#define virt_to_bus(x) ((unsigned long)x)
#define bus_to_virt(x) ((void *)x)

#define virt_to_le32desc(addr)  virt_to_bus(addr)
#define le32desc_to_virt(addr)  bus_to_virt(addr)



#define TX_RING_SIZE    2

#define RX_RING_SIZE    2

#define TX_FIFO_SIZE (2048)
#define TX_BUG_FIFO_LIMIT (TX_FIFO_SIZE-1514-16)

#define TX_TIMEOUT  (10*TICKS_PER_MS)

#define PKT_BUF_SZ  1536  


#if !defined(__OPTIMIZE__)
#warning  You must compile this file with the correct options!
#warning  See the last lines of the source file.
#error You must compile this driver with "-O".
#endif

enum chip_capability_flags {CanHaveMII=1, HasBrokenTx=2};

#ifdef USE_IO_OPS
#define W840_FLAGS (PCI_USES_IO | PCI_ADDR0 | PCI_USES_MASTER)
#else
#define W840_FLAGS (PCI_USES_MEM | PCI_ADDR1 | PCI_USES_MASTER)
#endif

static u32 driver_flags = CanHaveMII | HasBrokenTx;

/* This driver was written to use PCI memory space, however some x86 systems
   work only with I/O space accesses.  Pass -DUSE_IO_OPS to use PCI I/O space
   accesses instead of memory space. */

#ifdef USE_IO_OPS
#undef readb
#undef readw
#undef readl
#undef writeb
#undef writew
#undef writel
#define readb inb
#define readw inw
#define readl inl
#define writeb outb
#define writew outw
#define writel outl
#endif

enum w840_offsets {
    PCIBusCfg=0x00, TxStartDemand=0x04, RxStartDemand=0x08,
    RxRingPtr=0x0C, TxRingPtr=0x10,
    IntrStatus=0x14, NetworkConfig=0x18, IntrEnable=0x1C,
    RxMissed=0x20, EECtrl=0x24, MIICtrl=0x24, BootRom=0x28, GPTimer=0x2C,
    CurRxDescAddr=0x30, CurRxBufAddr=0x34,            
    MulticastFilter0=0x38, MulticastFilter1=0x3C, StationAddr=0x40,
    CurTxDescAddr=0x4C, CurTxBufAddr=0x50,
};

enum intr_status_bits {
    NormalIntr=0x10000, AbnormalIntr=0x8000,
    IntrPCIErr=0x2000, TimerInt=0x800,
    IntrRxDied=0x100, RxNoBuf=0x80, IntrRxDone=0x40,
    TxFIFOUnderflow=0x20, RxErrIntr=0x10,
    TxIdle=0x04, IntrTxStopped=0x02, IntrTxDone=0x01,
};

enum rx_mode_bits {
    AcceptErr=0x80, AcceptRunt=0x40,
    AcceptBroadcast=0x20, AcceptMulticast=0x10,
    AcceptAllPhys=0x08, AcceptMyPhys=0x02,
};

enum mii_reg_bits {
    MDIO_ShiftClk=0x10000, MDIO_DataIn=0x80000, MDIO_DataOut=0x20000,
    MDIO_EnbOutput=0x40000, MDIO_EnbIn = 0x00000,
};

struct w840_rx_desc {
    s32 status;
    s32 length;
    u32 buffer1;
    u32 next_desc;
};

struct w840_tx_desc {
    s32 status;
    s32 length;
    u32 buffer1, buffer2;                
};

enum desc_status_bits {
    DescOwn=0x80000000, DescEndRing=0x02000000, DescUseLink=0x01000000,
    DescWholePkt=0x60000000, DescStartPkt=0x20000000, DescEndPkt=0x40000000,
    DescIntr=0x80000000,
};
#define PRIV_ALIGN    15     
#define PRIV_ALIGN_BYTES 32

static struct winbond_private
{
    
    struct w840_rx_desc rx_ring[RX_RING_SIZE];
    struct w840_tx_desc tx_ring[TX_RING_SIZE];
    struct net_device *next_module;        
    void *priv_addr;                    
    const char *product_name;
    
    int chip_id, drv_flags;
    struct pci_dev *pci_dev;
    int csr6;
    struct w840_rx_desc *rx_head_desc;
    unsigned int cur_rx, dirty_rx;        
    unsigned int rx_buf_sz;                
    unsigned int cur_tx, dirty_tx;
    int tx_q_bytes;
    unsigned int tx_full:1;                
    
    unsigned int full_duplex:1;            
    unsigned int duplex_lock:1;
    unsigned int medialock:1;            
    unsigned int default_port:4;        
    
    int mii_cnt;                        
    u16 advertising;                    
    unsigned char phys[2];                
} w840private __attribute__ ((aligned (PRIV_ALIGN_BYTES)));


static int ioaddr;
static unsigned short eeprom [0x40];

#ifdef    USE_LOWMEM_BUFFER
#define rx_packet ((char *)0x10000 - PKT_BUF_SZ * RX_RING_SIZE)
#define tx_packet ((char *)0x10000 - PKT_BUF_SZ * RX_RING_SIZE - PKT_BUF_SZ * TX_RING_SIZE)
#else
static char        rx_packet[PKT_BUF_SZ * RX_RING_SIZE];
static char        tx_packet[PKT_BUF_SZ * TX_RING_SIZE];
#endif

static int  eeprom_read(long ioaddr, int location);
static int  mdio_read(int base_address, int phy_id, int location);
static void mdio_write(int base_address, int phy_id, int location, int value);

static void check_duplex(void);
static void set_rx_mode(void);
static void init_ring(void);


#if defined W89C840_DEBUG
static void decode_interrupt(u32 intr_status)
{
    printf("Interrupt status: ");

#define TRACE_INTR(_intr_) \
    if (intr_status & (_intr_)) { printf (" " #_intr_); }

    TRACE_INTR(NormalIntr);
    TRACE_INTR(AbnormalIntr);
    TRACE_INTR(IntrPCIErr);
    TRACE_INTR(TimerInt);
    TRACE_INTR(IntrRxDied);
    TRACE_INTR(RxNoBuf);
    TRACE_INTR(IntrRxDone);
    TRACE_INTR(TxFIFOUnderflow);
    TRACE_INTR(RxErrIntr);
    TRACE_INTR(TxIdle);
    TRACE_INTR(IntrTxStopped);
    TRACE_INTR(IntrTxDone);

    printf("\n");
    
}
#endif

static void w89c840_reset(struct nic *nic)
{
    int i;

    writel(0x00000001, ioaddr + PCIBusCfg);

    init_ring();

    writel(virt_to_bus(w840private.rx_ring), ioaddr + RxRingPtr);
    writel(virt_to_bus(w840private.tx_ring), ioaddr + TxRingPtr);

    for (i = 0; i < ETH_ALEN; i++)
        writeb(nic->node_addr[i], ioaddr + StationAddr + i);

    

    writel(0xE010, ioaddr + PCIBusCfg);

    writel(0, ioaddr + RxStartDemand);
    w840private.csr6 = 0x20022002;
    check_duplex();
    set_rx_mode();

    
    writel(0x1A0F5, ioaddr + IntrStatus);
    writel(0x1A0F5, ioaddr + IntrEnable);

#if defined(W89C840_DEBUG)
    printf("winbond-840 : Done reset.\n");
#endif
}

static void handle_intr(u32 intr_stat)
{
    if ((intr_stat & (NormalIntr|AbnormalIntr)) == 0) {
        
        
    } else {
        
        writel(intr_stat & 0x001ffff, ioaddr + IntrStatus);
    }

    if (intr_stat & AbnormalIntr) {
        
        printf("\n-=- Abnormal interrupt.\n");

#if defined (W89C840_DEBUG)
        decode_interrupt(intr_stat);
#endif

        if (intr_stat & RxNoBuf) {
            
            printf("-=- <=> No receive buffers available.\n");
            writel(0, ioaddr + RxStartDemand);
        }
    }
}

static int w89c840_poll(struct nic *nic)
{
    
    
    
    int packet_received = 0;

    u32 intr_status = readl(ioaddr + IntrStatus);
     

    do {
        

        int entry = w840private.cur_rx % RX_RING_SIZE;

        struct w840_rx_desc *desc = w840private.rx_head_desc;
        s32 status = desc->status;

        if (status & DescOwn) {
            
            packet_received = 0;
            break;
        }

        if ((status & 0x38008300) != 0x0300) {
            if ((status & 0x38000300) != 0x0300) {
                
                if ((status & 0xffff) != 0x7fff) {
                    printf("winbond-840 : Oversized Ethernet frame spanned "
                           "multiple buffers, entry %d status %X !\n",
                           w840private.cur_rx, status);
                }
            } else if (status & 0x8000) {
                
#if defined(W89C840_DEBUG)
                printf("winbond-840 : Receive error, Rx status %X :", status);
                if (status & 0x0890) {
                    printf(" RXLEN_ERROR");
                }
                if (status & 0x004C) {
                    printf(", FRAME_ERROR");
                }
                if (status & 0x0002) {
                    printf(", CRC_ERROR");
                }
                printf("\n");
#endif

                
                w89c840_reset(nic);

                packet_received = 0;
                break;
            }
        } else {
            
            int pkt_len = ((status >> 16) & 0x7ff) - 4;

#if defined(W89C840_DEBUG)
            printf(" netdev_rx() normal Rx pkt ring %d length %d status %X\n", entry, pkt_len, status);
#endif

            nic->packetlen = pkt_len;


            memcpy(nic->packet, le32desc_to_virt(w840private.rx_ring[entry].buffer1), pkt_len);
            packet_received = 1;

            
            w840private.rx_ring[entry].status = DescOwn;

#if defined(W89C840_DEBUG)
            
            printf("  Rx data %hhX:%hhX:%hhX:%hhX:%hhX:"
                   "%hhX %hhX:%hhX:%hhX:%hhX:%hhX:%hhX %hhX%hhX "
                   "%hhX.%hhX.%hhX.%hhX.\n",
                   nic->packet[0],  nic->packet[1],  nic->packet[2], nic->packet[3],
                   nic->packet[4],  nic->packet[5],  nic->packet[6], nic->packet[7],
                   nic->packet[8],  nic->packet[9],  nic->packet[10],
                   nic->packet[11], nic->packet[12], nic->packet[13],
                   nic->packet[14], nic->packet[15], nic->packet[16],
                   nic->packet[17]);
#endif

        }

        entry = (++w840private.cur_rx) % RX_RING_SIZE;
        w840private.rx_head_desc = &w840private.rx_ring[entry];
    } while (0);

    if (intr_status & (AbnormalIntr | TxFIFOUnderflow | IntrPCIErr |TimerInt | IntrTxStopped)) {
        handle_intr(intr_status);
    }

    return packet_received;
}


static void w89c840_transmit(
    struct nic *nic,
    const char *d,            
    unsigned int t,            
    unsigned int s,            
    const char *p)            
{
    
    unsigned entry;
    int transmit_status;


    
    entry = w840private.cur_tx % TX_RING_SIZE;

    memcpy (tx_packet, d, ETH_ALEN);    
    memcpy (tx_packet + ETH_ALEN, nic->node_addr, ETH_ALEN);

    *((char *) tx_packet + 12) = t >> 8;    
    *((char *) tx_packet + 13) = t;

    memcpy (tx_packet + ETH_HLEN, p, s);
    s += ETH_HLEN;

    while (s < ETH_ZLEN)
    *((char *) tx_packet + ETH_HLEN + (s++)) = 0;

    w840private.tx_ring[entry].buffer1 = virt_to_le32desc(tx_packet);

    w840private.tx_ring[entry].length = (DescWholePkt | s);
    if (entry >= TX_RING_SIZE-1)         
        w840private.tx_ring[entry].length |= (DescIntr | DescEndRing);
    w840private.tx_ring[entry].status = (DescOwn);
    w840private.cur_tx++;

    w840private.tx_q_bytes += s;
    writel(0, ioaddr + TxStartDemand);


    if ((w840private.drv_flags & HasBrokenTx) && w840private.tx_q_bytes > TX_BUG_FIFO_LIMIT) {
        w840private.tx_full = 1;
    }

#if defined(W89C840_DEBUG)
    printf("winbond-840 : Transmit frame # %d size %d queued in slot %d.\n", w840private.cur_tx, s, entry);
#endif

    
    transmit_status = w840private.tx_ring[entry].status;

    load_timer2(TX_TIMEOUT);

    {
        u32 intr_stat = 0;

        while (1) {

            intr_stat = readl(ioaddr + IntrStatus);
#if defined(W89C840_DEBUG)
            decode_interrupt(intr_stat);
#endif

            if (intr_stat & (NormalIntr | IntrTxDone)) {

                while ( (transmit_status & DescOwn) && timer2_running()) {

                    transmit_status = w840private.tx_ring[entry].status;
                }

                writel(intr_stat & 0x0001ffff, ioaddr + IntrStatus);
                break;
            }
        }
    }

    if ((transmit_status & DescOwn) == 0) {

#if defined(W89C840_DEBUG)
        printf("winbond-840 : transmission complete after %d wait loop iterations, status %X\n",
               TX_LOOP_COUNT - transmit_loop_counter, w840private.tx_ring[entry].status);
#endif

        return;
    }

    

    printf("winbond-840 : transmission TIMEOUT : status %X\n", w840private.tx_ring[entry].status);

    return;
}

static void w89c840_disable(struct nic *nic)
{
    
    
    
    writel(w840private.csr6 &= ~0x20FA, ioaddr + NetworkConfig);
}

struct nic *w89c840_probe(struct nic *nic, unsigned short *probe_addrs, struct pci_device *p)
{
    u16 sum = 0;
    int i, j, to;
    unsigned short value;
    int options;
    int promisc;

    if (probe_addrs == 0 || probe_addrs[0] == 0)
        return 0;

    ioaddr = probe_addrs[0]; 

#if defined(W89C840_DEBUG)
    printf("winbond-840: PCI bus %hhX device function %hhX: I/O address: %hX\n", p->bus, p->devfn, ioaddr);
#endif

    ioaddr = ioaddr & ~3; 

    

    
    if (p->vendor == PCI_VENDOR_ID_WINBOND2
        && p->dev_id == PCI_DEVICE_ID_WINBOND2_89C840) {

        

    } else if ( p->vendor == PCI_VENDOR_ID_COMPEX
                && p->dev_id == PCI_DEVICE_ID_COMPEX_RL100ATX) {

        

    } else {
        
        printf("device ID : %X - is not a Compex RL100ATX NIC.\n", p->dev_id);
        return 0;
    }

    printf(" %s\n", w89c840_version);

    adjust_pci_device(p);

    
    for (j = 0, i = 0; i < 0x40; i++) {
        value = eeprom_read(ioaddr, i);
        eeprom[i] = value;
        sum += value;
    }

    for (i=0;i<ETH_ALEN;i++) {
        nic->node_addr[i] =  (eeprom[i/2] >> (8*(i&1))) & 0xff;
    }
    printf ("Ethernet addr: %!\n", nic->node_addr);

#if defined(W89C840_DEBUG)
    printf("winbond-840: EEPROM checksum %hX, got eeprom", sum);
#endif

    writel(0x00000001, ioaddr + PCIBusCfg);

    if (driver_flags & CanHaveMII) {
        int phy, phy_idx = 0;
        for (phy = 1; phy < 32 && phy_idx < 4; phy++) {
            int mii_status = mdio_read(ioaddr, phy, 1);
            if (mii_status != 0xffff  &&  mii_status != 0x0000) {
                w840private.phys[phy_idx++] = phy;
                w840private.advertising = mdio_read(ioaddr, phy, 4);

#if defined(W89C840_DEBUG)
                printf("winbond-840 : MII PHY found at address %d, status "
                       "%X advertising %hX.\n", phy, mii_status, w840private.advertising);
#endif

            }
        }

        w840private.mii_cnt = phy_idx;

        if (phy_idx == 0) {
                printf("winbond-840 : MII PHY not found -- this device may not operate correctly.\n");
        }
    }

    
    nic->reset = w89c840_reset;
    nic->poll = w89c840_poll;
    nic->transmit = w89c840_transmit;
    nic->disable = w89c840_disable;

    w89c840_reset(nic);

    return nic;
}


#define eeprom_delay(ee_addr)    readl(ee_addr)

enum EEPROM_Ctrl_Bits {
    EE_ShiftClk=0x02, EE_Write0=0x801, EE_Write1=0x805,
    EE_ChipSelect=0x801, EE_DataIn=0x08,
};

enum EEPROM_Cmds {
    EE_WriteCmd=(5 << 6), EE_ReadCmd=(6 << 6), EE_EraseCmd=(7 << 6),
};

static int eeprom_read(long addr, int location)
{
    int i;
    int retval = 0;
    int ee_addr = addr + EECtrl;
    int read_cmd = location | EE_ReadCmd;
    writel(EE_ChipSelect, ee_addr);

    
    for (i = 10; i >= 0; i--) {
        short dataval = (read_cmd & (1 << i)) ? EE_Write1 : EE_Write0;
        writel(dataval, ee_addr);
        eeprom_delay(ee_addr);
        writel(dataval | EE_ShiftClk, ee_addr);
        eeprom_delay(ee_addr);
    }
    writel(EE_ChipSelect, ee_addr);

    for (i = 16; i > 0; i--) {
        writel(EE_ChipSelect | EE_ShiftClk, ee_addr);
        eeprom_delay(ee_addr);
        retval = (retval << 1) | ((readl(ee_addr) & EE_DataIn) ? 1 : 0);
        writel(EE_ChipSelect, ee_addr);
        eeprom_delay(ee_addr);
    }

    
    writel(0, ee_addr);
    return retval;
}

#define mdio_delay(mdio_addr) readl(mdio_addr)

static char mii_preamble_required = 1;

#define MDIO_WRITE0 (MDIO_EnbOutput)
#define MDIO_WRITE1 (MDIO_DataOut | MDIO_EnbOutput)

static void mdio_sync(long mdio_addr)
{
    int bits = 32;

    
    while (--bits >= 0) {
        writel(MDIO_WRITE1, mdio_addr);
        mdio_delay(mdio_addr);
        writel(MDIO_WRITE1 | MDIO_ShiftClk, mdio_addr);
        mdio_delay(mdio_addr);
    }
}

static int mdio_read(int base_address, int phy_id, int location)
{
    long mdio_addr = base_address + MIICtrl;
    int mii_cmd = (0xf6 << 10) | (phy_id << 5) | location;
    int i, retval = 0;

    if (mii_preamble_required)
        mdio_sync(mdio_addr);

    
    for (i = 15; i >= 0; i--) {
        int dataval = (mii_cmd & (1 << i)) ? MDIO_WRITE1 : MDIO_WRITE0;

        writel(dataval, mdio_addr);
        mdio_delay(mdio_addr);
        writel(dataval | MDIO_ShiftClk, mdio_addr);
        mdio_delay(mdio_addr);
    }
    
    for (i = 20; i > 0; i--) {
        writel(MDIO_EnbIn, mdio_addr);
        mdio_delay(mdio_addr);
        retval = (retval << 1) | ((readl(mdio_addr) & MDIO_DataIn) ? 1 : 0);
        writel(MDIO_EnbIn | MDIO_ShiftClk, mdio_addr);
        mdio_delay(mdio_addr);
    }
    return (retval>>1) & 0xffff;
}

static void mdio_write(int base_address, int phy_id, int location, int value)
{
    long mdio_addr = base_address + MIICtrl;
    int mii_cmd = (0x5002 << 16) | (phy_id << 23) | (location<<18) | value;
    int i;

    if (location == 4  &&  phy_id == w840private.phys[0])
        w840private.advertising = value;

    if (mii_preamble_required)
        mdio_sync(mdio_addr);

    
    for (i = 31; i >= 0; i--) {
        int dataval = (mii_cmd & (1 << i)) ? MDIO_WRITE1 : MDIO_WRITE0;

        writel(dataval, mdio_addr);
        mdio_delay(mdio_addr);
        writel(dataval | MDIO_ShiftClk, mdio_addr);
        mdio_delay(mdio_addr);
    }
    
    for (i = 2; i > 0; i--) {
        writel(MDIO_EnbIn, mdio_addr);
        mdio_delay(mdio_addr);
        writel(MDIO_EnbIn | MDIO_ShiftClk, mdio_addr);
        mdio_delay(mdio_addr);
    }
    return;
}

static void check_duplex(void)
{
    int mii_reg5 = mdio_read(ioaddr, w840private.phys[0], 5);
    int negotiated =  mii_reg5 & w840private.advertising;
    int duplex;

    if (w840private.duplex_lock  ||  mii_reg5 == 0xffff)
        return;

    duplex = (negotiated & 0x0100) || (negotiated & 0x01C0) == 0x0040;
    if (w840private.full_duplex != duplex) {
        w840private.full_duplex = duplex;       

#if defined(W89C840_DEBUG)
        printf("winbond-840 : Setting %s-duplex based on MII # %d negotiated capability %X\n",
               duplex ? "full" : "half", w840private.phys[0], negotiated);
#endif

        w840private.csr6 &= ~0x200;
        w840private.csr6 |= duplex ? 0x200 : 0;
    }
}

static void set_rx_mode(void)
{
    u32 mc_filter[2];            
    u32 rx_mode;

    
    memset(mc_filter, 0xff, sizeof(mc_filter));

    rx_mode = AcceptBroadcast | AcceptMyPhys;

    writel(mc_filter[0], ioaddr + MulticastFilter0);
    writel(mc_filter[1], ioaddr + MulticastFilter1);
    w840private.csr6 &= ~0x00F8;
    w840private.csr6 |= rx_mode;
    writel(w840private.csr6, ioaddr + NetworkConfig);

#if defined(W89C840_DEBUG)
    printf("winbond-840 : Done setting RX mode.\n");
#endif
}

static void init_ring(void)
{
    int i;
    char * p;

    w840private.tx_full = 0;
    w840private.tx_q_bytes = w840private.cur_rx = w840private.cur_tx = 0;
    w840private.dirty_rx = w840private.dirty_tx = 0;

    w840private.rx_buf_sz = PKT_BUF_SZ;
    w840private.rx_head_desc = &w840private.rx_ring[0];

    

    p = &rx_packet[0];

    for (i = 0; i < RX_RING_SIZE; i++) {
        w840private.rx_ring[i].length = w840private.rx_buf_sz;
        w840private.rx_ring[i].status = 0;
        w840private.rx_ring[i].next_desc = virt_to_le32desc(&w840private.rx_ring[i+1]);

        w840private.rx_ring[i].buffer1 = virt_to_le32desc(p + (PKT_BUF_SZ * i));
        w840private.rx_ring[i].status = DescOwn | DescIntr;
    }

    
    w840private.rx_ring[i-1].length |= DescEndRing;
    w840private.rx_ring[i-1].next_desc = virt_to_le32desc(&w840private.rx_ring[0]);

    w840private.dirty_rx = (unsigned int)(i - RX_RING_SIZE);

    for (i = 0; i < TX_RING_SIZE; i++) {
        w840private.tx_ring[i].status = 0;
    }
    return;
}
