

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


#include "libvex_basictypes.h"
#include "libvex_ir.h"
#include "libvex.h"
#include "libvex_guest_mips32.h"

#include "main_util.h"
#include "main_globals.h"
#include "guest_generic_bb_to_IR.h"
#include "guest_mips_defs.h"



static Bool host_is_bigendian;

static UChar *guest_code;

static Addr32 guest_PC_bbstart;

static Addr32 guest_PC_curr_instr;

static IRSB *irsb;

static Bool mode64 = False;


#define DIP(format, args...)           \
   if (vex_traceflags & VEX_TRACE_FE)  \
      vex_printf(format, ## args)



static UInt integerGuestRegOffset(UInt iregNo)
{
   UInt ret;
   switch (iregNo) {
      case 0:
         ret = offsetof(VexGuestMIPS32State, guest_r0); break;
      case 1:
         ret = offsetof(VexGuestMIPS32State, guest_r1); break;
      case 2:
         ret = offsetof(VexGuestMIPS32State, guest_r2); break;
      case 3:
         ret = offsetof(VexGuestMIPS32State, guest_r3); break;
      case 4:
         ret = offsetof(VexGuestMIPS32State, guest_r4); break;
      case 5:
         ret = offsetof(VexGuestMIPS32State, guest_r5); break;
      case 6:
         ret = offsetof(VexGuestMIPS32State, guest_r6); break;
      case 7:
         ret = offsetof(VexGuestMIPS32State, guest_r7); break;
      case 8:
         ret = offsetof(VexGuestMIPS32State, guest_r8); break;
      case 9:
         ret = offsetof(VexGuestMIPS32State, guest_r9); break;
      case 10:
         ret = offsetof(VexGuestMIPS32State, guest_r10); break;
      case 11:
         ret = offsetof(VexGuestMIPS32State, guest_r11); break;
      case 12:
         ret = offsetof(VexGuestMIPS32State, guest_r12); break;
      case 13:
         ret = offsetof(VexGuestMIPS32State, guest_r13); break;
      case 14:
         ret = offsetof(VexGuestMIPS32State, guest_r14); break;
      case 15:
         ret = offsetof(VexGuestMIPS32State, guest_r15); break;
      case 16:
         ret = offsetof(VexGuestMIPS32State, guest_r16); break;
      case 17:
         ret = offsetof(VexGuestMIPS32State, guest_r17); break;
      case 18:
         ret = offsetof(VexGuestMIPS32State, guest_r18); break;
      case 19:
         ret = offsetof(VexGuestMIPS32State, guest_r19); break;
      case 20:
         ret = offsetof(VexGuestMIPS32State, guest_r20); break;
      case 21:
         ret = offsetof(VexGuestMIPS32State, guest_r21); break;
      case 22:
         ret = offsetof(VexGuestMIPS32State, guest_r22); break;
      case 23:
         ret = offsetof(VexGuestMIPS32State, guest_r23); break;
      case 24:
         ret = offsetof(VexGuestMIPS32State, guest_r24); break;
      case 25:
         ret = offsetof(VexGuestMIPS32State, guest_r25); break;
      case 26:
         ret = offsetof(VexGuestMIPS32State, guest_r26); break;
      case 27:
         ret = offsetof(VexGuestMIPS32State, guest_r27); break;
      case 28:
         ret = offsetof(VexGuestMIPS32State, guest_r28); break;
      case 29:
         ret = offsetof(VexGuestMIPS32State, guest_r29); break;
      case 30:
         ret = offsetof(VexGuestMIPS32State, guest_r30); break;
      case 31:
         ret = offsetof(VexGuestMIPS32State, guest_r31); break;
      default:
         vassert(0);
         break;
   }
   return ret;
}

#define OFFB_PC     offsetof(VexGuestMIPS32State, guest_PC)


static UInt floatGuestRegOffset(UInt fregNo)
{
   vassert(fregNo < 32);
   UInt ret;
   switch (fregNo) {
      case 0:
         ret = offsetof(VexGuestMIPS32State, guest_f0); break;
      case 1:
         ret = offsetof(VexGuestMIPS32State, guest_f1); break;
      case 2:
         ret = offsetof(VexGuestMIPS32State, guest_f2); break;
      case 3:
         ret = offsetof(VexGuestMIPS32State, guest_f3); break;
      case 4:
         ret = offsetof(VexGuestMIPS32State, guest_f4); break;
      case 5:
         ret = offsetof(VexGuestMIPS32State, guest_f5); break;
      case 6:
         ret = offsetof(VexGuestMIPS32State, guest_f6); break;
      case 7:
         ret = offsetof(VexGuestMIPS32State, guest_f7); break;
      case 8:
         ret = offsetof(VexGuestMIPS32State, guest_f8); break;
      case 9:
         ret = offsetof(VexGuestMIPS32State, guest_f9); break;
      case 10:
         ret = offsetof(VexGuestMIPS32State, guest_f10); break;
      case 11:
         ret = offsetof(VexGuestMIPS32State, guest_f11); break;
      case 12:
         ret = offsetof(VexGuestMIPS32State, guest_f12); break;
      case 13:
         ret = offsetof(VexGuestMIPS32State, guest_f13); break;
      case 14:
         ret = offsetof(VexGuestMIPS32State, guest_f14); break;
      case 15:
         ret = offsetof(VexGuestMIPS32State, guest_f15); break;
      case 16:
         ret = offsetof(VexGuestMIPS32State, guest_f16); break;
      case 17:
         ret = offsetof(VexGuestMIPS32State, guest_f17); break;
      case 18:
         ret = offsetof(VexGuestMIPS32State, guest_f18); break;
      case 19:
         ret = offsetof(VexGuestMIPS32State, guest_f19); break;
      case 20:
         ret = offsetof(VexGuestMIPS32State, guest_f20); break;
      case 21:
         ret = offsetof(VexGuestMIPS32State, guest_f21); break;
      case 22:
         ret = offsetof(VexGuestMIPS32State, guest_f22); break;
      case 23:
         ret = offsetof(VexGuestMIPS32State, guest_f23); break;
      case 24:
         ret = offsetof(VexGuestMIPS32State, guest_f24); break;
      case 25:
         ret = offsetof(VexGuestMIPS32State, guest_f25); break;
      case 26:
         ret = offsetof(VexGuestMIPS32State, guest_f26); break;
      case 27:
         ret = offsetof(VexGuestMIPS32State, guest_f27); break;
      case 28:
         ret = offsetof(VexGuestMIPS32State, guest_f28); break;
      case 29:
         ret = offsetof(VexGuestMIPS32State, guest_f29); break;
      case 30:
         ret = offsetof(VexGuestMIPS32State, guest_f30); break;
      case 31:
         ret = offsetof(VexGuestMIPS32State, guest_f31); break;
      default:
         vassert(0);
         break;
   }
   return ret;
}

static inline UInt getUInt(UChar * p)
{
   UInt w = 0;
#if defined (_MIPSEL)
   w = (w << 8) | p[3];
   w = (w << 8) | p[2];
   w = (w << 8) | p[1];
   w = (w << 8) | p[0];
#elif defined (_MIPSEB)
   w = (w << 8) | p[0];
   w = (w << 8) | p[1];
   w = (w << 8) | p[2];
   w = (w << 8) | p[3];
#endif
   return w;
}

#define BITS2(_b1,_b0) \
   (((_b1) << 1) | (_b0))

#define BITS3(_b2,_b1,_b0)                      \
  (((_b2) << 2) | ((_b1) << 1) | (_b0))

#define BITS4(_b3,_b2,_b1,_b0) \
   (((_b3) << 3) | ((_b2) << 2) | ((_b1) << 1) | (_b0))

#define BITS5(_b4,_b3,_b2,_b1,_b0)  \
   (((_b4) << 4) | BITS4((_b3),(_b2),(_b1),(_b0)))

#define BITS6(_b5,_b4,_b3,_b2,_b1,_b0)  \
   ((BITS2((_b5),(_b4)) << 4) \
    | BITS4((_b3),(_b2),(_b1),(_b0)))

#define BITS8(_b7,_b6,_b5,_b4,_b3,_b2,_b1,_b0)  \
   ((BITS4((_b7),(_b6),(_b5),(_b4)) << 4) \
    | BITS4((_b3),(_b2),(_b1),(_b0)))

#define LOAD_STORE_PATTERN \
    t1 = newTemp(Ity_I32); \
    assign(t1, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm)))); \

#define LWX_SWX_PATTERN \
   t2 = newTemp(Ity_I32); \
   assign(t2, binop(Iop_And32, mkexpr(t1), mkU32(0xFFFFFFFC))); \
   t4 = newTemp(Ity_I32); \
   assign(t4, binop(Iop_And32, mkexpr(t1), mkU32(0x00000003)))

#define SXXV_PATTERN(op) \
   putIReg(rd, binop(op, \
         getIReg(rt), \
            unop(Iop_32to8, \
               binop(Iop_And32, \
                  getIReg(rs), \
                  mkU32(0x0000001F) \
               ) \
            ) \
         ) \
      )

#define SXX_PATTERN(op) \
   putIReg(rd, binop(op, getIReg(rt), mkU8(sa)));

#define ALU_PATTERN(op) \
   putIReg(rd, binop(op, getIReg(rs), getIReg(rt)));

#define ALUI_PATTERN(op) \
   putIReg(rt, binop(op, getIReg(rs), mkU32(imm)));

#define ALUI_PATTERN64(op) \
   putIReg(rt, binop(op, getIReg(rs), mkU64(imm)));

#define FP_CONDITIONAL_CODE \
    t3 = newTemp(Ity_I32);  \
    assign(t3, binop(Iop_And32, IRExpr_Mux0X( unop(Iop_1Uto8, \
               binop(Iop_CmpEQ32, mkU32(cc), mkU32(0))), \
               binop(Iop_Shr32, getFCSR(), mkU8(24+cc)),  \
               binop(Iop_Shr32, getFCSR(), mkU8(23))), mkU32(0x1)));


static UInt get_opcode(UInt mipsins)
{
   return (0xFC000000 & mipsins) >> 26;
}

static UInt get_rs(UInt mipsins)
{
   return (0x03E00000 & mipsins) >> 21;
}

static UInt get_rt(UInt mipsins)
{
   return (0x001F0000 & mipsins) >> 16;
}

static UInt get_imm(UInt mipsins)
{
   return (0x0000FFFF & mipsins);
}

static UInt get_instr_index(UInt mipsins)
{
   return (0x03FFFFFF & mipsins);
}

static UInt get_rd(UInt mipsins)
{
   return (0x0000F800 & mipsins) >> 11;
}

static UInt get_sa(UInt mipsins)
{
   return (0x000007C0 & mipsins) >> 6;
}

static UInt get_function(UInt mipsins)
{
   return (0x0000003F & mipsins);
}

static UInt get_ft(UInt mipsins)
{
   return (0x001F0000 & mipsins) >> 16;
}

static UInt get_fs(UInt mipsins)
{
   return (0x0000F800 & mipsins) >> 11;
}

static UInt get_fd(UInt mipsins)
{
   return (0x000007C0 & mipsins) >> 6;
}

static UInt get_mov_cc(UInt mipsins)
{
   return (0x001C0000 & mipsins) >> 18;
}

static UInt get_bc1_cc(UInt mipsins)
{
   return (0x001C0000 & mipsins) >> 18;
}

static UInt get_fpc_cc(UInt mipsins)
{
   return (0x00000700 & mipsins) >> 8;
}

static UInt get_tf(UInt mipsins)
{
   return (0x00010000 & mipsins) >> 16;
}

static UInt get_nd(UInt mipsins)
{
   return (0x00020000 & mipsins) >> 17;
}

static UInt get_fmt(UInt mipsins)
{
   return (0x03E00000 & mipsins) >> 21;
}

static UInt get_FC(UInt mipsins)
{
   return (0x000000F0 & mipsins) >> 4;
}

static UInt get_cond(UInt mipsins)
{
   return (0x0000000F & mipsins);
}

static UInt get_code(UInt mipsins)
{
   return (0xFFC0 & mipsins) >> 6;
}

static UInt get_lsb(UInt mipsins)
{
   return (0x7C0 & mipsins) >> 6;
}

static UInt get_msb(UInt mipsins)
{
   return (0x0000F800 & mipsins) >> 11;
}

static UInt get_rot(UInt mipsins)
{
   return (0x00200000 & mipsins) >> 21;
}

static UInt get_rotv(UInt mipsins)
{
   return (0x00000040 & mipsins) >> 6;
}

static UInt get_sel(UInt mipsins)
{
   return (0x00000007 & mipsins);
}

static Bool branch_or_jump(UChar * addr)
{
   UInt fmt;
   UInt cins = getUInt(addr);

   UInt opcode = get_opcode(cins);
   UInt rt = get_rt(cins);
   UInt function = get_function(cins);

   
   if (opcode == 0x07 || opcode == 0x06 || opcode == 0x05 || opcode == 0x04 
       || opcode == 0x03 || opcode == 0x02) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x01) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x11) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x10) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x00) {
      return True;
   }

   
   if (opcode == 0x00 && function == 0x09) {
      return True;
   }

   
   if (opcode == 0x00 && function == 0x08) {
      return True;
   }

   if (opcode == 0x11) {
      
      fmt = get_fmt(cins);
      if (fmt == 0x08) {
         return True;
      }
   }

   return False;
}

static Bool is_Branch_or_Jump_and_Link(UChar * addr)
{
   UInt cins = getUInt(addr);

   UInt opcode = get_opcode(cins);
   UInt rt = get_rt(cins);
   UInt function = get_function(cins);

   
   if (opcode == 0x02) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x11) {
      return True;
   }

   
   if (opcode == 0x01 && rt == 0x10) {
      return True;
   }

   
   if (opcode == 0x00 && function == 0x09) {
      return True;
   }

   return False;
}

