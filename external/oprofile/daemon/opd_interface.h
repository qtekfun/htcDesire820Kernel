/**
 * @file opd_interface.h
 *
 * Module / user space interface for 2.6 kernels and above
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 * Modified by Aravind Menon for Xen
 * These modifications are:
 * Copyright (C) 2005 Hewlett-Packard Co.
 */

#ifndef OPD_INTERFACE_H
#define OPD_INTERFACE_H

#define CTX_SWITCH_CODE			1
#define CPU_SWITCH_CODE			2
#define COOKIE_SWITCH_CODE		3
#define KERNEL_ENTER_SWITCH_CODE	4
#define USER_ENTER_SWITCH_CODE		5
#define MODULE_LOADED_CODE		6
#define CTX_TGID_CODE			7
#define TRACE_BEGIN_CODE		8
#define XEN_ENTER_SWITCH_CODE		10
#if defined(__powerpc__)
#define SPU_PROFILING_CODE		11
#define SPU_CTX_SWITCH_CODE		12
#else
#define DOMAIN_SWITCH_CODE		11
#endif

#define IBS_FETCH_SAMPLE		13
#define IBS_OP_SAMPLE			14
#define LAST_CODE			15
 
#endif 
