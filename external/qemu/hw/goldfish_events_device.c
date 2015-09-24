/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#include "qemu_file.h"
#include "android/hw-events.h"
#include "android/charmap.h"
#include "android/globals.h"  
#include "android/multitouch-screen.h"
#include "irq.h"
#include "user-events.h"
#include "console.h"

#define MAX_EVENTS 256*4

enum {
    REG_READ        = 0x00,
    REG_SET_PAGE    = 0x00,
    REG_LEN         = 0x04,
    REG_DATA        = 0x08,

    PAGE_NAME       = 0x00000,
    PAGE_EVBITS     = 0x10000,
    PAGE_ABSDATA    = 0x20000 | EV_ABS,
};

enum {
    STATE_INIT = 0,  
    STATE_BUFFERED,  
    STATE_LIVE       
};


typedef struct
{
    uint32_t base;
    qemu_irq  irq;
    int pending;
    int page;

    unsigned events[MAX_EVENTS];
    unsigned first;
    unsigned last;
    unsigned state;

    const char *name;

    struct {
        size_t   len;
        uint8_t *bits;
    } ev_bits[EV_MAX + 1];

    int32_t *abs_info;
    size_t abs_info_count;
} events_state;

typedef struct ABSEntry {
    
    uint32_t    min;
    
    uint32_t    max;
    
    uint32_t    fuzz;
    uint32_t    flat;
} ABSEntry;


#define  EVENTS_STATE_SAVE_VERSION  2

#undef  QFIELD_STRUCT
#define QFIELD_STRUCT  events_state

QFIELD_BEGIN(events_state_fields)
    QFIELD_INT32(pending),
    QFIELD_INT32(page),
    QFIELD_BUFFER(events),
    QFIELD_INT32(first),
    QFIELD_INT32(last),
    QFIELD_INT32(state),
QFIELD_END

static void  events_state_save(QEMUFile*  f, void*  opaque)
{
    events_state*  s = opaque;

    qemu_put_struct(f, events_state_fields, s);
}

static int  events_state_load(QEMUFile*  f, void* opaque, int  version_id)
{
    events_state*  s = opaque;

    if (version_id != EVENTS_STATE_SAVE_VERSION)
        return -1;

    return qemu_get_struct(f, events_state_fields, s);
}

static void enqueue_event(events_state *s, unsigned int type, unsigned int code, int value)
{
    int  enqueued = s->last - s->first;

    if (enqueued < 0)
        enqueued += MAX_EVENTS;

    if (enqueued + 3 > MAX_EVENTS) {
        fprintf(stderr, "##KBD: Full queue, lose event\n");
        return;
    }

    if(s->first == s->last) {
	if (s->state == STATE_LIVE)
	  qemu_irq_raise(s->irq);
	else {
	  s->state = STATE_BUFFERED;
	}
    }

    

    s->events[s->last] = type;
    s->last = (s->last + 1) & (MAX_EVENTS-1);
    s->events[s->last] = code;
    s->last = (s->last + 1) & (MAX_EVENTS-1);
    s->events[s->last] = value;
    s->last = (s->last + 1) & (MAX_EVENTS-1);
}

static unsigned dequeue_event(events_state *s)
{
    unsigned n;

    if(s->first == s->last) {
        return 0;
    }

    n = s->events[s->first];

    s->first = (s->first + 1) & (MAX_EVENTS - 1);

    if(s->first == s->last) {
        qemu_irq_lower(s->irq);
    }
#ifdef TARGET_I386
    else if (((s->first + 2) & (MAX_EVENTS - 1)) < s->last ||
               (s->first & (MAX_EVENTS - 1)) > s->last) { 
        qemu_irq_lower(s->irq);
        qemu_irq_raise(s->irq);
    }
#endif
    return n;
}

static int get_page_len(events_state *s)
{
    int page = s->page;
    if (page == PAGE_NAME) {
        const char* name = s->name;
        return strlen(name);
    } if (page >= PAGE_EVBITS && page <= PAGE_EVBITS + EV_MAX)
        return s->ev_bits[page - PAGE_EVBITS].len;
    if (page == PAGE_ABSDATA)
        return s->abs_info_count * sizeof(s->abs_info[0]);
    return 0;
}

static int get_page_data(events_state *s, int offset)
{
    int page_len = get_page_len(s);
    int page = s->page;
    if (offset > page_len)
        return 0;
    if (page == PAGE_NAME) {
        const char* name = s->name;
        return name[offset];
    } if (page >= PAGE_EVBITS && page <= PAGE_EVBITS + EV_MAX)
        return s->ev_bits[page - PAGE_EVBITS].bits[offset];
    if (page == PAGE_ABSDATA) {
        return s->abs_info[offset / sizeof(s->abs_info[0])];
    }
    return 0;
}

