/*
 * eepro100.c -- This file implements the eepro100 driver for etherboot.
 *
 *
 * Copyright (C) AW Computer Systems.
 * written by R.E.Wolff -- R.E.Wolff@BitWizard.nl
 *
 *
 * AW Computer Systems is contributing to the free software community
 * by paying for this driver and then putting the result under GPL.
 *
 * If you need a Linux device driver, please contact BitWizard for a
 * quote.
 *
 *
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
 *
 *
 *              date       version  by   what
 *  Written:    May 29 1997  V0.10  REW  Initial revision.
 * changes:     May 31 1997  V0.90  REW  Works!
 *              Jun 1  1997  V0.91  REW  Cleanup
 *              Jun 2  1997  V0.92  REW  Add some code documentation
 *              Jul 25 1997  V1.00  REW  Tested by AW to work in a PROM
 *                                       Cleanup for publication
 *
 * This is the etherboot intel etherexpress Pro/100B driver.
 *
 * It was written from scratch, with Donald Beckers eepro100.c kernel
 * driver as a guideline. Mostly the 82557 related definitions and the
 * lower level routines have been cut-and-pasted into this source.
 *
 * The driver was finished before Intel got the NDA out of the closet.
 * I still don't have the docs.
 * */



#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"
#include "timer.h"

#undef	virt_to_bus
#define	virt_to_bus(x)	((unsigned long)x)

static int ioaddr;

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned int   u32;
typedef   signed int   s32;

enum speedo_offsets {
  SCBStatus = 0, SCBCmd = 2,      
  SCBPointer = 4,                 
  SCBPort = 8,                    
  SCBflash = 12, SCBeeprom = 14,  
  SCBCtrlMDI = 16,                
  SCBEarlyRx = 20,                
};

static int do_eeprom_cmd(int cmd, int cmd_len);
void hd(void *where, int n);


#define EE_SHIFT_CLK    0x01    
#define EE_CS           0x02    
#define EE_DATA_WRITE   0x04    
#define EE_DATA_READ    0x08    
#define EE_WRITE_0      0x4802
#define EE_WRITE_1      0x4806
#define EE_ENB          (0x4800 | EE_CS)

#define udelay(n)       waiton_timer2(((n)*TICKS_PER_MS)/1000)

#define EE_READ_CMD     6

#define  CU_START       0x0010
#define  CU_RESUME      0x0020
#define  CU_STATSADDR   0x0040
#define  CU_SHOWSTATS   0x0050  
#define  CU_CMD_BASE    0x0060  
#define  CU_DUMPSTATS   0x0070  

#define  RX_START       0x0001
#define  RX_RESUME      0x0002
#define  RX_ABORT       0x0004
#define  RX_ADDR_LOAD   0x0006
#define  RX_RESUMENR    0x0007
#define INT_MASK        0x0100
#define DRVR_INT        0x0200          

enum phy_chips { NonSuchPhy=0, I82553AB, I82553C, I82503, DP83840, S80C240,
                                         S80C24, PhyUndefined, DP83840A=10, };

enum commands {
  CmdNOp = 0,
  CmdIASetup = 1,
  CmdConfigure = 2,
  CmdMulticastList = 3,
  CmdTx = 4,
  CmdTDR = 5,
  CmdDump = 6,
  CmdDiagnose = 7,

  
  CmdSuspend = 0x4000,      
  CmdIntr = 0x2000,         
  CmdTxFlex = 0x0008,       
};

static inline void wait_for_cmd_done(int cmd_ioaddr)
{
  short wait = 100;
  do   ;
  while(inb(cmd_ioaddr) && --wait >= 0);
}

static struct speedo_stats {
        u32 tx_good_frames;
        u32 tx_coll16_errs;
        u32 tx_late_colls;
        u32 tx_underruns;
        u32 tx_lost_carrier;
        u32 tx_deferred;
        u32 tx_one_colls;
        u32 tx_multi_colls;
        u32 tx_total_colls;
        u32 rx_good_frames;
        u32 rx_crc_errs;
        u32 rx_align_errs;
        u32 rx_resource_errs;
        u32 rx_overrun_errs;
        u32 rx_colls_errs;
        u32 rx_runt_errs;
        u32 done_marker;
} lstats;

