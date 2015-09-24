/**
 * @file op_interface.h
 *
 * Module / user space interface for 2.4
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_INTERFACE_H
#define OP_INTERFACE_H

#include "op_config.h"
#include "op_types.h"

#define OP_FORK 1
#define OP_MAP 2
#define OP_EXEC 4
#define OP_DROP_MODULES 8
#define OP_EXIT 16

struct op_sample {
	unsigned long eip; 
	u32 counter; 
	u32 pid; 
	u32 tgid; 
};

enum oprof_state {
	STOPPED = 0,
	STOPPING = 1,
	RUNNING = 2
};
 
struct op_buffer_head {
	int cpu_nr; 
	size_t count; 
	enum oprof_state state; 
	struct op_sample buffer[0]; 
} __attribute__((__packed__));
	 
struct op_note {
	unsigned long   addr;
	unsigned long   len;
	unsigned long   offset;
	unsigned int    hash;
	unsigned int	pid;
	unsigned int    tgid;
	unsigned short	type;
};

struct op_hash_index {
	
	u32 name;
	
	u32 parent;
} __attribute__((__packed__));

#define OP_HASH_MAP_SIZE (OP_HASH_MAP_NR * sizeof(struct op_hash_index) + POOL_SIZE)

#endif 
