/**
 * @file opd_extended.h
 * OProfile Extended Feature
 *
 * @remark Copyright 2007-2009 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Suravee Suthikulpanit <suravee.suthikulpanit@amd.com>
 * Copyright (c) 2009 Advanced Micro Devices, Inc.
 */

#ifndef OPD_EXTENDED_H
#define OPD_EXTENDED_H

#include "opd_trans.h"
#include "odb.h"

#include <stdlib.h>
#include <stdint.h>


struct opd_ext_feature {
	
	const char* feature;
	
	struct opd_ext_handlers * handlers;
};

struct opd_ext_handlers {
	
	int (*ext_init)(char const *);
	
	int (*ext_deinit)();
	
	int (*ext_print_stats)();
	
	struct opd_ext_sfile_handlers * ext_sfile;
};

struct opd_ext_sfile_handlers {
	int (*create)(struct sfile *);
	int (*dup)(struct sfile *, struct sfile *);
	int (*close)(struct sfile *);
	int (*sync)(struct sfile *);
	odb_t * (*get)(struct transient const *, int);
	struct opd_event * (*find_counter_event)(unsigned long);
};

extern int opd_ext_initialize(char const * value);

extern int opd_ext_deinitialize();

extern void opd_ext_print_stats();

extern void opd_ext_sfile_create(struct sfile * sf);
extern void opd_ext_sfile_dup (struct sfile * to, struct sfile * from);
extern void opd_ext_sfile_close(struct sfile * sf);
extern void opd_ext_sfile_sync(struct sfile * sf);
extern odb_t * opd_ext_sfile_get(struct transient const * trans, int is_cg);

extern struct opd_event * opd_ext_find_counter_event(unsigned long counter);


#endif
