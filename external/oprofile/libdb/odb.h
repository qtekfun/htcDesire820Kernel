/**
 * @file odb.h
 * This file contains various definitions and interface for management
 * of in-memory, through mmaped file, growable hash table, that stores
 * sample files.
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#ifndef ODB_HASH_H
#define ODB_HASH_H

#include <stddef.h>
#include <stdint.h>

#include "op_list.h"

typedef uint64_t odb_key_t;
typedef unsigned int odb_value_t;
typedef unsigned int odb_index_t;
typedef odb_index_t odb_node_nr_t;
typedef odb_index_t odb_hash_mask_t;

#define BUCKET_FACTOR 1

typedef struct {
	odb_key_t key;			
	odb_value_t value;		
	odb_index_t next;		
} odb_node_t;

typedef struct {
	odb_node_nr_t size;		
	odb_node_nr_t current_size;	
	int padding[6];			
} odb_descr_t;

typedef struct odb_data {
	odb_node_t * node_base;		
	odb_index_t * hash_base;	
	odb_descr_t * descr;		
	odb_hash_mask_t hash_mask;	
	unsigned int sizeof_header;	
	unsigned int offset_node;	
	void * base_memory;		
	int fd;				
	char * filename;                
	int ref_count;                  
	struct list_head list;          
} odb_data_t;

typedef struct {
	odb_data_t * data;
} odb_t;

#ifdef __cplusplus
extern "C" {
#endif


enum odb_rw {
	ODB_RDONLY = 0,	
	ODB_RDWR = 1	
};

void odb_init(odb_t * odb);

int odb_open(odb_t * odb, char const * filename,
             enum odb_rw rw, size_t sizeof_header);

void odb_close(odb_t * odb);

int odb_open_count(odb_t const * odb);

void * odb_get_data(odb_t * odb);

void odb_sync(odb_t const * odb);

int odb_grow_hashtable(odb_data_t * data);
static __inline void odb_commit_reservation(odb_data_t * data)
{
	++data->descr->current_size;
}

#define ODB_NODE_NR_INVALID ((odb_node_nr_t)-1)

int odb_check_hash(odb_t const * odb);

typedef struct odb_hash_stat_t odb_hash_stat_t;
odb_hash_stat_t * odb_hash_stat(odb_t const * odb);
void odb_hash_display_stat(odb_hash_stat_t const * stats);
void odb_hash_free_stat(odb_hash_stat_t * stats);

int odb_update_node(odb_t * odb, odb_key_t key);

int odb_update_node_with_offset(odb_t * odb, 
				odb_key_t key, 
				unsigned long int offset);

int odb_add_node(odb_t * odb, odb_key_t key, odb_value_t value);

odb_node_t * odb_get_iterator(odb_t const * odb, odb_node_nr_t * nr);

static __inline unsigned int
odb_do_hash(odb_data_t const * data, odb_key_t value)
{
	uint32_t temp = value & 0xffffffff;
	return ((temp << 0) ^ (temp >> 8)) & data->hash_mask;
}

#ifdef __cplusplus
}
#endif

#endif 
