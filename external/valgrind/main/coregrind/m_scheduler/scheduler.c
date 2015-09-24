

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
#include "pub_core_debuglog.h"
#include "pub_core_vki.h"
#include "pub_core_vkiscnums.h"    
#include "pub_core_libcsetjmp.h"   
#include "pub_core_threadstate.h"
#include "pub_core_aspacemgr.h"
#include "pub_core_clreq.h"         
#include "pub_core_dispatch.h"
#include "pub_core_errormgr.h"      
#include "pub_core_gdbserver.h"     
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcproc.h"
#include "pub_core_libcsignal.h"
#if defined(VGO_darwin)
#include "pub_core_mach.h"
#endif
#include "pub_core_machine.h"
#include "pub_core_mallocfree.h"
#include "pub_core_options.h"
#include "pub_core_replacemalloc.h"
#include "pub_core_signals.h"
#include "pub_core_stacks.h"
#include "pub_core_stacktrace.h"    
#include "pub_core_syscall.h"
#include "pub_core_syswrap.h"
#include "pub_core_tooliface.h"
#include "pub_core_translate.h"     
#include "pub_core_transtab.h"
#include "pub_core_debuginfo.h"     
#include "priv_sched-lock.h"
#include "pub_core_scheduler.h"     
#include "pub_core_redir.h"




#define SCHEDULING_QUANTUM   100000

Bool VG_(in_generated_code) = False;

static ULong bbs_done = 0;

static ULong vgdb_next_poll;

static void do_client_request ( ThreadId tid );
static void scheduler_sanity ( ThreadId tid );
static void mostly_clear_thread_record ( ThreadId tid );

static ULong n_scheduling_events_MINOR = 0;
static ULong n_scheduling_events_MAJOR = 0;

static ULong stats__n_xindirs = 0;
static ULong stats__n_xindir_misses = 0;

 UInt VG_(stats__n_xindirs_32) = 0;
 UInt VG_(stats__n_xindir_misses_32) = 0;

static UInt sanity_fast_count = 0;
static UInt sanity_slow_count = 0;

void VG_(print_scheduler_stats)(void)
{
   VG_(message)(Vg_DebugMsg,
      "scheduler: %'llu event checks.\n", bbs_done );
   VG_(message)(Vg_DebugMsg,
                "scheduler: %'llu indir transfers, %'llu misses (1 in %llu)\n",
                stats__n_xindirs, stats__n_xindir_misses,
                stats__n_xindirs / (stats__n_xindir_misses 
                                    ? stats__n_xindir_misses : 1));
   VG_(message)(Vg_DebugMsg,
      "scheduler: %'llu/%'llu major/minor sched events.\n",
      n_scheduling_events_MAJOR, n_scheduling_events_MINOR);
   VG_(message)(Vg_DebugMsg, 
                "   sanity: %d cheap, %d expensive checks.\n",
                sanity_fast_count, sanity_slow_count );
}

static struct sched_lock *the_BigLock;



static
void print_sched_event ( ThreadId tid, Char* what )
{
   VG_(message)(Vg_DebugMsg, "  SCHED[%d]: %s\n", tid, what );
}

#define SHOW_SBCOUNT_EVERY (20ULL * 1000 * 1000)
static ULong bbs_done_lastcheck = 0;

static
void maybe_show_sb_counts ( void )
{
   Long delta = bbs_done - bbs_done_lastcheck;
   vg_assert(delta >= 0);
   if (UNLIKELY(delta >= SHOW_SBCOUNT_EVERY)) {
      VG_(umsg)("%'lld superblocks executed\n", bbs_done);
      bbs_done_lastcheck = bbs_done;
   }
}

static
HChar* name_of_sched_event ( UInt event )
{
   switch (event) {
      case VEX_TRC_JMP_TINVAL:         return "TINVAL";
      case VEX_TRC_JMP_NOREDIR:        return "NOREDIR";
      case VEX_TRC_JMP_SIGTRAP:        return "SIGTRAP";
      case VEX_TRC_JMP_SIGSEGV:        return "SIGSEGV";
      case VEX_TRC_JMP_SIGBUS:         return "SIGBUS";
      case VEX_TRC_JMP_EMWARN:         return "EMWARN";
      case VEX_TRC_JMP_EMFAIL:         return "EMFAIL";
      case VEX_TRC_JMP_CLIENTREQ:      return "CLIENTREQ";
      case VEX_TRC_JMP_YIELD:          return "YIELD";
      case VEX_TRC_JMP_NODECODE:       return "NODECODE";
      case VEX_TRC_JMP_MAPFAIL:        return "MAPFAIL";
      case VEX_TRC_JMP_SYS_SYSCALL:    return "SYSCALL";
      case VEX_TRC_JMP_SYS_INT32:      return "INT32";
      case VEX_TRC_JMP_SYS_INT128:     return "INT128";
      case VEX_TRC_JMP_SYS_INT129:     return "INT129";
      case VEX_TRC_JMP_SYS_INT130:     return "INT130";
      case VEX_TRC_JMP_SYS_SYSENTER:   return "SYSENTER";
      case VEX_TRC_JMP_BORING:         return "VEX_BORING";

      case VG_TRC_BORING:              return "VG_BORING";
      case VG_TRC_INNER_FASTMISS:      return "FASTMISS";
      case VG_TRC_INNER_COUNTERZERO:   return "COUNTERZERO";
      case VG_TRC_FAULT_SIGNAL:        return "FAULTSIGNAL";
      case VG_TRC_INVARIANT_FAILED:    return "INVFAILED";
      case VG_TRC_CHAIN_ME_TO_SLOW_EP: return "CHAIN_ME_SLOW";
      case VG_TRC_CHAIN_ME_TO_FAST_EP: return "CHAIN_ME_FAST";
      default:                         return "??UNKNOWN??";
  }
}

ThreadId VG_(alloc_ThreadState) ( void )
{
   Int i;
   for (i = 1; i < VG_N_THREADS; i++) {
      if (VG_(threads)[i].status == VgTs_Empty) {
	 VG_(threads)[i].status = VgTs_Init;
	 VG_(threads)[i].exitreason = VgSrc_None;
         return i;
      }
   }
   VG_(printf)("vg_alloc_ThreadState: no free slots available\n");
   VG_(printf)("Increase VG_N_THREADS, rebuild and try again.\n");
   VG_(core_panic)("VG_N_THREADS is too low");
   
}

void VG_(acquire_BigLock)(ThreadId tid, HChar* who)
{
   ThreadState *tst;

#if 0
   if (VG_(clo_trace_sched)) {
      HChar buf[100];
      vg_assert(VG_(strlen)(who) <= 100-50);
      VG_(sprintf)(buf, "waiting for lock (%s)", who);
      print_sched_event(tid, buf);
   }
#endif

   VG_(acquire_BigLock_LL)(NULL);

   tst = VG_(get_ThreadState)(tid);

   vg_assert(tst->status != VgTs_Runnable);
   
   tst->status = VgTs_Runnable;

   if (VG_(running_tid) != VG_INVALID_THREADID)
      VG_(printf)("tid %d found %d running\n", tid, VG_(running_tid));
   vg_assert(VG_(running_tid) == VG_INVALID_THREADID);
   VG_(running_tid) = tid;

   { Addr gsp = VG_(get_SP)(tid);
     VG_(unknown_SP_update)(gsp, gsp, 0);
   }

   if (VG_(clo_trace_sched)) {
      HChar buf[150];
      vg_assert(VG_(strlen)(who) <= 150-50);
      VG_(sprintf)(buf, " acquired lock (%s)", who);
      print_sched_event(tid, buf);
   }
}

