
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
#include "pub_core_libcbase.h"
#include "pub_core_libcfile.h"
#include "pub_core_mallocfree.h"
#include "pub_core_machine.h"
#include "pub_core_cpuid.h"
#include "pub_core_libcsignal.h"   
#include "pub_core_debuglog.h"


#define INSTR_PTR(regs)    ((regs).vex.VG_INSTR_PTR)
#define STACK_PTR(regs)    ((regs).vex.VG_STACK_PTR)
#define FRAME_PTR(regs)    ((regs).vex.VG_FRAME_PTR)

Addr VG_(get_IP) ( ThreadId tid ) {
   return INSTR_PTR( VG_(threads)[tid].arch );
}
Addr VG_(get_SP) ( ThreadId tid ) {
   return STACK_PTR( VG_(threads)[tid].arch );
}
Addr VG_(get_FP) ( ThreadId tid ) {
   return FRAME_PTR( VG_(threads)[tid].arch );
}

void VG_(set_IP) ( ThreadId tid, Addr ip ) {
   INSTR_PTR( VG_(threads)[tid].arch ) = ip;
}
void VG_(set_SP) ( ThreadId tid, Addr sp ) {
   STACK_PTR( VG_(threads)[tid].arch ) = sp;
}

void VG_(get_UnwindStartRegs) ( UnwindStartRegs* regs,
                                ThreadId tid )
{
#  if defined(VGA_x86)
   regs->r_pc = (ULong)VG_(threads)[tid].arch.vex.guest_EIP;
   regs->r_sp = (ULong)VG_(threads)[tid].arch.vex.guest_ESP;
   regs->misc.X86.r_ebp
      = VG_(threads)[tid].arch.vex.guest_EBP;
#  elif defined(VGA_amd64)
   regs->r_pc = VG_(threads)[tid].arch.vex.guest_RIP;
   regs->r_sp = VG_(threads)[tid].arch.vex.guest_RSP;
   regs->misc.AMD64.r_rbp
      = VG_(threads)[tid].arch.vex.guest_RBP;
#  elif defined(VGA_ppc32)
   regs->r_pc = (ULong)VG_(threads)[tid].arch.vex.guest_CIA;
   regs->r_sp = (ULong)VG_(threads)[tid].arch.vex.guest_GPR1;
   regs->misc.PPC32.r_lr
      = VG_(threads)[tid].arch.vex.guest_LR;
#  elif defined(VGA_ppc64)
   regs->r_pc = VG_(threads)[tid].arch.vex.guest_CIA;
   regs->r_sp = VG_(threads)[tid].arch.vex.guest_GPR1;
   regs->misc.PPC64.r_lr
      = VG_(threads)[tid].arch.vex.guest_LR;
#  elif defined(VGA_arm)
   regs->r_pc = (ULong)VG_(threads)[tid].arch.vex.guest_R15T;
   regs->r_sp = (ULong)VG_(threads)[tid].arch.vex.guest_R13;
   regs->misc.ARM.r14
      = VG_(threads)[tid].arch.vex.guest_R14;
   regs->misc.ARM.r12
      = VG_(threads)[tid].arch.vex.guest_R12;
   regs->misc.ARM.r11
      = VG_(threads)[tid].arch.vex.guest_R11;
   regs->misc.ARM.r7
      = VG_(threads)[tid].arch.vex.guest_R7;
#  elif defined(VGA_s390x)
   regs->r_pc = (ULong)VG_(threads)[tid].arch.vex.guest_IA;
   regs->r_sp = (ULong)VG_(threads)[tid].arch.vex.guest_SP;
   regs->misc.S390X.r_fp
      = VG_(threads)[tid].arch.vex.guest_r11;
   regs->misc.S390X.r_lr
      = VG_(threads)[tid].arch.vex.guest_r14;
#  elif defined(VGA_mips32)
   regs->r_pc = VG_(threads)[tid].arch.vex.guest_PC;
   regs->r_sp = VG_(threads)[tid].arch.vex.guest_r29;
   regs->misc.MIPS32.r30
      = VG_(threads)[tid].arch.vex.guest_r30;
   regs->misc.MIPS32.r31
      = VG_(threads)[tid].arch.vex.guest_r31;
   regs->misc.MIPS32.r28
      = VG_(threads)[tid].arch.vex.guest_r28;
#  else
#    error "Unknown arch"
#  endif
}


void VG_(set_syscall_return_shadows) ( ThreadId tid,
                                       
                                       UWord s1res, UWord s2res,
                                       
                                       UWord s1err, UWord s2err )
{
#  if defined(VGP_x86_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_EAX = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_EAX = s2res;
#  elif defined(VGP_amd64_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_RAX = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_RAX = s2res;
#  elif defined(VGP_ppc32_linux) || defined(VGP_ppc64_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_GPR3 = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_GPR3 = s2res;
#  elif defined(VGP_arm_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_R0 = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_R0 = s2res;
#  elif defined(VGO_darwin)
   
#  elif defined(VGP_s390x_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_r2 = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_r2 = s2res;
#  elif defined(VGP_mips32_linux)
   VG_(threads)[tid].arch.vex_shadow1.guest_r2 = s1res;
   VG_(threads)[tid].arch.vex_shadow2.guest_r2 = s2res;
#  else
#    error "Unknown plat"
#  endif
}

void
VG_(get_shadow_regs_area) ( ThreadId tid, 
                            UChar* dst,
                            Int shadowNo, PtrdiffT offset, SizeT size )
{
   void*        src;
   ThreadState* tst;
   vg_assert(shadowNo == 0 || shadowNo == 1 || shadowNo == 2);
   vg_assert(VG_(is_valid_tid)(tid));
   
   vg_assert(0 <= offset && offset < sizeof(VexGuestArchState));
   vg_assert(offset + size <= sizeof(VexGuestArchState));
   
   tst = & VG_(threads)[tid];
   src = NULL;
   switch (shadowNo) {
      case 0: src = (void*)(((Addr)&(tst->arch.vex)) + offset); break;
      case 1: src = (void*)(((Addr)&(tst->arch.vex_shadow1)) + offset); break;
      case 2: src = (void*)(((Addr)&(tst->arch.vex_shadow2)) + offset); break;
   }
   tl_assert(src != NULL);
   VG_(memcpy)( dst, src, size);
}

