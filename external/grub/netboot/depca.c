/*  depca.c: A DIGITAL DEPCA  & EtherWORKS ethernet driver for linux.

    Written 1994, 1995 by David C. Davies.


                      Copyright 1994 David C. Davies
		                   and 
			 United States Government
	 (as represented by the Director, National Security Agency).  

               Copyright 1995  Digital Equipment Corporation.


    This software may be used and distributed according to the terms of
    the GNU Public License, incorporated herein by reference.

    This driver is written for the Digital Equipment Corporation series
    of DEPCA and EtherWORKS ethernet cards:

        DEPCA       (the original)
    	DE100
    	DE101
	DE200 Turbo
	DE201 Turbo
	DE202 Turbo (TP BNC)
	DE210
	DE422       (EISA)

    The  driver has been tested on DE100, DE200 and DE202 cards  in  a
    relatively busy network. The DE422 has been tested a little.

    This  driver will NOT work   for the DE203,  DE204  and DE205 series  of
    cards,  since they have  a  new custom ASIC in   place of the AMD  LANCE
    chip.  See the 'ewrk3.c'   driver in the  Linux  source tree for running
    those cards.

    I have benchmarked the driver with a  DE100 at 595kB/s to (542kB/s from)
    a DECstation 5000/200.

    The author may be reached at davies@maniac.ultranet.com

    =========================================================================

    The  driver was originally based  on   the 'lance.c' driver from  Donald
    Becker   which  is included with  the  standard  driver distribution for
    linux.  V0.4  is  a complete  re-write  with only  the kernel  interface
    remaining from the original code.

    1) Lance.c code in /linux/drivers/net/
    2) "Ethernet/IEEE 802.3 Family. 1992 World Network Data Book/Handbook",
       AMD, 1992 [(800) 222-9323].
    3) "Am79C90 CMOS Local Area Network Controller for Ethernet (C-LANCE)",
       AMD, Pub. #17881, May 1993.
    4) "Am79C960 PCnet-ISA(tm), Single-Chip Ethernet Controller for ISA",
       AMD, Pub. #16907, May 1992
    5) "DEC EtherWORKS LC Ethernet Controller Owners Manual",
       Digital Equipment corporation, 1990, Pub. #EK-DE100-OM.003
    6) "DEC EtherWORKS Turbo Ethernet Controller Owners Manual",
       Digital Equipment corporation, 1990, Pub. #EK-DE200-OM.003
    7) "DEPCA Hardware Reference Manual", Pub. #EK-DEPCA-PR
       Digital Equipment Corporation, 1989
    8) "DEC EtherWORKS Turbo_(TP BNC) Ethernet Controller Owners Manual",
       Digital Equipment corporation, 1991, Pub. #EK-DE202-OM.001
    

    Peter Bauer's depca.c (V0.5) was referred to when debugging V0.1 of this
    driver.

    The original DEPCA  card requires that the  ethernet ROM address counter
    be enabled to count and has an 8 bit NICSR.  The ROM counter enabling is
    only  done when a  0x08 is read as the  first address octet (to minimise
    the chances  of writing over some  other hardware's  I/O register).  The
    NICSR accesses   have been changed  to  byte accesses  for all the cards
    supported by this driver, since there is only one  useful bit in the MSB
    (remote boot timeout) and it  is not used.  Also, there  is a maximum of
    only 48kB network  RAM for this  card.  My thanks  to Torbjorn Lindh for
    help debugging all this (and holding my feet to  the fire until I got it
    right).

    The DE200  series  boards have  on-board 64kB  RAM for  use  as a shared
    memory network  buffer. Only the DE100  cards make use  of a  2kB buffer
    mode which has not  been implemented in  this driver (only the 32kB  and
    64kB modes are supported [16kB/48kB for the original DEPCA]).

    At the most only 2 DEPCA cards can  be supported on  the ISA bus because
    there is only provision  for two I/O base addresses  on each card (0x300
    and 0x200). The I/O address is detected by searching for a byte sequence
    in the Ethernet station address PROM at the expected I/O address for the
    Ethernet  PROM.   The shared memory  base   address  is 'autoprobed'  by
    looking  for the self  test PROM  and detecting the  card name.   When a
    second  DEPCA is  detected,  information  is   placed in the   base_addr
    variable of the  next device structure (which  is created if necessary),
    thus  enabling ethif_probe  initialization  for the device.  More than 2
    EISA cards can  be  supported, but  care will  be  needed assigning  the
    shared memory to ensure that each slot has the  correct IRQ, I/O address
    and shared memory address assigned.

    ************************************************************************

    NOTE: If you are using two  ISA DEPCAs, it is  important that you assign
    the base memory addresses correctly.   The  driver autoprobes I/O  0x300
    then 0x200.  The  base memory address for  the first device must be less
    than that of the second so that the auto probe will correctly assign the
    I/O and memory addresses on the same card.  I can't think of a way to do
    this unambiguously at the moment, since there is nothing on the cards to
    tie I/O and memory information together.

    I am unable  to  test  2 cards   together for now,    so this  code   is
    unchecked. All reports, good or bad, are welcome.

    ************************************************************************

    The board IRQ   setting must be  at an  unused IRQ which  is auto-probed
    using Donald Becker's autoprobe routines. DEPCA and DE100 board IRQs are
    {2,3,4,5,7}, whereas the  DE200 is at {5,9,10,11,15}.  Note that IRQ2 is
    really IRQ9 in machines with 16 IRQ lines.

    No 16MB memory  limitation should exist with this  driver as DMA is  not
    used and the common memory area is in low memory on the network card (my
    current system has 20MB and I've not had problems yet).

    The ability to load this driver as a loadable module has been added. To
    utilise this ability, you have to do <8 things:

    0) have a copy of the loadable modules code installed on your system.
    1) copy depca.c from the  /linux/drivers/net directory to your favourite
    temporary directory.
    2) if you wish, edit the  source code near  line 1530 to reflect the I/O
    address and IRQ you're using (see also 5).
    3) compile  depca.c, but include -DMODULE in  the command line to ensure
    that the correct bits are compiled (see end of source code).
    4) if you are wanting to add a new  card, goto 5. Otherwise, recompile a
    kernel with the depca configuration turned off and reboot.
    5) insmod depca.o [irq=7] [io=0x200] [mem=0xd0000] [adapter_name=DE100]
       [Alan Cox: Changed the code to allow command line irq/io assignments]
       [Dave Davies: Changed the code to allow command line mem/name
                                                                assignments]
    6) run the net startup bits for your eth?? interface manually 
    (usually /etc/rc.inet[12] at boot time). 
    7) enjoy!

    Note that autoprobing is not allowed in loadable modules - the system is
    already up and running and you're messing with interrupts.

    To unload a module, turn off the associated interface 
    'ifconfig eth?? down' then 'rmmod depca'.

    To assign a base memory address for the shared memory  when running as a
    loadable module, see 5 above.  To include the adapter  name (if you have
    no PROM  but know the card name)  also see 5  above. Note that this last
    option  will not work  with kernel  built-in  depca's. 

    The shared memory assignment for a loadable module  makes sense to avoid
    the 'memory autoprobe' picking the wrong shared memory  (for the case of
    2 depca's in a PC).

    ************************************************************************
    Support for MCA EtherWORKS cards added 11-3-98.
    Verified to work with up to 2 DE212 cards in a system (although not
      fully stress-tested).  

    Currently known bugs/limitations:

    Note:  with the MCA stuff as a module, it trusts the MCA configuration,
           not the command line for IRQ and memory address.  You can
           specify them if you want, but it will throw your values out.
           You still have to pass the IO address it was configured as
           though.

    ************************************************************************
    TO DO:
    ------


    Revision History
    ----------------

    Version   Date        Description
  
      0.1     25-jan-94   Initial writing.
      0.2     27-jan-94   Added LANCE TX hardware buffer chaining.
      0.3      1-feb-94   Added multiple DEPCA support.
      0.31     4-feb-94   Added DE202 recognition.
      0.32    19-feb-94   Tidy up. Improve multi-DEPCA support.
      0.33    25-feb-94   Fix DEPCA ethernet ROM counter enable.
                          Add jabber packet fix from murf@perftech.com
			  and becker@super.org
      0.34     7-mar-94   Fix DEPCA max network memory RAM & NICSR access.
      0.35     8-mar-94   Added DE201 recognition. Tidied up.
      0.351   30-apr-94   Added EISA support. Added DE422 recognition.
      0.36    16-may-94   DE422 fix released.
      0.37    22-jul-94   Added MODULE support
      0.38    15-aug-94   Added DBR ROM switch in depca_close(). 
                          Multi DEPCA bug fix.
      0.38axp 15-sep-94   Special version for Alpha AXP Linux V1.0.
      0.381   12-dec-94   Added DE101 recognition, fix multicast bug.
      0.382    9-feb-95   Fix recognition bug reported by <bkm@star.rl.ac.uk>.
      0.383   22-feb-95   Fix for conflict with VESA SCSI reported by
                          <stromain@alf.dec.com>
      0.384   17-mar-95   Fix a ring full bug reported by <bkm@star.rl.ac.uk>
      0.385    3-apr-95   Fix a recognition bug reported by 
                                                <ryan.niemi@lastfrontier.com>
      0.386   21-apr-95   Fix the last fix...sorry, must be galloping senility
      0.40    25-May-95   Rewrite for portability & updated.
                          ALPHA support from <jestabro@amt.tay1.dec.com>
      0.41    26-Jun-95   Added verify_area() calls in depca_ioctl() from
                          suggestion by <heiko@colossus.escape.de>
      0.42    27-Dec-95   Add 'mem' shared memory assignment for loadable 
                          modules.
                          Add 'adapter_name' for loadable modules when no PROM.
			  Both above from a suggestion by 
			  <pchen@woodruffs121.residence.gatech.edu>.
			  Add new multicasting code.
      0.421   22-Apr-96	  Fix alloc_device() bug <jari@markkus2.fimr.fi>
      0.422   29-Apr-96	  Fix depca_hw_init() bug <jari@markkus2.fimr.fi>
      0.423    7-Jun-96   Fix module load bug <kmg@barco.be>
      0.43    16-Aug-96   Update alloc_device() to conform to de4x5.c
      0.44     1-Sep-97   Fix *_probe() to test check_region() first - bug
                           reported by <mmogilvi@elbert.uccs.edu>
      0.45     3-Nov-98   Added support for MCA EtherWORKS (DE210/DE212) cards
                           by <tymm@computer.org> 
      0.451    5-Nov-98   Fixed mca stuff cuz I'm a dummy. <tymm@computer.org>
      0.5     14-Nov-98   Re-spin for 2.1.x kernels.
      0.51    27-Jun-99   Correct received packet length for CRC from
                           report by <worm@dkik.dk>

    =========================================================================
*/

