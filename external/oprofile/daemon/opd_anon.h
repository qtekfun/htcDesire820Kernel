/**
 * @file opd_anon.h
 * Anonymous region handling.
 *
 * @remark Copyright 2005 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#ifndef OPD_ANON_H
#define OPD_ANON_H

#include "op_types.h"
#include "op_list.h"

#include "opd_cookie.h"

#include <sys/types.h>

struct transient;

#define VMA_SHIFT 13 

#define MAX_IMAGE_NAME_SIZE 20

struct anon_mapping {
	
	vma_t start;
	
	vma_t end;
	
	pid_t tgid;
	
	cookie_t app_cookie;
	
	struct list_head list;
	
	struct list_head lru_list;
	char name[MAX_IMAGE_NAME_SIZE+1];
};

struct anon_mapping * find_anon_mapping(struct transient *);

void anon_init(void);

#endif 