static struct TxFD {
  volatile s16 status;
  s16 command;
  u32 link;          
  u32 tx_desc_addr;  
  s32 count;         
                     
  u32 tx_buf_addr0;  
  s32 tx_buf_size0;  
  u32 tx_buf_addr1;  
  s32 tx_buf_size1;  
} txfd;

struct RxFD {               
  volatile s16 status;
  s16 command;
  u32 link;                 
  u32 rx_buf_addr;          
  u16 count;
  u16 size;
  char packet[1518];
};

#ifdef	USE_LOWMEM_BUFFER
#define rxfd ((struct RxFD *)(0x10000 - sizeof(struct RxFD)))
#define ACCESS(x) x->
#else
static struct RxFD rxfd;
#define ACCESS(x) x.
#endif

static int congenb = 0;         
static int txfifo = 8;          
static int rxfifo = 8;          
static int txdmacount = 0;      
static int rxdmacount = 0;      

static struct ConfCmd {
  s16 status;
  s16 command;
  u32 link;
  unsigned char data[22];
} confcmd = {
  0, CmdConfigure,
  (u32) & txfd,
  {22, 0x08, 0, 0,  0, 0x80, 0x32, 0x03,  1, 
   0, 0x2E, 0,  0x60, 0,
   0xf2, 0x48,   0, 0x40, 0xf2, 0x80,        
   0x3f, 0x05, }
};



static int mdio_write(int phy_id, int location, int value)
{
  int val, boguscnt = 64*4;         

  outl(0x04000000 | (location<<16) | (phy_id<<21) | value,
       ioaddr + SCBCtrlMDI);
  do {
    udelay(16);

    val = inl(ioaddr + SCBCtrlMDI);
    if (--boguscnt < 0) {
      printf(" mdio_write() timed out with val = %X.\n", val);
    }
  } while (! (val & 0x10000000));
  return val & 0xffff;
}

static int mdio_read(int phy_id, int location)
{
  int val, boguscnt = 64*4;               
  outl(0x08000000 | (location<<16) | (phy_id<<21), ioaddr + SCBCtrlMDI);
  do {
    udelay(16);

    val = inl(ioaddr + SCBCtrlMDI);
    if (--boguscnt < 0) {
      printf( " mdio_read() timed out with val = %X.\n", val);
    }
  } while (! (val & 0x10000000));
  return val & 0xffff;
}

static int do_eeprom_cmd(int cmd, int cmd_len)
{
	unsigned retval = 0;
	long ee_addr = ioaddr + SCBeeprom;

	outw(EE_ENB, ee_addr); udelay(2);
	outw(EE_ENB | EE_SHIFT_CLK, ee_addr); udelay(2);

	
	do {
		short dataval = (cmd & (1 << cmd_len)) ? EE_WRITE_1 : EE_WRITE_0;
		outw(dataval, ee_addr); udelay(2);
		outw(dataval | EE_SHIFT_CLK, ee_addr); udelay(2);
		retval = (retval << 1) | ((inw(ee_addr) & EE_DATA_READ) ? 1 : 0);
	} while (--cmd_len >= 0);
	outw(EE_ENB, ee_addr); udelay(2);

	
	outw(EE_ENB & ~EE_CS, ee_addr);
	return retval;
}

static inline void whereami (const char *str)
{
#if	0
  printf ("%s\n", str);
  sleep (2);
#endif
}


static void eepro100_reset(struct nic *nic)
{
  outl(0, ioaddr + SCBPort);
}