#include "etherboot.h"
#include "nic.h"
#include "cards.h"

#define DEPCA_NICSR ioaddr+0x00   
#define DEPCA_RBI   ioaddr+0x02   
#define DEPCA_DATA  ioaddr+0x04   
#define DEPCA_ADDR  ioaddr+0x06   
#define DEPCA_HBASE ioaddr+0x08   
#define DEPCA_PROM  ioaddr+0x0c   
#define DEPCA_CNFG  ioaddr+0x0c   
#define DEPCA_RBSA  ioaddr+0x0e   

#define CSR0       0
#define CSR1       1
#define CSR2       2
#define CSR3       3

 
#define TO       	0x0100	
#define SHE      	0x0080  
#define BS       	0x0040  
#define BUF      	0x0020	
#define RBE      	0x0010	
#define AAC      	0x0008  
#define _128KB      	0x0008  
#define IM       	0x0004	
#define IEN      	0x0002	
#define LED      	0x0001	


#define ERR     	0x8000 	
#define BABL    	0x4000 	
#define CERR    	0x2000 	
#define MISS    	0x1000 	
#define MERR    	0x0800 	
#define RINT    	0x0400 	
#define TINT    	0x0200 	
#define IDON    	0x0100 	
#define INTR    	0x0080 	
#define INEA    	0x0040 	
#define RXON    	0x0020 	
#define TXON    	0x0010 	
#define TDMD    	0x0008 	
#define STOP    	0x0004 	
#define STRT    	0x0002 	
#define INIT    	0x0001 	
#define INTM            0xff00  
#define INTE            0xfff0  


