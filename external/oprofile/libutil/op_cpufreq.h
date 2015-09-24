/**
 * @file op_cpufreq.h
 * get cpu frequency declaration
 *
 * @remark Copyright 2003 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_CPUFREQ_H
#define OP_CPUFREQ_H

#if defined(__cplusplus)
extern "C" {
#endif

double op_cpu_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif 