static Bool branch_or_link_likely(UChar * addr)
{
   UInt cins = getUInt(addr);
   UInt opcode = get_opcode(cins);
   UInt rt = get_rt(cins);

   
   if (opcode == 0x17 || opcode == 0x16 || opcode == 0x15 || opcode == 0x14)
      return True;

   
   if (opcode == 0x01 && rt == 0x03)
      return True;

   
   if (opcode == 0x01 && rt == 0x13)
      return True;

   
   if (opcode == 0x01 && rt == 0x12)
      return True;

   
   if (opcode == 0x01 && rt == 0x02)
      return True;

   return False;
}


static IRExpr *mkU8(UInt i)
{
   vassert(i < 256);
   return IRExpr_Const(IRConst_U8((UChar) i));
}

static IRExpr *mkU32(UInt i)
{
   return IRExpr_Const(IRConst_U32(i));
}

static IRExpr *mkU64(ULong i)
{
   return IRExpr_Const(IRConst_U64(i));
}

static IRExpr *mkexpr(IRTemp tmp)
{
   return IRExpr_RdTmp(tmp);
}

static IRExpr *unop(IROp op, IRExpr * a)
{
   return IRExpr_Unop(op, a);
}

static IRExpr *binop(IROp op, IRExpr * a1, IRExpr * a2)
{
   return IRExpr_Binop(op, a1, a2);
}

static IRExpr *triop(IROp op, IRExpr * a1, IRExpr * a2, IRExpr * a3)
{
   return IRExpr_Triop(op, a1, a2, a3);
}

static IRExpr *load(IRType ty, IRExpr * addr)
{
   IRExpr *load1 = NULL;
#if defined (_MIPSEL)
   load1 = IRExpr_Load(Iend_LE, ty, addr);
#elif defined (_MIPSEB)
   load1 = IRExpr_Load(Iend_BE, ty, addr);
#endif
   return load1;
}

static void stmt(IRStmt * st)
{
   addStmtToIRSB(irsb, st);
}

static void assign(IRTemp dst, IRExpr * e)
{
   stmt(IRStmt_WrTmp(dst, e));
}

static void store(IRExpr * addr, IRExpr * data)
{
#if defined (_MIPSEL)
   stmt(IRStmt_Store(Iend_LE, addr, data));
#elif defined (_MIPSEB)
   stmt(IRStmt_Store(Iend_BE, addr, data));
#endif
}

static IRTemp newTemp(IRType ty)
{
   vassert(isPlausibleIRType(ty));
   return newIRTemp(irsb->tyenv, ty);
}

static IRExpr *genROR32(IRExpr * src, Int rot)
{
   vassert(rot >= 0 && rot < 32);
   if (rot == 0)
      return src;
   return binop(Iop_Or32, binop(Iop_Shl32, src, mkU8(32 - rot)),
                          binop(Iop_Shr32, src, mkU8(rot)));
}

static IRExpr *genRORV32(IRExpr * src, IRExpr * rs)
{
   IRTemp t0 = newTemp(Ity_I8);
   IRTemp t1 = newTemp(Ity_I8);

   assign(t0, unop(Iop_32to8, binop(Iop_And32, rs, mkU32(0x0000001F))));
   assign(t1, binop(Iop_Sub8, mkU8(32), mkexpr(t0)));
   return binop(Iop_Or32, binop(Iop_Shl32, src, mkexpr(t1)),
                          binop(Iop_Shr32, src, mkexpr(t0)));
}

static UInt extend_s_16to32(UInt x)
{
   return (UInt) ((((Int) x) << 16) >> 16);
}

static UInt extend_s_18to32(UInt x)
{
   return (UInt) ((((Int) x) << 14) >> 14);
}

static void jmp_lit( DisResult* dres,
                     IRJumpKind kind, Addr32 d32 )
{
   vassert(dres->whatNext    == Dis_Continue);
   vassert(dres->len         == 0);
   vassert(dres->continueAt  == 0);
   vassert(dres->jk_StopHere == Ijk_INVALID);
   dres->whatNext    = Dis_StopHere;
   dres->jk_StopHere = kind;
   stmt( IRStmt_Put( OFFB_PC, mkU32(d32) ) );
}

static UChar getIByte(Int delta)
{
   return guest_code[delta];
}

static IRExpr *getIReg(UInt iregNo)
{
   if (0 == iregNo) {
      return mode64 ? mkU64(0x0) : mkU32(0x0);
   } else {
      IRType ty = mode64 ? Ity_I64 : Ity_I32;
      vassert(iregNo < 32);
      return IRExpr_Get(integerGuestRegOffset(iregNo), ty);
   }
}

static IRExpr *getHI(void)
{
   return IRExpr_Get(offsetof(VexGuestMIPS32State, guest_HI), Ity_I32);
}

static IRExpr *getLO(void)
{
   return IRExpr_Get(offsetof(VexGuestMIPS32State, guest_LO), Ity_I32);
}

static IRExpr *getFCSR(void)
{
   return IRExpr_Get(offsetof(VexGuestMIPS32State, guest_FCSR), Ity_I32);
}

static void putFCSR(IRExpr * e)
{
   stmt(IRStmt_Put(offsetof(VexGuestMIPS32State, guest_FCSR), e));
}

static IRExpr *getULR(void)
{
   return IRExpr_Get(offsetof(VexGuestMIPS32State, guest_ULR), Ity_I32);
}

static void putIReg(UInt archreg, IRExpr * e)
{
   IRType ty = mode64 ? Ity_I64 : Ity_I32;
   vassert(archreg < 32);
   vassert(typeOfIRExpr(irsb->tyenv, e) == ty);
   if (archreg != 0)
      stmt(IRStmt_Put(integerGuestRegOffset(archreg), e));
}

static void putLO(IRExpr * e)
{
   stmt(IRStmt_Put(offsetof(VexGuestMIPS32State, guest_LO), e));
}

static void putHI(IRExpr * e)
{
   stmt(IRStmt_Put(offsetof(VexGuestMIPS32State, guest_HI), e));
}

static void putPC(IRExpr * e)
{
   stmt(IRStmt_Put(OFFB_PC, e));
}

static IRExpr *mkWidenFrom32(IRType ty, IRExpr * src, Bool sined)
{
   vassert(ty == Ity_I32 || ty == Ity_I64);
   if (ty == Ity_I32)
      return src;
   return (sined) ? unop(Iop_32Sto64, src) : unop(Iop_32Uto64, src);
}

static IRExpr *narrowTo(IRType dst_ty, IRExpr * e)
{
   IRType src_ty = typeOfIRExpr(irsb->tyenv, e);
   if (src_ty == dst_ty)
      return e;
   if (src_ty == Ity_I32 && dst_ty == Ity_I16)
      return unop(Iop_32to16, e);
   if (src_ty == Ity_I32 && dst_ty == Ity_I8)
      return unop(Iop_32to8, e);
   if (src_ty == Ity_I64 && dst_ty == Ity_I8) {
      vassert(mode64);
      return unop(Iop_64to8, e);
   }
   if (src_ty == Ity_I64 && dst_ty == Ity_I16) {
      vassert(mode64);
      return unop(Iop_64to16, e);
   }

   if (vex_traceflags & VEX_TRACE_FE) {
      vex_printf("\nsrc, dst tys are: ");
      ppIRType(src_ty);
      vex_printf(", ");
      ppIRType(dst_ty);
      vex_printf("\n");
   }

   vpanic("narrowTo(mips)");
   return 0;
}

static IRExpr *mkNarrowTo32(IRType ty, IRExpr * src)
{
   vassert(ty == Ity_I32 || ty == Ity_I64);
   return ty == Ity_I64 ? unop(Iop_64to32, src) : src;
}

static IRExpr *getLoFromF64(IRType ty, IRExpr * src)
{
   vassert(ty == Ity_F32 || ty == Ity_F64);
   if (ty == Ity_F64) {
      IRTemp t0, t1;
      t0 = newTemp(Ity_I64);
      t1 = newTemp(Ity_I32);
      assign(t0, unop(Iop_ReinterpF64asI64, src));
      assign(t1, unop(Iop_64to32, mkexpr(t0)));
      return unop(Iop_ReinterpI32asF32, mkexpr(t1));
   } else
      return src;
}

static IRExpr *mkWidenFromF32(IRType ty, IRExpr * src)
{
   vassert(ty == Ity_F32 || ty == Ity_F64);
   return ty == Ity_F64 ? unop(Iop_F32toF64, src) : src;
}

static IRExpr *dis_branch_likely(IRExpr * guard, UInt imm)
{
   ULong branch_offset;
   IRTemp t0;

   branch_offset = extend_s_18to32(imm << 2);

   t0 = newTemp(Ity_I1);
   assign(t0, guard);

   stmt(IRStmt_Exit(mkexpr(t0), Ijk_Boring, 
                    IRConst_U32(guest_PC_curr_instr + 8), OFFB_PC));

   irsb->jumpkind = Ijk_Boring;

   return mkU32(guest_PC_curr_instr + 4 + branch_offset);
}

static void dis_branch(Bool link, IRExpr * guard, UInt imm, IRStmt ** set)
{
   ULong branch_offset;
   IRTemp t0;

   if (link) {    
      putIReg(31, mkU32(guest_PC_curr_instr + 8));
   }


   branch_offset = extend_s_18to32(imm << 2);

   t0 = newTemp(Ity_I1);
   assign(t0, guard);
   *set = IRStmt_Exit(mkexpr(t0), link ? Ijk_Call : Ijk_Boring,
                   IRConst_U32(guest_PC_curr_instr + 4 + (UInt) branch_offset),
                   OFFB_PC);
}

static IRExpr *getFReg(UInt dregNo)
{
   vassert(dregNo < 32);
   IRType ty = mode64 ? Ity_F64 : Ity_F32;
   return IRExpr_Get(floatGuestRegOffset(dregNo), ty);
}

static IRExpr *getDReg(UInt dregNo)
{
   vassert(dregNo < 32);
   IRTemp t0 = newTemp(Ity_F32);
   IRTemp t1 = newTemp(Ity_F32);
   IRTemp t2 = newTemp(Ity_F64);
   IRTemp t3 = newTemp(Ity_I32);
   IRTemp t4 = newTemp(Ity_I32);
   IRTemp t5 = newTemp(Ity_I64);

   assign(t0, getFReg(dregNo));
   assign(t1, getFReg(dregNo + 1));

   assign(t3, unop(Iop_ReinterpF32asI32, mkexpr(t0)));
   assign(t4, unop(Iop_ReinterpF32asI32, mkexpr(t1)));
   assign(t5, binop(Iop_32HLto64, mkexpr(t4), mkexpr(t3)));
   assign(t2, unop(Iop_ReinterpI64asF64, mkexpr(t5)));

   return mkexpr(t2);
}

static void putFReg(UInt dregNo, IRExpr * e)
{
   vassert(dregNo < 32);
   IRType ty = mode64 ? Ity_F64 : Ity_F32;
   vassert(typeOfIRExpr(irsb->tyenv, e) == ty);
   stmt(IRStmt_Put(floatGuestRegOffset(dregNo), e));
}

static void putDReg(UInt dregNo, IRExpr * e)
{
   vassert(dregNo < 32);
   vassert(typeOfIRExpr(irsb->tyenv, e) == Ity_F64);
   IRTemp t1 = newTemp(Ity_F64);
   IRTemp t4 = newTemp(Ity_I32);
   IRTemp t5 = newTemp(Ity_I32);
   IRTemp t6 = newTemp(Ity_I64);
   assign(t1, e);
   assign(t6, unop(Iop_ReinterpF64asI64, mkexpr(t1)));
   assign(t4, unop(Iop_64HIto32, mkexpr(t6)));  
   assign(t5, unop(Iop_64to32, mkexpr(t6))); 
   putFReg(dregNo, unop(Iop_ReinterpI32asF32, mkexpr(t5)));
   putFReg(dregNo + 1, unop(Iop_ReinterpI32asF32, mkexpr(t4)));
}

static void setFPUCondCode(IRExpr * e, UInt cc)
{
   if (cc == 0) {
      DIP("setFpu: %d\n", cc);
      putFCSR(binop(Iop_And32, getFCSR(), mkU32(0xFF7FFFFF)));
      putFCSR(binop(Iop_Or32, getFCSR(), binop(Iop_Shl32, e, mkU8(23))));
   } else {
      DIP("setFpu1: %d\n", cc);
      putFCSR(binop(Iop_And32, getFCSR(), unop(Iop_Not32, 
                               binop(Iop_Shl32, mkU32(0x01000000), mkU8(cc)))));
      putFCSR(binop(Iop_Or32, getFCSR(), binop(Iop_Shl32, e, mkU8(24 + cc))));
   }
}

static IRExpr *get_IR_roundingmode(void)
{
   IRTemp rm_MIPS = newTemp(Ity_I32);
   

   assign(rm_MIPS, binop(Iop_And32, IRExpr_Get(offsetof(VexGuestMIPS32State,
                                    guest_FCSR), Ity_I32), mkU32(3)));

   

   return binop(Iop_Xor32, mkexpr(rm_MIPS), binop(Iop_And32,
                binop(Iop_Shl32, mkexpr(rm_MIPS), mkU8(1)), mkU32(2)));
}

