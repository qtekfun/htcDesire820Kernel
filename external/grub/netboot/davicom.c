/*  
    DAVICOM DM9009/DM9102/DM9102A Etherboot Driver	V1.00

    This driver was ported from Marty Conner's Tulip Etherboot driver. 
    Thanks Marty Connor (mdc@thinguin.org) 
    You can get Tulip driver source file from this URL:

    "http://etherboot.sourceforge..net/#Distribution"
    
    This davicom etherboot driver supports DM9009/DM9102/DM9102A/
    DM9102A+DM9801/DM9102A+DM9802 NICs.

    This software may be used and distributed according to the terms
    of the GNU Public License, incorporated herein by reference.

*/





#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"

#undef DAVICOM_DEBUG
#undef DAVICOM_DEBUG_WHERE

#define TX_TIME_OUT       2*TICKS_PER_SEC

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned int   u32;
typedef   signed int   s32;

enum davicom_offsets {
   CSR0=0,     CSR1=0x08,  CSR2=0x10,  CSR3=0x18,  CSR4=0x20,  CSR5=0x28,
   CSR6=0x30,  CSR7=0x38,  CSR8=0x40,  CSR9=0x48, CSR10=0x50, CSR11=0x58,
  CSR12=0x60, CSR13=0x68, CSR14=0x70, CSR15=0x78, CSR16=0x80, CSR20=0xA0
};

#define EEPROM_ADDRLEN 6
#define EEPROM_SIZE    32              

static unsigned char ee_data[EEPROM_SIZE];

#define EE_WRITE_CMD    (5 << addr_len)
#define EE_READ_CMD     (6 << addr_len)
#define EE_ERASE_CMD    (7 << addr_len)

#define EE_SHIFT_CLK    0x02    
#define EE_CS           0x01    
#define EE_DATA_WRITE   0x04    
#define EE_WRITE_0      0x01
#define EE_WRITE_1      0x05
#define EE_DATA_READ    0x08    
#define EE_ENB          (0x4800 | EE_CS)

#define PHY_DATA_0	0x0
#define PHY_DATA_1	0x20000
#define MDCLKH		0x10000

#define eeprom_delay()  inl(ee_addr)

#define le16_to_cpu(val) (val)

struct txdesc {
  volatile unsigned long   status;         
  unsigned long   buf1sz:11,      
    buf2sz:11,                    
    control:10;                   
  const unsigned char *buf1addr;  
  const unsigned char *buf2addr;  
};

struct rxdesc {
  volatile unsigned long   status;         
  unsigned long   buf1sz:11,      
    buf2sz:11,                    
    control:10;                   
  unsigned char   *buf1addr;      
  unsigned char   *buf2addr;      
};

#define BUFLEN 1536


static unsigned short vendor, dev_id;
static unsigned long ioaddr;


#define NTXD 2
static struct txdesc txd[NTXD] __attribute__ ((aligned(4)));
#ifdef	USE_LOWMEM_BUFFER
#define txb ((char *)0x10000 - BUFLEN)
#else
static unsigned char txb[BUFLEN] __attribute__ ((aligned(4)));
#endif

#define NRXD 4
static struct rxdesc rxd[NRXD] __attribute__ ((aligned(4)));
#ifdef	USE_LOWMEM_BUFFER
#define rxb ((char *)0x10000 - NRXD * BUFLEN - BUFLEN)
#else
static unsigned char rxb[NRXD * BUFLEN] __attribute__ ((aligned(4)));
#endif
static int rxd_tail;
static int TxPtr;


static void whereami(const char *str);
static int read_eeprom(unsigned long ioaddr, int location, int addr_len);
struct nic *davicom_probe(struct nic *nic, unsigned short *io_addrs,
			struct pci_device *pci);
static void davicom_init_chain(struct nic *nic);	
static void davicom_reset(struct nic *nic);
static void davicom_transmit(struct nic *nic, const char *d, unsigned int t,
			   unsigned int s, const char *p);
static int davicom_poll(struct nic *nic);
static void davicom_disable(struct nic *nic);
static void whereami (const char *str);
#ifdef	DAVICOM_DEBUG
static void davicom_more(void);
#endif 
static void davicom_wait(unsigned int nticks);
static int phy_read(int);
static void phy_write(int, u16);
static void phy_write_1bit(u32, u32);
static int phy_read_1bit(u32);
static void davicom_media_chk(struct nic *);



