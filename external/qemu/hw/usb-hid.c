/*
 * QEMU USB HID devices
 *
 * Copyright (c) 2005 Fabrice Bellard
 * Copyright (c) 2007 OpenMoko, Inc.  (andrew@openedhand.com)
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
#include "console.h"
#include "usb.h"

#define GET_REPORT   0xa101
#define GET_IDLE     0xa102
#define GET_PROTOCOL 0xa103
#define SET_REPORT   0x2109
#define SET_IDLE     0x210a
#define SET_PROTOCOL 0x210b

#define USB_DT_HID    0x21
#define USB_DT_REPORT 0x22
#define USB_DT_PHY    0x23

#define USB_MOUSE     1
#define USB_TABLET    2
#define USB_KEYBOARD  3

typedef struct USBMouseState {
    int dx, dy, dz, buttons_state;
    int x, y;
    int mouse_grabbed;
    QEMUPutMouseEntry *eh_entry;
} USBMouseState;

typedef struct USBKeyboardState {
    uint16_t modifiers;
    uint8_t leds;
    uint8_t key[16];
    int keys;
} USBKeyboardState;

typedef struct USBHIDState {
    USBDevice dev;
    union {
        USBMouseState ptr;
        USBKeyboardState kbd;
    };
    int kind;
    int protocol;
    uint8_t idle;
    int changed;
    void *datain_opaque;
    void (*datain)(void *);
} USBHIDState;

static const uint8_t qemu_mouse_dev_descriptor[] = {
	0x12,       
	0x01,       
	0x00, 0x01, 

	0x00,	    
	0x00,	    
	0x00,       
	0x08,       

	0x27, 0x06, 
 	0x01, 0x00, 
	0x00, 0x00, 

	0x03,       
	0x02,       
	0x01,       
	0x01        
};

static const uint8_t qemu_mouse_config_descriptor[] = {
	
	0x09,       
	0x02,       
	0x22, 0x00, 
	0x01,       
	0x01,       
	0x04,       
	0xa0,       
	50,         


	
	0x09,       
	0x04,       
	0x00,       
	0x00,       
	0x01,       
	0x03,       
	0x01,       
	0x02,       
	0x07,       

        
        0x09,        
        0x21,        
        0x01, 0x00,  
        0x00,        
        0x01,        
        0x22,        
        52, 0,       

	
	0x07,       
	0x05,       
	0x81,       
 	0x03,       
 	0x04, 0x00, 
	0x0a,       
};

static const uint8_t qemu_tablet_config_descriptor[] = {
	
	0x09,       
	0x02,       
	0x22, 0x00, 
	0x01,       
	0x01,       
	0x05,       
	0xa0,       
	50,         


	
	0x09,       
	0x04,       
	0x00,       
	0x00,       
	0x01,       
	0x03,       
	0x01,       
	0x02,       
	0x07,       

        
        0x09,        
        0x21,        
        0x01, 0x00,  
        0x00,        
        0x01,        
        0x22,        
        74, 0,       

	
	0x07,       
	0x05,       
	0x81,       
 	0x03,       
 	0x08, 0x00, 
	0x0a,       
};

static const uint8_t qemu_keyboard_config_descriptor[] = {
    
    0x09,		
    USB_DT_CONFIG,	
    0x22, 0x00,		
    0x01,		
    0x01,		
    0x06,		
    0xa0,		
    0x32,		


    
    0x09,		
    USB_DT_INTERFACE,	
    0x00,		
    0x00,		
    0x01,		
    0x03,		
    0x01,		
    0x01,		
    0x07,		

    
    0x09,		
    USB_DT_HID,		
    0x11, 0x01,		
    0x00,		
    0x01,		
    USB_DT_REPORT,	
    0x3f, 0x00,		

    
    0x07,		
    USB_DT_ENDPOINT,	
    USB_DIR_IN | 0x01,	
    0x03,		
    0x08, 0x00,		
    0x0a,		
};

static const uint8_t qemu_mouse_hid_report_descriptor[] = {
    0x05, 0x01,		
    0x09, 0x02,		
    0xa1, 0x01,		
    0x09, 0x01,		
    0xa1, 0x00,		
    0x05, 0x09,		
    0x19, 0x01,		
    0x29, 0x03,		
    0x15, 0x00,		
    0x25, 0x01,		
    0x95, 0x03,		
    0x75, 0x01,		
    0x81, 0x02,		
    0x95, 0x01,		
    0x75, 0x05,		
    0x81, 0x01,		
    0x05, 0x01,		
    0x09, 0x30,		
    0x09, 0x31,		
    0x09, 0x38,		
    0x15, 0x81,		
    0x25, 0x7f,		
    0x75, 0x08,		
    0x95, 0x03,		
    0x81, 0x06,		
    0xc0,		
    0xc0,		
};

static const uint8_t qemu_tablet_hid_report_descriptor[] = {
    0x05, 0x01,		
    0x09, 0x01,		
    0xa1, 0x01,		
    0x09, 0x01,		
    0xa1, 0x00,		
    0x05, 0x09,		
    0x19, 0x01,		
    0x29, 0x03,		
    0x15, 0x00,		
    0x25, 0x01,		
    0x95, 0x03,		
    0x75, 0x01,		
    0x81, 0x02,		
    0x95, 0x01,		
    0x75, 0x05,		
    0x81, 0x01,		
    0x05, 0x01,		
    0x09, 0x30,		
    0x09, 0x31,		
    0x15, 0x00,		
    0x26, 0xff, 0x7f,	
    0x35, 0x00,		
    0x46, 0xff, 0x7f,	
    0x75, 0x10,		
    0x95, 0x02,		
    0x81, 0x02,		
    0x05, 0x01,		
    0x09, 0x38,		
    0x15, 0x81,		
    0x25, 0x7f,		
    0x35, 0x00,		
    0x45, 0x00,		
    0x75, 0x08,		
    0x95, 0x01,		
    0x81, 0x06,		
    0xc0,		
    0xc0,		
};

static const uint8_t qemu_keyboard_hid_report_descriptor[] = {
    0x05, 0x01,		
    0x09, 0x06,		
    0xa1, 0x01,		
    0x75, 0x01,		
    0x95, 0x08,		
    0x05, 0x07,		
    0x19, 0xe0,		
    0x29, 0xe7,		
    0x15, 0x00,		
    0x25, 0x01,		
    0x81, 0x02,		
    0x95, 0x01,		
    0x75, 0x08,		
    0x81, 0x01,		
    0x95, 0x05,		
    0x75, 0x01,		
    0x05, 0x08,		
    0x19, 0x01,		
    0x29, 0x05,		
    0x91, 0x02,		
    0x95, 0x01,		
    0x75, 0x03,		
    0x91, 0x01,		
    0x95, 0x06,		
    0x75, 0x08,		
    0x15, 0x00,		
    0x25, 0xff,		
    0x05, 0x07,		
    0x19, 0x00,		
    0x29, 0xff,		
    0x81, 0x00,		
    0xc0,		
};

#define USB_HID_USAGE_ERROR_ROLLOVER	0x01
#define USB_HID_USAGE_POSTFAIL		0x02
#define USB_HID_USAGE_ERROR_UNDEFINED	0x03

static const uint8_t usb_hid_usage_keys[0x100] = {
    0x00, 0x29, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x2d, 0x2e, 0x2a, 0x2b,
    0x14, 0x1a, 0x08, 0x15, 0x17, 0x1c, 0x18, 0x0c,
    0x12, 0x13, 0x2f, 0x30, 0x28, 0xe0, 0x04, 0x16,
    0x07, 0x09, 0x0a, 0x0b, 0x0d, 0x0e, 0x0f, 0x33,
    0x34, 0x35, 0xe1, 0x31, 0x1d, 0x1b, 0x06, 0x19,
    0x05, 0x11, 0x10, 0x36, 0x37, 0x38, 0xe5, 0x55,
    0xe2, 0x2c, 0x32, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
    0x3f, 0x40, 0x41, 0x42, 0x43, 0x53, 0x47, 0x5f,
    0x60, 0x61, 0x56, 0x5c, 0x5d, 0x5e, 0x57, 0x59,
    0x5a, 0x5b, 0x62, 0x63, 0x00, 0x00, 0x00, 0x44,
    0x45, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
    0xe8, 0xe9, 0x71, 0x72, 0x73, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0xe7, 0x65,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x58, 0xe4, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x46,
    0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x4a,
    0x52, 0x4b, 0x00, 0x50, 0x00, 0x4f, 0x00, 0x4d,
    0x51, 0x4e, 0x49, 0x4c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static void usb_hid_changed(USBHIDState *hs)
{
    hs->changed = 1;

    if (hs->datain)
        hs->datain(hs->datain_opaque);
}

static void usb_mouse_event(void *opaque,
                            int dx1, int dy1, int dz1, int buttons_state)
{
    USBHIDState *hs = opaque;
    USBMouseState *s = &hs->ptr;

    s->dx += dx1;
    s->dy += dy1;
    s->dz += dz1;
    s->buttons_state = buttons_state;

    usb_hid_changed(hs);
}

static void usb_tablet_event(void *opaque,
			     int x, int y, int dz, int buttons_state)
{
    USBHIDState *hs = opaque;
    USBMouseState *s = &hs->ptr;

    s->x = x;
    s->y = y;
    s->dz += dz;
    s->buttons_state = buttons_state;

    usb_hid_changed(hs);
}

static void usb_keyboard_event(void *opaque, int keycode)
{
    USBHIDState *hs = opaque;
    USBKeyboardState *s = &hs->kbd;
    uint8_t hid_code, key;
    int i;

    key = keycode & 0x7f;
    hid_code = usb_hid_usage_keys[key | ((s->modifiers >> 1) & (1 << 7))];
    s->modifiers &= ~(1 << 8);

    switch (hid_code) {
    case 0x00:
        return;

    case 0xe0:
        if (s->modifiers & (1 << 9)) {
            s->modifiers ^= 3 << 8;
            return;
        }
    case 0xe1 ... 0xe7:
        if (keycode & (1 << 7)) {
            s->modifiers &= ~(1 << (hid_code & 0x0f));
            return;
        }
    case 0xe8 ... 0xef:
        s->modifiers |= 1 << (hid_code & 0x0f);
        return;
    }

    if (keycode & (1 << 7)) {
        for (i = s->keys - 1; i >= 0; i --)
            if (s->key[i] == hid_code) {
                s->key[i] = s->key[-- s->keys];
                s->key[s->keys] = 0x00;
                usb_hid_changed(hs);
                break;
            }
        if (i < 0)
            return;
    } else {
        for (i = s->keys - 1; i >= 0; i --)
            if (s->key[i] == hid_code)
                break;
        if (i < 0) {
            if (s->keys < sizeof(s->key))
                s->key[s->keys ++] = hid_code;
        } else
            return;
    }

    usb_hid_changed(hs);
}

static inline int int_clamp(int val, int vmin, int vmax)
{
    if (val < vmin)
        return vmin;
    else if (val > vmax)
        return vmax;
    else
        return val;
}

static int usb_mouse_poll(USBHIDState *hs, uint8_t *buf, int len)
{
    int dx, dy, dz, b, l;
    USBMouseState *s = &hs->ptr;

    if (!s->mouse_grabbed) {
	s->eh_entry = qemu_add_mouse_event_handler(usb_mouse_event, hs,
                                                  0, "QEMU USB Mouse");
	s->mouse_grabbed = 1;
    }

    dx = int_clamp(s->dx, -127, 127);
    dy = int_clamp(s->dy, -127, 127);
    dz = int_clamp(s->dz, -127, 127);

    s->dx -= dx;
    s->dy -= dy;
    s->dz -= dz;

    
    dz = 0 - dz;

    b = 0;
    if (s->buttons_state & MOUSE_EVENT_LBUTTON)
        b |= 0x01;
    if (s->buttons_state & MOUSE_EVENT_RBUTTON)
        b |= 0x02;
    if (s->buttons_state & MOUSE_EVENT_MBUTTON)
        b |= 0x04;

    l = 0;
    if (len > l)
        buf[l ++] = b;
    if (len > l)
        buf[l ++] = dx;
    if (len > l)
        buf[l ++] = dy;
    if (len > l)
        buf[l ++] = dz;
    return l;
}

static int usb_tablet_poll(USBHIDState *hs, uint8_t *buf, int len)
{
    int dz, b, l;
    USBMouseState *s = &hs->ptr;

    if (!s->mouse_grabbed) {
	s->eh_entry = qemu_add_mouse_event_handler(usb_tablet_event, hs,
                                                  1, "QEMU USB Tablet");
	s->mouse_grabbed = 1;
    }

    dz = int_clamp(s->dz, -127, 127);
    s->dz -= dz;

    
    dz = 0 - dz;
    b = 0;
    if (s->buttons_state & MOUSE_EVENT_LBUTTON)
        b |= 0x01;
    if (s->buttons_state & MOUSE_EVENT_RBUTTON)
        b |= 0x02;
    if (s->buttons_state & MOUSE_EVENT_MBUTTON)
        b |= 0x04;

    buf[0] = b;
    buf[1] = s->x & 0xff;
    buf[2] = s->x >> 8;
    buf[3] = s->y & 0xff;
    buf[4] = s->y >> 8;
    buf[5] = dz;
    l = 6;

    return l;
}

static int usb_keyboard_poll(USBKeyboardState *s, uint8_t *buf, int len)
{
    if (len < 2)
        return 0;

    buf[0] = s->modifiers & 0xff;
    buf[1] = 0;
    if (s->keys > 6)
        memset(buf + 2, USB_HID_USAGE_ERROR_ROLLOVER, MIN(8, len) - 2);
    else
        memcpy(buf + 2, s->key, MIN(8, len) - 2);

    return MIN(8, len);
}

static int usb_keyboard_write(USBKeyboardState *s, uint8_t *buf, int len)
{
    if (len > 0) {
        s->leds = buf[0];
    }
    return 0;
}

static void usb_mouse_handle_reset(USBDevice *dev)
{
    USBHIDState *s = (USBHIDState *)dev;

    s->ptr.dx = 0;
    s->ptr.dy = 0;
    s->ptr.dz = 0;
    s->ptr.x = 0;
    s->ptr.y = 0;
    s->ptr.buttons_state = 0;
    s->protocol = 1;
}

static void usb_keyboard_handle_reset(USBDevice *dev)
{
    USBHIDState *s = (USBHIDState *)dev;

    qemu_add_kbd_event_handler(usb_keyboard_event, s);
    s->protocol = 1;
}

static int usb_hid_handle_control(USBDevice *dev, int request, int value,
                                  int index, int length, uint8_t *data)
{
    USBHIDState *s = (USBHIDState *)dev;
    int ret = 0;

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
            memcpy(data, qemu_mouse_dev_descriptor,
                   sizeof(qemu_mouse_dev_descriptor));
            ret = sizeof(qemu_mouse_dev_descriptor);
            break;
        case USB_DT_CONFIG:
	    if (s->kind == USB_MOUSE) {
		memcpy(data, qemu_mouse_config_descriptor,
		       sizeof(qemu_mouse_config_descriptor));
		ret = sizeof(qemu_mouse_config_descriptor);
	    } else if (s->kind == USB_TABLET) {
		memcpy(data, qemu_tablet_config_descriptor,
		       sizeof(qemu_tablet_config_descriptor));
		ret = sizeof(qemu_tablet_config_descriptor);
            } else if (s->kind == USB_KEYBOARD) {
                memcpy(data, qemu_keyboard_config_descriptor,
                       sizeof(qemu_keyboard_config_descriptor));
                ret = sizeof(qemu_keyboard_config_descriptor);
            }
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
                
                ret = set_usb_string(data, "1");
                break;
            case 2:
                
                ret = set_usb_string(data, s->dev.devname);
                break;
            case 3:
                
                ret = set_usb_string(data, "QEMU " QEMU_VERSION);
                break;
            case 4:
                ret = set_usb_string(data, "HID Mouse");
                break;
            case 5:
                ret = set_usb_string(data, "HID Tablet");
                break;
            case 6:
                ret = set_usb_string(data, "HID Keyboard");
                break;
            case 7:
                ret = set_usb_string(data, "Endpoint1 Interrupt Pipe");
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
        
    case InterfaceRequest | USB_REQ_GET_DESCRIPTOR:
        switch(value >> 8) {
        case 0x22:
	    if (s->kind == USB_MOUSE) {
		memcpy(data, qemu_mouse_hid_report_descriptor,
		       sizeof(qemu_mouse_hid_report_descriptor));
		ret = sizeof(qemu_mouse_hid_report_descriptor);
	    } else if (s->kind == USB_TABLET) {
		memcpy(data, qemu_tablet_hid_report_descriptor,
		       sizeof(qemu_tablet_hid_report_descriptor));
		ret = sizeof(qemu_tablet_hid_report_descriptor);
            } else if (s->kind == USB_KEYBOARD) {
                memcpy(data, qemu_keyboard_hid_report_descriptor,
                       sizeof(qemu_keyboard_hid_report_descriptor));
                ret = sizeof(qemu_keyboard_hid_report_descriptor);
            }
            break;
        default:
            goto fail;
        }
        break;
    case GET_REPORT:
	if (s->kind == USB_MOUSE)
            ret = usb_mouse_poll(s, data, length);
	else if (s->kind == USB_TABLET)
            ret = usb_tablet_poll(s, data, length);
        else if (s->kind == USB_KEYBOARD)
            ret = usb_keyboard_poll(&s->kbd, data, length);
        break;
    case SET_REPORT:
        if (s->kind == USB_KEYBOARD)
            ret = usb_keyboard_write(&s->kbd, data, length);
        else
            goto fail;
        break;
    case GET_PROTOCOL:
        if (s->kind != USB_KEYBOARD)
            goto fail;
        ret = 1;
        data[0] = s->protocol;
        break;
    case SET_PROTOCOL:
        if (s->kind != USB_KEYBOARD)
            goto fail;
        ret = 0;
        s->protocol = value;
        break;
    case GET_IDLE:
        ret = 1;
        data[0] = s->idle;
        break;
    case SET_IDLE:
        s->idle = (uint8_t) (value >> 8);
        ret = 0;
        break;
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static int usb_hid_handle_data(USBDevice *dev, USBPacket *p)
{
    USBHIDState *s = (USBHIDState *)dev;
    int ret = 0;

    switch(p->pid) {
    case USB_TOKEN_IN:
        if (p->devep == 1) {
            
            if (!(s->changed || s->idle))
                return USB_RET_NAK;
            s->changed = 0;
            if (s->kind == USB_MOUSE)
                ret = usb_mouse_poll(s, p->data, p->len);
            else if (s->kind == USB_TABLET)
                ret = usb_tablet_poll(s, p->data, p->len);
            else if (s->kind == USB_KEYBOARD)
                ret = usb_keyboard_poll(&s->kbd, p->data, p->len);
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

static void usb_hid_handle_destroy(USBDevice *dev)
{
    USBHIDState *s = (USBHIDState *)dev;

    if (s->kind != USB_KEYBOARD)
        qemu_remove_mouse_event_handler(s->ptr.eh_entry);
    
    qemu_free(s);
}

USBDevice *usb_tablet_init(void)
{
    USBHIDState *s;

    s = qemu_mallocz(sizeof(USBHIDState));
    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_generic_handle_packet;

    s->dev.handle_reset = usb_mouse_handle_reset;
    s->dev.handle_control = usb_hid_handle_control;
    s->dev.handle_data = usb_hid_handle_data;
    s->dev.handle_destroy = usb_hid_handle_destroy;
    s->kind = USB_TABLET;
    
    s->changed = 1;

    pstrcpy(s->dev.devname, sizeof(s->dev.devname), "QEMU USB Tablet");

    return (USBDevice *)s;
}

USBDevice *usb_mouse_init(void)
{
    USBHIDState *s;

    s = qemu_mallocz(sizeof(USBHIDState));
    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_generic_handle_packet;

    s->dev.handle_reset = usb_mouse_handle_reset;
    s->dev.handle_control = usb_hid_handle_control;
    s->dev.handle_data = usb_hid_handle_data;
    s->dev.handle_destroy = usb_hid_handle_destroy;
    s->kind = USB_MOUSE;
    
    s->changed = 1;

    pstrcpy(s->dev.devname, sizeof(s->dev.devname), "QEMU USB Mouse");

    return (USBDevice *)s;
}

USBDevice *usb_keyboard_init(void)
{
    USBHIDState *s;

    s = qemu_mallocz(sizeof(USBHIDState));
    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_generic_handle_packet;

    s->dev.handle_reset = usb_keyboard_handle_reset;
    s->dev.handle_control = usb_hid_handle_control;
    s->dev.handle_data = usb_hid_handle_data;
    s->dev.handle_destroy = usb_hid_handle_destroy;
    s->kind = USB_KEYBOARD;

    pstrcpy(s->dev.devname, sizeof(s->dev.devname), "QEMU USB Keyboard");

    return (USBDevice *) s;
}

void usb_hid_datain_cb(USBDevice *dev, void *opaque, void (*datain)(void *))
{
    USBHIDState *s = (USBHIDState *)dev;

    s->datain_opaque = opaque;
    s->datain = datain;
}
