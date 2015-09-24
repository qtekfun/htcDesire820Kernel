/**
 * @file opagent.h
 * Interface to report symbol names and dynamically generated code to Oprofile
 *
 * @remark Copyright 2007 OProfile authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @author Jens Wilke
 * @Modifications Daniel Hansel
 *
 * Copyright IBM Corporation 2007
 *
 */

#ifndef _LIB_OPAGENT_H
#define _LIB_OPAGENT_H

#include <sys/types.h>

#if defined(__cplusplus)
extern "C" {
#endif
  
struct debug_line_info {
	unsigned long vma;
	unsigned int lineno;
	
	char const * filename;
};

typedef void * op_agent_t;

op_agent_t op_open_agent(void);

int op_close_agent(op_agent_t hdl);

int op_write_native_code(op_agent_t hdl, char const * symbol_name,
			 uint64_t vma, void const * code,
			 const unsigned int code_size);

int op_write_debug_line_info(op_agent_t hdl, void const * code,
			     size_t nr_entry,
			     struct debug_line_info const * compile_map);

int op_unload_native_code(op_agent_t hdl, uint64_t vma);

int op_major_version(void);

int op_minor_version(void);


#if defined(__cplusplus)
}
#endif

#endif