static inline void whereami (const char *str)
{
#ifdef	DAVICOM_DEBUG_WHERE
  printf("%s\n", str);
  
#endif
}

#ifdef	DAVICOM_DEBUG
static void davicom_more()
{
  printf("\n\n-- more --");
  while (!iskey())
    ;
  getchar();
  printf("\n\n");
}
#endif 

static void davicom_wait(unsigned int nticks)
{
  unsigned int to = currticks() + nticks;
  while (currticks() < to)
     ;
}


static int phy_read(int location)
{
 int i, phy_addr=1;
 u16 phy_data;
 u32 io_dcr9;

 whereami("phy_read\n");

 io_dcr9 = ioaddr + CSR9;

 
 for (i=0; i<34; i++)
     phy_write_1bit(io_dcr9, PHY_DATA_1);

 
 phy_write_1bit(io_dcr9, PHY_DATA_0);
 phy_write_1bit(io_dcr9, PHY_DATA_1);

 
 phy_write_1bit(io_dcr9, PHY_DATA_1);
 phy_write_1bit(io_dcr9, PHY_DATA_0);

 
 for (i=0x10; i>0; i=i>>1)
     phy_write_1bit(io_dcr9, phy_addr&i ? PHY_DATA_1: PHY_DATA_0);
   
 
 for (i=0x10; i>0; i=i>>1)
     phy_write_1bit(io_dcr9, location&i ? PHY_DATA_1: PHY_DATA_0);

 
 phy_read_1bit(io_dcr9);

 
 for (phy_data=0, i=0; i<16; i++) {
   phy_data<<=1;
   phy_data|=phy_read_1bit(io_dcr9);
 }

 return phy_data;
}

static void phy_write(int location, u16 phy_data)
{
 u16 i, phy_addr=1;
 u32 io_dcr9; 

 whereami("phy_write\n");

 io_dcr9 = ioaddr + CSR9;

 
 for (i=0; i<34; i++)
   phy_write_1bit(io_dcr9, PHY_DATA_1);

 
 phy_write_1bit(io_dcr9, PHY_DATA_0);
 phy_write_1bit(io_dcr9, PHY_DATA_1);

 
 phy_write_1bit(io_dcr9, PHY_DATA_0);
 phy_write_1bit(io_dcr9, PHY_DATA_1);

 
 for (i=0x10; i>0; i=i>>1)
   phy_write_1bit(io_dcr9, phy_addr&i ? PHY_DATA_1: PHY_DATA_0);

 
 for (i=0x10; i>0; i=i>>1)
   phy_write_1bit(io_dcr9, location&i ? PHY_DATA_1: PHY_DATA_0);

 /* written trasnition */
 phy_write_1bit(io_dcr9, PHY_DATA_1);
 phy_write_1bit(io_dcr9, PHY_DATA_0);

 
 for (i=0x8000; i>0; i>>=1)
   phy_write_1bit(io_dcr9, phy_data&i ? PHY_DATA_1: PHY_DATA_0);
}

static void phy_write_1bit(u32 ee_addr, u32 phy_data)
{
 whereami("phy_write_1bit\n");
 outl(phy_data, ee_addr);                        
 eeprom_delay();
 outl(phy_data|MDCLKH, ee_addr);                 
 eeprom_delay();
 outl(phy_data, ee_addr);                        
 eeprom_delay();
}

static int phy_read_1bit(u32 ee_addr)
{
 int phy_data;

 whereami("phy_read_1bit\n");

 outl(0x50000, ee_addr);
 eeprom_delay();

 phy_data=(inl(ee_addr)>>19) & 0x1;

 outl(0x40000, ee_addr);
 eeprom_delay();

 return phy_data;
}

static void HPNA_process(void)
{

 if ( (phy_read(3) & 0xfff0) == 0xb900 ) {
   if ( phy_read(31) == 0x4404 ) {
     
     if (phy_read(3) == 0xb901)
       phy_write(16, 0x5);	
     else
       phy_write(16, 0x1005); 
     phy_write(25, ((phy_read(24) + 3) & 0xff) | 0xf000);
   } else {
     
     phy_write(16, 0x5);
     phy_write(25, (phy_read(25) & 0xff00) + 2);
   }
 }
}