void
VG_(set_shadow_regs_area) ( ThreadId tid, 
                            Int shadowNo, PtrdiffT offset, SizeT size,
                            const UChar* src )
{
   void*        dst;
   ThreadState* tst;
   vg_assert(shadowNo == 0 || shadowNo == 1 || shadowNo == 2);
   vg_assert(VG_(is_valid_tid)(tid));
   
   vg_assert(0 <= offset && offset < sizeof(VexGuestArchState));
   vg_assert(offset + size <= sizeof(VexGuestArchState));
   
   tst = & VG_(threads)[tid];
   dst = NULL;
   switch (shadowNo) {
      case 0: dst = (void*)(((Addr)&(tst->arch.vex)) + offset); break;
      case 1: dst = (void*)(((Addr)&(tst->arch.vex_shadow1)) + offset); break;
      case 2: dst = (void*)(((Addr)&(tst->arch.vex_shadow2)) + offset); break;
   }
   tl_assert(dst != NULL);
   VG_(memcpy)( dst, src, size);
}


static void apply_to_GPs_of_tid(ThreadId tid, void (*f)(ThreadId, HChar*, Addr))
{
   VexGuestArchState* vex = &(VG_(get_ThreadState)(tid)->arch.vex);
#if defined(VGA_x86)
   (*f)(tid, "EAX", vex->guest_EAX);
   (*f)(tid, "ECX", vex->guest_ECX);
   (*f)(tid, "EDX", vex->guest_EDX);
   (*f)(tid, "EBX", vex->guest_EBX);
   (*f)(tid, "ESI", vex->guest_ESI);
   (*f)(tid, "EDI", vex->guest_EDI);
   (*f)(tid, "ESP", vex->guest_ESP);
   (*f)(tid, "EBP", vex->guest_EBP);
#elif defined(VGA_amd64)
   (*f)(tid, "RAX", vex->guest_RAX);
   (*f)(tid, "RCX", vex->guest_RCX);
   (*f)(tid, "RDX", vex->guest_RDX);
   (*f)(tid, "RBX", vex->guest_RBX);
   (*f)(tid, "RSI", vex->guest_RSI);
   (*f)(tid, "RDI", vex->guest_RDI);
   (*f)(tid, "RSP", vex->guest_RSP);
   (*f)(tid, "RBP", vex->guest_RBP);
   (*f)(tid, "R8" , vex->guest_R8 );
   (*f)(tid, "R9" , vex->guest_R9 );
   (*f)(tid, "R10", vex->guest_R10);
   (*f)(tid, "R11", vex->guest_R11);
   (*f)(tid, "R12", vex->guest_R12);
   (*f)(tid, "R13", vex->guest_R13);
   (*f)(tid, "R14", vex->guest_R14);
   (*f)(tid, "R15", vex->guest_R15);
#elif defined(VGA_ppc32) || defined(VGA_ppc64)
   (*f)(tid, "GPR0" , vex->guest_GPR0 );
   (*f)(tid, "GPR1" , vex->guest_GPR1 );
   (*f)(tid, "GPR2" , vex->guest_GPR2 );
   (*f)(tid, "GPR3" , vex->guest_GPR3 );
   (*f)(tid, "GPR4" , vex->guest_GPR4 );
   (*f)(tid, "GPR5" , vex->guest_GPR5 );
   (*f)(tid, "GPR6" , vex->guest_GPR6 );
   (*f)(tid, "GPR7" , vex->guest_GPR7 );
   (*f)(tid, "GPR8" , vex->guest_GPR8 );
   (*f)(tid, "GPR9" , vex->guest_GPR9 );
   (*f)(tid, "GPR10", vex->guest_GPR10);
   (*f)(tid, "GPR11", vex->guest_GPR11);
   (*f)(tid, "GPR12", vex->guest_GPR12);
   (*f)(tid, "GPR13", vex->guest_GPR13);
   (*f)(tid, "GPR14", vex->guest_GPR14);
   (*f)(tid, "GPR15", vex->guest_GPR15);
   (*f)(tid, "GPR16", vex->guest_GPR16);
   (*f)(tid, "GPR17", vex->guest_GPR17);
   (*f)(tid, "GPR18", vex->guest_GPR18);
   (*f)(tid, "GPR19", vex->guest_GPR19);
   (*f)(tid, "GPR20", vex->guest_GPR20);
   (*f)(tid, "GPR21", vex->guest_GPR21);
   (*f)(tid, "GPR22", vex->guest_GPR22);
   (*f)(tid, "GPR23", vex->guest_GPR23);
   (*f)(tid, "GPR24", vex->guest_GPR24);
   (*f)(tid, "GPR25", vex->guest_GPR25);
   (*f)(tid, "GPR26", vex->guest_GPR26);
   (*f)(tid, "GPR27", vex->guest_GPR27);
   (*f)(tid, "GPR28", vex->guest_GPR28);
   (*f)(tid, "GPR29", vex->guest_GPR29);
   (*f)(tid, "GPR30", vex->guest_GPR30);
   (*f)(tid, "GPR31", vex->guest_GPR31);
   (*f)(tid, "CTR"  , vex->guest_CTR  );
   (*f)(tid, "LR"   , vex->guest_LR   );
#elif defined(VGA_arm)
   (*f)(tid, "R0" , vex->guest_R0 );
   (*f)(tid, "R1" , vex->guest_R1 );
   (*f)(tid, "R2" , vex->guest_R2 );
   (*f)(tid, "R3" , vex->guest_R3 );
   (*f)(tid, "R4" , vex->guest_R4 );
   (*f)(tid, "R5" , vex->guest_R5 );
   (*f)(tid, "R6" , vex->guest_R6 );
   (*f)(tid, "R8" , vex->guest_R8 );
   (*f)(tid, "R9" , vex->guest_R9 );
   (*f)(tid, "R10", vex->guest_R10);
   (*f)(tid, "R11", vex->guest_R11);
   (*f)(tid, "R12", vex->guest_R12);
   (*f)(tid, "R13", vex->guest_R13);
   (*f)(tid, "R14", vex->guest_R14);
#elif defined(VGA_s390x)
   (*f)(tid, "r0" , vex->guest_r0 );
   (*f)(tid, "r1" , vex->guest_r1 );
   (*f)(tid, "r2" , vex->guest_r2 );
   (*f)(tid, "r3" , vex->guest_r3 );
   (*f)(tid, "r4" , vex->guest_r4 );
   (*f)(tid, "r5" , vex->guest_r5 );
   (*f)(tid, "r6" , vex->guest_r6 );
   (*f)(tid, "r7" , vex->guest_r7 );
   (*f)(tid, "r8" , vex->guest_r8 );
   (*f)(tid, "r9" , vex->guest_r9 );
   (*f)(tid, "r10", vex->guest_r10);
   (*f)(tid, "r11", vex->guest_r11);
   (*f)(tid, "r12", vex->guest_r12);
   (*f)(tid, "r13", vex->guest_r13);
   (*f)(tid, "r14", vex->guest_r14);
   (*f)(tid, "r15", vex->guest_r15);
#elif defined(VGA_mips32)
   (*f)(tid, "r0" , vex->guest_r0 );
   (*f)(tid, "r1" , vex->guest_r1 );
   (*f)(tid, "r2" , vex->guest_r2 );
   (*f)(tid, "r3" , vex->guest_r3 );
   (*f)(tid, "r4" , vex->guest_r4 );
   (*f)(tid, "r5" , vex->guest_r5 );
   (*f)(tid, "r6" , vex->guest_r6 );
   (*f)(tid, "r7" , vex->guest_r7 );
   (*f)(tid, "r8" , vex->guest_r8 );
   (*f)(tid, "r9" , vex->guest_r9 );
   (*f)(tid, "r10", vex->guest_r10);
   (*f)(tid, "r11", vex->guest_r11);
   (*f)(tid, "r12", vex->guest_r12);
   (*f)(tid, "r13", vex->guest_r13);
   (*f)(tid, "r14", vex->guest_r14);
   (*f)(tid, "r15", vex->guest_r15);
   (*f)(tid, "r16", vex->guest_r16);
   (*f)(tid, "r17", vex->guest_r17);
   (*f)(tid, "r18", vex->guest_r18);
   (*f)(tid, "r19", vex->guest_r19);
   (*f)(tid, "r20", vex->guest_r20);
   (*f)(tid, "r21", vex->guest_r21);
   (*f)(tid, "r22", vex->guest_r22);
   (*f)(tid, "r23", vex->guest_r23);
   (*f)(tid, "r24", vex->guest_r24);
   (*f)(tid, "r25", vex->guest_r25);
   (*f)(tid, "r26", vex->guest_r26);
   (*f)(tid, "r27", vex->guest_r27);
   (*f)(tid, "r28", vex->guest_r28);
   (*f)(tid, "r29", vex->guest_r29);
   (*f)(tid, "r30", vex->guest_r30);
   (*f)(tid, "r31", vex->guest_r31);
#else
#  error Unknown arch
#endif
}


