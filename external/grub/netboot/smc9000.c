 /*------------------------------------------------------------------------
 * smc9000.c
 * This is a Etherboot driver for SMC's 9000 series of Ethernet cards.
 *
 * Copyright (C) 1998 Daniel Engström <daniel.engstrom@riksnett.no>
 * Based on the Linux SMC9000 driver, smc9194.c by Eric Stahlman
 * Copyright (C) 1996 by Erik Stahlman <eric@vt.edu>
 *
 * This software may be used and distributed according to the terms
 * of the GNU Public License, incorporated herein by reference.
 *
 * "Features" of the SMC chip:
 *   4608 byte packet memory. ( for the 91C92/4.  Others have more )
 *   EEPROM for configuration
 *   AUI/TP selection
 *
 * Authors
 *	Erik Stahlman				<erik@vt.edu>
 *      Daniel Engström                         <daniel.engstrom@riksnett.no>
 *
 * History
 * 98-09-25              Daniel Engström Etherboot driver crated from Eric's
 *                                       Linux driver.
 *
 *---------------------------------------------------------------------------*/
#define LINUX_OUT_MACROS 1
#define SMC9000_VERBOSE  1
#define SMC9000_DEBUG    0

#include "etherboot.h"
#include "nic.h"
#include "cards.h"
#include "smc9000.h"

# define _outb outb
# define _outw outw

static const char       smc9000_version[] = "Version 0.99 98-09-30";
static unsigned int	smc9000_base=0;
static const char       *interfaces[ 2 ] = { "TP", "AUI" };
static const char       *chip_ids[ 15 ] =  {
   NULL, NULL, NULL,
    "SMC91C90/91C92",
    "SMC91C94",
    "SMC91C95",
   NULL,
    "SMC91C100",
    "SMC91C100FD",
   NULL, NULL, NULL,
   NULL, NULL, NULL
};
static const char      smc91c96_id[] = "SMC91C96";

static void smc_reset(int ioaddr)
{
   SMC_SELECT_BANK(ioaddr, 0);
   _outw( RCR_SOFTRESET, ioaddr + RCR );

   
   SMC_DELAY(ioaddr);

   _outw(RCR_CLEAR, ioaddr + RCR);
   _outw(TCR_CLEAR, ioaddr + TCR);

   
   SMC_SELECT_BANK(ioaddr, 2);
   _outw( MC_RESET, ioaddr + MMU_CMD );

   _outb(0, ioaddr + INT_MASK);
}


static int smc_probe( int ioaddr )
{
   word bank;
   word	revision_register;
   word base_address_register;

   
   bank = inw(ioaddr + BANK_SELECT);
   if ((bank & 0xFF00) != 0x3300) {
      return -1;
   }
   _outw(0x0, ioaddr + BANK_SELECT);
   bank = inw(ioaddr + BANK_SELECT);
   if ((bank & 0xFF00) != 0x3300) {
      return -1;
   }

   /* well, we've already written once, so hopefully another time won't
    *  hurt.  This time, I need to switch the bank register to bank 1,
    *  so I can access the base address register */
   SMC_SELECT_BANK(ioaddr, 1);
   base_address_register = inw(ioaddr + BASE);

   if (ioaddr != (base_address_register >> 3 & 0x3E0))  {
#ifdef	SMC9000_VERBOSE
      printf("SMC9000: IOADDR %hX doesn't match configuration (%hX)."
	     "Probably not a SMC chip\n",
	     ioaddr, base_address_register >> 3 & 0x3E0);
#endif
      return -1;
   }


   SMC_SELECT_BANK(ioaddr, 3);
   revision_register  = inw(ioaddr + REVISION);
   if (!chip_ids[(revision_register >> 4) & 0xF]) {
      
#ifdef	SMC9000_VERBOSE
      printf("SMC9000: IO %hX: Unrecognized revision register:"
	     " %hX, Contact author.\n", ioaddr, revision_register);
#endif
      return -1;
   }

   return 0;
}



static void smc9000_reset(struct nic *nic)
{
   smc_reset(smc9000_base);
}

