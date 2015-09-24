/*
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 2000 - 2001 by Kanoj Sarcar (kanoj@sgi.com)
 * Copyright (C) 2000 - 2001 by Silicon Graphics, Inc.
 * Copyright (C) 2000, 2001, 2002 Ralf Baechle
 * Copyright (C) 2000, 2001 Broadcom Corporation
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/bitops.h>
#include <linux/linkage.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

#include <asm/atomic.h>
#include <asm/smp-ops.h>

extern int smp_num_siblings;
extern cpumask_t cpu_sibling_map[];

#define raw_smp_processor_id() (current_thread_info()->cpu)

extern int __cpu_number_map[NR_CPUS];
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

#define NO_PROC_ID	(-1)

#define SMP_RESCHEDULE_YOURSELF	0x1	
#define SMP_CALL_FUNCTION	0x2

extern cpumask_t phys_cpu_present_map;
#define cpu_possible_map	phys_cpu_present_map

extern void asmlinkage smp_bootstrap(void);

static inline void smp_send_reschedule(int cpu)
{
	extern struct plat_smp_ops *mp_ops;	

	mp_ops->send_ipi_single(cpu, SMP_RESCHEDULE_YOURSELF);
}

extern asmlinkage void smp_call_function_interrupt(void);

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi(cpumask_t mask);

#endif 
