
#include "etherboot.h"
#include "nic.h"
#include "pci.h"
#include "cards.h"
#include "otulip.h"

static unsigned short vendor, dev_id;
static unsigned short ioaddr;
static unsigned int *membase;
static unsigned char srom[1024];

#define BUFLEN 1536     
                        

static struct txdesc txd;

#define NRXD 4
static struct rxdesc rxd[NRXD];
static int rxd_tail = 0;
#ifdef	USE_LOWMEM_BUFFER
#define rxb ((char *)0x10000 - NRXD * BUFLEN)
#define txb ((char *)0x10000 - NRXD * BUFLEN - BUFLEN)
#else
static unsigned char rxb[NRXD * BUFLEN];
static unsigned char txb[BUFLEN];
#endif

static unsigned char ehdr[ETH_HLEN];    

enum tulip_offsets {
        CSR0=0,    CSR1=0x08, CSR2=0x10, CSR3=0x18, CSR4=0x20, CSR5=0x28,
        CSR6=0x30, CSR7=0x38, CSR8=0x40, CSR9=0x48, CSR10=0x50, CSR11=0x58,
        CSR12=0x60, CSR13=0x68, CSR14=0x70, CSR15=0x78 };




#define UADJUST 870
static void udelay(unsigned long usec) {
  unsigned long i;
  for (i=((usec*UADJUST)/33)+1; i>0; i--) (void) TULIP_CSR_READ(csr_0);
}


static void delay_300ns(void)
{
    int idx;
    for (idx = (300 / 33) + 1; idx > 0; idx--)
        (void) TULIP_CSR_READ(csr_busmode);
}

#define EMIT do { TULIP_CSR_WRITE(csr_srom_mii, csr); delay_300ns(); } while (0)

static void srom_idle(void)
{
    unsigned bit, csr;

    csr  = SROMSEL ; EMIT;
    csr  = SROMSEL | SROMRD; EMIT;
    csr ^= SROMCS; EMIT;
    csr ^= SROMCLKON; EMIT;
    for (bit = 3 + SROM_BITWIDTH + 16; bit > 0; bit--) {
        csr ^= SROMCLKOFF; EMIT;    
        csr ^= SROMCLKON; EMIT;     
    }
    csr ^= SROMCLKOFF; EMIT;
    csr ^= SROMCS; EMIT;
    csr  = 0; EMIT;
}

static void srom_read(void)
{
    unsigned idx;
    const unsigned bitwidth = SROM_BITWIDTH;
    const unsigned cmdmask = (SROMCMD_RD << bitwidth);
    const unsigned msb = 1 << (bitwidth + 3 - 1);
    unsigned lastidx = (1 << bitwidth) - 1;

    srom_idle();

    for (idx = 0; idx <= lastidx; idx++) {
        unsigned lastbit, data, bits, bit, csr;
        csr  = SROMSEL ;                EMIT;
        csr  = SROMSEL | SROMRD;        EMIT;
        csr ^= SROMCSON;                EMIT;
        csr ^=            SROMCLKON;    EMIT;

        lastbit = 0;
        for (bits = idx|cmdmask, bit = bitwidth + 3; bit > 0; bit--, bits <<= 1)
 {
            const unsigned thisbit = bits & msb;
            csr ^= SROMCLKOFF; EMIT;    
            if (thisbit != lastbit) {
                csr ^= SROMDOUT; EMIT;  
            } else {
                EMIT;
            }
            csr ^= SROMCLKON; EMIT;     
            lastbit = thisbit;
        }
        csr ^= SROMCLKOFF; EMIT;

        for (data = 0, bits = 0; bits < 16; bits++) {
            data <<= 1;
            csr ^= SROMCLKON; EMIT;     
            data |= TULIP_CSR_READ(csr_srom_mii) & SROMDIN ? 1 : 0;
            csr ^= SROMCLKOFF; EMIT;    
        }
        srom[idx*2] = data & 0xFF;
        srom[idx*2+1] = data >> 8;
        csr  = SROMSEL | SROMRD; EMIT;
        csr  = 0; EMIT;
    }
    srom_idle();
}

