
/*-
 * Copyright (C) 1994 by PJD Weichmann & SWS Bern, Switzerland
 *
 * This software may be used and distributed according to the terms
 * of the GNU Public License, incorporated herein by reference.
 *
 * Module         : sk_g16.c
 *
 * Version        : $Revision: 1.4 $
 *
 * Author         : Patrick J.D. Weichmann
 *
 * Date Created   : 94/05/26
 * Last Updated   : $Date: 2002/01/02 21:56:40 $
 *
 * Description    : Schneider & Koch G16 Ethernet Device Driver for
 *                  Linux Kernel >= 1.1.22
 * Update History :
 *
-*/


#include "etherboot.h"
#include "nic.h"

#define ETH_ZLEN	60		

#include "sk_g16.h"


#define SK_NAME   "SK_G16"



#define SK_ADDR         0xcc000


#define POS_ADDR       (rom_addr>>14)  




#define SK_IO_PORTS     { 0x100, 0x180, 0x208, 0x220, 0x288, 0x320, 0x328, 0x390, 0 }



#define SK_POS_SIZE 8           

#define SK_POS0     ioaddr      
#define SK_POS1     ioaddr+1    
#define SK_POS2     ioaddr+2    
#define SK_POS3     ioaddr+3    
#define SK_POS4     ioaddr+4    




#define SK_MAC0         0x00
#define SK_MAC1         0x00
#define SK_MAC2         0x5a



#define SK_IDLOW  0xfd
#define SK_IDHIGH 0x6a



#define SK_ROM_RAM_ON  (POS2_CARD)
#define SK_ROM_RAM_OFF (POS2_EPROM)
#define SK_ROM_ON      (inb(SK_POS2) & POS2_CARD)
#define SK_ROM_OFF     (inb(SK_POS2) | POS2_EPROM)
#define SK_RAM_ON      (inb(SK_POS2) | POS2_CARD)
#define SK_RAM_OFF     (inb(SK_POS2) & POS2_EPROM)

#define POS2_CARD  0x0001              
#define POS2_EPROM 0x0002              


#define SK_IOREG        (board->ioreg) 
#define SK_PORT         (board->port)  
#define SK_IOCOM        (board->iocom) 


#define SK_IORUN        0x20

#define SK_IRQ          0x10

#define SK_RESET        0x08   
#define SK_RW           0x02   
#define SK_ADR          0x01   


#define SK_RREG         SK_RW  
#define SK_WREG         0      
#define SK_RAP          SK_ADR 
#define SK_RDATA        0      


/*
 * Any bitcombination sets the internal I/O bit (transfer will start)
 * when written to I/O Command
 */

#define SK_DOIO         0x80   



#define CSR0            0x00
#define CSR1            0x01
#define CSR2            0x02
#define CSR3            0x03



#define LC_LOG_TX_BUFFERS 1               
#define LC_LOG_RX_BUFFERS 2               


#define TMDNUM (1 << (LC_LOG_TX_BUFFERS)) 
#define RMDNUM (1 << (LC_LOG_RX_BUFFERS)) 


#define TMDNUMMASK (LC_LOG_TX_BUFFERS << 29)
#define RMDNUMMASK (LC_LOG_RX_BUFFERS << 29)


#define PKT_BUF_SZ              1518


#define ETHERCARD_TOTAL_SIZE    SK_POS_SIZE


#ifndef	HAVE_PORTRESERVE

#define check_region(ioaddr1, size)              0
#define request_region(ioaddr1, size,name)       do ; while (0)

#endif


#undef  SK_DEBUG	
#undef  SK_DEBUG2	

#ifdef	SK_DEBUG
#define PRINTF(x) printf x
#else
#define PRINTF(x) 
#endif

#ifdef	SK_DEBUG2
#define PRINTF2(x) printf x
#else
#define PRINTF2(x) 
#endif

