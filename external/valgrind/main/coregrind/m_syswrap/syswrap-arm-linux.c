

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Nicholas Nethercote
      njn@valgrind.org
   Copyright (C) 2008-2012 Evan Geller
      gaze@bea.ms

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

#if defined(VGP_arm_linux)

#include "pub_core_basics.h"
#include "pub_core_vki.h"
#include "pub_core_vkiscnums.h"
#include "pub_core_libcsetjmp.h"    
#include "pub_core_threadstate.h"
#include "pub_core_aspacemgr.h"
#include "pub_core_debuglog.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcproc.h"
#include "pub_core_libcsignal.h"
#include "pub_core_options.h"
#include "pub_core_scheduler.h"
#include "pub_core_sigframe.h"      
#include "pub_core_signals.h"
#include "pub_core_syscall.h"
#include "pub_core_syswrap.h"
#include "pub_core_tooliface.h"
#include "pub_core_stacks.h"        
#include "pub_core_transtab.h"      

#include "priv_types_n_macros.h"
#include "priv_syswrap-generic.h"   
#include "priv_syswrap-linux.h"     
#include "priv_syswrap-main.h"



__attribute__((noreturn))
void ML_(call_on_new_stack_0_1) ( Addr stack,
                                  Addr retaddr,
                                  void (*f)(Word),
                                  Word arg1 );
asm(
".text\n"
".globl vgModuleLocal_call_on_new_stack_0_1\n"
"vgModuleLocal_call_on_new_stack_0_1:\n"
"   mov    sp,r0\n\t" 
"   mov    lr,r1\n\t" 
"   mov    r0,r3\n\t" 
"   push   {r2}\n\t"  
"   mov    r1, #0\n\t" 
"   mov    r2, #0\n\t"
"   mov    r3, #0\n\t"
"   mov    r4, #0\n\t"
"   mov    r5, #0\n\t"
"   mov    r6, #0\n\t"
"   mov    r7, #0\n\t"
"   mov    r8, #0\n\t"
"   mov    r9, #0\n\t"
"   mov    r10, #0\n\t"
"   mov    r11, #0\n\t"
"   mov    r12, #0\n\t"
"   pop    {pc}\n\t"  
".previous\n"
);


#define __NR_CLONE        VG_STRINGIFY(__NR_clone)
#define __NR_EXIT         VG_STRINGIFY(__NR_exit)

extern
ULong do_syscall_clone_arm_linux   ( Word (*fn)(void *), 
                                     void* stack, 
                                     Int   flags, 
                                     void* arg,
                                     Int*  child_tid,
                                     Int*  parent_tid,
                                     void* tls );
asm(
".text\n"
".globl do_syscall_clone_arm_linux\n"
"do_syscall_clone_arm_linux:\n"

"   str     r0, [r1, #-4]!\n"
"   str     r3, [r1, #-4]!\n"
"   push {r4,r7}\n" 
"   mov r0, r2\n" 
"   ldr r2, [sp, #12]\n" 
"   ldr r3, [sp, #16]\n" 
"   ldr r4, [sp, #8]\n" 
"   mov r7, #"__NR_CLONE"\n"
"   svc 0x00000000\n"
"   cmp r0, #0\n"
"   beq 1f\n"

"   pop {r4,r7}\n"
"   bx lr\n"

"1:\n" 
"   mov     lr, pc\n"
"   pop     {r0,pc}\n"
"   mov r7, #"__NR_EXIT"\n"
"   svc 0x00000000\n"
"   .long 0\n"
"   .previous\n"
);

#undef __NR_CLONE
#undef __NR_EXIT

static void setup_child ( ThreadArchState*, ThreadArchState* );
static void assign_guest_tls(ThreadId ctid, Addr tlsptr);
static SysRes sys_set_tls ( ThreadId tid, Addr tlsptr );
            
static SysRes do_clone ( ThreadId ptid, 
                         UInt flags, Addr sp, 
                         Int *parent_tidptr, 
                         Int *child_tidptr, 
                         Addr child_tls)
{
   const Bool debug = False;

   ThreadId ctid = VG_(alloc_ThreadState)();
   ThreadState* ptst = VG_(get_ThreadState)(ptid);
   ThreadState* ctst = VG_(get_ThreadState)(ctid);
   UInt r0;
   UWord *stack;
   NSegment const* seg;
   SysRes res;
   vki_sigset_t blockall, savedmask;

   VG_(sigfillset)(&blockall);

   vg_assert(VG_(is_running_thread)(ptid));
   vg_assert(VG_(is_valid_tid)(ctid));

   stack = (UWord*)ML_(allocstack)(ctid);

   if(stack == NULL) {
      res = VG_(mk_SysRes_Error)( VKI_ENOMEM );
      goto out;
   }

   setup_child( &ctst->arch, &ptst->arch );

   ctst->arch.vex.guest_R0 = 0;
   if(sp != 0)
      ctst->arch.vex.guest_R13 = sp;

   ctst->os_state.parent = ptid;

   ctst->sig_mask = ptst->sig_mask;
   ctst->tmp_sig_mask = ptst->sig_mask;

   ctst->os_state.threadgroup = ptst->os_state.threadgroup;

   seg = VG_(am_find_nsegment)((Addr)sp);
   if (seg && seg->kind != SkResvn) {
      ctst->client_stack_highest_word = (Addr)VG_PGROUNDUP(sp);
      ctst->client_stack_szB = ctst->client_stack_highest_word - seg->start;
   
      VG_(register_stack)(seg->start, ctst->client_stack_highest_word);
   
      if (debug)
         VG_(printf)("tid %d: guessed client stack range %#lx-%#lx\n",
         ctid, seg->start, VG_PGROUNDUP(sp));
   } else {
      VG_(message)(Vg_UserMsg, "!? New thread %d starts with sp+%#lx) unmapped\n", ctid, sp);
      ctst->client_stack_szB  = 0;
   }

   vg_assert(VG_(owns_BigLock_LL)(ptid));
   VG_TRACK ( pre_thread_ll_create, ptid, ctid );

   if (flags & VKI_CLONE_SETTLS) {
      
      assign_guest_tls(ctid, child_tls);
   }
    
   flags &= ~VKI_CLONE_SETTLS;

   VG_(sigprocmask)(VKI_SIG_SETMASK, &blockall, &savedmask);

   r0 = do_syscall_clone_arm_linux(
      ML_(start_thread_NORETURN), stack, flags, &VG_(threads)[ctid],
      child_tidptr, parent_tidptr, NULL
   );
   
    
   res = VG_(mk_SysRes_arm_linux)( r0 );

   VG_(sigprocmask)(VKI_SIG_SETMASK, &savedmask, NULL);

out:
   if (sr_isError(res)) {
      VG_(cleanup_thread)(&ctst->arch);
      ctst->status = VgTs_Empty;
      VG_TRACK( pre_thread_ll_exit, ctid );
   }

   return res;
}




void VG_(cleanup_thread) ( ThreadArchState* arch )
{
}  

void setup_child (  ThreadArchState *child,
                     ThreadArchState *parent )
{
   child->vex = parent->vex;
   child->vex_shadow1 = parent->vex_shadow1;
   child->vex_shadow2 = parent->vex_shadow2;
}

static void assign_guest_tls(ThreadId tid, Addr tlsptr)
{
   VG_(threads)[tid].arch.vex.guest_TPIDRURO = tlsptr;
}

static SysRes sys_set_tls ( ThreadId tid, Addr tlsptr )
{
   assign_guest_tls(tid, tlsptr);
#if defined(ANDROID_HARDWARE_emulator)
   return VG_(do_syscall1) (__NR_ARM_set_tls, tlsptr);
#else
   return VG_(mk_SysRes_Success)( 0 );
#endif
}


#define PRE(name)       DEFN_PRE_TEMPLATE(arm_linux, name)
#define POST(name)      DEFN_POST_TEMPLATE(arm_linux, name)


