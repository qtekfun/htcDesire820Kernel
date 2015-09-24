

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
#include "pub_core_debuginfo.h"     
#include "pub_core_aspacemgr.h"     
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_machine.h"
#include "pub_core_options.h"
#include "pub_core_stacks.h"        
#include "pub_core_stacktrace.h"
#include "pub_core_xarray.h"
#include "pub_core_clientstate.h"   
#include "pub_core_trampoline.h"





#if defined(VGP_x86_linux) || defined(VGP_x86_darwin)

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   D3UnwindRegs uregs;
   uregs.xip = (Addr)startRegs->r_pc;
   uregs.xsp = (Addr)startRegs->r_sp;
   uregs.xbp = startRegs->misc.X86.r_ebp;
   Addr fp_min = uregs.xsp;


   
   
   
   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("max_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx ip=0x%lx fp=0x%lx\n",
                  max_n_ips, fp_min, fp_max_orig, fp_max,
                  uregs.xip, uregs.xbp);

   
   
   
#  if !defined(VGO_darwin)
   if (fp_min + 512 >= fp_max) {
      if (sps) sps[0] = uregs.xsp;
      if (fps) fps[0] = uregs.xbp;
      ips[0] = uregs.xip;
      return 1;
   } 
#  endif

   

   if (sps) sps[0] = uregs.xsp;
   if (fps) fps[0] = uregs.xbp;
   ips[0] = uregs.xip;
   i = 1;

   while (True) {

      if (i >= max_n_ips)
         break;


      if (fp_min <= uregs.xbp &&
          uregs.xbp <= fp_max - 1 * sizeof(UWord))
      {
         
         uregs.xip = (((UWord*)uregs.xbp)[1]);
         
         
         
         
         
         
         if (0 == uregs.xip || 1 == uregs.xip) break;
         uregs.xsp = uregs.xbp + sizeof(Addr)  
                               + sizeof(Addr) ;
         uregs.xbp = (((UWord*)uregs.xbp)[0]);
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip - 1; 
         if (debug)
            VG_(printf)("     ipsF[%d]=0x%08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1;
            
         continue;
      }

      if ( VG_(use_CF_info)( &uregs, fp_min, fp_max ) ) {
         if (0 == uregs.xip || 1 == uregs.xip) break;
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip - 1; 
         if (debug)
            VG_(printf)("     ipsC[%d]=0x%08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1;
            
         continue;
      }

      
      if ( VG_(use_FPO_info)( &uregs.xip, &uregs.xsp, &uregs.xbp,
                              fp_min, fp_max ) ) {
         if (0 == uregs.xip || 1 == uregs.xip) break;
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip;
         if (debug)
            VG_(printf)("     ipsC[%d]=0x%08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1;
         continue;
      }

      
      break;
   }

   n_found = i;
   return n_found;
}

#endif


#if defined(VGP_amd64_linux) || defined(VGP_amd64_darwin)

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   D3UnwindRegs uregs;
   uregs.xip = startRegs->r_pc;
   uregs.xsp = startRegs->r_sp;
   uregs.xbp = startRegs->misc.AMD64.r_rbp;
   Addr fp_min = uregs.xsp;


   
   
   
   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("max_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx ip=0x%lx fp=0x%lx\n",
                  max_n_ips, fp_min, fp_max_orig, fp_max,
                  uregs.xip, uregs.xbp);

   
   
   
#  if !defined(VGO_darwin)
   if (fp_min + 256 >= fp_max) {
      if (sps) sps[0] = uregs.xsp;
      if (fps) fps[0] = uregs.xbp;
      ips[0] = uregs.xip;
      return 1;
   } 
#  endif

   

   ips[0] = uregs.xip;
   if (sps) sps[0] = uregs.xsp;
   if (fps) fps[0] = uregs.xbp;
   i = 1;

   while (True) {

      if (i >= max_n_ips)
         break;

      

      if ( VG_(use_CF_info)( &uregs, fp_min, fp_max ) ) {
         if (0 == uregs.xip || 1 == uregs.xip) break;
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip - 1; 
         if (debug)
            VG_(printf)("     ipsC[%d]=%#08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1; 
         continue;
      }

      if (fp_min <= uregs.xbp && uregs.xbp <= fp_max - 1 * sizeof(UWord)) {
         
         uregs.xip = (((UWord*)uregs.xbp)[1]);
         if (0 == uregs.xip || 1 == uregs.xip) break;
         uregs.xsp = uregs.xbp + sizeof(Addr)  
                               + sizeof(Addr) ;
         uregs.xbp = (((UWord*)uregs.xbp)[0]);
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip - 1; 
         if (debug)
            VG_(printf)("     ipsF[%d]=%#08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1; 
         continue;
      }

      if (fp_min <= uregs.xsp && uregs.xsp < fp_max) {
         uregs.xip = ((UWord*)uregs.xsp)[0];
         if (0 == uregs.xip || 1 == uregs.xip) break;
         if (sps) sps[i] = uregs.xsp;
         if (fps) fps[i] = uregs.xbp;
         ips[i++] = uregs.xip == 0 
                    ? 0 
                    : uregs.xip - 1;
                        
         if (debug)
            VG_(printf)("     ipsH[%d]=%#08lx\n", i-1, ips[i-1]);
         uregs.xip = uregs.xip - 1; 
         uregs.xsp += 8;
         continue;
      }

      
      break;
   }

   n_found = i;
   return n_found;
}

#endif


#if defined(VGP_ppc32_linux) || defined(VGP_ppc64_linux)

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  lr_is_first_RA = False;
#  if defined(VG_PLAT_USES_PPCTOC)
   Word redir_stack_size = 0;
   Word redirs_used      = 0;
#  endif

   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   Addr ip = (Addr)startRegs->r_pc;
   Addr sp = (Addr)startRegs->r_sp;
   Addr fp = sp;
#  if defined(VGP_ppc32_linux)
   Addr lr = startRegs->misc.PPC32.r_lr;
#  elif defined(VGP_ppc64_linux)
   Addr lr = startRegs->misc.PPC64.r_lr;
#  endif
   Addr fp_min = sp;


   
   
   
   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("max_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx ip=0x%lx fp=0x%lx\n",
		  max_n_ips, fp_min, fp_max_orig, fp_max, ip, fp);

   
   if (fp_min + 512 >= fp_max) {
      if (sps) sps[0] = sp;
      if (fps) fps[0] = fp;
      ips[0] = ip;
      return 1;
   } 


#  if defined(VGP_ppc64_linux)
   redir_stack_size = VEX_GUEST_PPC64_REDIR_STACK_SIZE;
   redirs_used      = 0;
#  endif

#  if defined(VG_PLAT_USES_PPCTOC)
   if (ULong_to_Ptr(lr) == (void*)&VG_(ppctoc_magic_redirect_return_stub)
       && VG_(is_valid_tid)(tid_if_known)) {
      Word hsp = VG_(threads)[tid_if_known].arch.vex.guest_REDIR_SP;
      redirs_used++;
      if (hsp >= 1 && hsp < redir_stack_size)
         lr = VG_(threads)[tid_if_known]
                 .arch.vex.guest_REDIR_STACK[hsp-1];
   }
#  endif

   lr_is_first_RA = False;
   {
#     define M_VG_ERRTXT 1000
      UChar buf_lr[M_VG_ERRTXT], buf_ip[M_VG_ERRTXT];
      if (VG_(get_fnname_raw) (lr, buf_lr, M_VG_ERRTXT))
         if (VG_(get_fnname_raw) (ip, buf_ip, M_VG_ERRTXT))
            if (VG_(strncmp)(buf_lr, buf_ip, M_VG_ERRTXT))
               lr_is_first_RA = True;
#     undef M_VG_ERRTXT
   }

   if (sps) sps[0] = fp; 
   if (fps) fps[0] = fp;
   ips[0] = ip;
   i = 1;

   if (fp_min <= fp && fp < fp_max-VG_WORDSIZE+1) {

      
      fp = (((UWord*)fp)[0]);

      while (True) {

#        if defined(VG_PLAT_USES_PPCTOC)
         const Int lr_offset = 2;
#        else
         const Int lr_offset = 1;
#        endif

         if (i >= max_n_ips)
            break;

         

         if (fp_min <= fp && fp <= fp_max - lr_offset * sizeof(UWord)) {
            

            if (i == 1 && lr_is_first_RA)
               ip = lr;
            else
               ip = (((UWord*)fp)[lr_offset]);

#           if defined(VG_PLAT_USES_PPCTOC)
            if (ip == (Addr)&VG_(ppctoc_magic_redirect_return_stub)
                && VG_(is_valid_tid)(tid_if_known)) {
               Word hsp = VG_(threads)[tid_if_known]
                             .arch.vex.guest_REDIR_SP;
               hsp -= 2 * redirs_used;
               redirs_used ++;
               if (hsp >= 1 && hsp < redir_stack_size)
                  ip = VG_(threads)[tid_if_known]
                          .arch.vex.guest_REDIR_STACK[hsp-1];
            }
#           endif

            if (0 == ip || 1 == ip) break;
            if (sps) sps[i] = fp; 
            if (fps) fps[i] = fp;
            fp = (((UWord*)fp)[0]);
            ips[i++] = ip - 1; 
            if (debug)
               VG_(printf)("     ipsF[%d]=%#08lx\n", i-1, ips[i-1]);
            ip = ip - 1; 
            continue;
         }

         
         break;
      }
   }

   n_found = i;
   return n_found;
}

#endif


#if defined(VGP_arm_linux)

static Bool in_same_fn ( Addr a1, Addr a2 )
{
#  define M_VG_ERRTXT 500
   UChar buf_a1[M_VG_ERRTXT], buf_a2[M_VG_ERRTXT];
   if (VG_(get_fnname_raw) (a1, buf_a1, M_VG_ERRTXT))
      if (VG_(get_fnname_raw) (a2, buf_a2, M_VG_ERRTXT))
         if (VG_(strncmp)(buf_a1, buf_a2, M_VG_ERRTXT))
            return True;
#  undef M_VG_ERRTXT
   return False;
}

static Bool in_same_page ( Addr a1, Addr a2 ) {
   return (a1 & ~0xFFF) == (a2 & ~0xFFF);
}

static Addr abs_diff ( Addr a1, Addr a2 ) {
   return (Addr)(a1 > a2 ? a1 - a2 : a2 - a1);
}

static Bool has_XT_perms ( Addr a )
{
   NSegment const* seg = VG_(am_find_nsegment)(a);
   return seg && seg->hasX && seg->hasT;
}

static Bool looks_like_Thumb_call32 ( UShort w0, UShort w1 )
{
   if (0)
      VG_(printf)("isT32call %04x %04x\n", (UInt)w0, (UInt)w1);
   
   if ((w0 & 0xF800) == 0xF000 && (w1 & 0xC000) == 0xC000) return True;
   
   if ((w0 & 0xF800) == 0xF000 && (w1 & 0xC000) == 0xC000) return True;
   return False;
}

static Bool looks_like_Thumb_call16 ( UShort w0 )
{
   return False;
}

static Bool looks_like_ARM_call ( UInt a0 )
{
   if (0)
      VG_(printf)("isA32call %08x\n", a0);
   
   if ((a0 & 0xFF000000) == 0xEB000000) return True;
   return False;
}

static Bool looks_like_RA ( Addr ra )
{
   Bool isT = (ra & 1);
   if (isT) {
      
      ra &= ~1;
      ra -= 4;
      if (has_XT_perms(ra)) {
         UShort w0 = *(UShort*)ra;
         UShort w1 = in_same_page(ra, ra+2) ? *(UShort*)(ra+2) : 0;
         if (looks_like_Thumb_call16(w1) || looks_like_Thumb_call32(w0,w1))
            return True;
      }
   } else {
      
      ra &= ~3;
      ra -= 4;
      if (has_XT_perms(ra)) {
         UInt a0 = *(UInt*)ra;
         if (looks_like_ARM_call(a0))
            return True;
      }
   }
   return False;
}

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   D3UnwindRegs uregs;
   uregs.r15 = startRegs->r_pc & 0xFFFFFFFE;
   uregs.r14 = startRegs->misc.ARM.r14;
   uregs.r13 = startRegs->r_sp;
   uregs.r12 = startRegs->misc.ARM.r12;
   uregs.r11 = startRegs->misc.ARM.r11;
   uregs.r7  = startRegs->misc.ARM.r7;
   Addr fp_min = uregs.r13;


   
   
   
   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("\nmax_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx r15=0x%lx r13=0x%lx\n",
                  max_n_ips, fp_min, fp_max_orig, fp_max,
                  uregs.r15, uregs.r13);

   
   
   
   if (fp_min + 512 >= fp_max) {
      if (sps) sps[0] = uregs.r13;
      if (fps) fps[0] = 0;
      ips[0] = uregs.r15;
      return 1;
   } 

   

   if (sps) sps[0] = uregs.r13;
   if (fps) fps[0] = 0;
   ips[0] = uregs.r15;
   i = 1;

   
   Bool do_stack_scan = False;

   while (True) {
      if (debug) {
         VG_(printf)("i: %d, r15: 0x%lx, r13: 0x%lx\n",
                     i, uregs.r15, uregs.r13);
      }

      if (i >= max_n_ips)
         break;

      if (VG_(use_CF_info)( &uregs, fp_min, fp_max )) {
         if (sps) sps[i] = uregs.r13;
         if (fps) fps[i] = 0;
         ips[i++] = (uregs.r15 & 0xFFFFFFFE) - 1;
         if (debug)
            VG_(printf)("USING CFI: r15: 0x%lx, r13: 0x%lx\n",
                        uregs.r15, uregs.r13);
         uregs.r15 = (uregs.r15 & 0xFFFFFFFE) - 1;
         continue;
      }
      
      do_stack_scan = True;
      break;
   }

   if (0 && do_stack_scan && i < max_n_ips && i <= 2) {
      Int  nByStackScan = 0;
      Addr lr = uregs.r14;
      Addr sp = uregs.r13 & ~3;
      Addr pc = uregs.r15;
      
      
      if (!in_same_fn(lr, pc) && looks_like_RA(lr)) {
         
         
         Addr cand = (lr & 0xFFFFFFFE) - 1;
         if (abs_diff(cand, ips[i-1]) > 1) {
            if (sps) sps[i] = 0;
            if (fps) fps[i] = 0;
            ips[i++] = cand;
            nByStackScan++;
         }
      }
      while (in_same_page(sp, uregs.r13)) {
         if (i >= max_n_ips)
            break;
         
         UWord w = *(UWord*)(sp & ~0x3);
         if (looks_like_RA(w)) {
            Addr cand = (w & 0xFFFFFFFE) - 1;
            
            
            if (abs_diff(cand, ips[i-1]) > 1) {
               if (sps) sps[i] = 0;
               if (fps) fps[i] = 0;
               ips[i++] = cand;
               if (++nByStackScan >= 5) break;
            }
         }
         sp += 4;
      }
   }

   n_found = i;
   return n_found;
}

#endif


#if defined(VGP_s390x_linux)

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   D3UnwindRegs uregs;
   uregs.ia = startRegs->r_pc;
   uregs.sp = startRegs->r_sp;
   Addr fp_min = uregs.sp;
   uregs.fp = startRegs->misc.S390X.r_fp;
   uregs.lr = startRegs->misc.S390X.r_lr;

   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("max_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx IA=0x%lx SP=0x%lx FP=0x%lx\n",
                  max_n_ips, fp_min, fp_max_orig, fp_max,
                  uregs.ia, uregs.sp,uregs.fp);

   
   ips[0] = uregs.ia;
   if (sps) sps[0] = uregs.sp;
   if (fps) fps[0] = uregs.fp;
   i = 1;

   while (True) {
      if (i >= max_n_ips)
         break;

      if (VG_(use_CF_info)( &uregs, fp_min, fp_max )) {
         if (sps) sps[i] = uregs.sp;
         if (fps) fps[i] = uregs.fp;
         ips[i++] = uregs.ia - 1;
         uregs.ia = uregs.ia - 1;
         continue;
      }
      if (i == 1) {
         if (sps) {
            sps[i] = sps[0];
            uregs.sp = sps[0];
         }
         if (fps) {
            fps[i] = fps[0];
            uregs.fp = fps[0];
         }
         uregs.ia = uregs.lr - 1;
         ips[i++] = uregs.lr - 1;
         continue;
      }

      
      break;
   }

   n_found = i;
   return n_found;
}

