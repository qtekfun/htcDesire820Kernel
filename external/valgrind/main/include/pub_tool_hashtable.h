

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2005-2012 Nicholas Nethercote
      njn@valgrind.org

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

#ifndef __PUB_TOOL_HASHTABLE_H
#define __PUB_TOOL_HASHTABLE_H



typedef
   struct _VgHashNode {
      struct _VgHashNode * next;
      UWord              key;
   }
   VgHashNode;

typedef struct _VgHashTable * VgHashTable;

extern VgHashTable VG_(HT_construct) ( HChar* name );

extern Int VG_(HT_count_nodes) ( VgHashTable table );

extern void VG_(HT_add_node) ( VgHashTable t, void* node );

extern void* VG_(HT_lookup) ( VgHashTable table, UWord key );

extern void* VG_(HT_remove) ( VgHashTable table, UWord key );

extern VgHashNode** VG_(HT_to_array) ( VgHashTable t,  UInt* n_elems );

extern void VG_(HT_ResetIter) ( VgHashTable table );

extern void* VG_(HT_Next) ( VgHashTable table );

extern void VG_(HT_destruct) ( VgHashTable t, void(*freenode_fn)(void*) );


#endif   

