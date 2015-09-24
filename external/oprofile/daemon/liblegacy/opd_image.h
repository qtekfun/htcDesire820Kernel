/**
 * @file opd_image.h
 * Management of binary images
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_IMAGE_H
#define OPD_IMAGE_H

#include "op_list.h"
#include "op_config_24.h"
#include "op_types.h"

#include <time.h>

struct opd_24_sfile;

struct opd_image {
	
	struct list_head hash_next;
	
	int ref_count;
	
	struct opd_24_sfile ** sfiles[NR_CPUS];
	
	char * name;
	char * app_name;
	
	pid_t tid;
	
	pid_t tgid;
	
	time_t mtime;
	
	int kernel;
	
	int ignored;
};

typedef void (*opd_image_cb)(struct opd_image *);

void opd_for_each_image(opd_image_cb imagecb);

void opd_init_images(void);

void opd_delete_image(struct opd_image * image);

struct opd_image * opd_get_kernel_image(char const * name,
     char const * app_name, pid_t tid, pid_t tgid);

struct opd_image * opd_get_image(char const * name, char const * app_name,
                                 int kernel, pid_t tid, pid_t tgid);

int opd_get_nr_images(void);

#endif 
