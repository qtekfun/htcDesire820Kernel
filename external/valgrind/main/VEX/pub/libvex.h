

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2004-2012 OpenWorks LLP
      info@open-works.net

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   The GNU General Public License is contained in the file COPYING.

   Neither the names of the U.S. Department of Energy nor the
   University of California nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without prior written permission.
*/

#ifndef __LIBVEX_H
#define __LIBVEX_H


#include "libvex_basictypes.h"
#include "libvex_ir.h"




typedef 
   enum { 
      VexArch_INVALID,
      VexArchX86, 
      VexArchAMD64, 
      VexArchARM,
      VexArchPPC32,
      VexArchPPC64,
      VexArchS390X,
      VexArchMIPS32
   }
   VexArch;



#define VEX_HWCAPS_X86_SSE1    (1<<1)  
#define VEX_HWCAPS_X86_SSE2    (1<<2)  
#define VEX_HWCAPS_X86_SSE3    (1<<3)  
#define VEX_HWCAPS_X86_LZCNT   (1<<4)  

#define VEX_HWCAPS_AMD64_SSE3  (1<<5)  
#define VEX_HWCAPS_AMD64_CX16  (1<<6)  
#define VEX_HWCAPS_AMD64_LZCNT (1<<7)  
#define VEX_HWCAPS_AMD64_AVX   (1<<8)  

#define VEX_HWCAPS_PPC32_F     (1<<8)  
#define VEX_HWCAPS_PPC32_V     (1<<9)  
#define VEX_HWCAPS_PPC32_FX    (1<<10) 
#define VEX_HWCAPS_PPC32_GX    (1<<11) 
#define VEX_HWCAPS_PPC32_VX    (1<<12) 
#define VEX_HWCAPS_PPC32_DFP   (1<<17) 

#define VEX_HWCAPS_PPC64_V     (1<<13) 
#define VEX_HWCAPS_PPC64_FX    (1<<14) 
#define VEX_HWCAPS_PPC64_GX    (1<<15) 
#define VEX_HWCAPS_PPC64_VX    (1<<16) 
#define VEX_HWCAPS_PPC64_DFP   (1<<18) 


#define VEX_S390X_MODEL_Z900     0
#define VEX_S390X_MODEL_Z800     1
#define VEX_S390X_MODEL_Z990     2
#define VEX_S390X_MODEL_Z890     3
#define VEX_S390X_MODEL_Z9_EC    4
#define VEX_S390X_MODEL_Z9_BC    5
#define VEX_S390X_MODEL_Z10_EC   6
#define VEX_S390X_MODEL_Z10_BC   7
#define VEX_S390X_MODEL_Z196     8
#define VEX_S390X_MODEL_Z114     9
#define VEX_S390X_MODEL_UNKNOWN  10     
#define VEX_S390X_MODEL_MASK     0x3F

#define VEX_HWCAPS_S390X_LDISP (1<<6)   
#define VEX_HWCAPS_S390X_EIMM  (1<<7)   
#define VEX_HWCAPS_S390X_GIE   (1<<8)   
#define VEX_HWCAPS_S390X_DFP   (1<<9)   
#define VEX_HWCAPS_S390X_FGX   (1<<10)  
#define VEX_HWCAPS_S390X_ETF2  (1<<11)  
#define VEX_HWCAPS_S390X_STFLE (1<<12)  
#define VEX_HWCAPS_S390X_ETF3  (1<<13)  

#define VEX_HWCAPS_S390X_ALL   (VEX_HWCAPS_S390X_LDISP | \
                                VEX_HWCAPS_S390X_EIMM  | \
                                VEX_HWCAPS_S390X_GIE   | \
                                VEX_HWCAPS_S390X_DFP   | \
                                VEX_HWCAPS_S390X_FGX   | \
                                VEX_HWCAPS_S390X_STFLE | \
                                VEX_HWCAPS_S390X_ETF3  | \
                                VEX_HWCAPS_S390X_ETF2)

#define VEX_HWCAPS_S390X(x)  ((x) & ~VEX_S390X_MODEL_MASK)
#define VEX_S390X_MODEL(x)   ((x) &  VEX_S390X_MODEL_MASK)

#define VEX_HWCAPS_ARM_VFP    (1<<6)  
#define VEX_HWCAPS_ARM_VFP2   (1<<7)  
#define VEX_HWCAPS_ARM_VFP3   (1<<8)  
#define VEX_HWCAPS_ARM_NEON   (1<<16) 

#define VEX_ARM_ARCHLEVEL(x) ((x) & 0x3f)


#define VEX_PRID_COMP_MIPS      0x00010000
#define VEX_PRID_COMP_BROADCOM  0x00020000


extern const HChar* LibVEX_ppVexArch    ( VexArch );
extern const HChar* LibVEX_ppVexHwCaps  ( VexArch, UInt );



typedef
   struct {
      
      UInt hwcaps;
      
      Int ppc_cache_line_szB;
      UInt ppc_dcbz_szB;
      UInt ppc_dcbzl_szB; 
   }
   VexArchInfo;

extern 
void LibVEX_default_VexArchInfo ( VexArchInfo* vai );



typedef
   struct {
      Int guest_stack_redzone_size;

      Bool guest_amd64_assume_fs_is_zero;

      Bool guest_amd64_assume_gs_is_0x60;

      Bool guest_ppc_zap_RZ_at_blr;

      Bool (*guest_ppc_zap_RZ_at_bl)(Addr64);

      Bool guest_ppc_sc_continues_at_LR;

      Bool host_ppc_calls_use_fndescrs;

      Bool host_ppc32_regalign_int64_args;
   }
   VexAbiInfo;