void VG_(apply_to_GP_regs)(void (*f)(ThreadId, HChar*, UWord))
{
   ThreadId tid;

   for (tid = 1; tid < VG_N_THREADS; tid++) {
      if (VG_(is_valid_tid)(tid)) {
         apply_to_GPs_of_tid(tid, f);
      }
   }
}

void VG_(thread_stack_reset_iter)(ThreadId* tid)
{
   *tid = (ThreadId)(-1);
}

Bool VG_(thread_stack_next)(ThreadId* tid,
                            Addr* stack_min, 
                            Addr* stack_max)
{
   ThreadId i;
   for (i = (*tid)+1; i < VG_N_THREADS; i++) {
      if (i == VG_INVALID_THREADID)
         continue;
      if (VG_(threads)[i].status != VgTs_Empty) {
         *tid       = i;
         *stack_min = VG_(get_SP)(i);
         *stack_max = VG_(threads)[i].client_stack_highest_word;
         return True;
      }
   }
   return False;
}

Addr VG_(thread_get_stack_max)(ThreadId tid)
{
   vg_assert(0 <= tid && tid < VG_N_THREADS && tid != VG_INVALID_THREADID);
   vg_assert(VG_(threads)[tid].status != VgTs_Empty);
   return VG_(threads)[tid].client_stack_highest_word;
}

SizeT VG_(thread_get_stack_size)(ThreadId tid)
{
   vg_assert(0 <= tid && tid < VG_N_THREADS && tid != VG_INVALID_THREADID);
   vg_assert(VG_(threads)[tid].status != VgTs_Empty);
   return VG_(threads)[tid].client_stack_szB;
}

Addr VG_(thread_get_altstack_min)(ThreadId tid)
{
   vg_assert(0 <= tid && tid < VG_N_THREADS && tid != VG_INVALID_THREADID);
   vg_assert(VG_(threads)[tid].status != VgTs_Empty);
   return (Addr)VG_(threads)[tid].altstack.ss_sp;
}

SizeT VG_(thread_get_altstack_size)(ThreadId tid)
{
   vg_assert(0 <= tid && tid < VG_N_THREADS && tid != VG_INVALID_THREADID);
   vg_assert(VG_(threads)[tid].status != VgTs_Empty);
   return VG_(threads)[tid].altstack.ss_size;
}


static Bool hwcaps_done = False;

static VexArch     va = VexArch_INVALID;
static VexArchInfo vai;

#if defined(VGA_x86)
UInt VG_(machine_x86_have_mxcsr) = 0;
#endif
#if defined(VGA_ppc32)
UInt VG_(machine_ppc32_has_FP)  = 0;
UInt VG_(machine_ppc32_has_VMX) = 0;
#endif
#if defined(VGA_ppc64)
ULong VG_(machine_ppc64_has_VMX) = 0;
#endif
#if defined(VGA_arm)
Int VG_(machine_arm_archlevel) = 4;
#endif


#if defined(VGA_ppc32) || defined(VGA_ppc64) \
    || defined(VGA_arm) || defined(VGA_s390x)
#include "pub_tool_libcsetjmp.h"
static VG_MINIMAL_JMP_BUF(env_unsup_insn);
static void handler_unsup_insn ( Int x ) {
   VG_MINIMAL_LONGJMP(env_unsup_insn);
}
#endif


