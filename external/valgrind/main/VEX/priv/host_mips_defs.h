

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2010-2012 RT-RK
      mips-valgrind@rt-rk.com

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

#ifndef __VEX_HOST_MIPS_DEFS_H
#define __VEX_HOST_MIPS_DEFS_H

#define MIPS_N_REGPARMS 4



extern void ppHRegMIPS(HReg, Bool);

extern HReg hregMIPS_GPR0(Bool mode64);   
extern HReg hregMIPS_GPR1(Bool mode64);   
extern HReg hregMIPS_GPR2(Bool mode64);   
extern HReg hregMIPS_GPR3(Bool mode64);
extern HReg hregMIPS_GPR4(Bool mode64);
extern HReg hregMIPS_GPR5(Bool mode64);
extern HReg hregMIPS_GPR6(Bool mode64);
extern HReg hregMIPS_GPR7(Bool mode64);
extern HReg hregMIPS_GPR8(Bool mode64);
extern HReg hregMIPS_GPR9(Bool mode64);
extern HReg hregMIPS_GPR10(Bool mode64);
extern HReg hregMIPS_GPR11(Bool mode64);
extern HReg hregMIPS_GPR12(Bool mode64);
extern HReg hregMIPS_GPR13(Bool mode64);
extern HReg hregMIPS_GPR14(Bool mode64);
extern HReg hregMIPS_GPR15(Bool mode64);
extern HReg hregMIPS_GPR16(Bool mode64);
extern HReg hregMIPS_GPR17(Bool mode64);
extern HReg hregMIPS_GPR18(Bool mode64);
extern HReg hregMIPS_GPR19(Bool mode64);
extern HReg hregMIPS_GPR20(Bool mode64);
extern HReg hregMIPS_GPR21(Bool mode64);
extern HReg hregMIPS_GPR22(Bool mode64);
extern HReg hregMIPS_GPR23(Bool mode64);  
extern HReg hregMIPS_GPR24(Bool mode64);  
extern HReg hregMIPS_GPR25(Bool mode64);
extern HReg hregMIPS_GPR26(Bool mode64);
extern HReg hregMIPS_GPR27(Bool mode64);
extern HReg hregMIPS_GPR28(Bool mode64);
extern HReg hregMIPS_GPR29(Bool mode64);
extern HReg hregMIPS_GPR30(Bool mode64);
extern HReg hregMIPS_GPR31(Bool mode64);
extern HReg hregMIPS_PC(Bool mode64);

extern HReg hregMIPS_HI(Bool mode64);
extern HReg hregMIPS_LO(Bool mode64);

extern HReg hregMIPS_F0(Bool mode64);
extern HReg hregMIPS_F1(Bool mode64);
extern HReg hregMIPS_F2(Bool mode64);
extern HReg hregMIPS_F3(Bool mode64);
extern HReg hregMIPS_F4(Bool mode64);
extern HReg hregMIPS_F5(Bool mode64);
extern HReg hregMIPS_F6(Bool mode64);
extern HReg hregMIPS_F7(Bool mode64);
extern HReg hregMIPS_F8(Bool mode64);
extern HReg hregMIPS_F9(Bool mode64);
extern HReg hregMIPS_F10(Bool mode64);
extern HReg hregMIPS_F11(Bool mode64);
extern HReg hregMIPS_F12(Bool mode64);
extern HReg hregMIPS_F13(Bool mode64);
extern HReg hregMIPS_F14(Bool mode64);
extern HReg hregMIPS_F15(Bool mode64);
extern HReg hregMIPS_F16(Bool mode64);
extern HReg hregMIPS_F17(Bool mode64);
extern HReg hregMIPS_F18(Bool mode64);
extern HReg hregMIPS_F19(Bool mode64);
extern HReg hregMIPS_F20(Bool mode64);
extern HReg hregMIPS_F21(Bool mode64);
extern HReg hregMIPS_F22(Bool mode64);
extern HReg hregMIPS_F23(Bool mode64);
extern HReg hregMIPS_F24(Bool mode64);
extern HReg hregMIPS_F25(Bool mode64);
extern HReg hregMIPS_F26(Bool mode64);
extern HReg hregMIPS_F27(Bool mode64);
extern HReg hregMIPS_F28(Bool mode64);
extern HReg hregMIPS_F29(Bool mode64);
extern HReg hregMIPS_F30(Bool mode64);
extern HReg hregMIPS_F31(Bool mode64);
extern HReg hregMIPS_FIR(void);
extern HReg hregMIPS_FCCR(void);
extern HReg hregMIPS_FEXR(void);
extern HReg hregMIPS_FENR(void);
extern HReg hregMIPS_FCSR(void);
extern HReg hregMIPS_COND(void);