#endif


#if defined(VGP_mips32_linux)

UInt VG_(get_StackTrace_wrk) ( ThreadId tid_if_known,
                               Addr* ips, UInt max_n_ips,
                               Addr* sps, Addr* fps,
                               UnwindStartRegs* startRegs,
                               Addr fp_max_orig )
{
   Bool  debug = False;
   Int   i;
   Addr  fp_max;
   UInt  n_found = 0;

   vg_assert(sizeof(Addr) == sizeof(UWord));
   vg_assert(sizeof(Addr) == sizeof(void*));

   D3UnwindRegs uregs;
   uregs.pc = startRegs->r_pc;
   uregs.sp = startRegs->r_sp;
   Addr fp_min = uregs.sp;

   uregs.fp = startRegs->misc.MIPS32.r30;
   uregs.ra = startRegs->misc.MIPS32.r31;


   fp_max = VG_PGROUNDUP(fp_max_orig);
   if (fp_max >= sizeof(Addr))
      fp_max -= sizeof(Addr);

   if (debug)
      VG_(printf)("max_n_ips=%d fp_min=0x%lx fp_max_orig=0x%lx, "
                  "fp_max=0x%lx pc=0x%lx sp=0x%lx fp=0x%lx\n",
                  max_n_ips, fp_min, fp_max_orig, fp_max,
                  uregs.pc, uregs.sp, uregs.fp);

   if (sps) sps[0] = uregs.sp;
   if (fps) fps[0] = uregs.fp;
   ips[0] = uregs.pc;
   i = 1;

   

   while (True) {
      if (debug) {
         VG_(printf)("i: %d, pc: 0x%lx, sp: 0x%lx, ra: 0x%lx\n",
                     i, uregs.pc, uregs.sp, uregs.ra);
      }
      if (i >= max_n_ips)
         break;

      if (VG_(use_CF_info)( &uregs, fp_min, fp_max )) {
         if (debug)
            VG_(printf)("USING CFI: pc: 0x%lx, sp: 0x%lx, ra: 0x%lx\n",
                        uregs.pc, uregs.sp, uregs.ra);
         if (0 == uregs.pc || 1 == uregs.pc) break;
         if (sps) sps[i] = uregs.sp;
         if (fps) fps[i] = uregs.fp;
         ips[i++] = uregs.pc - 4;
         uregs.pc = uregs.pc - 4;
         continue;
      }

      int seen_sp_adjust = 0;
      long frame_offset = 0;
      PtrdiffT offset;
      if (VG_(get_inst_offset_in_function)(uregs.pc, &offset)) {
         Addr start_pc = uregs.pc - offset;
         Addr limit_pc = uregs.pc;
         Addr cur_pc;
         for (cur_pc = start_pc; cur_pc < limit_pc; cur_pc += 4) {
            unsigned long inst, high_word, low_word;
            unsigned long * cur_inst;
            int reg;
            
            cur_inst = (unsigned long *)cur_pc;
            inst = *((UInt *) cur_inst);
            if(debug)
               VG_(printf)("cur_pc: 0x%lx, inst: 0x%lx\n", cur_pc, inst);

            
            high_word = (inst >> 16) & 0xffff;
            low_word = inst & 0xffff;
            reg = high_word & 0x1f;

            if (high_word == 0x27bd        
                || high_word == 0x23bd     
                || high_word == 0x67bd) {  
               if (low_word & 0x8000)	
                  frame_offset += 0x10000 - low_word;
               else
               break;
            seen_sp_adjust = 1;
            }
         }
         if(debug)
            VG_(printf)("offset: 0x%lx\n", frame_offset);
      }
      if (seen_sp_adjust) {
         if (0 == uregs.pc || 1 == uregs.pc) break;
         if (uregs.pc == uregs.ra - 8) break;
         if (sps) {
            sps[i] = uregs.sp + frame_offset;
         }
         uregs.sp = uregs.sp + frame_offset;
         
         if (fps) {
            fps[i] = fps[0];
            uregs.fp = fps[0];
         }
         if (0 == uregs.ra || 1 == uregs.ra) break;
         uregs.pc = uregs.ra - 8;
         ips[i++] = uregs.ra - 8;
         continue;
      }

      if (i == 1) {
         if (sps) {
            sps[i] = sps[0];
            uregs.sp = sps[0];
         }
         if (fps) {
            fps[i] = fps[0];
            uregs.fp = fps[0];
         }
         if (0 == uregs.ra || 1 == uregs.ra) break;
         uregs.pc = uregs.ra - 8;
         ips[i++] = uregs.ra - 8;
         continue;
      }
      
      break;
   }

   n_found = i;
   return n_found;
}