DECL_TEMPLATE(arm_linux, sys_socketcall);
DECL_TEMPLATE(arm_linux, sys_socket);
DECL_TEMPLATE(arm_linux, sys_setsockopt);
DECL_TEMPLATE(arm_linux, sys_getsockopt);
DECL_TEMPLATE(arm_linux, sys_connect);
DECL_TEMPLATE(arm_linux, sys_accept);
DECL_TEMPLATE(arm_linux, sys_accept4);
DECL_TEMPLATE(arm_linux, sys_sendto);
DECL_TEMPLATE(arm_linux, sys_recvfrom);
DECL_TEMPLATE(arm_linux, sys_semget);
DECL_TEMPLATE(arm_linux, sys_semop);
DECL_TEMPLATE(arm_linux, sys_semctl);
DECL_TEMPLATE(arm_linux, sys_semtimedop);
DECL_TEMPLATE(arm_linux, wrap_sys_shmat);
DECL_TEMPLATE(arm_linux, sys_shmget);
DECL_TEMPLATE(arm_linux, sys_shmdt);
DECL_TEMPLATE(arm_linux, sys_shmctl);
DECL_TEMPLATE(arm_linux, sys_sendmsg);
DECL_TEMPLATE(arm_linux, sys_recvmsg);
DECL_TEMPLATE(arm_linux, sys_msgget);
DECL_TEMPLATE(arm_linux, sys_msgrcv);
DECL_TEMPLATE(arm_linux, sys_msgsnd);
DECL_TEMPLATE(arm_linux, sys_msgctl);
DECL_TEMPLATE(arm_linux, sys_shutdown);
DECL_TEMPLATE(arm_linux, sys_bind);
DECL_TEMPLATE(arm_linux, sys_listen);
DECL_TEMPLATE(arm_linux, sys_getsockname);
DECL_TEMPLATE(arm_linux, sys_getpeername);
DECL_TEMPLATE(arm_linux, sys_socketpair);
DECL_TEMPLATE(arm_linux, sys_send);
DECL_TEMPLATE(arm_linux, sys_recv);
DECL_TEMPLATE(arm_linux, sys_mmap2);
DECL_TEMPLATE(arm_linux, sys_stat64);
DECL_TEMPLATE(arm_linux, sys_lstat64);
DECL_TEMPLATE(arm_linux, sys_fstatat64);
DECL_TEMPLATE(arm_linux, sys_fstat64);
DECL_TEMPLATE(arm_linux, sys_clone);
DECL_TEMPLATE(arm_linux, sys_sigreturn);
DECL_TEMPLATE(arm_linux, sys_rt_sigreturn);
DECL_TEMPLATE(arm_linux, sys_sigsuspend);
DECL_TEMPLATE(arm_linux, sys_set_tls);
DECL_TEMPLATE(arm_linux, sys_cacheflush);
DECL_TEMPLATE(arm_linux, sys_ptrace);

PRE(sys_socketcall)
{
#  define ARG2_0  (((UWord*)ARG2)[0])
#  define ARG2_1  (((UWord*)ARG2)[1])
#  define ARG2_2  (((UWord*)ARG2)[2])
#  define ARG2_3  (((UWord*)ARG2)[3])
#  define ARG2_4  (((UWord*)ARG2)[4])
#  define ARG2_5  (((UWord*)ARG2)[5])

   *flags |= SfMayBlock;
   PRINT("sys_socketcall ( %ld, %#lx )",ARG1,ARG2);
   PRE_REG_READ2(long, "socketcall", int, call, unsigned long *, args);

   switch (ARG1 ) {

   case VKI_SYS_SOCKETPAIR:
     
      PRE_MEM_READ( "socketcall.socketpair(args)", ARG2, 4*sizeof(Addr) );
      ML_(generic_PRE_sys_socketpair)( tid, ARG2_0, ARG2_1, ARG2_2, ARG2_3 );
      break;

   case VKI_SYS_SOCKET:
     
      PRE_MEM_READ( "socketcall.socket(args)", ARG2, 3*sizeof(Addr) );
      break;

   case VKI_SYS_BIND:
      PRE_MEM_READ( "socketcall.bind(args)", ARG2, 3*sizeof(Addr) );
      ML_(generic_PRE_sys_bind)( tid, ARG2_0, ARG2_1, ARG2_2 );
      break;

   case VKI_SYS_LISTEN:
     
      PRE_MEM_READ( "socketcall.listen(args)", ARG2, 2*sizeof(Addr) );
      break;

   case VKI_SYS_ACCEPT: {
     
      PRE_MEM_READ( "socketcall.accept(args)", ARG2, 3*sizeof(Addr) );
      ML_(generic_PRE_sys_accept)( tid, ARG2_0, ARG2_1, ARG2_2 );
      break;
   }

   case VKI_SYS_ACCEPT4: {
      
      PRE_MEM_READ( "socketcall.accept4(args)", ARG2, 4*sizeof(Addr) );
      ML_(generic_PRE_sys_accept)( tid, ARG2_0, ARG2_1, ARG2_2 );
      break;
   }

   case VKI_SYS_SENDTO:
     PRE_MEM_READ( "socketcall.sendto(args)", ARG2, 6*sizeof(Addr) );
     ML_(generic_PRE_sys_sendto)( tid, ARG2_0, ARG2_1, ARG2_2,
              ARG2_3, ARG2_4, ARG2_5 );
     break;

   case VKI_SYS_SEND:
     
     PRE_MEM_READ( "socketcall.send(args)", ARG2, 4*sizeof(Addr) );
     ML_(generic_PRE_sys_send)( tid, ARG2_0, ARG2_1, ARG2_2 );
     break;

   case VKI_SYS_RECVFROM:
     PRE_MEM_READ( "socketcall.recvfrom(args)", ARG2, 6*sizeof(Addr) );
     ML_(generic_PRE_sys_recvfrom)( tid, ARG2_0, ARG2_1, ARG2_2,
                ARG2_3, ARG2_4, ARG2_5 );
     break;

   case VKI_SYS_RECV:
     
     PRE_MEM_READ( "socketcall.recv(args)", ARG2, 4*sizeof(Addr) );
     ML_(generic_PRE_sys_recv)( tid, ARG2_0, ARG2_1, ARG2_2 );
     break;

   case VKI_SYS_CONNECT:
     PRE_MEM_READ( "socketcall.connect(args)", ARG2, 3*sizeof(Addr) );
     ML_(generic_PRE_sys_connect)( tid, ARG2_0, ARG2_1, ARG2_2 );
     break;

   case VKI_SYS_SETSOCKOPT:
     PRE_MEM_READ( "socketcall.setsockopt(args)", ARG2, 5*sizeof(Addr) );
     ML_(generic_PRE_sys_setsockopt)( tid, ARG2_0, ARG2_1, ARG2_2,
                  ARG2_3, ARG2_4 );
     break;

   case VKI_SYS_GETSOCKOPT:
     PRE_MEM_READ( "socketcall.getsockopt(args)", ARG2, 5*sizeof(Addr) );
     ML_(linux_PRE_sys_getsockopt)( tid, ARG2_0, ARG2_1, ARG2_2,
                  ARG2_3, ARG2_4 );
     break;

   case VKI_SYS_GETSOCKNAME:
     
     PRE_MEM_READ( "socketcall.getsockname(args)", ARG2, 3*sizeof(Addr) );
     ML_(generic_PRE_sys_getsockname)( tid, ARG2_0, ARG2_1, ARG2_2 );
     break;

   case VKI_SYS_GETPEERNAME:
     
     PRE_MEM_READ( "socketcall.getpeername(args)", ARG2, 3*sizeof(Addr) );
     ML_(generic_PRE_sys_getpeername)( tid, ARG2_0, ARG2_1, ARG2_2 );
     break;

   case VKI_SYS_SHUTDOWN:
     
     PRE_MEM_READ( "socketcall.shutdown(args)", ARG2, 2*sizeof(Addr) );
     break;

   case VKI_SYS_SENDMSG: {
     

     ML_(generic_PRE_sys_sendmsg)( tid, "msg", (struct vki_msghdr *)ARG2_1 );
     break;
   }

   case VKI_SYS_RECVMSG: {
     

     ML_(generic_PRE_sys_recvmsg)( tid, "msg", (struct vki_msghdr *)ARG2_1 );
     break;
   }

   default:
     VG_(message)(Vg_DebugMsg,"Warning: unhandled socketcall 0x%lx",ARG1);
     SET_STATUS_Failure( VKI_EINVAL );
     break;
   }
#  undef ARG2_0
#  undef ARG2_1
#  undef ARG2_2
#  undef ARG2_3
#  undef ARG2_4
#  undef ARG2_5
}

