/* taskstats.h - exporting per-task statistics
 *
 * Copyright (C) Shailabh Nagar, IBM Corp. 2006
 *           (C) Balbir Singh,   IBM Corp. 2006
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef _LINUX_TASKSTATS_H
#define _LINUX_TASKSTATS_H


#define TASKSTATS_VERSION	1

struct taskstats {

	
	__u16	version;
	__u16	padding[3];	


	__u64	cpu_count;
	__u64	cpu_delay_total;

	

	__u64	blkio_count;
	__u64	blkio_delay_total;

	
	__u64	swapin_count;
	__u64	swapin_delay_total;

	__u64	cpu_run_real_total;

	__u64	cpu_run_virtual_total;
	
	
};



enum {
	TASKSTATS_CMD_UNSPEC = 0,	
	TASKSTATS_CMD_GET,		
	TASKSTATS_CMD_NEW,		
	__TASKSTATS_CMD_MAX,
};

#define TASKSTATS_CMD_MAX (__TASKSTATS_CMD_MAX - 1)

enum {
	TASKSTATS_TYPE_UNSPEC = 0,	
	TASKSTATS_TYPE_PID,		
	TASKSTATS_TYPE_TGID,		
	TASKSTATS_TYPE_STATS,		
	TASKSTATS_TYPE_AGGR_PID,	
	TASKSTATS_TYPE_AGGR_TGID,	
	__TASKSTATS_TYPE_MAX,
};

#define TASKSTATS_TYPE_MAX (__TASKSTATS_TYPE_MAX - 1)

enum {
	TASKSTATS_CMD_ATTR_UNSPEC = 0,
	TASKSTATS_CMD_ATTR_PID,
	TASKSTATS_CMD_ATTR_TGID,
	TASKSTATS_CMD_ATTR_REGISTER_CPUMASK,
	TASKSTATS_CMD_ATTR_DEREGISTER_CPUMASK,
	__TASKSTATS_CMD_ATTR_MAX,
};

#define TASKSTATS_CMD_ATTR_MAX (__TASKSTATS_CMD_ATTR_MAX - 1)


#define TASKSTATS_GENL_NAME	"TASKSTATS"
#define TASKSTATS_GENL_VERSION	0x1

#endif 
