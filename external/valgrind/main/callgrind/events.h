
/*
   This file is part of Callgrind, a Valgrind tool for call tracing.

   Copyright (C) 2002-2012, Josef Weidendorfer (Josef.Weidendorfer@gmx.de)

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/


#ifndef CLG_EVENTS
#define CLG_EVENTS

#include "pub_tool_basics.h"

#define CLG_(str) VGAPPEND(vgCallgrind_,str)


#define MAX_EVENTGROUP_COUNT 10

typedef struct _EventGroup EventGroup;
struct _EventGroup {
    Int size;
    Char* name[0];
};

EventGroup* CLG_(register_event_group) (int id, Char*);
EventGroup* CLG_(register_event_group2)(int id, Char*, Char*);
EventGroup* CLG_(register_event_group3)(int id, Char*, Char*, Char*);
EventGroup* CLG_(register_event_group4)(int id, Char*, Char*, Char*, Char*);
EventGroup* CLG_(get_event_group)(int id);


typedef struct _EventSet EventSet;
struct _EventSet {
    
    UInt mask;
    Int count;
    Int size;
    Int offset[MAX_EVENTGROUP_COUNT];
 };

EventSet* CLG_(get_event_set)(Int id);
EventSet* CLG_(get_event_set2)(Int id1, Int id2);
EventSet* CLG_(get_event_set3)(Int id1, Int id2, Int id3);
EventSet* CLG_(add_event_group)(EventSet*, Int id);
EventSet* CLG_(add_event_group2)(EventSet*, Int id1, Int id2);
EventSet* CLG_(add_event_set)(EventSet*, EventSet*);
/* Writes event names into buf. Returns number of characters written */
Int CLG_(sprint_eventset)(Char* buf, EventSet*);


ULong* CLG_(get_eventset_cost)(EventSet*);
void CLG_(init_cost)(EventSet*,ULong*);
void CLG_(init_cost_lz)(EventSet*,ULong**);
void CLG_(zero_cost)(EventSet*,ULong*);
Bool CLG_(is_zero_cost)(EventSet*,ULong*);
Bool CLG_(is_equal_cost)(EventSet*,ULong*,ULong*);
void CLG_(copy_cost)(EventSet*,ULong* dst, ULong* src);
void CLG_(copy_cost_lz)(EventSet*,ULong** pdst, ULong* src);
void CLG_(add_cost)(EventSet*,ULong* dst, ULong* src);
void CLG_(add_cost_lz)(EventSet*,ULong** pdst, ULong* src);
Bool CLG_(add_and_zero_cost)(EventSet*,ULong* dst, ULong* src);
Bool CLG_(add_and_zero_cost2)(EventSet*,ULong* dst,EventSet*,ULong* src);
Bool CLG_(add_diff_cost)(EventSet*,ULong* dst, ULong* old, ULong* new_cost);
Bool CLG_(add_diff_cost_lz)(EventSet*,ULong** pdst, ULong* old, ULong* new_cost);
/* Returns number of characters written */
Int CLG_(sprint_cost)(Char* buf, EventSet*, ULong*);

struct EventMappingEntry {
    Int group;
    Int index;
    Int offset;
};
typedef struct _EventMapping EventMapping;
struct _EventMapping {
  EventSet* es;
  Int size;
  Int capacity;
  struct EventMappingEntry entry[0];
};

EventMapping* CLG_(get_eventmapping)(EventSet*);
void CLG_(append_event)(EventMapping*, Char*);
/* Returns number of characters written */
Int CLG_(sprint_eventmapping)(Char* buf, EventMapping*);
/* Returns number of characters written */
Int CLG_(sprint_mappingcost)(Char* buf, EventMapping*, ULong*);

#endif 