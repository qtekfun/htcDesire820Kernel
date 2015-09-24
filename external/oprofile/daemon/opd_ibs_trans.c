/**
 * @file daemon/opd_ibs_trans.c
 * AMD Instruction Based Sampling (IBS) translation.
 *
 * @remark Copyright 2008 - 2010 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Jason Yeh <jason.yeh@amd.com>
 * @author Paul Drongowski <paul.drongowski@amd.com>
 * @author Suravee Suthikulpanit <suravee.suthikulpanit@amd.com>
 * Copyright (c) 2008 Advanced Micro Devices, Inc.
 */

#include "opd_ibs.h"
#include "opd_ibs_macro.h"
#include "opd_ibs_trans.h"
#include "opd_trans.h"
#include "opd_printf.h"

#include <stdlib.h>
#include <stdio.h>

extern FILE * bta_log;
extern FILE * memaccess_log;

void trans_ibs_fetch (struct transient * trans, unsigned int selected_flag)
{
	struct ibs_fetch_sample * trans_fetch = ((struct ibs_sample*)(trans->ext))->fetch;

	if ((selected_flag) == 0)
		return;

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_ALL) {
		
		AGG_IBS_EVENT(DE_IBS_FETCH_ALL);
	}		

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_KILLED) {
		if (IBS_FETCH_KILLED(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_FETCH_KILLED);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_ATTEMPTED) {
		
		AGG_IBS_EVENT(DE_IBS_FETCH_ATTEMPTED);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_COMPLETED) {
		if (IBS_FETCH_FETCH_COMPLETION(trans_fetch))
			
			AGG_IBS_EVENT(DE_IBS_FETCH_COMPLETED);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_ABORTED) {
		if (!IBS_FETCH_FETCH_COMPLETION(trans_fetch))
			
			AGG_IBS_EVENT(DE_IBS_FETCH_ABORTED);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_L1_ITLB_HIT) {
		
		if (IBS_FETCH_L1_TLB_HIT(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_L1_ITLB_HIT);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_ITLB_L1M_L2H) {
		
		if (IBS_FETCH_ITLB_L1M_L2H(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_ITLB_L1M_L2H);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_ITLB_L1M_L2M) {
		
		if (IBS_FETCH_ITLB_L1M_L2M(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_ITLB_L1M_L2M);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_IC_MISS) {
		
		if (IBS_FETCH_INST_CACHE_MISS(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_IC_MISS);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_IC_HIT) {
		
		if (IBS_FETCH_INST_CACHE_HIT(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_IC_HIT);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_4K_PAGE) {
		if (IBS_FETCH_PHYS_ADDR_VALID(trans_fetch)
		    && IBS_FETCH_TLB_PAGE_SIZE_4K(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_FETCH_4K_PAGE);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_2M_PAGE) {
		if (IBS_FETCH_PHYS_ADDR_VALID(trans_fetch)
		    && IBS_FETCH_TLB_PAGE_SIZE_2M(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_FETCH_2M_PAGE);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_1G_PAGE) {
		if (IBS_FETCH_PHYS_ADDR_VALID(trans_fetch)
		    && IBS_FETCH_TLB_PAGE_SIZE_1G(trans_fetch))
			AGG_IBS_EVENT(DE_IBS_FETCH_1G_PAGE);
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_XX_PAGE) {
	}

	CHECK_FETCH_SELECTED_FLAG(DE_IBS_FETCH_LATENCY) {
		if (IBS_FETCH_FETCH_LATENCY(trans_fetch))
			AGG_IBS_COUNT(DE_IBS_FETCH_LATENCY,
				      IBS_FETCH_FETCH_LATENCY(trans_fetch));
	}
}


void trans_ibs_op (struct transient * trans, unsigned int selected_flag)
{
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	if ((selected_flag) == 0)
		return;

	CHECK_OP_SELECTED_FLAG(DE_IBS_OP_ALL) {
		
		AGG_IBS_EVENT(DE_IBS_OP_ALL);
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_OP_TAG_TO_RETIRE) {
		if (IBS_OP_TAG_TO_RETIRE_CYCLES(trans_op))
			AGG_IBS_COUNT(DE_IBS_OP_TAG_TO_RETIRE,
				IBS_OP_TAG_TO_RETIRE_CYCLES(trans_op));
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_OP_COMP_TO_RETIRE) {
		
		if (IBS_OP_COM_TO_RETIRE_CYCLES(trans_op))
			AGG_IBS_COUNT(DE_IBS_OP_COMP_TO_RETIRE,
				IBS_OP_COM_TO_RETIRE_CYCLES(trans_op));
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_BRANCH_RETIRED) {
		if (IBS_OP_BRANCH_RETIRED(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_BRANCH_RETIRED) ;
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_BRANCH_MISP) {
		if (IBS_OP_BRANCH_RETIRED(trans_op)
		    
		    
		    && IBS_OP_BRANCH_MISPREDICT(trans_op))
			AGG_IBS_EVENT(DE_IBS_BRANCH_MISP) ;
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_BRANCH_TAKEN) {
		if (IBS_OP_BRANCH_RETIRED(trans_op)
		    
		    && IBS_OP_BRANCH_TAKEN(trans_op))
			AGG_IBS_EVENT(DE_IBS_BRANCH_TAKEN);
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_BRANCH_MISP_TAKEN) {
		if (IBS_OP_BRANCH_RETIRED(trans_op)
		    
		    && IBS_OP_BRANCH_TAKEN(trans_op)
		    && IBS_OP_BRANCH_MISPREDICT(trans_op))
			AGG_IBS_EVENT(DE_IBS_BRANCH_MISP_TAKEN);
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_RETURN) {
		if (IBS_OP_BRANCH_RETIRED(trans_op)
		    
		    && IBS_OP_RETURN(trans_op))
			AGG_IBS_EVENT(DE_IBS_RETURN);
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_RETURN_MISP) {
		if (IBS_OP_BRANCH_RETIRED(trans_op)
		    
		    && IBS_OP_RETURN(trans_op)
		    && IBS_OP_BRANCH_MISPREDICT(trans_op))
			AGG_IBS_EVENT(DE_IBS_RETURN_MISP);
	}

	CHECK_OP_SELECTED_FLAG(DE_IBS_RESYNC) {
		
		if (IBS_OP_BRANCH_RESYNC(trans_op))
			AGG_IBS_EVENT(DE_IBS_RESYNC);
	}
}


void trans_ibs_op_ls (struct transient * trans, unsigned int selected_flag)
{
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	
	if (!IBS_OP_IBS_LD_OP(trans_op) && !IBS_OP_IBS_ST_OP(trans_op))
		return;


	if ((selected_flag) == 0)
		return;

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_ALL_OP) {
		
		AGG_IBS_EVENT(DE_IBS_LS_ALL_OP) ;
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_LOAD_OP) {
		if (IBS_OP_IBS_LD_OP(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_LOAD_OP) ;
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_STORE_OP) {
		if (IBS_OP_IBS_ST_OP(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_STORE_OP);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DTLB_L1H) {
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && !IBS_OP_IBS_DC_L1_TLB_MISS(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_DTLB_L1H);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DTLB_L1M_L2H) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_MISS(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_DTLB_L1M_L2H);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DTLB_L1M_L2M) {
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)
		    && IBS_OP_IBS_DC_L2_TLB_MISS(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_DTLB_L1M_L2M);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DC_MISS) {
		if (IBS_OP_IBS_DC_MISS(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_DC_MISS);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DC_HIT) {
		if (!IBS_OP_IBS_DC_MISS(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_DC_HIT);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_MISALIGNED) {
		if (IBS_OP_IBS_DC_MISS_ACC(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_MISALIGNED);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_BNK_CONF_LOAD) {
		if (IBS_OP_IBS_DC_LD_BNK_CON(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_BNK_CONF_LOAD);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_BNK_CONF_STORE) {
		if (IBS_OP_IBS_DC_ST_BNK_CON(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_BNK_CONF_STORE);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_STL_FORWARDED) {
		if (IBS_OP_IBS_LD_OP(trans_op)
		    
		    && IBS_OP_IBS_DC_ST_TO_LD_FWD(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_STL_FORWARDED) ;
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_STL_CANCELLED) {
		if (IBS_OP_IBS_LD_OP(trans_op))
		if (IBS_OP_IBS_DC_ST_TO_LD_CAN(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_STL_CANCELLED) ;
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_UC_MEM_ACCESS) {
		if (IBS_OP_IBS_DC_UC_MEM_ACC(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_UC_MEM_ACCESS);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_WC_MEM_ACCESS) {
		if (IBS_OP_IBS_DC_WC_MEM_ACC(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_WC_MEM_ACCESS);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_LOCKED_OP) {
		if (IBS_OP_IBS_LOCKED_OP(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_LOCKED_OP);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_MAB_HIT) {
		if (IBS_OP_IBS_DC_MAB_HIT(trans_op))
			AGG_IBS_EVENT(DE_IBS_LS_MAB_HIT);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L1_DTLB_4K) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && !IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)

		    && !IBS_OP_IBS_DC_L1_TLB_HIT_2MB(trans_op)
		    && !IBS_OP_IBS_DC_L1_TLB_HIT_1GB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L1_DTLB_4K) ;
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L1_DTLB_2M) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && !IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)

		    && IBS_OP_IBS_DC_L1_TLB_HIT_2MB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L1_DTLB_2M);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L1_DTLB_1G) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && !IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)

		    && !IBS_OP_IBS_DC_L1_TLB_HIT_2MB(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_HIT_1GB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L1_DTLB_1G);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L1_DTLB_RES) {
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L2_DTLB_4K) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_MISS(trans_op)

		    
		    && !IBS_OP_IBS_DC_L2_TLB_HIT_2MB(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_HIT_1GB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L2_DTLB_4K);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L2_DTLB_2M) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_MISS(trans_op)

		    
		    && IBS_OP_IBS_DC_L2_TLB_HIT_2MB(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_HIT_1GB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L2_DTLB_2M);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L2_DTLB_1G) {
		
		if (IBS_OP_IBS_DC_LIN_ADDR_VALID(trans_op)
		    && IBS_OP_IBS_DC_L1_TLB_MISS(trans_op)
		    && !IBS_OP_IBS_DC_L2_TLB_MISS(trans_op)

		    
		    && !IBS_OP_IBS_DC_L2_TLB_HIT_2MB(trans_op)
		    && IBS_OP_IBS_DC_L2_TLB_HIT_1GB(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_LS_L2_DTLB_1G);
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_L2_DTLB_RES2) {
	}

	CHECK_OP_LS_SELECTED_FLAG(DE_IBS_LS_DC_LOAD_LAT) {
		if (IBS_OP_IBS_LD_OP(trans_op)
		    
		    && IBS_OP_IBS_DC_MISS(trans_op))
			
			AGG_IBS_COUNT(DE_IBS_LS_DC_LOAD_LAT,
				      IBS_OP_DC_MISS_LATENCY(trans_op)) ;
	}
}

void trans_ibs_op_nb (struct transient * trans, unsigned int selected_flag)
{
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	
	if ((selected_flag) == 0)
		return;

	if (!IBS_OP_IBS_LD_OP(trans_op))
		return;

	if (!IBS_OP_IBS_DC_MISS(trans_op))
		return;

	if (IBS_OP_NB_IBS_REQ_SRC(trans_op) == 0)
		return;

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_NB_LOCAL) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_REMOTE) {
		if (IBS_OP_NB_IBS_REQ_DST_PROC(trans_op))
			
			AGG_IBS_EVENT(DE_IBS_NB_REMOTE) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL_L3) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_01(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_LOCAL_L3);
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL_CACHE) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_02(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_LOCAL_CACHE);
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_REMOTE_CACHE) {
		if (IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_02(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_REMOTE_CACHE) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL_DRAM) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_03(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_LOCAL_DRAM);
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_REMOTE_DRAM) {
		if (IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_03(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_REMOTE_DRAM) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL_OTHER) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_07(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_LOCAL_OTHER);
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_REMOTE_OTHER) {
		if (IBS_OP_NB_IBS_REQ_DST_PROC(trans_op)
		    &&  IBS_OP_NB_IBS_REQ_SRC_07(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_REMOTE_OTHER) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_CACHE_STATE_M) {
		if (IBS_OP_NB_IBS_REQ_SRC_02(trans_op)
		    && !IBS_OP_NB_IBS_CACHE_HIT_ST(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_CACHE_STATE_M) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_CACHE_STATE_O) {
		if (IBS_OP_NB_IBS_REQ_SRC_02(trans_op)
		    && IBS_OP_NB_IBS_CACHE_HIT_ST(trans_op))
			AGG_IBS_EVENT(DE_IBS_NB_CACHE_STATE_O) ;
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_LOCAL_LATENCY) {
		if (!IBS_OP_NB_IBS_REQ_DST_PROC(trans_op))
			
			AGG_IBS_COUNT(DE_IBS_NB_LOCAL_LATENCY,
				      IBS_OP_DC_MISS_LATENCY(trans_op));
	}

	CHECK_OP_NB_SELECTED_FLAG(DE_IBS_NB_REMOTE_LATENCY) {
		if (IBS_OP_NB_IBS_REQ_DST_PROC(trans_op))
			
			AGG_IBS_COUNT(DE_IBS_NB_REMOTE_LATENCY,
				      IBS_OP_DC_MISS_LATENCY(trans_op));
	}
}


int trans_ibs_op_rip_invalid (struct transient * trans)
{
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	if (IBS_OP_RIP_INVALID(trans_op))
		return 1;	

	return 0;
}


void trans_ibs_op_mask_reserved (unsigned int family, struct transient * trans)
{
	struct ibs_op_sample * trans_op    = ((struct ibs_sample*)(trans->ext))->op;

	switch (family) {
	case 0x10:
		
		trans_op->ibs_op_data1_high &= ~MASK_RIP_INVALID;
		break;
	case 0x12:
		
		trans_op->ibs_op_data2_low &= ~NB_MASK_REQ_DST_PROC;
		
		trans_op->ibs_op_data2_low &= ~NB_MASK_L3_STATE;
		break;
	case 0x14:
		
		trans_op->ibs_op_data2_low &= ~NB_MASK_REQ_DST_PROC;
		
		trans_op->ibs_op_data2_low &= ~NB_MASK_L3_STATE;
		
		trans_op->ibs_op_data3_low &= ~DC_MASK_L1_HIT_1G;
		
		trans_op->ibs_op_data3_low &= ~DC_MASK_LD_BANK_CONFLICT;
		
		trans_op->ibs_op_data3_low &= ~DC_MASK_ST_BANK_CONFLICT;
		
		trans_op->ibs_op_data3_low &= ~DC_MASK_ST_TO_LD_CANCEL;
		
		trans_op->ibs_op_data3_low &= ~DC_MASK_L2_HIT_1G;
		
		break;
	case 0x15:
	default:
		break;
	
	}
}


void trans_ibs_op_bta(struct transient * trans)
{
	static cookie_t old_cookie     = NO_COOKIE;
	static cookie_t old_app_cookie = NO_COOKIE;
	static char const * mod        = NULL;
	static char const * app        = NULL;
	const char vmlinux[10]         = "vmlinux";
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	if (!bta_log)
		return;

	if (!trans_op->ibs_op_brtgt_addr)
		return;

	if( old_app_cookie == INVALID_COOKIE 
	||  old_app_cookie == NO_COOKIE 
	||  old_app_cookie != trans->app_cookie) {
		app = find_cookie(trans->app_cookie);
		old_app_cookie = trans->cookie;
	}

	if (trans->in_kernel == 1) {
		mod = vmlinux;
		old_cookie = NO_COOKIE;
	} else {
		if( old_cookie == INVALID_COOKIE 
		||  old_cookie == NO_COOKIE 
		||  old_cookie != trans->cookie) {
			mod = find_cookie(trans->cookie);
			old_cookie = trans->cookie;
		}
	}

	fprintf(bta_log, "0x%016llx,0x%016llx,%02lu %08u,%08u,0x%08x,0x%08lx\n",
                        trans->app_cookie, trans->cookie, trans->cpu, trans->tgid, trans->tid, (unsigned int)trans->pc,
			trans_op->ibs_op_brtgt_addr);
}


void trans_ibs_op_ls_memaccess(struct transient * trans)
{
	static cookie_t old_cookie     = NO_COOKIE;
	static cookie_t old_app_cookie = NO_COOKIE;
	static char const * mod        = NULL;
	static char const * app        = NULL;
	const char vmlinux[10]         = "vmlinux";
	struct ibs_op_sample * trans_op = ((struct ibs_sample*)(trans->ext))->op;

	if (!memaccess_log)
		return;

	if( old_app_cookie == INVALID_COOKIE 
	||  old_app_cookie == NO_COOKIE 
	||  old_app_cookie != trans->app_cookie) {
		app = find_cookie(trans->app_cookie);
		old_app_cookie = trans->cookie;
	}

	if (trans->in_kernel == 1) {
		mod = vmlinux;
		old_cookie = NO_COOKIE;
	} else {
		if( old_cookie == INVALID_COOKIE 
		||  old_cookie == NO_COOKIE 
		||  old_cookie != trans->cookie) {
			mod = find_cookie(trans->cookie);
			old_cookie = trans->cookie;
		}
	}

	fprintf(memaccess_log, "0x%016llx,0x%016llx,%02lu,%08u,%08u,0x%08x,0x%08u:%08x,0x%08x:%08x,%s,%08u\n",
                        trans->app_cookie, 
trans->cookie, 
trans->cpu, 
trans->tgid, 
trans->tid, 
(unsigned int)trans->pc, 
			trans_op->ibs_op_phys_addr_high, trans_op->ibs_op_phys_addr_low, 
			trans_op->ibs_op_ldst_linaddr_high, trans_op->ibs_op_ldst_linaddr_low, 
			(IBS_OP_IBS_LD_OP(trans_op))? "LD": "ST", 
			(unsigned int) IBS_OP_DC_MISS_LATENCY(trans_op));
}