POST(sys_socketcall)
{
#  define ARG2_0  (((UWord*)ARG2)[0])
#  define ARG2_1  (((UWord*)ARG2)[1])
#  define ARG2_2  (((UWord*)ARG2)[2])
#  define ARG2_3  (((UWord*)ARG2)[3])
#  define ARG2_4  (((UWord*)ARG2)[4])
#  define ARG2_5  (((UWord*)ARG2)[5])

  SysRes r;
  vg_assert(SUCCESS);
  switch (ARG1 ) {

  case VKI_SYS_SOCKETPAIR:
    r = ML_(generic_POST_sys_socketpair)(
                tid, VG_(mk_SysRes_Success)(RES),
                ARG2_0, ARG2_1, ARG2_2, ARG2_3
                );
    SET_STATUS_from_SysRes(r);
    break;

  case VKI_SYS_SOCKET:
    r = ML_(generic_POST_sys_socket)( tid, VG_(mk_SysRes_Success)(RES) );
    SET_STATUS_from_SysRes(r);
    break;

  case VKI_SYS_BIND:
    break;

  case VKI_SYS_LISTEN:
    
    break;

  case VKI_SYS_ACCEPT:
  case VKI_SYS_ACCEPT4:
    
    
    r = ML_(generic_POST_sys_accept)( tid, VG_(mk_SysRes_Success)(RES),
                  ARG2_0, ARG2_1, ARG2_2 );
    SET_STATUS_from_SysRes(r);
    break;

  case VKI_SYS_SENDTO:
    break;

  case VKI_SYS_SEND:
    break;

  case VKI_SYS_RECVFROM:
    ML_(generic_POST_sys_recvfrom)( tid, VG_(mk_SysRes_Success)(RES),
                ARG2_0, ARG2_1, ARG2_2,
                ARG2_3, ARG2_4, ARG2_5 );
    break;

  case VKI_SYS_RECV:
    ML_(generic_POST_sys_recv)( tid, RES, ARG2_0, ARG2_1, ARG2_2 );
    break;

  case VKI_SYS_CONNECT:
    break;

  case VKI_SYS_SETSOCKOPT:
    break;

  case VKI_SYS_GETSOCKOPT:
    ML_(linux_POST_sys_getsockopt)( tid, VG_(mk_SysRes_Success)(RES),
                  ARG2_0, ARG2_1,
                  ARG2_2, ARG2_3, ARG2_4 );
    break;

  case VKI_SYS_GETSOCKNAME:
    ML_(generic_POST_sys_getsockname)( tid, VG_(mk_SysRes_Success)(RES),
                   ARG2_0, ARG2_1, ARG2_2 );
    break;

  case VKI_SYS_GETPEERNAME:
    ML_(generic_POST_sys_getpeername)( tid, VG_(mk_SysRes_Success)(RES),
                   ARG2_0, ARG2_1, ARG2_2 );
    break;

  case VKI_SYS_SHUTDOWN:
    break;

  case VKI_SYS_SENDMSG:
    break;

  case VKI_SYS_RECVMSG:
    ML_(generic_POST_sys_recvmsg)( tid, "msg", (struct vki_msghdr *)ARG2_1, RES );
    break;

  default:
    VG_(message)(Vg_DebugMsg,"FATAL: unhandled socketcall 0x%lx",ARG1);
    VG_(core_panic)("... bye!\n");
    break; 
  }
#  undef ARG2_0
#  undef ARG2_1
#  undef ARG2_2
#  undef ARG2_3
#  undef ARG2_4
#  undef ARG2_5
}

