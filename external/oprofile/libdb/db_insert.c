/**
 * @file db_insert.c
 * Inserting a key-value pair into a DB
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "odb.h"


static inline int add_node(odb_data_t * data, odb_key_t key, odb_value_t value)
{
	odb_index_t new_node;
	odb_node_t * node;
	odb_index_t index;

	if (data->descr->current_size >= data->descr->size) {
		if (odb_grow_hashtable(data))
			return EINVAL;
	}
	new_node = data->descr->current_size;

	node = &data->node_base[new_node];
	node->value = value;
	node->key = key;

	index = odb_do_hash(data, key);
	node->next = data->hash_base[index];
	data->hash_base[index] = new_node;

	
	odb_commit_reservation(data);

	return 0;
}

int odb_update_node(odb_t * odb, odb_key_t key)
{
	return odb_update_node_with_offset(odb, key, 1);
}

int odb_update_node_with_offset(odb_t * odb, 
				odb_key_t key, 
				unsigned long int offset)
{
	odb_index_t index;
	odb_node_t * node;
	odb_data_t * data;

	data = odb->data;
	index = data->hash_base[odb_do_hash(data, key)];
	while (index) {
		node = &data->node_base[index];
		if (node->key == key) {
			if (node->value + offset != 0) {
				node->value += offset;
			} else {
				
			}
			return 0;
		}

		index = node->next;
	}

	return add_node(data, key, offset);
}


int odb_add_node(odb_t * odb, odb_key_t key, odb_value_t value)
{
	return add_node(odb->data, key, value);
}