void VG_(release_BigLock)(ThreadId tid, ThreadStatus sleepstate, HChar* who)
{
   ThreadState *tst = VG_(get_ThreadState)(tid);

   vg_assert(tst->status == VgTs_Runnable);

   vg_assert(sleepstate == VgTs_WaitSys ||
	     sleepstate == VgTs_Yielding);

   tst->status = sleepstate;

   vg_assert(VG_(running_tid) == tid);
   VG_(running_tid) = VG_INVALID_THREADID;

   if (VG_(clo_trace_sched)) {
      Char buf[200];
      vg_assert(VG_(strlen)(who) <= 200-100);
      VG_(sprintf)(buf, "releasing lock (%s) -> %s",
                        who, VG_(name_of_ThreadStatus)(sleepstate));
      print_sched_event(tid, buf);
   }

   VG_(release_BigLock_LL)(NULL);
}

static void init_BigLock(void)
{
   vg_assert(!the_BigLock);
   the_BigLock = ML_(create_sched_lock)();
}

static void deinit_BigLock(void)
{
   ML_(destroy_sched_lock)(the_BigLock);
   the_BigLock = NULL;
}

void VG_(acquire_BigLock_LL) ( HChar* who )
{
   ML_(acquire_sched_lock)(the_BigLock);
}

void VG_(release_BigLock_LL) ( HChar* who )
{
   ML_(release_sched_lock)(the_BigLock);
}

Bool VG_(owns_BigLock_LL) ( ThreadId tid )
{
   return (ML_(get_sched_lock_owner)(the_BigLock)
           == VG_(threads)[tid].os_state.lwpid);
}


void VG_(exit_thread)(ThreadId tid)
{
   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(VG_(is_running_thread)(tid));
   vg_assert(VG_(is_exiting)(tid));

   mostly_clear_thread_record(tid);
   VG_(running_tid) = VG_INVALID_THREADID;

   
   vg_assert(VG_(threads)[tid].exitreason != VgSrc_None);

   if (VG_(clo_trace_sched))
      print_sched_event(tid, "release lock in VG_(exit_thread)");

   VG_(release_BigLock_LL)(NULL);
}

void VG_(get_thread_out_of_syscall)(ThreadId tid)
{
   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(!VG_(is_running_thread)(tid));

   if (VG_(threads)[tid].status == VgTs_WaitSys) {
      if (VG_(clo_trace_signals)) {
	 VG_(message)(Vg_DebugMsg, 
                      "get_thread_out_of_syscall zaps tid %d lwp %d\n",
		      tid, VG_(threads)[tid].os_state.lwpid);
      }
#     if defined(VGO_darwin)
      {
         
         
         
         extern kern_return_t thread_abort(mach_port_t);
         thread_abort(VG_(threads)[tid].os_state.lwpid);
      }
#     else
      {
         __attribute__((unused))
         Int r = VG_(tkill)(VG_(threads)[tid].os_state.lwpid, VG_SIGVGKILL);
         
      }
#     endif
   }
}

void VG_(vg_yield)(void)
{
   ThreadId tid = VG_(running_tid);

   vg_assert(tid != VG_INVALID_THREADID);
   vg_assert(VG_(threads)[tid].os_state.lwpid == VG_(gettid)());

   VG_(release_BigLock)(tid, VgTs_Yielding, "VG_(vg_yield)");

   VG_(do_syscall0)(__NR_sched_yield);

   VG_(acquire_BigLock)(tid, "VG_(vg_yield)");
}


static void block_signals(void)
{
   vki_sigset_t mask;

   VG_(sigfillset)(&mask);

   
   VG_(sigdelset)(&mask, VKI_SIGSEGV);
   VG_(sigdelset)(&mask, VKI_SIGBUS);
   VG_(sigdelset)(&mask, VKI_SIGFPE);
   VG_(sigdelset)(&mask, VKI_SIGILL);
   VG_(sigdelset)(&mask, VKI_SIGTRAP);

   
   VG_(sigdelset)(&mask, VKI_SIGSTOP);
   VG_(sigdelset)(&mask, VKI_SIGKILL);

   VG_(sigprocmask)(VKI_SIG_SETMASK, &mask, NULL);
}

static void os_state_clear(ThreadState *tst)
{
   tst->os_state.lwpid       = 0;
   tst->os_state.threadgroup = 0;
#  if defined(VGO_linux)
   
#  elif defined(VGO_darwin)
   tst->os_state.post_mach_trap_fn = NULL;
   tst->os_state.pthread           = 0;
   tst->os_state.func_arg          = 0;
   VG_(memset)(&tst->os_state.child_go, 0, sizeof(tst->os_state.child_go));
   VG_(memset)(&tst->os_state.child_done, 0, sizeof(tst->os_state.child_done));
   tst->os_state.wq_jmpbuf_valid   = False;
   tst->os_state.remote_port       = 0;
   tst->os_state.msgh_id           = 0;
   VG_(memset)(&tst->os_state.mach_args, 0, sizeof(tst->os_state.mach_args));
#  else
#    error "Unknown OS"
#  endif
}

static void os_state_init(ThreadState *tst)
{
   tst->os_state.valgrind_stack_base    = 0;
   tst->os_state.valgrind_stack_init_SP = 0;
   os_state_clear(tst);
}

static 
void mostly_clear_thread_record ( ThreadId tid )
{
   vki_sigset_t savedmask;

   vg_assert(tid >= 0 && tid < VG_N_THREADS);
   VG_(cleanup_thread)(&VG_(threads)[tid].arch);
   VG_(threads)[tid].tid = tid;

   VG_(threads)[tid].status               = VgTs_Zombie;

   VG_(sigemptyset)(&VG_(threads)[tid].sig_mask);
   VG_(sigemptyset)(&VG_(threads)[tid].tmp_sig_mask);

   os_state_clear(&VG_(threads)[tid]);

   
   VG_(threads)[tid].altstack.ss_sp = (void *)0xdeadbeef;
   VG_(threads)[tid].altstack.ss_size = 0;
   VG_(threads)[tid].altstack.ss_flags = VKI_SS_DISABLE;

   VG_(clear_out_queued_signals)(tid, &savedmask);

   VG_(threads)[tid].sched_jmpbuf_valid = False;
}

                                          
static void sched_fork_cleanup(ThreadId me)
{
   ThreadId tid;
   vg_assert(VG_(running_tid) == me);

#  if defined(VGO_darwin)
   
   VG_(mach_init)();
#  endif

   VG_(threads)[me].os_state.lwpid = VG_(gettid)();
   VG_(threads)[me].os_state.threadgroup = VG_(getpid)();

   
   for (tid = 1; tid < VG_N_THREADS; tid++) {
      if (tid != me) {
         mostly_clear_thread_record(tid);
	 VG_(threads)[tid].status = VgTs_Empty;
         VG_(clear_syscallInfo)(tid);
      }
   }

   
   deinit_BigLock();
   init_BigLock();
   VG_(acquire_BigLock_LL)(NULL);
}