#define BSWP    	0x0004	
#define ACON    	0x0002	
#define BCON    	0x0001	


#define PROM       	0x8000 	
#define EMBA       	0x0080	
#define INTL       	0x0040 	
#define DRTY       	0x0020 	
#define COLL       	0x0010 	
#define DTCR       	0x0008 	
#define LOOP       	0x0004 	
#define DTX        	0x0002 	
#define DRX        	0x0001 	


#define R_OWN       0x80000000 	
#define R_ERR     	0x4000 	
#define R_FRAM    	0x2000 	
#define R_OFLO    	0x1000 	
#define R_CRC     	0x0800 	
#define R_BUFF    	0x0400 	
#define R_STP     	0x0200 	
#define R_ENP     	0x0100 	


#define T_OWN       0x80000000 	
#define T_ERR     	0x4000 	
#define T_ADD_FCS 	0x2000 	
#define T_MORE    	0x1000	
#define T_ONE     	0x0800 	
#define T_DEF     	0x0400 	
#define T_STP       0x02000000 	
#define T_ENP       0x01000000	
#define T_FLAGS     0xff000000  


#define TMD3_BUFF    0x8000	
#define TMD3_UFLO    0x4000	
#define TMD3_RES     0x2000	
#define TMD3_LCOL    0x1000	
#define TMD3_LCAR    0x0800	
#define TMD3_RTRY    0x0400	

