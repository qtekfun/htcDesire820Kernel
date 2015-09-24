

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright IBM Corp. 2010-2012

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
*/


#ifndef __VEX_HOST_S390_DEFS_H
#define __VEX_HOST_S390_DEFS_H

#include "libvex_basictypes.h"            
#include "libvex.h"                       
#include "main_util.h"                    
#include "host_generic_regs.h"            

const HChar *s390_hreg_as_string(HReg);

HReg s390_hreg_guest_state_pointer(void);


static __inline__ unsigned
s390_gprno_from_arg_index(unsigned ix)
{
   return ix + 2;
}



typedef enum {
   S390_AMODE_B12,
   S390_AMODE_B20,
   S390_AMODE_BX12,
   S390_AMODE_BX20
} s390_amode_t;

typedef struct {
   s390_amode_t tag;
   HReg b;
   HReg x;       
   Int  d;       
} s390_amode;


s390_amode *s390_amode_b12(Int d, HReg b);
s390_amode *s390_amode_b20(Int d, HReg b);
s390_amode *s390_amode_bx12(Int d, HReg b, HReg x);
s390_amode *s390_amode_bx20(Int d, HReg b, HReg x);
s390_amode *s390_amode_for_guest_state(Int d);
Bool        s390_amode_is_sane(const s390_amode *);

const HChar *s390_amode_as_string(const s390_amode *);


typedef enum {
   S390_OPND_REG,
   S390_OPND_IMMEDIATE,
   S390_OPND_AMODE
} s390_opnd_t;



typedef struct {
   s390_opnd_t tag;
   union {
      HReg        reg;
      s390_amode *am;
      ULong       imm;
   } variant;
} s390_opnd_RMI;


typedef enum {
   S390_INSN_LOAD,   
   S390_INSN_STORE,  
   S390_INSN_MOVE,   
   S390_INSN_COND_MOVE, 
   S390_INSN_LOAD_IMMEDIATE,
   S390_INSN_ALU,
   S390_INSN_MUL,    
   S390_INSN_DIV,    
   S390_INSN_DIVS,   
   S390_INSN_CLZ,    
   S390_INSN_UNOP,
   S390_INSN_TEST,   
   S390_INSN_CC2BOOL,
   S390_INSN_COMPARE,
   S390_INSN_HELPER_CALL,
   S390_INSN_CAS,    
   S390_INSN_CDAS,   
   S390_INSN_BFP_BINOP, 
   S390_INSN_BFP_UNOP,
   S390_INSN_BFP_TRIOP,
   S390_INSN_BFP_COMPARE,
   S390_INSN_BFP128_BINOP, 
   S390_INSN_BFP128_UNOP,
   S390_INSN_BFP128_COMPARE,
   S390_INSN_BFP128_CONVERT_TO,
   S390_INSN_BFP128_CONVERT_FROM,
   S390_INSN_MFENCE,
   S390_INSN_GZERO,   
   S390_INSN_GADD,    
   
   S390_INSN_XDIRECT,     
   S390_INSN_XINDIR,      
   S390_INSN_XASSISTED,   
   S390_INSN_EVCHECK,     
   S390_INSN_PROFINC      
} s390_insn_tag;


typedef enum {
   S390_ALU_ADD,
   S390_ALU_SUB,
   S390_ALU_MUL,   
   S390_ALU_AND,
   S390_ALU_OR,
   S390_ALU_XOR,
   S390_ALU_LSH,
   S390_ALU_RSH,
   S390_ALU_RSHA   
} s390_alu_t;


typedef enum {
   S390_ZERO_EXTEND_8,
   S390_ZERO_EXTEND_16,
   S390_ZERO_EXTEND_32,
   S390_SIGN_EXTEND_8,
   S390_SIGN_EXTEND_16,
   S390_SIGN_EXTEND_32,
   S390_NEGATE
} s390_unop_t;

typedef enum {
   S390_BFP_MADD,
   S390_BFP_MSUB,
} s390_bfp_triop_t;

typedef enum {
   S390_BFP_ADD,
   S390_BFP_SUB,
   S390_BFP_MUL,
   S390_BFP_DIV
} s390_bfp_binop_t;


