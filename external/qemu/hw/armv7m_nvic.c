/*
 * ARM Nested Vectored Interrupt Controller
 *
 * Copyright (c) 2006-2007 CodeSourcery.
 * Written by Paul Brook
 *
 * This code is licenced under the GPL.
 *
 * The ARMv7M System controller is fairly tightly tied in with the
 * NVIC.  Much of that is also implemented here.
 */

#include "sysbus.h"
#include "qemu-timer.h"
#include "arm-misc.h"

#define GIC_NIRQ 96
#define NCPU 1
#define NVIC 1

static inline int
gic_get_current_cpu(void)
{
    return 0;
}

static uint32_t nvic_readl(void *opaque, uint32_t offset);
static void nvic_writel(void *opaque, uint32_t offset, uint32_t value);

#include "arm_gic.c"

typedef struct {
    gic_state gic;
    struct {
        uint32_t control;
        uint32_t reload;
        int64_t tick;
        QEMUTimer *timer;
    } systick;
} nvic_state;

#define SYSTICK_SCALE 1000ULL

#define SYSTICK_ENABLE    (1 << 0)
#define SYSTICK_TICKINT   (1 << 1)
#define SYSTICK_CLKSOURCE (1 << 2)
#define SYSTICK_COUNTFLAG (1 << 16)

int system_clock_scale;

static inline int64_t systick_scale(nvic_state *s)
{
    if (s->systick.control & SYSTICK_CLKSOURCE)
        return system_clock_scale;
    else
        return 1000;
}

static void systick_reload(nvic_state *s, int reset)
{
    if (reset)
        s->systick.tick = qemu_get_clock_ns(vm_clock);
    s->systick.tick += (s->systick.reload + 1) * systick_scale(s);
    qemu_mod_timer(s->systick.timer, s->systick.tick);
}

static void systick_timer_tick(void * opaque)
{
    nvic_state *s = (nvic_state *)opaque;
    s->systick.control |= SYSTICK_COUNTFLAG;
    if (s->systick.control & SYSTICK_TICKINT) {
        
        armv7m_nvic_set_pending(s, ARMV7M_EXCP_SYSTICK);
    }
    if (s->systick.reload == 0) {
        s->systick.control &= ~SYSTICK_ENABLE;
    } else {
        systick_reload(s, 0);
    }
}

void armv7m_nvic_set_pending(void *opaque, int irq)
{
    nvic_state *s = (nvic_state *)opaque;
    if (irq >= 16)
        irq += 16;
    gic_set_pending_private(&s->gic, 0, irq);
}

int armv7m_nvic_acknowledge_irq(void *opaque)
{
    nvic_state *s = (nvic_state *)opaque;
    uint32_t irq;

    irq = gic_acknowledge_irq(&s->gic, 0);
    if (irq == 1023)
        hw_error("Interrupt but no vector\n");
    if (irq >= 32)
        irq -= 16;
    return irq;
}

void armv7m_nvic_complete_irq(void *opaque, int irq)
{
    nvic_state *s = (nvic_state *)opaque;
    if (irq >= 16)
        irq += 16;
    gic_complete_irq(&s->gic, 0, irq);
}

