/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999, 2003 by Ralf Baechle
 */
#ifndef _ASM_A_OUT_H
#define _ASM_A_OUT_H

#ifdef __KERNEL__


#endif

struct exec
{
	unsigned long a_info;	
	unsigned a_text;	
	unsigned a_data;	
	unsigned a_bss;		
	unsigned a_syms;	
	unsigned a_entry;	
	unsigned a_trsize;	
	unsigned a_drsize;	
};

#define N_TRSIZE(a)	((a).a_trsize)
#define N_DRSIZE(a)	((a).a_drsize)
#define N_SYMSIZE(a)	((a).a_syms)

#endif 