extern HReg hregMIPS_D0(void);
extern HReg hregMIPS_D1(void);
extern HReg hregMIPS_D2(void);
extern HReg hregMIPS_D3(void);
extern HReg hregMIPS_D4(void);
extern HReg hregMIPS_D5(void);
extern HReg hregMIPS_D6(void);
extern HReg hregMIPS_D7(void);
extern HReg hregMIPS_D8(void);
extern HReg hregMIPS_D9(void);
extern HReg hregMIPS_D10(void);
extern HReg hregMIPS_D11(void);
extern HReg hregMIPS_D12(void);
extern HReg hregMIPS_D13(void);
extern HReg hregMIPS_D14(void);
extern HReg hregMIPS_D15(void);

#define GuestStatePointer(_mode64)     hregMIPS_GPR10(_mode64)

#define StackFramePointer(_mode64)     hregMIPS_GPR30(_mode64)
#define LinkRegister(_mode64)          hregMIPS_GPR31(_mode64)
#define StackPointer(_mode64)          hregMIPS_GPR29(_mode64)
#define FCSR()                         hregMIPS_FCSR()
#define COND()                         hregMIPS_COND()

#define HIRegister(_mode64)        hregMIPS_HI(_mode64)
#define LORegister(_mode64)        hregMIPS_LO(_mode64)

#define MIPS_N_ARGREGS 4

typedef enum {
   MIPScc_EQ = 0,    
   MIPScc_NE = 1,    

   MIPScc_HS = 2,    
   MIPScc_LO = 3,    

   MIPScc_MI = 4,    
   MIPScc_PL = 5,    

   MIPScc_VS = 6,    
   MIPScc_VC = 7,    

   MIPScc_HI = 8,    
   MIPScc_LS = 9,    

   MIPScc_GE = 10,      
   MIPScc_LT = 11,      

   MIPScc_GT = 12,      
   MIPScc_LE = 13,      

   MIPScc_AL = 14,      
   MIPScc_NV = 15    
} MIPSCondCode;

extern HChar *showMIPSCondCode(MIPSCondCode);

typedef enum {
   Mam_IR,        
   Mam_RR         
} MIPSAModeTag;

typedef struct {
   MIPSAModeTag tag;
   union {
      struct {
         HReg base;
         Int index;
      } IR;
      struct {
         HReg base;
         HReg index;
      } RR;
   } Mam;
} MIPSAMode;

extern MIPSAMode *MIPSAMode_IR(Int, HReg);
extern MIPSAMode *MIPSAMode_RR(HReg, HReg);

extern MIPSAMode *dopyMIPSAMode(MIPSAMode *);
extern MIPSAMode *nextMIPSAModeFloat(MIPSAMode *);
extern MIPSAMode *nextMIPSAModeInt(MIPSAMode *);

extern void ppMIPSAMode(MIPSAMode *, Bool);

typedef enum {
   Mrh_Imm,
   Mrh_Reg
} MIPSRHTag;

typedef struct {
   MIPSRHTag tag;
   union {
      struct {
         Bool syned;
         UShort imm16;
      } Imm;
      struct {
         HReg reg;
      } Reg;
   } Mrh;
} MIPSRH;

extern void ppMIPSRH(MIPSRH *, Bool);

extern MIPSRH *MIPSRH_Imm(Bool, UShort);
extern MIPSRH *MIPSRH_Reg(HReg);

typedef struct {
   HReg reg;
   Int simm11;
} MIPSAModeV;

extern MIPSAModeV *mkMIPSAModeV(HReg reg, Int simm11);

extern void ppMIPSAModeV(MIPSAModeV *);


typedef enum {
   MIPSri84_I84 = 5, 
   MIPSri84_R     
} MIPSRI84Tag;

typedef struct {
   MIPSRI84Tag tag;
   union {
      struct {
         UShort imm8;
         UShort imm4;
      } I84;
      struct {
         HReg reg;
      } R;
   } MIPSri84;
} MIPSRI84;

extern MIPSRI84 *MIPSRI84_I84(UShort imm8, UShort imm4);
extern MIPSRI84 *MIPSRI84_R(HReg);

extern void ppMIPSRI84(MIPSRI84 *);

typedef enum {
   MIPSri5_I5 = 7,      
   MIPSri5_R      
} MIPSRI5Tag;

typedef struct {
   MIPSRI5Tag tag;
   union {
      struct {
         UInt imm5;
      } I5;
      struct {
         HReg reg;
      } R;
   } MIPSri5;
} MIPSRI5;

