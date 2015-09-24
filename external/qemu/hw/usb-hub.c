/*
 * QEMU USB HUB emulation
 *
 * Copyright (c) 2005 Fabrice Bellard
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
#include "qemu-common.h"
#include "usb.h"


#define MAX_PORTS 8

typedef struct USBHubPort {
    USBPort port;
    uint16_t wPortStatus;
    uint16_t wPortChange;
} USBHubPort;

typedef struct USBHubState {
    USBDevice dev;
    int nb_ports;
    USBHubPort ports[MAX_PORTS];
} USBHubState;

#define ClearHubFeature		(0x2000 | USB_REQ_CLEAR_FEATURE)
#define ClearPortFeature	(0x2300 | USB_REQ_CLEAR_FEATURE)
#define GetHubDescriptor	(0xa000 | USB_REQ_GET_DESCRIPTOR)
#define GetHubStatus		(0xa000 | USB_REQ_GET_STATUS)
#define GetPortStatus		(0xa300 | USB_REQ_GET_STATUS)
#define SetHubFeature		(0x2000 | USB_REQ_SET_FEATURE)
#define SetPortFeature		(0x2300 | USB_REQ_SET_FEATURE)

#define PORT_STAT_CONNECTION	0x0001
#define PORT_STAT_ENABLE	0x0002
#define PORT_STAT_SUSPEND	0x0004
#define PORT_STAT_OVERCURRENT	0x0008
#define PORT_STAT_RESET		0x0010
#define PORT_STAT_POWER		0x0100
#define PORT_STAT_LOW_SPEED	0x0200
#define PORT_STAT_HIGH_SPEED    0x0400
#define PORT_STAT_TEST          0x0800
#define PORT_STAT_INDICATOR     0x1000

#define PORT_STAT_C_CONNECTION	0x0001
#define PORT_STAT_C_ENABLE	0x0002
#define PORT_STAT_C_SUSPEND	0x0004
#define PORT_STAT_C_OVERCURRENT	0x0008
#define PORT_STAT_C_RESET	0x0010

#define PORT_CONNECTION	        0
#define PORT_ENABLE		1
#define PORT_SUSPEND		2
#define PORT_OVERCURRENT	3
#define PORT_RESET		4
#define PORT_POWER		8
#define PORT_LOWSPEED		9
#define PORT_HIGHSPEED		10
#define PORT_C_CONNECTION	16
#define PORT_C_ENABLE		17
#define PORT_C_SUSPEND		18
#define PORT_C_OVERCURRENT	19
#define PORT_C_RESET		20
#define PORT_TEST               21
#define PORT_INDICATOR          22


static const uint8_t qemu_hub_dev_descriptor[] = {
	0x12,       
	0x01,       
	0x10, 0x01, 

	0x09,	    
	0x00,	    
	0x00,       
	0x08,       

	0x00, 0x00, 
 	0x00, 0x00, 
	0x01, 0x01, 

	0x03,       
	0x02,       
	0x01,       
	0x01        
};

static const uint8_t qemu_hub_config_descriptor[] = {

	
	0x09,       
	0x02,       
	0x19, 0x00, 
	0x01,       
	0x01,       
	0x00,       
	0xc0,       
	0x00,       


	
	0x09,       
	0x04,       
	0x00,       
	0x00,       
	0x01,       
	0x09,       
	0x00,       
	0x00,       
	0x00,       

	
	0x07,       
	0x05,       
	0x81,       
 	0x03,       
 	0x02, 0x00, 
	0xff        
};

static const uint8_t qemu_hub_hub_descriptor[] =
{
	0x00,			
	0x29,			
	0x00,			
	0x0a,			
	0x00,			
	0x01,			
	0x00			

        
};

static void usb_hub_attach(USBPort *port1, USBDevice *dev)
{
    USBHubState *s = port1->opaque;
    USBHubPort *port = &s->ports[port1->index];

    if (dev) {
        if (port->port.dev)
            usb_attach(port1, NULL);

        port->wPortStatus |= PORT_STAT_CONNECTION;
        port->wPortChange |= PORT_STAT_C_CONNECTION;
        if (dev->speed == USB_SPEED_LOW)
            port->wPortStatus |= PORT_STAT_LOW_SPEED;
        else
            port->wPortStatus &= ~PORT_STAT_LOW_SPEED;
        port->port.dev = dev;
        
        usb_send_msg(dev, USB_MSG_ATTACH);
    } else {
        dev = port->port.dev;
        if (dev) {
            port->wPortStatus &= ~PORT_STAT_CONNECTION;
            port->wPortChange |= PORT_STAT_C_CONNECTION;
            if (port->wPortStatus & PORT_STAT_ENABLE) {
                port->wPortStatus &= ~PORT_STAT_ENABLE;
                port->wPortChange |= PORT_STAT_C_ENABLE;
            }
            
            usb_send_msg(dev, USB_MSG_DETACH);
            port->port.dev = NULL;
        }
    }
}

static void usb_hub_handle_reset(USBDevice *dev)
{
    
}

static int usb_hub_handle_control(USBDevice *dev, int request, int value,
                                  int index, int length, uint8_t *data)
{
    USBHubState *s = (USBHubState *)dev;
    int ret;

    switch(request) {
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
    case EndpointOutRequest | USB_REQ_CLEAR_FEATURE:
        if (value == 0 && index != 0x81) { 
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
            memcpy(data, qemu_hub_dev_descriptor,
                   sizeof(qemu_hub_dev_descriptor));
            ret = sizeof(qemu_hub_dev_descriptor);
            break;
        case USB_DT_CONFIG:
            memcpy(data, qemu_hub_config_descriptor,
                   sizeof(qemu_hub_config_descriptor));

            data[22] = (s->nb_ports + 1 + 7) / 8;

            ret = sizeof(qemu_hub_config_descriptor);
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
                
                ret = set_usb_string(data, "314159");
                break;
            case 2:
                
                ret = set_usb_string(data, "QEMU USB Hub");
                break;
            case 3:
                
                ret = set_usb_string(data, "QEMU " QEMU_VERSION);
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
        
    case GetHubStatus:
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        ret = 4;
        break;
    case GetPortStatus:
        {
            unsigned int n = index - 1;
            USBHubPort *port;
            if (n >= s->nb_ports)
                goto fail;
            port = &s->ports[n];
            data[0] = port->wPortStatus;
            data[1] = port->wPortStatus >> 8;
            data[2] = port->wPortChange;
            data[3] = port->wPortChange >> 8;
            ret = 4;
        }
        break;
    case SetHubFeature:
    case ClearHubFeature:
        if (value == 0 || value == 1) {
        } else {
            goto fail;
        }
        ret = 0;
        break;
    case SetPortFeature:
        {
            unsigned int n = index - 1;
            USBHubPort *port;
            USBDevice *dev;
            if (n >= s->nb_ports)
                goto fail;
            port = &s->ports[n];
            dev = port->port.dev;
            switch(value) {
            case PORT_SUSPEND:
                port->wPortStatus |= PORT_STAT_SUSPEND;
                break;
            case PORT_RESET:
                if (dev) {
                    usb_send_msg(dev, USB_MSG_RESET);
                    port->wPortChange |= PORT_STAT_C_RESET;
                    
                    port->wPortStatus |= PORT_STAT_ENABLE;
                }
                break;
            case PORT_POWER:
                break;
            default:
                goto fail;
            }
            ret = 0;
        }
        break;
    case ClearPortFeature:
        {
            unsigned int n = index - 1;
            USBHubPort *port;
            USBDevice *dev;
            if (n >= s->nb_ports)
                goto fail;
            port = &s->ports[n];
            dev = port->port.dev;
            switch(value) {
            case PORT_ENABLE:
                port->wPortStatus &= ~PORT_STAT_ENABLE;
                break;
            case PORT_C_ENABLE:
                port->wPortChange &= ~PORT_STAT_C_ENABLE;
                break;
            case PORT_SUSPEND:
                port->wPortStatus &= ~PORT_STAT_SUSPEND;
                break;
            case PORT_C_SUSPEND:
                port->wPortChange &= ~PORT_STAT_C_SUSPEND;
                break;
            case PORT_C_CONNECTION:
                port->wPortChange &= ~PORT_STAT_C_CONNECTION;
                break;
            case PORT_C_OVERCURRENT:
                port->wPortChange &= ~PORT_STAT_C_OVERCURRENT;
                break;
            case PORT_C_RESET:
                port->wPortChange &= ~PORT_STAT_C_RESET;
                break;
            default:
                goto fail;
            }
            ret = 0;
        }
        break;
    case GetHubDescriptor:
        {
            unsigned int n, limit, var_hub_size = 0;
            memcpy(data, qemu_hub_hub_descriptor,
                   sizeof(qemu_hub_hub_descriptor));
            data[2] = s->nb_ports;

            
            limit = ((s->nb_ports + 1 + 7) / 8) + 7;
            for (n = 7; n < limit; n++) {
                data[n] = 0x00;
                var_hub_size++;
            }

            
            limit = limit + ((s->nb_ports + 7) / 8);
            for (;n < limit; n++) {
                data[n] = 0xff;
                var_hub_size++;
            }

            ret = sizeof(qemu_hub_hub_descriptor) + var_hub_size;
            data[0] = ret;
            break;
        }
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static int usb_hub_handle_data(USBDevice *dev, USBPacket *p)
{
    USBHubState *s = (USBHubState *)dev;
    int ret;

    switch(p->pid) {
    case USB_TOKEN_IN:
        if (p->devep == 1) {
            USBHubPort *port;
            unsigned int status;
            int i, n;
            n = (s->nb_ports + 1 + 7) / 8;
            if (p->len == 1) { 
                n = 1;
            } else if (n > p->len) {
                return USB_RET_BABBLE;
            }
            status = 0;
            for(i = 0; i < s->nb_ports; i++) {
                port = &s->ports[i];
                if (port->wPortChange)
                    status |= (1 << (i + 1));
            }
            if (status != 0) {
                for(i = 0; i < n; i++) {
                    p->data[i] = status >> (8 * i);
                }
                ret = n;
            } else {
                ret = USB_RET_NAK; 
            }
        } else {
            goto fail;
        }
        break;
    case USB_TOKEN_OUT:
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static int usb_hub_broadcast_packet(USBHubState *s, USBPacket *p)
{
    USBHubPort *port;
    USBDevice *dev;
    int i, ret;

    for(i = 0; i < s->nb_ports; i++) {
        port = &s->ports[i];
        dev = port->port.dev;
        if (dev && (port->wPortStatus & PORT_STAT_ENABLE)) {
            ret = dev->handle_packet(dev, p);
            if (ret != USB_RET_NODEV) {
                return ret;
            }
        }
    }
    return USB_RET_NODEV;
}

static int usb_hub_handle_packet(USBDevice *dev, USBPacket *p)
{
    USBHubState *s = (USBHubState *)dev;

#if defined(DEBUG) && 0
    printf("usb_hub: pid=0x%x\n", pid);
#endif
    if (dev->state == USB_STATE_DEFAULT &&
        dev->addr != 0 &&
        p->devaddr != dev->addr &&
        (p->pid == USB_TOKEN_SETUP ||
         p->pid == USB_TOKEN_OUT ||
         p->pid == USB_TOKEN_IN)) {
        
        return usb_hub_broadcast_packet(s, p);
    }
    return usb_generic_handle_packet(dev, p);
}

static void usb_hub_handle_destroy(USBDevice *dev)
{
    USBHubState *s = (USBHubState *)dev;

    qemu_free(s);
}

USBDevice *usb_hub_init(int nb_ports)
{
    USBHubState *s;
    USBHubPort *port;
    int i;

    if (nb_ports > MAX_PORTS)
        return NULL;
    s = qemu_mallocz(sizeof(USBHubState));
    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_hub_handle_packet;

    
    s->dev.handle_reset = usb_hub_handle_reset;
    s->dev.handle_control = usb_hub_handle_control;
    s->dev.handle_data = usb_hub_handle_data;
    s->dev.handle_destroy = usb_hub_handle_destroy;

    pstrcpy(s->dev.devname, sizeof(s->dev.devname), "QEMU USB Hub");

    s->nb_ports = nb_ports;
    for(i = 0; i < s->nb_ports; i++) {
        port = &s->ports[i];
        qemu_register_usb_port(&port->port, s, i, usb_hub_attach);
        port->wPortStatus = PORT_STAT_POWER;
        port->wPortChange = 0;
    }
    return (USBDevice *)s;
}