/*
 * SK_G16 RAM
 *
 * The components are memory mapped and can be set in a region from
 * 0x00000 through 0xfc000 in 16KB steps.
 *
 * The Network components are: dual ported RAM, Prom, I/O Reg, Status-,
 * Controlregister and I/O Command.
 *
 * dual ported RAM: This is the only memory region which the LANCE chip
 *      has access to. From the Lance it is addressed from 0x0000 to
 *      0x3fbf. The host accesses it normally.
 *
 * PROM: The PROM obtains the ETHERNET-MAC-Address. It is realised as a
 *       8-Bit PROM, this means only the 16 even addresses are used of the
 *       32 Byte Address region. Access to a odd address results in invalid
 *       data.
 *
 * LANCE I/O Reg: The I/O Reg is build of 4 single Registers, Low-Byte Write,
 *       Hi-Byte Write, Low-Byte Read, Hi-Byte Read.
 *       Transfer from or to the LANCE is always in 16Bit so Low and High
 *       registers are always relevant.
 *
 *       The Data from the Readregister is not the data in the Writeregister!!
 *
 * Port: Status- and Controlregister.
 *       Two different registers which share the same address, Status is
 *       read-only, Control is write-only.
 *
 * I/O Command:
 *       Any bitcombination written in here starts the transmission between
 *       Host and LANCE.
 */

typedef struct
{
	unsigned char  ram[0x3fc0];   
	unsigned char  rom[0x0020];   
	unsigned char  res1[0x0010];  
	unsigned volatile short ioreg;
	unsigned volatile char  port; 
	unsigned char  iocom;         
} SK_RAM;



struct SK_ram
{
    struct init_block ib;
    struct tmd tmde[TMDNUM];
    struct rmd rmde[RMDNUM];
    char tmdbuf[TMDNUM][PKT_BUF_SZ];
    char rmdbuf[RMDNUM][PKT_BUF_SZ];
};


struct priv
{
    struct SK_ram *ram;  
    struct rmd *rmdhead; 
    struct tmd *tmdhead; 
    int        rmdnum;   
    int        tmdnum;   
    int        tmdlast;  
    void       *rmdbufs[RMDNUM]; 
    void       *tmdbufs[TMDNUM]; 
};



static SK_RAM *board;  
static unsigned short	ioaddr; 
static struct priv	p_data;





static int   SK_probe1(struct nic *nic, short ioaddr1);

static void SK_reset(struct nic *nic);
static int SK_poll(struct nic *nic);
static void SK_transmit(
struct nic *nic,
const char *d,			
unsigned int t,			
unsigned int s,			
const char *p);			
static void SK_disable(struct nic *nic);
struct nic *SK_probe(struct nic *nic, unsigned short *probe_addrs);


static int SK_lance_init(struct nic *nic, unsigned short mode);
static void SK_reset_board(void);
static void SK_set_RAP(int reg_number);
static int SK_read_reg(int reg_number);
static int SK_rread_reg(void);
static void SK_write_reg(int reg_number, int value);


static void SK_print_pos(struct nic *nic, char *text);
static void SK_print_ram(struct nic *nic);


static void SK_reset(struct nic *nic)
{
	
	SK_lance_init(nic, MODE_NORMAL);
}

static int SK_poll(struct nic *nic)
{
	
	struct priv *p;         
	struct rmd *rmdp;
	int csr0, rmdstat, packet_there;
    PRINTF2(("## %s: At beginning of SK_poll(). CSR0: %#hX\n",
           SK_NAME, SK_read_reg(CSR0)));

	p = nic->priv_data;
    csr0 = SK_read_reg(CSR0);      


    SK_write_reg(CSR0, csr0 & CSR0_CLRALL);

    if (csr0 & CSR0_ERR) 
    {
	printf("%s: error: %#hX", SK_NAME, csr0);

        if (csr0 & CSR0_MISS)      
        {
		printf(", Packet dropped.");
        }
	putchar('\n');
    }

    rmdp = p->rmdhead + p->rmdnum;
    packet_there = 0;

    while (!( (rmdstat = rmdp->u.s.status) & RX_OWN))
    {

	if ((rmdstat & (RX_STP | RX_ENP)) != (RX_STP | RX_ENP))
	{
	    

	    if (rmdstat & RX_STP)
	    {
		printf("%s: packet too long\n", SK_NAME);
	    }


	    rmdp->u.s.status = RX_OWN;      

	}
	else if (rmdstat & RX_ERR)          
	{
	    printf("%s: RX error: %#hX\n", SK_NAME, (int) rmdstat);
	    rmdp->u.s.status = RX_OWN;      
	}
	else 
	{

	    int len = (rmdp->mlen & 0x0fff);  


	    memcpy(nic->packet, (unsigned char *) (rmdp->u.buffer & 0x00ffffff), nic->packetlen = len);
	    packet_there = 1;



	    rmdp->u.s.status = RX_OWN;

	    p->rmdnum++;
	    p->rmdnum %= RMDNUM;

	    rmdp = p->rmdhead + p->rmdnum;
	}
    }
    SK_write_reg(CSR0, CSR0_INEA); 
	return (packet_there);
}

