/**
 * @file daemon/opd_sfile.h
 * Management of sample files
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_SFILE_H
#define OPD_SFILE_H

#include "opd_cookie.h"

#include "odb.h"
#include "op_hw_config.h"
#include "op_types.h"
#include "op_list.h"

#include <sys/types.h>

struct kernel_image;
struct transient;

#define CG_HASH_SIZE 16
#define UNUSED_EMBEDDED_OFFSET ~0LLU

struct sfile {
	
	unsigned long hashval;
	
	cookie_t cookie;
	
	cookie_t app_cookie;
	
	pid_t tid;
	
	pid_t tgid;
	
	unsigned int cpu;
	
	struct kernel_image * kernel;
	
	struct anon_mapping * anon;
	
	uint64_t embedded_offset;

	
	struct list_head hash;
	
	struct list_head lru;
	
	int ignored;
	
	odb_t files[OP_MAX_COUNTERS];
	
	odb_t * ext_files;
	
	struct list_head cg_hash[CG_HASH_SIZE];
};

struct cg_entry {
	
	struct sfile to;
	
	struct list_head hash;
};

void sfile_clear_kernel(void);

struct anon_mapping;

void sfile_clear_anon(struct anon_mapping *);

void sfile_sync_files(void);

void sfile_close_files(void);

int sfile_lru_clear(void);

void sfile_get(struct sfile * sf);

void sfile_put(struct sfile * sf);

struct sfile * sfile_find(struct transient const * trans);

void sfile_log_sample(struct transient const * trans);

void sfile_log_sample_count(struct transient const * trans,
                            unsigned long int count);

void sfile_init(void);

#endif 