ThreadId VG_(scheduler_init_phase1) ( void )
{
   Int i;
   ThreadId tid_main;

   VG_(debugLog)(1,"sched","sched_init_phase1\n");

   if (VG_(clo_fair_sched) != disable_fair_sched
       && !ML_(set_sched_lock_impl)(sched_lock_ticket)
       && VG_(clo_fair_sched) == enable_fair_sched)
   {
      VG_(printf)("Error: fair scheduling is not supported on this system.\n");
      VG_(exit)(1);
   }

   if (VG_(clo_verbosity) > 1) {
      VG_(message)(Vg_DebugMsg,
                   "Scheduler: using %s scheduler lock implementation.\n",
                   ML_(get_sched_lock_name)());
   }

   init_BigLock();

   for (i = 0 ; i < VG_N_THREADS; i++) {
      
      VG_(memset)( & VG_(threads)[i], 0, sizeof( VG_(threads)[i] ) );

      VG_(threads)[i].sig_queue = NULL;

      os_state_init(&VG_(threads)[i]);
      mostly_clear_thread_record(i);

      VG_(threads)[i].status                    = VgTs_Empty;
      VG_(threads)[i].client_stack_szB          = 0;
      VG_(threads)[i].client_stack_highest_word = (Addr)NULL;
      VG_(threads)[i].err_disablement_level     = 0;
   }

   tid_main = VG_(alloc_ThreadState)();

   vg_assert(tid_main == 1);

   return tid_main;
}


void VG_(scheduler_init_phase2) ( ThreadId tid_main,
                                  Addr     clstack_end, 
                                  SizeT    clstack_size )
{
   VG_(debugLog)(1,"sched","sched_init_phase2: tid_main=%d, "
                   "cls_end=0x%lx, cls_sz=%ld\n",
                   tid_main, clstack_end, clstack_size);

   vg_assert(VG_IS_PAGE_ALIGNED(clstack_end+1));
   vg_assert(VG_IS_PAGE_ALIGNED(clstack_size));

   VG_(threads)[tid_main].client_stack_highest_word 
      = clstack_end + 1 - sizeof(UWord);
   VG_(threads)[tid_main].client_stack_szB 
      = clstack_size;

   VG_(atfork)(NULL, NULL, sched_fork_cleanup);
}



#define SCHEDSETJMP(tid, jumped, stmt)					\
   do {									\
      ThreadState * volatile _qq_tst = VG_(get_ThreadState)(tid);	\
									\
      (jumped) = VG_MINIMAL_SETJMP(_qq_tst->sched_jmpbuf);              \
      if ((jumped) == ((UWord)0)) {                                     \
	 vg_assert(!_qq_tst->sched_jmpbuf_valid);			\
	 _qq_tst->sched_jmpbuf_valid = True;				\
	 stmt;								\
      }	else if (VG_(clo_trace_sched))					\
	 VG_(printf)("SCHEDSETJMP(line %d) tid %d, jumped=%ld\n",       \
                     __LINE__, tid, jumped);                            \
      vg_assert(_qq_tst->sched_jmpbuf_valid);				\
      _qq_tst->sched_jmpbuf_valid = False;				\
   } while(0)


static void do_pre_run_checks ( ThreadState* tst )
{
   Addr a_vex     = (Addr) & tst->arch.vex;
   Addr a_vexsh1  = (Addr) & tst->arch.vex_shadow1;
   Addr a_vexsh2  = (Addr) & tst->arch.vex_shadow2;
   Addr a_spill   = (Addr) & tst->arch.vex_spill;
   UInt sz_vex    = (UInt) sizeof tst->arch.vex;
   UInt sz_vexsh1 = (UInt) sizeof tst->arch.vex_shadow1;
   UInt sz_vexsh2 = (UInt) sizeof tst->arch.vex_shadow2;
   UInt sz_spill  = (UInt) sizeof tst->arch.vex_spill;

   if (0)
   VG_(printf)("gst %p %d, sh1 %p %d, "
               "sh2 %p %d, spill %p %d\n",
               (void*)a_vex, sz_vex,
               (void*)a_vexsh1, sz_vexsh1,
               (void*)a_vexsh2, sz_vexsh2,
               (void*)a_spill, sz_spill );

   vg_assert(VG_IS_16_ALIGNED(sz_vex));
   vg_assert(VG_IS_16_ALIGNED(sz_vexsh1));
   vg_assert(VG_IS_16_ALIGNED(sz_vexsh2));
   vg_assert(VG_IS_16_ALIGNED(sz_spill));

   vg_assert(VG_IS_16_ALIGNED(a_vex));
   vg_assert(VG_IS_16_ALIGNED(a_vexsh1));
   vg_assert(VG_IS_16_ALIGNED(a_vexsh2));
   vg_assert(VG_IS_16_ALIGNED(a_spill));

   vg_assert(sz_vex == sz_vexsh1);
   vg_assert(sz_vex == sz_vexsh2);
   vg_assert(a_vex + 1 * sz_vex == a_vexsh1);
   vg_assert(a_vex + 2 * sz_vex == a_vexsh2);
   vg_assert(sz_spill == LibVEX_N_SPILL_BYTES);
   vg_assert(a_vex + 3 * sz_vex == a_spill);

#  if defined(VGA_x86)
   vg_assert(
      (offsetof(VexGuestX86State,guest_XMM7)
       - offsetof(VexGuestX86State,guest_XMM0))
      == (8-1) * 16
   );
   vg_assert(VG_IS_16_ALIGNED(offsetof(VexGuestX86State,guest_XMM0)));
   vg_assert(VG_IS_8_ALIGNED(offsetof(VexGuestX86State,guest_FPREG)));
   vg_assert(8 == offsetof(VexGuestX86State,guest_EAX));
   vg_assert(VG_IS_4_ALIGNED(offsetof(VexGuestX86State,guest_EAX)));
   vg_assert(VG_IS_4_ALIGNED(offsetof(VexGuestX86State,guest_EIP)));
#  endif

#  if defined(VGA_amd64)
   vg_assert(
      (offsetof(VexGuestAMD64State,guest_YMM16)
       - offsetof(VexGuestAMD64State,guest_YMM0))
      == (17-1) * 32
   );
   vg_assert(VG_IS_16_ALIGNED(offsetof(VexGuestAMD64State,guest_YMM0)));
   vg_assert(VG_IS_8_ALIGNED(offsetof(VexGuestAMD64State,guest_FPREG)));
   vg_assert(16 == offsetof(VexGuestAMD64State,guest_RAX));
   vg_assert(VG_IS_8_ALIGNED(offsetof(VexGuestAMD64State,guest_RAX)));
   vg_assert(VG_IS_8_ALIGNED(offsetof(VexGuestAMD64State,guest_RIP)));
#  endif

#  if defined(VGA_ppc32) || defined(VGA_ppc64)
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex.guest_VSR0));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow1.guest_VSR0));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow2.guest_VSR0));
   
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex.guest_VSR1));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow1.guest_VSR1));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow2.guest_VSR1));
#  endif

#  if defined(VGA_arm)
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex.guest_D0));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow1.guest_D0));
   vg_assert(VG_IS_16_ALIGNED(& tst->arch.vex_shadow2.guest_D0));
   
   vg_assert(VG_IS_8_ALIGNED(& tst->arch.vex.guest_D1));
   vg_assert(VG_IS_8_ALIGNED(& tst->arch.vex_shadow1.guest_D1));
   vg_assert(VG_IS_8_ALIGNED(& tst->arch.vex_shadow2.guest_D1));
