/*
 * USB Mass Storage Device emulation
 *
 * Copyright (c) 2006 CodeSourcery.
 * Written by Paul Brook
 *
 * This code is licenced under the LGPL.
 */

#include "qemu-common.h"
#include "usb.h"
#include "block.h"
#include "scsi-disk.h"
#include "console.h"


#ifdef DEBUG_MSD
#define DPRINTF(fmt, ...) \
do { printf("usb-msd: " fmt , ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) do {} while(0)
#endif

#define MassStorageReset  0xff
#define GetMaxLun         0xfe

enum USBMSDMode {
    USB_MSDM_CBW, 
    USB_MSDM_DATAOUT, 
    USB_MSDM_DATAIN, 
    USB_MSDM_CSW 
};

typedef struct {
    USBDevice dev;
    enum USBMSDMode mode;
    uint32_t scsi_len;
    uint8_t *scsi_buf;
    uint32_t usb_len;
    uint8_t *usb_buf;
    uint32_t data_len;
    uint32_t residue;
    uint32_t tag;
    BlockDriverState *bs;
    SCSIDevice *scsi_dev;
    int result;
    
    USBPacket *packet;
} MSDState;

struct usb_msd_cbw {
    uint32_t sig;
    uint32_t tag;
    uint32_t data_len;
    uint8_t flags;
    uint8_t lun;
    uint8_t cmd_len;
    uint8_t cmd[16];
};

struct usb_msd_csw {
    uint32_t sig;
    uint32_t tag;
    uint32_t residue;
    uint8_t status;
};

static const uint8_t qemu_msd_dev_descriptor[] = {
	0x12,       
	0x01,       
	0x00, 0x01, 

	0x00,	    
	0x00,	    
	0x00,       
	0x08,       

        
	0x00, 0x00, 
 	0x00, 0x00, 
	0x00, 0x00, 

	0x01,       
	0x02,       
	0x03,       
	0x01        
};

static const uint8_t qemu_msd_config_descriptor[] = {

	
	0x09,       
	0x02,       
	0x20, 0x00, 
	0x01,       
	0x01,       
	0x00,       
	0xc0,       
	0x00,       

	
	0x09,       
	0x04,       
	0x00,       
	0x00,       
	0x02,       
	0x08,       
	0x06,       
	0x50,       
	0x00,       

	
	0x07,       
	0x05,       
	0x81,       
 	0x02,       
 	0x40, 0x00, 
	0x00,       

	
	0x07,       
	0x05,       
	0x02,       
 	0x02,       
 	0x40, 0x00, 
	0x00        
};

static void usb_msd_copy_data(MSDState *s)
{
    uint32_t len;
    len = s->usb_len;
    if (len > s->scsi_len)
        len = s->scsi_len;
    if (s->mode == USB_MSDM_DATAIN) {
        memcpy(s->usb_buf, s->scsi_buf, len);
    } else {
        memcpy(s->scsi_buf, s->usb_buf, len);
    }
    s->usb_len -= len;
    s->scsi_len -= len;
    s->usb_buf += len;
    s->scsi_buf += len;
    s->data_len -= len;
    if (s->scsi_len == 0) {
        if (s->mode == USB_MSDM_DATAIN) {
            s->scsi_dev->read_data(s->scsi_dev, s->tag);
        } else if (s->mode == USB_MSDM_DATAOUT) {
            s->scsi_dev->write_data(s->scsi_dev, s->tag);
        }
    }
}

static void usb_msd_send_status(MSDState *s)
{
    struct usb_msd_csw csw;

    csw.sig = cpu_to_le32(0x53425355);
    csw.tag = cpu_to_le32(s->tag);
    csw.residue = s->residue;
    csw.status = s->result;
    memcpy(s->usb_buf, &csw, 13);
}

static void usb_msd_command_complete(void *opaque, int reason, uint32_t tag,
                                     uint32_t arg)
{
    MSDState *s = (MSDState *)opaque;
    USBPacket *p = s->packet;

    if (tag != s->tag) {
        fprintf(stderr, "usb-msd: Unexpected SCSI Tag 0x%x\n", tag);
    }
    if (reason == SCSI_REASON_DONE) {
        DPRINTF("Command complete %d\n", arg);
        s->residue = s->data_len;
        s->result = arg != 0;
        if (s->packet) {
            if (s->data_len == 0 && s->mode == USB_MSDM_DATAOUT) {
                usb_msd_send_status(s);
                s->mode = USB_MSDM_CBW;
            } else {
                if (s->data_len) {
                    s->data_len -= s->usb_len;
                    if (s->mode == USB_MSDM_DATAIN)
                        memset(s->usb_buf, 0, s->usb_len);
                    s->usb_len = 0;
                }
                if (s->data_len == 0)
                    s->mode = USB_MSDM_CSW;
            }
            s->packet = NULL;
            usb_packet_complete(p);
        } else if (s->data_len == 0) {
            s->mode = USB_MSDM_CSW;
        }
        return;
    }
    s->scsi_len = arg;
    s->scsi_buf = s->scsi_dev->get_buf(s->scsi_dev, tag);
    if (p) {
        usb_msd_copy_data(s);
        if (s->usb_len == 0) {
            DPRINTF("Packet complete %p\n", p);
            s->packet = NULL;
            usb_packet_complete(p);
        }
    }
}

static void usb_msd_handle_reset(USBDevice *dev)
{
    MSDState *s = (MSDState *)dev;

    DPRINTF("Reset\n");
    s->mode = USB_MSDM_CBW;
}

static int usb_msd_handle_control(USBDevice *dev, int request, int value,
                                  int index, int length, uint8_t *data)
{
    MSDState *s = (MSDState *)dev;
    int ret = 0;

    switch (request) {
    case DeviceRequest | USB_REQ_GET_STATUS:
        data[0] = (1 << USB_DEVICE_SELF_POWERED) |
            (dev->remote_wakeup << USB_DEVICE_REMOTE_WAKEUP);
        data[1] = 0x00;
        ret = 2;
        break;
    case DeviceOutRequest | USB_REQ_CLEAR_FEATURE:
        if (value == USB_DEVICE_REMOTE_WAKEUP) {
            dev->remote_wakeup = 0;
        } else {
            goto fail;
        }
        ret = 0;
        break;
    case DeviceOutRequest | USB_REQ_SET_FEATURE:
        if (value == USB_DEVICE_REMOTE_WAKEUP) {
            dev->remote_wakeup = 1;
        } else {
            goto fail;
        }
        ret = 0;
        break;
    case DeviceOutRequest | USB_REQ_SET_ADDRESS:
        dev->addr = value;
        ret = 0;
        break;
    case DeviceRequest | USB_REQ_GET_DESCRIPTOR:
        switch(value >> 8) {
        case USB_DT_DEVICE:
            memcpy(data, qemu_msd_dev_descriptor,
                   sizeof(qemu_msd_dev_descriptor));
            ret = sizeof(qemu_msd_dev_descriptor);
            break;
        case USB_DT_CONFIG:
            memcpy(data, qemu_msd_config_descriptor,
                   sizeof(qemu_msd_config_descriptor));
            ret = sizeof(qemu_msd_config_descriptor);
            break;
        case USB_DT_STRING:
            switch(value & 0xff) {
            case 0:
                
                data[0] = 4;
                data[1] = 3;
                data[2] = 0x09;
                data[3] = 0x04;
                ret = 4;
                break;
            case 1:
                
                ret = set_usb_string(data, "QEMU " QEMU_VERSION);
                break;
            case 2:
                
                ret = set_usb_string(data, "QEMU USB HARDDRIVE");
                break;
            case 3:
                
                ret = set_usb_string(data, "1");
                break;
            default:
                goto fail;
            }
            break;
        default:
            goto fail;
        }
        break;
    case DeviceRequest | USB_REQ_GET_CONFIGURATION:
        data[0] = 1;
        ret = 1;
        break;
    case DeviceOutRequest | USB_REQ_SET_CONFIGURATION:
        ret = 0;
        break;
    case DeviceRequest | USB_REQ_GET_INTERFACE:
        data[0] = 0;
        ret = 1;
        break;
    case DeviceOutRequest | USB_REQ_SET_INTERFACE:
        ret = 0;
        break;
    case EndpointOutRequest | USB_REQ_CLEAR_FEATURE:
        if (value == 0 && index != 0x81) { 
            goto fail;
        }
        ret = 0;
        break;
        
    case MassStorageReset:
        
        s->mode = USB_MSDM_CBW;
        ret = 0;
        break;
    case GetMaxLun:
        data[0] = 0;
        ret = 1;
        break;
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static void usb_msd_cancel_io(USBPacket *p, void *opaque)
{
    MSDState *s = opaque;
    s->scsi_dev->cancel_io(s->scsi_dev, s->tag);
    s->packet = NULL;
    s->scsi_len = 0;
}

static int usb_msd_handle_data(USBDevice *dev, USBPacket *p)
{
    MSDState *s = (MSDState *)dev;
    int ret = 0;
    struct usb_msd_cbw cbw;
    uint8_t devep = p->devep;
    uint8_t *data = p->data;
    int len = p->len;

    switch (p->pid) {
    case USB_TOKEN_OUT:
        if (devep != 2)
            goto fail;

        switch (s->mode) {
        case USB_MSDM_CBW:
            if (len != 31) {
                fprintf(stderr, "usb-msd: Bad CBW size");
                goto fail;
            }
            memcpy(&cbw, data, 31);
            if (le32_to_cpu(cbw.sig) != 0x43425355) {
                fprintf(stderr, "usb-msd: Bad signature %08x\n",
                        le32_to_cpu(cbw.sig));
                goto fail;
            }
            DPRINTF("Command on LUN %d\n", cbw.lun);
            if (cbw.lun != 0) {
                fprintf(stderr, "usb-msd: Bad LUN %d\n", cbw.lun);
                goto fail;
            }
            s->tag = le32_to_cpu(cbw.tag);
            s->data_len = le32_to_cpu(cbw.data_len);
            if (s->data_len == 0) {
                s->mode = USB_MSDM_CSW;
            } else if (cbw.flags & 0x80) {
                s->mode = USB_MSDM_DATAIN;
            } else {
                s->mode = USB_MSDM_DATAOUT;
            }
            DPRINTF("Command tag 0x%x flags %08x len %d data %d\n",
                    s->tag, cbw.flags, cbw.cmd_len, s->data_len);
            s->residue = 0;
            s->scsi_dev->send_command(s->scsi_dev, s->tag, cbw.cmd, 0);
            if (s->residue == 0) {
                if (s->mode == USB_MSDM_DATAIN) {
                    s->scsi_dev->read_data(s->scsi_dev, s->tag);
                } else if (s->mode == USB_MSDM_DATAOUT) {
                    s->scsi_dev->write_data(s->scsi_dev, s->tag);
                }
            }
            ret = len;
            break;

        case USB_MSDM_DATAOUT:
            DPRINTF("Data out %d/%d\n", len, s->data_len);
            if (len > s->data_len)
                goto fail;

            s->usb_buf = data;
            s->usb_len = len;
            if (s->scsi_len) {
                usb_msd_copy_data(s);
            }
            if (s->residue && s->usb_len) {
                s->data_len -= s->usb_len;
                if (s->data_len == 0)
                    s->mode = USB_MSDM_CSW;
                s->usb_len = 0;
            }
            if (s->usb_len) {
                DPRINTF("Deferring packet %p\n", p);
                usb_defer_packet(p, usb_msd_cancel_io, s);
                s->packet = p;
                ret = USB_RET_ASYNC;
            } else {
                ret = len;
            }
            break;

        default:
            DPRINTF("Unexpected write (len %d)\n", len);
            goto fail;
        }
        break;

    case USB_TOKEN_IN:
        if (devep != 1)
            goto fail;

        switch (s->mode) {
        case USB_MSDM_DATAOUT:
            if (s->data_len != 0 || len < 13)
                goto fail;
            
            usb_defer_packet(p, usb_msd_cancel_io, s);
            s->packet = p;
            ret = USB_RET_ASYNC;
            break;

        case USB_MSDM_CSW:
            DPRINTF("Command status %d tag 0x%x, len %d\n",
                    s->result, s->tag, len);
            if (len < 13)
                goto fail;

            s->usb_len = len;
            s->usb_buf = data;
            usb_msd_send_status(s);
            s->mode = USB_MSDM_CBW;
            ret = 13;
            break;

        case USB_MSDM_DATAIN:
            DPRINTF("Data in %d/%d\n", len, s->data_len);
            if (len > s->data_len)
                len = s->data_len;
            s->usb_buf = data;
            s->usb_len = len;
            if (s->scsi_len) {
                usb_msd_copy_data(s);
            }
            if (s->residue && s->usb_len) {
                s->data_len -= s->usb_len;
                memset(s->usb_buf, 0, s->usb_len);
                if (s->data_len == 0)
                    s->mode = USB_MSDM_CSW;
                s->usb_len = 0;
            }
            if (s->usb_len) {
                DPRINTF("Deferring packet %p\n", p);
                usb_defer_packet(p, usb_msd_cancel_io, s);
                s->packet = p;
                ret = USB_RET_ASYNC;
            } else {
                ret = len;
            }
            break;

        default:
            DPRINTF("Unexpected read (len %d)\n", len);
            goto fail;
        }
        break;

    default:
        DPRINTF("Bad token\n");
    fail:
        ret = USB_RET_STALL;
        break;
    }

    return ret;
}

static void usb_msd_handle_destroy(USBDevice *dev)
{
    MSDState *s = (MSDState *)dev;

    s->scsi_dev->destroy(s->scsi_dev);
    bdrv_delete(s->bs);
    qemu_free(s);
}

USBDevice *usb_msd_init(const char *filename)
{
    MSDState *s;
    BlockDriverState *bdrv;
    BlockDriver *drv = NULL;
    const char *p1;
    char fmt[32];

    p1 = strchr(filename, ':');
    if (p1++) {
        const char *p2;

        if (strstart(filename, "format=", &p2)) {
            int len = MIN(p1 - p2, sizeof(fmt));
            pstrcpy(fmt, len, p2);

            drv = bdrv_find_format(fmt);
            if (!drv) {
                printf("invalid format %s\n", fmt);
                return NULL;
            }
        } else if (*filename != ':') {
            printf("unrecognized USB mass-storage option %s\n", filename);
            return NULL;
        }

        filename = p1;
    }

    if (!*filename) {
        printf("block device specification needed\n");
        return NULL;
    }

    s = qemu_mallocz(sizeof(MSDState));

    bdrv = bdrv_new("usb");
    if (bdrv_open(bdrv, filename, 0, drv) < 0)
        goto fail;
    s->bs = bdrv;

    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_generic_handle_packet;

    s->dev.handle_reset = usb_msd_handle_reset;
    s->dev.handle_control = usb_msd_handle_control;
    s->dev.handle_data = usb_msd_handle_data;
    s->dev.handle_destroy = usb_msd_handle_destroy;

    snprintf(s->dev.devname, sizeof(s->dev.devname), "QEMU USB MSD(%.16s)",
             filename);

    s->scsi_dev = scsi_disk_init(bdrv, 0, usb_msd_command_complete, s);
    usb_msd_handle_reset((USBDevice *)s);
    return (USBDevice *)s;
 fail:
    qemu_free(s);
    return NULL;
}

BlockDriverState *usb_msd_get_bdrv(USBDevice *dev)
{
    MSDState *s = (MSDState *)dev;

    return s->bs;
}