static uint32_t nvic_readl(void *opaque, uint32_t offset)
{
    nvic_state *s = (nvic_state *)opaque;
    uint32_t val;
    int irq;

    switch (offset) {
    case 4: 
        return (GIC_NIRQ / 32) - 1;
    case 0x10: 
        val = s->systick.control;
        s->systick.control &= ~SYSTICK_COUNTFLAG;
        return val;
    case 0x14: 
        return s->systick.reload;
    case 0x18: 
        {
            int64_t t;
            if ((s->systick.control & SYSTICK_ENABLE) == 0)
                return 0;
            t = qemu_get_clock(vm_clock);
            if (t >= s->systick.tick)
                return 0;
            val = ((s->systick.tick - (t + 1)) / systick_scale(s)) + 1;
            if (val > s->systick.reload)
                val = 0;
            return val;
        }
    case 0x1c: 
        return 10000;
    case 0xd00: 
        return cpu_single_env->cp15.c0_cpuid;
    case 0xd04: 
        
        val = s->gic.running_irq[0];
        if (val == 1023) {
            val = 0;
        } else if (val >= 32) {
            val -= 16;
        }
        
        if (s->gic.running_irq[0] == 1023
                || s->gic.last_active[s->gic.running_irq[0]][0] == 1023) {
            val |= (1 << 11);
        }
        
        if (s->gic.current_pending[0] != 1023)
            val |= (s->gic.current_pending[0] << 12);
        
        for (irq = 32; irq < GIC_NIRQ; irq++) {
            if (s->gic.irq_state[irq].pending) {
                val |= (1 << 22);
                break;
            }
        }
        
        if (s->gic.irq_state[ARMV7M_EXCP_SYSTICK].pending)
            val |= (1 << 26);
        
        if (s->gic.irq_state[ARMV7M_EXCP_PENDSV].pending)
            val |= (1 << 28);
        
        if (s->gic.irq_state[ARMV7M_EXCP_NMI].pending)
            val |= (1 << 31);
        return val;
    case 0xd08: 
        return cpu_single_env->v7m.vecbase;
    case 0xd0c: 
        return 0xfa05000;
    case 0xd10: 
        
        return 0;
    case 0xd14: 
        
        return 0;
    case 0xd18: case 0xd1c: case 0xd20: 
        irq = offset - 0xd14;
        val = 0;
        val = s->gic.priority1[irq++][0];
        val = s->gic.priority1[irq++][0] << 8;
        val = s->gic.priority1[irq++][0] << 16;
        val = s->gic.priority1[irq][0] << 24;
        return val;
    case 0xd24: 
        val = 0;
        if (s->gic.irq_state[ARMV7M_EXCP_MEM].active) val |= (1 << 0);
        if (s->gic.irq_state[ARMV7M_EXCP_BUS].active) val |= (1 << 1);
        if (s->gic.irq_state[ARMV7M_EXCP_USAGE].active) val |= (1 << 3);
        if (s->gic.irq_state[ARMV7M_EXCP_SVC].active) val |= (1 << 7);
        if (s->gic.irq_state[ARMV7M_EXCP_DEBUG].active) val |= (1 << 8);
        if (s->gic.irq_state[ARMV7M_EXCP_PENDSV].active) val |= (1 << 10);
        if (s->gic.irq_state[ARMV7M_EXCP_SYSTICK].active) val |= (1 << 11);
        if (s->gic.irq_state[ARMV7M_EXCP_USAGE].pending) val |= (1 << 12);
        if (s->gic.irq_state[ARMV7M_EXCP_MEM].pending) val |= (1 << 13);
        if (s->gic.irq_state[ARMV7M_EXCP_BUS].pending) val |= (1 << 14);
        if (s->gic.irq_state[ARMV7M_EXCP_SVC].pending) val |= (1 << 15);
        if (s->gic.irq_state[ARMV7M_EXCP_MEM].enabled) val |= (1 << 16);
        if (s->gic.irq_state[ARMV7M_EXCP_BUS].enabled) val |= (1 << 17);
        if (s->gic.irq_state[ARMV7M_EXCP_USAGE].enabled) val |= (1 << 18);
        return val;
    case 0xd28: 
        
        hw_error("Not implemented: Configurable Fault Status.");
        return 0;
    case 0xd2c: 
    case 0xd30: 
    case 0xd34: 
    case 0xd38: 
    case 0xd3c: 
        
        goto bad_reg;
    case 0xd40: 
        return 0x00000030;
    case 0xd44: 
        return 0x00000200;
    case 0xd48: 
        return 0x00100000;
    case 0xd4c: 
        return 0x00000000;
    case 0xd50: 
        return 0x00000030;
    case 0xd54: 
        return 0x00000000;
    case 0xd58: 
        return 0x00000000;
    case 0xd5c: 
        return 0x00000000;
    case 0xd60: 
        return 0x01141110;
    case 0xd64: 
        return 0x02111000;
    case 0xd68: 
        return 0x21112231;
    case 0xd6c: 
        return 0x01111110;
    case 0xd70: 
        return 0x01310102;
    
    default:
    bad_reg:
        hw_error("NVIC: Bad read offset 0x%x\n", offset);
    }
}

