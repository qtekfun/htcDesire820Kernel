
#ifndef _HW_PCMCIA_H
#define _HW_PCMCIA_H

#include "qemu-common.h"

typedef struct {
    qemu_irq irq;
    int attached;
    const char *slot_string;
    const char *card_string;
} PCMCIASocket;

void pcmcia_socket_register(PCMCIASocket *socket);
void pcmcia_socket_unregister(PCMCIASocket *socket);
void pcmcia_info(Monitor *mon);

struct PCMCIACardState {
    void *state;
    PCMCIASocket *slot;
    int (*attach)(void *state);
    int (*detach)(void *state);
    const uint8_t *cis;
    int cis_len;

    
    uint8_t (*attr_read)(void *state, uint32_t address);
    void (*attr_write)(void *state, uint32_t address, uint8_t value);
    uint16_t (*common_read)(void *state, uint32_t address);
    void (*common_write)(void *state, uint32_t address, uint16_t value);
    uint16_t (*io_read)(void *state, uint32_t address);
    void (*io_write)(void *state, uint32_t address, uint16_t value);
};

#define CISTPL_DEVICE		0x01	
#define CISTPL_NO_LINK		0x14	
#define CISTPL_VERS_1		0x15	
#define CISTPL_JEDEC_C		0x18	
#define CISTPL_JEDEC_A		0x19	
#define CISTPL_CONFIG		0x1a	
#define CISTPL_CFTABLE_ENTRY	0x1b	
#define CISTPL_DEVICE_OC	0x1c	
#define CISTPL_DEVICE_OA	0x1d	
#define CISTPL_DEVICE_GEO	0x1e	
#define CISTPL_DEVICE_GEO_A	0x1f	
#define CISTPL_MANFID		0x20	
#define CISTPL_FUNCID		0x21	
#define CISTPL_FUNCE		0x22	
#define CISTPL_END		0xff	
#define CISTPL_ENDMARK		0xff

PCMCIACardState *dscm1xxxx_init(BlockDriverState *bdrv);

#endif 
