/**
 * @file jitdump.h
 * Header structure of a JIT-dump file.
 *
 * @remark Copyright 2007 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Jens Wilke
 * @Modifications Daniel Hansel
 *
 * Copyright IBM Corporation 2007
 *
 */

#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#include "op_types.h"

#ifndef JITDUMP_H
#define JITDUMP_H

#define JITHEADER_MAGIC 0x4F74496A

#define PADDING_8ALIGNED(x) ((((x) + 7) & 7) ^ 7)

#define JITHEADER_VERSION 1

struct jitheader {
	
	u32 magic;
	
	u32 version;
	u32 totalsize;
	u32 bfd_arch;
	u32 bfd_mach;
	u64 timestamp;
	char bfd_target[0];
};

enum jit_record_type {
	JIT_CODE_LOAD=0,
	JIT_CODE_UNLOAD=1,
	JIT_CODE_CLOSE=2,
	JIT_CODE_DEBUG_INFO=3
};

struct jr_prefix {
	u32 id;
	u32 total_size;
};

struct jr_code_load {
	u32 id;
	u32 total_size;
	u64 timestamp;
	u64 vma;
	u64 code_addr;
	u32 code_size;
	u32 align;
};

struct jr_code_unload {
	u32 id;
	u32 total_size;
	u64 timestamp;
	u64 vma;
};

struct jr_code_close {
	u32 id;
	u32 total_size;
	u64 timestamp;
};

struct jr_code_debug_info {
	u32 id;
	u32 total_size;
	u64 timestamp;
	u64 code_addr;
	u32 nr_entry;
	u32 align;
};

#endif 