#endif




UInt VG_(get_StackTrace) ( ThreadId tid, 
                           StackTrace ips, UInt max_n_ips,
                           StackTrace sps,
                           StackTrace fps,
                           Word first_ip_delta )
{
   
   UnwindStartRegs startRegs;
   VG_(memset)( &startRegs, 0, sizeof(startRegs) );
   VG_(get_UnwindStartRegs)( &startRegs, tid );

   Addr stack_highest_word = VG_(threads)[tid].client_stack_highest_word;
   Addr stack_lowest_word  = 0;

#  if defined(VGP_x86_linux)
   if (VG_(client__dl_sysinfo_int80) != 0 
       && startRegs.r_pc >= VG_(client__dl_sysinfo_int80)
       && startRegs.r_pc < VG_(client__dl_sysinfo_int80)+3
       && VG_(am_is_valid_for_client)(startRegs.r_pc, sizeof(Addr),
                                      VKI_PROT_READ)) {
      startRegs.r_pc  = (ULong) *(Addr*)(UWord)startRegs.r_sp;
      startRegs.r_sp += (ULong) sizeof(Addr);
   }
#  endif

   
   VG_(stack_limits)( (Addr)startRegs.r_sp,
                      &stack_lowest_word, &stack_highest_word );

   
   startRegs.r_pc += (Long)(Word)first_ip_delta;

   if (0)
      VG_(printf)("tid %d: stack_highest=0x%08lx ip=0x%010llx "
                  "sp=0x%010llx\n",
		  tid, stack_highest_word,
                  startRegs.r_pc, startRegs.r_sp);

   return VG_(get_StackTrace_wrk)(tid, ips, max_n_ips, 
                                       sps, fps,
                                       &startRegs,
                                       stack_highest_word);
}

