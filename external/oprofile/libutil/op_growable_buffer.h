/**
 * @file op_growable_buffer.h
 * a growable buffer interface
 *
 * @remark Copyright 2007 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Philippe Elie
 */

#ifndef OP_GROWABLE_BUFFER_H
#define OP_GROWABLE_BUFFER_H

#include <stddef.h>

struct growable_buffer {
	void * p;
	size_t size;
	size_t max_size;
};

void init_buffer(struct growable_buffer * buffer);

void free_buffer(struct growable_buffer * buffer);

void add_data(struct growable_buffer * b, void const * data, size_t len);

#endif 