extern MIPSRI5 *MIPSRI5_I5(UInt imm5);
extern MIPSRI5 *MIPSRI5_R(HReg);

extern void ppMIPSRI5(MIPSRI5 *);



typedef enum {
   Mun_CLO,
   Mun_CLZ,
   Mun_NOP,
} MIPSUnaryOp;

extern HChar *showMIPSUnaryOp(MIPSUnaryOp);


typedef enum {
   Malu_INVALID,
   Malu_ADD, Malu_SUB,
   Malu_AND, Malu_OR, Malu_NOR, Malu_XOR,
} MIPSAluOp;

extern HChar *showMIPSAluOp(MIPSAluOp,
                            Bool  );

typedef enum {
   Mshft_INVALID,
   Mshft_SLL, Mshft_SRL,
   Mshft_SRA
} MIPSShftOp;

extern HChar *showMIPSShftOp(MIPSShftOp,
                             Bool  ,
                             Bool  );

typedef enum {
   Macc_ADD,
   Macc_SUB
} MIPSMaccOp;

extern HChar *showMIPSMaccOp(MIPSMaccOp, Bool);

typedef enum {
   Min_LI,        
   Min_Alu,    
   Min_Shft,      
   Min_Unary,     

   Min_Cmp,    

   Min_Mul,    
   Min_Div,    

   Min_Call,      

   
   Min_XDirect,     
   Min_XIndir,      
   Min_XAssisted,   
   Min_EvCheck,     
   Min_ProfInc,     

   Min_RdWrLR,    
   Min_Mthi,      
   Min_Mtlo,      
   Min_Mfhi,      
   Min_Mflo,      
   Min_Macc,      

   Min_Load,      
   Min_Store,     
   Min_LoadL,     
   Min_StoreC,    

   Min_FpUnary,      
   Min_FpBinary,     
   Min_FpConvert,    
   Min_FpMulAcc,     
   Min_FpLdSt,    
   Min_FpSTFIW,      
   Min_FpRSP,     
   Min_FpCftI,    
   Min_FpCMov,    
   Min_MtFCSR,    
   Min_MfFCSR,    
   Min_FpCompare,    
   Min_MovCond
} MIPSInstrTag;

typedef enum {
   Mfp_INVALID,

   
   Mfp_MADDD, Mfp_MSUBD,
   Mfp_MADDS, Mfp_MSUBS,

   
   Mfp_ADDD, Mfp_SUBD, Mfp_MULD, Mfp_DIVD,
   Mfp_ADDS, Mfp_SUBS, Mfp_MULS, Mfp_DIVS, Mfp_CVTSD, Mfp_CVTSW, Mfp_CVTWD,
   Mfp_CVTWS, Mfp_TRULS, Mfp_TRULD, Mfp_TRUWS, Mfp_TRUWD, Mfp_FLOORWS,
   Mfp_FLOORWD, Mfp_ROUNDWS, Mfp_ROUNDWD, Mfp_CVTDW, Mfp_CMP,
   Mfp_CEILWS, Mfp_CEILWD, Mfp_CEILLS, Mfp_CEILLD,

   
   Mfp_SQRTS, Mfp_SQRTD, Mfp_RSQRTS, Mfp_RSQRTD, Mfp_RECIPS, Mfp_RECIPD,
   Mfp_ABSS, Mfp_ABSD, Mfp_NEGS, Mfp_NEGD, Mfp_MOVS, Mfp_MOVD,
   Mfp_RES, Mfp_RSQRTE, Mfp_FRIN, Mfp_FRIM, Mfp_FRIP, Mfp_FRIZ, Mfp_CVTD
} MIPSFpOp;

extern HChar *showMIPSFpOp(MIPSFpOp);


