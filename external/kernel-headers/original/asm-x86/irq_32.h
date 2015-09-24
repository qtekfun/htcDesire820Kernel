#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H


#include <linux/sched.h>
#include "irq_vectors.h"
#include <asm/thread_info.h>

static __inline__ int irq_canonicalize(int irq)
{
	return ((irq == 2) ? 9 : irq);
}

#ifdef CONFIG_X86_LOCAL_APIC
# define ARCH_HAS_NMI_WATCHDOG		
#endif

#ifdef CONFIG_4KSTACKS
  extern void irq_ctx_init(int cpu);
  extern void irq_ctx_exit(int cpu);
# define __ARCH_HAS_DO_SOFTIRQ
#else
# define irq_ctx_init(cpu) do { } while (0)
# define irq_ctx_exit(cpu) do { } while (0)
#endif

#ifdef CONFIG_IRQBALANCE
extern int irqbalance_disable(char *str);
#endif

#ifdef CONFIG_HOTPLUG_CPU
extern void fixup_irqs(cpumask_t map);
#endif

unsigned int do_IRQ(struct pt_regs *regs);
void init_IRQ(void);
void __init native_init_IRQ(void);

extern DECLARE_BITMAP(used_vectors, NR_VECTORS);

#endif 