static uint32_t events_read(void *x, target_phys_addr_t off)
{
    events_state *s = (events_state *) x;
    int offset = off; 

    if (offset == REG_LEN && s->page == PAGE_ABSDATA) {
	if (s->state == STATE_BUFFERED)
	  qemu_irq_raise(s->irq);
	s->state = STATE_LIVE;
    }

    if (offset == REG_READ)
        return dequeue_event(s);
    else if (offset == REG_LEN)
        return get_page_len(s);
    else if (offset >= REG_DATA)
        return get_page_data(s, offset - REG_DATA);
    return 0; 
}

static void events_write(void *x, target_phys_addr_t off, uint32_t val)
{
    events_state *s = (events_state *) x;
    int offset = off; 
    if (offset == REG_SET_PAGE)
        s->page = val;
}

static CPUReadMemoryFunc *events_readfn[] = {
   events_read,
   events_read,
   events_read
};

static CPUWriteMemoryFunc *events_writefn[] = {
   events_write,
   events_write,
   events_write
};

static void events_put_keycode(void *x, int keycode)
{
    events_state *s = (events_state *) x;

    enqueue_event(s, EV_KEY, keycode&0x1ff, (keycode&0x200) ? 1 : 0);
}

static void events_put_mouse(void *opaque, int dx, int dy, int dz, int buttons_state)
{
    events_state *s = (events_state *) opaque;
    if (dz == 0) {
        if (androidHwConfig_isScreenMultiTouch(android_hw)) {
            
            multitouch_update_pointer(MTES_MOUSE, 0, dx, dy,
                                      (buttons_state & 1) ? 0x81 : 0);
        } else if (androidHwConfig_isScreenTouch(android_hw)) {
            enqueue_event(s, EV_ABS, ABS_X, dx);
            enqueue_event(s, EV_ABS, ABS_Y, dy);
            enqueue_event(s, EV_ABS, ABS_Z, dz);
            enqueue_event(s, EV_KEY, BTN_TOUCH, buttons_state&1);
            enqueue_event(s, EV_SYN, 0, 0);
        }
    } else {
        enqueue_event(s, EV_REL, REL_X, dx);
        enqueue_event(s, EV_REL, REL_Y, dy);
        enqueue_event(s, EV_SYN, 0, 0);
    }
}

static void  events_put_generic(void*  opaque, int  type, int  code, int  value)
{
    events_state *s = (events_state *) opaque;

    enqueue_event(s, type, code, value);
}

static void
events_set_bits(events_state *s, int type, int bitl, int bith)
{
    uint8_t *bits;
    uint8_t maskl, maskh;
    int il, ih;
    il = bitl / 8;
    ih = bith / 8;
    if (ih >= s->ev_bits[type].len) {
        bits = qemu_mallocz(ih + 1);
        if (bits == NULL)
            return;
        memcpy(bits, s->ev_bits[type].bits, s->ev_bits[type].len);
        qemu_free(s->ev_bits[type].bits);
        s->ev_bits[type].bits = bits;
        s->ev_bits[type].len = ih + 1;
    }
    else
        bits = s->ev_bits[type].bits;
    maskl = 0xffU << (bitl & 7);
    maskh = 0xffU >> (7 - (bith & 7));
    if (il >= ih)
        maskh &= maskl;
    else {
        bits[il] |= maskl;
        while (++il < ih)
            bits[il] = 0xff;
    }
    bits[ih] |= maskh;
}

static void
events_set_bit(events_state* s, int  type, int  bit)
{
    events_set_bits(s, type, bit, bit);
}

static void
events_clr_bit(events_state* s, int type, int bit)
{
    int ii = bit / 8;
    if (ii < s->ev_bits[type].len) {
        uint8_t* bits = s->ev_bits[type].bits;
        uint8_t  mask = 0x01U << (bit & 7);
        bits[ii] &= ~mask;
    }
}

