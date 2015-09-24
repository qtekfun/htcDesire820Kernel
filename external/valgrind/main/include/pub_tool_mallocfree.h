

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Julian Seward
      jseward@acm.org

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

#ifndef __PUB_TOOL_MALLOCFREE_H
#define __PUB_TOOL_MALLOCFREE_H

extern void* VG_(malloc)         ( HChar* cc, SizeT nbytes );
extern void  VG_(free)           ( void* p );
extern void* VG_(calloc)         ( HChar* cc, SizeT n, SizeT bytes_per_elem );
extern void* VG_(realloc)        ( HChar* cc, void* p, SizeT size );
extern Char* VG_(strdup)         ( HChar* cc, const Char* s );

extern SizeT VG_(malloc_usable_size)( void* p );

extern SizeT VG_(malloc_effective_client_redzone_size)(void);

__attribute__((noreturn))
extern void VG_(out_of_memory_NORETURN) ( HChar* who, SizeT szB );

#endif   