static void eepro100_transmit(struct nic *nic, const char *d, unsigned int t, unsigned int s, const char *p)
{
  struct eth_hdr {
    unsigned char dst_addr[ETH_ALEN];
    unsigned char src_addr[ETH_ALEN];
    unsigned short type;
  } hdr;
  unsigned short status;
  int to;
  int s1, s2;

  status = inw(ioaddr + SCBStatus);
  
  outw(status & 0xfc00, ioaddr + SCBStatus);

#ifdef	DEBUG
  printf ("transmitting type %hX packet (%d bytes). status = %hX, cmd=%hX\n",
	  t, s, status, inw (ioaddr + SCBCmd));
#endif

  memcpy (&hdr.dst_addr, d, ETH_ALEN);
  memcpy (&hdr.src_addr, nic->node_addr, ETH_ALEN);

  hdr.type = htons (t);

  txfd.status = 0;
  txfd.command = CmdSuspend | CmdTx | CmdTxFlex;
  txfd.link   = virt_to_bus (&txfd);
  txfd.count   = 0x02208000;
  txfd.tx_desc_addr = (u32)&txfd.tx_buf_addr0;

  txfd.tx_buf_addr0 = virt_to_bus (&hdr);
  txfd.tx_buf_size0 = sizeof (hdr);

  txfd.tx_buf_addr1 = virt_to_bus (p);
  txfd.tx_buf_size1 = s;

#ifdef	DEBUG
  printf ("txfd: \n");
  hd (&txfd, sizeof (txfd));
#endif

  outl(virt_to_bus(&txfd), ioaddr + SCBPointer);
  outw(INT_MASK | CU_START, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  s1 = inw (ioaddr + SCBStatus);
  load_timer2(10*TICKS_PER_MS);		
  while (!txfd.status && timer2_running())
    ;
  s2 = inw (ioaddr + SCBStatus);

#ifdef	DEBUG
  printf ("s1 = %hX, s2 = %hX.\n", s1, s2);
#endif
}


static int eepro100_poll(struct nic *nic)
{
  if (!ACCESS(rxfd)status)
    return 0;

  
  ACCESS(rxfd)status = 0;
  ACCESS(rxfd)command = 0xc000;
  outl(virt_to_bus(&(ACCESS(rxfd)status)), ioaddr + SCBPointer);
  outw(INT_MASK | RX_START, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

#ifdef	DEBUG
  printf ("Got a packet: Len = %d.\n", ACCESS(rxfd)count & 0x3fff);
#endif
  nic->packetlen =  ACCESS(rxfd)count & 0x3fff;
  memcpy (nic->packet, ACCESS(rxfd)packet, nic->packetlen);
#ifdef	DEBUG
  hd (nic->packet, 0x30);
#endif
  return 1;
}

static void eepro100_disable(struct nic *nic)
{
    outl(2, ioaddr + SCBPort);
}


struct nic *eepro100_probe(struct nic *nic, unsigned short *probeaddrs, struct pci_device *p)
{
	unsigned short sum = 0;
	int i;
	int read_cmd, ee_size;
	unsigned short value;
	int options;
	int promisc;

	unsigned short eeprom[16];

	if (probeaddrs == 0 || probeaddrs[0] == 0)
		return 0;
	ioaddr = probeaddrs[0] & ~3; 

	adjust_pci_device(p);

	if ((do_eeprom_cmd(EE_READ_CMD << 24, 27) & 0xffe0000)
		== 0xffe0000) {
		ee_size = 0x100;
		read_cmd = EE_READ_CMD << 24;
	} else {
		ee_size = 0x40;
		read_cmd = EE_READ_CMD << 22;
	}

	for (i = 0, sum = 0; i < ee_size; i++) {
		unsigned short value = do_eeprom_cmd(read_cmd | (i << 16), 27);
		if (i < (int)(sizeof(eeprom)/sizeof(eeprom[0])))
			eeprom[i] = value;
		sum += value;
	}

  for (i=0;i<ETH_ALEN;i++) {
	nic->node_addr[i] =  (eeprom[i/2] >> (8*(i&1))) & 0xff;
  }
  printf ("Ethernet addr: %!\n", nic->node_addr);

  if (sum != 0xBABA)
	printf("eepro100: Invalid EEPROM checksum %#hX, "
	       "check settings before activating this device!\n", sum);
  outl(0, ioaddr + SCBPort);
  udelay (10000);

  whereami ("Got eeprom.");

  outl(virt_to_bus(&lstats), ioaddr + SCBPointer);
  outw(INT_MASK | CU_STATSADDR, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  whereami ("set stats addr.");
  

  
  outl(0, ioaddr + SCBPointer);
  outw(INT_MASK | RX_ADDR_LOAD, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  whereami ("set rx base addr.");

  ACCESS(rxfd)status  = 0x0001;
  ACCESS(rxfd)command = 0x0000;
  ACCESS(rxfd)link    = virt_to_bus(&(ACCESS(rxfd)status));
  ACCESS(rxfd)rx_buf_addr = (int) &nic->packet;
  ACCESS(rxfd)count   = 0;
  ACCESS(rxfd)size    = 1528;

  outl(virt_to_bus(&(ACCESS(rxfd)status)), ioaddr + SCBPointer);
  outw(INT_MASK | RX_START, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  whereami ("started RX process.");

  
  ACCESS(rxfd)status = 0;
  ACCESS(rxfd)command = 0xc000;
  outl(virt_to_bus(&(ACCESS(rxfd)status)), ioaddr + SCBPointer);
  outw(INT_MASK | RX_START, ioaddr + SCBCmd);

  

  
  outl(0, ioaddr + SCBPointer);
  outw(INT_MASK | CU_CMD_BASE, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  whereami ("set TX base addr.");

  txfd.command      = (CmdIASetup);
  txfd.status       = 0x0000;
  txfd.link         = virt_to_bus (&confcmd);

  {
	char *t = (char *)&txfd.tx_desc_addr;

	for (i=0;i<ETH_ALEN;i++)
		t[i] = nic->node_addr[i];
  }

#ifdef	DEBUG
  printf ("Setup_eaddr:\n");
  hd (&txfd, 0x20);
#endif
   
  options = 0x00;            

  promisc = 0;

  if (   ((eeprom[6]>>8) & 0x3f) == DP83840
	  || ((eeprom[6]>>8) & 0x3f) == DP83840A) {
	int mdi_reg23 = mdio_read(eeprom[6] & 0x1f, 23) | 0x0422;
	if (congenb)
	  mdi_reg23 |= 0x0100;
	printf("  DP83840 specific setup, setting register 23 to %hX.\n",
	       mdi_reg23);
	mdio_write(eeprom[6] & 0x1f, 23, mdi_reg23);
  }
  whereami ("Done DP8340 special setup.");
  if (options != 0) {
	mdio_write(eeprom[6] & 0x1f, 0,
		   ((options & 0x20) ? 0x2000 : 0) |    
		   ((options & 0x10) ? 0x0100 : 0)); 
	whereami ("set mdio_register.");
  }

  confcmd.command  = CmdSuspend | CmdConfigure;
  confcmd.status   = 0x0000;
  confcmd.link     = virt_to_bus (&txfd);
  confcmd.data[1]  = (txfifo << 4) | rxfifo;
  confcmd.data[4]  = rxdmacount;
  confcmd.data[5]  = txdmacount + 0x80;
  confcmd.data[15] = promisc ? 0x49: 0x48;
  confcmd.data[19] = (options & 0x10) ? 0xC0 : 0x80;
  confcmd.data[21] = promisc ? 0x0D: 0x05;

  outl(virt_to_bus(&txfd), ioaddr + SCBPointer);
  outw(INT_MASK | CU_START, ioaddr + SCBCmd);
  wait_for_cmd_done(ioaddr + SCBCmd);

  whereami ("started TX thingy (config, iasetup).");

  load_timer2(10*TICKS_PER_MS);
  while (!txfd.status && timer2_running())
	;

  nic->reset = eepro100_reset;
  nic->poll = eepro100_poll;
  nic->transmit = eepro100_transmit;
  nic->disable = eepro100_disable;
  return nic;
}


#ifdef	DEBUG

void hd (void *where, int n)
{
  int i;

  while (n > 0) {
    printf ("%X ", where);
    for (i=0;i < ( (n>16)?16:n);i++)
      printf (" %hhX", ((char *)where)[i]);
    printf ("\n");
    n -= 16;
    where += 16;
  }
}
#endif

