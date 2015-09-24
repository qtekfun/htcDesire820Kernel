/**
 * @file op_sample_file.h
 * Sample file format
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_SAMPLE_FILE_H
#define OP_SAMPLE_FILE_H

#include "op_types.h"

#include <stdint.h>
#include <time.h>

struct opd_header {
	u8 magic[4];
	u32 version;
	u32 cpu_type;
	u32 ctr_event;
	u32 ctr_um;
	u32 ctr_count;
	
	u32 is_kernel;
	double cpu_speed;
	time_t mtime;
	u32 cg_to_is_kernel;
	
	u32 spu_profile;
	uint64_t embedded_offset;
	u64 anon_start;
	u64 cg_to_anon_start;
	
	u32 reserved1[1];
};

#endif 