typedef enum {
   S390_BFP_ABS,
   S390_BFP_NABS,
   S390_BFP_NEG,
   S390_BFP_SQRT,
   S390_BFP_I32_TO_F32,
   S390_BFP_I32_TO_F64,
   S390_BFP_I32_TO_F128,
   S390_BFP_I64_TO_F32,
   S390_BFP_I64_TO_F64,
   S390_BFP_I64_TO_F128,
   S390_BFP_F32_TO_I32,
   S390_BFP_F32_TO_I64,
   S390_BFP_F32_TO_F64,
   S390_BFP_F32_TO_F128,
   S390_BFP_F64_TO_I32,
   S390_BFP_F64_TO_I64,
   S390_BFP_F64_TO_F32,
   S390_BFP_F64_TO_F128,
   S390_BFP_F128_TO_I32,
   S390_BFP_F128_TO_I64,
   S390_BFP_F128_TO_F32,
   S390_BFP_F128_TO_F64
} s390_bfp_unop_t;


typedef enum {
   S390_CC_NEVER=  0,
   S390_CC_OVFL =  1,   
   S390_CC_H    =  2,   
   S390_CC_NLE  =  3,   
   S390_CC_L    =  4,   
   S390_CC_NHE  =  5,   
   S390_CC_LH   =  6,   
   S390_CC_NE   =  7,   
   S390_CC_E    =  8,   
   S390_CC_NLH  =  9,   
   S390_CC_HE   = 10,   
   S390_CC_NL   = 11,   
   S390_CC_LE   = 12,   
   S390_CC_NH   = 13,   
   S390_CC_NO   = 14,   
   S390_CC_ALWAYS = 15
} s390_cc_t;


typedef enum {
   S390_ROUND_NEAREST_EVEN = 4,
   S390_ROUND_ZERO         = 5,
   S390_ROUND_POSINF       = 6,
   S390_ROUND_NEGINF       = 7
} s390_round_t;


static __inline__ s390_cc_t
s390_cc_invert(s390_cc_t cond)
{
   return S390_CC_ALWAYS - cond;
}


typedef struct {
   s390_insn_tag tag;
   UChar size;            
   union {
      struct {
         HReg        dst;
         s390_amode *src;
      } load;
      struct {
         s390_amode *dst;
         HReg        src;
      } store;
      struct {
         HReg        dst;
         HReg        src;
      } move;
      struct {
         s390_cc_t     cond;
         HReg          dst;
         s390_opnd_RMI src;
      } cond_move;
      struct {
         HReg        dst;
         ULong       value;  
      } load_immediate;
      
      struct {
         s390_alu_t    tag;
         HReg          dst; 
         s390_opnd_RMI op2;
      } alu;
      struct {
         Bool          signed_multiply;
         HReg          dst_hi;  
         HReg          dst_lo;  
         s390_opnd_RMI op2;
      } mul;
      struct {
         Bool          signed_divide;
         HReg          op1_hi;  
         HReg          op1_lo;  
         s390_opnd_RMI op2;
      } div;
      struct {
         HReg          rem; 
         HReg          op1; 
         s390_opnd_RMI op2;
      } divs;
      struct {
         HReg          num_bits; 
         HReg          clobber;  
         s390_opnd_RMI src;
      } clz;
      struct {
         s390_unop_t   tag;
         HReg          dst;
         s390_opnd_RMI src;
      } unop;
      struct {
         Bool          signed_comparison;
         HReg          src1;
         s390_opnd_RMI src2;
      } compare;
      struct {
         HReg          dst;  
         HReg          op1;
         HReg          op2;
      } bfp_compare;
      struct {
         s390_opnd_RMI src;
      } test;
      
      struct {
         s390_cc_t cond;
         HReg      dst;
      } cc2bool;
      struct {
         HReg        op1;
         s390_amode *op2;
         HReg        op3;
         HReg        old_mem;
      } cas;
      struct {
         HReg        op1_high;
         HReg        op1_low;
         s390_amode *op2;
         HReg        op3_high;
         HReg        op3_low;
         HReg        old_mem_high;
         HReg        old_mem_low;
         HReg        scratch;
      } cdas;
      struct {
         s390_cc_t cond;
         Addr64    target;
         UInt      num_args;
         HReg      dst;       
         HChar    *name;      
      } helper_call;
      struct {
         s390_bfp_triop_t tag;
         s390_round_t     rounding_mode;
         HReg             dst; 
         HReg             op2; 
         HReg             op3; 
      } bfp_triop;
      struct {
         s390_bfp_binop_t tag;
         s390_round_t     rounding_mode;
         HReg             dst; 
         HReg             op2; 
      } bfp_binop;
      struct {
         s390_bfp_unop_t tag;
         s390_round_t    rounding_mode;
         HReg            dst;  
         HReg            op;   
      } bfp_unop;
      struct {
         s390_bfp_binop_t tag;
         s390_round_t     rounding_mode;
         HReg             dst_hi; 
         HReg             dst_lo; 
         HReg             op2_hi; 
         HReg             op2_lo; 
      } bfp128_binop;
      struct {
         s390_bfp_unop_t  tag;
         s390_round_t     rounding_mode;
         HReg             dst_hi; 
         HReg             dst_lo; 
         HReg             op_hi;  
         HReg             op_lo;  
      } bfp128_unop;
      struct {
         HReg             dst;    
         HReg             op1_hi; 
         HReg             op1_lo; 
         HReg             op2_hi; 
         HReg             op2_lo; 
      } bfp128_compare;
      struct {
         UInt             offset;
      } gzero;
      struct {
         UInt             offset;
         UChar            delta;
         ULong            value;  
      } gadd;

      

      struct {
         s390_cc_t     cond;
         Bool          to_fast_entry;  
         Addr64        dst;            
         s390_amode   *guest_IA;
      } xdirect;
      struct {
         s390_cc_t     cond;
         HReg          dst;
         s390_amode   *guest_IA;
      } xindir;
      struct {
         s390_cc_t     cond;
         IRJumpKind    kind;
         HReg          dst;
         s390_amode   *guest_IA;
      } xassisted;
      struct {
         s390_amode   *counter;    
         s390_amode   *fail_addr;
      } evcheck;
      struct {
      } profinc;

   } variant;
} s390_insn;