#define PROBE_LENGTH    32

#define NUM_RX_DESC     2               
#define NUM_TX_DESC     2               
#define RX_BUFF_SZ	1536            
#define TX_BUFF_SZ	1536            

#define DEPCA_IO_PORTS	{0x300, 0x200, 0}

#ifndef	DEPCA_MODEL
#define	DEPCA_MODEL	DEPCA
#endif

static enum {
	DEPCA, DE100, DE101, DE200, DE201, DE202, DE210, DE212, DE422, unknown
} adapter = DEPCA_MODEL;


static char *adapter_name[] = {
	"DEPCA",
	"DE100","DE101",
	"DE200","DE201","DE202",
	"DE210","DE212",
	"DE422",
	""
};

#ifndef	DEPCA_RAM_BASE
#define DEPCA_RAM_BASE	0xd0000
#endif

#define ALIGN4      ((u32)4 - 1)       
#define ALIGN8      ((u32)8 - 1)       
#define ALIGN         ALIGN8              

typedef	long		s32;
typedef	unsigned long	u32;
typedef	short		s16;
typedef	unsigned short	u16;
typedef	char		s8;
typedef	unsigned char	u8;

struct depca_rx_desc {
    volatile s32 base;
    s16 buf_length;		
    s16 msg_length;		
};

struct depca_tx_desc {
    volatile s32 base;
    s16 length;		        
    s16 misc;                   
};

#define LA_MASK 0x0000ffff      

struct depca_init {
    u16 mode;	                
    u8  phys_addr[ETH_ALEN];	
    u8  mcast_table[8];	        
    u32 rx_ring;     	        
    u32 tx_ring;	        
};

struct depca_private {
	struct depca_rx_desc	*rx_ring;
	struct depca_tx_desc	*tx_ring;
	struct depca_init	init_block;	
	char			*rx_memcpy[NUM_RX_DESC];
	char			*tx_memcpy[NUM_TX_DESC];
	u32			bus_offset;	
	u32			sh_mem;		
	u32			dma_buffs;	
	int			rx_cur, tx_cur;	
	int			txRingMask, rxRingMask;
	s32			rx_rlen, tx_rlen;
	
};

static Address		mem_start = DEPCA_RAM_BASE;
static Address		mem_len, offset;
static unsigned short	ioaddr = 0;
static struct depca_private	lp;

#define STOP_DEPCA \
    outw(CSR0, DEPCA_ADDR);\
    outw(STOP, DEPCA_DATA)

