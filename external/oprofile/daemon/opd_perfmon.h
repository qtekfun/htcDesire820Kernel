/**
 * @file opd_perfmon.h
 * perfmonctl() handling
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef OPD_PERFMON_H
#define OPD_PERFMON_H

#ifdef __ia64__

#include <stdlib.h>

void perfmon_init(void);
void perfmon_exit(void);
void perfmon_start(void);
void perfmon_stop(void);


typedef unsigned char pfm_uuid_t[16];	

typedef struct {
	pfm_uuid_t     ctx_smpl_buf_id;	 
	unsigned long  ctx_flags;	 
	unsigned short ctx_nextra_sets;	 
	unsigned short ctx_reserved1;	 
	int	       ctx_fd;		 
	void	       *ctx_smpl_vaddr;	 
	unsigned long  ctx_reserved2[11];
} pfarg_context_t;

typedef struct {
	unsigned int	reg_num;	   
	unsigned short	reg_set;	   
	unsigned short	reg_reserved1;	   

	unsigned long	reg_value;	   
	unsigned long	reg_flags;	   

	unsigned long	reg_long_reset;	   
	unsigned long	reg_short_reset;   

	unsigned long	reg_reset_pmds[4]; 
	unsigned long	reg_random_seed;   
	unsigned long	reg_random_mask;   
	unsigned long   reg_last_reset_val;

	unsigned long	reg_smpl_pmds[4];  
	unsigned long	reg_smpl_eventid;  

	unsigned long   reg_reserved2[3];   
} pfarg_reg_t;

typedef struct {
	pid_t		load_pid;	   
	unsigned short	load_set;	   
	unsigned short	load_reserved1;	   
	unsigned long	load_reserved2[3]; 
} pfarg_load_t;

#define PFM_WRITE_PMCS      0x01
#define PFM_WRITE_PMDS      0x02
#define PFM_STOP            0x04
#define PFM_START           0x05
#define PFM_CREATE_CONTEXT  0x08
#define PFM_LOAD_CONTEXT    0x10
#define PFM_FL_SYSTEM_WIDE  0x02

#else

void perfmon_init(void)
{
}


void perfmon_exit(void)
{
}


void perfmon_start(void)
{
}


void perfmon_stop(void)
{
}

#endif 

#endif 
