#ifndef _ASM_X86_SETUP_H
#define _ASM_X86_SETUP_H

#define COMMAND_LINE_SIZE 2048

#ifdef __KERNEL__

#ifdef __i386__

#include <linux/pfn.h>
#define MAXMEM_PFN	PFN_DOWN(MAXMEM)
#define MAX_NONPAE_PFN	(1 << 20)

#endif 

#define PARAM_SIZE 4096		

#define OLD_CL_MAGIC		0xA33F
#define OLD_CL_ADDRESS		0x020	
#define NEW_CL_POINTER		0x228	

#ifndef __ASSEMBLY__
#include <asm/bootparam.h>

#ifndef _SETUP

extern struct boot_params boot_params;

#ifdef __i386__
#define LOWMEMSIZE()	(0x9f000)

struct e820entry;

char * __init machine_specific_memory_setup(void);
char *memory_setup(void);

int __init copy_e820_map(struct e820entry * biosmap, int nr_map);
int __init sanitize_e820_map(struct e820entry * biosmap, char * pnr_map);
void __init add_memory_region(unsigned long long start,
			      unsigned long long size, int type);

extern unsigned long init_pg_tables_end;

#ifndef CONFIG_PARAVIRT
#define paravirt_post_allocator_init()	do {} while (0)
#endif

#endif 
#endif 
#endif 
#endif  

#endif 