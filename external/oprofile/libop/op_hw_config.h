/**
 * @file op_hw_config.h
 * Configuration parameters that are dependent on CPU/architecture
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_HW_CONFIG_H
#define OP_HW_CONFIG_H

#ifdef __alpha__
#define OP_MAX_COUNTERS	20
#else
#define OP_MAX_COUNTERS	8
#endif

#define OP_MAX_PERF_COUNT	2147483647UL

#endif 