static void davicom_media_chk(struct nic * nic)
{
  unsigned long to, csr6;

  csr6 = 0x00200000;	
  outl(csr6, ioaddr + CSR6);

  if (vendor == PCI_VENDOR_ID_DAVICOM && dev_id == PCI_DEVICE_ID_DM9009) {
    
    phy_write(0, 0);
  } else {
    
    to = currticks() + 2 * TICKS_PER_SEC;
    while ( ((phy_read(1) & 0x24)!=0x24) && (currticks() < to))
       ;

    if ( (phy_read(1) & 0x24) == 0x24 ) {
      if (phy_read(17) & 0xa000)  
        csr6 |= 0x00000200;	
    } else
      csr6 |= 0x00040000; 
  }

  
  outl(csr6, ioaddr + CSR6);

  
  if (csr6 & 0x40000)
    HPNA_process();
}


static int read_eeprom(unsigned long ioaddr, int location, int addr_len)
{
  int i;
  unsigned short retval = 0;
  long ee_addr = ioaddr + CSR9;
  int read_cmd = location | EE_READ_CMD;

  whereami("read_eeprom\n");

  outl(EE_ENB & ~EE_CS, ee_addr);
  outl(EE_ENB, ee_addr);

  
  for (i = 4 + addr_len; i >= 0; i--) {
    short dataval = (read_cmd & (1 << i)) ? EE_DATA_WRITE : 0;
    outl(EE_ENB | dataval, ee_addr);
    eeprom_delay();
    outl(EE_ENB | dataval | EE_SHIFT_CLK, ee_addr);
    eeprom_delay();
  }
  outl(EE_ENB, ee_addr);

  for (i = 16; i > 0; i--) {
    outl(EE_ENB | EE_SHIFT_CLK, ee_addr);
    eeprom_delay();
    retval = (retval << 1) | ((inl(ee_addr) & EE_DATA_READ) ? 1 : 0);
    outl(EE_ENB, ee_addr);
    eeprom_delay();
  }

  
  outl(EE_ENB & ~EE_CS, ee_addr);
  return retval;
}

static void davicom_init_chain(struct nic *nic)
{
  int i;

  
  for (i=0; i<NTXD; i++) {
    txd[i].buf1addr = &txb[0];		
    txd[i].buf2addr = (unsigned char *)&txd[i+1]; 
    txd[i].buf1sz   = 0;
    txd[i].buf2sz   = 0;
    txd[i].control  = 0x184;           
    txd[i].status   = 0x00000000;      
  }

  for (i=0; i<192; i++) txb[i] = 0xFF;
  txb[0] = nic->node_addr[0];
  txb[1] = nic->node_addr[1];
  txb[4] = nic->node_addr[2];
  txb[5] = nic->node_addr[3];
  txb[8] = nic->node_addr[4];
  txb[9] = nic->node_addr[5];

  
  for (i=0; i<NRXD; i++) {
    rxd[i].buf1addr = &rxb[i * BUFLEN];
    rxd[i].buf2addr = (unsigned char *)&rxd[i+1]; 
    rxd[i].buf1sz   = BUFLEN;
    rxd[i].buf2sz   = 0;        
    rxd[i].control  = 0x4;		
    rxd[i].status   = 0x80000000;   
  }

  
  txd[NTXD - 1].buf2addr = (unsigned char *)&txd[0];
  rxd[NRXD - 1].buf2addr = (unsigned char *)&rxd[0];
  TxPtr = 0;
  rxd_tail = 0;
}


static void davicom_reset(struct nic *nic)
{
  unsigned long to;
  u32 addr_low, addr_high;

  whereami("davicom_reset\n");

  
  outl(inl(ioaddr + CSR6) & ~0x00002002, ioaddr + CSR6);

  
  outl(0x00000001, ioaddr + CSR0);

  davicom_wait(TICKS_PER_SEC);

  
  outl(0x0C00000, ioaddr + CSR0);	

  
  davicom_init_chain(nic);	

  
  outl((unsigned long)&rxd[0], ioaddr + CSR3);
  outl((unsigned long)&txd[0], ioaddr + CSR4);	

  davicom_media_chk(nic);

  
  txd[TxPtr].buf1sz = 192;
  txd[TxPtr].control = 0x024;		
  txd[TxPtr].status = 0x80000000;	

  
  outl(inl(ioaddr + CSR6) | 0x00002000, ioaddr + CSR6);
  
  outl(0, ioaddr + CSR1);

  to = currticks() + TX_TIME_OUT;
  while ((txd[TxPtr].status & 0x80000000) && (currticks() < to)) 
     ;

  if (currticks() >= to) {
    printf ("TX Setup Timeout!\n");
  }
  
 TxPtr = (++TxPtr >= NTXD) ? 0:TxPtr;	

#ifdef DAVICOM_DEBUG
  printf("txd.status = %X\n", txd.status);
  printf("ticks = %d\n", currticks() - (to - TX_TIME_OUT));
  davicom_more();
#endif

  
  outl(inl(ioaddr + CSR6) | 0x00000002, ioaddr + CSR6);
  
  outl(0, ioaddr + CSR2);
}


