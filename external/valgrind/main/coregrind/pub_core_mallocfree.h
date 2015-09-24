

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

#ifndef __PUB_CORE_MALLOCFREE_H
#define __PUB_CORE_MALLOCFREE_H

#include "pub_tool_mallocfree.h"


typedef Int ArenaId;

#define VG_N_ARENAS        8

#define VG_AR_CORE         0
#define VG_AR_TOOL         1
#define VG_AR_DINFO        2
#define VG_AR_CLIENT       3
#define VG_AR_DEMANGLE     4
#define VG_AR_EXECTXT      5
#define VG_AR_ERRORS       6
#define VG_AR_TTAUX        7

#if   defined(VGP_x86_linux)   || \
      defined(VGP_arm_linux)
#  define VG_MIN_MALLOC_SZB        8
#elif defined(VGP_amd64_linux) || \
      defined(VGP_ppc32_linux) || \
      defined(VGP_ppc64_linux) || \
      defined(VGP_s390x_linux) || \
      defined(VGP_mips32_linux) || \
      defined(VGP_x86_darwin)  || \
      defined(VGP_amd64_darwin)
#  define VG_MIN_MALLOC_SZB       16
#else
#  error Unknown platform
#endif

struct vg_mallinfo {
   int arena;    
   int ordblks;  
   int smblks;   
   int hblks;    
   int hblkhd;   
   int usmblks;  
   int fsmblks;  
   int uordblks; 
   int fordblks; 
   int keepcost; 
};

extern void* VG_(arena_malloc)  ( ArenaId arena, HChar* cc, SizeT nbytes );
extern void  VG_(arena_free)    ( ArenaId arena, void* ptr );
extern void* VG_(arena_calloc)  ( ArenaId arena, HChar* cc,
                                  SizeT nmemb, SizeT bytes_per_memb );
extern void* VG_(arena_realloc) ( ArenaId arena, HChar* cc,
                                  void* ptr, SizeT size );
extern void* VG_(arena_memalign)( ArenaId aid, HChar* cc,
                                  SizeT req_alignB, SizeT req_pszB );
extern Char* VG_(arena_strdup)  ( ArenaId aid, HChar* cc, 
                                  const Char* s);

extern SizeT VG_(arena_malloc_usable_size) ( ArenaId aid, void* payload );

extern void  VG_(mallinfo) ( ThreadId tid, struct vg_mallinfo* mi );

extern void  VG_(sanity_check_malloc_all) ( void );

extern void  VG_(print_all_arena_stats) ( void );

extern void  VG_(print_arena_cc_analysis) ( void );

#endif   