static Bool dis_instr_CCondFmt(UInt cins)
{
   IRTemp t0, t1, t2, t3;
   IRTemp ccIR = newTemp(Ity_I32);
   IRTemp ccMIPS = newTemp(Ity_I32);
   UInt FC = get_FC(cins);
   UInt fmt = get_fmt(cins);
   UInt fs = get_fs(cins);
   UInt ft = get_ft(cins);
   UInt cond = get_cond(cins);

   if (FC == 0x3) {  
      UInt fpc_cc = get_fpc_cc(cins);
      switch (fmt) {
         case 0x10: {  
            DIP("C.cond.S %d f%d, f%d\n", fpc_cc, fs, ft);
            t0 = newTemp(Ity_I32);
            t1 = newTemp(Ity_I32);
            t2 = newTemp(Ity_I32);
            t3 = newTemp(Ity_I32);

            assign(ccIR, binop(Iop_CmpF64, unop(Iop_F32toF64, getFReg(fs)),
                                           unop(Iop_F32toF64, getFReg(ft))));
            

            
            
            assign(ccMIPS, binop(Iop_Shl32, mkU32(1), unop(Iop_32to8, 
                           binop(Iop_Or32, binop(Iop_And32, unop(Iop_Not32,
                           binop(Iop_Shr32, mkexpr(ccIR), mkU8(5))), mkU32(2)),
                           binop(Iop_And32, binop(Iop_Xor32, mkexpr(ccIR),
                           binop(Iop_Shr32, mkexpr(ccIR), mkU8(6))), 
                           mkU32(1))))));
            assign(t0, binop(Iop_And32, mkexpr(ccMIPS), mkU32(0x1)));   
            assign(t1, binop(Iop_And32, binop(Iop_Shr32, mkexpr(ccMIPS),
                   mkU8(0x1)), mkU32(0x1))); 
            assign(t2, binop(Iop_And32, unop(Iop_Not32, binop(Iop_Shr32,
                   mkexpr(ccMIPS), mkU8(0x2))), mkU32(0x1)));  
            assign(t3, binop(Iop_And32, binop(Iop_Shr32, mkexpr(ccMIPS),
                   mkU8(0x3)), mkU32(0x1))); 

            switch (cond) {
               case 0x0:
                  setFPUCondCode(mkU32(0), fpc_cc);
                  break;
               case 0x1:
                  DIP("unorderd: %d\n", fpc_cc);
                  setFPUCondCode(mkexpr(t0), fpc_cc);
                  break;
               case 0x2:
                  setFPUCondCode(mkexpr(t1), fpc_cc);
                  break;
               case 0x3:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0x4:
                  setFPUCondCode(mkexpr(t3), fpc_cc);
                  break;
               case 0x5:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t3)),
                                       fpc_cc);
                  break;
               case 0x6:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t3), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0x7:
                  setFPUCondCode(mkexpr(t2), fpc_cc);
                  break;
               case 0x8:
                  setFPUCondCode(mkU32(0), fpc_cc);
                  break;
               case 0x9:
                  setFPUCondCode(mkexpr(t0), fpc_cc);
                  break;
               case 0xA:
                  setFPUCondCode(mkexpr(t1), fpc_cc);
                  break;
               case 0xB:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0xC:
                  setFPUCondCode(mkexpr(t3), fpc_cc);
                  break;
               case 0xD:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t3)),
                                       fpc_cc);
                  break;
               case 0xE:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t3), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0xF:
                  setFPUCondCode(mkexpr(t2), fpc_cc);
                  break;

               default:
                  return False;
            }
         }
            break;

         case 0x11:  
            DIP("C.%d.D %d f%d, f%d\n", cond, fpc_cc, fs, ft);
            t0 = newTemp(Ity_I32);
            t1 = newTemp(Ity_I32);
            t2 = newTemp(Ity_I32);
            t3 = newTemp(Ity_I32);
            assign(ccIR, binop(Iop_CmpF64, getDReg(fs), getDReg(ft)));
            

            
            
            assign(ccMIPS, binop(Iop_Shl32, mkU32(1), unop(Iop_32to8,
                           binop(Iop_Or32, binop(Iop_And32, unop(Iop_Not32,
                           binop(Iop_Shr32, mkexpr(ccIR), mkU8(5))), mkU32(2)),
                           binop(Iop_And32, binop(Iop_Xor32, mkexpr(ccIR),
                           binop(Iop_Shr32, mkexpr(ccIR), mkU8(6))),
                           mkU32(1))))));

            assign(t0, binop(Iop_And32, mkexpr(ccMIPS), mkU32(0x1)));   
            assign(t1, binop(Iop_And32, binop(Iop_Shr32, mkexpr(ccMIPS),
                   mkU8(0x1)), mkU32(0x1))); 
            assign(t2, binop(Iop_And32, unop(Iop_Not32, binop(Iop_Shr32,
                   mkexpr(ccMIPS), mkU8(0x2))), mkU32(0x1)));  
            assign(t3, binop(Iop_And32, binop(Iop_Shr32, mkexpr(ccMIPS),
                   mkU8(0x3)), mkU32(0x1))); 

            switch (cond) {
               case 0x0:
                  setFPUCondCode(mkU32(0), fpc_cc);
                  break;
               case 0x1:
                  DIP("unorderd: %d\n", fpc_cc);
                  setFPUCondCode(mkexpr(t0), fpc_cc);
                  break;
               case 0x2:
                  setFPUCondCode(mkexpr(t1), fpc_cc);
                  break;
               case 0x3:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0x4:
                  setFPUCondCode(mkexpr(t3), fpc_cc);
                  break;
               case 0x5:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t3)),
                                       fpc_cc);
                  break;
               case 0x6:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t3), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0x7:
                  setFPUCondCode(mkexpr(t2), fpc_cc);
                  break;
               case 0x8:
                  setFPUCondCode(mkU32(0), fpc_cc);
                  break;
               case 0x9:
                  setFPUCondCode(mkexpr(t0), fpc_cc);
                  break;
               case 0xA:
                  setFPUCondCode(mkexpr(t1), fpc_cc);
                  break;
               case 0xB:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0xC:
                  setFPUCondCode(mkexpr(t3), fpc_cc);
                  break;
               case 0xD:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t0), mkexpr(t3)),
                                       fpc_cc);
                  break;
               case 0xE:
                  setFPUCondCode(binop(Iop_Or32, mkexpr(t3), mkexpr(t1)),
                                       fpc_cc);
                  break;
               case 0xF:
                  setFPUCondCode(mkexpr(t2), fpc_cc);
                  break;
               default:
                  return False;
            }
            break;

            default:
               return False;
      }
   } else {
      return False;
   }

   return True;
}