#if defined(VGA_ppc32) || defined(VGA_ppc64)
static void find_ppc_dcbz_sz(VexArchInfo *arch_info)
{
   Int dcbz_szB = 0;
   Int dcbzl_szB;
#  define MAX_DCBZL_SZB (128) 
   char test_block[4*MAX_DCBZL_SZB];
   char *aligned = test_block;
   Int i;

   
   aligned = (char *)(((HWord)aligned + MAX_DCBZL_SZB) & ~(MAX_DCBZL_SZB - 1));
   vg_assert((aligned + MAX_DCBZL_SZB) <= &test_block[sizeof(test_block)]);

   VG_(memset)(test_block, 0xff, sizeof(test_block));
   __asm__ __volatile__("dcbz 0,%0"
                        : 
                        : "r" (aligned) 
                        : "memory" );
   for (dcbz_szB = 0, i = 0; i < sizeof(test_block); ++i) {
      if (!test_block[i])
         ++dcbz_szB;
   }
   vg_assert(dcbz_szB == 32 || dcbz_szB == 64 || dcbz_szB == 128);

   
   if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
      dcbzl_szB = 0; 
   }
   else {
      VG_(memset)(test_block, 0xff, sizeof(test_block));
      __asm__ __volatile__("mr 9, %0 ; .long 0x7C204FEC" 
                           : 
                           : "r" (aligned) 
                           : "memory", "r9" );
      for (dcbzl_szB = 0, i = 0; i < sizeof(test_block); ++i) {
         if (!test_block[i])
            ++dcbzl_szB;
      }
      vg_assert(dcbzl_szB == 32 || dcbzl_szB == 64 || dcbzl_szB == 128);
   }

   arch_info->ppc_dcbz_szB  = dcbz_szB;
   arch_info->ppc_dcbzl_szB = dcbzl_szB;

   VG_(debugLog)(1, "machine", "dcbz_szB=%d dcbzl_szB=%d\n",
                 dcbz_szB, dcbzl_szB);
#  undef MAX_DCBZL_SZB
}
#endif 

#ifdef VGA_s390x


static UInt VG_(get_machine_model)(void)
{
   static struct model_map {
      HChar name[5];
      UInt  id;
   } model_map[] = {
      { "2064", VEX_S390X_MODEL_Z900 },
      { "2066", VEX_S390X_MODEL_Z800 },
      { "2084", VEX_S390X_MODEL_Z990 },
      { "2086", VEX_S390X_MODEL_Z890 },
      { "2094", VEX_S390X_MODEL_Z9_EC },
      { "2096", VEX_S390X_MODEL_Z9_BC },
      { "2097", VEX_S390X_MODEL_Z10_EC },
      { "2098", VEX_S390X_MODEL_Z10_BC },
      { "2817", VEX_S390X_MODEL_Z196 },
      { "2818", VEX_S390X_MODEL_Z114 },
   };

   Int    model, n, fh;
   SysRes fd;
   SizeT  num_bytes, file_buf_size;
   HChar *p, *m, *model_name, *file_buf;

   
   fd = VG_(open)( "/proc/cpuinfo", 0, VKI_S_IRUSR );
   if ( sr_isError(fd) ) return VEX_S390X_MODEL_UNKNOWN;

   fh  = sr_Res(fd);

   num_bytes = 0;
   file_buf_size = 1000;
   file_buf = VG_(malloc)("cpuinfo", file_buf_size + 1);
   while (42) {
      n = VG_(read)(fh, file_buf, file_buf_size);
      if (n < 0) break;

      num_bytes += n;
      if (n < file_buf_size) break;  
   }

   if (n < 0) num_bytes = 0;   

   if (num_bytes > file_buf_size) {
      VG_(free)( file_buf );
      VG_(lseek)( fh, 0, VKI_SEEK_SET );
      file_buf = VG_(malloc)( "cpuinfo", num_bytes + 1 );
      n = VG_(read)( fh, file_buf, num_bytes );
      if (n < 0) num_bytes = 0;
   }

   file_buf[num_bytes] = '\0';
   VG_(close)(fh);

   
   model = VEX_S390X_MODEL_UNKNOWN;
   for (p = file_buf; *p; ++p) {
      
     if (VG_(strncmp)( p, "processor", sizeof "processor" - 1 ) != 0) continue;

     m = VG_(strstr)( p, "machine" );
     if (m == NULL) continue;

     p = m + sizeof "machine" - 1;
     while ( VG_(isspace)( *p ) || *p == '=') {
       if (*p == '\n') goto next_line;
       ++p;
     }

     model_name = p;
     for (n = 0; n < sizeof model_map / sizeof model_map[0]; ++n) {
       struct model_map *mm = model_map + n;
       SizeT len = VG_(strlen)( mm->name );
       if ( VG_(strncmp)( mm->name, model_name, len ) == 0 &&
            VG_(isspace)( model_name[len] )) {
         if (mm->id < model) model = mm->id;
         p = model_name + len;
         break;
       }
     }
     
     while (*p != '\n')
       ++p;
   next_line: ;
   }

   VG_(free)( file_buf );
   VG_(debugLog)(1, "machine", "model = %s\n",
                 model == VEX_S390X_MODEL_UNKNOWN ? "UNKNOWN"
                                                  : model_map[model].name);
   return model;
}

#endif 

#ifdef VGA_mips32

static UInt VG_(get_machine_model)(void)
{
   char *search_MIPS_str = "MIPS";
   char *search_Broadcom_str = "Broadcom";
   Int    n, fh;
   SysRes fd;
   SizeT  num_bytes, file_buf_size;
   HChar  *file_buf;

   
   fd = VG_(open)( "/proc/cpuinfo", 0, VKI_S_IRUSR );
   if ( sr_isError(fd) ) return -1;

   fh  = sr_Res(fd);

   num_bytes = 0;
   file_buf_size = 1000;
   file_buf = VG_(malloc)("cpuinfo", file_buf_size + 1);
   while (42) {
      n = VG_(read)(fh, file_buf, file_buf_size);
      if (n < 0) break;

      num_bytes += n;
      if (n < file_buf_size) break;  
   }

   if (n < 0) num_bytes = 0;   

   if (num_bytes > file_buf_size) {
      VG_(free)( file_buf );
      VG_(lseek)( fh, 0, VKI_SEEK_SET );
      file_buf = VG_(malloc)( "cpuinfo", num_bytes + 1 );
      n = VG_(read)( fh, file_buf, num_bytes );
      if (n < 0) num_bytes = 0;
   }

   file_buf[num_bytes] = '\0';
   VG_(close)(fh);

   
   if (VG_(strstr) (file_buf, search_Broadcom_str) != NULL)
       return VEX_PRID_COMP_BROADCOM;
   if (VG_(strstr) (file_buf, search_MIPS_str) != NULL)
       return VEX_PRID_COMP_MIPS;

   
   return -1;
}