s390_insn *s390_insn_load(UChar size, HReg dst, s390_amode *src);
s390_insn *s390_insn_store(UChar size, s390_amode *dst, HReg src);
s390_insn *s390_insn_move(UChar size, HReg dst, HReg src);
s390_insn *s390_insn_cond_move(UChar size, s390_cc_t cond, HReg dst,
                               s390_opnd_RMI src);
s390_insn *s390_insn_load_immediate(UChar size, HReg dst, ULong val);
s390_insn *s390_insn_alu(UChar size, s390_alu_t, HReg dst,
                         s390_opnd_RMI op2);
s390_insn *s390_insn_mul(UChar size, HReg dst_hi, HReg dst_lo,
                         s390_opnd_RMI op2, Bool signed_multiply);
s390_insn *s390_insn_div(UChar size, HReg op1_hi, HReg op1_lo,
                         s390_opnd_RMI op2, Bool signed_divide);
s390_insn *s390_insn_divs(UChar size, HReg rem, HReg op1, s390_opnd_RMI op2);
s390_insn *s390_insn_clz(UChar size, HReg num_bits, HReg clobber,
                         s390_opnd_RMI op);
s390_insn *s390_insn_cas(UChar size, HReg op1, s390_amode *op2, HReg op3,
                         HReg old);
s390_insn *s390_insn_cdas(UChar size, HReg op1_high, HReg op1_low,
                          s390_amode *op2, HReg op3_high, HReg op3_low,
                          HReg old_high, HReg old_low, HReg scratch);
s390_insn *s390_insn_unop(UChar size, s390_unop_t tag, HReg dst,
                          s390_opnd_RMI opnd);
s390_insn *s390_insn_cc2bool(HReg dst, s390_cc_t src);
s390_insn *s390_insn_test(UChar size, s390_opnd_RMI src);
s390_insn *s390_insn_compare(UChar size, HReg dst, s390_opnd_RMI opnd,
                             Bool signed_comparison);
s390_insn *s390_insn_helper_call(s390_cc_t cond, Addr64 target, UInt num_args,
                                 HChar *name, HReg dst);
s390_insn *s390_insn_bfp_triop(UChar size, s390_bfp_triop_t, HReg dst, HReg op2,
                               HReg op3, s390_round_t);
