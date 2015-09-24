/**
 * @file op_cpu_type.h
 * CPU type determination
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_CPU_TYPE_H
#define OP_CPU_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	CPU_NO_GOOD = -1, 
	CPU_PPRO, 
	CPU_PII, 
	CPU_PIII, 
	CPU_ATHLON, 
	CPU_TIMER_INT, 
	CPU_RTC, 
	CPU_P4,  
	CPU_IA64, 
	CPU_IA64_1, 
	CPU_IA64_2, 
	CPU_HAMMER, 
	CPU_P4_HT2, 
	CPU_AXP_EV4, 
	CPU_AXP_EV5, 
	CPU_AXP_PCA56, 
	CPU_AXP_EV6, 
	CPU_AXP_EV67, 
	CPU_P6_MOBILE, 
	CPU_ARM_XSCALE1, 
	CPU_ARM_XSCALE2, 
	CPU_PPC64_POWER4, 
	CPU_PPC64_POWER5, 
	CPU_PPC64_POWER5p, 
	CPU_PPC64_970, 
	CPU_MIPS_20K, 
	CPU_MIPS_24K, 
	CPU_MIPS_25K, 
	CPU_MIPS_34K, 
	CPU_MIPS_5K, 
	CPU_MIPS_R10000, 
	CPU_MIPS_R12000, 
	CPU_MIPS_RM7000, 
	CPU_MIPS_RM9000, 
	CPU_MIPS_SB1, 
	CPU_MIPS_VR5432, 
	CPU_MIPS_VR5500, 
	CPU_PPC_E500,	
	CPU_PPC_E500_2,	
	CPU_CORE, 
	CPU_PPC_7450, 
	CPU_CORE_2, 
	CPU_PPC64_POWER6, 
	CPU_PPC64_970MP, 
	CPU_PPC64_CELL, 
	CPU_FAMILY10, 
 	CPU_PPC64_PA6T, 
	CPU_ARM_MPCORE, 
	CPU_ARM_V6, 
	CPU_PPC64_POWER5pp,  
	CPU_PPC_E300, 
	CPU_AVR32, 
	CPU_ARM_V7, 
 	CPU_ARCH_PERFMON, 
	CPU_FAMILY11H, 
	CPU_PPC64_POWER7, 
	CPU_PPC64_IBM_COMPAT_V1, 
   	CPU_CORE_I7, 
   	CPU_ATOM, 
	CPU_MIPS_LOONGSON2, 
	CPU_NEHALEM, 
	CPU_ARM_V7_CA9, 
	CPU_MIPS_74K, 
	CPU_MIPS_1004K, 
	CPU_FAMILY12H, 
	CPU_FAMILY14H, 
	CPU_FAMILY15H, 
	CPU_WESTMERE, 
	MAX_CPU_TYPE
} op_cpu;

int op_cpu_variations(op_cpu cpu_type);

op_cpu op_cpu_base_type(op_cpu cpu_type);

op_cpu op_get_cpu_type(void);

op_cpu op_get_cpu_number(char const * cpu_string);

char const * op_get_cpu_type_str(op_cpu cpu_type);

char const * op_get_cpu_name(op_cpu cpu_type);

int op_get_nr_counters(op_cpu cpu_type);

typedef enum {
	OP_INTERFACE_NO_GOOD = -1,
	OP_INTERFACE_24,
	OP_INTERFACE_26
} op_interface;

op_interface op_get_interface(void);

#ifdef __cplusplus
}
#endif

#endif 
