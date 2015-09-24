/*
 * QEMU NE2000 emulation
 *
 * Copyright (c) 2003-2004 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "hw.h"
#include "pci.h"
#include "pc.h"
#include "net.h"


#define MAX_ETH_FRAME_SIZE 1514

#define E8390_CMD	0x00  
#define EN0_CLDALO	0x01	
#define EN0_STARTPG	0x01	
#define EN0_CLDAHI	0x02	
#define EN0_STOPPG	0x02	
#define EN0_BOUNDARY	0x03	
#define EN0_TSR		0x04	
#define EN0_TPSR	0x04	
#define EN0_NCR		0x05	
#define EN0_TCNTLO	0x05	
#define EN0_FIFO	0x06	
#define EN0_TCNTHI	0x06	
#define EN0_ISR		0x07	
#define EN0_CRDALO	0x08	
#define EN0_RSARLO	0x08	
#define EN0_CRDAHI	0x09	
#define EN0_RSARHI	0x09	
#define EN0_RCNTLO	0x0a	
#define EN0_RTL8029ID0	0x0a	
#define EN0_RCNTHI	0x0b	
#define EN0_RTL8029ID1	0x0b	
#define EN0_RSR		0x0c	
#define EN0_RXCR	0x0c	
#define EN0_TXCR	0x0d	
#define EN0_COUNTER0	0x0d	
#define EN0_DCFG	0x0e	
#define EN0_COUNTER1	0x0e	
#define EN0_IMR		0x0f	
#define EN0_COUNTER2	0x0f	

#define EN1_PHYS        0x11
#define EN1_CURPAG      0x17
#define EN1_MULT        0x18

#define EN2_STARTPG	0x21	
#define EN2_STOPPG	0x22	

#define EN3_CONFIG0	0x33
#define EN3_CONFIG1	0x34
#define EN3_CONFIG2	0x35
#define EN3_CONFIG3	0x36

#define E8390_STOP	0x01	
#define E8390_START	0x02	
#define E8390_TRANS	0x04	
#define E8390_RREAD	0x08	
#define E8390_RWRITE	0x10	
#define E8390_NODMA	0x20	
#define E8390_PAGE0	0x00	
#define E8390_PAGE1	0x40	
#define E8390_PAGE2	0x80	

#define ENISR_RX	0x01	
#define ENISR_TX	0x02	
#define ENISR_RX_ERR	0x04	
#define ENISR_TX_ERR	0x08	
#define ENISR_OVER	0x10	
#define ENISR_COUNTERS	0x20	
#define ENISR_RDC	0x40	
#define ENISR_RESET	0x80	
#define ENISR_ALL	0x3f	

#define ENRSR_RXOK	0x01	
#define ENRSR_CRC	0x02	
#define ENRSR_FAE	0x04	
#define ENRSR_FO	0x08	
#define ENRSR_MPA	0x10	
#define ENRSR_PHY	0x20	
#define ENRSR_DIS	0x40	
#define ENRSR_DEF	0x80	

#define ENTSR_PTX 0x01	
#define ENTSR_ND  0x02	
#define ENTSR_COL 0x04	
#define ENTSR_ABT 0x08  
#define ENTSR_CRS 0x10	
#define ENTSR_FU  0x20  
#define ENTSR_CDH 0x40	
#define ENTSR_OWC 0x80  

#define NE2000_PMEM_SIZE    (32*1024)
#define NE2000_PMEM_START   (16*1024)
#define NE2000_PMEM_END     (NE2000_PMEM_SIZE+NE2000_PMEM_START)
#define NE2000_MEM_SIZE     NE2000_PMEM_END

typedef struct NE2000State {
    uint8_t cmd;
    uint32_t start;
    uint32_t stop;
    uint8_t boundary;
    uint8_t tsr;
    uint8_t tpsr;
    uint16_t tcnt;
    uint16_t rcnt;
    uint32_t rsar;
    uint8_t rsr;
    uint8_t rxcr;
    uint8_t isr;
    uint8_t dcfg;
    uint8_t imr;
    uint8_t phys[6]; 
    uint8_t curpag;
    uint8_t mult[8]; 
    qemu_irq irq;
    int isa_io_base;
    PCIDevice *pci_dev;
    VLANClientState *vc;
    uint8_t macaddr[6];
    uint8_t mem[NE2000_MEM_SIZE];
} NE2000State;

static void ne2000_reset(NE2000State *s)
{
    int i;

    s->isr = ENISR_RESET;
    memcpy(s->mem, s->macaddr, 6);
    s->mem[14] = 0x57;
    s->mem[15] = 0x57;

    
    for(i = 15;i >= 0; i--) {
        s->mem[2 * i] = s->mem[i];
        s->mem[2 * i + 1] = s->mem[i];
    }
}

static void ne2000_update_irq(NE2000State *s)
{
    int isr;
    isr = (s->isr & s->imr) & 0x7f;
#if defined(DEBUG_NE2000)
    printf("NE2000: Set IRQ to %d (%02x %02x)\n",
	   isr ? 1 : 0, s->isr, s->imr);
#endif
    qemu_set_irq(s->irq, (isr != 0));
}

#define POLYNOMIAL 0x04c11db6

static int compute_mcast_idx(const uint8_t *ep)
{
    uint32_t crc;
    int carry, i, j;
    uint8_t b;

    crc = 0xffffffff;
    for (i = 0; i < 6; i++) {
        b = *ep++;
        for (j = 0; j < 8; j++) {
            carry = ((crc & 0x80000000L) ? 1 : 0) ^ (b & 0x01);
            crc <<= 1;
            b >>= 1;
            if (carry)
                crc = ((crc ^ POLYNOMIAL) | carry);
        }
    }
    return (crc >> 26);
}

static int ne2000_buffer_full(NE2000State *s)
{
    int avail, index, boundary;

    index = s->curpag << 8;
    boundary = s->boundary << 8;
    if (index < boundary)
        avail = boundary - index;
    else
        avail = (s->stop - s->start) - (index - boundary);
    if (avail < (MAX_ETH_FRAME_SIZE + 4))
        return 1;
    return 0;
}

static int ne2000_can_receive(VLANClientState *vc)
{
    NE2000State *s = vc->opaque;

    if (s->cmd & E8390_STOP)
        return 1;
    return !ne2000_buffer_full(s);
}

#define MIN_BUF_SIZE 60

static ssize_t ne2000_receive(VLANClientState *vc, const uint8_t *buf, size_t size_)
{
    NE2000State *s = vc->opaque;
    int size = size_;
    uint8_t *p;
    unsigned int total_len, next, avail, len, index, mcast_idx;
    uint8_t buf1[60];
    static const uint8_t broadcast_macaddr[6] =
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

#if defined(DEBUG_NE2000)
    printf("NE2000: received len=%d\n", size);
#endif

    if (s->cmd & E8390_STOP || ne2000_buffer_full(s))
        return -1;

    
    if (s->rxcr & 0x10) {
        
    } else {
        if (!memcmp(buf,  broadcast_macaddr, 6)) {
            
            if (!(s->rxcr & 0x04))
                return size;
        } else if (buf[0] & 0x01) {
            
            if (!(s->rxcr & 0x08))
                return size;
            mcast_idx = compute_mcast_idx(buf);
            if (!(s->mult[mcast_idx >> 3] & (1 << (mcast_idx & 7))))
                return size;
        } else if (s->mem[0] == buf[0] &&
                   s->mem[2] == buf[1] &&
                   s->mem[4] == buf[2] &&
                   s->mem[6] == buf[3] &&
                   s->mem[8] == buf[4] &&
                   s->mem[10] == buf[5]) {
            
        } else {
            return size;
        }
    }


    
    if (size < MIN_BUF_SIZE) {
        memcpy(buf1, buf, size);
        memset(buf1 + size, 0, MIN_BUF_SIZE - size);
        buf = buf1;
        size = MIN_BUF_SIZE;
    }

    index = s->curpag << 8;
    
    total_len = size + 4;
    
    next = index + ((total_len + 4 + 255) & ~0xff);
    if (next >= s->stop)
        next -= (s->stop - s->start);
    
    p = s->mem + index;
    s->rsr = ENRSR_RXOK; 
    
    if (buf[0] & 0x01)
        s->rsr |= ENRSR_PHY;
    p[0] = s->rsr;
    p[1] = next >> 8;
    p[2] = total_len;
    p[3] = total_len >> 8;
    index += 4;

    
    while (size > 0) {
        if (index <= s->stop)
            avail = s->stop - index;
        else
            avail = 0;
        len = size;
        if (len > avail)
            len = avail;
        memcpy(s->mem + index, buf, len);
        buf += len;
        index += len;
        if (index == s->stop)
            index = s->start;
        size -= len;
    }
    s->curpag = next >> 8;

    
    s->isr |= ENISR_RX;
    ne2000_update_irq(s);

    return size_;
}

static void ne2000_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = opaque;
    int offset, page, index;

    addr &= 0xf;
#ifdef DEBUG_NE2000
    printf("NE2000: write addr=0x%x val=0x%02x\n", addr, val);
#endif
    if (addr == E8390_CMD) {
        
        s->cmd = val;
        if (!(val & E8390_STOP)) { 
            s->isr &= ~ENISR_RESET;
            
            if ((val & (E8390_RREAD | E8390_RWRITE)) &&
                s->rcnt == 0) {
                s->isr |= ENISR_RDC;
                ne2000_update_irq(s);
            }
            if (val & E8390_TRANS) {
                index = (s->tpsr << 8);
                
                if (index >= NE2000_PMEM_END)
                    index -= NE2000_PMEM_SIZE;
                
                if (index + s->tcnt <= NE2000_PMEM_END) {
                    qemu_send_packet(s->vc, s->mem + index, s->tcnt);
                }
                
                s->tsr = ENTSR_PTX;
                s->isr |= ENISR_TX;
                s->cmd &= ~E8390_TRANS;
                ne2000_update_irq(s);
            }
        }
    } else {
        page = s->cmd >> 6;
        offset = addr | (page << 4);
        switch(offset) {
        case EN0_STARTPG:
            s->start = val << 8;
            break;
        case EN0_STOPPG:
            s->stop = val << 8;
            break;
        case EN0_BOUNDARY:
            s->boundary = val;
            break;
        case EN0_IMR:
            s->imr = val;
            ne2000_update_irq(s);
            break;
        case EN0_TPSR:
            s->tpsr = val;
            break;
        case EN0_TCNTLO:
            s->tcnt = (s->tcnt & 0xff00) | val;
            break;
        case EN0_TCNTHI:
            s->tcnt = (s->tcnt & 0x00ff) | (val << 8);
            break;
        case EN0_RSARLO:
            s->rsar = (s->rsar & 0xff00) | val;
            break;
        case EN0_RSARHI:
            s->rsar = (s->rsar & 0x00ff) | (val << 8);
            break;
        case EN0_RCNTLO:
            s->rcnt = (s->rcnt & 0xff00) | val;
            break;
        case EN0_RCNTHI:
            s->rcnt = (s->rcnt & 0x00ff) | (val << 8);
            break;
        case EN0_RXCR:
            s->rxcr = val;
            break;
        case EN0_DCFG:
            s->dcfg = val;
            break;
        case EN0_ISR:
            s->isr &= ~(val & 0x7f);
            ne2000_update_irq(s);
            break;
        case EN1_PHYS ... EN1_PHYS + 5:
            s->phys[offset - EN1_PHYS] = val;
            break;
        case EN1_CURPAG:
            s->curpag = val;
            break;
        case EN1_MULT ... EN1_MULT + 7:
            s->mult[offset - EN1_MULT] = val;
            break;
        }
    }
}

static uint32_t ne2000_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = opaque;
    int offset, page, ret;

    addr &= 0xf;
    if (addr == E8390_CMD) {
        ret = s->cmd;
    } else {
        page = s->cmd >> 6;
        offset = addr | (page << 4);
        switch(offset) {
        case EN0_TSR:
            ret = s->tsr;
            break;
        case EN0_BOUNDARY:
            ret = s->boundary;
            break;
        case EN0_ISR:
            ret = s->isr;
            break;
	case EN0_RSARLO:
	    ret = s->rsar & 0x00ff;
	    break;
	case EN0_RSARHI:
	    ret = s->rsar >> 8;
	    break;
        case EN1_PHYS ... EN1_PHYS + 5:
            ret = s->phys[offset - EN1_PHYS];
            break;
        case EN1_CURPAG:
            ret = s->curpag;
            break;
        case EN1_MULT ... EN1_MULT + 7:
            ret = s->mult[offset - EN1_MULT];
            break;
        case EN0_RSR:
            ret = s->rsr;
            break;
        case EN2_STARTPG:
            ret = s->start >> 8;
            break;
        case EN2_STOPPG:
            ret = s->stop >> 8;
            break;
	case EN0_RTL8029ID0:
	    ret = 0x50;
	    break;
	case EN0_RTL8029ID1:
	    ret = 0x43;
	    break;
	case EN3_CONFIG0:
	    ret = 0;		
	    break;
	case EN3_CONFIG2:
	    ret = 0x40;		
	    break;
	case EN3_CONFIG3:
	    ret = 0x40;		
	    break;
        default:
            ret = 0x00;
            break;
        }
    }
#ifdef DEBUG_NE2000
    printf("NE2000: read addr=0x%x val=%02x\n", addr, ret);
#endif
    return ret;
}

static inline void ne2000_mem_writeb(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        s->mem[addr] = val;
    }
}

static inline void ne2000_mem_writew(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    addr &= ~1; 
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        *(uint16_t *)(s->mem + addr) = cpu_to_le16(val);
    }
}

static inline void ne2000_mem_writel(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    addr &= ~1; 
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        cpu_to_le32wu((uint32_t *)(s->mem + addr), val);
    }
}

static inline uint32_t ne2000_mem_readb(NE2000State *s, uint32_t addr)
{
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return s->mem[addr];
    } else {
        return 0xff;
    }
}

static inline uint32_t ne2000_mem_readw(NE2000State *s, uint32_t addr)
{
    addr &= ~1; 
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return le16_to_cpu(*(uint16_t *)(s->mem + addr));
    } else {
        return 0xffff;
    }
}

static inline uint32_t ne2000_mem_readl(NE2000State *s, uint32_t addr)
{
    addr &= ~1; 
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return le32_to_cpupu((uint32_t *)(s->mem + addr));
    } else {
        return 0xffffffff;
    }
}

static inline void ne2000_dma_update(NE2000State *s, int len)
{
    s->rsar += len;
    
    
    if (s->rsar == s->stop)
        s->rsar = s->start;

    if (s->rcnt <= len) {
        s->rcnt = 0;
        
        s->isr |= ENISR_RDC;
        ne2000_update_irq(s);
    } else {
        s->rcnt -= len;
    }
}

static void ne2000_asic_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = opaque;

#ifdef DEBUG_NE2000
    printf("NE2000: asic write val=0x%04x\n", val);
#endif
    if (s->rcnt == 0)
        return;
    if (s->dcfg & 0x01) {
        
        ne2000_mem_writew(s, s->rsar, val);
        ne2000_dma_update(s, 2);
    } else {
        
        ne2000_mem_writeb(s, s->rsar, val);
        ne2000_dma_update(s, 1);
    }
}

static uint32_t ne2000_asic_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = opaque;
    int ret;

    if (s->dcfg & 0x01) {
        
        ret = ne2000_mem_readw(s, s->rsar);
        ne2000_dma_update(s, 2);
    } else {
        
        ret = ne2000_mem_readb(s, s->rsar);
        ne2000_dma_update(s, 1);
    }
#ifdef DEBUG_NE2000
    printf("NE2000: asic read val=0x%04x\n", ret);
#endif
    return ret;
}

static void ne2000_asic_ioport_writel(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = opaque;

#ifdef DEBUG_NE2000
    printf("NE2000: asic writel val=0x%04x\n", val);
#endif
    if (s->rcnt == 0)
        return;
    
    ne2000_mem_writel(s, s->rsar, val);
    ne2000_dma_update(s, 4);
}

static uint32_t ne2000_asic_ioport_readl(void *opaque, uint32_t addr)
{
    NE2000State *s = opaque;
    int ret;

    
    ret = ne2000_mem_readl(s, s->rsar);
    ne2000_dma_update(s, 4);
#ifdef DEBUG_NE2000
    printf("NE2000: asic readl val=0x%04x\n", ret);
#endif
    return ret;
}

static void ne2000_reset_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    
}

static uint32_t ne2000_reset_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = opaque;
    ne2000_reset(s);
    return 0;
}

static void ne2000_save(QEMUFile* f,void* opaque)
{
	NE2000State* s=(NE2000State*)opaque;
        uint32_t tmp;

        if (s->pci_dev)
            pci_device_save(s->pci_dev, f);

        qemu_put_8s(f, &s->rxcr);

	qemu_put_8s(f, &s->cmd);
	qemu_put_be32s(f, &s->start);
	qemu_put_be32s(f, &s->stop);
	qemu_put_8s(f, &s->boundary);
	qemu_put_8s(f, &s->tsr);
	qemu_put_8s(f, &s->tpsr);
	qemu_put_be16s(f, &s->tcnt);
	qemu_put_be16s(f, &s->rcnt);
	qemu_put_be32s(f, &s->rsar);
	qemu_put_8s(f, &s->rsr);
	qemu_put_8s(f, &s->isr);
	qemu_put_8s(f, &s->dcfg);
	qemu_put_8s(f, &s->imr);
	qemu_put_buffer(f, s->phys, 6);
	qemu_put_8s(f, &s->curpag);
	qemu_put_buffer(f, s->mult, 8);
        tmp = 0;
	qemu_put_be32s(f, &tmp); 
	qemu_put_buffer(f, s->mem, NE2000_MEM_SIZE);
}

static int ne2000_load(QEMUFile* f,void* opaque,int version_id)
{
	NE2000State* s=(NE2000State*)opaque;
        int ret;
        uint32_t tmp;

        if (version_id > 3)
            return -EINVAL;

        if (s->pci_dev && version_id >= 3) {
            ret = pci_device_load(s->pci_dev, f);
            if (ret < 0)
                return ret;
        }

        if (version_id >= 2) {
            qemu_get_8s(f, &s->rxcr);
        } else {
            s->rxcr = 0x0c;
        }

	qemu_get_8s(f, &s->cmd);
	qemu_get_be32s(f, &s->start);
	qemu_get_be32s(f, &s->stop);
	qemu_get_8s(f, &s->boundary);
	qemu_get_8s(f, &s->tsr);
	qemu_get_8s(f, &s->tpsr);
	qemu_get_be16s(f, &s->tcnt);
	qemu_get_be16s(f, &s->rcnt);
	qemu_get_be32s(f, &s->rsar);
	qemu_get_8s(f, &s->rsr);
	qemu_get_8s(f, &s->isr);
	qemu_get_8s(f, &s->dcfg);
	qemu_get_8s(f, &s->imr);
	qemu_get_buffer(f, s->phys, 6);
	qemu_get_8s(f, &s->curpag);
	qemu_get_buffer(f, s->mult, 8);
	qemu_get_be32s(f, &tmp); 
	qemu_get_buffer(f, s->mem, NE2000_MEM_SIZE);

	return 0;
}

static void isa_ne2000_cleanup(VLANClientState *vc)
{
    NE2000State *s = vc->opaque;

    unregister_savevm("ne2000", s);

    isa_unassign_ioport(s->isa_io_base, 16);
    isa_unassign_ioport(s->isa_io_base + 0x10, 2);
    isa_unassign_ioport(s->isa_io_base + 0x1f, 1);

    qemu_free(s);
}

void isa_ne2000_init(int base, qemu_irq irq, NICInfo *nd)
{
    NE2000State *s;

    qemu_check_nic_model(nd, "ne2k_isa");

    s = qemu_mallocz(sizeof(NE2000State));

    register_ioport_write(base, 16, 1, ne2000_ioport_write, s);
    register_ioport_read(base, 16, 1, ne2000_ioport_read, s);

    register_ioport_write(base + 0x10, 1, 1, ne2000_asic_ioport_write, s);
    register_ioport_read(base + 0x10, 1, 1, ne2000_asic_ioport_read, s);
    register_ioport_write(base + 0x10, 2, 2, ne2000_asic_ioport_write, s);
    register_ioport_read(base + 0x10, 2, 2, ne2000_asic_ioport_read, s);

    register_ioport_write(base + 0x1f, 1, 1, ne2000_reset_ioport_write, s);
    register_ioport_read(base + 0x1f, 1, 1, ne2000_reset_ioport_read, s);
    s->isa_io_base = base;
    s->irq = irq;
    memcpy(s->macaddr, nd->macaddr, 6);

    ne2000_reset(s);

    s->vc = qemu_new_vlan_client(nd->vlan, nd->model, nd->name,
                                 ne2000_can_receive, ne2000_receive, NULL,
                                 isa_ne2000_cleanup, s);

    qemu_format_nic_info_str(s->vc, s->macaddr);

    register_savevm("ne2000", -1, 2, ne2000_save, ne2000_load, s);
}


typedef struct PCINE2000State {
    PCIDevice dev;
    NE2000State ne2000;
} PCINE2000State;

static void ne2000_map(PCIDevice *pci_dev, int region_num,
                       uint32_t addr, uint32_t size, int type)
{
    PCINE2000State *d = (PCINE2000State *)pci_dev;
    NE2000State *s = &d->ne2000;

    register_ioport_write(addr, 16, 1, ne2000_ioport_write, s);
    register_ioport_read(addr, 16, 1, ne2000_ioport_read, s);

    register_ioport_write(addr + 0x10, 1, 1, ne2000_asic_ioport_write, s);
    register_ioport_read(addr + 0x10, 1, 1, ne2000_asic_ioport_read, s);
    register_ioport_write(addr + 0x10, 2, 2, ne2000_asic_ioport_write, s);
    register_ioport_read(addr + 0x10, 2, 2, ne2000_asic_ioport_read, s);
    register_ioport_write(addr + 0x10, 4, 4, ne2000_asic_ioport_writel, s);
    register_ioport_read(addr + 0x10, 4, 4, ne2000_asic_ioport_readl, s);

    register_ioport_write(addr + 0x1f, 1, 1, ne2000_reset_ioport_write, s);
    register_ioport_read(addr + 0x1f, 1, 1, ne2000_reset_ioport_read, s);
}

static void ne2000_cleanup(VLANClientState *vc)
{
    NE2000State *s = vc->opaque;

    unregister_savevm("ne2000", s);
}

static void pci_ne2000_init(PCIDevice *pci_dev)
{
    PCINE2000State *d = (PCINE2000State *)pci_dev;
    NE2000State *s;
    uint8_t *pci_conf;

    pci_conf = d->dev.config;
    pci_config_set_vendor_id(pci_conf, PCI_VENDOR_ID_REALTEK);
    pci_config_set_device_id(pci_conf, PCI_DEVICE_ID_REALTEK_8029);
    pci_config_set_class(pci_conf, PCI_CLASS_NETWORK_ETHERNET);
    pci_conf[PCI_HEADER_TYPE] = PCI_HEADER_TYPE_NORMAL; 
    pci_conf[0x3d] = 1; 

    pci_register_bar(&d->dev, 0, 0x100,
                           PCI_ADDRESS_SPACE_IO, ne2000_map);
    s = &d->ne2000;
    s->irq = d->dev.irq[0];
    s->pci_dev = (PCIDevice *)d;
    qdev_get_macaddr(&d->dev.qdev, s->macaddr);
    ne2000_reset(s);
    s->vc = qdev_get_vlan_client(&d->dev.qdev,
                                 ne2000_can_receive, ne2000_receive, NULL,
                                 ne2000_cleanup, s);

    qemu_format_nic_info_str(s->vc, s->macaddr);

    register_savevm("ne2000", -1, 3, ne2000_save, ne2000_load, s);
}

static void ne2000_register_devices(void)
{
    pci_qdev_register("ne2k_pci", sizeof(PCINE2000State), pci_ne2000_init);
}

device_init(ne2000_register_devices)