void events_dev_init(uint32_t base, qemu_irq irq)
{
    events_state *s;
    int iomemtype;
    AndroidHwConfig*  config = android_hw;

    s = (events_state *) qemu_mallocz(sizeof(events_state));

    

    events_set_bit (s, EV_SYN, EV_KEY );

    events_set_bit(s, EV_KEY, KEY_HOME);
    events_set_bit(s, EV_KEY, KEY_BACK);
    events_set_bit(s, EV_KEY, KEY_SEND);
    events_set_bit(s, EV_KEY, KEY_END);
    events_set_bit(s, EV_KEY, KEY_SOFT1);
    events_set_bit(s, EV_KEY, KEY_VOLUMEUP);
    events_set_bit(s, EV_KEY, KEY_VOLUMEDOWN);
    events_set_bit(s, EV_KEY, KEY_SOFT2);
    events_set_bit(s, EV_KEY, KEY_POWER);
    events_set_bit(s, EV_KEY, KEY_SEARCH);

    if (config->hw_dPad) {
        events_set_bit(s, EV_KEY, KEY_DOWN);
        events_set_bit(s, EV_KEY, KEY_UP);
        events_set_bit(s, EV_KEY, KEY_LEFT);
        events_set_bit(s, EV_KEY, KEY_RIGHT);
        events_set_bit(s, EV_KEY, KEY_CENTER);
    }

    if (config->hw_trackBall) {
        events_set_bit(s, EV_KEY, BTN_MOUSE);
    }
    if (androidHwConfig_isScreenTouch(config)) {
        events_set_bit(s, EV_KEY, BTN_TOUCH);
    }

    if (strcmp(config->hw_camera_back, "none") ||
        strcmp(config->hw_camera_front, "none")) {
        
        events_set_bit(s, EV_KEY, KEY_CAMERA);
    }

    if (config->hw_keyboard) {
        events_set_bits(s, EV_KEY, 1, 0xff);
        events_set_bits(s, EV_KEY, 0x160, 0x1ff);

        if (!config->hw_dPad) {
            events_clr_bit(s, EV_KEY, KEY_DOWN);
            events_clr_bit(s, EV_KEY, KEY_UP);
            events_clr_bit(s, EV_KEY, KEY_LEFT);
            events_clr_bit(s, EV_KEY, KEY_RIGHT);
            events_clr_bit(s, EV_KEY, KEY_CENTER);
        }
    }

    if (config->hw_trackBall) {
        events_set_bit (s, EV_SYN, EV_REL );
        events_set_bits(s, EV_REL, REL_X, REL_Y);
    }

    if (!androidHwConfig_isScreenNoTouch(config)) {
        ABSEntry* abs_values;

        events_set_bit (s, EV_SYN, EV_ABS );
        events_set_bits(s, EV_ABS, ABS_X, ABS_Z);
        s->abs_info_count = androidHwConfig_isScreenMultiTouch(config) ? ABS_MAX * 4 : 3 * 4;
        const int abs_size = sizeof(uint32_t) * s->abs_info_count;
        s->abs_info = malloc(abs_size);
        memset(s->abs_info, 0, abs_size);
        abs_values = (ABSEntry*)s->abs_info;

        abs_values[ABS_X].max = config->hw_lcd_width-1;
        abs_values[ABS_Y].max = config->hw_lcd_height-1;
        abs_values[ABS_Z].max = 1;

        if (androidHwConfig_isScreenMultiTouch(config)) {
            events_set_bit(s, EV_ABS, ABS_MT_SLOT);
            events_set_bit(s, EV_ABS, ABS_MT_POSITION_X);
            events_set_bit(s, EV_ABS, ABS_MT_POSITION_Y);
            events_set_bit(s, EV_ABS, ABS_MT_TRACKING_ID);
            events_set_bit(s, EV_ABS, ABS_MT_TOUCH_MAJOR);
            events_set_bit(s, EV_ABS, ABS_MT_PRESSURE);

            abs_values[ABS_MT_SLOT].max = multitouch_get_max_slot();
            abs_values[ABS_MT_TRACKING_ID].max = abs_values[ABS_MT_SLOT].max + 1;
            abs_values[ABS_MT_POSITION_X].max = abs_values[ABS_X].max;
            abs_values[ABS_MT_POSITION_Y].max = abs_values[ABS_Y].max;
            abs_values[ABS_MT_TOUCH_MAJOR].max = 0x7fffffff; 
            abs_values[ABS_MT_PRESSURE].max = 0x100; 
        }
    }

    if (config->hw_keyboard && config->hw_keyboard_lid) {
        events_set_bit(s, EV_SYN, EV_SW);
        events_set_bit(s, EV_SW, 0);
    }

    iomemtype = cpu_register_io_memory(events_readfn, events_writefn, s);

    cpu_register_physical_memory(base, 0xfff, iomemtype);

    qemu_add_kbd_event_handler(events_put_keycode, s);
    qemu_add_mouse_event_handler(events_put_mouse, s, 1, "goldfish-events");

    s->base = base;
    s->irq = irq;

    s->first = 0;
    s->last = 0;
    s->state = STATE_INIT;
    s->name = qemu_strdup(config->hw_keyboard_charmap);

    user_event_register_generic(s, events_put_generic);

    register_savevm( "events_state", 0, EVENTS_STATE_SAVE_VERSION,
                      events_state_save, events_state_load, s );
}