#  endif

#  if defined(VGA_s390x)
   
#  endif

#  if defined(VGA_mips32)
  
#  endif
}

#define NO_VGDB_POLL    0xffffffffffffffffULL
#define VGDB_POLL_ASAP  0x0ULL

void VG_(disable_vgdb_poll) (void )
{
   vgdb_next_poll = NO_VGDB_POLL;
}
void VG_(force_vgdb_poll) ( void )
{
   vgdb_next_poll = VGDB_POLL_ASAP;
}

static
void run_thread_for_a_while ( HWord* two_words,
                              Int*   dispatchCtrP,
                              ThreadId      tid,
                              HWord         alt_host_addr,
                              Bool          use_alt_host_addr )
{
   volatile HWord        jumped         = 0;
   volatile ThreadState* tst            = NULL; 
   volatile Int          done_this_time = 0;
   volatile HWord        host_code_addr = 0;

   
   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(VG_(is_running_thread)(tid));
   vg_assert(!VG_(is_exiting)(tid));
   vg_assert(*dispatchCtrP > 0);

   tst = VG_(get_ThreadState)(tid);
   do_pre_run_checks( (ThreadState*)tst );
   

   
   vg_assert(VG_(stats__n_xindirs_32) == 0);
   vg_assert(VG_(stats__n_xindir_misses_32) == 0);

   
   two_words[0] = two_words[1] = 0;

   
   if (use_alt_host_addr) {
      
      host_code_addr = alt_host_addr;
   } else {
      
      UInt cno = (UInt)VG_TT_FAST_HASH((Addr)tst->arch.vex.VG_INSTR_PTR);
      if (LIKELY(VG_(tt_fast)[cno].guest == (Addr)tst->arch.vex.VG_INSTR_PTR))
         host_code_addr = VG_(tt_fast)[cno].host;
      else {
         AddrH res   = 0;
         Bool  found = VG_(search_transtab)(&res, NULL, NULL,
                                            (Addr)tst->arch.vex.VG_INSTR_PTR,
                                            True
                                            );
         if (LIKELY(found)) {
            host_code_addr = res;
         } else {
            two_words[0] = VG_TRC_INNER_FASTMISS; 
            return;
         }
      }
   }
   
   vg_assert(host_code_addr != 0); 

   
   

   
   tst->arch.vex.host_EvC_COUNTER = *dispatchCtrP;
   tst->arch.vex.host_EvC_FAILADDR
      = (HWord)VG_(fnptr_to_fnentry)( &VG_(disp_cp_evcheck_fail) );

   if (0) {
      vki_sigset_t m;
      Int i, err = VG_(sigprocmask)(VKI_SIG_SETMASK, NULL, &m);
      vg_assert(err == 0);
      VG_(printf)("tid %d: entering code with unblocked signals: ", tid);
      for (i = 1; i <= _VKI_NSIG; i++)
         if (!VG_(sigismember)(&m, i))
            VG_(printf)("%d ", i);
      VG_(printf)("\n");
   }

   

   
   VG_TRACK( start_client_code, tid, bbs_done );

   vg_assert(VG_(in_generated_code) == False);
   VG_(in_generated_code) = True;

   SCHEDSETJMP(
      tid, 
      jumped, 
      VG_(disp_run_translations)( 
         two_words,
         (void*)&tst->arch.vex,
         host_code_addr
      )
   );

   vg_assert(VG_(in_generated_code) == True);
   VG_(in_generated_code) = False;

   if (jumped != (HWord)0) {
      vg_assert(two_words[0] == 0 && two_words[1] == 0); 
      two_words[0] = VG_TRC_FAULT_SIGNAL;
      two_words[1] = 0;
      block_signals();
   } 

   stats__n_xindirs += (ULong)VG_(stats__n_xindirs_32);
   VG_(stats__n_xindirs_32) = 0;
   stats__n_xindir_misses += (ULong)VG_(stats__n_xindir_misses_32);
   VG_(stats__n_xindir_misses_32) = 0;

   
   vg_assert((Int)tst->arch.vex.host_EvC_COUNTER >= -1);
   vg_assert(tst->arch.vex.host_EvC_FAILADDR
             == (HWord)VG_(fnptr_to_fnentry)( &VG_(disp_cp_evcheck_fail)) );

   done_this_time = *dispatchCtrP - ((Int)tst->arch.vex.host_EvC_COUNTER + 1);

   vg_assert(done_this_time >= 0);
   bbs_done += (ULong)done_this_time;

   *dispatchCtrP -= done_this_time;
   vg_assert(*dispatchCtrP >= 0);

   
   VG_TRACK( stop_client_code, tid, bbs_done );

   if (bbs_done >= vgdb_next_poll) {
      if (VG_(clo_vgdb_poll))
         vgdb_next_poll = bbs_done + (ULong)VG_(clo_vgdb_poll);
      else
         
         vgdb_next_poll = NO_VGDB_POLL;
      if (VG_(gdbserver_activity) (tid))
         VG_(gdbserver) (tid);
   }

   
   if (two_words[0] == VG_TRC_CHAIN_ME_TO_SLOW_EP
       || two_words[0] == VG_TRC_CHAIN_ME_TO_FAST_EP) {
      vg_assert(two_words[1] != 0); 
   } else {
      vg_assert(two_words[1] == 0); 
   }
}



static void handle_tt_miss ( ThreadId tid )
{
   Bool found;
   Addr ip = VG_(get_IP)(tid);

   found = VG_(search_transtab)( NULL, NULL, NULL,
                                 ip, True );
   if (UNLIKELY(!found)) {
      
      if (VG_(translate)( tid, ip, False, 0, 
                          bbs_done, True )) {
         found = VG_(search_transtab)( NULL, NULL, NULL,
                                       ip, True ); 
         vg_assert2(found, "handle_tt_miss: missing tt_fast entry");
      
      } else {
	 
	 
	 
	 
	 
      }
   }
}

static
void handle_chain_me ( ThreadId tid, void* place_to_chain, Bool toFastEP )
{
   Bool found          = False;
   Addr ip             = VG_(get_IP)(tid);
   UInt to_sNo         = (UInt)-1;
   UInt to_tteNo       = (UInt)-1;

   found = VG_(search_transtab)( NULL, &to_sNo, &to_tteNo,
                                 ip, False );
   if (!found) {
      
      if (VG_(translate)( tid, ip, False, 0, 
                          bbs_done, True )) {
         found = VG_(search_transtab)( NULL, &to_sNo, &to_tteNo,
                                       ip, False ); 
         vg_assert2(found, "handle_chain_me: missing tt_fast entry");
      } else {
	 
	 
	 
	 
	 
        return;
      }
   }
   vg_assert(found);
   vg_assert(to_sNo != -1);
   vg_assert(to_tteNo != -1);

   VG_(tt_tc_do_chaining)( place_to_chain,
                           to_sNo, to_tteNo, toFastEP );
}