static void smc9000_transmit(
	struct nic *nic,
	const char *d,			
	unsigned int t,			
	unsigned int s,			
	const char *p)			
{
   word length; 
   word numPages;
   unsigned long time_out;
   byte	packet_no;
   word status;
   int i;

   
   length = (s + ETH_HLEN + 1)&~1;

   
   numPages = length / 256;

   if (numPages > 7 ) {
#ifdef	SMC9000_VERBOSE
      printf("SMC9000: Far too big packet error. \n");
#endif
      return;
   }

   
   for (i=0;i<30;i++) {
      
      SMC_SELECT_BANK(smc9000_base, 2);
      _outw(MC_ALLOC | numPages, smc9000_base + MMU_CMD);

      status = 0;
      
      for (time_out = currticks() + 5*TICKS_PER_SEC; currticks() < time_out; ) {
	 status = inb(smc9000_base + INTERRUPT);
	 if ( status & IM_ALLOC_INT ) {
	    
	    _outb(IM_ALLOC_INT, smc9000_base + INTERRUPT);
	    break;
	 }
      }

      if ((status & IM_ALLOC_INT) != 0 ) {
	 
	 break;
      } else {
	 printf("SMC9000: Memory allocation timed out, resetting MMU.\n");
	 _outw(MC_RESET, smc9000_base + MMU_CMD);
      }
   }

   
   packet_no = inb(smc9000_base + PNR_ARR + 1);
   if (packet_no & 0x80) {
      
      printf("SMC9000: Memory allocation failed. \n");
      return;
   }

   
   _outb(packet_no, smc9000_base + PNR_ARR);

   
   _outw(PTR_AUTOINC, smc9000_base + POINTER);

#if	SMC9000_DEBUG > 2
   printf("Trying to xmit packet of length %hX\n", length );
#endif

   _outw(0, smc9000_base + DATA_1 );

   
   _outb((length+6) & 0xFF,  smc9000_base + DATA_1);
   _outb((length+6) >> 8 ,   smc9000_base + DATA_1);

   

   
   outsw(smc9000_base + DATA_1, d, ETH_ALEN >> 1);
   outsw(smc9000_base + DATA_1, nic->node_addr, ETH_ALEN >> 1);
   _outw(htons(t), smc9000_base + DATA_1);

   
   outsw(smc9000_base + DATA_1 , p, s >> 1);

   
   if ((s & 1) == 0) {
      _outw(0, smc9000_base + DATA_1);
   } else {
      _outb(p[s-1], smc9000_base + DATA_1);
      _outb(0x20, smc9000_base + DATA_1);
   }

   
   _outw(MC_ENQUEUE , smc9000_base + MMU_CMD);

   status = 0; time_out = currticks() + 5*TICKS_PER_SEC;
   do {
      status = inb(smc9000_base + INTERRUPT);

      if ((status & IM_TX_INT ) != 0) {
	 word tx_status;

	 
	 _outb(IM_TX_INT, smc9000_base + INTERRUPT);

	 packet_no = inw(smc9000_base + FIFO_PORTS);
	 packet_no &= 0x7F;

	 
	 _outb( packet_no, smc9000_base + PNR_ARR );

	 
	 _outw( PTR_AUTOINC | PTR_READ, smc9000_base + POINTER );

	 tx_status = inw( smc9000_base + DATA_1 );

	 if (0 == (tx_status & TS_SUCCESS)) {
#ifdef	SMC9000_VERBOSE
	    printf("SMC9000: TX FAIL STATUS: %hX \n", tx_status);
#endif
	    
	    SMC_SELECT_BANK(smc9000_base, 0);
	    _outw(inw(smc9000_base + TCR ) | TCR_ENABLE, smc9000_base + TCR );
	 }

	 
	 SMC_SELECT_BANK(smc9000_base, 2);
	 _outw(MC_FREEPKT, smc9000_base + MMU_CMD);

	 return;
      }
   }while(currticks() < time_out);

   printf("SMC9000: Waring TX timed out, resetting board\n");
   smc_reset(smc9000_base);
   return;
}

static int smc9000_poll(struct nic *nic)
{
   if(!smc9000_base)
     return 0;

   SMC_SELECT_BANK(smc9000_base, 2);
   if (inw(smc9000_base + FIFO_PORTS) & FP_RXEMPTY)
     return 0;

   
   _outw(PTR_READ | PTR_RCV | PTR_AUTOINC, smc9000_base + POINTER);

   
   if (!(inw(smc9000_base + DATA_1) & RS_ERRORS)) {
      
      nic->packetlen = (inw(smc9000_base + DATA_1) & 0x07ff);

      
      nic->packetlen -= 6;
#if	SMC9000_DEBUG > 2
      printf(" Reading %d words (and %d byte(s))\n",
	       (nic->packetlen >> 1), nic->packetlen & 1);
#endif
      
      insw(smc9000_base + DATA_1, nic->packet, (nic->packetlen+2) >> 1);
      
      if (nic->packet[nic->packetlen+1] & 0x20)
	 nic->packetlen++;

      
      _outw(MC_RELEASE, smc9000_base + MMU_CMD);
      return 1;
   }

   printf("SMC9000: RX error\n");
   
   _outw(MC_RELEASE, smc9000_base + MMU_CMD);
   return 0;
}