#endif


Bool VG_(machine_get_hwcaps)( void )
{
   vg_assert(hwcaps_done == False);
   hwcaps_done = True;

   
   
   LibVEX_default_VexArchInfo(&vai);

#if defined(VGA_x86)
   { Bool have_sse1, have_sse2, have_cx8, have_lzcnt;
     UInt eax, ebx, ecx, edx, max_extended;
     UChar vstr[13];
     vstr[0] = 0;

     if (!VG_(has_cpuid)())
        
        return False;

     VG_(cpuid)(0, 0, &eax, &ebx, &ecx, &edx);
     if (eax < 1)
        
        return False;

     VG_(memcpy)(&vstr[0], &ebx, 4);
     VG_(memcpy)(&vstr[4], &edx, 4);
     VG_(memcpy)(&vstr[8], &ecx, 4);
     vstr[12] = 0;

     VG_(cpuid)(0x80000000, 0, &eax, &ebx, &ecx, &edx);
     max_extended = eax;

     
     VG_(cpuid)(1, 0, &eax, &ebx, &ecx, &edx);

     have_sse1 = (edx & (1<<25)) != 0; 
     have_sse2 = (edx & (1<<26)) != 0; 

     have_cx8 = (edx & (1<<8)) != 0; 
     if (!have_cx8)
        return False;

     
     have_lzcnt = False;
     if (0 == VG_(strcmp)(vstr, "AuthenticAMD")
         && max_extended >= 0x80000001) {
        VG_(cpuid)(0x80000001, 0, &eax, &ebx, &ecx, &edx);
        have_lzcnt = (ecx & (1<<5)) != 0; 
     }

     if (have_sse2 && have_sse1) {
        va          = VexArchX86;
        vai.hwcaps  = VEX_HWCAPS_X86_SSE1;
        vai.hwcaps |= VEX_HWCAPS_X86_SSE2;
        if (have_lzcnt)
           vai.hwcaps |= VEX_HWCAPS_X86_LZCNT;
        VG_(machine_x86_have_mxcsr) = 1;
        return True;
     }

     if (have_sse1) {
        va          = VexArchX86;
        vai.hwcaps  = VEX_HWCAPS_X86_SSE1;
        VG_(machine_x86_have_mxcsr) = 1;
        return True;
     }

     va         = VexArchX86;
     vai.hwcaps = 0; 
     VG_(machine_x86_have_mxcsr) = 0;
     return True;
   }

#elif defined(VGA_amd64)
   { Bool have_sse3, have_cx8, have_cx16;
     Bool have_lzcnt, have_avx ;
     UInt eax, ebx, ecx, edx, max_extended;
     UChar vstr[13];
     vstr[0] = 0;

     if (!VG_(has_cpuid)())
        
        return False;

     VG_(cpuid)(0, 0, &eax, &ebx, &ecx, &edx);
     if (eax < 1)
        
        return False;

     VG_(memcpy)(&vstr[0], &ebx, 4);
     VG_(memcpy)(&vstr[4], &edx, 4);
     VG_(memcpy)(&vstr[8], &ecx, 4);
     vstr[12] = 0;

     VG_(cpuid)(0x80000000, 0, &eax, &ebx, &ecx, &edx);
     max_extended = eax;

     
     VG_(cpuid)(1, 0, &eax, &ebx, &ecx, &edx);

     
     have_sse3 = (ecx & (1<<0)) != 0;  
     
     
     

     
     
     
     have_avx = False;
     
     if ( (ecx & ((1<<27)|(1<<28))) == ((1<<27)|(1<<28)) ) {
        ULong w;
        __asm__ __volatile__("movq $0,%%rcx ; "
                             ".byte 0x0F,0x01,0xD0 ; " 
                             "movq %%rax,%0"
                             :"=r"(w) :
                             :"rdx","rcx");
        if ((w & 6) == 6) {
           
           have_avx = True;
           
        }
     }


     have_cx8 = (edx & (1<<8)) != 0; 
     if (!have_cx8)
        return False;

     
     have_cx16 = (ecx & (1<<13)) != 0; 

     
     have_lzcnt = False;
     if (0 == VG_(strcmp)(vstr, "AuthenticAMD")
         && max_extended >= 0x80000001) {
        VG_(cpuid)(0x80000001, 0, &eax, &ebx, &ecx, &edx);
        have_lzcnt = (ecx & (1<<5)) != 0; 
     }

     va         = VexArchAMD64;
     vai.hwcaps = (have_sse3  ? VEX_HWCAPS_AMD64_SSE3  : 0)
                | (have_cx16  ? VEX_HWCAPS_AMD64_CX16  : 0)
                | (have_lzcnt ? VEX_HWCAPS_AMD64_LZCNT : 0)
                | (have_avx   ? VEX_HWCAPS_AMD64_AVX   : 0);
     return True;
   }

#elif defined(VGA_ppc32)
   {
     vki_sigset_t          saved_set, tmp_set;
     vki_sigaction_fromK_t saved_sigill_act, saved_sigfpe_act;
     vki_sigaction_toK_t     tmp_sigill_act,   tmp_sigfpe_act;

     volatile Bool have_F, have_V, have_FX, have_GX, have_VX, have_DFP;
     Int r;

     vg_assert(sizeof(vki_sigaction_fromK_t) == sizeof(vki_sigaction_toK_t));

     VG_(sigemptyset)(&tmp_set);
     VG_(sigaddset)(&tmp_set, VKI_SIGILL);
     VG_(sigaddset)(&tmp_set, VKI_SIGFPE);

     r = VG_(sigprocmask)(VKI_SIG_UNBLOCK, &tmp_set, &saved_set);
     vg_assert(r == 0);

     r = VG_(sigaction)(VKI_SIGILL, NULL, &saved_sigill_act);
     vg_assert(r == 0);
     tmp_sigill_act = saved_sigill_act;

     r = VG_(sigaction)(VKI_SIGFPE, NULL, &saved_sigfpe_act);
     vg_assert(r == 0);
     tmp_sigfpe_act = saved_sigfpe_act;

     tmp_sigill_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigill_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigill_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigill_act.ksa_handler = handler_unsup_insn;
     r = VG_(sigaction)(VKI_SIGILL, &tmp_sigill_act, NULL);
     vg_assert(r == 0);

     tmp_sigfpe_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigfpe_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigfpe_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigfpe_act.ksa_handler = handler_unsup_insn;
     r = VG_(sigaction)(VKI_SIGFPE, &tmp_sigfpe_act, NULL);
     vg_assert(r == 0);

     
     have_F = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_F = False;
     } else {
        __asm__ __volatile__(".long 0xFC000090"); 
     }

     
     have_V = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_V = False;
     } else {
        __asm__ __volatile__(".long 0x10000484"); 
     }

     
     have_FX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_FX = False;
     } else {
        __asm__ __volatile__(".long 0xFC00002C"); 
     }

     
     have_GX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_GX = False;
     } else {
        __asm__ __volatile__(".long 0xFC000034"); 
     }

     
     have_VX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_VX = False;
     } else {
        __asm__ __volatile__(".long 0xf0000564"); 
     }

     
     have_DFP = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_DFP = False;
     } else {
        __asm__ __volatile__(".long 0xee4e8005"); 
     }

     find_ppc_dcbz_sz(&vai);

     r = VG_(sigaction)(VKI_SIGILL, &saved_sigill_act, NULL);
     vg_assert(r == 0);
     r = VG_(sigaction)(VKI_SIGFPE, &saved_sigfpe_act, NULL);
     vg_assert(r == 0);
     r = VG_(sigprocmask)(VKI_SIG_SETMASK, &saved_set, NULL);
     vg_assert(r == 0);
     VG_(debugLog)(1, "machine", "F %d V %d FX %d GX %d VX %d DFP %d\n",
                    (Int)have_F, (Int)have_V, (Int)have_FX,
                    (Int)have_GX, (Int)have_VX, (Int)have_DFP);
     
     if (have_V && !have_F)
        have_V = False;
     if (have_FX && !have_F)
        have_FX = False;
     if (have_GX && !have_F)
        have_GX = False;

     VG_(machine_ppc32_has_FP)  = have_F ? 1 : 0;
     VG_(machine_ppc32_has_VMX) = have_V ? 1 : 0;

     va = VexArchPPC32;

     vai.hwcaps = 0;
     if (have_F)  vai.hwcaps |= VEX_HWCAPS_PPC32_F;
     if (have_V)  vai.hwcaps |= VEX_HWCAPS_PPC32_V;
     if (have_FX) vai.hwcaps |= VEX_HWCAPS_PPC32_FX;
     if (have_GX) vai.hwcaps |= VEX_HWCAPS_PPC32_GX;
     if (have_VX) vai.hwcaps |= VEX_HWCAPS_PPC32_VX;
     if (have_DFP) vai.hwcaps |= VEX_HWCAPS_PPC32_DFP;


     return True;
   }

