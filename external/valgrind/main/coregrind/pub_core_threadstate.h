

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

#ifndef __PUB_CORE_THREADSTATE_H
#define __PUB_CORE_THREADSTATE_H


#include "pub_tool_threadstate.h"


typedef
   enum ThreadStatus { 
      VgTs_Empty,      
      VgTs_Init,       
      VgTs_Runnable,   
      VgTs_WaitSys,    
      VgTs_Yielding,   
      VgTs_Zombie,     
   }
   ThreadStatus;

typedef
   enum { 
      VgSrc_None,	 
      VgSrc_ExitThread,  
      VgSrc_ExitProcess, 
      VgSrc_FatalSig	 
   }
   VgSchedReturnCode;


#if defined(VGA_x86)
   typedef VexGuestX86State   VexGuestArchState;
#elif defined(VGA_amd64)
   typedef VexGuestAMD64State VexGuestArchState;
#elif defined(VGA_ppc32)
   typedef VexGuestPPC32State VexGuestArchState;
#elif defined(VGA_ppc64)
   typedef VexGuestPPC64State VexGuestArchState;
#elif defined(VGA_arm)
   typedef VexGuestARMState   VexGuestArchState;
#elif defined(VGA_s390x)
   typedef VexGuestS390XState VexGuestArchState;
#elif defined(VGA_mips32)
   typedef VexGuestMIPS32State VexGuestArchState;
#else
#  error Unknown architecture
#endif

struct SyscallStatus;
struct SyscallArgs;

typedef 
   struct {
      


      
      VexGuestArchState vex __attribute__((aligned(16)));

      
      VexGuestArchState vex_shadow1 __attribute__((aligned(16)));
      VexGuestArchState vex_shadow2 __attribute__((aligned(16)));

      
      UChar vex_spill[LibVEX_N_SPILL_BYTES] __attribute__((aligned(16)));

      
   } 
   ThreadArchState;


typedef
   struct {
      
      Int lwpid;        
      Int threadgroup;  

      ThreadId parent;  

      
      Addr valgrind_stack_base;    
      Addr valgrind_stack_init_SP; 

      
      Word exitcode; 
      Int  fatalsig; 

#     if defined(VGO_darwin)
      
      void (*post_mach_trap_fn)(ThreadId tid,
                                struct SyscallArgs *, struct SyscallStatus *);
    
      
      Addr pthread;
    
      
      Addr func_arg;

      
      semaphore_t child_go;
      semaphore_t child_done;

      
      
      
      
      
      
      Bool wq_jmpbuf_valid;
      

      
      Addr remote_port;  
      Int msgh_id;       
      union {
         struct {
            Addr port;
         } mach_port;
         struct {
            Int right;
         } mach_port_allocate;
         struct {
            Addr port;
            Int right;
            Int delta;
         } mach_port_mod_refs;
         struct {
            Addr task;
            Addr name;
            Int disposition;
         } mach_port_insert_right;
         struct {
            Addr size;
            int flags;
         } vm_allocate;
         struct {
            Addr address;
            Addr size;
         } vm_deallocate;
         struct {
            Addr src;
            Addr dst;
            Addr size;
         } vm_copy;
         struct {
            Addr address;
            Addr size;
            int set_maximum;
            UWord new_protection;
         } vm_protect;
         struct {
            Addr addr;
            SizeT size;
         } vm_read;
         struct {
            ULong addr;
            ULong size;
         } mach_vm_read;
         struct {
            Addr addr;
            SizeT size;
            Addr data;
         } vm_read_overwrite;
         struct {
            Addr size;
            int copy;
            UWord protection;
         } vm_map;
         struct {
            Addr size;
         } vm_remap;
         struct {
            ULong size;
            int flags;
         } mach_vm_allocate;
         struct {
            ULong address;
            ULong size;
         } mach_vm_deallocate;
         struct {
            ULong address;
            ULong size;
            int set_maximum;
            unsigned int new_protection;
         } mach_vm_protect;
         struct {
            ULong size;
            int copy;
            UWord protection;
         } mach_vm_map;
         struct {
            Addr thread;
            UWord flavor;
         } thread_get_state;
         struct {
            Addr address;
         } io_connect_unmap_memory;
         struct {
            int which_port;
         } task_get_special_port;
         struct {
            char *service_name;
         } bootstrap_look_up;
         struct {
            vki_size_t size;
         } WindowServer_29828;
         struct {
            Int access_rights;
         } WindowServer_29831;
         struct {
            char *path;
         } io_registry_entry_from_path;
      } mach_args;
#     endif

   }
   ThreadOSstate;


typedef struct {
   ThreadId tid;

   
   ThreadStatus status;

   VgSchedReturnCode exitreason;

   
   ThreadArchState arch;

   vki_sigset_t sig_mask;

   vki_sigset_t tmp_sig_mask;

   struct SigQueue *sig_queue;


   
   SizeT client_stack_szB;

   Addr client_stack_highest_word;

   
   vki_stack_t altstack;

   
   ThreadOSstate os_state;

   UInt err_disablement_level;

   
   Bool               sched_jmpbuf_valid;
   VG_MINIMAL_JMP_BUF(sched_jmpbuf);
}
ThreadState;



extern ThreadState VG_(threads)[VG_N_THREADS];

extern ThreadId VG_(running_tid);



void VG_(init_Threads)(void);

const HChar* VG_(name_of_ThreadStatus) ( ThreadStatus status );

extern ThreadState *VG_(get_ThreadState) ( ThreadId tid );

extern Bool VG_(is_valid_tid) ( ThreadId tid );

extern Bool VG_(is_running_thread)(ThreadId tid);

extern Bool VG_(is_exiting)(ThreadId tid);

extern Int VG_(count_living_threads)(void);

extern Int VG_(count_runnable_threads)(void);

extern ThreadId VG_(lwpid_to_vgtid)(Int lwpid);

#endif   

