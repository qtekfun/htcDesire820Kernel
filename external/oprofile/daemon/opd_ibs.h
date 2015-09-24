/**
 * @file daemon/opd_ibs.h
 * AMD Family10h Instruction Based Sampling (IBS) handling.
 *
 * @remark Copyright 2008-2010 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Jason Yeh <jason.yeh@amd.com>
 * @author Paul Drongowski <paul.drongowski@amd.com>
 * @author Suravee Suthikulpanit <suravee.suthikulpanit@amd.com>
 * Copyright (c) 2008 Advanced Micro Devices, Inc.
 */

#ifndef OPD_IBS_H
#define OPD_IBS_H

#include <stdint.h>

#include "opd_ibs_macro.h"

struct transient;
struct opd_event;


struct ibs_sample {
	struct ibs_fetch_sample * fetch;
	struct ibs_op_sample * op;
};

struct ibs_fetch_sample {
	unsigned long int rip;
	
	unsigned int ibs_fetch_ctl_low;
	unsigned int ibs_fetch_ctl_high;
	
	unsigned int ibs_fetch_lin_addr_low;
	unsigned int ibs_fetch_lin_addr_high;
	
	unsigned int ibs_fetch_phys_addr_low;
	unsigned int ibs_fetch_phys_addr_high;
	unsigned int dummy_event;
};



struct ibs_op_sample {
	unsigned long int rip;
	
	unsigned int ibs_op_lin_addr_low;
	unsigned int ibs_op_lin_addr_high;
	
	unsigned int ibs_op_data1_low;
	unsigned int ibs_op_data1_high;
	
	unsigned int ibs_op_data2_low;
	unsigned int ibs_op_data2_high;
	
	unsigned int ibs_op_data3_low;
	unsigned int ibs_op_data3_high;
	
	unsigned int ibs_op_ldst_linaddr_low;
	unsigned int ibs_op_ldst_linaddr_high;
	
	unsigned int ibs_op_phys_addr_low;
	unsigned int ibs_op_phys_addr_high;
	
	unsigned long ibs_op_brtgt_addr;
};


extern void code_ibs_fetch_sample(struct transient * trans);

extern void code_ibs_op_sample(struct transient * trans);

extern void opd_log_ibs_event(unsigned int event, struct transient * trans);

extern void opd_log_ibs_count(unsigned int event, struct transient * trans, unsigned int count);


#endif 