typedef struct {
   MIPSInstrTag tag;
   union {
      struct {
         HReg dst;
         ULong imm;
      } LI;
      struct {
         MIPSAluOp op;
         HReg dst;
         HReg srcL;
         MIPSRH *srcR;
      } Alu;
      struct {
         MIPSShftOp op;
         Bool sz32;  
         HReg dst;
         HReg srcL;
         MIPSRH *srcR;
      } Shft;
      
      struct {
         MIPSUnaryOp op;
         HReg dst;
         HReg src;
      } Unary;
      
      struct {
         Bool syned;
         Bool sz32;
         HReg dst;
         HReg srcL;
         HReg srcR;

         MIPSCondCode cond;
      } Cmp;
      struct {
         Bool widening; 
         Bool syned; 
         Bool sz32;
         HReg dst;
         HReg srcL;
         HReg srcR;
      } Mul;
      struct {
         Bool syned; 
         Bool sz32;
         HReg srcL;
         HReg srcR;
      } Div;
      struct {
         MIPSCondCode cond;
         Addr32 target;
         UInt argiregs;
         HReg src;
      } Call;
      struct {
         Addr32      dstGA;    
         MIPSAMode*   amPC;    
         MIPSCondCode cond;     
         Bool        toFastEP; 
      } XDirect;
      struct {
         HReg        dstGA;
         MIPSAMode*   amPC;
         MIPSCondCode cond; 
      } XIndir;
      struct {
         HReg        dstGA;
         MIPSAMode*   amPC;
         MIPSCondCode cond; 
         IRJumpKind  jk;
      } XAssisted;
      
      struct {
         UChar sz;   
         HReg dst;
         MIPSAMode *src;
      } Load;
      
      struct {
         UChar sz;   
         MIPSAMode *dst;
         HReg src;
      } Store;
      struct {
         UChar sz;   
         HReg dst;
         MIPSAMode *src;
      } LoadL;
      struct {
         UChar sz;   
         MIPSAMode *dst;
         HReg src;
      } StoreC;
      
      struct {
         HReg dst;
      } MfHL;

      
      struct {
         HReg src;
      } MtHL;

      
      struct {
         Bool wrLR;
         HReg gpr;
      } RdWrLR;

      
      struct {
         MIPSMaccOp op;
         Bool syned;

         HReg srcL;
         HReg srcR;
      } Macc;

      
      struct {
         MIPSFpOp op;
         HReg dst;
         HReg src;
      } FpUnary;
      struct {
         MIPSFpOp op;
         HReg dst;
         HReg srcL;
         HReg srcR;
      } FpBinary;
      struct {
         MIPSFpOp op;
         HReg dst;
         HReg srcML;
         HReg srcMR;
         HReg srcAcc;
      } FpMulAcc;
      struct {
         Bool isLoad;
         UChar sz;   
         HReg reg;
         MIPSAMode *addr;
      } FpLdSt;

      struct {
         MIPSFpOp op;
         HReg dst;
         HReg src;
      } FpConvert;
      struct {
         MIPSFpOp op;
         HReg dst;
         HReg srcL;
         HReg srcR;
         UChar cond1;
      } FpCompare;
      struct {
         MIPSFpOp op;
         HReg dst;
         HReg srcL;
         MIPSRH *srcR;
         HReg condR;
         MIPSCondCode cond;
      } MovCond;
      
      struct {
         HReg src;
      } MtFCSR;
      
      struct {
         HReg dst;
      } MfFCSR;
      struct {
         MIPSAMode* amCounter;
         MIPSAMode* amFailAddr;
      } EvCheck;
      struct {
      } ProfInc;

   } Min;
} MIPSInstr;