#elif defined(VGA_ppc64)
   {
     
     vki_sigset_t          saved_set, tmp_set;
     vki_sigaction_fromK_t saved_sigill_act, saved_sigfpe_act;
     vki_sigaction_toK_t     tmp_sigill_act,   tmp_sigfpe_act;

     volatile Bool have_F, have_V, have_FX, have_GX, have_VX, have_DFP;
     Int r;

     vg_assert(sizeof(vki_sigaction_fromK_t) == sizeof(vki_sigaction_toK_t));

     VG_(sigemptyset)(&tmp_set);
     VG_(sigaddset)(&tmp_set, VKI_SIGILL);
     VG_(sigaddset)(&tmp_set, VKI_SIGFPE);

     r = VG_(sigprocmask)(VKI_SIG_UNBLOCK, &tmp_set, &saved_set);
     vg_assert(r == 0);

     r = VG_(sigaction)(VKI_SIGILL, NULL, &saved_sigill_act);
     vg_assert(r == 0);
     tmp_sigill_act = saved_sigill_act;

     VG_(sigaction)(VKI_SIGFPE, NULL, &saved_sigfpe_act);
     tmp_sigfpe_act = saved_sigfpe_act;

     tmp_sigill_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigill_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigill_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigill_act.ksa_handler = handler_unsup_insn;
     VG_(sigaction)(VKI_SIGILL, &tmp_sigill_act, NULL);

     tmp_sigfpe_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigfpe_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigfpe_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigfpe_act.ksa_handler = handler_unsup_insn;
     VG_(sigaction)(VKI_SIGFPE, &tmp_sigfpe_act, NULL);

     
     have_F = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_F = False;
     } else {
        __asm__ __volatile__("fmr 0,0");
     }

     
     have_V = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_V = False;
     } else {
        __asm__ __volatile__(".long 0x10000484"); 
     }

     
     have_FX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_FX = False;
     } else {
        __asm__ __volatile__(".long 0xFC00002C"); 
     }

     
     have_GX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_GX = False;
     } else {
        __asm__ __volatile__(".long 0xFC000034"); 
     }

     
     have_VX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_VX = False;
     } else {
        __asm__ __volatile__(".long 0xf0000564"); 
     }

     
     have_DFP = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_DFP = False;
     } else {
        __asm__ __volatile__(".long 0xee4e8005"); 
     }

     find_ppc_dcbz_sz(&vai);

     VG_(sigaction)(VKI_SIGILL, &saved_sigill_act, NULL);
     VG_(sigaction)(VKI_SIGFPE, &saved_sigfpe_act, NULL);
     VG_(sigprocmask)(VKI_SIG_SETMASK, &saved_set, NULL);
     VG_(debugLog)(1, "machine", "F %d V %d FX %d GX %d VX %d DFP %d\n",
                    (Int)have_F, (Int)have_V, (Int)have_FX,
                    (Int)have_GX, (Int)have_VX, (Int)have_DFP);
     
     if (!have_F)
        return False;

     VG_(machine_ppc64_has_VMX) = have_V ? 1 : 0;

     va = VexArchPPC64;

     vai.hwcaps = 0;
     if (have_V)  vai.hwcaps |= VEX_HWCAPS_PPC64_V;
     if (have_FX) vai.hwcaps |= VEX_HWCAPS_PPC64_FX;
     if (have_GX) vai.hwcaps |= VEX_HWCAPS_PPC64_GX;
     if (have_VX) vai.hwcaps |= VEX_HWCAPS_PPC64_VX;
     if (have_DFP) vai.hwcaps |= VEX_HWCAPS_PPC64_DFP;

     return True;
   }

