/**
 * @file op_parse_event.h
 * event parsing
 *
 * You can have silliness here.
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_PARSE_EVENT_H
#define OP_PARSE_EVENT_H

#include <stddef.h>

struct parsed_event {
	char * name;
	int count;
	int unit_mask;
	int kernel;
	int user;
	int unit_mask_valid;
};

size_t parse_events(struct parsed_event * parsed_events, size_t max_events,
                    char const * const * events);

#endif 