static DisResult disInstr_MIPS_WRK ( Bool(*resteerOkFn) (void *,
                                                                    Addr64),
                                     Bool         resteerCisOk,
                                     void*        callback_opaque,
                                     Long         delta64,
                                     VexArchInfo* archinfo,
                                     VexAbiInfo*  abiinfo )
{
   IRTemp t0, t1, t2, t3, t4, t5, t6, t7, t8;
   UInt opcode, cins, rs, rt, rd, sa, ft, fs, fd, fmt, tf, nd, function,
        trap_code, imm, instr_index, p, msb, lsb, size, rot, sel;

   DisResult dres;

   static IRExpr *lastn = NULL;  
   static IRStmt *bstmt = NULL;  

   
   Int delta = (Int) delta64;

   Int delta_start = delta;

   
   Bool delay_slot_branch, likely_delay_slot, delay_slot_jump;

   
   dres.whatNext = Dis_Continue;
   dres.len = 0;
   dres.continueAt = 0;
   dres.jk_StopHere = Ijk_INVALID;

   delay_slot_branch = likely_delay_slot = delay_slot_jump = False;

   UChar *code = (UChar *) (guest_code + delta);
   cins = getUInt(code);

   if (delta != 0) {
      if (branch_or_jump(guest_code + delta - 4)) {
         if (lastn == NULL && bstmt == NULL) {
            DIP("Info: jump to delay slot insn...\n");
         } else {
            dres.whatNext = Dis_StopHere;

            DIP("lastn = %p bstmt = %p\n", lastn, bstmt);
            if (lastn != NULL) {
               DIP("delay slot jump\n");
               if (vex_traceflags & VEX_TRACE_FE)
                  ppIRExpr(lastn);
               delay_slot_jump = True;
            } else if (bstmt != NULL) {
               DIP("\ndelay slot branch\n");
               delay_slot_branch = True;
            }
            DIP("delay slot\n");
         }
      }

      if (branch_or_link_likely(guest_code + delta - 4)) {
         likely_delay_slot = True;
      }
   }

   
   {
      UInt word1 = 0x00000342;
      UInt word2 = 0x00000742;
      UInt word3 = 0x000000C2;
      UInt word4 = 0x000004C2;
      if (getUInt(code + 0) == word1 && getUInt(code + 4) == word2 &&
          getUInt(code + 8) == word3 && getUInt(code + 12) == word4) {
         
         if (getUInt(code + 16) == 0x01ad6825  ) {
            
            DIP("v0 = client_request ( t9 )\n");
            putPC(mkU32(guest_PC_curr_instr + 20));
            dres.jk_StopHere = Ijk_ClientReq;
            dres.whatNext    = Dis_StopHere;

            goto decode_success;
         } else if (getUInt(code + 16) == 0x01ce7025  ) {
            
            DIP("t9 = guest_NRADDR\n");
            dres.len = 20;
            delta += 20;
            putIReg(11, IRExpr_Get(offsetof(VexGuestMIPS32State, guest_NRADDR),
                                   Ity_I32));
            goto decode_success;
         } else if (getUInt(code + 16) == 0x01ef7825 ) {
            
            DIP("branch-and-link-to-noredir t9\n");
            putIReg(31, mkU32(guest_PC_curr_instr + 20));
            putPC(getIReg(25));
            dres.jk_StopHere = Ijk_NoRedir;
            dres.whatNext    = Dis_StopHere;
            goto decode_success;
         }

         delta += 16;
         goto decode_failure;
       }
   }

   opcode = get_opcode(cins);
   imm = get_imm(cins);
   rs = get_rs(cins);
   rt = get_rt(cins);
   rd = get_rd(cins);
   sa = get_sa(cins);
   fs = get_fs(cins);
   fd = get_fd(cins);
   ft = get_ft(cins);
   tf = get_tf(cins);
   nd = get_nd(cins);
   sel = get_sel(cins);
   fmt = get_fmt(cins);
   instr_index = get_instr_index(cins);
   trap_code = get_code(cins);
   function = get_function(cins);
   IRType ty = mode64 ? Ity_I64 : Ity_I32;
   IRType tyF = mode64 ? Ity_F64 : Ity_F32;

   DIP("[cins = 0x%08x] ", cins);

   switch (opcode) {

   case 0x03:     
      DIP("jal 0x%x", instr_index);
      putIReg(31, mkU32(guest_PC_curr_instr + 8));
      t0 = newTemp(ty);
      assign(t0, mkU32((guest_PC_curr_instr & 0xF0000000) |
                       (instr_index << 2)));
      lastn = mkexpr(t0);
      break;
   case 0x02:     
      DIP("j 0x%x", instr_index);
      t0 = newTemp(ty);
      assign(t0, mkU32((guest_PC_curr_instr & 0xF0000000) |
                       (instr_index << 2)));
      lastn = mkexpr(t0);
      break;

   case 0x11:     
      {
         UInt bc1_cc = get_bc1_cc(cins);
         if (0x08 == fmt) {
            switch (fmt) {
            case 0x08:  
               {
                  DIP("tf: %d, nd: %d\n", tf, nd);
                  
                  t1 = newTemp(Ity_I32);
                  t2 = newTemp(Ity_I32);
                  t3 = newTemp(Ity_I1);

                  assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                    mkU32(bc1_cc))));
                  assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                             binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                             mkU8(24 + bc1_cc)), mkU32(0x1)), binop(Iop_And32,
                             binop(Iop_Shr32, getFCSR(), mkU8(23)),
                                   mkU32(0x1))));

                  if (tf == 1 && nd == 0) {
                     
                     DIP("bc1t %d, %d", bc1_cc, imm);
                     assign(t3, binop(Iop_CmpEQ32, mkU32(1), mkexpr(t2)));
                     dis_branch(False, mkexpr(t3), imm, &bstmt);
                     break;
                  } else if (tf == 0 && nd == 0) {
                     
                     DIP("bc1f %d, %d", bc1_cc, imm);
                     assign(t3, binop(Iop_CmpEQ32, mkU32(0), mkexpr(t2)));
                     dis_branch(False, mkexpr(t3), imm, &bstmt);
                     break;
                  } else if (nd == 1 && tf == 0) {
                     DIP("bc1fl %d, %d", bc1_cc, imm);
                     lastn = dis_branch_likely(binop(Iop_CmpNE32, mkexpr(t2),
                           mode64 ? mkU64(0x0) : mkU32(0x0)), imm);
                     break;
                  } else if (nd == 1 && tf == 1) {
                     DIP("bc1tl %d, %d", bc1_cc, imm);
                     lastn = dis_branch_likely(binop(Iop_CmpEQ32, mkexpr(t2),
                                               mkU32(0x0)), imm);
                     break;
                  } else
                     goto decode_failure;
               }

            default:
               goto decode_failure;
            }
         } else {
            switch (function) {

            case 0x4:   
               {
                  switch (fmt) {
                  case 0x10:  
                     {
                        IRExpr *rm = get_IR_roundingmode();
                        putFReg(fd, mkWidenFromF32(tyF, binop(Iop_SqrtF32, rm,
                                    getLoFromF64(tyF, getFReg(fs)))));
                     }
                     break;
                  case 0x11:  
                     {
                        IRExpr *rm = get_IR_roundingmode();
                        putDReg(fd, binop(Iop_SqrtF64, rm, getDReg(fs)));
                     }
                     break;
                  }
               }
               break;
            case 0x5:   
               switch (fmt) {
               case 0x10:  
                  DIP("abs.s f%d, f%d\n", fd, fs);
                  putFReg(fd, mkWidenFromF32(tyF, unop(Iop_AbsF32,
                              getLoFromF64(tyF, getFReg(fs)))));
                  break;
               case 0x11:  
                  DIP("abs.d f%d, f%d\n", fd, fs);
                  putDReg(fd, unop(Iop_AbsF64, getDReg(fs)));
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x02:  
               switch (fmt) {
               case 0x11:  
                  {
                     DIP("mul.d f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_MulF64, rm, getDReg(fs),
                                       getDReg(ft)));
                     break;
                  }
               case 0x10:  
                  {
                     DIP("mul.s f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_MulF32, rm,
                                 getLoFromF64(tyF, getFReg(fs)),
                                 getLoFromF64(tyF, getFReg(ft)))));
                     break;
                  }
               default:
                  goto decode_failure;
               }
               break;   

            case 0x03:  
               switch (fmt) {
               case 0x11:  
                  {
                     DIP("div.d f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_DivF64, rm, getDReg(fs),
                                 getDReg(ft)));
                     break;
                  }
               case 0x10:  
                  {
                     DIP("div.s f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_DivF32, rm,
                                 getLoFromF64(tyF, getFReg(fs)),
                                 getLoFromF64(tyF, getFReg(ft)))));
                     break;
                  }
               default:
                  goto decode_failure;
               }
               break;   

            case 0x01:  
               switch (fmt) {
               case 0x11:  
                  {
                     DIP("sub.d f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_SubF64, rm, getDReg(fs), getDReg(ft)));
                     break;
                  }
               case 0x10:  
                  {
                     DIP("sub.s f%d, f%d, f%d", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_SubF32, rm,
                                 getLoFromF64(tyF, getFReg(fs)),
                                 getLoFromF64(tyF, getFReg(ft)))));
                     break;
                  }
               default:
                  goto decode_failure;
               }
               break;   

            case 0x06:  
               switch (fmt) {
               case 0x11:  
                  
                  DIP("mov.d f%d, f%d", fd, fs);
                  putFReg(fd, getFReg(fs));
                  putFReg(fd + 1, getFReg(fs + 1));
                  break;
               case 0x10:  
                  DIP("mov.s f%d, f%d", fd, fs);
                  putFReg(fd, getFReg(fs));
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x7:   
               switch (fmt) {
               case 0x10:  
                  DIP("neg.s f%d, f%d", fd, fs);
                  putFReg(fd, mkWidenFromF32(tyF, unop(Iop_NegF32,
                              getLoFromF64(tyF, getFReg(fs)))));
                  break;
               case 0x11:  
                  DIP("neg.d f%d, f%d", fd, fs);
                  putDReg(fd, unop(Iop_NegF64, getDReg(fs)));
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x15:  
               switch (fmt) {
               case 0x10:
                  {  
                     DIP("recip.s f%d, f%d\n", fd, fs);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_DivF32,
                                 rm, unop(Iop_ReinterpI32asF32,
                                 mkU32(0x3F800000)), getLoFromF64(tyF,
                                 getFReg(fs)))));
                     break;
                  }
               case 0x11:
                  {  
                     DIP("recip.d f%d, f%d\n", fd, fs);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_DivF64, rm, 
                                 unop(Iop_ReinterpI64asF64,
                                 mkU64(0x3FF0000000000000ULL)), getDReg(fs)));
                     break;
                  }
               default:
                  goto decode_failure;

               }
               break;   

            case 0x13:  
               switch (fmt) {
               case 0x10:  
                  DIP("movn.s f%d, f%d, r%d", fd, fs, rt);

                  t1 = newTemp(Ity_F64);
                  t2 = newTemp(Ity_F64);
                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_F64);

                  assign(t1, unop(Iop_F32toF64, getFReg(fs)));
                  assign(t2, unop(Iop_F32toF64, getFReg(fd)));
                  assign(t3, unop(Iop_1Sto32, binop(Iop_CmpNE32, mkU32(0),
                                                    getIReg(rt))));

                  assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                               mkexpr(t2), mkexpr(t1)));

                  putFReg(fd, binop(Iop_F64toF32, get_IR_roundingmode(),
                                    mkexpr(t4)));
                  break;
               case 0x11:  
                  DIP("movn.d f%d, f%d, r%d", fd, fs, rt);

                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_F64);

                  assign(t3, unop(Iop_1Sto32, binop(Iop_CmpNE32, mkU32(0),
                                                    getIReg(rt))));
                  putDReg(fd, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                                getDReg(fd), getDReg(fs)));
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x12:  
               switch (fmt) {
               case 0x10:  
                  DIP("movz.s f%d, f%d, r%d", fd, fs, rt);

                  t1 = newTemp(Ity_F64);
                  t2 = newTemp(Ity_F64);
                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_F64);

                  assign(t1, unop(Iop_F32toF64, getFReg(fs)));
                  assign(t2, unop(Iop_F32toF64, getFReg(fd)));
                  assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                    getIReg(rt))));
                  assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                               mkexpr(t2), mkexpr(t1)));

                  putFReg(fd, binop(Iop_F64toF32, get_IR_roundingmode(),
                                    mkexpr(t4)));

                  break;
               case 0x11:  
                  DIP("movz.d f%d, f%d, r%d", fd, fs, rt);

                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_F64);

                  assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                    getIReg(rt))));
                  putDReg(fd, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                                getDReg(fd), getDReg(fs)));
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x11:  
               if (tf == 1) {
                  UInt mov_cc = get_mov_cc(cins);
                  switch (fmt)   
                  {
                  case 0x11:  
                     DIP("movt.d f%d, f%d, %d", fd, fs, mov_cc);
                     t1 = newTemp(Ity_I32);
                     t2 = newTemp(Ity_I32);
                     t3 = newTemp(Ity_I32);
                     t4 = newTemp(Ity_F64);

                     assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                       mkU32(mov_cc))));
                     assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                                binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                 mkU8(24 + mov_cc)), mkU32(0x1)),
                                 binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                 mkU8(23)), mkU32(0x1))));

                     assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(1),
                                mkexpr(t2))));
                     assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                             getDReg(fs), getDReg(fd)));
                     putDReg(fd, mkexpr(t4));
                     break;
                  case 0x10:  
                     DIP("movt.s f%d, f%d, %d", fd, fs, mov_cc);
                     t1 = newTemp(Ity_I32);
                     t2 = newTemp(Ity_I32);
                     t3 = newTemp(Ity_I32);
                     t4 = newTemp(Ity_F64);
                     t5 = newTemp(Ity_F64);
                     t6 = newTemp(Ity_F64);
                     t7 = newTemp(Ity_I64);

                     assign(t5, unop(Iop_F32toF64, getFReg(fs)));
                     assign(t6, unop(Iop_F32toF64, getFReg(fd)));

                     assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                     mkU32(mov_cc))));
                     assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                                             binop(Iop_And32, binop(Iop_Shr32,
                                             getFCSR(), mkU8(24 + mov_cc)),
                                             mkU32(0x1)), binop(Iop_And32,
                                             binop(Iop_Shr32, getFCSR(),
                                             mkU8(23)), mkU32(0x1))));

                     assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(1),
                                                       mkexpr(t2))));
                     assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                             mkexpr(t5), mkexpr(t6)));

                     putFReg(fd, binop(Iop_F64toF32, get_IR_roundingmode(),
                                       mkexpr(t4)));
                     break;
                  default:
                     goto decode_failure;
                  }
               } else if (tf == 0)  
               {
                  UInt mov_cc = get_mov_cc(cins);
                  switch (fmt)   
                  {
                  case 0x11:  
                     DIP("movf.d f%d, f%d, %d", fd, fs, mov_cc);
                     t1 = newTemp(Ity_I32);
                     t2 = newTemp(Ity_I32);
                     t3 = newTemp(Ity_I32);
                     t4 = newTemp(Ity_F64);

                     assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32,
                                                 mkU32(0), mkU32(mov_cc))));
                     assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                                binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                mkU8(24 + mov_cc)), mkU32(0x1)),
                                binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                mkU8(23)), mkU32(0x1))));

                     assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(1),
                                                       mkexpr(t2))));
                     assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                             getDReg(fd), getDReg(fs)));
                     putDReg(fd, mkexpr(t4));
                     break;
                  case 0x10:  
                     DIP("movf.s f%d, f%d, %d", fd, fs, mov_cc);
                     {
                        t1 = newTemp(Ity_I32);
                        t2 = newTemp(Ity_I32);
                        t3 = newTemp(Ity_I32);
                        t4 = newTemp(Ity_F64);
                        t5 = newTemp(Ity_F64);
                        t6 = newTemp(Ity_F64);

                        assign(t5, unop(Iop_F32toF64, getFReg(fs)));
                        assign(t6, unop(Iop_F32toF64, getFReg(fd)));

                        assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                          mkU32(mov_cc))));
                        assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                                   binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                   mkU8(24 + mov_cc)), mkU32(0x1)),
                                   binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                                   mkU8(23)), mkU32(0x1))));

                        assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(1),
                                                          mkexpr(t2))));
                        assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                                                     mkexpr(t6), mkexpr(t5)));
                        putFReg(fd, binop(Iop_F64toF32, get_IR_roundingmode(),
                                          mkexpr(t4)));
                     }
                     break;
                  default:
                     goto decode_failure;
                  }
               }

               break;   

            case 0x0:   
               switch (fmt) {
               case 0x10:  
                  {
                     DIP("add.s f%d, f%d, f%d\n", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_AddF32, rm,
                                 getLoFromF64(tyF, getFReg(fs)),
                                 getLoFromF64(tyF, getFReg(ft)))));
                     break;
                  }
               case 0x11:  
                  {
                     DIP("add.d f%d, f%d, f%d\n", fd, fs, ft);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_AddF64, rm, getDReg(fs), 
                                       getDReg(ft)));
                     break;
                  }

               case 0x4:   
                  DIP("mtc1 r%d, f%d", rt, fs);
                  putFReg(fs, unop(Iop_ReinterpI32asF32, getIReg(rt)));
                  break;

               case 0x0:   
                  DIP("mfc1 r%d, f%d", rt, fs);
                  putIReg(rt, unop(Iop_ReinterpF32asI32, getFReg(fs)));
                  break;

               case 0x6:   
                  DIP("ctc1 r%d, f%d", rt, fs);
                  t0 = newTemp(Ity_I32);
                  t1 = newTemp(Ity_I32);
                  t2 = newTemp(Ity_I32);
                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_I32);
                  t5 = newTemp(Ity_I32);
                  t6 = newTemp(Ity_I32);
                  assign(t0, mkNarrowTo32(ty, getIReg(rt)));
                  if (fs == 25) {   
                     assign(t1, binop(Iop_Shl32, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x000000FE)), mkU8(24)));
                     assign(t2, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x01000000)));
                     assign(t3, binop(Iop_Shl32, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x00000001)), mkU8(23)));
                     assign(t4, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x007FFFFF)));
                     putFCSR(binop(Iop_Or32, binop(Iop_Or32, mkexpr(t1),
                                   mkexpr(t2)), binop(Iop_Or32, mkexpr(t3),
                                   mkexpr(t4))));
                  } else if (fs == 26) {  
                     assign(t1, binop(Iop_And32, getFCSR(), mkU32(0xFFFC0000)));
                     assign(t2, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x0003F000)));
                     assign(t3, binop(Iop_And32, getFCSR(), mkU32(0x00000F80)));
                     assign(t4, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x0000007C)));
                     assign(t5, binop(Iop_And32, getFCSR(), mkU32(0x00000003)));
                     putFCSR(binop(Iop_Or32, binop(Iop_Or32, binop(Iop_Or32,
                                   mkexpr(t1), mkexpr(t2)), binop(Iop_Or32,
                                   mkexpr(t3), mkexpr(t4))), mkexpr(t5)));
                  } else if (fs == 28) {
                     assign(t1, binop(Iop_And32, getFCSR(), mkU32(0xFE000000)));
                     assign(t2, binop(Iop_Shl32, binop(Iop_And32, mkexpr(t0),
                                mkU32(0x00000002)), mkU8(22)));
                     assign(t3, binop(Iop_And32, getFCSR(), mkU32(0x00FFF000)));
                     assign(t4, binop(Iop_And32, mkexpr(t0),
                                mkU32(0x00000F80)));
                     assign(t5, binop(Iop_And32, getFCSR(), mkU32(0x0000007C)));
                     assign(t6, binop(Iop_And32, mkexpr(t0),
                                mkU32(0x00000003)));
                     putFCSR(binop(Iop_Or32, binop(Iop_Or32, binop(Iop_Or32,
                                   mkexpr(t1), mkexpr(t2)), binop(Iop_Or32,
                                   mkexpr(t3), mkexpr(t4))), binop(Iop_Or32,
                                   mkexpr(t5), mkexpr(t6))));
                  } else if (fs == 31) {
                     putFCSR(mkexpr(t0));
                  }
                  break;
               case 0x2:   
                  DIP("cfc1 r%d, f%d", rt, fs);
                  t0 = newTemp(Ity_I32);
                  t1 = newTemp(Ity_I32);
                  t2 = newTemp(Ity_I32);
                  t3 = newTemp(Ity_I32);
                  t4 = newTemp(Ity_I32);
                  t5 = newTemp(Ity_I32);
                  t6 = newTemp(Ity_I32);
                  assign(t0, getFCSR());
                  if (fs == 0) {
                     putIReg(rt, mkWidenFrom32(ty,
                             IRExpr_Get(offsetof(VexGuestMIPS32State,
                                                 guest_FIR),
                                       Ity_I32),
                             False));
                  } else if (fs == 25) {
                     assign(t1, mkU32(0x000000FF));
                     assign(t2, binop(Iop_Shr32, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0xFE000000)), mkU8(25)));
                     assign(t3, binop(Iop_Shr32, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x00800000)), mkU8(23)));
                     putIReg(rt, mkWidenFrom32(ty, binop(Iop_Or32,
                                 binop(Iop_Or32, mkexpr(t1), mkexpr(t2)),
                                 mkexpr(t3)), False));
                  } else if (fs == 26) {
                     assign(t1, mkU32(0xFFFFF07C));
                     assign(t2, binop(Iop_And32, mkexpr(t0),
                                mkU32(0x0003F000)));
                     assign(t3, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x0000007C)));
                     putIReg(rt, mkWidenFrom32(ty, binop(Iop_Or32,
                                 binop(Iop_Or32, mkexpr(t1), mkexpr(t2)),
                                 mkexpr(t3)), False));
                  } else if (fs == 28) {
                     assign(t1, mkU32(0x00000F87));
                     assign(t2, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x00000F83)));
                     assign(t3, binop(Iop_Shr32, binop(Iop_And32, mkexpr(t0),
                                      mkU32(0x01000000)), mkU8(22)));
                     putIReg(rt, mkWidenFrom32(ty, binop(Iop_Or32,
                                 binop(Iop_Or32, mkexpr(t1), mkexpr(t2)),
                                 mkexpr(t3)), False));
                  } else if (fs == 31) {
                     putIReg(rt, mkWidenFrom32(ty, getFCSR(), False));
                  }
                  break;
               default:
                  goto decode_failure;
               }
               break;   

            case 0x21:  
               switch (fmt) {
               case 0x10:  
                  DIP("cvt.d.s f%d, f%d", fd, fs);
                  putDReg(fd, unop(Iop_F32toF64, getFReg(fs)));
                  break;

               case 0x14:
                  {  
                     DIP("cvt.d.w %d, %d\n", fd, fs);
                     t0 = newTemp(Ity_I32);
                     assign(t0, unop(Iop_ReinterpF32asI32, getFReg(fs)));
                     putDReg(fd, unop(Iop_I32StoF64, mkexpr(t0)));
                  }
                  break;

               default:
                  goto decode_failure;
               }
               break;   

            case 0x20:  
               switch (fmt) {
               case 0x14:  
                  DIP("cvt.s.w %d, %d\n", fd, fs);
                  t0 = newTemp(Ity_I32);
                  assign(t0, unop(Iop_ReinterpF32asI32, getFReg(fs)));
                  putFReg(fd, binop(Iop_I32StoF32, get_IR_roundingmode(),
                              mkexpr(t0)));
                  break;

               case 0x11:  
                  DIP("cvt.s.d %d, %d\n", fd, fs);
                  putFReg(fd, binop(Iop_F64toF32, get_IR_roundingmode(),
                                    getDReg(fs)));
                  break;

               default:
                  goto decode_failure;
               }
               break;   

            case 0x24:  
               switch (fmt) {
               case 0x10:  
                  DIP("cvt.w.s %d, %d\n", fd, fs);
                  putFReg(fd, binop(Iop_RoundF32toInt, get_IR_roundingmode(),
                                    getFReg(fs)));
                  break;

               case 0x11:
                  {  
                     DIP("cvt.w.d %d, %d\n", fd, fs);
                     t0 = newTemp(Ity_I32);

                     assign(t0, binop(Iop_F64toI32S, get_IR_roundingmode(),
                                      getDReg(fs)));

                     putFReg(fd, unop(Iop_ReinterpI32asF32, mkexpr(t0)));
                  }
                  break;

               default:
                  goto decode_failure;

               }
               break;

            case 0x09:  
               switch (fmt) {
               case 0x10:  
                  DIP("trunc.l.s %d, %d\n", fd, fs);
                  goto decode_failure;

               case 0x11:  
                  DIP("trunc.l.d %d, %d\n", fd, fs);
                  goto decode_failure;

               default:
                  goto decode_failure;

               }
               break;   

            case 0x0C:  
               switch (fmt) {
               case 0x10:  
                  DIP("round.w.s f%d, f%d\n", fd, fs);
                  putFReg(fd, binop(Iop_RoundF32toInt, mkU32(0x0),
                                    getFReg(fs)));
                  break;

               case 0x11:  
                  DIP("round.w.d f%d, f%d\n", fd, fs);
                  t0 = newTemp(Ity_I32);

                  assign(t0, binop(Iop_F64toI32S, mkU32(0x0), getDReg(fs)));

                  putFReg(fd, unop(Iop_ReinterpI32asF32, mkexpr(t0)));
                  break;

               default:
                  goto decode_failure;

               }
               break;   

            case 0x0F:  
               switch (fmt) {
               case 0x10:  
                  DIP("floor.w.s f%d, f%d\n", fd, fs);
                  putFReg(fd, binop(Iop_RoundF32toInt, mkU32(0x1),
                                    getFReg(fs)));
                  break;

               case 0x11:  
                  DIP("floor.w.d f%d, f%d\n", fd, fs);
                  t0 = newTemp(Ity_I32);

                  assign(t0, binop(Iop_F64toI32S, mkU32(0x1), getDReg(fs)));

                  putFReg(fd, unop(Iop_ReinterpI32asF32, mkexpr(t0)));
                  break;

               default:
                  goto decode_failure;

               }
               break;   

            case 0x0D:  
               switch (fmt) {
               case 0x10:  
                  DIP("trunc.w.s %d, %d\n", fd, fs);
                  putFReg(fd, binop(Iop_RoundF32toInt, mkU32(0x3),
                                    getFReg(fs)));
                  break;

               case 0x11:  
                  DIP("trunc.w.d %d, %d\n", fd, fs);
                  t0 = newTemp(Ity_I32);

                  assign(t0, binop(Iop_F64toI32S, mkU32(0x3), getDReg(fs)));

                  putFReg(fd, unop(Iop_ReinterpI32asF32, mkexpr(t0)));
                  break;

               default:
                  goto decode_failure;

               }
               break;
            case 0x0E:  
               switch (fmt) {
               case 0x10:  
                  DIP("ceil.w.s %d, %d\n", fd, fs);
                  putFReg(fd, binop(Iop_RoundF32toInt, mkU32(0x2),
                                    getFReg(fs)));
                  break;

               case 0x11:  
                  DIP("ceil.w.d %d, %d\n", fd, fs);
                  t0 = newTemp(Ity_I32);

                  assign(t0, binop(Iop_F64toI32S, mkU32(0x2), getDReg(fs)));

                  putFReg(fd, unop(Iop_ReinterpI32asF32, mkexpr(t0)));
                  break;

               default:
                  goto decode_failure;

               }
               break;
            case 0x0A:  
               switch (fmt) {
               case 0x10:  
                  DIP("ceil.l.s %d, %d\n", fd, fs);
                  goto decode_failure;

               case 0x11:  
                  DIP("ceil.l.d %d, %d\n", fd, fs);

                  goto decode_failure;

               default:
                  goto decode_failure;

               }
               break;

            case 0x16:  
               switch (fmt) {
               case 0x10:
                  {  
                     DIP("rsqrt.s %d, %d\n", fd, fs);
                     IRExpr *rm = get_IR_roundingmode();
                     putFReg(fd, mkWidenFromF32(tyF, triop(Iop_DivF32, rm,
                                 unop(Iop_ReinterpI32asF32, mkU32(0x3F800000)),
                                 binop(Iop_SqrtF32, rm, getLoFromF64(tyF,
                                 getFReg(fs))))));
                     break;
                  }
               case 0x11:
                  {  
                     DIP("rsqrt.d %d, %d\n", fd, fs);
                     IRExpr *rm = get_IR_roundingmode();
                     putDReg(fd, triop(Iop_DivF64, rm,
                                 unop(Iop_ReinterpI64asF64,
                                 mkU64(0x3FF0000000000000ULL)),
                                 binop(Iop_SqrtF64, rm, getDReg(fs))));
                     break;
                  }
               default:
                  goto decode_failure;

               }
               break;

            default:
               if (dis_instr_CCondFmt(cins))
                  break;
               goto decode_failure;

            }

         }
      }
      break;      
   case 0x10:     
      if (rs == 0) { 
         DIP("mfc0 r%d, r%d, %d", rt, rd, sel);

         IRTemp   val  = newTemp(Ity_I32);
         IRExpr** args = mkIRExprVec_2 (mkU32(rd), mkU32(sel));
         IRDirty *d = unsafeIRDirty_1_N(val,
                                        0,
                                        "mips32_dirtyhelper_mfc0",
                                        &mips32_dirtyhelper_mfc0,
                                        args);

         stmt(IRStmt_Dirty(d));
         putIReg(rt, mkexpr(val));
      } else
         goto decode_failure;
      break;
   case 0x31:     
      
      LOAD_STORE_PATTERN;
      putFReg(ft, load(Ity_F32, mkexpr(t1)));

      DIP("lwc1 f%d, %d(r%d)", ft, imm, rs);
      break;

   case 0x39:     
      LOAD_STORE_PATTERN;
      store(mkexpr(t1), getFReg(ft));
      DIP("swc1 f%d, %d(r%d)", ft, imm, rs);
      break;

   case 0x33:     
      DIP("pref");
      break;

   case 0x35:
      
      LOAD_STORE_PATTERN;

      t2 = newTemp(Ity_I32);
      assign(t2, binop(Iop_Add32, getIReg(rs),
                       mkU32(extend_s_16to32(imm + 4))));