#elif defined(VGA_s390x)
   {
     
     vki_sigset_t          saved_set, tmp_set;
     vki_sigaction_fromK_t saved_sigill_act;
     vki_sigaction_toK_t     tmp_sigill_act;

     volatile Bool have_LDISP, have_EIMM, have_GIE, have_DFP, have_FGX;
     volatile Bool have_STFLE, have_ETF2, have_ETF3;
     Int r, model;

     
     VG_(sigemptyset)(&tmp_set);
     VG_(sigaddset)(&tmp_set, VKI_SIGILL);

     r = VG_(sigprocmask)(VKI_SIG_UNBLOCK, &tmp_set, &saved_set);
     vg_assert(r == 0);

     r = VG_(sigaction)(VKI_SIGILL, NULL, &saved_sigill_act);
     vg_assert(r == 0);
     tmp_sigill_act = saved_sigill_act;

     tmp_sigill_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigill_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigill_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigill_act.ksa_handler = handler_unsup_insn;
     VG_(sigaction)(VKI_SIGILL, &tmp_sigill_act, NULL);


     have_LDISP = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_LDISP = False;
     } else {
        __asm__ __volatile__("basr %%r1,%%r0\n\t"
                             ".long  0xe3001000\n\t"  
                             ".short 0x0057" : : : "r0", "r1", "cc", "memory");
     }

     have_EIMM = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_EIMM = False;
     } else {
        __asm__ __volatile__(".long  0xc0090000\n\t"  
                             ".short 0x0000" : : : "r0", "memory");
     }

     have_GIE = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_GIE = False;
     } else {
        __asm__ __volatile__(".long  0xc2010000\n\t"  
                             ".short 0x0000" : : : "r0", "memory");
     }

     have_DFP = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_DFP = False;
     } else {
        __asm__ __volatile__(".long 0xb3d20000"
                               : : : "r0", "cc", "memory");  
     }

     have_FGX = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_FGX = False;
     } else {
        __asm__ __volatile__(".long 0xb3cd0000" : : : "r0");  
     }

     have_STFLE = True;
     have_ETF2 = False;
     have_ETF3 = False;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_STFLE = False;
     } else {
         ULong hoststfle[1];
         register ULong reg0 asm("0") = 0; 

         __asm__ __volatile__(" .insn s,0xb2b00000,%0\n"   
                              : "=m" (hoststfle), "+d"(reg0)
                              : : "cc", "memory");
         if (hoststfle[0] & (1ULL << (63 - 24)))
             have_ETF2 = True;
         if (hoststfle[0] & (1ULL << (63 - 30)))
             have_ETF3 = True;
     }

     
     r = VG_(sigaction)(VKI_SIGILL, &saved_sigill_act, NULL);
     vg_assert(r == 0);
     r = VG_(sigprocmask)(VKI_SIG_SETMASK, &saved_set, NULL);
     vg_assert(r == 0);
     va = VexArchS390X;

     model = VG_(get_machine_model)();


     VG_(debugLog)(1, "machine", "machine %d  LDISP %d EIMM %d GIE %d DFP %d "
                   "FGX %d STFLE %d ETF2 %d ETF3 %d\n", model, have_LDISP, have_EIMM,
                   have_GIE, have_DFP, have_FGX, have_STFLE, have_ETF2, have_ETF3);

     vai.hwcaps = model;
     if (have_LDISP) {
        if (model >= VEX_S390X_MODEL_Z990)
           vai.hwcaps |= VEX_HWCAPS_S390X_LDISP;
     }
     if (have_EIMM)  vai.hwcaps |= VEX_HWCAPS_S390X_EIMM;
     if (have_GIE)   vai.hwcaps |= VEX_HWCAPS_S390X_GIE;
     if (have_DFP)   vai.hwcaps |= VEX_HWCAPS_S390X_DFP;
     if (have_FGX)   vai.hwcaps |= VEX_HWCAPS_S390X_FGX;
     if (have_ETF2)  vai.hwcaps |= VEX_HWCAPS_S390X_ETF2;
     if (have_ETF3)  vai.hwcaps |= VEX_HWCAPS_S390X_ETF3;
     if (have_STFLE) vai.hwcaps |= VEX_HWCAPS_S390X_STFLE;

     VG_(debugLog)(1, "machine", "hwcaps = 0x%x\n", vai.hwcaps);

     return True;
   }