s390_insn *s390_insn_bfp_binop(UChar size, s390_bfp_binop_t, HReg dst, HReg op2,
                               s390_round_t);
s390_insn *s390_insn_bfp_unop(UChar size, s390_bfp_unop_t tag, HReg dst,
                              HReg op, s390_round_t);
s390_insn *s390_insn_bfp_compare(UChar size, HReg dst, HReg op1, HReg op2);
s390_insn *s390_insn_bfp128_binop(UChar size, s390_bfp_binop_t, HReg dst_hi,
                                  HReg dst_lo, HReg op2_hi, HReg op2_lo,
                                  s390_round_t);
s390_insn *s390_insn_bfp128_unop(UChar size, s390_bfp_unop_t, HReg dst_hi,
                                 HReg dst_lo, HReg op_hi, HReg op_lo,
                                 s390_round_t);
s390_insn *s390_insn_bfp128_compare(UChar size, HReg dst, HReg op1_hi,
                                    HReg op1_lo, HReg op2_hi, HReg op2_lo);
s390_insn *s390_insn_bfp128_convert_to(UChar size, s390_bfp_unop_t,
                                       HReg dst_hi, HReg dst_lo, HReg op);
s390_insn *s390_insn_bfp128_convert_from(UChar size, s390_bfp_unop_t,
                                         HReg dst, HReg op_hi, HReg op_lo,
                                         s390_round_t);
s390_insn *s390_insn_mfence(void);
s390_insn *s390_insn_gzero(UChar size, UInt offset);
s390_insn *s390_insn_gadd(UChar size, UInt offset, UChar delta, ULong value);

s390_insn *s390_insn_xdirect(s390_cc_t cond, Addr64 dst, s390_amode *guest_IA,
                             Bool to_fast_entry);
s390_insn *s390_insn_xindir(s390_cc_t cond, HReg dst, s390_amode *guest_IA);
s390_insn *s390_insn_xassisted(s390_cc_t cond, HReg dst, s390_amode *guest_IA,
                               IRJumpKind kind);
s390_insn *s390_insn_evcheck(s390_amode *counter, s390_amode *fail_addr);
s390_insn *s390_insn_profinc(void);

const HChar *s390_insn_as_string(const s390_insn *);


void ppS390AMode(s390_amode *);
void ppS390Instr(s390_insn *, Bool mode64);
void ppHRegS390(HReg);

void  getRegUsage_S390Instr( HRegUsage *, s390_insn *, Bool );
void  mapRegs_S390Instr    ( HRegRemap *, s390_insn *, Bool );
Bool  isMove_S390Instr     ( s390_insn *, HReg *, HReg * );
Int   emit_S390Instr       ( Bool *, UChar *, Int, s390_insn *, Bool,
                             void *, void *, void *, void *);
void  getAllocableRegs_S390( Int *, HReg **, Bool );
void  genSpill_S390        ( HInstr **, HInstr **, HReg , Int , Bool );
void  genReload_S390       ( HInstr **, HInstr **, HReg , Int , Bool );
s390_insn *directReload_S390 ( s390_insn *, HReg, Short );
HInstrArray *iselSB_S390   ( IRSB *, VexArch, VexArchInfo *, VexAbiInfo *,
                             Int, Int, Bool, Bool, Addr64);

Int evCheckSzB_S390(void);

VexInvalRange chainXDirect_S390(void *place_to_chain,
                                void *disp_cp_chain_me_EXPECTED,
                                void *place_to_jump_to);

VexInvalRange unchainXDirect_S390(void *place_to_unchain,
                                  void *place_to_jump_to_EXPECTED,
                                  void *disp_cp_chain_me);

VexInvalRange patchProfInc_S390(void  *code_to_patch,
                                ULong *location_of_counter);

extern UInt s390_host_hwcaps;

#define s390_host_has_ldisp \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_LDISP))
#define s390_host_has_eimm \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_EIMM))
#define s390_host_has_gie \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_GIE))
#define s390_host_has_dfp \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_DFP))
#define s390_host_has_fgx \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_FGX))
#define s390_host_has_etf2 \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_ETF2))
#define s390_host_has_stfle \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_STFLE))
#define s390_host_has_etf3 \
                      (s390_host_hwcaps & (VEX_HWCAPS_S390X_ETF3))

#endif 