static void handle_syscall(ThreadId tid, UInt trc)
{
   ThreadState * volatile tst = VG_(get_ThreadState)(tid);
   volatile UWord jumped; 


   if (VG_(clo_sanity_level >= 3))
      VG_(am_do_sync_check)("(BEFORE SYSCALL)",__FILE__,__LINE__);

   SCHEDSETJMP(tid, jumped, VG_(client_syscall)(tid, trc));

   if (VG_(clo_sanity_level >= 3))
      VG_(am_do_sync_check)("(AFTER SYSCALL)",__FILE__,__LINE__);

   if (!VG_(is_running_thread)(tid))
      VG_(printf)("tid %d not running; VG_(running_tid)=%d, tid %d status %d\n",
		  tid, VG_(running_tid), tid, tst->status);
   vg_assert(VG_(is_running_thread)(tid));
   
   if (jumped != (UWord)0) {
      block_signals();
      VG_(poll_signals)(tid);
   }
}

static
void handle_noredir_jump ( HWord* two_words,
                           Int*   dispatchCtrP,
                           ThreadId tid )
{
   
   two_words[0] = two_words[1] = 0;

   AddrH hcode = 0;
   Addr  ip    = VG_(get_IP)(tid);

   Bool  found = VG_(search_unredir_transtab)( &hcode, ip );
   if (!found) {
      
      if (VG_(translate)( tid, ip, False, 0, bbs_done,
                          False )) {

         found = VG_(search_unredir_transtab)( &hcode, ip );
         vg_assert2(found, "unredir translation missing after creation?!");
      } else {
	 
	 
	 
	 
	 
         two_words[0] = VG_TRC_BORING;
         return;
      }

   }

   vg_assert(found);
   vg_assert(hcode != 0);

   
   vg_assert(*dispatchCtrP > 0); 
   run_thread_for_a_while( two_words, dispatchCtrP, tid,
                           hcode, True );
}


VgSchedReturnCode VG_(scheduler) ( ThreadId tid )
{
   
   Int dispatch_ctr = 0;

   ThreadState *tst = VG_(get_ThreadState)(tid);
   static Bool vgdb_startup_action_done = False;

   if (VG_(clo_trace_sched))
      print_sched_event(tid, "entering VG_(scheduler)");      

   if (!vgdb_startup_action_done) {
      vg_assert(tid == 1); 
      vgdb_startup_action_done = True;
      if (VG_(clo_vgdb) != Vg_VgdbNo) {
         if (VG_(clo_vgdb_poll))
            VG_(force_vgdb_poll) ();
         else
            VG_(disable_vgdb_poll) ();

         vg_assert (VG_(dyn_vgdb_error) == VG_(clo_vgdb_error));

         VG_(gdbserver_prerun_action) (1);
      } else {
         VG_(disable_vgdb_poll) ();
      }
   }

   
   block_signals();
   
   vg_assert(VG_(is_running_thread)(tid));

   dispatch_ctr = SCHEDULING_QUANTUM;

   while (!VG_(is_exiting)(tid)) {

      vg_assert(dispatch_ctr >= 0);
      if (dispatch_ctr == 0) {




	 VG_(release_BigLock)(tid, VgTs_Yielding, 
                                   "VG_(scheduler):timeslice");
	 

	 VG_(acquire_BigLock)(tid, "VG_(scheduler):timeslice");
	 

	 
	 scheduler_sanity(tid);
	 VG_(sanity_check_general)(False);

	 VG_(poll_signals)(tid);

	 if (VG_(is_exiting)(tid))
	    break;		

	 
	 n_scheduling_events_MAJOR++;

         
         dispatch_ctr = SCHEDULING_QUANTUM;

	 
	 vg_assert(tst->tid == tid);
	 vg_assert(tst->os_state.lwpid == VG_(gettid)());
      }

      
      n_scheduling_events_MINOR++;

      if (0)
         VG_(message)(Vg_DebugMsg, "thread %d: running for %d bbs\n", 
                                   tid, dispatch_ctr - 1 );

      HWord trc[2]; 
      run_thread_for_a_while( &trc[0],
                              &dispatch_ctr,
                              tid, 0, False );

      if (VG_(clo_trace_sched) && VG_(clo_verbosity) > 2) {
	 HChar buf[50];
	 VG_(sprintf)(buf, "TRC: %s", name_of_sched_event(trc[0]));
	 print_sched_event(tid, buf);
      }

      if (trc[0] == VEX_TRC_JMP_NOREDIR) {
         handle_noredir_jump( &trc[0], 
                              &dispatch_ctr,
                              tid );
         vg_assert(trc[0] != VEX_TRC_JMP_NOREDIR);

         vg_assert(trc[0] != VG_TRC_INNER_COUNTERZERO);

         vg_assert(trc[0] != VG_TRC_CHAIN_ME_TO_SLOW_EP
                   && trc[0] != VG_TRC_CHAIN_ME_TO_FAST_EP);
      }

      switch (trc[0]) {
      case VEX_TRC_JMP_BORING:
      case VG_TRC_BORING:
         
         break;

      case VG_TRC_INNER_FASTMISS:
	 vg_assert(dispatch_ctr > 0);
	 handle_tt_miss(tid);
	 break;

      case VG_TRC_CHAIN_ME_TO_SLOW_EP: {
         if (0) VG_(printf)("sched: CHAIN_TO_SLOW_EP: %p\n", (void*)trc[1] );
         handle_chain_me(tid, (void*)trc[1], False);
         break;
      }

      case VG_TRC_CHAIN_ME_TO_FAST_EP: {
         if (0) VG_(printf)("sched: CHAIN_TO_FAST_EP: %p\n", (void*)trc[1] );
         handle_chain_me(tid, (void*)trc[1], True);
         break;
      }

      case VEX_TRC_JMP_CLIENTREQ:
	 do_client_request(tid);
	 break;

      case VEX_TRC_JMP_SYS_INT128:  
      case VEX_TRC_JMP_SYS_INT129:  
      case VEX_TRC_JMP_SYS_INT130:  
      case VEX_TRC_JMP_SYS_SYSCALL: 
	 handle_syscall(tid, trc[0]);
	 if (VG_(clo_sanity_level) > 2)
	    VG_(sanity_check_general)(True); 
	 break;

      case VEX_TRC_JMP_YIELD:
	 if (dispatch_ctr > 2000) 
            dispatch_ctr = 2000;
	 break;

      case VG_TRC_INNER_COUNTERZERO:
	 
	 vg_assert(dispatch_ctr == 0);
	 break;

      case VG_TRC_FAULT_SIGNAL:
	 break;

      case VEX_TRC_JMP_MAPFAIL:
         
         VG_(synth_fault)(tid);
         break;

      case VEX_TRC_JMP_EMWARN: {
         static Int  counts[EmWarn_NUMBER];
         static Bool counts_initted = False;
         VexEmWarn ew;
         HChar*    what;
         Bool      show;
         Int       q;
         if (!counts_initted) {
            counts_initted = True;
            for (q = 0; q < EmWarn_NUMBER; q++)
               counts[q] = 0;
         }
         ew   = (VexEmWarn)VG_(threads)[tid].arch.vex.guest_EMWARN;
         what = (ew < 0 || ew >= EmWarn_NUMBER)
                   ? "unknown (?!)"
                   : LibVEX_EmWarn_string(ew);
         show = (ew < 0 || ew >= EmWarn_NUMBER)
                   ? True
                   : counts[ew]++ < 3;
         if (show && VG_(clo_show_emwarns) && !VG_(clo_xml)) {
            VG_(message)( Vg_UserMsg,
                          "Emulation warning: unsupported action:\n");
            VG_(message)( Vg_UserMsg, "  %s\n", what);
            VG_(get_and_pp_StackTrace)( tid, VG_(clo_backtrace_size) );
         }
         break;
      }

      case VEX_TRC_JMP_EMFAIL: {
         VexEmWarn ew;
         HChar*    what;
         ew   = (VexEmWarn)VG_(threads)[tid].arch.vex.guest_EMWARN;
         what = (ew < 0 || ew >= EmWarn_NUMBER)
                   ? "unknown (?!)"
                   : LibVEX_EmWarn_string(ew);
         VG_(message)( Vg_UserMsg,
                       "Emulation fatal error -- Valgrind cannot continue:\n");
         VG_(message)( Vg_UserMsg, "  %s\n", what);
         VG_(get_and_pp_StackTrace)( tid, VG_(clo_backtrace_size) );
         VG_(message)(Vg_UserMsg, "\n");
         VG_(message)(Vg_UserMsg, "Valgrind has to exit now.  Sorry.\n");
         VG_(message)(Vg_UserMsg, "\n");
         VG_(exit)(1);
         break;
      }

      case VEX_TRC_JMP_SIGTRAP:
         VG_(synth_sigtrap)(tid);
         break;

      case VEX_TRC_JMP_SIGSEGV:
         VG_(synth_fault)(tid);
         break;

      case VEX_TRC_JMP_SIGBUS:
         VG_(synth_sigbus)(tid);
         break;

      case VEX_TRC_JMP_NODECODE: {
         Addr addr = VG_(get_IP)(tid);

         VG_(umsg)(
            "valgrind: Unrecognised instruction at address %#lx.\n", addr);
         VG_(get_and_pp_StackTrace)(tid, VG_(clo_backtrace_size));
#define M(a) VG_(umsg)(a "\n");
   M("Your program just tried to execute an instruction that Valgrind" );
   M("did not recognise.  There are two possible reasons for this."    );
   M("1. Your program has a bug and erroneously jumped to a non-code"  );
   M("   location.  If you are running Memcheck and you just saw a"    );
   M("   warning about a bad jump, it's probably your program's fault.");
   M("2. The instruction is legitimate but Valgrind doesn't handle it,");
   M("   i.e. it's Valgrind's fault.  If you think this is the case or");
   M("   you are not sure, please let us know and we'll try to fix it.");
   M("Either way, Valgrind will now raise a SIGILL signal which will"  );
   M("probably kill your program."                                     );
#undef M

#if defined(VGA_s390x)
         UChar byte = ((UChar *)addr)[0];
         UInt  insn_length = ((((byte >> 6) + 1) >> 1) + 1) << 1;
         Addr  next_insn_addr = addr + insn_length;

         VG_(set_IP)(tid, next_insn_addr);
#endif
         VG_(synth_sigill)(tid, addr);
         break;
      }
      case VEX_TRC_JMP_TINVAL:
         VG_(discard_translations)(
            (Addr64)VG_(threads)[tid].arch.vex.guest_TISTART,
            VG_(threads)[tid].arch.vex.guest_TILEN,
            "scheduler(VEX_TRC_JMP_TINVAL)"
         );
         if (0)
            VG_(printf)("dump translations done.\n");
         break;

      case VG_TRC_INVARIANT_FAILED:
         
         
         
         
         
         
         vg_assert2(0, "VG_(scheduler), phase 3: "
                       "run_innerloop detected host "
                       "state invariant failure", trc);

      case VEX_TRC_JMP_SYS_SYSENTER:
#        if defined(VGP_x86_linux)
         vg_assert2(0, "VG_(scheduler), phase 3: "
                       "sysenter_x86 on x86-linux is not supported");
#        elif defined(VGP_x86_darwin)
         
         VG_(threads)[tid].arch.vex.guest_EIP
            = VG_(threads)[tid].arch.vex.guest_EDX;
         handle_syscall(tid, trc[0]);
#        else
         vg_assert2(0, "VG_(scheduler), phase 3: "
                       "sysenter_x86 on non-x86 platform?!?!");
#        endif
         break;

      default: 
	 vg_assert2(0, "VG_(scheduler), phase 3: "
                       "unexpected thread return code (%u)", trc[0]);
	 
	 break;

      } 

      if (0)
         maybe_show_sb_counts();
   }

   if (VG_(clo_trace_sched))
      print_sched_event(tid, "exiting VG_(scheduler)");

   vg_assert(VG_(is_exiting)(tid));

   return tst->exitreason;
}


