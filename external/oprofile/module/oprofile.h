/**
 * @file oprofile.h
 * Main driver code
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPROFILE_H
#define OPROFILE_H

#include <linux/version.h>
#include <linux/module.h>
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/sysctl.h>
#include <linux/smp_lock.h>

#include <asm/uaccess.h>

#include "compat.h"

#include "op_config_24.h"
#include "op_hw_config.h"
#include "op_interface.h"
#include "op_cpu_type.h"

#undef min
#undef max

#define streq(a, b) (!strcmp((a), (b)))

struct _oprof_data {
	
	struct op_sample * buffer;
	
	uint buf_size;
	
	uint buf_watermark;
	
	uint nextbuf;
	
	uint nr_irq;
	
	uint nr_buffer_overflow;
	
	uint ctr_count[OP_MAX_COUNTERS];
};

struct oprof_counter {
	int count;
	int enabled;
	int event;
	int kernel;
	int user;
	int unit_mask;
};

struct oprof_sysctl {
	
	int buf_size;
	
	int dump;
	
	int dump_stop;
	
	int note_size;
	
	int nr_interrupts;
	
	int cpu_type;
	
	int nr_note_buffer_overflow;
	
	int nr_buffer_overflow;
	
	struct oprof_counter ctr[OP_MAX_COUNTERS];
};

struct op_int_operations {
	int (*init)(void);
	
	void (*deinit)(void);
	
	int (*add_sysctls)(ctl_table * next);
	
	void (*remove_sysctls)(ctl_table * next);
	
	int (*check_params)(void);
	
	int (*setup)(void);
	
	void (*start)(void);
	
	void (*stop)(void);
	
	void (*start_cpu)(uint);
	
	void (*stop_cpu)(uint);
};

#define DNAME_STACK_MAX 1024

extern struct oprof_sysctl sysctl;

int oprof_init(void);
void oprof_exit(void);
unsigned long is_map_ready(void);
int oprof_hash_map_open(void);
int oprof_hash_map_release(void);
int oprof_hash_map_mmap(struct file * file, struct vm_area_struct * vma);
int oprof_map_open(void);
int oprof_map_release(void);
int oprof_init_hashmap(void);
void oprof_free_hashmap(void);

extern struct op_int_operations op_rtc_ops;

void op_do_profile(uint cpu, long eip, long irq_enabled, int ctr);
extern struct _oprof_data oprof_data[NR_CPUS];
extern struct oprof_sysctl sysctl_parms;
extern int lproc_dointvec(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp);

op_cpu get_cpu_type(void);
struct op_int_operations const * op_int_interface(void);
void op_intercept_syscalls(void);
void op_restore_syscalls(void);
void op_save_syscalls(void);

#endif 