#elif defined(VGA_arm)
   {
     
     vki_sigset_t          saved_set, tmp_set;
     vki_sigaction_fromK_t saved_sigill_act, saved_sigfpe_act;
     vki_sigaction_toK_t     tmp_sigill_act,   tmp_sigfpe_act;

     volatile Bool have_VFP, have_VFP2, have_VFP3, have_NEON;
     volatile Int archlevel;
     Int r;

     vg_assert(sizeof(vki_sigaction_fromK_t) == sizeof(vki_sigaction_toK_t));

     VG_(sigemptyset)(&tmp_set);
     VG_(sigaddset)(&tmp_set, VKI_SIGILL);
     VG_(sigaddset)(&tmp_set, VKI_SIGFPE);

     r = VG_(sigprocmask)(VKI_SIG_UNBLOCK, &tmp_set, &saved_set);
     vg_assert(r == 0);

     r = VG_(sigaction)(VKI_SIGILL, NULL, &saved_sigill_act);
     vg_assert(r == 0);
     tmp_sigill_act = saved_sigill_act;

     VG_(sigaction)(VKI_SIGFPE, NULL, &saved_sigfpe_act);
     tmp_sigfpe_act = saved_sigfpe_act;

     tmp_sigill_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigill_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigill_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigill_act.ksa_handler = handler_unsup_insn;
     VG_(sigaction)(VKI_SIGILL, &tmp_sigill_act, NULL);

     tmp_sigfpe_act.sa_flags &= ~VKI_SA_RESETHAND;
     tmp_sigfpe_act.sa_flags &= ~VKI_SA_SIGINFO;
     tmp_sigfpe_act.sa_flags |=  VKI_SA_NODEFER;
     tmp_sigfpe_act.ksa_handler = handler_unsup_insn;
     VG_(sigaction)(VKI_SIGFPE, &tmp_sigfpe_act, NULL);

     
     have_VFP = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_VFP = False;
     } else {
        __asm__ __volatile__(".word 0xEEB02B42"); 
     }
     have_VFP2 = have_VFP;
     have_VFP3 = have_VFP;

     
     have_NEON = True;
     if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
        have_NEON = False;
     } else {
        __asm__ __volatile__(".word 0xF2244154"); 
     }

     
     archlevel = 5; 
     if (archlevel < 7) {
        archlevel = 7;
        if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
           archlevel = 5;
        } else {
           __asm__ __volatile__(".word 0xF45FF000"); 
        }
     }
     if (archlevel < 6) {
        archlevel = 6;
        if (VG_MINIMAL_SETJMP(env_unsup_insn)) {
           archlevel = 5;
        } else {
           __asm__ __volatile__(".word 0xE6822012"); 
        }
     }

     VG_(convert_sigaction_fromK_to_toK)(&saved_sigill_act, &tmp_sigill_act);
     VG_(convert_sigaction_fromK_to_toK)(&saved_sigfpe_act, &tmp_sigfpe_act);
     VG_(sigaction)(VKI_SIGILL, &tmp_sigill_act, NULL);
     VG_(sigaction)(VKI_SIGFPE, &tmp_sigfpe_act, NULL);
     VG_(sigprocmask)(VKI_SIG_SETMASK, &saved_set, NULL);

     VG_(debugLog)(1, "machine", "ARMv%d VFP %d VFP2 %d VFP3 %d NEON %d\n",
           archlevel, (Int)have_VFP, (Int)have_VFP2, (Int)have_VFP3,
           (Int)have_NEON);

     VG_(machine_arm_archlevel) = archlevel;

     va = VexArchARM;

     vai.hwcaps = VEX_ARM_ARCHLEVEL(archlevel);
     if (have_VFP3) vai.hwcaps |= VEX_HWCAPS_ARM_VFP3;
     if (have_VFP2) vai.hwcaps |= VEX_HWCAPS_ARM_VFP2;
     if (have_VFP)  vai.hwcaps |= VEX_HWCAPS_ARM_VFP;
     if (have_NEON) vai.hwcaps |= VEX_HWCAPS_ARM_NEON;

     return True;
   }

#elif defined(VGA_mips32)
   {
     va = VexArchMIPS32;
     UInt model = VG_(get_machine_model)();
     if (model== -1)
         return False;

     vai.hwcaps = model;
     return True;
   }

#else
#  error "Unknown arch"
#endif
}

#if defined(VGA_ppc32)
void VG_(machine_ppc32_set_clszB)( Int szB )
{
   vg_assert(hwcaps_done);

   vg_assert(vai.ppc_cache_line_szB == 0
             || vai.ppc_cache_line_szB == szB);

   vg_assert(szB == 32 || szB == 64 || szB == 128);
   vai.ppc_cache_line_szB = szB;
}
#endif


#if defined(VGA_ppc64)
void VG_(machine_ppc64_set_clszB)( Int szB )
{
   vg_assert(hwcaps_done);

   vg_assert(vai.ppc_cache_line_szB == 0
             || vai.ppc_cache_line_szB == szB);

   vg_assert(szB == 32 || szB == 64 || szB == 128);
   vai.ppc_cache_line_szB = szB;
}
#endif


#if defined(VGA_arm)
void VG_(machine_arm_set_has_NEON)( Bool has_neon )
{
   vg_assert(hwcaps_done);
   

   if (has_neon) {
      vai.hwcaps |= VEX_HWCAPS_ARM_NEON;
   } else {
      vai.hwcaps &= ~VEX_HWCAPS_ARM_NEON;
   }
}
#endif


void VG_(machine_get_VexArchInfo)( VexArch* pVa,
                                   VexArchInfo* pVai )
{
   vg_assert(hwcaps_done);
   if (pVa)  *pVa  = va;
   if (pVai) *pVai = vai;
}


Int VG_(machine_get_size_of_largest_guest_register) ( void )
{
   vg_assert(hwcaps_done);

#  if defined(VGA_x86)
   vg_assert(va == VexArchX86);
   return 16;

#  elif defined(VGA_amd64)
   
   return (vai.hwcaps & VEX_HWCAPS_AMD64_AVX) ? 32 : 16;

#  elif defined(VGA_ppc32)
   
   if (vai.hwcaps & VEX_HWCAPS_PPC32_V) return 16;
   if (vai.hwcaps & VEX_HWCAPS_PPC32_VX) return 16;
   if (vai.hwcaps & VEX_HWCAPS_PPC32_DFP) return 16;
   return 8;

#  elif defined(VGA_ppc64)
   
   if (vai.hwcaps & VEX_HWCAPS_PPC64_V) return 16;
   if (vai.hwcaps & VEX_HWCAPS_PPC64_VX) return 16;
   if (vai.hwcaps & VEX_HWCAPS_PPC64_DFP) return 16;
   return 8;

#  elif defined(VGA_s390x)
   return 8;

#  elif defined(VGA_arm)
   return 16;

#  elif defined(VGA_mips32)
   return 8;

#  else
#    error "Unknown arch"
#  endif
}


void* VG_(fnptr_to_fnentry)( void* f )
{
#  if defined(VGP_x86_linux) || defined(VGP_amd64_linux)  \
      || defined(VGP_arm_linux)                           \
      || defined(VGP_ppc32_linux) || defined(VGO_darwin)  \
      || defined(VGP_s390x_linux) || defined(VGP_mips32_linux)
   return f;
#  elif defined(VGP_ppc64_linux)
   UWord* descr = (UWord*)f;
   return (void*)(descr[0]);
#  else
#    error "Unknown platform"
#  endif
}