#if defined (_MIPSEL)
      putFReg(ft, load(Ity_F32, mkexpr(t1)));
      putFReg(ft + 1, load(Ity_F32, mkexpr(t2)));
#elif defined (_MIPSEB)
      putFReg(ft + 1, load(Ity_F32, mkexpr(t1)));
      putFReg(ft, load(Ity_F32, mkexpr(t2)));
#endif
      DIP("ldc1 f%d, %d(%d) \n", rt, imm, rs);
      break;

   case 0x3D:
      
      LOAD_STORE_PATTERN;

      t2 = newTemp(Ity_I32);
      assign(t2, binop(Iop_Add32, getIReg(rs),
                       mkU32(extend_s_16to32(imm + 4))));

#if defined (_MIPSEL)
      store(mkexpr(t1), getFReg(ft));
      store(mkexpr(t2), getFReg(ft + 1));
#elif defined (_MIPSEB)
      store(mkexpr(t1), getFReg(ft + 1));
      store(mkexpr(t2), getFReg(ft));
#endif
      DIP("sdc1 f%d, %d(%d)", ft, imm, rs);
      break;

   case 0x23:     
      DIP("lw r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      putIReg(rt, mkWidenFrom32(ty, load(Ity_I32, mkexpr(t1)), True));
      break;

   case 0x20:     
      DIP("lb r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      putIReg(rt, unop(Iop_8Sto32, load(Ity_I8, mkexpr(t1))));
      break;

   case 0x24:     
      DIP("lbu r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      putIReg(rt, unop(Iop_8Uto32, load(Ity_I8, mkexpr(t1))));
      break;

   case 0x21:     
      DIP("lh r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      putIReg(rt, unop(Iop_16Sto32, load(Ity_I16, mkexpr(t1))));
      break;

   case 0x25:     
      DIP("lhu r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      putIReg(rt, unop(Iop_16Uto32, load(Ity_I16, mkexpr(t1))));
      break;

   case 0x0F:     
      p = (imm << 16);
      DIP("lui rt: %d, imm: %d, imm << 16: %d", rt, imm, p);
      if ((vex_traceflags & VEX_TRACE_FE) && !mode64)
         ppIRExpr(mkU32(p));
      putIReg(rt, mkU32(p));
      break;

   case 0x13:     
      switch (function) {
      case 0x0: { 
         
         DIP("lwxc1 f%d, r%d(r%d) \n", fd, rt, rs);
         t0 = newTemp(Ity_I32);
         assign(t0, binop(Iop_Add32, getIReg(rs), getIReg(rt)));
         putFReg(fd, load(Ity_F32, mkexpr(t0)));
         break;
      }

      case 0x1: { 
         
         t0 = newTemp(Ity_I32);
         assign(t0, binop(Iop_Add32, getIReg(rs), getIReg(rt)));

         t1 = newTemp(Ity_I32);
         assign(t1, binop(Iop_Add32, mkexpr(t0), mkU32(4)));

#if defined (_MIPSEL)
         putFReg(fd, load(Ity_F32, mkexpr(t0)));
         putFReg(fd + 1, load(Ity_F32, mkexpr(t1)));
#elif defined (_MIPSEB)
         putFReg(fd + 1, load(Ity_F32, mkexpr(t0)));
         putFReg(fd, load(Ity_F32, mkexpr(t1)));
#endif
         DIP("ldxc1 f%d, r%d(r%d) \n", fd, rt, rs);
         break;
      }

      case 0x5:   
         
         DIP("luxc1 f%d, r%d(r%d) \n", fd, rt, rs);
         t0 = newTemp(Ity_I64);
         t1 = newTemp(Ity_I64);
         assign(t0, binop(Iop_Add64, getIReg(rs), getIReg(rt)));
         assign(t1, binop(Iop_And64, mkexpr(t0), mkU64(0xfffffffffffffff8ULL)));
         putFReg(fd, load(Ity_F64, mkexpr(t1)));
         break;

      case 0x8: { 
         
         t0 = newTemp(Ity_I32);
         assign(t0, binop(Iop_Add32, getIReg(rs), getIReg(rt)));

         store(mkexpr(t0), getFReg(fs));

         DIP("swxc1 f%d, r%d(r%d)", ft, rt, rs);
         break;
      }
      case 0x9: { 
         
         t0 = newTemp(Ity_I32);
         assign(t0, binop(Iop_Add32, getIReg(rs), getIReg(rt)));

         t1 = newTemp(Ity_I32);
         assign(t1, binop(Iop_Add32, mkexpr(t0), mkU32(4)));

#if defined (_MIPSEL)
         store(mkexpr(t0), getFReg(fs));
         store(mkexpr(t1), getFReg(fs + 1));
#elif defined (_MIPSEB)
         store(mkexpr(t0), getFReg(fs + 1));
         store(mkexpr(t1), getFReg(fs));
#endif

         DIP("sdc1 f%d, %d(%d)", ft, imm, rs);
         break;
      }
      case 0x0F: {
         DIP("prefx");
         break;
      }
      case 0x20:  { 
         DIP("madd.s f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F32);
         assign(t1, triop(Iop_MulF32, rm, getLoFromF64(tyF, getFReg(fs)),
                          getLoFromF64(tyF, getFReg(ft))));

         putFReg(fd, mkWidenFromF32(tyF, triop(Iop_AddF32, rm, mkexpr(t1),
                                    getLoFromF64(tyF, getFReg(fmt)))));
         break;   
      }
      case 0x21: { 
         DIP("madd.d f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F64);
         assign(t1, triop(Iop_MulF64, rm, getDReg(fs), getDReg(ft)));

         putDReg(fd, triop(Iop_AddF64, rm, mkexpr(t1), getDReg(fmt)));
         break;   
      }
      case 0x28: { 
         DIP("msub.s f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F32);
         assign(t1, triop(Iop_MulF32, rm, getLoFromF64(tyF, getFReg(fs)),
                          getLoFromF64(tyF, getFReg(ft))));

         putFReg(fd, mkWidenFromF32(tyF, triop(Iop_SubF32, rm,
                     mkexpr(t1), getLoFromF64(tyF, getFReg(fmt)))));
         break;   
      }
      case 0x29: { 
         DIP("msub.d f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F64);
         assign(t1, triop(Iop_MulF64, rm, getDReg(fs), getDReg(ft)));

         putDReg(fd, triop(Iop_SubF64, rm, mkexpr(t1), getDReg(fmt)));
         break;   
      }
      case 0x30: { 
         DIP("nmadd.s f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F32);
         t2 = newTemp(Ity_F32);
         assign(t1, triop(Iop_MulF32, rm, getLoFromF64(tyF, getFReg(fs)),
                getLoFromF64(tyF, getFReg(ft))));

         assign(t2, triop(Iop_AddF32, rm, mkexpr(t1),
                          getLoFromF64(tyF, getFReg(fmt))));

         putFReg(fd, mkWidenFromF32(tyF, unop(Iop_NegF32, mkexpr(t2))));
         break;   
      }
      case 0x31: { 
         DIP("nmadd.d f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F64);
         t2 = newTemp(Ity_F64);
         assign(t1, triop(Iop_MulF64, rm, getDReg(fs), getDReg(ft)));

         assign(t2, triop(Iop_AddF64, rm, mkexpr(t1), getDReg(fmt)));
         putDReg(fd, unop(Iop_NegF64, mkexpr(t2)));
         break;   
      }
      case 0x38: { 
         DIP("nmsub.s f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F32);
         t2 = newTemp(Ity_F32);
         assign(t1, triop(Iop_MulF32, rm, getLoFromF64(tyF, getFReg(fs)),
                          getLoFromF64(tyF, getFReg(ft))));

         assign(t2, triop(Iop_SubF32, rm, mkexpr(t1), getLoFromF64(tyF,
                                                      getFReg(fmt))));

         putFReg(fd, mkWidenFromF32(tyF, unop(Iop_NegF32, mkexpr(t2))));
         break;   
      }
      case 0x39: { 
         DIP("nmsub.d f%d, f%d, f%d, f%d", fmt, ft, fs, fd);
         IRExpr *rm = get_IR_roundingmode();
         t1 = newTemp(Ity_F64);
         t2 = newTemp(Ity_F64);
         assign(t1, triop(Iop_MulF64, rm, getDReg(fs), getDReg(ft)));

         assign(t2, triop(Iop_SubF64, rm, mkexpr(t1), getDReg(fmt)));
         putDReg(fd, unop(Iop_NegF64, mkexpr(t2)));
         break;   
      }

      default:
         goto decode_failure;
      }
      break;

   case 0x22:     

      DIP("lwl r%d, %d(r%d)", rt, imm, rs);
      {
         
         t1 = newTemp(Ity_I32);
#if defined (_MIPSEL)
         assign(t1, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
#elif defined (_MIPSEB)
         assign(t1, binop(Iop_Xor32, mkU32(0x3), binop(Iop_Add32, getIReg(rs),
                                     mkU32(extend_s_16to32(imm)))));
#endif

         
         
         LWX_SWX_PATTERN;

         
         t3 = newTemp(Ity_I32);
         assign(t3, binop(Iop_Shl32, load(Ity_I32, mkexpr(t2)), narrowTo(Ity_I8,
                    binop(Iop_Shl32, binop(Iop_Sub32, mkU32(0x03), mkexpr(t4)),
                    mkU8(3)))));

         
         t5 = newTemp(Ity_I32);
         assign(t5, binop(Iop_And32, getIReg(rt), binop(Iop_Shr32,
                    mkU32(0xFFFFFFFF), narrowTo(Ity_I8, binop(Iop_Shl32,
                    binop(Iop_Add32, mkexpr(t4), mkU32(0x1)), mkU8(0x3))))));

         putIReg(rt, binop(Iop_Or32, mkexpr(t5), mkexpr(t3)));
      }
      break;

   case 0x26:     

      DIP("lwr r%d, %d(r%d)", rt, imm, rs);
      {
         
         t1 = newTemp(Ity_I32);
#if defined (_MIPSEL)
         assign(t1, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
#elif defined (_MIPSEB)
         assign(t1, binop(Iop_Xor32, mkU32(0x3), binop(Iop_Add32, getIReg(rs),
                                     mkU32(extend_s_16to32(imm)))));
#endif

         
         
         LWX_SWX_PATTERN;

         
         t3 = newTemp(Ity_I32);
         assign(t3, binop(Iop_Shr32, load(Ity_I32, mkexpr(t2)),
                    narrowTo(Ity_I8, binop(Iop_Shl32, mkexpr(t4),
                    mkU8(3)))));

         
         t5 = newTemp(Ity_I32);
         assign(t5, binop(Iop_And32, getIReg(rt), unop(Iop_Not32,
                    binop(Iop_Shr32, mkU32(0xFFFFFFFF), narrowTo(Ity_I8,
                          binop(Iop_Shl32, mkexpr(t4), mkU8(0x3)))))));

         putIReg(rt, binop(Iop_Or32, mkexpr(t5), mkexpr(t3)));
      }
      break;

   case 0x2B:     
      DIP("sw r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      store(mkexpr(t1), mkNarrowTo32(ty, getIReg(rt)));
      break;

   case 0x28:     
      DIP("sb r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      store(mkexpr(t1), narrowTo(Ity_I8, getIReg(rt)));
      break;

   case 0x29:     
      DIP("sh r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;
      store(mkexpr(t1), narrowTo(Ity_I16, getIReg(rt)));
      break;

   case 0x2A:     

      DIP("swl r%d, %d(r%d)", rt, imm, rs);
      {
         
         t1 = newTemp(Ity_I32);
#if defined (_MIPSEL)
         assign(t1, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
#elif defined (_MIPSEB)
         assign(t1, binop(Iop_Xor32, mkU32(0x3), binop(Iop_Add32, getIReg(rs),
                                     mkU32(extend_s_16to32(imm)))));
#endif

         
         
         LWX_SWX_PATTERN;

         
         t3 = newTemp(Ity_I32);
         assign(t3, binop(Iop_Shr32, getIReg(rt), narrowTo(Ity_I8,
                    binop(Iop_Shl32, binop(Iop_Sub32, mkU32(0x03), mkexpr(t4)),
                    mkU8(3)))));

         
         t5 = newTemp(Ity_I32);
         t6 = newTemp(Ity_I32);
         t7 = newTemp(Ity_I32);
         t8 = newTemp(Ity_I32);

         
         assign(t5, binop(Iop_Mul32, binop(Iop_Sub32, mkU32(0x3), mkexpr(t4)),
                          mkU32(0x8)));

         assign(t6, binop(Iop_Shr32, mkU32(0xFFFFFFFF), narrowTo(Ity_I8,
                                                        mkexpr(t5))));
         assign(t7, binop(Iop_Xor32, mkU32(0xFFFFFFFF), mkexpr(t6)));
         assign(t8, binop(Iop_And32, load(Ity_I32, mkexpr(t2)), mkexpr(t7)));
         store(mkexpr(t2), binop(Iop_Or32, mkexpr(t8), mkexpr(t3)));
      }
      break;

   case 0x2E:     

      DIP("swr r%d, %d(r%d)", rt, imm, rs);
      {
         
         t1 = newTemp(Ity_I32);
#if defined (_MIPSEL)
         assign(t1, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
#elif defined (_MIPSEB)
         assign(t1, binop(Iop_Xor32, mkU32(0x3), binop(Iop_Add32, getIReg(rs),
                                     mkU32(extend_s_16to32(imm)))));
#endif

         
         
         LWX_SWX_PATTERN;

         
         t3 = newTemp(Ity_I32);
         assign(t3, binop(Iop_Shl32, getIReg(rt), narrowTo(Ity_I8,
                    binop(Iop_Shl32, mkexpr(t4), mkU8(3)))));

         
         t5 = newTemp(Ity_I32);
         assign(t5, binop(Iop_And32, load(Ity_I32, mkexpr(t2)), unop(Iop_Not32,
                    binop(Iop_Shl32, mkU32(0xFFFFFFFF), narrowTo(Ity_I8,
                          binop(Iop_Shl32, mkexpr(t4), mkU8(0x3)))))));

         store(mkexpr(t2), binop(Iop_Xor32, mkexpr(t5), mkexpr(t3)));
      }
      break;

   case 0x1C:     
      switch (function) {
      case 0x02: { 
         DIP("mul r%d, r%d, r%d", rd, rs, rt);
         putIReg(rd, binop(Iop_Mul32, getIReg(rs), getIReg(rt)));
         break;
      }

      case 0x00: { 
         DIP("madd r%d, r%d", rs, rt);
         t1 = newTemp(Ity_I32);
         t2 = newTemp(Ity_I32);
         t3 = newTemp(Ity_I64);
         t4 = newTemp(Ity_I32);
         t5 = newTemp(Ity_I32);
         t6 = newTemp(Ity_I32);

         assign(t1, getHI());
         assign(t2, getLO());

         assign(t3, binop(Iop_MullS32, getIReg(rs), getIReg(rt)));

         assign(t4, binop(Iop_Add32, mkexpr(t2), unop(Iop_64to32,
                                                      mkexpr(t3))));

         assign(t5, unop(Iop_1Uto32, binop(Iop_CmpLT32U, mkexpr(t4),
                                     unop(Iop_64to32, mkexpr(t3)))));
         assign(t6, binop(Iop_Add32, mkexpr(t5), mkexpr(t1)));

         putHI(binop(Iop_Add32, mkexpr(t6), unop(Iop_64HIto32, mkexpr(t3))));
         putLO(mkexpr(t4));
         break;
      }

      case 0x01: { 
         DIP("maddu r%d, r%d", rs, rt);
         t1 = newTemp(Ity_I32);
         t2 = newTemp(Ity_I32);
         t3 = newTemp(Ity_I64);
         t4 = newTemp(Ity_I32);
         t5 = newTemp(Ity_I32);
         t6 = newTemp(Ity_I32);

         assign(t1, getHI());
         assign(t2, getLO());

         assign(t3, binop(Iop_MullU32, getIReg(rs), getIReg(rt)));

         assign(t4, binop(Iop_Add32, mkexpr(t2), unop(Iop_64to32,
                                                      mkexpr(t3))));
         assign(t5, unop(Iop_1Uto32, binop(Iop_CmpLT32U, mkexpr(t4),
                                     unop(Iop_64to32, mkexpr(t3)))));
         assign(t6, binop(Iop_Add32, mkexpr(t5), mkexpr(t1)));

         putHI(binop(Iop_Add32, mkexpr(t6), unop(Iop_64HIto32, mkexpr(t3))));
         putLO(mkexpr(t4));
         break;
      }

      case 0x04: { 
         DIP("msub r%d, r%d", rs, rt);
         t1 = newTemp(Ity_I32);
         t2 = newTemp(Ity_I32);
         t3 = newTemp(Ity_I64);
         t4 = newTemp(Ity_I32);
         t5 = newTemp(Ity_I32);
         t6 = newTemp(Ity_I32);

         assign(t1, getHI());
         assign(t2, getLO());

         assign(t3, binop(Iop_MullS32, getIReg(rs), getIReg(rt)));
         assign(t4, unop(Iop_64to32, mkexpr(t3))); 

         
         assign(t5, unop(Iop_1Sto32, binop(Iop_CmpLT32U, mkexpr(t2),
                                           mkexpr(t4))));

         assign(t6, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t5)), mkexpr(t1),
                                 binop(Iop_Sub32, mkexpr(t1), mkU32(0x1))));

         putHI(binop(Iop_Sub32, mkexpr(t6), unop(Iop_64HIto32, mkexpr(t3))));
         putLO(binop(Iop_Sub32, mkexpr(t2), mkexpr(t4)));
         break;
      }

      case 0x05: { 
         DIP("msubu r%d, r%d", rs, rt);
         t1 = newTemp(Ity_I32);
         t2 = newTemp(Ity_I32);
         t3 = newTemp(Ity_I64);
         t4 = newTemp(Ity_I32);
         t5 = newTemp(Ity_I32);
         t6 = newTemp(Ity_I32);

         assign(t1, getHI());
         assign(t2, getLO());

         assign(t3, binop(Iop_MullU32, getIReg(rs), getIReg(rt)));
         assign(t4, unop(Iop_64to32, mkexpr(t3))); 

         
         assign(t5, unop(Iop_1Sto32, binop(Iop_CmpLT32U, mkexpr(t2),
                                           mkexpr(t4))));

         assign(t6, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t5)),
                    mkexpr(t1), binop(Iop_Sub32, mkexpr(t1), mkU32(0x1))));

         putHI(binop(Iop_Sub32, mkexpr(t6), unop(Iop_64HIto32, mkexpr(t3))));
         putLO(binop(Iop_Sub32, mkexpr(t2), mkexpr(t4)));
         break;
      }

      case 0x20: { 
         DIP("clz r%d, r%d", rd, rs);
         t1 = newTemp(Ity_I32);
         assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, getIReg(rs),
                                           mkU32(0))));
         putIReg(rd, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                     unop(Iop_Clz32, getIReg(rs)), mkU32(0x00000020)));
         break;
      }

      case 0x21: { 
         DIP("clo r%d, r%d", rd, rs);
         t1 = newTemp(Ity_I32);
         assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, getIReg(rs),
                                           mkU32(0xffffffff))));
         putIReg(rd, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                     unop(Iop_Clz32, unop(Iop_Not32, getIReg(rs))),
                     mkU32(0x00000020)));
         break;
      }

      default:
         goto decode_failure;
      }
      break;

   case 0x1F:     
      switch (function) {
      case 0x3B:
           {
            DIP("rdhwr r%d, r%d", rt, rd);
            if (rd == 29) {
               putIReg(rt, getULR());
            } else
               goto decode_failure;
            break;
         }
      case 0x04:
           msb = get_msb(cins);
         lsb = get_lsb(cins);

         size = msb - lsb + 1;

         vassert(lsb + size <= 32);
         vassert(lsb + size > 0);

         DIP("ins size:%d msb:%d lsb:%d", size, msb, lsb);
         
         t0 = newTemp(Ity_I32);
         t3 = newTemp(Ity_I32);
         assign(t0, binop(Iop_Shl32, getIReg(rs), mkU8(32 - size)));
         
         t1 = newTemp(Ity_I32);
         assign(t1, binop(Iop_Shr32, mkexpr(t0), mkU8(32 - size - lsb)));

         if (lsb > 0) {
            t2 = newTemp(Ity_I32);
            
            assign(t2, binop(Iop_Shl32, getIReg(rt), mkU8(32 - lsb)));
            assign(t3, binop(Iop_Shr32, mkexpr(t2), mkU8(32 - lsb)));
         }

         if (msb < 31) {
            t4 = newTemp(Ity_I32);
            
            assign(t4, binop(Iop_Shr32, getIReg(rt), mkU8(msb + 1)));
            t5 = newTemp(Ity_I32);
            assign(t5, binop(Iop_Shl32, mkexpr(t4), mkU8(msb + 1)));

            
            if (lsb > 0) {
               t6 = newTemp(Ity_I32);
               assign(t6, binop(Iop_Or32, mkexpr(t5), mkexpr(t1)));
               putIReg(rt, binop(Iop_Or32, mkexpr(t6), mkexpr(t3)));
            } else {
               putIReg(rt, binop(Iop_Or32, mkexpr(t1), mkexpr(t5)));
            }
         }

         else {
            putIReg(rt, binop(Iop_Or32, mkexpr(t1), mkexpr(t3)));

         }
         break;

      case 0x00:
          msb = get_msb(cins);
         lsb = get_lsb(cins);
         size = msb + 1;
         DIP("ext size:%d msb:%d lsb:%d", size, msb, lsb);
         vassert(lsb + size <= 32);
         vassert(lsb + size > 0);
         
         if (lsb + size < 32) {
            t0 = newTemp(Ity_I32);
            assign(t0, binop(Iop_Shl32, getIReg(rs), mkU8(32 - lsb - size)));
            putIReg(rt, binop(Iop_Shr32, mkexpr(t0), mkU8(32 - size)));
         } else {
            putIReg(rt, binop(Iop_Shr32, getIReg(rs), mkU8(32 - size)));

         }
         break;

      case 0x20:
          switch (sa) {
         case 0x10:
              DIP("seb r%d, r%d", rd, rt);
            putIReg(rd, unop(Iop_8Sto32, unop(Iop_32to8, getIReg(rt))));
            break;

         case 0x18:
              DIP("seh r%d, r%d", rd, rt);
            putIReg(rd, unop(Iop_16Sto32, unop(Iop_32to16, getIReg(rt))));
            break;

         case 0x02:
              DIP("wsbh r%d, r%d", rd, rt);
            t0 = newTemp(Ity_I32);
            t1 = newTemp(Ity_I32);
            t2 = newTemp(Ity_I32);
            t3 = newTemp(Ity_I32);
            assign(t0, binop(Iop_Shl32, binop(Iop_And32, getIReg(rt),
                                        mkU32(0x00FF0000)), mkU8(0x8)));
            assign(t1, binop(Iop_Shr32, binop(Iop_And32, getIReg(rt),
                       mkU32(0xFF000000)), mkU8(0x8)));
            assign(t2, binop(Iop_Shl32, binop(Iop_And32, getIReg(rt),
                       mkU32(0x000000FF)), mkU8(0x8)));
            assign(t3, binop(Iop_Shr32, binop(Iop_And32, getIReg(rt),
                       mkU32(0x0000FF00)), mkU8(0x8)));
            putIReg(rd, binop(Iop_Or32, binop(Iop_Or32, mkexpr(t0),
                        mkexpr(t1)), binop(Iop_Or32, mkexpr(t2), mkexpr(t3))));
            break;

         default:
            goto decode_failure;

         }
         break;
        default:
         goto decode_failure;

      }
      break;      

   case 0x3B:
      if (0x3B == function && (archinfo->hwcaps & VEX_PRID_COMP_BROADCOM)) {
          
            DIP("rdhwr r%d, r%d", rt, rd);
            if (rd == 29) {
               putIReg(rt, getULR());
            } else
               goto decode_failure;
            break;
      } else {
         goto decode_failure;
      }

   case 0x00:     

      switch (function) {
      case 0x1: {
         UInt mov_cc = get_mov_cc(cins);
         if (tf == 0) { 
            DIP("movf r%d, r%d, %d", rd, rs, mov_cc);
            {
               t1 = newTemp(Ity_I32);
               t2 = newTemp(Ity_I32);
               t3 = newTemp(Ity_I32);
               t4 = newTemp(Ity_I32);

               assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                 mkU32(mov_cc))));
               assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                          binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                          mkU8(24 + mov_cc)), mkU32(0x1)), binop(Iop_And32,
                          binop(Iop_Shr32, getFCSR(), mkU8(23)),
                          mkU32(0x1))));

               assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                 mkexpr(t2))));
               assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                          getIReg(rd), getIReg(rs)));
               putIReg(rd, mkexpr(t4));
            }
         } else if (tf == 1) {   
            DIP("movt r%d, r%d, %d", rd, rs, mov_cc);
            {
               t1 = newTemp(Ity_I32);
               t2 = newTemp(Ity_I32);
               t3 = newTemp(Ity_I32);
               t4 = newTemp(Ity_I32);

               assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(0),
                                                 mkU32(mov_cc))));
               assign(t2, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t1)),
                          binop(Iop_And32, binop(Iop_Shr32, getFCSR(),
                          mkU8(24 + mov_cc)), mkU32(0x1)), binop(Iop_And32,
                          binop(Iop_Shr32, getFCSR(), mkU8(23)),
                          mkU32(0x1))));

               assign(t3, unop(Iop_1Sto32, binop(Iop_CmpEQ32, mkU32(1),
                                                 mkexpr(t2))));
               assign(t4, IRExpr_Mux0X(unop(Iop_32to8, mkexpr(t3)),
                          getIReg(rd), getIReg(rs)));
               putIReg(rd, mkexpr(t4));
            }
         }
         break;
      }
      case 0x0A: {
         
         DIP("movz r%d, r%d, r%d", rd, rs, rt);
         t1 = newTemp(ty);
         t2 = newTemp(ty);
         {
            assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, getIReg(rt),
                                              mkU32(0x0))));
            assign(t2, unop(Iop_1Sto32, binop(Iop_CmpNE32, getIReg(rt),
                                              mkU32(0x0))));
            putIReg(rd, binop(Iop_Add32, binop(Iop_And32, getIReg(rs),
                        mkexpr(t1)), binop(Iop_And32, getIReg(rd),
                        mkexpr(t2))));
         }
         break;
      }

      case 0x0B: {
         
         DIP("movn r%d, r%d, r%d", rd, rs, rt);
         t1 = newTemp(ty);
         t2 = newTemp(ty);
         {
            assign(t1, unop(Iop_1Sto32, binop(Iop_CmpEQ32, getIReg(rt),
                                              mkU32(0x0))));
            assign(t2, unop(Iop_1Sto32, binop(Iop_CmpNE32, getIReg(rt),
                                              mkU32(0x0))));
            putIReg(rd, binop(Iop_Add32, binop(Iop_And32, getIReg(rs),
                        mkexpr(t2)), binop(Iop_And32, getIReg(rd),
                        mkexpr(t1))));
         }
         break;
      }

      case 0x18:  
         DIP("mult r%d, r%d", rs, rt);
         t2 = newTemp(Ity_I64);

         assign(t2, binop(Iop_MullS32, mkNarrowTo32(ty, getIReg(rs)),
                          mkNarrowTo32(ty, getIReg(rt))));

         putHI(mkWidenFrom32(ty, unop(Iop_64HIto32, mkexpr(t2)), True));
         putLO(mkWidenFrom32(ty, unop(Iop_64to32, mkexpr(t2)), True));
         break;

      case 0x19:  
         DIP("multu r%d, r%d", rs, rt);
         t2 = newTemp(Ity_I64);

         assign(t2, binop(Iop_MullU32, mkNarrowTo32(ty, getIReg(rs)),
                                       mkNarrowTo32(ty, getIReg(rt))));

         putHI(mkWidenFrom32(ty, unop(Iop_64HIto32, mkexpr(t2)), True));
         putLO(mkWidenFrom32(ty, unop(Iop_64to32, mkexpr(t2)), True));
         break;

      case 0x20:  
         DIP("add r%d, r%d, r%d", rd, rs, rt);
         {
            t2 = newTemp(Ity_I32);

            assign(t2, binop(Iop_Add32, getIReg(rs), getIReg(rt)));
            putIReg(rd, mkexpr(t2));
         }
         break;

      case 0x1A:  
         DIP("div r%d, r%d", rs, rt);
         {
            t1 = newTemp(Ity_I64);
            t2 = newTemp(Ity_I64);

            assign(t1, unop(Iop_32Sto64, getIReg(rs)));
            assign(t2, binop(Iop_DivModS64to32, mkexpr(t1), getIReg(rt)));

            putHI(unop(Iop_64HIto32, mkexpr(t2)));
            putLO(unop(Iop_64to32, mkexpr(t2)));
         }
         break;

      case 0x1B:  
         DIP("divu r%d, r%d", rs, rt);
         {
            t1 = newTemp(Ity_I64);
            t2 = newTemp(Ity_I64);
            assign(t1, unop(Iop_32Uto64, getIReg(rs)));
            assign(t2, binop(Iop_DivModU64to32, mkexpr(t1), getIReg(rt)));
            putHI(unop(Iop_64HIto32, mkexpr(t2)));
            putLO(unop(Iop_64to32, mkexpr(t2)));
         }
         break;

      case 0x10:  
         DIP("mfhi r%d", rd);
         putIReg(rd, getHI());
         break;

      case 0x11:  
         DIP("mthi r%d", rs);
         putHI(getIReg(rs));
         break;

      case 0x12:  
         DIP("mflo r%d", rd);
         putIReg(rd, getLO());
         break;

      case 0x13:  
         DIP("mtlo r%d", rs);
         putLO(getIReg(rs));
         break;

      case 0x21:  
         DIP("addu r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_Add32);
         break;

      case 0x22:  
         DIP("sub r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_Sub32);
         break;

      case 0x23:  
         DIP("subu r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_Sub32);
         break;

      case 0x24:  
         DIP("and r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_And32);
         break;

      case 0x25:  
         DIP("or r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_Or32);
         break;

      case 0x26:  
         DIP("xor r%d, r%d, r%d", rd, rs, rt);
         ALU_PATTERN(Iop_Xor32);
         break;

      case 0x27:  
         DIP("nor r%d, r%d, r%d", rd, rs, rt);
         putIReg(rd, unop(Iop_Not32, binop(Iop_Or32, getIReg(rs),getIReg(rt))));
         break;

      case 0x08:  
         DIP("jr r%d", rs);
         t0 = newTemp(ty);
         assign(t0, getIReg(rs));
         lastn = mkexpr(t0);
         break;

      case 0x09:  
         DIP("jalr r%d r%d", rd, rs);
         putIReg(rd, mkU32(guest_PC_curr_instr + 8));
         t0 = newTemp(Ity_I32);
         assign(t0, getIReg(rs));
         lastn = mkexpr(t0);
         break;

      case 0x0C:  
         DIP("syscall");
         putPC(mkU32(guest_PC_curr_instr + 4));
         dres.jk_StopHere = Ijk_Sys_syscall;
         dres.whatNext    = Dis_StopHere;
         break;

      case 0x2A:  
         DIP("slt r%d, r%d, r%d", rd, rs, rt);
         putIReg(rd, unop(Iop_1Uto32, binop(Iop_CmpLT32S, getIReg(rs),
                                      getIReg(rt))));
         break;

      case 0x2B:  
         DIP("sltu r%d, r%d, r%d", rd, rs, rt);
         putIReg(rd, unop(Iop_1Uto32, binop(Iop_CmpLT32U, getIReg(rs),
                                      getIReg(rt))));
         break;

      case 0x00:
         
         DIP("sll r%d, r%d, %d", rd, rt, sa);
         SXX_PATTERN(Iop_Shl32);
         break;

      case 0x04:  
         DIP("sllv r%d, r%d, r%d", rd, rt, rs);
         SXXV_PATTERN(Iop_Shl32);
         break;

      case 0x03:  
         DIP("sra r%d, r%d, %d", rd, rt, sa);
         SXX_PATTERN(Iop_Sar32);
         break;

      case 0x07:  
         DIP("srav r%d, r%d, r%d", rd, rt, rs);
         SXXV_PATTERN(Iop_Sar32);
         break;

      case 0x02: {  
         rot = get_rot(cins);
         if (rot) {
            DIP("rotr r%d, r%d, %d", rd, rt, sa);
            putIReg(rd, mkWidenFrom32(ty, genROR32(mkNarrowTo32(ty,
                        getIReg(rt)), sa), False));
         } else {
            DIP("srl r%d, r%d, %d", rd, rt, sa);
            SXX_PATTERN(Iop_Shr32);
         }
      break;
      }

      case 0x06: {
         rot = get_rotv(cins);
         if (rot) {
            DIP("rotrv r%d, r%d, r%d", rd, rt, rs);
            putIReg(rd, mkWidenFrom32(ty, genRORV32(mkNarrowTo32(ty,
                        getIReg(rt)), mkNarrowTo32(ty, getIReg(rs))),False));
            break;
         } else {
            
            DIP("srlv r%d, r%d, r%d", rd, rt, rs);
            SXXV_PATTERN(Iop_Shr32);
            break;
         }
      } 

      case 0x0D:  
         DIP("Info: Breakpoint...code = %d", trap_code);
         jmp_lit(&dres, Ijk_SigTRAP, (guest_PC_curr_instr + 4));
         vassert(dres.whatNext == Dis_StopHere);
         break;

      case 0x30: { 
          DIP("tge r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32S, getIReg (rt), getIReg (rs)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x31: { 
          DIP("tgeu r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32U, getIReg (rt), getIReg (rs)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x32: { 
          DIP("tlt r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32S, getIReg (rs), getIReg (rt)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x33: { 
          DIP("tltu r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32U, getIReg (rs), getIReg (rt)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x34: { 
          DIP("teq r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit(binop (Iop_CmpEQ32, getIReg (rs), getIReg (rt)),
               Ijk_SigTRAP, IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x36: { 
          DIP("tne r%d, r%d %d", rs, rt, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpNE32, getIReg (rs), getIReg (rt)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x0F: {
          DIP("sync r%d, r%d, %d", rt, rd, sel);
         lsb = get_lsb(cins);
         IRDirty *d = unsafeIRDirty_0_N(0,
                                        "mips32_dirtyhelper_sync",
                                        &mips32_dirtyhelper_sync,
                                        mkIRExprVec_1
                                        (mkU32(lsb)));

         d->needsBBP = False;
         d->nFxState = 0;

         stmt(IRStmt_Dirty(d));
         break;
      }

      default:
         goto decode_failure;
      }
      break;

   case 0x01:     

      switch (rt) {
      case 0x01:  
         DIP("bgez r%d, %d", rs, imm);
         dis_branch(False, binop(Iop_CmpEQ32, binop(Iop_And32, getIReg(rs),
                           mkU32(0x80000000)), mkU32(0x0)), imm, &bstmt);
         break;

      case 0x03:  
         DIP("bgezl r%d, %d", rs, imm);
         lastn = dis_branch_likely(binop(Iop_CmpNE32, binop(Iop_And32,
                                   getIReg(rs), mode64 ?
                                      mkU64(0x8000000000000000ULL)
                                      :mkU32(0x80000000)),
                                   mkU32(0x0)), imm);
         break;

      case 0x00:  
         DIP("bltz r%d, %d", rs, imm);
         dis_branch(False, binop(Iop_CmpEQ32, binop(Iop_And32, getIReg(rs),
                    mkU32(0x80000000)), mkU32(0x80000000)), imm, &bstmt);
         break;

      case 0x02:  
         DIP("bltzl r%d, %d", rs, imm);
         lastn = dis_branch_likely(binop(Iop_CmpNE32, binop(Iop_And32,
                                   getIReg(rs), mkU32(0x80000000)),
                                   mkU32(0x80000000)), imm);
         break;

      case 0x10:  
         DIP("bltzal r%d, %d", rs, imm);
         dis_branch(True, binop(Iop_CmpEQ32, binop(Iop_And32, getIReg(rs),
                    mkU32(0x80000000)), mkU32(0x80000000)), imm, &bstmt);
         break;

      case 0x12:  
         DIP("bltzall r%d, %d", rs, imm);
         putIReg(31, mkU32(guest_PC_curr_instr + 8));
         lastn = dis_branch_likely(binop(Iop_CmpNE32, binop(Iop_And32,
                                   getIReg(rs), mkU32(0x80000000)),
                                                mkU32(0x80000000)), imm);
         break;

      case 0x11:  
         DIP("bgezal r%d, %d", rs, imm);
         dis_branch(True, binop(Iop_CmpEQ32, binop(Iop_And32, getIReg(rs),
                    mkU32(0x80000000)), mkU32(0x0)), imm, &bstmt);
         break;

      case 0x13:  
         DIP("bgezall r%d, %d", rs, imm);
         putIReg(31, mkU32(guest_PC_curr_instr + 8));
         lastn = dis_branch_likely(binop(Iop_CmpNE32, binop(Iop_And32,
                                   getIReg(rs), mkU32(0x80000000)),
                                   mkU32(0x0)), imm);
         break;

      case 0x08: { 
          DIP("tgei r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32S, mkU32 (imm), getIReg (rs)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x09: { 
          DIP("tgeiu r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32U, mkU32 (imm), getIReg (rs)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x0A: { 
          DIP("tlti r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32S, getIReg (rs), mkU32 (imm)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x0B: { 
          DIP("tltiu r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpLT32U, getIReg (rs), mkU32 (imm)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x0C: { 
          DIP("teqi r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpEQ32, getIReg (rs), mkU32 (imm)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x0E: { 
          DIP("tnei r%d, %d %d", rs, imm, trap_code);
         stmt (IRStmt_Exit (binop (Iop_CmpNE32, getIReg (rs), mkU32 (imm)),
                            Ijk_SigTRAP,
                            IRConst_U32 (guest_PC_curr_instr + 4), OFFB_PC));
         break;
      }
      case 0x1F:
          
             
             break;

      default:
         goto decode_failure;
      }
      break;

   case 0x04:
      DIP("beq r%d, r%d, %d", rs, rt, imm);
      dis_branch(False, binop(Iop_CmpEQ32, getIReg(rs), getIReg(rt)),
                              imm, &bstmt);
      break;

   case 0x14:
      DIP("beql r%d, r%d, %d", rs, rt, imm);
      lastn = dis_branch_likely(binop(Iop_CmpNE32, getIReg(rs), getIReg(rt)),
                                      imm);
      break;

   case 0x05:
      DIP("bne r%d, r%d, %d", rs, rt, imm);
      dis_branch(False, binop(Iop_CmpNE32, getIReg(rs), getIReg(rt)),
                              imm, &bstmt);
      break;

   case 0x15:
      DIP("bnel r%d, r%d, %d", rs, rt, imm);
      lastn =
          dis_branch_likely(binop(Iop_CmpEQ32, getIReg(rs), getIReg(rt)), imm);
      break;

   case 0x07:     
      DIP("bgtz r%d, %d", rs, imm);
      dis_branch(False, unop(Iop_Not1, binop(Iop_CmpLE32S, getIReg(rs),
                             mkU32(0x00))), imm, &bstmt);
      break;

   case 0x17:     
      DIP("bgtzl r%d, %d", rs, imm);
      lastn = dis_branch_likely(binop(Iop_CmpLE32S, getIReg(rs), mkU32(0x00)),
                                      imm);
      break;

   case 0x06:     
      DIP("blez r%d, %d", rs, imm);
      dis_branch(False,binop(Iop_CmpLE32S, getIReg(rs), mkU32(0x0)), imm,
                             &bstmt);
      break;

   case 0x16:     
      DIP("blezl r%d, %d", rs, imm);
      lastn = dis_branch_likely(unop(Iop_Not1, (binop(Iop_CmpLE32S,
                                     getIReg(rs), mkU32(0x0)))), imm);
      break;

   case 0x08:     
      DIP("addi r%d, r%d, %d", rt, rs, imm);
      putIReg(rt, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
      break;

   case 0x09:     
      DIP("addiu r%d, r%d, %d", rt, rs, imm);
      putIReg(rt, binop(Iop_Add32, getIReg(rs), mkU32(extend_s_16to32(imm))));
      break;

   case 0x0C:     
      DIP("andi r%d, r%d, %d", rt, rs, imm);
      ALUI_PATTERN(Iop_And32);
      break;

   case 0x0E:     
      DIP("xori r%d, r%d, %d", rt, rs, imm);
      ALUI_PATTERN(Iop_Xor32);
      break;

   case 0x0D:     
      DIP("ori r%d, r%d, %d", rt, rs, imm);
      ALUI_PATTERN(Iop_Or32);
      break;

   case 0x0A:     
      DIP("slti r%d, r%d, %d", rt, rs, imm);
      putIReg(rt, unop(Iop_1Uto32, binop(Iop_CmpLT32S, getIReg(rs),
                                         mkU32(extend_s_16to32(imm)))));
      break;

   case 0x0B:     
      DIP("sltiu r%d, r%d, %d", rt, rs, imm);
      putIReg(rt, unop(Iop_1Uto32, binop(Iop_CmpLT32U, getIReg(rs),
                                         mkU32(extend_s_16to32(imm)))));
      break;

   case 0x30:     
      DIP("ll r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;

      t2 = newTemp(Ity_I32);
#if defined (_MIPSEL)
      stmt(IRStmt_LLSC(Iend_LE, t2, mkexpr(t1), NULL  ));
#elif defined (_MIPSEB)
      stmt(IRStmt_LLSC(Iend_BE, t2, mkexpr(t1), NULL  ));
#endif

      putIReg(rt, mkexpr(t2));
      break;

   case 0x38:     
      DIP("sc r%d, %d(r%d)", rt, imm, rs);
      LOAD_STORE_PATTERN;

      t2 = newTemp(Ity_I1);
#if defined (_MIPSEL)
      stmt(IRStmt_LLSC(Iend_LE, t2, mkexpr(t1), mkNarrowTo32(ty, getIReg(rt))));
#elif defined (_MIPSEB)
      stmt(IRStmt_LLSC(Iend_BE, t2, mkexpr(t1), mkNarrowTo32(ty, getIReg(rt))));
#endif

      putIReg(rt, unop(Iop_1Uto32, mkexpr(t2)));
      break;

 decode_failure:
      
      DIP("vex mips->IR: unhandled instruction bytes: "
          "0x%x 0x%x 0x%x 0x%x\n",
          (Int) getIByte(delta_start + 0),
          (Int) getIByte(delta_start + 1),
          (Int) getIByte(delta_start + 2),
          (Int) getIByte(delta_start + 3));

      stmt(IRStmt_Put(offsetof(VexGuestMIPS32State, guest_PC),
           mkU32(guest_PC_curr_instr)));
      jmp_lit(&dres, Ijk_NoDecode, guest_PC_curr_instr);
      dres.whatNext = Dis_StopHere;
      dres.len = 0;
      return dres;
   }        

   

   if (delay_slot_branch) {
      delay_slot_branch = False;
      stmt(bstmt);
      bstmt = NULL;
      putPC(mkU32(guest_PC_curr_instr + 4));
      dres.jk_StopHere = is_Branch_or_Jump_and_Link(guest_code + delta - 4) ?
                         Ijk_Call : Ijk_Boring;
   }

   if (likely_delay_slot) {
      dres.jk_StopHere = Ijk_Boring;
      dres.whatNext = Dis_StopHere;
      putPC(lastn);
      lastn = NULL;
   }
   if (delay_slot_jump) {
      putPC(lastn);
      lastn = NULL;
      dres.jk_StopHere = is_Branch_or_Jump_and_Link(guest_code + delta - 4) ?
                         Ijk_Call : Ijk_Boring;
   }

 decode_success:
   
   switch (dres.whatNext) {
      case Dis_Continue:
         putPC(mkU32(guest_PC_curr_instr + 4));
         break;
      case Dis_ResteerU:
      case Dis_ResteerC:
         putPC(mkU32(dres.continueAt));
         break;
      case Dis_StopHere:
         break;
      default:
         vassert(0);
         break;
   }

   
   
   if ((vex_control.guest_max_insns - 1) == (delta+4)/4)
      if (branch_or_jump(guest_code + delta + 4)) {
         dres.whatNext = Dis_StopHere;
         dres.jk_StopHere = Ijk_Boring;
         putPC(mkU32(guest_PC_curr_instr + 4));
   }
   dres.len = 4;

   DIP("\n");

   return dres;

}



DisResult
disInstr_MIPS(IRSB*        irsb_IN,
              Bool         (*resteerOkFn) (void *, Addr64),
              Bool         resteerCisOk,
              void*        callback_opaque,
              UChar*       guest_code_IN,
              Long         delta,
              Addr64       guest_IP,
              VexArch      guest_arch,
              VexArchInfo* archinfo,
              VexAbiInfo*  abiinfo,
              Bool         host_bigendian_IN)
{
   DisResult dres;

   
   vassert(guest_arch == VexArchMIPS32);

   mode64 = guest_arch != VexArchMIPS32;

   guest_code = guest_code_IN;
   irsb = irsb_IN;
   host_is_bigendian = host_bigendian_IN;
   guest_PC_curr_instr = (Addr32) guest_IP;
   guest_PC_bbstart = (Addr32) toUInt(guest_IP - delta);

   dres = disInstr_MIPS_WRK(resteerOkFn, resteerCisOk, callback_opaque,
                            delta, archinfo, abiinfo);

   return dres;
}