static void SK_transmit(
struct nic *nic,
const char *d,			
unsigned int t,			
unsigned int s,			
const char *pack)		
{
	
    struct priv *p;         
    struct tmd *tmdp;
    short len;
    int csr0, i, tmdstat;

    PRINTF2(("## %s: At beginning of SK_transmit(). CSR0: %#hX\n",
           SK_NAME, SK_read_reg(CSR0)));
	p = nic->priv_data;
	tmdp = p->tmdhead + p->tmdnum; 

	

	memcpy(&p->ram->tmdbuf[p->tmdnum][0], d, ETH_ALEN);	
	memcpy(&p->ram->tmdbuf[p->tmdnum][ETH_ALEN], nic->node_addr, ETH_ALEN); 
	p->ram->tmdbuf[p->tmdnum][ETH_ALEN + ETH_ALEN] = t >> 8;	
	p->ram->tmdbuf[p->tmdnum][ETH_ALEN + ETH_ALEN + 1] = t;	
	memcpy(&p->ram->tmdbuf[p->tmdnum][ETH_HLEN], pack, s);
	s += ETH_HLEN;
	while (s < ETH_ZLEN)	
		p->ram->tmdbuf[p->tmdnum][s++] = 0;
	p->ram->tmde[p->tmdnum].status2 = 0x0;

	
	len = ETH_ZLEN < s ? s : ETH_ZLEN;

	

	tmdp->blen = -len;            


	tmdp->u.s.status = TX_OWN | TX_STP | TX_ENP;

	
	SK_write_reg(CSR0, CSR0_TDMD | CSR0_INEA);

    csr0 = SK_read_reg(CSR0);      


    SK_write_reg(CSR0, csr0 & CSR0_CLRALL);

    if (csr0 & CSR0_ERR) 
    {
	printf("%s: error: %#hX", SK_NAME, csr0);

        if (csr0 & CSR0_MISS)      
        {
		printf(", Packet dropped.");
        }
	putchar('\n');
    }


    
    p->tmdlast++;
    p->tmdlast &= TMDNUM-1;

    tmdstat = tmdp->u.s.status & 0xff00; 

    if (tmdstat & TX_ERR) 
    {
	printf("%s: TX error: %#hX %#hX\n", SK_NAME, (int) tmdstat,
		(int) tmdp->status2);

	if (tmdp->status2 & TX_TDR)    
	{
	    printf("%s: tdr-problems \n", SK_NAME);
	}

        if (tmdp->status2 & TX_UFLO)   
        {

            SK_lance_init(nic, MODE_NORMAL);
        }

	tmdp->status2 = 0;             
    }

    SK_write_reg(CSR0, CSR0_INEA); 

	
	p->tmdnum++;
	p->tmdnum &= TMDNUM-1;

}

static void SK_disable(struct nic *nic)
{
    PRINTF(("## %s: At beginning of SK_disable(). CSR0: %#hX\n",
           SK_NAME, SK_read_reg(CSR0)));
    PRINTF(("%s: Shutting %s down CSR0 %#hX\n", SK_NAME, SK_NAME,
           (int) SK_read_reg(CSR0)));

    SK_write_reg(CSR0, CSR0_STOP); 
}

struct nic *SK_probe(struct nic *nic, unsigned short *probe_addrs)
{
	unsigned short		*p;
	static unsigned short	io_addrs[] = SK_IO_PORTS;
	
	putchar('\n');
	nic->priv_data = &p_data;
	if (probe_addrs == 0)
		probe_addrs = io_addrs;
	for (p = probe_addrs; (ioaddr = *p) != 0; ++p)
	{
		long		offset1, offset0 = inb(ioaddr);
		if ((offset0 == SK_IDLOW) &&
		 ((offset1 = inb(ioaddr + 1)) == SK_IDHIGH))
			if (SK_probe1(nic, ioaddr) >= 0)
				break;
	}
	
	if (ioaddr != 0)
	{
		
		nic->reset = SK_reset;
		nic->poll = SK_poll;
		nic->transmit = SK_transmit;
		nic->disable = SK_disable;
		return nic;
	}
	
	{
		return 0;
	}
}

