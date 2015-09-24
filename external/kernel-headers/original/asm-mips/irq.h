/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 by Waldorf GMBH, written by Ralf Baechle
 * Copyright (C) 1995, 96, 97, 98, 99, 2000, 01, 02, 03 by Ralf Baechle
 */
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <linux/linkage.h>

#include <asm/mipsmtregs.h>

#include <irq.h>

#ifdef CONFIG_I8259
static inline int irq_canonicalize(int irq)
{
	return ((irq == I8259A_IRQ_BASE + 2) ? I8259A_IRQ_BASE + 9 : irq);
}
#else
#define irq_canonicalize(irq) (irq)	
#endif

#ifdef CONFIG_MIPS_MT_SMTC

struct irqaction;

extern unsigned long irq_hwmask[];
extern int setup_irq_smtc(unsigned int irq, struct irqaction * new,
                          unsigned long hwmask);

static inline void smtc_im_ack_irq(unsigned int irq)
{
	if (irq_hwmask[irq] & ST0_IM)
		set_c0_status(irq_hwmask[irq] & ST0_IM);
}

#else

static inline void smtc_im_ack_irq(unsigned int irq)
{
}

#endif 

#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF
#include <linux/cpumask.h>

extern void plat_set_irq_affinity(unsigned int irq, cpumask_t affinity);
extern void smtc_forward_irq(unsigned int irq);

#define IRQ_AFFINITY_HOOK(irq)						\
do {									\
    if (!cpu_isset(smp_processor_id(), irq_desc[irq].affinity)) {	\
	smtc_forward_irq(irq);						\
	irq_exit();							\
	return;								\
    }									\
} while (0)

#else 

#define IRQ_AFFINITY_HOOK(irq) do { } while (0)

#endif 

#ifdef CONFIG_MIPS_MT_SMTC_IM_BACKSTOP

#define __DO_IRQ_SMTC_HOOK(irq)						\
do {									\
	IRQ_AFFINITY_HOOK(irq);						\
	if (irq_hwmask[irq] & 0x0000ff00)				\
		write_c0_tccontext(read_c0_tccontext() &		\
				   ~(irq_hwmask[irq] & 0x0000ff00));	\
} while (0)

#define __NO_AFFINITY_IRQ_SMTC_HOOK(irq)				\
do {									\
	if (irq_hwmask[irq] & 0x0000ff00)                               \
		write_c0_tccontext(read_c0_tccontext() &		\
				   ~(irq_hwmask[irq] & 0x0000ff00));	\
} while (0)

#else

#define __DO_IRQ_SMTC_HOOK(irq)						\
do {									\
	IRQ_AFFINITY_HOOK(irq);						\
} while (0)
#define __NO_AFFINITY_IRQ_SMTC_HOOK(irq) do { } while (0)

#endif

#define do_IRQ(irq)							\
do {									\
	irq_enter();							\
	__DO_IRQ_SMTC_HOOK(irq);					\
	generic_handle_irq(irq);					\
	irq_exit();							\
} while (0)

#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF


#define do_IRQ_no_affinity(irq)						\
do {									\
	irq_enter();							\
	__NO_AFFINITY_IRQ_SMTC_HOOK(irq);				\
	generic_handle_irq(irq);					\
	irq_exit();							\
} while (0)

#endif 

extern void arch_init_irq(void);
extern void spurious_interrupt(void);

extern int allocate_irqno(void);
extern void alloc_legacy_irqno(void);
extern void free_irqno(unsigned int irq);

#define CP0_LEGACY_COMPARE_IRQ 7

extern int cp0_compare_irq;
extern int cp0_perfcount_irq;

#endif 
