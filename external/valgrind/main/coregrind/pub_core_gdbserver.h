

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2011-2012 Philippe Waroquiers

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

#ifndef __PUB_CORE_GDBSERVER_H
#define __PUB_CORE_GDBSERVER_H

#include "pub_tool_gdbserver.h"

HChar*  VG_(vgdb_prefix_default)(void);

void VG_(gdbserver_prerun_action) (ThreadId tid);

extern Bool VG_(gdbserver_activity) (ThreadId tid);


Bool VG_(gdbserver_point) (PointKind kind, Bool insert, 
                           Addr addr, int len);

extern void VG_(invoke_gdbserver) ( int check );

extern Bool VG_(gdbserver_report_signal) (Int signo, ThreadId tid);

extern IRSB* VG_(instrument_for_gdbserver_if_needed)
     (IRSB* sb_in,                   
      VexGuestLayout* layout,
      VexGuestExtents* vge,
      IRType gWordTy, IRType hWordTy);

typedef
   enum {
      orderly_finish,
      reset_after_error,
      reset_after_fork} FinishReason;

extern void VG_(gdbserver_status_output)(void);


typedef 
   struct {
      // nr of bytes vgdb has written to valgrind
      volatile int written_by_vgdb;
      
      volatile int seen_by_valgrind;
      
      
      Addr32 invoke_gdbserver;

      
      
      Addr32 threads;
      int sizeof_ThreadState;
      int offset_status;
      int offset_lwpid;

      
      
      int vgdb_pid;
   } VgdbShared32;

typedef 
   struct {
      volatile int written_by_vgdb;
      volatile int seen_by_valgrind;
      
      Addr64 invoke_gdbserver;

      Addr64 threads;
      int sizeof_ThreadState;
      int offset_status;
      int offset_lwpid;

      int vgdb_pid;
   } VgdbShared64;


#if VEX_HOST_WORDSIZE == 8
typedef VgdbShared64 VgdbShared;
#elif VEX_HOST_WORDSIZE == 4
typedef VgdbShared32 VgdbShared;
#else
# error "unexpected wordsize"
#endif


#endif   