void VG_(nuke_all_threads_except) ( ThreadId me, VgSchedReturnCode src )
{
   ThreadId tid;

   vg_assert(VG_(is_running_thread)(me));

   for (tid = 1; tid < VG_N_THREADS; tid++) {
      if (tid == me
          || VG_(threads)[tid].status == VgTs_Empty)
         continue;
      if (0)
         VG_(printf)(
            "VG_(nuke_all_threads_except): nuking tid %d\n", tid);

      VG_(threads)[tid].exitreason = src;
      if (src == VgSrc_FatalSig)
         VG_(threads)[tid].os_state.fatalsig = VKI_SIGKILL;
      VG_(get_thread_out_of_syscall)(tid);
   }
}



#if defined(VGA_x86)
#  define VG_CLREQ_ARGS       guest_EAX
#  define VG_CLREQ_RET        guest_EDX
#elif defined(VGA_amd64)
#  define VG_CLREQ_ARGS       guest_RAX
#  define VG_CLREQ_RET        guest_RDX
#elif defined(VGA_ppc32) || defined(VGA_ppc64)
#  define VG_CLREQ_ARGS       guest_GPR4
#  define VG_CLREQ_RET        guest_GPR3
#elif defined(VGA_arm)
#  define VG_CLREQ_ARGS       guest_R4
#  define VG_CLREQ_RET        guest_R3
#elif defined (VGA_s390x)
#  define VG_CLREQ_ARGS       guest_r2
#  define VG_CLREQ_RET        guest_r3
#elif defined(VGA_mips32)
#  define VG_CLREQ_ARGS       guest_r12
#  define VG_CLREQ_RET        guest_r11
#else
#  error Unknown arch
#endif

#define CLREQ_ARGS(regs)   ((regs).vex.VG_CLREQ_ARGS)
#define CLREQ_RET(regs)    ((regs).vex.VG_CLREQ_RET)
#define O_CLREQ_RET        (offsetof(VexGuestArchState, VG_CLREQ_RET))


#define SET_CLREQ_RETVAL(zztid, zzval) \
   do { CLREQ_RET(VG_(threads)[zztid].arch) = (zzval); \
        VG_TRACK( post_reg_write, \
                  Vg_CoreClientReq, zztid, O_CLREQ_RET, sizeof(UWord)); \
   } while (0)

#define SET_CLCALL_RETVAL(zztid, zzval, f) \
   do { CLREQ_RET(VG_(threads)[zztid].arch) = (zzval); \
        VG_TRACK( post_reg_write_clientcall_return, \
                  zztid, O_CLREQ_RET, sizeof(UWord), f); \
   } while (0)



static Bool os_client_request(ThreadId tid, UWord *args)
{
   Bool handled = True;

   vg_assert(VG_(is_running_thread)(tid));

   switch(args[0]) {
   case VG_USERREQ__LIBC_FREERES_DONE:
      if (0 || VG_(clo_trace_syscalls) || VG_(clo_trace_sched))
         VG_(message)(Vg_DebugMsg, 
                      "__libc_freeres() done; really quitting!\n");
      VG_(threads)[tid].exitreason = VgSrc_ExitThread;
      break;

   default:
      handled = False;
      break;
   }

   return handled;
}