int SK_probe1(struct nic *nic, short ioaddr1)
{
    int i,j;                
    int sk_addr_flag = 0;   
    unsigned int rom_addr;  

    struct priv *p;         

    if (SK_ADDR & 0x3fff || SK_ADDR < 0xa0000)
    {
            printf("%s: SK_ADDR %#hX is not valid. Check configuration.\n",
                    SK_NAME, SK_ADDR);
            return -1;
    }

    rom_addr = SK_ADDR;

    outb(SK_ROM_RAM_OFF, SK_POS2); 
    outb(POS_ADDR, SK_POS3);       
    outb(SK_ROM_RAM_ON, SK_POS2);  
#ifdef	SK_DEBUG
    SK_print_pos(nic, "POS registers after ROM, RAM config");
#endif

    board = (SK_RAM *) rom_addr;
	PRINTF(("adr[0]: %hX, adr[1]: %hX, adr[2]: %hX\n",
	board->rom[0], board->rom[2], board->rom[4]));

    
    for (i = 0, j = 0; i < ETH_ALEN; i++, j+=2)
    {
	*(nic->node_addr+i) = board->rom[j];
    }

    
#ifdef	SK_DEBUG
    if (!(*(nic->node_addr+0) == SK_MAC0 &&
	  *(nic->node_addr+1) == SK_MAC1 &&
	  *(nic->node_addr+2) == SK_MAC2) )
    {
        PRINTF(("## %s: We did not find SK_G16 at RAM location.\n",
                SK_NAME));
	return -1;                     
    }
#endif

    p = nic->priv_data;

    

    p->ram = (struct SK_ram *) rom_addr; 
    p->tmdhead = &(p->ram)->tmde[0];     
    p->rmdhead = &(p->ram)->rmde[0];     

    printf("Schneider & Koch G16 at %#hX, mem at %#hX, HW addr: %!\n",
	    (unsigned int) ioaddr, (unsigned int) p->ram, nic->node_addr);

    

    for (i = 0; i < TMDNUM; i++)
    {
	p->tmdbufs[i] = p->ram->tmdbuf[i];
    }

    for (i = 0; i < RMDNUM; i++)
    {
	p->rmdbufs[i] = p->ram->rmdbuf[i];
    }
    i = 0;

    if (!(i = SK_lance_init(nic, MODE_NORMAL)))  
    {

#ifdef	SK_DEBUG

        printf("## %s: After lance init. CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0));
        SK_write_reg(CSR0, CSR0_STOP);
        printf("## %s: LANCE stopped. CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0));
        SK_lance_init(nic, MODE_DTX | MODE_DRX);
        printf("## %s: Reinit with DTX + DRX off. CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0));
        SK_write_reg(CSR0, CSR0_STOP);
        printf("## %s: LANCE stopped. CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0));
        SK_lance_init(nic, MODE_NORMAL);
        printf("## %s: LANCE back to normal mode. CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0));
        SK_print_pos(nic, "POS regs before returning OK");

#endif	

    }
    else 
    {

	PRINTF(("## %s: LANCE init failed: CSR0: %#hX\n",
               SK_NAME, SK_read_reg(CSR0)));
	return -1;
    }

#ifdef	SK_DEBUG
    SK_print_pos(nic, "End of SK_probe1");
    SK_print_ram(nic);
#endif

    return 0;                            

} 

static int SK_lance_init(struct nic *nic, unsigned short mode)
{
    int i;
    struct priv *p = (struct priv *) nic->priv_data;
    struct tmd  *tmdp;
    struct rmd  *rmdp;

    PRINTF(("## %s: At beginning of LANCE init. CSR0: %#hX\n",
           SK_NAME, SK_read_reg(CSR0)));

    
    SK_reset_board();

    
    p->tmdnum = 0;                   
    p->tmdlast = 0;                  

    for (i = 0; i < TMDNUM; i++)     
    {
	tmdp = p->tmdhead + i;

	tmdp->u.buffer = (unsigned long) p->tmdbufs[i]; 

	
	tmdp->u.s.status = TX_STP | TX_ENP;
    }


    

    p->rmdnum = 0;                   

    for (i = 0; i < RMDNUM; i++)     
    {
	rmdp = p->rmdhead + i;


	rmdp->u.buffer = (unsigned long) p->rmdbufs[i]; 


	rmdp->u.s.status = RX_OWN;

	rmdp->blen = -PKT_BUF_SZ;    

	rmdp->mlen = 0;              

    }

    

    (p->ram)->ib.mode = mode;        

    for (i = 0; i < ETH_ALEN; i++)   
    {
	(p->ram)->ib.paddr[i] = *(nic->node_addr+i);
    }

    for (i = 0; i < 8; i++)          
    {
	(p->ram)->ib.laddr[i] = 0;   
    }

    

    (p->ram)->ib.rdrp = (int)  p->rmdhead | RMDNUMMASK;
    (p->ram)->ib.tdrp = (int)  p->tmdhead | TMDNUMMASK;

    

    SK_write_reg(CSR3, CSR3_ACON);   


    
    SK_write_reg(CSR1, 0);          
    SK_write_reg(CSR2, 0);          


    PRINTF(("## %s: After setting CSR1-3. CSR0: %#hX\n",
           SK_NAME, SK_read_reg(CSR0)));

    


    SK_write_reg(CSR0, CSR0_INIT);

    

    SK_set_RAP(CSR0);              

    for (i = 0; (i < 100) && !(SK_rread_reg() & CSR0_IDON); i++)
	; 

    if (i >= 100) 
    {
	printf("%s: can't init am7990, status: %#hX "
	       "init_block: %#hX\n",
		SK_NAME, (int) SK_read_reg(CSR0),
		(unsigned int) &(p->ram)->ib);

#ifdef	SK_DEBUG
	SK_print_pos(nic, "LANCE INIT failed");
#endif

	return -1;                 
    }

    PRINTF(("## %s: init done after %d ticks\n", SK_NAME, i));

    

    SK_write_reg(CSR0, CSR0_IDON | CSR0_INEA | CSR0_STRT);

    PRINTF(("## %s: LANCE started. CSR0: %#hX\n", SK_NAME,
            SK_read_reg(CSR0)));

    return 0;                      

} 


static void SK_reset_board(void)
{
    int i;

	PRINTF(("## %s: At beginning of SK_reset_board.\n", SK_NAME));
    SK_PORT = 0x00;           
    for (i = 0; i < 10 ; i++) 
	;
    SK_PORT = SK_RESET;       

} 

static void SK_set_RAP(int reg_number)
{
    SK_IOREG = reg_number;
    SK_PORT  = SK_RESET | SK_RAP | SK_WREG;
    SK_IOCOM = SK_DOIO;

    while (SK_PORT & SK_IORUN)
	;
} 

static int SK_read_reg(int reg_number)
{
    SK_set_RAP(reg_number);

    SK_PORT  = SK_RESET | SK_RDATA | SK_RREG;
    SK_IOCOM = SK_DOIO;

    while (SK_PORT & SK_IORUN)
	;
    return (SK_IOREG);

} 

static int SK_rread_reg(void)
{
    SK_PORT  = SK_RESET | SK_RDATA | SK_RREG;

    SK_IOCOM = SK_DOIO;

    while (SK_PORT & SK_IORUN)
	;
    return (SK_IOREG);

} 

static void SK_write_reg(int reg_number, int value)
{
    SK_set_RAP(reg_number);

    SK_IOREG = value;
    SK_PORT  = SK_RESET | SK_RDATA | SK_WREG;
    SK_IOCOM = SK_DOIO;

    while (SK_PORT & SK_IORUN)
	;
} 


#ifdef	SK_DEBUG
static void SK_print_pos(struct nic *nic, char *text)
{

    unsigned char pos0 = inb(SK_POS0),
		  pos1 = inb(SK_POS1),
		  pos2 = inb(SK_POS2),
		  pos3 = inb(SK_POS3),
		  pos4 = inb(SK_POS4);


    printf("## %s: %s.\n"
           "##   pos0=%#hX pos1=%#hX pos2=%#hX pos3=%#hX pos4=%#hX\n",
           SK_NAME, text, pos0, pos1, pos2, (pos3<<14), pos4);

} 

static void SK_print_ram(struct nic *nic)
{

    int i;
    struct priv *p = (struct priv *) nic->priv_data;

    printf("## %s: RAM Details.\n"
           "##   RAM at %#hX tmdhead: %#hX rmdhead: %#hX initblock: %#hX\n",
           SK_NAME,
           (unsigned int) p->ram,
           (unsigned int) p->tmdhead,
           (unsigned int) p->rmdhead,
           (unsigned int) &(p->ram)->ib);

    printf("##   ");

    for(i = 0; i < TMDNUM; i++)
    {
           if (!(i % 3)) 
           {
               printf("\n##   ");
           }
        printf("tmdbufs%d: %#hX ", (i+1), (int) p->tmdbufs[i]);
    }
    printf("##   ");

    for(i = 0; i < RMDNUM; i++)
    {
         if (!(i % 3)) 
           {
               printf("\n##   ");
           }
        printf("rmdbufs%d: %#hX ", (i+1), (int) p->rmdbufs[i]);
    }
    putchar('\n');

} 
#endif