static void depca_init_ring(struct nic *nic)
{
	int	i;
	u32	p;

	lp.rx_cur = lp.tx_cur = 0;
	
	for (i = 0; i <= lp.rxRingMask; i++) {
		writel((p = lp.dma_buffs + i * RX_BUFF_SZ) | R_OWN, &lp.rx_ring[i].base);
		writew(-RX_BUFF_SZ, &lp.rx_ring[i].buf_length);
		lp.rx_memcpy[i] = (char *) (p + lp.bus_offset);
	}
	for (i = 0; i <= lp.txRingMask; i++) {
		writel((p = lp.dma_buffs + (i + lp.txRingMask + 1) * TX_BUFF_SZ) & 0x00ffffff, &lp.tx_ring[i].base);
		lp.tx_memcpy[i] = (char *) (p + lp.bus_offset);
	}

	
	lp.init_block.rx_ring = ((u32) ((u32) lp.rx_ring) & LA_MASK) | lp.rx_rlen;
	lp.init_block.tx_ring = ((u32) ((u32) lp.tx_ring) & LA_MASK) | lp.tx_rlen;
	for (i = 0; i < ETH_ALEN; i++)
		lp.init_block.phys_addr[i] = nic->node_addr[i];
	lp.init_block.mode = 0x0000;	
	memset(lp.init_block.mcast_table, 0, sizeof(lp.init_block.mcast_table));
}

static void LoadCSRs(void)
{
	outw(CSR1, DEPCA_ADDR);	
	outw((u16) (lp.sh_mem & LA_MASK), DEPCA_DATA);
	outw(CSR2, DEPCA_ADDR);	
	outw((u16) ((lp.sh_mem & LA_MASK) >> 16), DEPCA_DATA);
	outw(CSR3, DEPCA_ADDR);	
	outw(ACON, DEPCA_DATA);
	outw(CSR0, DEPCA_ADDR);	
}

static int InitRestartDepca(void)
{
	int		i;

	
	memcpy_toio((char *)lp.sh_mem, &lp.init_block, sizeof(struct depca_init));
	outw(CSR0, DEPCA_ADDR);		
	outw(INIT, DEPCA_DATA);		

	for (i = 0; i < 100 && !(inw(DEPCA_DATA) & IDON); i++)
		;
	if (i < 100) {
		
		outw(IDON | STRT, DEPCA_DATA);
	} else {
		printf("DEPCA not initialised\n");
		return (1);
	}
	return (0);
}

static void depca_reset(struct nic *nic)
{
	s16	nicsr;
	int	i, j;

	STOP_DEPCA;
	nicsr = inb(DEPCA_NICSR);
	nicsr = ((nicsr & ~SHE & ~RBE & ~IEN) | IM);
	outb(nicsr, DEPCA_NICSR);
	if (inw(DEPCA_DATA) != STOP)
	{
		printf("depca: Cannot stop NIC\n");
		return;
	}

	
	lp.sh_mem = mem_start;
	mem_start += sizeof(struct depca_init);
	
	mem_start = (mem_start + ALIGN) & ~ALIGN;
	lp.rx_ring = (struct depca_rx_desc *) mem_start;
	mem_start += (sizeof(struct depca_rx_desc) * NUM_RX_DESC);
	lp.tx_ring = (struct depca_tx_desc *) mem_start;
	mem_start += (sizeof(struct depca_tx_desc) * NUM_TX_DESC);

	lp.bus_offset = mem_start & 0x00ff0000;
	
	lp.dma_buffs = mem_start & LA_MASK;

	
	lp.rxRingMask = NUM_RX_DESC - 1;
	lp.txRingMask = NUM_TX_DESC - 1;

	
	for (i = 0, j = lp.rxRingMask; j > 0; i++) {
		j >>= 1;
	}
	lp.rx_rlen = (s32) (i << 29);
	for (i = 0, j = lp.txRingMask; j > 0; i++) {
		j >>= 1;
	}
	lp.tx_rlen = (s32) (i << 29);

	
	depca_init_ring(nic);
	LoadCSRs();
	InitRestartDepca();
}

static int depca_poll(struct nic *nic)
{
	int		entry;
	u32		status;

	entry = lp.rx_cur;
	if ((status = readl(&lp.rx_ring[entry].base) & R_OWN))
		return (0);
	memcpy(nic->packet, lp.rx_memcpy[entry], nic->packetlen = lp.rx_ring[entry].msg_length);
	lp.rx_ring[entry].base |= R_OWN;
	lp.rx_cur = (++lp.rx_cur) & lp.rxRingMask;
	return (1);
}