PRE(sys_socket)
{
   PRINT("sys_socket ( %ld, %ld, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "socket", int, domain, int, type, int, protocol);
}
POST(sys_socket)
{
   SysRes r;
   vg_assert(SUCCESS);
   r = ML_(generic_POST_sys_socket)(tid, VG_(mk_SysRes_Success)(RES));
   SET_STATUS_from_SysRes(r);
}

PRE(sys_setsockopt)
{
   PRINT("sys_setsockopt ( %ld, %ld, %ld, %#lx, %ld )",ARG1,ARG2,ARG3,ARG4,ARG5);
   PRE_REG_READ5(long, "setsockopt",
                 int, s, int, level, int, optname,
                 const void *, optval, int, optlen);
   ML_(generic_PRE_sys_setsockopt)(tid, ARG1,ARG2,ARG3,ARG4,ARG5);
}

PRE(sys_getsockopt)
{
   PRINT("sys_getsockopt ( %ld, %ld, %ld, %#lx, %#lx )",ARG1,ARG2,ARG3,ARG4,ARG5);
   PRE_REG_READ5(long, "getsockopt",
                 int, s, int, level, int, optname,
                 void *, optval, int, *optlen);
   ML_(linux_PRE_sys_getsockopt)(tid, ARG1,ARG2,ARG3,ARG4,ARG5);
}
POST(sys_getsockopt)
{
   vg_assert(SUCCESS);
   ML_(linux_POST_sys_getsockopt)(tid, VG_(mk_SysRes_Success)(RES),
                                         ARG1,ARG2,ARG3,ARG4,ARG5);
}

PRE(sys_connect)
{
   *flags |= SfMayBlock;
   PRINT("sys_connect ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "connect",
                 int, sockfd, struct sockaddr *, serv_addr, int, addrlen);
   ML_(generic_PRE_sys_connect)(tid, ARG1,ARG2,ARG3);
}

PRE(sys_accept)
{
   *flags |= SfMayBlock;
   PRINT("sys_accept ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "accept",
                 int, s, struct sockaddr *, addr, int, *addrlen);
   ML_(generic_PRE_sys_accept)(tid, ARG1,ARG2,ARG3);
}
POST(sys_accept)
{
   SysRes r;
   vg_assert(SUCCESS);
   r = ML_(generic_POST_sys_accept)(tid, VG_(mk_SysRes_Success)(RES),
                                         ARG1,ARG2,ARG3);
   SET_STATUS_from_SysRes(r);
}

PRE(sys_accept4)
{
   *flags |= SfMayBlock;
   PRINT("sys_accept4 ( %ld, %#lx, %ld, %ld )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "accept4",
                 int, s, struct sockaddr *, addr, int, *addrlen, int, flags);
   ML_(generic_PRE_sys_accept)(tid, ARG1,ARG2,ARG3);
}
POST(sys_accept4)
{
   SysRes r;
   vg_assert(SUCCESS);
   r = ML_(generic_POST_sys_accept)(tid, VG_(mk_SysRes_Success)(RES),
                                         ARG1,ARG2,ARG3);
   SET_STATUS_from_SysRes(r);
}

PRE(sys_sendto)
{
   *flags |= SfMayBlock;
   PRINT("sys_sendto ( %ld, %#lx, %ld, %lu, %#lx, %ld )",ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);
   PRE_REG_READ6(long, "sendto",
                 int, s, const void *, msg, int, len, 
                 unsigned int, flags, 
                 const struct sockaddr *, to, int, tolen);
   ML_(generic_PRE_sys_sendto)(tid, ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);
}

PRE(sys_recvfrom)
{
   *flags |= SfMayBlock;
   PRINT("sys_recvfrom ( %ld, %#lx, %ld, %lu, %#lx, %#lx )",ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);
   PRE_REG_READ6(long, "recvfrom",
                 int, s, void *, buf, int, len, unsigned int, flags,
                 struct sockaddr *, from, int *, fromlen);
   ML_(generic_PRE_sys_recvfrom)(tid, ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);
}
POST(sys_recvfrom)
{
   vg_assert(SUCCESS);
   ML_(generic_POST_sys_recvfrom)(tid, VG_(mk_SysRes_Success)(RES),
                                       ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);
}

PRE(sys_sendmsg)
{
   *flags |= SfMayBlock;
   PRINT("sys_sendmsg ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "sendmsg",
                 int, s, const struct msghdr *, msg, int, flags);
   ML_(generic_PRE_sys_sendmsg)(tid, "msg", (struct vki_msghdr *)ARG2);
}

PRE(sys_recvmsg)
{
   *flags |= SfMayBlock;
   PRINT("sys_recvmsg ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "recvmsg", int, s, struct msghdr *, msg, int, flags);
   ML_(generic_PRE_sys_recvmsg)(tid, "msg", (struct vki_msghdr *)ARG2);
}
POST(sys_recvmsg)
{
   ML_(generic_POST_sys_recvmsg)(tid, "msg", (struct vki_msghdr *)ARG2, RES);
}

PRE(sys_semget)
{
   PRINT("sys_semget ( %ld, %ld, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "semget", vki_key_t, key, int, nsems, int, semflg);
}

PRE(sys_semop)
{
   *flags |= SfMayBlock;
   PRINT("sys_semop ( %ld, %#lx, %lu )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "semop",
                 int, semid, struct sembuf *, sops, unsigned, nsoops);
   ML_(generic_PRE_sys_semop)(tid, ARG1,ARG2,ARG3);
}

PRE(sys_semctl)
{
   switch (ARG3 & ~VKI_IPC_64) {
   case VKI_IPC_INFO:
   case VKI_SEM_INFO:
      PRINT("sys_semctl ( %ld, %ld, %ld, %#lx )",ARG1,ARG2,ARG3,ARG4);
      PRE_REG_READ4(long, "semctl",
                    int, semid, int, semnum, int, cmd, struct seminfo *, arg);
      break;
   case VKI_IPC_STAT:
   case VKI_SEM_STAT:
   case VKI_IPC_SET:
      PRINT("sys_semctl ( %ld, %ld, %ld, %#lx )",ARG1,ARG2,ARG3,ARG4);
      PRE_REG_READ4(long, "semctl",
                    int, semid, int, semnum, int, cmd, struct semid_ds *, arg);
      break;
   case VKI_GETALL:
   case VKI_SETALL:
      PRINT("sys_semctl ( %ld, %ld, %ld, %#lx )",ARG1,ARG2,ARG3,ARG4);
      PRE_REG_READ4(long, "semctl",
                    int, semid, int, semnum, int, cmd, unsigned short *, arg);
      break;
   default:
      PRINT("sys_semctl ( %ld, %ld, %ld )",ARG1,ARG2,ARG3);
      PRE_REG_READ3(long, "semctl",
                    int, semid, int, semnum, int, cmd);
      break;
   }
   ML_(generic_PRE_sys_semctl)(tid, ARG1,ARG2,ARG3,ARG4);
}

POST(sys_semctl)
{
   ML_(generic_POST_sys_semctl)(tid, RES,ARG1,ARG2,ARG3,ARG4);
}

PRE(sys_semtimedop)
{
   *flags |= SfMayBlock;
   PRINT("sys_semtimedop ( %ld, %#lx, %lu, %#lx )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "semtimedop",
                 int, semid, struct sembuf *, sops, unsigned, nsoops,
                 struct timespec *, timeout);
   ML_(generic_PRE_sys_semtimedop)(tid, ARG1,ARG2,ARG3,ARG4);
}

PRE(sys_msgget)
{
   PRINT("sys_msgget ( %ld, %ld )",ARG1,ARG2);
   PRE_REG_READ2(long, "msgget", vki_key_t, key, int, msgflg);
}

PRE(sys_msgsnd)
{
   PRINT("sys_msgsnd ( %ld, %#lx, %ld, %ld )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "msgsnd",
                 int, msqid, struct msgbuf *, msgp, vki_size_t, msgsz, int, msgflg);
   ML_(linux_PRE_sys_msgsnd)(tid, ARG1,ARG2,ARG3,ARG4);
   if ((ARG4 & VKI_IPC_NOWAIT) == 0)
      *flags |= SfMayBlock;
}

PRE(sys_msgrcv)
{
   PRINT("sys_msgrcv ( %ld, %#lx, %ld, %ld, %ld )",ARG1,ARG2,ARG3,ARG4,ARG5);
   PRE_REG_READ5(long, "msgrcv",
                 int, msqid, struct msgbuf *, msgp, vki_size_t, msgsz,
                 long, msgytp, int, msgflg);
   ML_(linux_PRE_sys_msgrcv)(tid, ARG1,ARG2,ARG3,ARG4,ARG5);
   if ((ARG4 & VKI_IPC_NOWAIT) == 0)
      *flags |= SfMayBlock;
}
POST(sys_msgrcv)
{
   ML_(linux_POST_sys_msgrcv)(tid, RES,ARG1,ARG2,ARG3,ARG4,ARG5);
}


PRE(sys_msgctl)
{
   PRINT("sys_msgctl ( %ld, %ld, %#lx )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "msgctl",
                 int, msqid, int, cmd, struct msqid_ds *, buf);
   ML_(linux_PRE_sys_msgctl)(tid, ARG1,ARG2,ARG3);
}
POST(sys_msgctl)
{
   ML_(linux_POST_sys_msgctl)(tid, RES,ARG1,ARG2,ARG3);
}

PRE(sys_shmget)
{
   PRINT("sys_shmget ( %ld, %ld, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "shmget", vki_key_t, key, vki_size_t, size, int, shmflg);
}

PRE(wrap_sys_shmat)
{
   UWord arg2tmp;
   PRINT("wrap_sys_shmat ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "shmat",
                 int, shmid, const void *, shmaddr, int, shmflg);
   if (ARG3 & VKI_SHM_RND)
      ARG2 = VG_ROUNDDN(ARG2, VKI_SHMLBA);
   arg2tmp = ML_(generic_PRE_sys_shmat)(tid, ARG1,ARG2,ARG3);
   if (arg2tmp == 0)
      SET_STATUS_Failure( VKI_EINVAL );
   else
      ARG2 = arg2tmp;
}

POST(wrap_sys_shmat)
{
   ML_(generic_POST_sys_shmat)(tid, RES,ARG1,ARG2,ARG3);
}

PRE(sys_shmdt)
{
   PRINT("sys_shmdt ( %#lx )",ARG1);
   PRE_REG_READ1(long, "shmdt", const void *, shmaddr);
   if (!ML_(generic_PRE_sys_shmdt)(tid, ARG1))
      SET_STATUS_Failure( VKI_EINVAL );
}

POST(sys_shmdt)
{
   ML_(generic_POST_sys_shmdt)(tid, RES,ARG1);
}

PRE(sys_shmctl)
{
   PRINT("sys_shmctl ( %ld, %ld, %#lx )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "shmctl",
                 int, shmid, int, cmd, struct shmid_ds *, buf);
   ML_(generic_PRE_sys_shmctl)(tid, ARG1,ARG2,ARG3);
}

POST(sys_shmctl)
{
   ML_(generic_POST_sys_shmctl)(tid, RES,ARG1,ARG2,ARG3);
}

PRE(sys_shutdown)
{
   *flags |= SfMayBlock;
   PRINT("sys_shutdown ( %ld, %ld )",ARG1,ARG2);
   PRE_REG_READ2(int, "shutdown", int, s, int, how);
}

PRE(sys_bind)
{
   PRINT("sys_bind ( %ld, %#lx, %ld )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "bind",
                 int, sockfd, struct sockaddr *, my_addr, int, addrlen);
   ML_(generic_PRE_sys_bind)(tid, ARG1,ARG2,ARG3);
}

PRE(sys_listen)
{
   PRINT("sys_listen ( %ld, %ld )",ARG1,ARG2);
   PRE_REG_READ2(long, "listen", int, s, int, backlog);
}

PRE(sys_getsockname)
{
   PRINT("sys_getsockname ( %ld, %#lx, %#lx )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "getsockname",
                 int, s, struct sockaddr *, name, int *, namelen);
   ML_(generic_PRE_sys_getsockname)(tid, ARG1,ARG2,ARG3);
}
POST(sys_getsockname)
{
   vg_assert(SUCCESS);
   ML_(generic_POST_sys_getsockname)(tid, VG_(mk_SysRes_Success)(RES),
                                          ARG1,ARG2,ARG3);
}

PRE(sys_getpeername)
{
   PRINT("sys_getpeername ( %ld, %#lx, %#lx )",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "getpeername",
                 int, s, struct sockaddr *, name, int *, namelen);
   ML_(generic_PRE_sys_getpeername)(tid, ARG1,ARG2,ARG3);
}
POST(sys_getpeername)
{
   vg_assert(SUCCESS);
   ML_(generic_POST_sys_getpeername)(tid, VG_(mk_SysRes_Success)(RES),
                                          ARG1,ARG2,ARG3);
}

PRE(sys_socketpair)
{
   PRINT("sys_socketpair ( %ld, %ld, %ld, %#lx )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "socketpair",
                 int, d, int, type, int, protocol, int*, sv);
   ML_(generic_PRE_sys_socketpair)(tid, ARG1,ARG2,ARG3,ARG4);
}
POST(sys_socketpair)
{
   vg_assert(SUCCESS);
   ML_(generic_POST_sys_socketpair)(tid, VG_(mk_SysRes_Success)(RES),
                                         ARG1,ARG2,ARG3,ARG4);
}

PRE(sys_send)
{
   *flags |= SfMayBlock;
   PRINT("sys_send ( %ld, %#lx, %ld, %lu )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "send",
                 int, s, const void *, msg, int, len, 
                 unsigned int, flags);

   ML_(generic_PRE_sys_send)( tid, ARG1, ARG2, ARG3 );
}

PRE(sys_recv)
{
   *flags |= SfMayBlock;
   PRINT("sys_recv ( %ld, %#lx, %ld, %lu )",ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(long, "recv",
                 int, s, void *, buf, int, len, unsigned int, flags);
   ML_(generic_PRE_sys_recv)( tid, ARG1, ARG2, ARG3 );
}

POST(sys_recv)
{
   ML_(generic_POST_sys_recv)( tid, RES, ARG1, ARG2, ARG3 );
}

PRE(sys_mmap2)
{
   SysRes r;

   
   
   
   
   
   
   vg_assert(VKI_PAGE_SIZE == 4096);
   PRINT("sys_mmap2 ( %#lx, %llu, %ld, %ld, %ld, %ld )",
         ARG1, (ULong)ARG2, ARG3, ARG4, ARG5, ARG6 );
   PRE_REG_READ6(long, "mmap2",
                 unsigned long, start, unsigned long, length,
                 unsigned long, prot,  unsigned long, flags,
                 unsigned long, fd,    unsigned long, offset);

   r = ML_(generic_PRE_sys_mmap)( tid, ARG1, ARG2, ARG3, ARG4, ARG5, 
                                       4096 * (Off64T)ARG6 );
   SET_STATUS_from_SysRes(r);
}

PRE(sys_lstat64)
{
   PRINT("sys_lstat64 ( %#lx(%s), %#lx )",ARG1,(char*)ARG1,ARG2);
   PRE_REG_READ2(long, "lstat64", char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "lstat64(file_name)", ARG1 );
   PRE_MEM_WRITE( "lstat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_lstat64)
{
   vg_assert(SUCCESS);
   if (RES == 0) {
      POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
   }
}

PRE(sys_stat64)
{
   PRINT("sys_stat64 ( %#lx(%s), %#lx )",ARG1,(char*)ARG1,ARG2);
   PRE_REG_READ2(long, "stat64", char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "stat64(file_name)", ARG1 );
   PRE_MEM_WRITE( "stat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_stat64)
{
   POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
}

PRE(sys_fstatat64)
{
   PRINT("sys_fstatat64 ( %ld, %#lx(%s), %#lx )",ARG1,ARG2,(char*)ARG2,ARG3);
   PRE_REG_READ3(long, "fstatat64",
                 int, dfd, char *, file_name, struct stat64 *, buf);
   PRE_MEM_RASCIIZ( "fstatat64(file_name)", ARG2 );
   PRE_MEM_WRITE( "fstatat64(buf)", ARG3, sizeof(struct vki_stat64) );
}

POST(sys_fstatat64)
{
   POST_MEM_WRITE( ARG3, sizeof(struct vki_stat64) );
}

PRE(sys_fstat64)
{
   PRINT("sys_fstat64 ( %ld, %#lx )",ARG1,ARG2);
   PRE_REG_READ2(long, "fstat64", unsigned long, fd, struct stat64 *, buf);
   PRE_MEM_WRITE( "fstat64(buf)", ARG2, sizeof(struct vki_stat64) );
}

POST(sys_fstat64)
{
   POST_MEM_WRITE( ARG2, sizeof(struct vki_stat64) );
}

PRE(sys_clone)
{
    UInt cloneflags;

   PRINT("sys_clone ( %lx, %#lx, %#lx, %#lx, %#lx )",ARG1,ARG2,ARG3,ARG4,ARG5);
   PRE_REG_READ5(int, "clone",
                 unsigned long, flags,
                 void *, child_stack,
                 int *, parent_tidptr,
                 void *, child_tls,
                 int *, child_tidptr);

   if (ARG1 & VKI_CLONE_PARENT_SETTID) {
      PRE_MEM_WRITE("clone(parent_tidptr)", ARG3, sizeof(Int));
      if (!VG_(am_is_valid_for_client)(ARG3, sizeof(Int), 
                                             VKI_PROT_WRITE)) {
         SET_STATUS_Failure( VKI_EFAULT );
         return;
      }
   }
   if (ARG1 & (VKI_CLONE_CHILD_SETTID | VKI_CLONE_CHILD_CLEARTID)) {
      PRE_MEM_WRITE("clone(child_tidptr)", ARG5, sizeof(Int));
      if (!VG_(am_is_valid_for_client)(ARG5, sizeof(Int), 
                                             VKI_PROT_WRITE)) {
         SET_STATUS_Failure( VKI_EFAULT );
         return;
      }
   }
   if (ARG1 & VKI_CLONE_SETTLS) {
      PRE_MEM_READ("clone(tls_user_desc)", ARG4, sizeof(vki_modify_ldt_t));
      if (!VG_(am_is_valid_for_client)(ARG4, sizeof(vki_modify_ldt_t), 
                                             VKI_PROT_READ)) {
         SET_STATUS_Failure( VKI_EFAULT );
         return;
      }
   }

   cloneflags = ARG1;

   if (!ML_(client_signal_OK)(ARG1 & VKI_CSIGNAL)) {
      SET_STATUS_Failure( VKI_EINVAL );
      return;
   }

   
   switch (cloneflags & (VKI_CLONE_VM | VKI_CLONE_FS 
                         | VKI_CLONE_FILES | VKI_CLONE_VFORK)) {
   case VKI_CLONE_VM | VKI_CLONE_FS | VKI_CLONE_FILES:
      
      SET_STATUS_from_SysRes(
         do_clone(tid,
                  ARG1,         
                  (Addr)ARG2,   
                  (Int *)ARG3,  
                  (Int *)ARG5,  
                  (Addr)ARG4)); 
      break;

   case VKI_CLONE_VFORK | VKI_CLONE_VM: 
      
      cloneflags &= ~(VKI_CLONE_VFORK | VKI_CLONE_VM);

   case 0: 
      SET_STATUS_from_SysRes(
         ML_(do_fork_clone)(tid,
                       cloneflags,      
                       (Int *)ARG3,     
                       (Int *)ARG5));   
      break;

   default:
      
      VG_(message)(Vg_UserMsg, "");
      VG_(message)(Vg_UserMsg, "Unsupported clone() flags: 0x%lx", ARG1);
      VG_(message)(Vg_UserMsg, "");
      VG_(message)(Vg_UserMsg, "The only supported clone() uses are:");
      VG_(message)(Vg_UserMsg, " - via a threads library (LinuxThreads or NPTL)");
      VG_(message)(Vg_UserMsg, " - via the implementation of fork or vfork");
      VG_(message)(Vg_UserMsg, " - for the Quadrics Elan3 user-space driver");
      VG_(unimplemented)
         ("Valgrind does not support general clone().");
   }

   if (SUCCESS) {
      if (ARG1 & VKI_CLONE_PARENT_SETTID)
         POST_MEM_WRITE(ARG3, sizeof(Int));
      if (ARG1 & (VKI_CLONE_CHILD_SETTID | VKI_CLONE_CHILD_CLEARTID))
         POST_MEM_WRITE(ARG5, sizeof(Int));

      *flags |= SfYieldAfter;
   }
}

PRE(sys_sigreturn)
{

   PRINT("sys_sigreturn ( )");

   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(tid >= 1 && tid < VG_N_THREADS);
   vg_assert(VG_(is_running_thread)(tid));

   
   VG_(sigframe_destroy)(tid, False);

   *flags |= SfNoWriteResult;
   SET_STATUS_Success(0);

   
   *flags |= SfPollAfter;
}

PRE(sys_rt_sigreturn)
{

   PRINT("rt_sigreturn ( )");

   vg_assert(VG_(is_valid_tid)(tid));
   vg_assert(tid >= 1 && tid < VG_N_THREADS);
   vg_assert(VG_(is_running_thread)(tid));

   
   VG_(sigframe_destroy)(tid, True);

   *flags |= SfNoWriteResult;
   SET_STATUS_Success(0);

   
   *flags |= SfPollAfter;
}

PRE(sys_sigsuspend)
{
   *flags |= SfMayBlock;
   PRINT("sys_sigsuspend ( %ld, %ld, %ld )", ARG1,ARG2,ARG3 );
   PRE_REG_READ3(int, "sigsuspend",
                 int, history0, int, history1,
                 vki_old_sigset_t, mask);
}


PRE(sys_set_tls)
{
   PRINT("set_tls (%lx)",ARG1);
   PRE_REG_READ1(long, "set_tls", unsigned long, addr);

   SET_STATUS_from_SysRes( sys_set_tls( tid, ARG1 ) );
}

PRE(sys_cacheflush)
{
   PRINT("cacheflush (%lx, %#lx, %#lx)",ARG1,ARG2,ARG3);
   PRE_REG_READ3(long, "cacheflush", void*, addrlow,void*, addrhigh,int, flags);
   VG_(discard_translations)( (Addr64)ARG1,
                              ((ULong)ARG2) - ((ULong)ARG1) + 1ULL,
                              "PRE(sys_cacheflush)" );
   SET_STATUS_Success(0);
}

PRE(sys_ptrace)
{
   PRINT("sys_ptrace ( %ld, %ld, %#lx, %#lx )", ARG1,ARG2,ARG3,ARG4);
   PRE_REG_READ4(int, "ptrace", 
                 long, request, long, pid, long, addr, long, data);
   switch (ARG1) {
   case VKI_PTRACE_PEEKTEXT:
   case VKI_PTRACE_PEEKDATA:
   case VKI_PTRACE_PEEKUSR:
      PRE_MEM_WRITE( "ptrace(peek)", ARG4, 
		     sizeof (long));
      break;
   case VKI_PTRACE_GETREGS:
      PRE_MEM_WRITE( "ptrace(getregs)", ARG4, 
		     sizeof (struct vki_user_regs_struct));
      break;
   case VKI_PTRACE_GETFPREGS:
      PRE_MEM_WRITE( "ptrace(getfpregs)", ARG4, 
		     sizeof (struct vki_user_fp));
      break;
   case VKI_PTRACE_GETWMMXREGS:
      PRE_MEM_WRITE( "ptrace(getwmmxregs)", ARG4, 
		     VKI_IWMMXT_SIZE);
      break;
   case VKI_PTRACE_GETCRUNCHREGS:
      PRE_MEM_WRITE( "ptrace(getcrunchregs)", ARG4, 
		     VKI_CRUNCH_SIZE);
      break;
   case VKI_PTRACE_GETVFPREGS:
      PRE_MEM_WRITE( "ptrace(getvfpregs)", ARG4, 
                     sizeof (struct vki_user_vfp) );
      break;
   case VKI_PTRACE_GETHBPREGS:
      PRE_MEM_WRITE( "ptrace(gethbpregs)", ARG4, 
                     sizeof (unsigned long) );
      break;
   case VKI_PTRACE_SETREGS:
      PRE_MEM_READ( "ptrace(setregs)", ARG4, 
		     sizeof (struct vki_user_regs_struct));
      break;
   case VKI_PTRACE_SETFPREGS:
      PRE_MEM_READ( "ptrace(setfpregs)", ARG4, 
		     sizeof (struct vki_user_fp));
      break;
   case VKI_PTRACE_SETWMMXREGS:
      PRE_MEM_READ( "ptrace(setwmmxregs)", ARG4, 
		     VKI_IWMMXT_SIZE);
      break;
   case VKI_PTRACE_SETCRUNCHREGS:
      PRE_MEM_READ( "ptrace(setcrunchregs)", ARG4, 
		     VKI_CRUNCH_SIZE);
      break;
   case VKI_PTRACE_SETVFPREGS:
      PRE_MEM_READ( "ptrace(setvfpregs)", ARG4, 
                     sizeof (struct vki_user_vfp));
      break;
   case VKI_PTRACE_SETHBPREGS:
      PRE_MEM_READ( "ptrace(sethbpregs)", ARG4, sizeof(unsigned long));
      break;
   case VKI_PTRACE_GET_THREAD_AREA:
      PRE_MEM_WRITE( "ptrace(get_thread_area)", ARG4, sizeof(unsigned long));
      break;
   case VKI_PTRACE_GETEVENTMSG:
      PRE_MEM_WRITE( "ptrace(geteventmsg)", ARG4, sizeof(unsigned long));
      break;
   case VKI_PTRACE_GETSIGINFO:
      PRE_MEM_WRITE( "ptrace(getsiginfo)", ARG4, sizeof(vki_siginfo_t));
      break;
   case VKI_PTRACE_SETSIGINFO:
      PRE_MEM_READ( "ptrace(setsiginfo)", ARG4, sizeof(vki_siginfo_t));
      break;
   default:
      break;
   }
}

POST(sys_ptrace)
{
   switch (ARG1) {
   case VKI_PTRACE_PEEKTEXT:
   case VKI_PTRACE_PEEKDATA:
   case VKI_PTRACE_PEEKUSR:
      POST_MEM_WRITE( ARG4, sizeof (long));
      break;
   case VKI_PTRACE_GETREGS:
      POST_MEM_WRITE( ARG4, sizeof (struct vki_user_regs_struct));
      break;
   case VKI_PTRACE_GETFPREGS:
      POST_MEM_WRITE( ARG4, sizeof (struct vki_user_fp));
      break;
   case VKI_PTRACE_GETWMMXREGS:
      POST_MEM_WRITE( ARG4, VKI_IWMMXT_SIZE);
      break;
   case VKI_PTRACE_GETCRUNCHREGS:
      POST_MEM_WRITE( ARG4, VKI_CRUNCH_SIZE);
      break;
   case VKI_PTRACE_GETVFPREGS:
      POST_MEM_WRITE( ARG4, sizeof(struct vki_user_vfp));
      break;
   case VKI_PTRACE_GET_THREAD_AREA:
   case VKI_PTRACE_GETHBPREGS:
   case VKI_PTRACE_GETEVENTMSG:
      POST_MEM_WRITE( ARG4, sizeof(unsigned long));
      break;
   case VKI_PTRACE_GETSIGINFO:
      POST_MEM_WRITE( ARG4, sizeof(vki_siginfo_t));
      break;
   default:
      break;
   }
}

#undef PRE
#undef POST


#if 0
#define __NR_OABI_SYSCALL_BASE 0x900000
#else
#define __NR_OABI_SYSCALL_BASE 0x0
#endif

#define PLAX_(sysno, name)    WRAPPER_ENTRY_X_(arm_linux, sysno, name) 
#define PLAXY(sysno, name)    WRAPPER_ENTRY_XY(arm_linux, sysno, name)


static SyscallTableEntry syscall_main_table[] = {
   GENX_(__NR_exit,              sys_exit),           
   GENX_(__NR_fork,              sys_fork),           
   GENXY(__NR_read,              sys_read),           
   GENX_(__NR_write,             sys_write),          

   GENXY(__NR_open,              sys_open),           
   GENXY(__NR_close,             sys_close),          
   GENXY(__NR_creat,             sys_creat),          
   GENX_(__NR_link,              sys_link),           

   GENX_(__NR_unlink,            sys_unlink),         
   GENX_(__NR_execve,            sys_execve),         
   GENX_(__NR_chdir,             sys_chdir),          
   GENXY(__NR_time,              sys_time),           
   GENX_(__NR_mknod,             sys_mknod),          

   GENX_(__NR_chmod,             sys_chmod),          
   LINX_(__NR_lseek,             sys_lseek),          

   GENX_(__NR_getpid,            sys_getpid),         
   LINX_(__NR_mount,             sys_mount),          
   LINX_(__NR_umount,            sys_oldumount),      
   LINX_(__NR_setuid,            sys_setuid16),       
   LINX_(__NR_getuid,            sys_getuid16),       
   PLAXY(__NR_ptrace,            sys_ptrace),         
   GENX_(__NR_alarm,             sys_alarm),          
   GENX_(__NR_pause,             sys_pause),          

   LINX_(__NR_utime,             sys_utime),          
   GENX_(__NR_access,            sys_access),         
   GENX_(__NR_nice,              sys_nice),           

   GENX_(__NR_sync,              sys_sync),           
   GENX_(__NR_kill,              sys_kill),           
   GENX_(__NR_rename,            sys_rename),         
   GENX_(__NR_mkdir,             sys_mkdir),          

   GENX_(__NR_rmdir,             sys_rmdir),          
   GENXY(__NR_dup,               sys_dup),            
   LINXY(__NR_pipe,              sys_pipe),           
   GENXY(__NR_times,             sys_times),          
   GENX_(__NR_brk,               sys_brk),            
   LINX_(__NR_setgid,            sys_setgid16),       
   LINX_(__NR_getgid,            sys_getgid16),       
   LINX_(__NR_geteuid,           sys_geteuid16),      

   LINX_(__NR_getegid,           sys_getegid16),      
   GENX_(__NR_acct,              sys_acct),           
   LINX_(__NR_umount2,           sys_umount),         
   LINXY(__NR_ioctl,             sys_ioctl),          

   LINXY(__NR_fcntl,             sys_fcntl),          
   GENX_(__NR_setpgid,           sys_setpgid),        
   GENX_(__NR_umask,             sys_umask),          
   GENX_(__NR_chroot,            sys_chroot),         
   GENXY(__NR_dup2,              sys_dup2),           
   GENX_(__NR_getppid,           sys_getppid),        

   GENX_(__NR_getpgrp,           sys_getpgrp),        
   GENX_(__NR_setsid,            sys_setsid),         
   LINXY(__NR_sigaction,         sys_sigaction),      
   LINX_(__NR_setreuid,          sys_setreuid16),     
   LINX_(__NR_setregid,          sys_setregid16),     
   PLAX_(__NR_sigsuspend,        sys_sigsuspend),     
   LINXY(__NR_sigpending,        sys_sigpending),     
   GENX_(__NR_setrlimit,         sys_setrlimit),      
   GENXY(__NR_getrlimit,         sys_old_getrlimit),  
   GENXY(__NR_getrusage,         sys_getrusage),      
   GENXY(__NR_gettimeofday,      sys_gettimeofday),   
   GENX_(__NR_settimeofday,      sys_settimeofday),   

   LINXY(__NR_getgroups,         sys_getgroups16),    
   LINX_(__NR_setgroups,         sys_setgroups16),    
   GENX_(__NR_symlink,           sys_symlink),        
   GENX_(__NR_readlink,          sys_readlink),       
   GENXY(__NR_munmap,            sys_munmap),         
   GENX_(__NR_truncate,          sys_truncate),       
   GENX_(__NR_ftruncate,         sys_ftruncate),      
   GENX_(__NR_fchmod,            sys_fchmod),         

   LINX_(__NR_fchown,            sys_fchown16),       
   GENX_(__NR_getpriority,       sys_getpriority),    
   GENX_(__NR_setpriority,       sys_setpriority),    
   GENXY(__NR_statfs,            sys_statfs),         

   GENXY(__NR_fstatfs,           sys_fstatfs),        
   PLAXY(__NR_socketcall,        sys_socketcall),     
   LINXY(__NR_syslog,            sys_syslog),         
   GENXY(__NR_setitimer,         sys_setitimer),      

   GENXY(__NR_getitimer,         sys_getitimer),      
   GENXY(__NR_stat,              sys_newstat),        
   GENXY(__NR_lstat,             sys_newlstat),       
   GENXY(__NR_fstat,             sys_newfstat),       
   LINX_(__NR_vhangup,           sys_vhangup),        
   GENXY(__NR_wait4,             sys_wait4),          
   LINXY(__NR_sysinfo,           sys_sysinfo),        
   GENX_(__NR_fsync,             sys_fsync),          
   PLAX_(__NR_sigreturn,         sys_sigreturn),      

   PLAX_(__NR_clone,             sys_clone),          
   GENXY(__NR_uname,             sys_newuname),       
   GENXY(__NR_mprotect,          sys_mprotect),       
   LINXY(__NR_sigprocmask,       sys_sigprocmask),    
   LINX_(__NR_init_module,       sys_init_module),    
   LINX_(__NR_delete_module,     sys_delete_module),  
   LINX_(__NR_quotactl,          sys_quotactl),       
   GENX_(__NR_getpgid,           sys_getpgid),        
   GENX_(__NR_fchdir,            sys_fchdir),         
   LINX_(__NR_personality,       sys_personality),    
   LINX_(__NR_setfsuid,          sys_setfsuid16),     
   LINX_(__NR_setfsgid,          sys_setfsgid16),     
 
   LINXY(__NR__llseek,           sys_llseek),         
   GENXY(__NR_getdents,          sys_getdents),       
   GENX_(__NR__newselect,        sys_select),         
   GENX_(__NR_flock,             sys_flock),          
   GENX_(__NR_msync,             sys_msync),          

   GENXY(__NR_readv,             sys_readv),          
   GENX_(__NR_writev,            sys_writev),         
   GENX_(__NR_getsid,            sys_getsid),         
   GENX_(__NR_fdatasync,         sys_fdatasync),      
   LINXY(__NR__sysctl,           sys_sysctl),         

   GENX_(__NR_mlock,             sys_mlock),          
   GENX_(__NR_munlock,           sys_munlock),        
   GENX_(__NR_mlockall,          sys_mlockall),       
   LINX_(__NR_munlockall,        sys_munlockall),     
   LINXY(__NR_sched_setparam,    sys_sched_setparam), 

   LINXY(__NR_sched_getparam,         sys_sched_getparam),        
   LINX_(__NR_sched_setscheduler,     sys_sched_setscheduler),    
   LINX_(__NR_sched_getscheduler,     sys_sched_getscheduler),    
   LINX_(__NR_sched_yield,            sys_sched_yield),           
   LINX_(__NR_sched_get_priority_max, sys_sched_get_priority_max),

   LINX_(__NR_sched_get_priority_min, sys_sched_get_priority_min),
   GENXY(__NR_nanosleep,         sys_nanosleep),      
   GENX_(__NR_mremap,            sys_mremap),         
   LINX_(__NR_setresuid,         sys_setresuid16),    

   LINXY(__NR_getresuid,         sys_getresuid16),    
   GENXY(__NR_poll,              sys_poll),           
   LINX_(__NR_setresgid,         sys_setresgid16),    
   LINXY(__NR_getresgid,         sys_getresgid16),    
   LINXY(__NR_prctl,             sys_prctl),          
   PLAX_(__NR_rt_sigreturn,      sys_rt_sigreturn),   
   LINXY(__NR_rt_sigaction,      sys_rt_sigaction),   

   LINXY(__NR_rt_sigprocmask,    sys_rt_sigprocmask), 
   LINXY(__NR_rt_sigpending,     sys_rt_sigpending),  
   LINXY(__NR_rt_sigtimedwait,   sys_rt_sigtimedwait),
   LINXY(__NR_rt_sigqueueinfo,   sys_rt_sigqueueinfo),
   LINX_(__NR_rt_sigsuspend,     sys_rt_sigsuspend),  

   GENXY(__NR_pread64,           sys_pread64),        
   GENX_(__NR_pwrite64,          sys_pwrite64),       
   LINX_(__NR_chown,             sys_chown16),        
   GENXY(__NR_getcwd,            sys_getcwd),         
   LINXY(__NR_capget,            sys_capget),         

   LINX_(__NR_capset,            sys_capset),         
   GENXY(__NR_sigaltstack,       sys_sigaltstack),    
   LINXY(__NR_sendfile,          sys_sendfile),       

   
   GENX_(__NR_vfork,             sys_fork),           
   GENXY(__NR_ugetrlimit,        sys_getrlimit),      
   PLAX_(__NR_mmap2,             sys_mmap2),          
   GENX_(__NR_truncate64,        sys_truncate64),     
   GENX_(__NR_ftruncate64,       sys_ftruncate64),    
   
   PLAXY(__NR_stat64,            sys_stat64),         
   PLAXY(__NR_lstat64,           sys_lstat64),        
   PLAXY(__NR_fstat64,           sys_fstat64),        
   GENX_(__NR_lchown32,          sys_lchown),         
   GENX_(__NR_getuid32,          sys_getuid),         

   GENX_(__NR_getgid32,          sys_getgid),         
   GENX_(__NR_geteuid32,         sys_geteuid),        
   GENX_(__NR_getegid32,         sys_getegid),        
   GENX_(__NR_setreuid32,        sys_setreuid),       
   GENX_(__NR_setregid32,        sys_setregid),       

   GENXY(__NR_getgroups32,       sys_getgroups),      
   GENX_(__NR_setgroups32,       sys_setgroups),      
   GENX_(__NR_fchown32,          sys_fchown),         
   LINX_(__NR_setresuid32,       sys_setresuid),      
   LINXY(__NR_getresuid32,       sys_getresuid),      

   LINX_(__NR_setresgid32,       sys_setresgid),      
   LINXY(__NR_getresgid32,       sys_getresgid),      
   GENX_(__NR_chown32,           sys_chown),          
   GENX_(__NR_setuid32,          sys_setuid),         
   GENX_(__NR_setgid32,          sys_setgid),         

   LINX_(__NR_setfsuid32,        sys_setfsuid),       
   LINX_(__NR_setfsgid32,        sys_setfsgid),       
   GENXY(__NR_mincore,           sys_mincore),        
   GENX_(__NR_madvise,           sys_madvise),        

   GENXY(__NR_getdents64,        sys_getdents64),     
   LINXY(__NR_fcntl64,           sys_fcntl64),        
   LINX_(__NR_gettid,            sys_gettid),         

   LINX_(__NR_readahead,         sys_readahead),      
   LINX_(__NR_setxattr,          sys_setxattr),       
   LINX_(__NR_lsetxattr,         sys_lsetxattr),      
   LINX_(__NR_fsetxattr,         sys_fsetxattr),      
   LINXY(__NR_getxattr,          sys_getxattr),       

   LINXY(__NR_lgetxattr,         sys_lgetxattr),      
   LINXY(__NR_fgetxattr,         sys_fgetxattr),      
   LINXY(__NR_listxattr,         sys_listxattr),      
   LINXY(__NR_llistxattr,        sys_llistxattr),     
   LINXY(__NR_flistxattr,        sys_flistxattr),     

   LINX_(__NR_removexattr,       sys_removexattr),    
   LINX_(__NR_lremovexattr,      sys_lremovexattr),   
   LINX_(__NR_fremovexattr,      sys_fremovexattr),   
   LINXY(__NR_tkill,             sys_tkill),          
   LINXY(__NR_sendfile64,        sys_sendfile64),     

   LINXY(__NR_futex,             sys_futex),             
   LINX_(__NR_sched_setaffinity, sys_sched_setaffinity), 
   LINXY(__NR_sched_getaffinity, sys_sched_getaffinity), 

   LINXY(__NR_io_setup,          sys_io_setup),       
   LINX_(__NR_io_destroy,        sys_io_destroy),     
   LINXY(__NR_io_getevents,      sys_io_getevents),   
   LINX_(__NR_io_submit,         sys_io_submit),      
   LINXY(__NR_io_cancel,         sys_io_cancel),      

   GENX_(251,                    sys_ni_syscall),     
   LINX_(__NR_exit_group,        sys_exit_group),     
   LINXY(__NR_epoll_create,      sys_epoll_create),   

   LINX_(__NR_epoll_ctl,         sys_epoll_ctl),         
   LINXY(__NR_epoll_wait,        sys_epoll_wait),        
   LINX_(__NR_set_tid_address,   sys_set_tid_address),   
   LINXY(__NR_timer_create,      sys_timer_create),      

   LINXY(__NR_timer_settime,     sys_timer_settime),  
   LINXY(__NR_timer_gettime,     sys_timer_gettime),  
   LINX_(__NR_timer_getoverrun,  sys_timer_getoverrun),
   LINX_(__NR_timer_delete,      sys_timer_delete),   
   LINX_(__NR_clock_settime,     sys_clock_settime),  

   LINXY(__NR_clock_gettime,     sys_clock_gettime),  
   LINXY(__NR_clock_getres,      sys_clock_getres),   
   LINXY(__NR_clock_nanosleep,   sys_clock_nanosleep),
   GENXY(__NR_statfs64,          sys_statfs64),       
   GENXY(__NR_fstatfs64,         sys_fstatfs64),      

   LINX_(__NR_tgkill,            sys_tgkill),         
   GENX_(__NR_utimes,            sys_utimes),         
   GENX_(__NR_vserver,           sys_ni_syscall),     
   LINX_(__NR_mbind,             sys_mbind),          

   LINXY(__NR_get_mempolicy,     sys_get_mempolicy),  
   LINX_(__NR_set_mempolicy,     sys_set_mempolicy),  
   LINXY(__NR_mq_open,           sys_mq_open),        
   LINX_(__NR_mq_unlink,         sys_mq_unlink),      
   LINX_(__NR_mq_timedsend,      sys_mq_timedsend),   

   LINXY(__NR_mq_timedreceive,   sys_mq_timedreceive),
   LINX_(__NR_mq_notify,         sys_mq_notify),      
   LINXY(__NR_mq_getsetattr,     sys_mq_getsetattr),  
   LINXY(__NR_waitid,            sys_waitid),         

   PLAXY(__NR_socket,            sys_socket),         
   PLAX_(__NR_bind,              sys_bind),           
   PLAX_(__NR_connect,           sys_connect),        
   PLAX_(__NR_listen,            sys_listen),         
   PLAXY(__NR_accept,            sys_accept),         
   PLAXY(__NR_getsockname,       sys_getsockname),    
   PLAXY(__NR_getpeername,       sys_getpeername),    
   PLAXY(__NR_socketpair,        sys_socketpair),     
   PLAX_(__NR_send,              sys_send),
   PLAX_(__NR_sendto,            sys_sendto),         
   PLAXY(__NR_recv,              sys_recv),
   PLAXY(__NR_recvfrom,          sys_recvfrom),       
   PLAX_(__NR_shutdown,          sys_shutdown),       
   PLAX_(__NR_setsockopt,        sys_setsockopt),     
   PLAXY(__NR_getsockopt,        sys_getsockopt),     
   PLAX_(__NR_sendmsg,           sys_sendmsg),        
   PLAXY(__NR_recvmsg,           sys_recvmsg),        
   PLAX_(__NR_semop,             sys_semop),          
   PLAX_(__NR_semget,            sys_semget),         
   PLAXY(__NR_semctl,            sys_semctl),         
   PLAX_(__NR_msgget,            sys_msgget),         
   PLAX_(__NR_msgsnd,            sys_msgsnd),          
   PLAXY(__NR_msgrcv,            sys_msgrcv),         
   PLAXY(__NR_msgctl,            sys_msgctl),         
   PLAX_(__NR_semtimedop,        sys_semtimedop),     

   LINX_(__NR_add_key,           sys_add_key),        
   LINX_(__NR_request_key,       sys_request_key),    
   LINXY(__NR_keyctl,            sys_keyctl),         

   LINX_(__NR_inotify_init,    sys_inotify_init),   
   LINX_(__NR_inotify_add_watch, sys_inotify_add_watch), 
   LINX_(__NR_inotify_rm_watch,    sys_inotify_rm_watch), 

   LINXY(__NR_openat,       sys_openat),           
   LINX_(__NR_mkdirat,       sys_mkdirat),          
   LINX_(__NR_mknodat,       sys_mknodat),          
   LINX_(__NR_fchownat,       sys_fchownat),         
   LINX_(__NR_futimesat,    sys_futimesat),        

   PLAXY(__NR_fstatat64,    sys_fstatat64),        
   LINX_(__NR_unlinkat,       sys_unlinkat),         
   LINX_(__NR_renameat,       sys_renameat),         
   LINX_(__NR_linkat,       sys_linkat),           
   LINX_(__NR_symlinkat,    sys_symlinkat),        

   LINX_(__NR_readlinkat,    sys_readlinkat),       
   LINX_(__NR_fchmodat,       sys_fchmodat),         
   LINX_(__NR_faccessat,    sys_faccessat),        
   PLAXY(__NR_shmat,         wrap_sys_shmat),       
   PLAXY(__NR_shmdt,             sys_shmdt),          
   PLAX_(__NR_shmget,            sys_shmget),         
   PLAXY(__NR_shmctl,            sys_shmctl),         

   LINX_(__NR_set_robust_list,    sys_set_robust_list),  
   LINXY(__NR_get_robust_list,    sys_get_robust_list),  

   LINXY(__NR_move_pages,        sys_move_pages),       

   LINX_(__NR_utimensat,         sys_utimensat),        
   LINXY(__NR_signalfd,          sys_signalfd),         
   LINXY(__NR_timerfd_create,    sys_timerfd_create),   
   LINX_(__NR_eventfd,           sys_eventfd),          

   LINXY(__NR_timerfd_settime,   sys_timerfd_settime),  
   LINXY(__NR_timerfd_gettime,   sys_timerfd_gettime),   

   

   
   
   
   
   
   

   LINX_(__NR_pselect6,          sys_pselect6),         
   LINXY(__NR_ppoll,             sys_ppoll),            

   LINXY(__NR_epoll_pwait,       sys_epoll_pwait),      

   LINX_(__NR_fallocate,         sys_fallocate),        

   LINXY(__NR_signalfd4,         sys_signalfd4),        
   LINX_(__NR_eventfd2,          sys_eventfd2),         
   LINXY(__NR_epoll_create1,     sys_epoll_create1),    
   LINXY(__NR_dup3,              sys_dup3),             
   LINXY(__NR_pipe2,             sys_pipe2),            
   LINXY(__NR_inotify_init1,     sys_inotify_init1),    
   LINXY(__NR_preadv,            sys_preadv),           
   LINX_(__NR_pwritev,           sys_pwritev),          
   LINXY(__NR_rt_tgsigqueueinfo, sys_rt_tgsigqueueinfo),
   LINXY(__NR_perf_event_open,   sys_perf_event_open),  

   PLAXY(__NR_accept4,           sys_accept4)           
};



static SyscallTableEntry ste___ARM_set_tls
   = { WRAPPER_PRE_NAME(arm_linux,sys_set_tls), NULL };

static SyscallTableEntry ste___ARM_cacheflush
   = { WRAPPER_PRE_NAME(arm_linux,sys_cacheflush), NULL };

SyscallTableEntry* ML_(get_linux_syscall_entry) ( UInt sysno )
{
   const UInt syscall_main_table_size
      = sizeof(syscall_main_table) / sizeof(syscall_main_table[0]);

   
   if (sysno < syscall_main_table_size) {
      SyscallTableEntry* sys = &syscall_main_table[sysno];
      if (sys->before == NULL)
         return NULL; 
      else
         return sys;
   }

   
   switch (sysno) {
      case __NR_ARM_set_tls:    return &ste___ARM_set_tls;
      case __NR_ARM_cacheflush: return &ste___ARM_cacheflush;
      default: break;
   }

   
   return NULL;
}

#endif 

