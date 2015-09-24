/**
 * @file op_events.h
 * Details of PMC profiling events
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_EVENTS_H
#define OP_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "op_cpu_type.h"
#include "op_types.h"
#include "op_list.h"

enum unit_mask_type {
	utm_mandatory,		
	utm_exclusive,		
	utm_bitmask		
};

#define MAX_UNIT_MASK 32


struct op_unit_mask {
	char * name;		
	u32 num;		
	enum unit_mask_type unit_type_mask;
	u32 default_mask;	
	struct op_described_um {
		u32 value;
		char * desc;
	} um[MAX_UNIT_MASK];
	struct list_head um_next; 
	int used;                 
};


struct op_event {
	u32 counter_mask;	
	u32 val;		
	
	struct op_unit_mask * unit;			
	char * name;		
	char * desc;      	
	int min_count;		
	int filter;		
	char * ext;		
	struct list_head event_next;   
};

struct list_head * op_events(op_cpu cpu_type);

struct op_event * op_find_event(op_cpu cpu_type, u32 nr, u32 um);
struct op_event * op_find_event_any(op_cpu cpu_type, u32 nr);

struct op_event * find_event_by_name(char const * name, unsigned um,
                                     int um_valid);

char const * find_mapping_for_event(u32 val, op_cpu cpu_type);


enum op_event_check {
	OP_OK_EVENT = 0, 
	OP_INVALID_EVENT = 1, 
	OP_INVALID_UM = 2, 
	OP_INVALID_COUNTER = 4 
};

int op_check_events(int ctr, u32 event, u32 um, op_cpu cpu_type);

void op_free_events(void);

struct op_default_event_descr {
	char * name;
	unsigned long count;
	unsigned long um;
};

void op_default_event(op_cpu cpu_type, struct op_default_event_descr * descr);

#ifdef __cplusplus
}
#endif

#endif 