static void davicom_transmit(struct nic *nic, const char *d, unsigned int t,
                           unsigned int s, const char *p)
{
  unsigned long to;

  whereami("davicom_transmit\n");

  
  

  
  memcpy(&txb[0], d, ETH_ALEN);	
  memcpy(&txb[ETH_ALEN], nic->node_addr, ETH_ALEN); 
  txb[ETH_ALEN*2] = (t >> 8) & 0xFF; 
  txb[ETH_ALEN*2+1] = t & 0xFF;
  memcpy(&txb[ETH_HLEN], p, s); 

  
  txd[TxPtr].buf1sz   = ETH_HLEN+s;
  txd[TxPtr].control  = 0x00000184;      
  txd[TxPtr].status   = 0x80000000;      

  
  outl(0, ioaddr + CSR1);

  to = currticks() + TX_TIME_OUT;
  while ((txd[TxPtr].status & 0x80000000) && (currticks() < to))
     ;

  if (currticks() >= to) {
    printf ("TX Timeout!\n");
  }
 
  
  TxPtr = (++TxPtr >= NTXD) ? 0:TxPtr;	

}

static int davicom_poll(struct nic *nic)
{
  whereami("davicom_poll\n");

  if (rxd[rxd_tail].status & 0x80000000)
    return 0;

  whereami("davicom_poll got one\n");

  nic->packetlen = (rxd[rxd_tail].status & 0x3FFF0000) >> 16;

  if( rxd[rxd_tail].status & 0x00008000){
      rxd[rxd_tail].status = 0x80000000;
      rxd_tail++;
      if (rxd_tail == NRXD) rxd_tail = 0;
      return 0;
  }

  

  memcpy(nic->packet, rxb + rxd_tail * BUFLEN, nic->packetlen);

  
  rxd[rxd_tail].status = 0x80000000;
  rxd_tail++;
  if (rxd_tail == NRXD) rxd_tail = 0;

  return 1;
}

static void davicom_disable(struct nic *nic)
{
  whereami("davicom_disable\n");

  
  outl(0x00000000, ioaddr + CSR7);

  
  outl(inl(ioaddr + CSR6) & ~0x00002002, ioaddr + CSR6);

  
  (volatile unsigned long)inl(ioaddr + CSR8);
}

struct nic *davicom_probe(struct nic *nic, unsigned short *io_addrs,
                          struct pci_device *pci)
{
  unsigned int i;
  u32 l1, l2;

  whereami("davicom_probe\n");

  if (io_addrs == 0 || *io_addrs == 0)
    return 0;

  vendor  = pci->vendor;
  dev_id  = pci->dev_id;
  ioaddr  = *io_addrs;

  
  pcibios_write_config_dword(pci->bus, pci->devfn, 0x40, 0x00000000);

  
  outl(inl(ioaddr + CSR6) & ~0x00002002, ioaddr + CSR6);

  
  (volatile unsigned long)inl(ioaddr + CSR8);

  
  
  for (i = 0; i < sizeof(ee_data)/2; i++)
    ((unsigned short *)ee_data)[i] =
        le16_to_cpu(read_eeprom(ioaddr, i, EEPROM_ADDRLEN));

  
  for (i=0; i<ETH_ALEN; i++)
    nic->node_addr[i] = ee_data[20+i];

  printf("Davicom %! at ioaddr %#hX\n", nic->node_addr, ioaddr);

  
  davicom_reset(nic);

  nic->reset    = davicom_reset;
  nic->poll     = davicom_poll;
  nic->transmit = davicom_transmit;
  nic->disable  = davicom_disable;

  return nic;
}