static
void do_client_request ( ThreadId tid )
{
   UWord* arg = (UWord*)(CLREQ_ARGS(VG_(threads)[tid].arch));
   UWord req_no = arg[0];

   if (0)
      VG_(printf)("req no = 0x%llx, arg = %p\n", (ULong)req_no, arg);
   switch (req_no) {

      case VG_USERREQ__CLIENT_CALL0: {
         UWord (*f)(ThreadId) = (void*)arg[1];
	 if (f == NULL)
	    VG_(message)(Vg_DebugMsg, "VG_USERREQ__CLIENT_CALL0: func=%p\n", f);
	 else
	    SET_CLCALL_RETVAL(tid, f ( tid ), (Addr)f);
         break;
      }
      case VG_USERREQ__CLIENT_CALL1: {
         UWord (*f)(ThreadId, UWord) = (void*)arg[1];
	 if (f == NULL)
	    VG_(message)(Vg_DebugMsg, "VG_USERREQ__CLIENT_CALL1: func=%p\n", f);
	 else
	    SET_CLCALL_RETVAL(tid, f ( tid, arg[2] ), (Addr)f );
         break;
      }
      case VG_USERREQ__CLIENT_CALL2: {
         UWord (*f)(ThreadId, UWord, UWord) = (void*)arg[1];
	 if (f == NULL)
	    VG_(message)(Vg_DebugMsg, "VG_USERREQ__CLIENT_CALL2: func=%p\n", f);
	 else
	    SET_CLCALL_RETVAL(tid, f ( tid, arg[2], arg[3] ), (Addr)f );
         break;
      }
      case VG_USERREQ__CLIENT_CALL3: {
         UWord (*f)(ThreadId, UWord, UWord, UWord) = (void*)arg[1];
	 if (f == NULL)
	    VG_(message)(Vg_DebugMsg, "VG_USERREQ__CLIENT_CALL3: func=%p\n", f);
	 else
	    SET_CLCALL_RETVAL(tid, f ( tid, arg[2], arg[3], arg[4] ), (Addr)f );
         break;
      }

      
      
      case VG_USERREQ__RUNNING_ON_VALGRIND:
         SET_CLREQ_RETVAL(tid, RUNNING_ON_VALGRIND+1);
         break;

      case VG_USERREQ__PRINTF: {
         if (sizeof(va_list) != sizeof(UWord))
            goto va_list_casting_error_NORETURN;
         union {
            va_list vargs;
            unsigned long uw;
         } u;
         u.uw = (unsigned long)arg[2];
         Int count = 
            VG_(vmessage)( Vg_ClientMsg, (char *)arg[1], u.vargs );
         VG_(message_flush)();
         SET_CLREQ_RETVAL( tid, count );
         break;
      }

      case VG_USERREQ__PRINTF_BACKTRACE: {
         if (sizeof(va_list) != sizeof(UWord))
            goto va_list_casting_error_NORETURN;
         union {
            va_list vargs;
            unsigned long uw;
         } u;
         u.uw = (unsigned long)arg[2];
         Int count =
            VG_(vmessage)( Vg_ClientMsg, (char *)arg[1], u.vargs );
         VG_(message_flush)();
         VG_(get_and_pp_StackTrace)( tid, VG_(clo_backtrace_size) );
         SET_CLREQ_RETVAL( tid, count );
         break;
      }

      case VG_USERREQ__PRINTF_VALIST_BY_REF: {
         va_list* vargsp = (va_list*)arg[2];
         Int count = 
            VG_(vmessage)( Vg_ClientMsg, (char *)arg[1], *vargsp );
         VG_(message_flush)();
         SET_CLREQ_RETVAL( tid, count );
         break;
      }

      case VG_USERREQ__PRINTF_BACKTRACE_VALIST_BY_REF: {
         va_list* vargsp = (va_list*)arg[2];
         Int count =
            VG_(vmessage)( Vg_ClientMsg, (char *)arg[1], *vargsp );
         VG_(message_flush)();
         VG_(get_and_pp_StackTrace)( tid, VG_(clo_backtrace_size) );
         SET_CLREQ_RETVAL( tid, count );
         break;
      }

      case VG_USERREQ__INTERNAL_PRINTF_VALIST_BY_REF: {
         va_list* vargsp = (va_list*)arg[2];
         Int count = 
            VG_(vmessage)( Vg_DebugMsg, (char *)arg[1], *vargsp );
         VG_(message_flush)();
         SET_CLREQ_RETVAL( tid, count );
         break;
      }

      case VG_USERREQ__ADD_IFUNC_TARGET: {
         VG_(redir_add_ifunc_target)( arg[1], arg[2] );
         SET_CLREQ_RETVAL( tid, 0);
         break; }

      case VG_USERREQ__STACK_REGISTER: {
         UWord sid = VG_(register_stack)((Addr)arg[1], (Addr)arg[2]);
         SET_CLREQ_RETVAL( tid, sid );
         break; }

      case VG_USERREQ__STACK_DEREGISTER: {
         VG_(deregister_stack)(arg[1]);
         SET_CLREQ_RETVAL( tid, 0 );     
         break; }

      case VG_USERREQ__STACK_CHANGE: {
         VG_(change_stack)(arg[1], (Addr)arg[2], (Addr)arg[3]);
         SET_CLREQ_RETVAL( tid, 0 );     
         break; }

      case VG_USERREQ__GET_MALLOCFUNCS: {
	 struct vg_mallocfunc_info *info = (struct vg_mallocfunc_info *)arg[1];

	 info->tl_malloc               = VG_(tdict).tool_malloc;
	 info->tl_calloc               = VG_(tdict).tool_calloc;
	 info->tl_realloc              = VG_(tdict).tool_realloc;
	 info->tl_memalign             = VG_(tdict).tool_memalign;
	 info->tl___builtin_new        = VG_(tdict).tool___builtin_new;
	 info->tl___builtin_vec_new    = VG_(tdict).tool___builtin_vec_new;
	 info->tl_free                 = VG_(tdict).tool_free;
	 info->tl___builtin_delete     = VG_(tdict).tool___builtin_delete;
	 info->tl___builtin_vec_delete = VG_(tdict).tool___builtin_vec_delete;
         info->tl_malloc_usable_size   = VG_(tdict).tool_malloc_usable_size;

	 info->mallinfo                = VG_(mallinfo);
	 info->clo_trace_malloc        = VG_(clo_trace_malloc);

         SET_CLREQ_RETVAL( tid, 0 );     

	 break;
      }

      

      case VG_USERREQ__DISCARD_TRANSLATIONS:
         if (VG_(clo_verbosity) > 2)
            VG_(printf)( "client request: DISCARD_TRANSLATIONS,"
                         " addr %p,  len %lu\n",
                         (void*)arg[1], arg[2] );

         VG_(discard_translations)( 
            arg[1], arg[2], "scheduler(VG_USERREQ__DISCARD_TRANSLATIONS)" 
         );

         SET_CLREQ_RETVAL( tid, 0 );     
	 break;

      case VG_USERREQ__COUNT_ERRORS:  
         SET_CLREQ_RETVAL( tid, VG_(get_n_errs_found)() );
         break;

      case VG_USERREQ__LOAD_PDB_DEBUGINFO:
         VG_(di_notify_pdb_debuginfo)( arg[1], arg[2], arg[3], arg[4] );
         SET_CLREQ_RETVAL( tid, 0 );     
         break;

      case VG_USERREQ__MAP_IP_TO_SRCLOC: {
         Addr   ip    = arg[1];
         UChar* buf64 = (UChar*)arg[2];

         VG_(memset)(buf64, 0, 64);
         UInt linenum = 0;
         Bool ok = VG_(get_filename_linenum)(
                      ip, &buf64[0], 50, NULL, 0, NULL, &linenum
                   );
         if (ok) {
            
            UInt i;
            for (i = 0; i < 50; i++) {
               if (buf64[i] == 0)
                  break;
            }
            vg_assert(i < 50);
            VG_(sprintf)(&buf64[i], ":%u", linenum);
         } else {
            buf64[0] = 0;
         }

         SET_CLREQ_RETVAL( tid, 0 ); 
         break;
      }

      case VG_USERREQ__CHANGE_ERR_DISABLEMENT: {
         Word delta = arg[1];
         vg_assert(delta == 1 || delta == -1);
         ThreadState* tst = VG_(get_ThreadState)(tid);
         vg_assert(tst);
         if (delta == 1 && tst->err_disablement_level < 0xFFFFFFFF) {
            tst->err_disablement_level++;
         }
         else
         if (delta == -1 && tst->err_disablement_level > 0) {
            tst->err_disablement_level--;
         }
         SET_CLREQ_RETVAL( tid, 0 ); 
         break;
      }

      case VG_USERREQ__MALLOCLIKE_BLOCK:
      case VG_USERREQ__RESIZEINPLACE_BLOCK:
      case VG_USERREQ__FREELIKE_BLOCK:
         
         if (!arg[1]) {
            SET_CLREQ_RETVAL( tid, 0 );     
            break;
         } else {
            goto my_default;
         }

      default:
       my_default:
	 if (os_client_request(tid, arg)) {
	    
         } else if (VG_(needs).client_requests) {
	    UWord ret;

            if (VG_(clo_verbosity) > 2)
               VG_(printf)("client request: code %lx,  addr %p,  len %lu\n",
                           arg[0], (void*)arg[1], arg[2] );

	    if ( VG_TDICT_CALL(tool_handle_client_request, tid, arg, &ret) )
	       SET_CLREQ_RETVAL(tid, ret);
         } else {
	    static Bool whined = False;

	    if (!whined && VG_(clo_verbosity) > 2) {
               
               
               Char c1 = (arg[0] >> 24) & 0xff;
               Char c2 = (arg[0] >> 16) & 0xff;
               if (c1 == 0) c1 = '_';
               if (c2 == 0) c2 = '_';
	       VG_(message)(Vg_UserMsg, "Warning:\n"
                   "  unhandled client request: 0x%lx (%c%c+0x%lx).  Perhaps\n"
		   "  VG_(needs).client_requests should be set?\n",
			    arg[0], c1, c2, arg[0] & 0xffff);
	       whined = True;
	    }
         }
         break;
   }
   return;

   
  va_list_casting_error_NORETURN:
   VG_(umsg)(
      "Valgrind: fatal error - cannot continue: use of the deprecated\n"
      "client requests VG_USERREQ__PRINTF or VG_USERREQ__PRINTF_BACKTRACE\n"
      "on a platform where they cannot be supported.  Please use the\n"
      "equivalent _VALIST_BY_REF versions instead.\n"
      "\n"
      "This is a binary-incompatible change in Valgrind's client request\n"
      "mechanism.  It is unfortunate, but difficult to avoid.  End-users\n"
      "are expected to almost never see this message.  The only case in\n"
      "which you might see this message is if your code uses the macros\n"
      "VALGRIND_PRINTF or VALGRIND_PRINTF_BACKTRACE.  If so, you will need\n"
      "to recompile such code, using the header files from this version of\n"
      "Valgrind, and not any previous version.\n"
      "\n"
      "If you see this mesage in any other circumstances, it is probably\n"
      "a bug in Valgrind.  In this case, please file a bug report at\n"
      "\n"
      "   http://www.valgrind.org/support/bug_reports.html\n"
      "\n"
      "Will now abort.\n"
   );
   vg_assert(0);
}



