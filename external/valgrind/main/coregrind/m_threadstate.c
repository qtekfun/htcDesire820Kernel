

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

#include "pub_core_basics.h"
#include "pub_core_vki.h"
#include "pub_core_libcsetjmp.h"    
#include "pub_core_threadstate.h"
#include "pub_core_libcassert.h"
#include "pub_tool_inner.h"
#if defined(ENABLE_INNER_CLIENT_REQUEST)
#include "helgrind/helgrind.h"
#endif


ThreadId VG_(running_tid) = VG_INVALID_THREADID;

ThreadState VG_(threads)[VG_N_THREADS] __attribute__((aligned(16)));


void VG_(init_Threads)(void)
{
   ThreadId tid;

   for (tid = 1; tid < VG_N_THREADS; tid++) {
      INNER_REQUEST(
         ANNOTATE_BENIGN_RACE_SIZED(&VG_(threads)[tid].status,
                                    sizeof(VG_(threads)[tid].status), ""));
      INNER_REQUEST(
         ANNOTATE_BENIGN_RACE_SIZED(&VG_(threads)[tid].os_state.exitcode,
                                    sizeof(VG_(threads)[tid].os_state.exitcode),
                                    ""));
   }
}

const HChar* VG_(name_of_ThreadStatus) ( ThreadStatus status )
{
   switch (status) {
   case VgTs_Empty:     return "VgTs_Empty";
   case VgTs_Init:      return "VgTs_Init";
   case VgTs_Runnable:  return "VgTs_Runnable";
   case VgTs_WaitSys:   return "VgTs_WaitSys";
   case VgTs_Yielding:  return "VgTs_Yielding";
   case VgTs_Zombie:    return "VgTs_Zombie";
   default:             return "VgTs_???";
  }
}

ThreadState *VG_(get_ThreadState)(ThreadId tid)
{
   vg_assert(tid >= 0 && tid < VG_N_THREADS);
   vg_assert(VG_(threads)[tid].tid == tid);
   return &VG_(threads)[tid];
}

Bool VG_(is_valid_tid) ( ThreadId tid )
{
   
   if (tid == 0) return False;
   if (tid >= VG_N_THREADS) return False;
   if (VG_(threads)[tid].status == VgTs_Empty) return False;
   return True;
}

ThreadId VG_(get_running_tid)(void)
{
   return VG_(running_tid);
}

Bool VG_(is_running_thread)(ThreadId tid)
{
   ThreadState *tst = VG_(get_ThreadState)(tid);

   return 
      VG_(running_tid) == tid	           &&	
      tst->status == VgTs_Runnable;		
}

inline Bool VG_(is_exiting)(ThreadId tid)
{
   vg_assert(VG_(is_valid_tid)(tid));
   return VG_(threads)[tid].exitreason != VgSrc_None;
}

Int VG_(count_living_threads)(void)
{
   Int count = 0;
   ThreadId tid;

   for(tid = 1; tid < VG_N_THREADS; tid++)
      if (VG_(threads)[tid].status != VgTs_Empty &&
	  VG_(threads)[tid].status != VgTs_Zombie)
	 count++;

   return count;
}

Int VG_(count_runnable_threads)(void)
{
   Int count = 0;
   ThreadId tid;

   for(tid = 1; tid < VG_N_THREADS; tid++)
      if (VG_(threads)[tid].status == VgTs_Runnable)
	 count++;

   return count;
}

ThreadId VG_(lwpid_to_vgtid)(Int lwp)
{
   ThreadId tid;
   
   for(tid = 1; tid < VG_N_THREADS; tid++)
      if (VG_(threads)[tid].status != VgTs_Empty 
          && VG_(threads)[tid].os_state.lwpid == lwp)
	 return tid;

   return VG_INVALID_THREADID;
}