static void printIpDesc(UInt n, Addr ip, void* uu_opaque)
{
   #define BUF_LEN   4096
   
   static UChar buf[BUF_LEN];

   VG_(describe_IP)(ip, buf, BUF_LEN);

   if (VG_(clo_xml)) {
      VG_(printf_xml)("    %s\n", buf);
   } else {
      VG_(message)(Vg_UserMsg, "   %s %s\n", ( n == 0 ? "at" : "by" ), buf);
   }
}

void VG_(pp_StackTrace) ( StackTrace ips, UInt n_ips )
{
   vg_assert( n_ips > 0 );

   if (VG_(clo_xml))
      VG_(printf_xml)("  <stack>\n");

   VG_(apply_StackTrace)( printIpDesc, NULL, ips, n_ips );

   if (VG_(clo_xml))
      VG_(printf_xml)("  </stack>\n");
}

void VG_(get_and_pp_StackTrace) ( ThreadId tid, UInt max_n_ips )
{
   Addr ips[max_n_ips];
   UInt n_ips
      = VG_(get_StackTrace)(tid, ips, max_n_ips,
                            NULL,
                            NULL,
                            0);
   VG_(pp_StackTrace)(ips, n_ips);
}

void VG_(apply_StackTrace)(
        void(*action)(UInt n, Addr ip, void* opaque),
        void* opaque,
        StackTrace ips, UInt n_ips
     )
{
   Bool main_done = False;
   Int i = 0;

   vg_assert(n_ips > 0);
   do {
      Addr ip = ips[i];

      
      
      
      if ( ! VG_(clo_show_below_main) ) {
         Vg_FnNameKind kind = VG_(get_fnname_kind_from_IP)(ip);
         if (Vg_FnNameMain == kind || Vg_FnNameBelowMain == kind) {
            main_done = True;
         }
      }

      
      action(i, ip, opaque);

      i++;
   } while (i < n_ips && !main_done);

   #undef MYBUF_LEN
}


