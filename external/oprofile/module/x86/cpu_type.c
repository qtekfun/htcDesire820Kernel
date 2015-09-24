/**
 * @file cpu_type.c
 * CPU determination
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#include "oprofile.h"
#include "op_msr.h"

#include <linux/smp.h>

EXPORT_NO_SYMBOLS;

MODULE_PARM(force_rtc, "i");
MODULE_PARM_DESC(force_rtc, "force RTC mode.");
static int force_rtc;

#ifndef HT_SUPPORT
static int p4_threads(void)
{
	int processor_threads = 1;

#ifdef CONFIG_SMP
	if (test_bit(X86_FEATURE_HT, &current_cpu_data.x86_capability)) {
		
		int eax, ebx, ecx, edx;

		cpuid (1, &eax, &ebx, &ecx, &edx);
		processor_threads = (ebx >> 16) & 0xff;
	}
#endif 

	return processor_threads;
}

#ifdef CONFIG_SMP
static void do_cpu_id(void * data)
{
	int eax, ebx, ecx, edx;
	unsigned char * ptr = (unsigned char *) data;

	cpuid(1, &eax, &ebx, &ecx, &edx);

	
	ptr[smp_processor_id()] = (unsigned char) ((ebx & 0xff000000) >> 24);
}
#endif

static int p4_ht_enabled(void)
{
#ifndef CONFIG_SMP
	return 0;
#else
	int enabled, threads, i;
	unsigned char mask;
	unsigned char apic_id[smp_num_cpus];
	unsigned int cpu;

	
	threads = p4_threads();
	if (threads == 1) return 0;
	
	mask = 0xff;
	i = 1;
	while (i < threads) {
		i *= 2;
		mask <<= 1;
	}
	
	smp_call_function(do_cpu_id, apic_id, 1, 1);
	do_cpu_id(apic_id);
	
	enabled = 0;
	cpu = 0;
	do {
		if (apic_id[cpu] & ~mask) {
			enabled = 1;
			break;
		}
		cpu++;
	} while (cpu < smp_num_cpus);

	return enabled;
#endif 
}
#endif 


op_cpu p4_cpu_type(void)
{
	__u8 model = current_cpu_data.x86_model;
	if (model <= 4) {
#ifdef HT_SUPPORT
		if (smp_num_siblings == 1) {
			return CPU_P4;
		} else if (smp_num_siblings == 2) {
			return CPU_P4_HT2;
		} else {
			printk(KERN_INFO 
			       "oprofile: P4 HT unsupported number of siblings"
			       "processor, reverting to RTC\n");
			return CPU_RTC;
		}
#else
		
		if ((p4_threads() > 1) && p4_ht_enabled()) {
			printk(KERN_INFO 
			       "oprofile: P4 HT enabled, reverting to RTC\n");
			return CPU_RTC;
		}
		else
			return CPU_P4;
#endif
	} else
		
		return CPU_RTC;
}

 
__init op_cpu get_cpu_type(void)
{
	__u8 vendor = current_cpu_data.x86_vendor;
	__u8 family = current_cpu_data.x86;
	__u8 model = current_cpu_data.x86_model;
	__u16 val;

	if (force_rtc)
		return CPU_RTC;

	switch (vendor) {
		case X86_VENDOR_AMD:
			if (family == 6) {
				val = cpuid_edx(1);
				if (!(val & (1 << 9)))
					return CPU_RTC;
				return CPU_ATHLON;
			}
			if (family == 15)
				return CPU_HAMMER;
			return CPU_RTC;

		case X86_VENDOR_INTEL:
			switch (family) {
			default:
				return CPU_RTC;
			case 6:
				
				if (model == 14)
					return CPU_CORE;
				if (model > 0xd)
					return CPU_RTC;
				if (model > 5)
					return CPU_PIII;
				else if (model > 2)
					return CPU_PII;
				return CPU_PPRO;
			case 0xf:
				return p4_cpu_type();
			}
			
		default:
			return CPU_RTC;
	}
}
