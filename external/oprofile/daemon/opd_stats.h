/**
 * @file daemon/opd_stats.h
 * Management of daemon statistics
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_STATS_H
#define OPD_STATS_H

extern unsigned long opd_stats[];

enum {	OPD_SAMPLES, 
	OPD_KERNEL, 
	OPD_PROCESS, 
	OPD_NO_CTX, 
	OPD_LOST_KERNEL,  
	OPD_LOST_SAMPLEFILE, 
	OPD_LOST_NO_MAPPING, 
	OPD_DUMP_COUNT, 
	OPD_DANGLING_CODE, 
	OPD_MAX_STATS 
};

void opd_print_stats(void);

#endif 