static
void scheduler_sanity ( ThreadId tid )
{
   Bool bad = False;
   static UInt lasttime = 0;
   UInt now;
   Int lwpid = VG_(gettid)();

   if (!VG_(is_running_thread)(tid)) {
      VG_(message)(Vg_DebugMsg,
		   "Thread %d is supposed to be running, "
                   "but doesn't own the_BigLock (owned by %d)\n", 
		   tid, VG_(running_tid));
      bad = True;
   }

   if (lwpid != VG_(threads)[tid].os_state.lwpid) {
      VG_(message)(Vg_DebugMsg,
                   "Thread %d supposed to be in LWP %d, but we're actually %d\n",
                   tid, VG_(threads)[tid].os_state.lwpid, VG_(gettid)());
      bad = True;
   }

   if (lwpid != ML_(get_sched_lock_owner)(the_BigLock)) {
      VG_(message)(Vg_DebugMsg,
                   "Thread (LWPID) %d doesn't own the_BigLock\n",
                   tid);
      bad = True;
   }

   now = VG_(read_millisecond_timer)();
   if (0 && (!bad) && (lasttime + 4000 <= now)) {
      lasttime = now;
      VG_(printf)("\n------------ Sched State at %d ms ------------\n",
                  (Int)now);
      VG_(show_sched_status)();
   }

   if (bad)
      VG_(core_panic)("scheduler_sanity: failed");
}

void VG_(sanity_check_general) ( Bool force_expensive )
{
   ThreadId tid;

   static UInt next_slow_check_at = 1;
   static UInt slow_check_interval = 25;

   if (VG_(clo_sanity_level) < 1) return;

   

   sanity_fast_count++;

   

   if (VG_(needs).sanity_checks) {
      vg_assert(VG_TDICT_CALL(tool_cheap_sanity_check));
   }

   

   if ( force_expensive
        || VG_(clo_sanity_level) > 1
        || (VG_(clo_sanity_level) == 1 
            && sanity_fast_count == next_slow_check_at)) {

      if (0) VG_(printf)("SLOW at %d\n", sanity_fast_count-1);

      next_slow_check_at = sanity_fast_count - 1 + slow_check_interval;
      slow_check_interval++;
      sanity_slow_count++;

      if (VG_(needs).sanity_checks) {
          vg_assert(VG_TDICT_CALL(tool_expensive_sanity_check));
      }

      
      for (tid = 1; tid < VG_N_THREADS; tid++) {
	 SizeT    remains;
         VgStack* stack;

	 if (VG_(threads)[tid].status == VgTs_Empty ||
	     VG_(threads)[tid].status == VgTs_Zombie)
	    continue;

         stack 
            = (VgStack*)
              VG_(get_ThreadState)(tid)->os_state.valgrind_stack_base;
         SizeT limit
            = 4096; 
	 remains 
            = VG_(am_get_VgStack_unused_szB)(stack, limit);
	 if (remains < limit)
	    VG_(message)(Vg_DebugMsg, 
                         "WARNING: Thread %d is within %ld bytes "
                         "of running out of stack!\n",
		         tid, remains);
      }
   }

   if (VG_(clo_sanity_level) > 1) {
      VG_(sanity_check_malloc_all)();
   }
}

