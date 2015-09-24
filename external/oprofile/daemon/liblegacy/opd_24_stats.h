/**
 * @file opd_24_stats.h
 * Management of daemon statistics
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_24_STATS_H
#define OPD_24_STATS_H

extern unsigned long opd_24_stats[];

enum {  OPD_KERNEL, 
	OPD_MODULE, 
	OPD_LOST_MODULE, 
	OPD_LOST_PROCESS, 
	OPD_PROCESS, 
	OPD_LOST_MAP_PROCESS, 
	OPD_LOST_SAMPLEFILE, 
	OPD_PROC_QUEUE_ACCESS, 
	OPD_PROC_QUEUE_DEPTH, 
	OPD_DUMP_COUNT, 
	OPD_MAP_ARRAY_ACCESS, 
	OPD_MAP_ARRAY_DEPTH, 
	OPD_IMAGE_HASH_ACCESS,  
	OPD_IMAGE_HASH_DEPTH,  
	OPD_SAMPLES, 
	OPD_NOTIFICATIONS, 
	OPD_MAX_STATS 
	};

void opd_print_24_stats(void);

#endif 