static void smc9000_disable(struct nic *nic)
{
   if(!smc9000_base)
     return;

   
   SMC_SELECT_BANK(smc9000_base, 2);
   _outb( 0, smc9000_base + INT_MASK);

   
   SMC_SELECT_BANK(smc9000_base, 0);
   _outb( RCR_CLEAR, smc9000_base + RCR );
   _outb( TCR_CLEAR, smc9000_base + TCR );
}


struct nic *smc9000_probe(struct nic *nic, unsigned short *probe_addrs)
{
   unsigned short   revision;
   int	            memory;
   int              media;
   const char *	    version_string;
   const char *	    if_string;
   int              i;

   static unsigned short portlist[] = {
#ifdef	SMC9000_SCAN
      SMC9000_SCAN,
#else
      0x200, 0x220, 0x240, 0x260, 0x280, 0x2A0, 0x2C0, 0x2E0,
      0x300, 0x320, 0x340, 0x360, 0x380, 0x3A0, 0x3C0, 0x3E0,
#endif
      0 };

   printf("\nSMC9000 %s\n", smc9000_version);
#ifdef	SMC9000_VERBOSE
   printf("Copyright (C) 1998 Daniel Engstr\x94m\n");
   printf("Copyright (C) 1996 Eric Stahlman\n");
#endif
   
   if (probe_addrs == 0 || probe_addrs[0] == 0)
     probe_addrs = portlist;

   
   for (i = 0; probe_addrs[i]; i++) {
      
      if (smc_probe(probe_addrs[i]) == 0)
	smc9000_base = probe_addrs[i];
   }

   
   if(0 == smc9000_base)
     goto out;

   SMC_SELECT_BANK(smc9000_base, 1);
   for ( i = 0; i < 6; i += 2 ) {
      word address;

      address = inw(smc9000_base + ADDR0 + i);
      nic->node_addr[i+1] = address >> 8;
      nic->node_addr[i] = address & 0xFF;
   }


   
   SMC_SELECT_BANK(smc9000_base, 0);
   memory = ( inw(smc9000_base + MCR) >> 9 )  & 0x7;  
   memory *= 256 * (inw(smc9000_base + MIR) & 0xFF);

   SMC_SELECT_BANK(smc9000_base, 3);
   revision  = inw(smc9000_base + REVISION);
   version_string = chip_ids[(revision >> 4) & 0xF];

   if (((revision & 0xF0) >> 4 == CHIP_9196) &&
       ((revision & 0x0F) >= REV_9196)) {
      version_string = smc91c96_id;
   }

   if ( !version_string ) {
      
      goto out;
   }

   
   SMC_SELECT_BANK(smc9000_base, 1);
   if (inw(smc9000_base + CONFIG) & CFG_AUI_SELECT)
     media = 2;
   else
     media = 1;

   if_string = interfaces[media - 1];

   
   smc_reset(smc9000_base);

   printf("%s rev:%d I/O port:%hX Interface:%s RAM:%d bytes \n",
	  version_string, revision & 0xF,
	  smc9000_base, if_string, memory );
   printf("Ethernet MAC address: %!\n", nic->node_addr);

   SMC_SELECT_BANK(smc9000_base, 0);

   
   _outw(TCR_NORMAL, smc9000_base + TCR);
   _outw(RCR_NORMAL, smc9000_base + RCR);

   
   SMC_SELECT_BANK(smc9000_base, 1);
   if ( media == 1 ) {
      _outw( inw( smc9000_base + CONFIG ) & ~CFG_AUI_SELECT,
	   smc9000_base + CONFIG );
   }
   else if ( media == 2 ) {
      _outw( inw( smc9000_base + CONFIG ) | CFG_AUI_SELECT,
	   smc9000_base + CONFIG );
   }

   nic->reset = smc9000_reset;
   nic->poll = smc9000_poll;
   nic->transmit = smc9000_transmit;
   nic->disable = smc9000_disable;


   return nic;

out:
#ifdef	SMC9000_VERBOSE
   printf("No SMC9000 adapters found\n");
#endif
   smc9000_base = 0;

   return (0);
}



