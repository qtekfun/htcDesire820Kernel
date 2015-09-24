/**
 * @file op_apic.c
 *
 * APIC setup etc. routines
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 * @author Dave Jones
 * @author Graydon Hoare
 */

#include <linux/mm.h>
#include <linux/init.h>
#include <linux/config.h>
#include <asm/io.h>

#include "oprofile.h"
#include "op_msr.h"
#include "op_apic.h"

static struct gate_struct kernel_nmi;
static ulong lvtpc_masked;

static void mask_lvtpc(void * e)
{
	u32 v = apic_read(APIC_LVTPC);
	lvtpc_masked = v & APIC_LVT_MASKED;
	apic_write(APIC_LVTPC, v | APIC_LVT_MASKED);
}

static void unmask_lvtpc(void * e)
{
	if (!lvtpc_masked)
		apic_write(APIC_LVTPC, apic_read(APIC_LVTPC) & ~APIC_LVT_MASKED);
}


void install_nmi(void)
{
	struct _descr descr;

	
	

	smp_call_function(mask_lvtpc, NULL, 0, 1);
	mask_lvtpc(NULL);

	store_idt(descr);
	kernel_nmi = descr.base[NMI_VECTOR_NUM];
	SET_NMI_GATE;

	smp_call_function(unmask_lvtpc, NULL, 0, 1);
	unmask_lvtpc(NULL);
}

void restore_nmi(void)
{
	struct _descr descr;

	smp_call_function(mask_lvtpc, NULL, 0, 1);
	mask_lvtpc(NULL);

	store_idt(descr);
	descr.base[NMI_VECTOR_NUM] = kernel_nmi;

	smp_call_function(unmask_lvtpc, NULL, 0, 1);
	unmask_lvtpc(NULL);
}


static uint saved_lvtpc[NR_CPUS];

void __init lvtpc_apic_setup(void * dummy)
{
	uint val;

	
	
	val = apic_read(APIC_LVTPC);
	saved_lvtpc[op_cpu_id()] = val;
	
	val &= ~APIC_LVT_MASKED;
	
	val = SET_APIC_DELIVERY_MODE(val, APIC_MODE_NMI);
	apic_write(APIC_LVTPC, val);
}

void lvtpc_apic_restore(void * dummy)
{
	uint v = apic_read(APIC_LVTERR);
	apic_write(APIC_LVTERR, v | APIC_LVT_MASKED);
	apic_write(APIC_LVTPC, saved_lvtpc[op_cpu_id()]);
	apic_write(APIC_LVTERR, v);
}

static int __init enable_apic(void)
{
	uint msr_low, msr_high;
	uint val;

	rdmsr(MSR_IA32_APICBASE, msr_low, msr_high);
	if ((msr_low & (1 << 11)) == 0)
		wrmsr(MSR_IA32_APICBASE, msr_low | (1 << 11), msr_high);

	

	
	val = apic_read(APIC_LVR);
	if (!APIC_INTEGRATED(GET_APIC_VERSION(val)))
		goto not_local_apic;

	
	if (GET_APIC_MAXLVT(apic_read(APIC_LVR)) < 4)
		goto not_local_apic;

	
	val = apic_read(APIC_SPIV);
	if (!(val & APIC_SPIV_APIC_ENABLED))
		apic_write(APIC_SPIV, val | APIC_SPIV_APIC_ENABLED);

	return !!(val & APIC_SPIV_APIC_ENABLED);

not_local_apic:
	
	if ((msr_low & (1 << 11)) == 0)
		wrmsr(MSR_IA32_APICBASE, msr_low & ~(1 << 11), msr_high);

	printk(KERN_ERR "oprofile: no suitable local APIC. Falling back to RTC mode.\n");
	return -ENODEV;
}

static void __init do_apic_setup(void)
{
	uint val;

	local_irq_disable();

	val = APIC_LVT_LEVEL_TRIGGER;
	val = SET_APIC_DELIVERY_MODE(val, APIC_MODE_EXINT);
	apic_write(APIC_LVT0, val);

	
	val = SET_APIC_DELIVERY_MODE(0, APIC_MODE_NMI);
	apic_write(APIC_LVT1, val);

	
	
	apic_write(APIC_ESR, 0);

	
	
	val = apic_read(APIC_LVTERR);
	val |= APIC_LVT_MASKED;
	apic_write(APIC_LVTERR, val);

	
	
	apic_write(APIC_LVTT, APIC_SEND_PENDING | 0x31);

	
	val = APIC_TDR_DIV_1;
	apic_write(APIC_TDCR, val);

	local_irq_enable();
}

static int __init check_cpu_ok(void)
{
	if (sysctl.cpu_type != CPU_PPRO &&
		sysctl.cpu_type != CPU_PII &&
		sysctl.cpu_type != CPU_PIII &&
		sysctl.cpu_type != CPU_ATHLON &&
		sysctl.cpu_type != CPU_HAMMER &&
		sysctl.cpu_type != CPU_P4 &&
		sysctl.cpu_type != CPU_P4_HT2)
		return 0;

	return 1;
}

int __init apic_setup(void)
{
	u32 val;

	if (!check_cpu_ok())
		goto nodev;

	fixmap_setup();

	switch (enable_apic()) {
		case 0:
			do_apic_setup();
			val = apic_read(APIC_ESR);
			printk(KERN_INFO "oprofile: enabled local APIC. Err code %.08x\n", val);
			break;
		case 1:
			printk(KERN_INFO "oprofile: APIC was already enabled\n");
			break;
		default:
			goto nodev;
	}

	lvtpc_apic_setup(NULL);
	return 0;
nodev:
	printk(KERN_WARNING "Your CPU does not have a local APIC, e.g. "
	       "mobile P6. Falling back to RTC mode.\n");
	return -ENODEV;
}

void apic_restore(void)
{
	fixmap_restore();
}