static void depca_transmit(
	struct nic *nic,
	const char *d,			
	unsigned int t,			
	unsigned int s,			
	const char *p)			
{
	int		entry, len;
	char		*mem;

	
	mem = lp.tx_memcpy[entry = lp.tx_cur];
	memcpy_toio(mem, d, ETH_ALEN);
	memcpy_toio(mem + ETH_ALEN, nic->node_addr, ETH_ALEN);
	mem[ETH_ALEN * 2] = t >> 8;
	mem[ETH_ALEN * 2 + 1] = t;
	memcpy_toio(mem + ETH_HLEN, p, s);
	s += ETH_HLEN;
	len = (s < ETH_ZLEN ? ETH_ZLEN : s);
	
	writel(readl(&lp.tx_ring[entry].base) & ~T_FLAGS, &lp.tx_ring[entry].base);
	
	writew(0x0000, &lp.tx_ring[entry].misc);
	
	writew(-len, &lp.tx_ring[entry].length);
	
	writel(readl(&lp.tx_ring[entry].base) | (T_STP|T_ENP|T_OWN), &lp.tx_ring[entry].base);
	
	lp.tx_cur = (++lp.tx_cur) & lp.txRingMask;
}

static void depca_disable(struct nic *nic)
{
	STOP_DEPCA;
}

static int depca_probe1(struct nic *nic)
{
	u8	data, nicsr;
	u8	sig[] = { 0xFF, 0x00, 0x55, 0xAA, 0xFF, 0x00, 0x55, 0xAA };
	int	i, j;
	long	sum, chksum;

	data = inb(DEPCA_PROM);		
	data = inb(DEPCA_PROM);		
	if (data == 0x8) {
		nicsr = inb(DEPCA_NICSR);
		nicsr |= AAC;
		outb(nicsr, DEPCA_NICSR);
	}
	for (i = 0, j = 0; j < (int)sizeof(sig) && i < PROBE_LENGTH+((int)sizeof(sig))-1; ++i) {
		data = inb(DEPCA_PROM);
		if (data == sig[j])		
			++j;
		else
			j = (data == sig[0]) ? 1 : 0;
	}
	if (j != sizeof(sig))
		return (0);
	
	STOP_DEPCA;
	nicsr = ((inb(DEPCA_NICSR) & ~SHE & ~RBE & ~IEN) | IM);
	outb(nicsr, DEPCA_NICSR);
	if (inw(DEPCA_DATA) != STOP)
		return (0);
	memcpy((char *)mem_start, sig, sizeof(sig));
	if (memcmp((char *)mem_start, sig, sizeof(sig)) != 0)
		return (0);
	for (i = 0, j = 0, sum = 0; j < 3; j++) {
		sum <<= 1;
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
		sum += (u8)(nic->node_addr[i++] = inb(DEPCA_PROM));
		sum += (u16)((nic->node_addr[i++] = inb(DEPCA_PROM)) << 8);
		if (sum > 0xFFFF)
			sum -= 0xFFFF;
	}
	if (sum == 0xFFFF)
		sum = 0;
	chksum = (u8)inb(DEPCA_PROM);
	chksum |= (u16)(inb(DEPCA_PROM) << 8);
	mem_len = (adapter == DEPCA) ? (48 << 10) : (64 << 10);
	offset = 0;
	if (nicsr & BUF) {
		offset = 0x8000;
		nicsr &= ~BS;
		mem_len -= (32 << 10);
	}
	if (adapter != DEPCA)	
		outb(nicsr |= SHE, DEPCA_NICSR);
	printf("%s base %#hX, memory [%#hX-%#hX], addr %!",
		adapter_name[adapter], ioaddr, mem_start, mem_start + mem_len,
		nic->node_addr);
	if (sum != chksum)
		printf(" (bad checksum)");
	putchar('\n');
	return (1);
}

struct nic *depca_probe(struct nic *nic, unsigned short *probe_addrs)
{
	static unsigned short	base[] = DEPCA_IO_PORTS;
	int			i;

	if (probe_addrs == 0 || probe_addrs[0] == 0)
		probe_addrs = base;	
	for (i = 0; (ioaddr = base[i]) != 0; ++i) {
		if (depca_probe1(nic))
			break;
	}
	if (ioaddr == 0)
		return (0);
	depca_reset(nic);
	
	nic->reset = depca_reset;
	nic->poll = depca_poll;
	nic->transmit = depca_transmit;
	nic->disable = depca_disable;
	return (nic);
}
