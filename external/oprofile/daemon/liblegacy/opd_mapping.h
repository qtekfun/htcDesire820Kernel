/**
 * @file opd_mapping.h
 * Management of process mappings
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OPD_MAPPING_H
#define OPD_MAPPING_H

#include "op_list.h"

struct opd_image;
struct opd_proc;
struct op_note;

struct opd_map {
	
	struct list_head next;
	
	struct opd_image * image;
	
	unsigned long start;
	
	unsigned long offset;
	
	unsigned long end;
};

void opd_init_hash_map(void);

void opd_cleanup_hash_name(void);

void opd_handle_mapping(struct op_note const * note);

void opd_add_mapping(struct opd_proc * proc, struct opd_image * image,
		unsigned long start, unsigned long offset, unsigned long end);

void opd_kill_maps(struct opd_proc * proc);

inline static int opd_is_in_map(struct opd_map * map, unsigned long eip)
{
	return (eip >= map->start && eip < map->end);
}


inline static unsigned long opd_map_offset(struct opd_map * map,
					   unsigned long eip)
{
	return (eip - map->start) + map->offset;
}

#endif 
