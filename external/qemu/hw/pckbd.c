/*
 * QEMU PC keyboard emulation
 *
 * Copyright (c) 2003 Fabrice Bellard
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
#include "isa.h"
#include "pc.h"
#include "ps2.h"
#include "sysemu.h"


#define KBD_CCMD_READ_MODE	0x20	
#define KBD_CCMD_WRITE_MODE	0x60	
#define KBD_CCMD_GET_VERSION	0xA1	
#define KBD_CCMD_MOUSE_DISABLE	0xA7	
#define KBD_CCMD_MOUSE_ENABLE	0xA8	
#define KBD_CCMD_TEST_MOUSE	0xA9	
#define KBD_CCMD_SELF_TEST	0xAA	
#define KBD_CCMD_KBD_TEST	0xAB	
#define KBD_CCMD_KBD_DISABLE	0xAD	
#define KBD_CCMD_KBD_ENABLE	0xAE	
#define KBD_CCMD_READ_INPORT    0xC0    
#define KBD_CCMD_READ_OUTPORT	0xD0    
#define KBD_CCMD_WRITE_OUTPORT	0xD1    
#define KBD_CCMD_WRITE_OBUF	0xD2
#define KBD_CCMD_WRITE_AUX_OBUF	0xD3    
#define KBD_CCMD_WRITE_MOUSE	0xD4	
#define KBD_CCMD_DISABLE_A20    0xDD    
#define KBD_CCMD_ENABLE_A20     0xDF    
#define KBD_CCMD_RESET	        0xFE

#define KBD_CMD_SET_LEDS	0xED	
#define KBD_CMD_ECHO     	0xEE
#define KBD_CMD_GET_ID 	        0xF2	
#define KBD_CMD_SET_RATE	0xF3	
#define KBD_CMD_ENABLE		0xF4	
#define KBD_CMD_RESET_DISABLE	0xF5	
#define KBD_CMD_RESET_ENABLE   	0xF6    
#define KBD_CMD_RESET		0xFF	

#define KBD_REPLY_POR		0xAA	
#define KBD_REPLY_ACK		0xFA	
#define KBD_REPLY_RESEND	0xFE	

#define KBD_STAT_OBF 		0x01	
#define KBD_STAT_IBF 		0x02	
#define KBD_STAT_SELFTEST	0x04	
#define KBD_STAT_CMD		0x08	
#define KBD_STAT_UNLOCKED	0x10	
#define KBD_STAT_MOUSE_OBF	0x20	
#define KBD_STAT_GTO 		0x40	
#define KBD_STAT_PERR 		0x80	

#define KBD_MODE_KBD_INT	0x01	
#define KBD_MODE_MOUSE_INT	0x02	
#define KBD_MODE_SYS 		0x04	
#define KBD_MODE_NO_KEYLOCK	0x08	
#define KBD_MODE_DISABLE_KBD	0x10	
#define KBD_MODE_DISABLE_MOUSE	0x20	
#define KBD_MODE_KCC 		0x40	
#define KBD_MODE_RFU		0x80

#define AUX_SET_SCALE11		0xE6	
#define AUX_SET_SCALE21		0xE7	
#define AUX_SET_RES		0xE8	
#define AUX_GET_SCALE		0xE9	
#define AUX_SET_STREAM		0xEA	
#define AUX_POLL		0xEB	
#define AUX_RESET_WRAP		0xEC	
#define AUX_SET_WRAP		0xEE	
#define AUX_SET_REMOTE		0xF0	
#define AUX_GET_TYPE		0xF2	
#define AUX_SET_SAMPLE		0xF3	
#define AUX_ENABLE_DEV		0xF4	
#define AUX_DISABLE_DEV		0xF5	
#define AUX_SET_DEFAULT		0xF6
#define AUX_RESET		0xFF	
#define AUX_ACK			0xFA	

#define MOUSE_STATUS_REMOTE     0x40
#define MOUSE_STATUS_ENABLED    0x20
#define MOUSE_STATUS_SCALE21    0x10

#define KBD_PENDING_KBD         1
#define KBD_PENDING_AUX         2

typedef struct KBDState {
    uint8_t write_cmd; 
    uint8_t status;
    uint8_t mode;
    
    uint8_t pending;
    void *kbd;
    void *mouse;

    qemu_irq irq_kbd;
    qemu_irq irq_mouse;
    target_phys_addr_t mask;
} KBDState;

static KBDState kbd_state;

static void kbd_update_irq(KBDState *s)
{
    int irq_kbd_level, irq_mouse_level;

    irq_kbd_level = 0;
    irq_mouse_level = 0;
    s->status &= ~(KBD_STAT_OBF | KBD_STAT_MOUSE_OBF);
    if (s->pending) {
        s->status |= KBD_STAT_OBF;
        
        if (s->pending == KBD_PENDING_AUX) {
            s->status |= KBD_STAT_MOUSE_OBF;
            if (s->mode & KBD_MODE_MOUSE_INT)
                irq_mouse_level = 1;
        } else {
            if ((s->mode & KBD_MODE_KBD_INT) &&
                !(s->mode & KBD_MODE_DISABLE_KBD))
                irq_kbd_level = 1;
        }
    }
    qemu_set_irq(s->irq_kbd, irq_kbd_level);
    qemu_set_irq(s->irq_mouse, irq_mouse_level);
}

static void kbd_update_kbd_irq(void *opaque, int level)
{
    KBDState *s = (KBDState *)opaque;

    if (level)
        s->pending |= KBD_PENDING_KBD;
    else
        s->pending &= ~KBD_PENDING_KBD;
    kbd_update_irq(s);
}

static void kbd_update_aux_irq(void *opaque, int level)
{
    KBDState *s = (KBDState *)opaque;

    if (level)
        s->pending |= KBD_PENDING_AUX;
    else
        s->pending &= ~KBD_PENDING_AUX;
    kbd_update_irq(s);
}

static uint32_t kbd_read_status(void *opaque, uint32_t addr)
{
    KBDState *s = opaque;
    int val;
    val = s->status;
#if defined(DEBUG_KBD)
    printf("kbd: read status=0x%02x\n", val);
#endif
    return val;
}

static void kbd_queue(KBDState *s, int b, int aux)
{
    if (aux)
        ps2_queue(s->mouse, b);
    else
        ps2_queue(s->kbd, b);
}

static void kbd_write_command(void *opaque, uint32_t addr, uint32_t val)
{
    KBDState *s = opaque;

#ifdef DEBUG_KBD
    printf("kbd: write cmd=0x%02x\n", val);
#endif
    switch(val) {
    case KBD_CCMD_READ_MODE:
        kbd_queue(s, s->mode, 0);
        break;
    case KBD_CCMD_WRITE_MODE:
    case KBD_CCMD_WRITE_OBUF:
    case KBD_CCMD_WRITE_AUX_OBUF:
    case KBD_CCMD_WRITE_MOUSE:
    case KBD_CCMD_WRITE_OUTPORT:
        s->write_cmd = val;
        break;
    case KBD_CCMD_MOUSE_DISABLE:
        s->mode |= KBD_MODE_DISABLE_MOUSE;
        break;
    case KBD_CCMD_MOUSE_ENABLE:
        s->mode &= ~KBD_MODE_DISABLE_MOUSE;
        break;
    case KBD_CCMD_TEST_MOUSE:
        kbd_queue(s, 0x00, 0);
        break;
    case KBD_CCMD_SELF_TEST:
        s->status |= KBD_STAT_SELFTEST;
        kbd_queue(s, 0x55, 0);
        break;
    case KBD_CCMD_KBD_TEST:
        kbd_queue(s, 0x00, 0);
        break;
    case KBD_CCMD_KBD_DISABLE:
        s->mode |= KBD_MODE_DISABLE_KBD;
        kbd_update_irq(s);
        break;
    case KBD_CCMD_KBD_ENABLE:
        s->mode &= ~KBD_MODE_DISABLE_KBD;
        kbd_update_irq(s);
        break;
    case KBD_CCMD_READ_INPORT:
        kbd_queue(s, 0x00, 0);
        break;
    case KBD_CCMD_READ_OUTPORT:
        
#ifdef TARGET_I386
        val = 0x01 | (ioport_get_a20() << 1);
#else
        val = 0x01;
#endif
        if (s->status & KBD_STAT_OBF)
            val |= 0x10;
        if (s->status & KBD_STAT_MOUSE_OBF)
            val |= 0x20;
        kbd_queue(s, val, 0);
        break;
#ifdef TARGET_I386
    case KBD_CCMD_ENABLE_A20:
        ioport_set_a20(1);
        break;
    case KBD_CCMD_DISABLE_A20:
        ioport_set_a20(0);
        break;
#endif
    case KBD_CCMD_RESET:
        qemu_system_reset_request();
        break;
    case 0xff:
        
        break;
    default:
        fprintf(stderr, "qemu: unsupported keyboard cmd=0x%02x\n", val);
        break;
    }
}

static uint32_t kbd_read_data(void *opaque, uint32_t addr)
{
    KBDState *s = opaque;
    uint32_t val;

    if (s->pending == KBD_PENDING_AUX)
        val = ps2_read_data(s->mouse);
    else
        val = ps2_read_data(s->kbd);

#if defined(DEBUG_KBD)
    printf("kbd: read data=0x%02x\n", val);
#endif
    return val;
}

static void kbd_write_data(void *opaque, uint32_t addr, uint32_t val)
{
    KBDState *s = opaque;

#ifdef DEBUG_KBD
    printf("kbd: write data=0x%02x\n", val);
#endif

    switch(s->write_cmd) {
    case 0:
        ps2_write_keyboard(s->kbd, val);
        break;
    case KBD_CCMD_WRITE_MODE:
        s->mode = val;
        ps2_keyboard_set_translation(s->kbd, (s->mode & KBD_MODE_KCC) != 0);
        
        kbd_update_irq(s);
        break;
    case KBD_CCMD_WRITE_OBUF:
        kbd_queue(s, val, 0);
        break;
    case KBD_CCMD_WRITE_AUX_OBUF:
        kbd_queue(s, val, 1);
        break;
    case KBD_CCMD_WRITE_OUTPORT:
#ifdef TARGET_I386
        ioport_set_a20((val >> 1) & 1);
#endif
        if (!(val & 1)) {
            qemu_system_reset_request();
        }
        break;
    case KBD_CCMD_WRITE_MOUSE:
        ps2_write_mouse(s->mouse, val);
        break;
    default:
        break;
    }
    s->write_cmd = 0;
}

static void kbd_reset(void *opaque)
{
    KBDState *s = opaque;

    s->mode = KBD_MODE_KBD_INT | KBD_MODE_MOUSE_INT;
    s->status = KBD_STAT_CMD | KBD_STAT_UNLOCKED;
}

static void kbd_save(QEMUFile* f, void* opaque)
{
    KBDState *s = (KBDState*)opaque;

    qemu_put_8s(f, &s->write_cmd);
    qemu_put_8s(f, &s->status);
    qemu_put_8s(f, &s->mode);
    qemu_put_8s(f, &s->pending);
}

static int kbd_load(QEMUFile* f, void* opaque, int version_id)
{
    KBDState *s = (KBDState*)opaque;

    if (version_id != 3)
        return -EINVAL;
    qemu_get_8s(f, &s->write_cmd);
    qemu_get_8s(f, &s->status);
    qemu_get_8s(f, &s->mode);
    qemu_get_8s(f, &s->pending);
    return 0;
}

void i8042_init(qemu_irq kbd_irq, qemu_irq mouse_irq, uint32_t io_base)
{
    KBDState *s = &kbd_state;

    s->irq_kbd = kbd_irq;
    s->irq_mouse = mouse_irq;

    kbd_reset(s);
    register_savevm("pckbd", 0, 3, kbd_save, kbd_load, s);
    register_ioport_read(io_base, 1, 1, kbd_read_data, s);
    register_ioport_write(io_base, 1, 1, kbd_write_data, s);
    register_ioport_read(io_base + 4, 1, 1, kbd_read_status, s);
    register_ioport_write(io_base + 4, 1, 1, kbd_write_command, s);

    s->kbd = ps2_kbd_init(kbd_update_kbd_irq, s);
    s->mouse = ps2_mouse_init(kbd_update_aux_irq, s);
#ifndef CONFIG_ANDROID
#ifdef TARGET_I386
    vmmouse_init(s->mouse);
#endif
#endif
    qemu_register_reset(kbd_reset, 0, s);
}

static uint32_t kbd_mm_readb (void *opaque, target_phys_addr_t addr)
{
    KBDState *s = opaque;

    if (addr & s->mask)
        return kbd_read_status(s, 0) & 0xff;
    else
        return kbd_read_data(s, 0) & 0xff;
}

static void kbd_mm_writeb (void *opaque, target_phys_addr_t addr, uint32_t value)
{
    KBDState *s = opaque;

    if (addr & s->mask)
        kbd_write_command(s, 0, value & 0xff);
    else
        kbd_write_data(s, 0, value & 0xff);
}

static CPUReadMemoryFunc *kbd_mm_read[] = {
    &kbd_mm_readb,
    &kbd_mm_readb,
    &kbd_mm_readb,
};

static CPUWriteMemoryFunc *kbd_mm_write[] = {
    &kbd_mm_writeb,
    &kbd_mm_writeb,
    &kbd_mm_writeb,
};

void i8042_mm_init(qemu_irq kbd_irq, qemu_irq mouse_irq,
                   target_phys_addr_t base, ram_addr_t size,
                   target_phys_addr_t mask)
{
    KBDState *s = &kbd_state;
    int s_io_memory;

    s->irq_kbd = kbd_irq;
    s->irq_mouse = mouse_irq;
    s->mask = mask;

    kbd_reset(s);
    register_savevm("pckbd", 0, 3, kbd_save, kbd_load, s);
    s_io_memory = cpu_register_io_memory(kbd_mm_read, kbd_mm_write, s);
    cpu_register_physical_memory(base, size, s_io_memory);

    s->kbd = ps2_kbd_init(kbd_update_kbd_irq, s);
    s->mouse = ps2_mouse_init(kbd_update_aux_irq, s);
#ifndef CONFIG_ANDROID
#ifdef TARGET_I386
    vmmouse_init(s->mouse);
#endif
#endif
    qemu_register_reset(kbd_reset, 0, s);
}