static void tulip_reset(struct nic *nic)
{
        int x,cnt=2;

        outl(0x00000001, ioaddr + CSR0);
        udelay(1000);
        
        outl(0x01A08000, ioaddr + CSR0);


        while(cnt--) {
        
        if (cnt == 1)
		outl(0x32404000, ioaddr + CSR6);
        else
		outl(0x32000040, ioaddr + CSR6);


        

        
        outl(0x0, ioaddr + CSR13);              
        outl(0x7f3f, ioaddr + CSR14);
        outl(0x8000008, ioaddr + CSR15);
        outl(0x0, ioaddr + CSR13);
        outl(0x1, ioaddr + CSR13);
        outl(0x2404000, ioaddr + CSR6);

        
        outl(0x8af0008, ioaddr + CSR15);
        outl(0x50008, ioaddr + CSR15);

        

	if (vendor == PCI_VENDOR_ID_MACRONIX && dev_id == PCI_DEVICE_ID_MX987x5) {
		
		outl(0x01a80000, ioaddr + CSR6);
		outl(0xFFFFFFFF, ioaddr + CSR14);
		outl(0x00001000, ioaddr + CSR12);
	}

        outl(0x0, ioaddr + CSR7);       


        memset(&txd, 0, sizeof(struct txdesc));
        txd.buf1addr = &txb[0];
        txd.buf2addr = &txb[0];         
        txd.buf1sz   = 192;             
        txd.buf2sz   = 0;
        txd.control  = 0x020;           
        txd.status   = 0x80000000;      

        
        
        

        for(x=0;x<192;x++) txb[x] = 0xff;
        txb[0] = nic->node_addr[0];
        txb[1] = nic->node_addr[1];
        txb[4] = nic->node_addr[2];
        txb[5] = nic->node_addr[3];
        txb[8] = nic->node_addr[4];
        txb[9] = nic->node_addr[5];
        outl((unsigned long)&txd, ioaddr + CSR4);        
        outl(0x2406000, ioaddr + CSR6);         

        udelay(50000);  

        }

        
        {
          int x;
          for(x=0;x<NRXD;x++) {
            memset(&rxd[x], 0, sizeof(struct rxdesc));
            rxd[x].buf1addr = &rxb[x * BUFLEN];
            rxd[x].buf2addr = 0;        
            rxd[x].buf1sz   = BUFLEN;
            rxd[x].buf2sz   = 0;        
            rxd[x].control  = 0x0;
            rxd[x].status   = 0x80000000;       
          }
          rxd[NRXD - 1].control  = 0x008;       
          rxd_tail = 0;
        }

        
        outl((unsigned long)&rxd[0], ioaddr + CSR3);
        
        outl(0x2406002, ioaddr + CSR6);

}

static const char padmap[] = {
        0, 3, 2, 1};

static void tulip_transmit(struct nic *nic, const char *d, unsigned int t, unsigned int s, const char *p)
{
        unsigned long time;

        

	memcpy(ehdr, d, ETH_ALEN);
	memcpy(&ehdr[ETH_ALEN], nic->node_addr, ETH_ALEN);
        ehdr[ETH_ALEN*2] = (t >> 8) & 0xff;
        ehdr[ETH_ALEN*2+1] = t & 0xff;

        

        memset(&txd, 0, sizeof(struct txdesc));

        txd.buf1addr = &ehdr[0];        
        txd.buf1sz   = ETH_HLEN;

        txd.buf2addr = p;               
        txd.buf2sz   = s;

        txd.control  = 0x188;           

        txd.status   = 0x80000000;      

        outl(inl(ioaddr + CSR6) & ~0x00004000, ioaddr + CSR6);
        outl((unsigned long)&txd, ioaddr + CSR4);
        outl(inl(ioaddr + CSR6) | 0x00004000, ioaddr + CSR6);


}

static int tulip_poll(struct nic *nic)
{
        if (rxd[rxd_tail].status & 0x80000000) return 0;

        nic->packetlen = (rxd[rxd_tail].status & 0x3FFF0000) >> 16;

        

        memcpy(nic->packet, rxb + rxd_tail * BUFLEN, nic->packetlen);

        
        rxd[rxd_tail].status = 0x80000000;
        rxd_tail++;
        if (rxd_tail == NRXD) rxd_tail = 0;

        return 1;
}

static void tulip_disable(struct nic *nic)
{
	
}

struct nic *otulip_probe(struct nic *nic, unsigned short *io_addrs, struct pci_device *pci)
{
        int i;

	if (io_addrs == 0 || *io_addrs == 0)
		return (0);
	vendor = pci->vendor;
	dev_id = pci->dev_id;
	ioaddr = *io_addrs;
	membase = (unsigned int *)pci->membase;

        
        pcibios_write_config_dword(pci->bus,pci->devfn,0x40,0x00000000);

        
        
        
        

        srom_read();

	for (i=0; i < ETH_ALEN; i++)
		nic->node_addr[i] = srom[20+i];

        printf("Tulip %! at ioaddr %#hX\n", nic->node_addr, ioaddr);

        tulip_reset(nic);

	nic->reset = tulip_reset;
	nic->poll = tulip_poll;
	nic->transmit = tulip_transmit;
	nic->disable = tulip_disable;
        return nic;
}