extern 
void LibVEX_default_VexAbiInfo ( VexAbiInfo* vbi );




typedef enum { VexRegUpdUnwindregsAtMemAccess,
               VexRegUpdAllregsAtMemAccess,
               VexRegUpdAllregsAtEachInsn } VexRegisterUpdates;


typedef
   struct {
      
      Int iropt_verbosity;
      Int iropt_level;
      
      VexRegisterUpdates iropt_register_updates;
      Int iropt_unroll_thresh;
      Int guest_max_insns;
      Int guest_chase_thresh;
      Bool guest_chase_cond;
   }
   VexControl;



extern 
void LibVEX_default_VexControl (  VexControl* vcon );



extern HChar* private_LibVEX_alloc_first;
extern HChar* private_LibVEX_alloc_curr;
extern HChar* private_LibVEX_alloc_last;
extern void   private_LibVEX_alloc_OOM(void) __attribute__((noreturn));

static inline void* LibVEX_Alloc ( Int nbytes )
{
   struct align {
      char c;
      union {
         char c;
         short s;
         int i;
         long l;
         long long ll;
         float f;
         double d;
         
         void *pto;
         void (*ptf)(void);
      } x;
   };

#if 0
  
  return malloc(nbytes);
#else
   HChar* curr;
   HChar* next;
   Int    ALIGN;
   ALIGN  = offsetof(struct align,x) - 1;
   nbytes = (nbytes + ALIGN) & ~ALIGN;
   curr   = private_LibVEX_alloc_curr;
   next   = curr + nbytes;
   if (next >= private_LibVEX_alloc_last)
      private_LibVEX_alloc_OOM();
   private_LibVEX_alloc_curr = next;
   return curr;
#endif
}

extern void LibVEX_ShowAllocStats ( void );




#define VEXGLO_N_ALWAYSDEFD  24

typedef
   struct {
      Int total_sizeB;
      
      Int offset_SP;
      Int sizeof_SP; 
      
      Int offset_FP;
      Int sizeof_FP; 
      
      Int offset_IP;
      Int sizeof_IP; 
      Int n_alwaysDefd;
      struct {
         Int offset;
         Int size;
      } alwaysDefd[VEXGLO_N_ALWAYSDEFD];
   }
   VexGuestLayout;


#define LibVEX_N_SPILL_BYTES 4096




extern void LibVEX_Init (

   
#  if __cplusplus == 1 && __GNUC__ && __GNUC__ <= 3
#  else
   __attribute__ ((noreturn))
#  endif
   void (*failure_exit) ( void ),

   
   void (*log_bytes) ( HChar*, Int nbytes ),

   
   Int debuglevel,

   
   Bool valgrind_support,

   
   VexControl* vcon
);



typedef
   struct {
      
      enum { VexTransOK,
             VexTransAccessFail, VexTransOutputFull } status;
      
      UInt n_sc_extents;
      Int offs_profInc;
      UInt n_guest_instrs;
   }
   VexTranslateResult;


typedef
   struct {
      Addr64 base[3];
      UShort len[3];
      UShort n_used;
   }
   VexGuestExtents;


typedef
   struct {
      VexArch      arch_guest;
      VexArchInfo  archinfo_guest;
      VexArch      arch_host;
      VexArchInfo  archinfo_host;
      VexAbiInfo   abiinfo_both;

      void*   callback_opaque;

      
      
      UChar*  guest_bytes;
      Addr64  guest_bytes_addr;

      Bool    (*chase_into_ok) ( void*, Addr64 );

      
      VexGuestExtents* guest_extents;

      
      UChar*  host_bytes;
      Int     host_bytes_size;
      
      Int*    host_bytes_used;

      IRSB*   (*instrument1) ( void*, 
                               IRSB*, 
                               VexGuestLayout*, 
                               VexGuestExtents*,
                               IRType gWordTy, IRType hWordTy );
      IRSB*   (*instrument2) ( void*, 
                               IRSB*, 
                               VexGuestLayout*, 
                               VexGuestExtents*,
                               IRType gWordTy, IRType hWordTy );

      IRSB* (*finaltidy) ( IRSB* );

      UInt (*needs_self_check)( void*,
                                VexGuestExtents* );

      Bool    (*preamble_function)(void*, IRSB*);

      
      Int     traceflags;

      Bool    addProfInc;

      void* disp_cp_chain_me_to_slowEP;
      void* disp_cp_chain_me_to_fastEP;
      void* disp_cp_xindir;
      void* disp_cp_xassisted;
   }
   VexTranslateArgs;


extern 
VexTranslateResult LibVEX_Translate ( VexTranslateArgs* );




typedef
   struct {
      HWord start;
      HWord len;
   }
   VexInvalRange;

extern
VexInvalRange LibVEX_Chain ( VexArch arch_host,
                             void*   place_to_chain,
                             void*   disp_cp_chain_me_EXPECTED,
                             void*   place_to_jump_to );

extern
VexInvalRange LibVEX_UnChain ( VexArch arch_host,
                               void*   place_to_unchain,
                               void*   place_to_jump_to_EXPECTED,
                               void*   disp_cp_chain_me );

extern
Int LibVEX_evCheckSzB ( VexArch arch_host );


extern
VexInvalRange LibVEX_PatchProfInc ( VexArch arch_host,
                                    void*   place_to_patch,
                                    ULong*  location_of_counter );



extern void LibVEX_ShowStats ( void );



#endif 