static void nvic_writel(void *opaque, uint32_t offset, uint32_t value)
{
    nvic_state *s = (nvic_state *)opaque;
    uint32_t oldval;
    switch (offset) {
    case 0x10: 
        oldval = s->systick.control;
        s->systick.control &= 0xfffffff8;
        s->systick.control |= value & 7;
        if ((oldval ^ value) & SYSTICK_ENABLE) {
            int64_t now = qemu_get_clock(vm_clock);
            if (value & SYSTICK_ENABLE) {
                if (s->systick.tick) {
                    s->systick.tick += now;
                    qemu_mod_timer(s->systick.timer, s->systick.tick);
                } else {
                    systick_reload(s, 1);
                }
            } else {
                qemu_del_timer(s->systick.timer);
                s->systick.tick -= now;
                if (s->systick.tick < 0)
                  s->systick.tick = 0;
            }
        } else if ((oldval ^ value) & SYSTICK_CLKSOURCE) {
            systick_reload(s, 1);
        }
        break;
    case 0x14: 
        s->systick.reload = value;
        break;
    case 0x18: 
        systick_reload(s, 1);
        s->systick.control &= ~SYSTICK_COUNTFLAG;
        break;
    case 0xd04: 
        if (value & (1 << 31)) {
            armv7m_nvic_set_pending(s, ARMV7M_EXCP_NMI);
        }
        if (value & (1 << 28)) {
            armv7m_nvic_set_pending(s, ARMV7M_EXCP_PENDSV);
        } else if (value & (1 << 27)) {
            s->gic.irq_state[ARMV7M_EXCP_PENDSV].pending = 0;
            gic_update(&s->gic);
        }
        if (value & (1 << 26)) {
            armv7m_nvic_set_pending(s, ARMV7M_EXCP_SYSTICK);
        } else if (value & (1 << 25)) {
            s->gic.irq_state[ARMV7M_EXCP_SYSTICK].pending = 0;
            gic_update(&s->gic);
        }
        break;
    case 0xd08: 
        cpu_single_env->v7m.vecbase = value & 0xffffff80;
        break;
    case 0xd0c: 
        if ((value >> 16) == 0x05fa) {
            if (value & 2) {
                hw_error("VECTCLRACTIVE not implemented");
            }
            if (value & 5) {
                hw_error("System reset");
            }
        }
        break;
    case 0xd10: 
    case 0xd14: 
        
        goto bad_reg;
    case 0xd18: case 0xd1c: case 0xd20: 
        {
            int irq;
            irq = offset - 0xd14;
            s->gic.priority1[irq++][0] = value & 0xff;
            s->gic.priority1[irq++][0] = (value >> 8) & 0xff;
            s->gic.priority1[irq++][0] = (value >> 16) & 0xff;
            s->gic.priority1[irq][0] = (value >> 24) & 0xff;
            gic_update(&s->gic);
        }
        break;
    case 0xd24: 
        s->gic.irq_state[ARMV7M_EXCP_MEM].enabled = (value & (1 << 16)) != 0;
        s->gic.irq_state[ARMV7M_EXCP_BUS].enabled = (value & (1 << 17)) != 0;
        s->gic.irq_state[ARMV7M_EXCP_USAGE].enabled = (value & (1 << 18)) != 0;
        break;
    case 0xd28: 
    case 0xd2c: 
    case 0xd30: 
    case 0xd34: 
    case 0xd38: 
    case 0xd3c: 
        goto bad_reg;
    default:
    bad_reg:
        hw_error("NVIC: Bad write offset 0x%x\n", offset);
    }
}

static void nvic_save(QEMUFile *f, void *opaque)
{
    nvic_state *s = (nvic_state *)opaque;

    qemu_put_be32(f, s->systick.control);
    qemu_put_be32(f, s->systick.reload);
    qemu_put_be64(f, s->systick.tick);
    qemu_put_timer(f, s->systick.timer);
}

static int nvic_load(QEMUFile *f, void *opaque, int version_id)
{
    nvic_state *s = (nvic_state *)opaque;

    if (version_id != 1)
        return -EINVAL;

    s->systick.control = qemu_get_be32(f);
    s->systick.reload = qemu_get_be32(f);
    s->systick.tick = qemu_get_be64(f);
    qemu_get_timer(f, s->systick.timer);

    return 0;
}

static void armv7m_nvic_init(SysBusDevice *dev)
{
    nvic_state *s= FROM_SYSBUSGIC(nvic_state, dev);

    gic_init(&s->gic);
    cpu_register_physical_memory(0xe000e000, 0x1000, s->gic.iomemtype);
    s->systick.timer = qemu_new_timer_ns(vm_clock, systick_timer_tick, s);
    register_savevm("armv7m_nvic", -1, 1, nvic_save, nvic_load, s);
}

static void armv7m_nvic_register_devices(void)
{
    sysbus_register_dev("armv7m_nvic", sizeof(nvic_state), armv7m_nvic_init);
}

device_init(armv7m_nvic_register_devices)