extern MIPSInstr *MIPSInstr_LI(HReg, ULong);
extern MIPSInstr *MIPSInstr_Alu(MIPSAluOp, HReg, HReg, MIPSRH *);
extern MIPSInstr *MIPSInstr_Shft(MIPSShftOp, Bool sz32, HReg, HReg, MIPSRH *);
extern MIPSInstr *MIPSInstr_Unary(MIPSUnaryOp op, HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_Cmp(Bool, Bool, HReg, HReg, HReg, MIPSCondCode);

extern MIPSInstr *MIPSInstr_Mul(Bool syned, Bool hi32, Bool sz32, HReg,
                                HReg, HReg);
extern MIPSInstr *MIPSInstr_Div(Bool syned, Bool sz32, HReg, HReg);
extern MIPSInstr *MIPSInstr_Madd(Bool, HReg, HReg);
extern MIPSInstr *MIPSInstr_Msub(Bool, HReg, HReg);

extern MIPSInstr *MIPSInstr_Load(UChar sz, HReg dst, MIPSAMode * src,
                                 Bool mode64);
extern MIPSInstr *MIPSInstr_Store(UChar sz, MIPSAMode * dst, HReg src,
                                  Bool mode64);

extern MIPSInstr *MIPSInstr_LoadL(UChar sz, HReg dst, MIPSAMode * src,
                                  Bool mode64);
extern MIPSInstr *MIPSInstr_StoreC(UChar sz, MIPSAMode * dst, HReg src,
                                   Bool mode64);

extern MIPSInstr *MIPSInstr_Call(MIPSCondCode, Addr32, UInt, HReg);
extern MIPSInstr *MIPSInstr_CallAlways(MIPSCondCode, Addr32, UInt);

extern MIPSInstr *MIPSInstr_XDirect(Addr32 dstGA, MIPSAMode* amPC,
                                     MIPSCondCode cond, Bool toFastEP);
extern MIPSInstr *MIPSInstr_XIndir(HReg dstGA, MIPSAMode* amPC,
                                     MIPSCondCode cond);
extern MIPSInstr *MIPSInstr_XAssisted(HReg dstGA, MIPSAMode* amPC,
                                      MIPSCondCode cond, IRJumpKind jk);

extern MIPSInstr *MIPSInstr_FpUnary(MIPSFpOp op, HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_FpBinary(MIPSFpOp op, HReg dst, HReg srcL,
                                     HReg srcR);
extern MIPSInstr *MIPSInstr_FpConvert(MIPSFpOp op, HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_FpCompare(MIPSFpOp op, HReg dst, HReg srcL,
                  HReg srcR, UChar cond1);
extern MIPSInstr *MIPSInstr_FpMulAcc(MIPSFpOp op, HReg dst, HReg srcML,
                                     HReg srcMR, HReg srcAcc);
extern MIPSInstr *MIPSInstr_FpLdSt(Bool isLoad, UChar sz, HReg, MIPSAMode *);
extern MIPSInstr *MIPSInstr_FpSTFIW(HReg addr, HReg data);
extern MIPSInstr *MIPSInstr_FpRSP(HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_FpCftI(Bool fromI, Bool int32, HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_FpCMov(MIPSCondCode, HReg dst, HReg src);
extern MIPSInstr *MIPSInstr_MtFCSR(HReg src);
extern MIPSInstr *MIPSInstr_MfFCSR(HReg dst);
extern MIPSInstr *MIPSInstr_FpCmp(HReg dst, HReg srcL, HReg srcR);

extern MIPSInstr *MIPSInstr_Mfhi(HReg dst);
extern MIPSInstr *MIPSInstr_Mflo(HReg dst);
extern MIPSInstr *MIPSInstr_Mthi(HReg src);
extern MIPSInstr *MIPSInstr_Mtlo(HReg src);

extern MIPSInstr *MIPSInstr_RdWrLR(Bool wrLR, HReg gpr);

extern MIPSInstr *MIPSInstr_MovCond(HReg dst, HReg srcL, MIPSRH * src,
                                    HReg condR, MIPSCondCode cond);

extern MIPSInstr *MIPSInstr_EvCheck(MIPSAMode* amCounter,
                                    MIPSAMode* amFailAddr );
extern MIPSInstr *MIPSInstr_ProfInc( void );

extern void ppMIPSInstr(MIPSInstr *, Bool mode64);

extern void       getRegUsage_MIPSInstr (HRegUsage *, MIPSInstr *, Bool);
extern void       mapRegs_MIPSInstr     (HRegRemap *, MIPSInstr *, Bool mode64);
extern Bool       isMove_MIPSInstr      (MIPSInstr *, HReg *, HReg *);
extern Int        emit_MIPSInstr        (Bool* is_profInc,
                                         UChar* buf, Int nbuf, MIPSInstr* i,
                                         Bool mode64,
                                         void* disp_cp_chain_me_to_slowEP,
                                         void* disp_cp_chain_me_to_fastEP,
                                         void* disp_cp_xindir,
                                         void* disp_cp_xassisted );

extern void genSpill_MIPS (  HInstr ** i1,  HInstr ** i2,
                            HReg rreg, Int offset, Bool);
extern void genReload_MIPS(  HInstr ** i1,  HInstr ** i2,
                            HReg rreg, Int offset, Bool);

extern void        getAllocableRegs_MIPS (Int *, HReg **, Bool mode64);
extern HInstrArray *iselSB_MIPS          ( IRSB*,
                                           VexArch,
                                           VexArchInfo*,
                                           VexAbiInfo*,
                                           Int offs_Host_EvC_Counter,
                                           Int offs_Host_EvC_FailAddr,
                                           Bool chainingAllowed,
                                           Bool addProfInc,
                                           Addr64 max_ga );

extern Int evCheckSzB_MIPS ( void );

extern VexInvalRange chainXDirect_MIPS ( void* place_to_chain,
                                         void* disp_cp_chain_me_EXPECTED,
                                         void* place_to_jump_to,
                                         Bool  mode64 );

extern VexInvalRange unchainXDirect_MIPS ( void* place_to_unchain,
                                           void* place_to_jump_to_EXPECTED,
                                           void* disp_cp_chain_me,
                                           Bool  mode64 );

extern VexInvalRange patchProfInc_MIPS ( void*  place_to_patch,
                                         ULong* location_of_counter,
                                         Bool  mode64 );

#endif            

