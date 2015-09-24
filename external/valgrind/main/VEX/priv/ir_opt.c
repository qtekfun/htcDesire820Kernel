

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

#include "libvex_basictypes.h"
#include "libvex_ir.h"
#include "libvex.h"

#include "main_util.h"
#include "main_globals.h"
#include "ir_opt.h"


#define DEBUG_IROPT 0

#define STATS_IROPT 0







typedef
   struct {
      Bool*  inuse;
      HWord* key;
      HWord* val;
      Int    size;
      Int    used;
   }
   HashHW;

static HashHW* newHHW ( void )
{
   HashHW* h = LibVEX_Alloc(sizeof(HashHW));
   h->size   = 8;
   h->used   = 0;
   h->inuse  = LibVEX_Alloc(h->size * sizeof(Bool));
   h->key    = LibVEX_Alloc(h->size * sizeof(HWord));
   h->val    = LibVEX_Alloc(h->size * sizeof(HWord));
   return h;
}



static Bool lookupHHW ( HashHW* h, HWord* val, HWord key )
{
   Int i;
   
   for (i = 0; i < h->used; i++) {
      if (h->inuse[i] && h->key[i] == key) {
         if (val)
            *val = h->val[i];
         return True;
      }
   }
   return False;
}



static void addToHHW ( HashHW* h, HWord key, HWord val )
{
   Int i, j;
   

   
   for (i = 0; i < h->used; i++) {
      if (h->inuse[i] && h->key[i] == key) {
         h->val[i] = val;
         return;
      }
   }

   
   if (h->used == h->size) {
      
      Bool*  inuse2 = LibVEX_Alloc(2 * h->size * sizeof(Bool));
      HWord* key2   = LibVEX_Alloc(2 * h->size * sizeof(HWord));
      HWord* val2   = LibVEX_Alloc(2 * h->size * sizeof(HWord));
      for (i = j = 0; i < h->size; i++) {
         if (!h->inuse[i]) continue;
         inuse2[j] = True;
         key2[j] = h->key[i];
         val2[j] = h->val[i];
         j++;
      }
      h->used = j;
      h->size *= 2;
      h->inuse = inuse2;
      h->key = key2;
      h->val = val2;
   }

   
   vassert(h->used < h->size);
   h->inuse[h->used] = True;
   h->key[h->used] = key;
   h->val[h->used] = val;
   h->used++;
}




static Bool isFlat ( IRExpr* e )
{
   if (e->tag == Iex_Get) 
      return True;
   if (e->tag == Iex_Binop)
      return toBool( isIRAtom(e->Iex.Binop.arg1) 
                     && isIRAtom(e->Iex.Binop.arg2) );
   if (e->tag == Iex_Load)
      return isIRAtom(e->Iex.Load.addr);
   return False;
}


static IRExpr* flatten_Expr ( IRSB* bb, IRExpr* ex )
{
   Int i;
   IRExpr** newargs;
   IRType ty = typeOfIRExpr(bb->tyenv, ex);
   IRTemp t1;

   switch (ex->tag) {

      case Iex_GetI:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1,
            IRExpr_GetI(ex->Iex.GetI.descr,
                        flatten_Expr(bb, ex->Iex.GetI.ix),
                        ex->Iex.GetI.bias)));
         return IRExpr_RdTmp(t1);

      case Iex_Get:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, 
            IRStmt_WrTmp(t1, ex));
         return IRExpr_RdTmp(t1);

      case Iex_Qop: {
         IRQop* qop = ex->Iex.Qop.details;
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1, 
            IRExpr_Qop(qop->op,
                         flatten_Expr(bb, qop->arg1),
                         flatten_Expr(bb, qop->arg2),
                         flatten_Expr(bb, qop->arg3),
                         flatten_Expr(bb, qop->arg4))));
         return IRExpr_RdTmp(t1);
      }

      case Iex_Triop: {
         IRTriop* triop = ex->Iex.Triop.details;
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1, 
            IRExpr_Triop(triop->op,
                         flatten_Expr(bb, triop->arg1),
                         flatten_Expr(bb, triop->arg2),
                         flatten_Expr(bb, triop->arg3))));
         return IRExpr_RdTmp(t1);
      }

      case Iex_Binop:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1, 
            IRExpr_Binop(ex->Iex.Binop.op,
                         flatten_Expr(bb, ex->Iex.Binop.arg1),
                         flatten_Expr(bb, ex->Iex.Binop.arg2))));
         return IRExpr_RdTmp(t1);

      case Iex_Unop:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1, 
            IRExpr_Unop(ex->Iex.Unop.op,
                        flatten_Expr(bb, ex->Iex.Unop.arg))));
         return IRExpr_RdTmp(t1);

      case Iex_Load:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1,
            IRExpr_Load(ex->Iex.Load.end,
                        ex->Iex.Load.ty, 
                        flatten_Expr(bb, ex->Iex.Load.addr))));
         return IRExpr_RdTmp(t1);

      case Iex_CCall:
         newargs = shallowCopyIRExprVec(ex->Iex.CCall.args);
         for (i = 0; newargs[i]; i++)
            newargs[i] = flatten_Expr(bb, newargs[i]);
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1,
            IRExpr_CCall(ex->Iex.CCall.cee,
                         ex->Iex.CCall.retty,
                         newargs)));
         return IRExpr_RdTmp(t1);

      case Iex_Mux0X:
         t1 = newIRTemp(bb->tyenv, ty);
         addStmtToIRSB(bb, IRStmt_WrTmp(t1,
            IRExpr_Mux0X(flatten_Expr(bb, ex->Iex.Mux0X.cond),
                         flatten_Expr(bb, ex->Iex.Mux0X.expr0),
                         flatten_Expr(bb, ex->Iex.Mux0X.exprX))));
         return IRExpr_RdTmp(t1);

      case Iex_Const:
         if (ex->Iex.Const.con->tag == Ico_F64i) {
            t1 = newIRTemp(bb->tyenv, ty);
            addStmtToIRSB(bb, IRStmt_WrTmp(t1,
               IRExpr_Const(ex->Iex.Const.con)));
            return IRExpr_RdTmp(t1);
         } else {
            
            return ex;
         }

      case Iex_RdTmp:
         return ex;

      default:
         vex_printf("\n");
         ppIRExpr(ex); 
         vex_printf("\n");
         vpanic("flatten_Expr");
   }
}



static void flatten_Stmt ( IRSB* bb, IRStmt* st )
{
   Int i;
   IRExpr  *e1, *e2, *e3, *e4, *e5;
   IRDirty *d,  *d2;
   IRCAS   *cas, *cas2;
   IRPutI  *puti, *puti2;
   switch (st->tag) {
      case Ist_Put:
         if (isIRAtom(st->Ist.Put.data)) {
            addStmtToIRSB(bb, st);
         } else {
            
            e1 = flatten_Expr(bb, st->Ist.Put.data);
            addStmtToIRSB(bb, IRStmt_Put(st->Ist.Put.offset, e1));
         }
         break;
      case Ist_PutI:
         puti = st->Ist.PutI.details;
         e1 = flatten_Expr(bb, puti->ix);
         e2 = flatten_Expr(bb, puti->data);
         puti2 = mkIRPutI(puti->descr, e1, puti->bias, e2);
         addStmtToIRSB(bb, IRStmt_PutI(puti2));
         break;
      case Ist_WrTmp:
         if (isFlat(st->Ist.WrTmp.data)) {
            addStmtToIRSB(bb, st);
         } else {
            
            e1 = flatten_Expr(bb, st->Ist.WrTmp.data);
            addStmtToIRSB(bb, IRStmt_WrTmp(st->Ist.WrTmp.tmp, e1));
         }
         break;
      case Ist_Store:
         e1 = flatten_Expr(bb, st->Ist.Store.addr);
         e2 = flatten_Expr(bb, st->Ist.Store.data);
         addStmtToIRSB(bb, IRStmt_Store(st->Ist.Store.end, e1,e2));
         break;
      case Ist_CAS:
         cas  = st->Ist.CAS.details;
         e1   = flatten_Expr(bb, cas->addr);
         e2   = cas->expdHi ? flatten_Expr(bb, cas->expdHi) : NULL;
         e3   = flatten_Expr(bb, cas->expdLo);
         e4   = cas->dataHi ? flatten_Expr(bb, cas->dataHi) : NULL;
         e5   = flatten_Expr(bb, cas->dataLo);
         cas2 = mkIRCAS( cas->oldHi, cas->oldLo, cas->end,
                         e1, e2, e3, e4, e5 );
         addStmtToIRSB(bb, IRStmt_CAS(cas2));
         break;
      case Ist_LLSC:
         e1 = flatten_Expr(bb, st->Ist.LLSC.addr);
         e2 = st->Ist.LLSC.storedata
                 ? flatten_Expr(bb, st->Ist.LLSC.storedata)
                 : NULL;
         addStmtToIRSB(bb, IRStmt_LLSC(st->Ist.LLSC.end,
                                       st->Ist.LLSC.result, e1, e2));
         break;
      case Ist_Dirty:
         d = st->Ist.Dirty.details;
         d2 = emptyIRDirty();
         *d2 = *d;
         d2->args = shallowCopyIRExprVec(d2->args);
         if (d2->mFx != Ifx_None) {
            d2->mAddr = flatten_Expr(bb, d2->mAddr);
         } else {
            vassert(d2->mAddr == NULL);
         }
         d2->guard = flatten_Expr(bb, d2->guard);
         for (i = 0; d2->args[i]; i++)
            d2->args[i] = flatten_Expr(bb, d2->args[i]);
         addStmtToIRSB(bb, IRStmt_Dirty(d2));
         break;
      case Ist_NoOp:
      case Ist_MBE:
      case Ist_IMark:
         addStmtToIRSB(bb, st);
         break;
      case Ist_AbiHint:
         e1 = flatten_Expr(bb, st->Ist.AbiHint.base);
         e2 = flatten_Expr(bb, st->Ist.AbiHint.nia);
         addStmtToIRSB(bb, IRStmt_AbiHint(e1, st->Ist.AbiHint.len, e2));
         break;
      case Ist_Exit:
         e1 = flatten_Expr(bb, st->Ist.Exit.guard);
         addStmtToIRSB(bb, IRStmt_Exit(e1, st->Ist.Exit.jk,
                                       st->Ist.Exit.dst,
                                       st->Ist.Exit.offsIP));
         break;
      default:
         vex_printf("\n");
         ppIRStmt(st); 
         vex_printf("\n");
         vpanic("flatten_Stmt");
   }
}


static IRSB* flatten_BB ( IRSB* in )
{
   Int   i;
   IRSB* out;
   out = emptyIRSB();
   out->tyenv = deepCopyIRTypeEnv( in->tyenv );
   for (i = 0; i < in->stmts_used; i++)
      if (in->stmts[i])
         flatten_Stmt( out, in->stmts[i] );
   out->next     = flatten_Expr( out, in->next );
   out->jumpkind = in->jumpkind;
   out->offsIP   = in->offsIP;
   return out;
}





inline
static void getArrayBounds ( IRRegArray* descr, 
                             UInt* minoff, UInt* maxoff )
{
   *minoff = descr->base;
   *maxoff = *minoff + descr->nElems*sizeofIRType(descr->elemTy) - 1;
   vassert((*minoff & ~0xFFFF) == 0);
   vassert((*maxoff & ~0xFFFF) == 0);
   vassert(*minoff <= *maxoff);
}


static UInt mk_key_GetPut ( Int offset, IRType ty )
{
   
   UInt minoff = offset;
   UInt maxoff = minoff + sizeofIRType(ty) - 1;
   vassert((minoff & ~0xFFFF) == 0);
   vassert((maxoff & ~0xFFFF) == 0);
   return (minoff << 16) | maxoff;
}

static UInt mk_key_GetIPutI ( IRRegArray* descr )
{
   UInt minoff, maxoff;
   getArrayBounds( descr, &minoff, &maxoff );
   vassert((minoff & ~0xFFFF) == 0);
   vassert((maxoff & ~0xFFFF) == 0);
   return (minoff << 16) | maxoff;
}

static void invalidateOverlaps ( HashHW* h, UInt k_lo, UInt k_hi )
{
   Int  j;
   UInt e_lo, e_hi;
   vassert(k_lo <= k_hi);
   

   for (j = 0; j < h->used; j++) {
      if (!h->inuse[j]) 
         continue;
      e_lo = (((UInt)h->key[j]) >> 16) & 0xFFFF;
      e_hi = ((UInt)h->key[j]) & 0xFFFF;
      vassert(e_lo <= e_hi);
      if (e_hi < k_lo || k_hi < e_lo)
         continue; 
      else
         
         h->inuse[j] = False;
   }
}


static void redundant_get_removal_BB ( IRSB* bb )
{
   HashHW* env = newHHW();
   UInt    key = 0; 
   Int     i, j;
   HWord   val;

   for (i = 0; i < bb->stmts_used; i++) {
      IRStmt* st = bb->stmts[i];

      if (st->tag == Ist_NoOp)
         continue;

      
      if (st->tag == Ist_WrTmp
          && st->Ist.WrTmp.data->tag == Iex_Get) {
         IRExpr* get = st->Ist.WrTmp.data;
         key = (HWord)mk_key_GetPut( get->Iex.Get.offset, 
                                     get->Iex.Get.ty );
         if (lookupHHW(env, &val, (HWord)key)) {
            
            IRExpr* valE    = (IRExpr*)val;
            Bool    typesOK = toBool( typeOfIRExpr(bb->tyenv,valE) 
                                      == st->Ist.WrTmp.data->Iex.Get.ty );
            if (typesOK && DEBUG_IROPT) {
               vex_printf("rGET: "); ppIRExpr(get);
               vex_printf("  ->  "); ppIRExpr(valE);
               vex_printf("\n");
            }
            if (typesOK)
               bb->stmts[i] = IRStmt_WrTmp(st->Ist.WrTmp.tmp, valE);
         } else {
            addToHHW( env, (HWord)key, 
                           (HWord)(void*)(IRExpr_RdTmp(st->Ist.WrTmp.tmp)) );
         }
      }

      if (st->tag == Ist_Put || st->tag == Ist_PutI) {
         UInt k_lo, k_hi;
         if (st->tag == Ist_Put) {
            key = mk_key_GetPut( st->Ist.Put.offset, 
                                 typeOfIRExpr(bb->tyenv,st->Ist.Put.data) );
         } else {
            vassert(st->tag == Ist_PutI);
            key = mk_key_GetIPutI( st->Ist.PutI.details->descr );
         }

         k_lo = (key >> 16) & 0xFFFF;
         k_hi = key & 0xFFFF;
         invalidateOverlaps(env, k_lo, k_hi);
      }
      else
      if (st->tag == Ist_Dirty) {
         IRDirty* d      = st->Ist.Dirty.details;
         Bool     writes = False;
         for (j = 0; j < d->nFxState; j++) {
            if (d->fxState[j].fx == Ifx_Modify 
                || d->fxState[j].fx == Ifx_Write)
            writes = True;
         }
         if (writes) {
            
            for (j = 0; j < env->used; j++)
               env->inuse[j] = False;
            if (0) vex_printf("rGET: trash env due to dirty helper\n");
         }
      }

      
      if (st->tag == Ist_Put) {
         vassert(isIRAtom(st->Ist.Put.data));
         addToHHW( env, (HWord)key, (HWord)(st->Ist.Put.data));
      }

   } 

}




static void handle_gets_Stmt ( 
               HashHW* env, 
               IRStmt* st,
               Bool (*preciseMemExnsFn)(Int,Int)
            )
{
   Int     j;
   UInt    key = 0; 
   Bool    isGet;
   Bool    memRW = False;
   IRExpr* e;

   switch (st->tag) {

      case Ist_WrTmp:
         e = st->Ist.WrTmp.data;
         switch (e->tag) {
            case Iex_Get:
               isGet = True;
               key = mk_key_GetPut ( e->Iex.Get.offset, e->Iex.Get.ty );
               break;
            case Iex_GetI:
               isGet = True;
               key = mk_key_GetIPutI ( e->Iex.GetI.descr );
               break;
            case Iex_Load:
               isGet = False;
               memRW = True;
               break;
            default: 
               isGet = False;
         }
         if (isGet) {
            UInt k_lo, k_hi;
            k_lo = (key >> 16) & 0xFFFF;
            k_hi = key & 0xFFFF;
            invalidateOverlaps(env, k_lo, k_hi);
         }
         break;

      case Ist_AbiHint:
         vassert(isIRAtom(st->Ist.AbiHint.base));
         vassert(isIRAtom(st->Ist.AbiHint.nia));
         
      case Ist_MBE:
      case Ist_Dirty:
      case Ist_CAS:
      case Ist_LLSC:
         for (j = 0; j < env->used; j++)
            env->inuse[j] = False;
         break;

      
      case Ist_Store:
         vassert(isIRAtom(st->Ist.Store.addr));
         vassert(isIRAtom(st->Ist.Store.data));
         memRW = True;
         break;

      case Ist_Exit:
         vassert(isIRAtom(st->Ist.Exit.guard));
         break;

      case Ist_PutI:
         vassert(isIRAtom(st->Ist.PutI.details->ix));
         vassert(isIRAtom(st->Ist.PutI.details->data));
         break;

      case Ist_NoOp:
      case Ist_IMark:
         break;

      default:
         vex_printf("\n");
         ppIRStmt(st);
         vex_printf("\n");
         vpanic("handle_gets_Stmt");
   }

   if (memRW) {
      switch (vex_control.iropt_register_updates) {
         case VexRegUpdAllregsAtMemAccess:
            for (j = 0; j < env->used; j++)
               env->inuse[j] = False;
            break;
         case VexRegUpdUnwindregsAtMemAccess:
            for (j = 0; j < env->used; j++) {
               if (!env->inuse[j])
                  continue;
               HWord k_lo = (env->key[j] >> 16) & 0xFFFF;
               HWord k_hi = env->key[j] & 0xFFFF;
               if (preciseMemExnsFn( k_lo, k_hi ))
                  env->inuse[j] = False;
            }
            break;
         default:
            
            
            vassert(0);
      }
   } 

}



static void redundant_put_removal_BB ( 
               IRSB* bb,
               Bool (*preciseMemExnsFn)(Int,Int)
            )
{
   Int     i, j;
   Bool    isPut;
   IRStmt* st;
   UInt    key = 0; 

   vassert
      (vex_control.iropt_register_updates == VexRegUpdUnwindregsAtMemAccess
       || vex_control.iropt_register_updates == VexRegUpdAllregsAtMemAccess);

   HashHW* env = newHHW();

   key = mk_key_GetPut(bb->offsIP, typeOfIRExpr(bb->tyenv, bb->next));
   addToHHW(env, (HWord)key, 0);

   
   for (i = bb->stmts_used-1; i >= 0; i--) {
      st = bb->stmts[i];

      if (st->tag == Ist_NoOp)
         continue;

      
      if (st->tag == Ist_Exit) {
         
         /* Need to throw out from the env, any part of it which
            doesn't overlap with the guest state written by this exit.
            Since the exit only writes one section, it's simplest to
            do this: (1) check whether env contains a write that
            completely overlaps the write done by this exit; (2) empty
            out env; and (3) if (1) was true, add the write done by
            this exit.

            To make (1) a bit simpler, merely search for a write that
            exactly matches the one done by this exit.  That's safe
            because it will fail as often or more often than a full
            overlap check, and failure to find an overlapping write in
            env is the safe case (we just nuke env if that
            happens). */
         
         
         
         
         
         
         for (j = 0; j < env->used; j++)
            env->inuse[j] = False;
         
         
         
         continue;
      }

      
      switch (st->tag) {
         case Ist_Put: 
            isPut = True;
            key = mk_key_GetPut( st->Ist.Put.offset, 
                                 typeOfIRExpr(bb->tyenv,st->Ist.Put.data) );
            vassert(isIRAtom(st->Ist.Put.data));
            break;
         case Ist_PutI:
            isPut = True;
            key = mk_key_GetIPutI( st->Ist.PutI.details->descr );
            vassert(isIRAtom(st->Ist.PutI.details->ix));
            vassert(isIRAtom(st->Ist.PutI.details->data));
            break;
         default: 
            isPut = False;
      }
      if (isPut && st->tag != Ist_PutI) {
         if (lookupHHW(env, NULL, (HWord)key)) {
            if (DEBUG_IROPT) {
               vex_printf("rPUT: "); ppIRStmt(st);
               vex_printf("\n");
            }
            bb->stmts[i] = IRStmt_NoOp();
         } else {
            addToHHW(env, (HWord)key, 0);
         }
         continue;
      }

      handle_gets_Stmt( env, st, preciseMemExnsFn );
   }
}



#if STATS_IROPT
static UInt invocation_count;
static UInt recursion_count;
static UInt success_count;
static UInt recursion_success_count;
static Bool recursion_helped;
static Bool recursed;
static UInt max_nodes_visited;
#endif 

static UInt num_nodes_visited;

#define NODE_LIMIT 30





__attribute__((noinline))
static Bool sameIRExprs_aux2 ( IRExpr** env, IRExpr* e1, IRExpr* e2 );

inline
static Bool sameIRExprs_aux ( IRExpr** env, IRExpr* e1, IRExpr* e2 )
{
   if (e1->tag != e2->tag) return False;
   return sameIRExprs_aux2(env, e1, e2);
}

__attribute__((noinline))
static Bool sameIRExprs_aux2 ( IRExpr** env, IRExpr* e1, IRExpr* e2 )
{
   if (num_nodes_visited++ > NODE_LIMIT) return False;

   switch (e1->tag) {
      case Iex_RdTmp:
         if (e1->Iex.RdTmp.tmp == e2->Iex.RdTmp.tmp) return True;

         if (env[e1->Iex.RdTmp.tmp] && env[e2->Iex.RdTmp.tmp]) {
            Bool same = sameIRExprs_aux(env, env[e1->Iex.RdTmp.tmp],
                                        env[e2->Iex.RdTmp.tmp]);
#if STATS_IROPT
            recursed = True;
            if (same) recursion_helped = True;
#endif
            return same;
         }
         return False;

      case Iex_Get:
      case Iex_GetI:
      case Iex_Load:
         
         return False;

      case Iex_Binop:
         return toBool( e1->Iex.Binop.op == e2->Iex.Binop.op
                        && sameIRExprs_aux( env, e1->Iex.Binop.arg1,
                                                 e2->Iex.Binop.arg1 )
                        && sameIRExprs_aux( env, e1->Iex.Binop.arg2,
                                                 e2->Iex.Binop.arg2 ));

      case Iex_Unop:
         return toBool( e1->Iex.Unop.op == e2->Iex.Unop.op
                        && sameIRExprs_aux( env, e1->Iex.Unop.arg,
                                                 e2->Iex.Unop.arg ));

      case Iex_Const: {
         IRConst *c1 = e1->Iex.Const.con;
         IRConst *c2 = e2->Iex.Const.con;
         vassert(c1->tag == c2->tag);
         switch (c1->tag) {
            case Ico_U1:   return toBool( c1->Ico.U1  == c2->Ico.U1 );
            case Ico_U8:   return toBool( c1->Ico.U8  == c2->Ico.U8 );
            case Ico_U16:  return toBool( c1->Ico.U16 == c2->Ico.U16 );
            case Ico_U32:  return toBool( c1->Ico.U32 == c2->Ico.U32 );
            case Ico_U64:  return toBool( c1->Ico.U64 == c2->Ico.U64 );
            default: break;
         }
         return False;
      }

      case Iex_Triop: {
         IRTriop *tri1 = e1->Iex.Triop.details;
         IRTriop *tri2 = e2->Iex.Triop.details;
         return toBool( tri1->op == tri2->op
                        && sameIRExprs_aux( env, tri1->arg1, tri2->arg1 )
                        && sameIRExprs_aux( env, tri1->arg2, tri2->arg2 )
                        && sameIRExprs_aux( env, tri1->arg3, tri2->arg3 ));
      }

      case Iex_Mux0X:
         return toBool(    sameIRExprs_aux( env, e1->Iex.Mux0X.cond,
                                                 e2->Iex.Mux0X.cond )
                        && sameIRExprs_aux( env, e1->Iex.Mux0X.expr0,
                                                 e2->Iex.Mux0X.expr0 )
                        && sameIRExprs_aux( env, e1->Iex.Mux0X.exprX,
                                                 e2->Iex.Mux0X.exprX ));

      default:
         
         break;
   }

   return False;
}

inline
static Bool sameIRExprs ( IRExpr** env, IRExpr* e1, IRExpr* e2 )
{
   Bool same;

   num_nodes_visited = 0;
   same = sameIRExprs_aux(env, e1, e2);

#if STATS_IROPT
   ++invocation_count;
   if (recursed) ++recursion_count;
   success_count += same;
   if (same && recursion_helped)
      ++recursion_success_count;
   if (num_nodes_visited > max_nodes_visited)
      max_nodes_visited = num_nodes_visited;
   recursed = False; 
   recursion_helped = False;  
#endif 

   return same;
}


static
Bool debug_only_hack_sameIRExprs_might_assert ( IRExpr* e1, IRExpr* e2 )
{
   if (e1->tag != e2->tag) return False;
   switch (e1->tag) {
      case Iex_Const: {
         
         IRConst *c1 = e1->Iex.Const.con;
         IRConst *c2 = e2->Iex.Const.con;
         return c1->tag != c2->tag;
      }
      default:
         break;
   }
   return False;
}


static Bool isZeroU32 ( IRExpr* e )
{
   return toBool( e->tag == Iex_Const 
                  && e->Iex.Const.con->tag == Ico_U32
                  && e->Iex.Const.con->Ico.U32 == 0);
}

static Bool isOnesU32 ( IRExpr* e )
{
   return toBool( e->tag == Iex_Const 
                  && e->Iex.Const.con->tag == Ico_U32
                  && e->Iex.Const.con->Ico.U32 == 0xFFFFFFFF );
}

static Bool isZeroU64 ( IRExpr* e )
{
   return toBool( e->tag == Iex_Const 
                  && e->Iex.Const.con->tag == Ico_U64
                  && e->Iex.Const.con->Ico.U64 == 0);
}

static Bool isZeroU ( IRExpr* e )
{
   if (e->tag != Iex_Const) return False;
   switch (e->Iex.Const.con->tag) {
      case Ico_U1:    return toBool( e->Iex.Const.con->Ico.U1  == 0);
      case Ico_U8:    return toBool( e->Iex.Const.con->Ico.U8  == 0);
      case Ico_U16:   return toBool( e->Iex.Const.con->Ico.U16 == 0);
      case Ico_U32:   return toBool( e->Iex.Const.con->Ico.U32 == 0);
      case Ico_U64:   return toBool( e->Iex.Const.con->Ico.U64 == 0);
      default: vpanic("isZeroU");
   }
}

static Bool isOnesU ( IRExpr* e )
{
   if (e->tag != Iex_Const) return False;
   switch (e->Iex.Const.con->tag) {
      case Ico_U8:    return toBool( e->Iex.Const.con->Ico.U8  == 0xFF);
      case Ico_U16:   return toBool( e->Iex.Const.con->Ico.U16 == 0xFFFF);
      case Ico_U32:   return toBool( e->Iex.Const.con->Ico.U32
                                     == 0xFFFFFFFF);
      case Ico_U64:   return toBool( e->Iex.Const.con->Ico.U64
                                     == 0xFFFFFFFFFFFFFFFFULL);
      default: ppIRExpr(e); vpanic("isOnesU");
   }
}

static Bool notBool ( Bool b )
{
   if (b == True) return False;
   if (b == False) return True;
   vpanic("notBool");
}

static IRExpr* mkZeroOfPrimopResultType ( IROp op )
{
   switch (op) {
      case Iop_CmpNE32: return IRExpr_Const(IRConst_U1(toBool(0)));
      case Iop_Xor8:  return IRExpr_Const(IRConst_U8(0));
      case Iop_Xor16: return IRExpr_Const(IRConst_U16(0));
      case Iop_Sub32:
      case Iop_Xor32: return IRExpr_Const(IRConst_U32(0));
      case Iop_Sub64:
      case Iop_Xor64: return IRExpr_Const(IRConst_U64(0));
      case Iop_XorV128: return IRExpr_Const(IRConst_V128(0));
      default: vpanic("mkZeroOfPrimopResultType: bad primop");
   }
}

static IRExpr* mkOnesOfPrimopResultType ( IROp op )
{
   switch (op) {
      case Iop_CmpEQ32:
      case Iop_CmpEQ64:
         return IRExpr_Const(IRConst_U1(toBool(1)));
      case Iop_Or8:
         return IRExpr_Const(IRConst_U8(0xFF));
      case Iop_Or16:
         return IRExpr_Const(IRConst_U16(0xFFFF));
      case Iop_Or32:
         return IRExpr_Const(IRConst_U32(0xFFFFFFFF));
      case Iop_CmpEQ8x8:
      case Iop_Or64:
         return IRExpr_Const(IRConst_U64(0xFFFFFFFFFFFFFFFFULL));
      case Iop_CmpEQ8x16:
      case Iop_CmpEQ16x8:
      case Iop_CmpEQ32x4:
         return IRExpr_Const(IRConst_V128(0xFFFF));
      default:
         ppIROp(op);
         vpanic("mkOnesOfPrimopResultType: bad primop");
   }
}

static UInt fold_Clz64 ( ULong value )
{
   UInt i;
   vassert(value != 0ULL); 
   for (i = 0; i < 64; ++i) {
      if (0ULL != (value & (((ULong)1) << (63 - i)))) return i;
   }
   vassert(0);
   
   return 0;
}

static UInt fold_Clz32 ( UInt value )
{
   UInt i;
   vassert(value != 0); 
   for (i = 0; i < 32; ++i) {
      if (0 != (value & (((UInt)1) << (31 - i)))) return i;
   }
   vassert(0);
   
   return 0;
}


static IRExpr* fold_Expr ( IRExpr** env, IRExpr* e )
{
   Int     shift;
   IRExpr* e2 = e; 

   switch (e->tag) {
   case Iex_Unop:
      
      if (e->Iex.Unop.arg->tag == Iex_Const) {
         switch (e->Iex.Unop.op) {
         case Iop_1Uto8:
            e2 = IRExpr_Const(IRConst_U8(toUChar(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 1 : 0)));
            break;
         case Iop_1Uto32:
            e2 = IRExpr_Const(IRConst_U32(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 1 : 0));
            break;
         case Iop_1Uto64:
            e2 = IRExpr_Const(IRConst_U64(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 1 : 0));
            break;

         case Iop_1Sto8:
            e2 = IRExpr_Const(IRConst_U8(toUChar(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 0xFF : 0)));
            break;
         case Iop_1Sto16:
            e2 = IRExpr_Const(IRConst_U16(toUShort(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 0xFFFF : 0)));
            break;
         case Iop_1Sto32:
            e2 = IRExpr_Const(IRConst_U32(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 0xFFFFFFFF : 0));
            break;
         case Iop_1Sto64:
            e2 = IRExpr_Const(IRConst_U64(
                    e->Iex.Unop.arg->Iex.Const.con->Ico.U1
                    ? 0xFFFFFFFFFFFFFFFFULL : 0));
            break;

         case Iop_8Sto32: {
             Int s32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U8;
            s32 <<= 24;
            s32 >>= 24;
            e2 = IRExpr_Const(IRConst_U32((UInt)s32));
            break;
         }
         case Iop_16Sto32: {
             Int s32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U16;
            s32 <<= 16;
            s32 >>= 16;
            e2 = IRExpr_Const(IRConst_U32( (UInt)s32) );
            break;
         }
         case Iop_8Uto64:
            e2 = IRExpr_Const(IRConst_U64(
                    0xFFULL & e->Iex.Unop.arg->Iex.Const.con->Ico.U8));
            break;
         case Iop_16Uto64:
            e2 = IRExpr_Const(IRConst_U64(
                    0xFFFFULL & e->Iex.Unop.arg->Iex.Const.con->Ico.U16));
            break;
         case Iop_8Uto32:
            e2 = IRExpr_Const(IRConst_U32(
                    0xFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U8));
            break;
         case Iop_8Sto16: {
             Short s16 = e->Iex.Unop.arg->Iex.Const.con->Ico.U8;
            s16 <<= 8;
            s16 >>= 8;
            e2 = IRExpr_Const(IRConst_U16( (UShort)s16) );
            break;
         }
         case Iop_8Uto16:
            e2 = IRExpr_Const(IRConst_U16(
                    0xFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U8));
            break;
         case Iop_16Uto32:
            e2 = IRExpr_Const(IRConst_U32(
                    0xFFFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U16));
            break;
         case Iop_32to16:
            e2 = IRExpr_Const(IRConst_U16(toUShort(
                    0xFFFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U32)));
            break;
         case Iop_32to8:
            e2 = IRExpr_Const(IRConst_U8(toUChar(
                    0xFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U32)));
            break;
         case Iop_32to1:
            e2 = IRExpr_Const(IRConst_U1(toBool(
                    1 == (1 & e->Iex.Unop.arg->Iex.Const.con->Ico.U32)
                 )));
            break;
         case Iop_64to1:
            e2 = IRExpr_Const(IRConst_U1(toBool(
                    1 == (1 & e->Iex.Unop.arg->Iex.Const.con->Ico.U64)
                 )));
            break;

         case Iop_NotV128:
            e2 = IRExpr_Const(IRConst_V128(
                    ~ (e->Iex.Unop.arg->Iex.Const.con->Ico.V128)));
            break;
         case Iop_Not64:
            e2 = IRExpr_Const(IRConst_U64(
                    ~ (e->Iex.Unop.arg->Iex.Const.con->Ico.U64)));
            break;
         case Iop_Not32:
            e2 = IRExpr_Const(IRConst_U32(
                    ~ (e->Iex.Unop.arg->Iex.Const.con->Ico.U32)));
            break;
         case Iop_Not16:
            e2 = IRExpr_Const(IRConst_U16(toUShort(
                    ~ (e->Iex.Unop.arg->Iex.Const.con->Ico.U16))));
            break;
         case Iop_Not8:
            e2 = IRExpr_Const(IRConst_U8(toUChar(
                    ~ (e->Iex.Unop.arg->Iex.Const.con->Ico.U8))));
            break;

         case Iop_Not1:
            e2 = IRExpr_Const(IRConst_U1(
                    notBool(e->Iex.Unop.arg->Iex.Const.con->Ico.U1)));
            break;

         case Iop_64to8: {
            ULong w64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            w64 &= 0xFFULL;
            e2 = IRExpr_Const(IRConst_U8( (UChar)w64 ));
            break;
         }
         case Iop_64to16: {
            ULong w64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            w64 &= 0xFFFFULL;
            e2 = IRExpr_Const(IRConst_U16( (UShort)w64 ));
            break;
         }
         case Iop_64to32: {
            ULong w64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            w64 &= 0x00000000FFFFFFFFULL;
            e2 = IRExpr_Const(IRConst_U32( (UInt)w64 ));
            break;
         }
         case Iop_64HIto32: {
            ULong w64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            w64 >>= 32;
            e2 = IRExpr_Const(IRConst_U32( (UInt)w64 ));
            break;
         }
         case Iop_32Uto64:
            e2 = IRExpr_Const(IRConst_U64(
                    0xFFFFFFFFULL 
                    & e->Iex.Unop.arg->Iex.Const.con->Ico.U32));
            break;
         case Iop_16Sto64: {
             Long s64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U16;
            s64 <<= 48;
            s64 >>= 48;
            e2 = IRExpr_Const(IRConst_U64((ULong)s64));
            break;
         }
         case Iop_32Sto64: {
             Long s64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U32;
            s64 <<= 32;
            s64 >>= 32;
            e2 = IRExpr_Const(IRConst_U64((ULong)s64));
            break;
         }

         case Iop_16to8: {
            UShort w16 = e->Iex.Unop.arg->Iex.Const.con->Ico.U16;
            w16 &= 0xFF;
            e2 = IRExpr_Const(IRConst_U8( (UChar)w16 ));
            break;
         }
         case Iop_16HIto8: {
            UShort w16 = e->Iex.Unop.arg->Iex.Const.con->Ico.U16;
            w16 >>= 8;
            w16 &= 0xFF;
            e2 = IRExpr_Const(IRConst_U8( (UChar)w16 ));
            break;
         }

         case Iop_CmpNEZ8:
            e2 = IRExpr_Const(IRConst_U1(toBool(
                    0 != 
                    (0xFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U8)
                 )));
            break;
         case Iop_CmpNEZ32:
            e2 = IRExpr_Const(IRConst_U1(toBool(
                    0 != 
                    (0xFFFFFFFF & e->Iex.Unop.arg->Iex.Const.con->Ico.U32)
                 )));
            break;
         case Iop_CmpNEZ64:
            e2 = IRExpr_Const(IRConst_U1(toBool(
                    0ULL != e->Iex.Unop.arg->Iex.Const.con->Ico.U64
                 )));
            break;

         case Iop_CmpwNEZ32: {
            UInt w32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U32;
            if (w32 == 0)
               e2 = IRExpr_Const(IRConst_U32( 0 ));
            else
               e2 = IRExpr_Const(IRConst_U32( 0xFFFFFFFF ));
            break;
         }
         case Iop_CmpwNEZ64: {
            ULong w64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            if (w64 == 0)
               e2 = IRExpr_Const(IRConst_U64( 0 ));
            else
               e2 = IRExpr_Const(IRConst_U64( 0xFFFFFFFFFFFFFFFFULL ));
            break;
         }

         case Iop_Left32: {
            UInt u32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U32;
            Int  s32 = (Int)(u32 & 0xFFFFFFFF);
            s32 = (s32 | (-s32));
            e2 = IRExpr_Const( IRConst_U32( (UInt)s32 ));
            break;
         }

         case Iop_Left64: {
            ULong u64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            Long  s64 = (Long)u64;
            s64 = (s64 | (-s64));
            e2 = IRExpr_Const( IRConst_U64( (ULong)s64 ));
            break;
         }

         case Iop_Clz32: {
            UInt u32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U32;
            if (u32 != 0)
               e2 = IRExpr_Const(IRConst_U32(fold_Clz32(u32)));
            break;
         }
         case Iop_Clz64: {
            ULong u64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            if (u64 != 0ULL)
               e2 = IRExpr_Const(IRConst_U64(fold_Clz64(u64)));
            break;
         }

         case Iop_32UtoV128: {
            UInt u32 = e->Iex.Unop.arg->Iex.Const.con->Ico.U32;
            if (0 == u32) {
               e2 = IRExpr_Const(IRConst_V128(0x0000));
            } else {
               goto unhandled;
            }
            break;
         }
         case Iop_V128to64: {
            UShort v128 = e->Iex.Unop.arg->Iex.Const.con->Ico.V128;
            if (0 == ((v128 >> 0) & 0xFF)) {
               e2 = IRExpr_Const(IRConst_U64(0));
            } else {
               goto unhandled;
            }
            break;
         }
         case Iop_V128HIto64: {
            UShort v128 = e->Iex.Unop.arg->Iex.Const.con->Ico.V128;
            if (0 == ((v128 >> 8) & 0xFF)) {
               e2 = IRExpr_Const(IRConst_U64(0));
            } else {
               goto unhandled;
            }
            break;
         }
         case Iop_64UtoV128: {
            ULong u64 = e->Iex.Unop.arg->Iex.Const.con->Ico.U64;
            if (0 == u64) {
               e2 = IRExpr_Const(IRConst_V128(0x0000));
            } else {
               goto unhandled;
            }
            break;
         }

         
         case Iop_V256to64_0: case Iop_V256to64_1:
         case Iop_V256to64_2: case Iop_V256to64_3: {
            UInt v256 = e->Iex.Unop.arg->Iex.Const.con->Ico.V256;
            if (v256 == 0x00000000) {
               e2 = IRExpr_Const(IRConst_U64(0));
            } else {
               goto unhandled;
            }
            break;
         }

         default: 
            goto unhandled;
      }
      }
      break;

   case Iex_Binop:
      
      if (e->Iex.Binop.arg1->tag == Iex_Const
          && e->Iex.Binop.arg2->tag == Iex_Const) {
         
         switch (e->Iex.Binop.op) {

            
            case Iop_Or8:
               e2 = IRExpr_Const(IRConst_U8(toUChar( 
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U8
                        | e->Iex.Binop.arg2->Iex.Const.con->Ico.U8))));
               break;
            case Iop_Or16:
               e2 = IRExpr_Const(IRConst_U16(toUShort(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U16
                        | e->Iex.Binop.arg2->Iex.Const.con->Ico.U16))));
               break;
            case Iop_Or32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        | e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_Or64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        | e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;
            case Iop_OrV128:
               e2 = IRExpr_Const(IRConst_V128(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.V128
                        | e->Iex.Binop.arg2->Iex.Const.con->Ico.V128)));
               break;

            
            case Iop_Xor8:
               e2 = IRExpr_Const(IRConst_U8(toUChar( 
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U8
                        ^ e->Iex.Binop.arg2->Iex.Const.con->Ico.U8))));
               break;
            case Iop_Xor16:
               e2 = IRExpr_Const(IRConst_U16(toUShort(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U16
                        ^ e->Iex.Binop.arg2->Iex.Const.con->Ico.U16))));
               break;
            case Iop_Xor32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        ^ e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_Xor64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        ^ e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;
            case Iop_XorV128:
               e2 = IRExpr_Const(IRConst_V128(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.V128
                        ^ e->Iex.Binop.arg2->Iex.Const.con->Ico.V128)));
               break;

            
            case Iop_And8:
               e2 = IRExpr_Const(IRConst_U8(toUChar( 
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U8
                        & e->Iex.Binop.arg2->Iex.Const.con->Ico.U8))));
               break;
            case Iop_And16:
               e2 = IRExpr_Const(IRConst_U16(toUShort(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U16
                        & e->Iex.Binop.arg2->Iex.Const.con->Ico.U16))));
               break;
            case Iop_And32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        & e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_And64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        & e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;
            case Iop_AndV128:
               e2 = IRExpr_Const(IRConst_V128(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.V128
                        & e->Iex.Binop.arg2->Iex.Const.con->Ico.V128)));
               break;

            
            case Iop_Add8:
               e2 = IRExpr_Const(IRConst_U8(toUChar( 
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U8
                        + e->Iex.Binop.arg2->Iex.Const.con->Ico.U8))));
               break;
            case Iop_Add32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        + e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_Add64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        + e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;

            
            case Iop_Sub8:
               e2 = IRExpr_Const(IRConst_U8(toUChar( 
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U8
                        - e->Iex.Binop.arg2->Iex.Const.con->Ico.U8))));
               break;
            case Iop_Sub32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        - e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_Sub64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        - e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;

            
            case Iop_Max32U: {
               UInt u32a = e->Iex.Binop.arg1->Iex.Const.con->Ico.U32;
               UInt u32b = e->Iex.Binop.arg2->Iex.Const.con->Ico.U32;
               UInt res  = u32a > u32b ? u32a : u32b;
               e2 = IRExpr_Const(IRConst_U32(res));
               break;
            }

            
            case Iop_Mul32:
               e2 = IRExpr_Const(IRConst_U32(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        * e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)));
               break;
            case Iop_Mul64:
               e2 = IRExpr_Const(IRConst_U64(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        * e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)));
               break;

            case Iop_MullS32: {
               
               UInt  u32a = e->Iex.Binop.arg1->Iex.Const.con->Ico.U32;
               UInt  u32b = e->Iex.Binop.arg2->Iex.Const.con->Ico.U32;
               Int   s32a = (Int)u32a;
               Int   s32b = (Int)u32b;
               Long  s64a = (Long)s32a;
               Long  s64b = (Long)s32b;
               Long  sres = s64a * s64b;
               ULong ures = (ULong)sres;
               e2 = IRExpr_Const(IRConst_U64(ures));
               break;
            }

            
            case Iop_Shl32:
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 31)
                  e2 = IRExpr_Const(IRConst_U32(
                          (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                           << shift)));
               break;
            case Iop_Shl64:
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 63)
                  e2 = IRExpr_Const(IRConst_U64(
                          (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                           << shift)));
               break;

            
            case Iop_Sar32: {
               
                Int s32;
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               s32   = (Int)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 31) {
                  s32 >>= shift;
                  e2 = IRExpr_Const(IRConst_U32((UInt)s32));
               }
               break;
            }
            case Iop_Sar64: {
               
                Long s64;
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               s64   = (Long)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 63) {
                  s64 >>= shift;
                  e2 = IRExpr_Const(IRConst_U64((ULong)s64));
               }
               break;
            }

            
            case Iop_Shr32: {
               
                UInt u32;
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               u32   = (UInt)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 31) {
                  u32 >>= shift;
                  e2 = IRExpr_Const(IRConst_U32(u32));
               }
               break;
            }
            case Iop_Shr64: {
               
                ULong u64;
               vassert(e->Iex.Binop.arg2->Iex.Const.con->tag == Ico_U8);
               u64   = (ULong)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64);
               shift = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U8);
               if (shift >= 0 && shift <= 63) {
                  u64 >>= shift;
                  e2 = IRExpr_Const(IRConst_U64(u64));
               }
               break;
            }

            
            case Iop_CmpEQ32:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        == e->Iex.Binop.arg2->Iex.Const.con->Ico.U32))));
               break;
            case Iop_CmpEQ64:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        == e->Iex.Binop.arg2->Iex.Const.con->Ico.U64))));
               break;

            
            case Iop_CmpNE8:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((0xFF & e->Iex.Binop.arg1->Iex.Const.con->Ico.U8)
                        != (0xFF & e->Iex.Binop.arg2->Iex.Const.con->Ico.U8)))));
               break;
            case Iop_CmpNE32:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U32
                        != e->Iex.Binop.arg2->Iex.Const.con->Ico.U32))));
               break;
            case Iop_CmpNE64:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       (e->Iex.Binop.arg1->Iex.Const.con->Ico.U64
                        != e->Iex.Binop.arg2->Iex.Const.con->Ico.U64))));
               break;

            
            case Iop_CmpLE32U:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((UInt)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32)
                        <= (UInt)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)))));
               break;
            case Iop_CmpLE64U:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((ULong)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64)
                        <= (ULong)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)))));
               break;

            
            case Iop_CmpLE32S:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((Int)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32)
                        <= (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)))));
               break;
            case Iop_CmpLE64S:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((Long)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64)
                        <= (Long)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)))));
               break;

            
            case Iop_CmpLT32S:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((Int)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32)
                        < (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)))));
               break;
            case Iop_CmpLT64S:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((Long)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64)
                        < (Long)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)))));
               break;

            
            case Iop_CmpLT32U:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((UInt)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U32)
                        < (UInt)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)))));
               break;
            case Iop_CmpLT64U:
               e2 = IRExpr_Const(IRConst_U1(toBool(
                       ((ULong)(e->Iex.Binop.arg1->Iex.Const.con->Ico.U64)
                        < (ULong)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U64)))));
               break;

            
            case Iop_CmpORD32S: {
               
               UInt  u32a = e->Iex.Binop.arg1->Iex.Const.con->Ico.U32;
               UInt  u32b = e->Iex.Binop.arg2->Iex.Const.con->Ico.U32;
               Int   s32a = (Int)u32a;
               Int   s32b = (Int)u32b;
               Int   r = 0x2; 
               if (s32a < s32b) {
                  r = 0x8; 
               } 
               else if (s32a > s32b) {
                  r = 0x4; 
               }
               e2 = IRExpr_Const(IRConst_U32(r));
               break;
            }

            
            case Iop_32HLto64:
               e2 = IRExpr_Const(IRConst_U64(
                       (((ULong)(e->Iex.Binop.arg1
                                  ->Iex.Const.con->Ico.U32)) << 32)
                       | ((ULong)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32)) 
                    ));
               break;
            case Iop_64HLto128:
               break;
            case Iop_64HLtoV128: {
               ULong argHi = e->Iex.Binop.arg1->Iex.Const.con->Ico.U64;
               ULong argLo = e->Iex.Binop.arg2->Iex.Const.con->Ico.U64;
               if (0 == argHi && 0 == argLo) {
                  e2 = IRExpr_Const(IRConst_V128(0));
               } else {
                  goto unhandled;
               }
               break;
            }

            
            case Iop_InterleaveLO8x16: {
               UShort arg1 =  e->Iex.Binop.arg1->Iex.Const.con->Ico.V128;
               UShort arg2 =  e->Iex.Binop.arg2->Iex.Const.con->Ico.V128;
               if (0 == arg1 && 0 == arg2) {
                  e2 = IRExpr_Const(IRConst_V128(0));
               } else {
                  goto unhandled;
               }
               break;
            }

            default:
               goto unhandled;
         }

      } else {

         
         switch (e->Iex.Binop.op) {

            case Iop_Shl32:
            case Iop_Shl64:
            case Iop_Shr64:
               
               if (isZeroU(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (isZeroU(e->Iex.Binop.arg1)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_Shr32:
               
               if (isZeroU(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_Or8:
            case Iop_Or16:
            case Iop_Or32:
            case Iop_Or64:
            case Iop_Max32U:
               
               if (isZeroU(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (isZeroU(e->Iex.Binop.arg1)) {
                  e2 = e->Iex.Binop.arg2;
                  break;
               }
               
               
               if (isOnesU(e->Iex.Binop.arg1) || isOnesU(e->Iex.Binop.arg2)) {
                  e2 = mkOnesOfPrimopResultType(e->Iex.Binop.op);
                  break;
               }
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_Add8:
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = IRExpr_Binop(Iop_Shl8, e->Iex.Binop.arg1,
                                    IRExpr_Const(IRConst_U8(1)));
                  break;
               }
               break;

               

            case Iop_Add32:
            case Iop_Add64:
               
               if (isZeroU(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (isZeroU(e->Iex.Binop.arg1)) {
                  e2 = e->Iex.Binop.arg2;
                  break;
               }
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = IRExpr_Binop(
                          e->Iex.Binop.op == Iop_Add32 ? Iop_Shl32 : Iop_Shl64,
                          e->Iex.Binop.arg1, IRExpr_Const(IRConst_U8(1)));
                  break;
               }
               break;

            case Iop_Sub64:
               
               if (isZeroU64(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = mkZeroOfPrimopResultType(e->Iex.Binop.op);
                  break;
               }
               break;

            case Iop_And32:
               
               if (isOnesU32(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (isZeroU32(e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg2;
                  break;
               }
               
               if (isZeroU32(e->Iex.Binop.arg1)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_And8:
            case Iop_And16:
            case Iop_And64:
            case Iop_AndV128:
            case Iop_AndV256:
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_OrV128:
            case Iop_OrV256:
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = e->Iex.Binop.arg1;
                  break;
               }
               break;

            case Iop_Xor8:
            case Iop_Xor16:
            case Iop_Xor32:
            case Iop_Xor64:
            case Iop_XorV128:
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = mkZeroOfPrimopResultType(e->Iex.Binop.op);
                  break;
               }
               break;

            case Iop_Sub32:
            case Iop_CmpNE32:
               
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = mkZeroOfPrimopResultType(e->Iex.Binop.op);
                  break;
               }
               break;

            case Iop_CmpEQ32:
            case Iop_CmpEQ64:
            case Iop_CmpEQ8x8:
            case Iop_CmpEQ8x16:
            case Iop_CmpEQ16x8:
            case Iop_CmpEQ32x4:
               if (sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
                  e2 = mkOnesOfPrimopResultType(e->Iex.Binop.op);
                  break;
               }
               break;

            default:
               break;
         }
      }
      break;

   case Iex_Mux0X:
      

      
      if (e->Iex.Mux0X.cond->tag == Iex_Const) {
         Bool zero;
         
         vassert(e->Iex.Mux0X.cond->Iex.Const.con->tag == Ico_U8);
         zero = toBool(0 == (0xFF & e->Iex.Mux0X.cond
                                     ->Iex.Const.con->Ico.U8));
         e2 = zero ? e->Iex.Mux0X.expr0 : e->Iex.Mux0X.exprX;
      }
      else
      
      if (sameIRExprs(env, e->Iex.Mux0X.expr0,
                      e->Iex.Mux0X.exprX)) {
         e2 = e->Iex.Mux0X.expr0;
      }
      break;

   default:
      
      break;
   }

   if (vex_control.iropt_verbosity > 0 
       && e == e2 
       && e->tag == Iex_Binop
       && !debug_only_hack_sameIRExprs_might_assert(e->Iex.Binop.arg1,
                                                    e->Iex.Binop.arg2)
       && sameIRExprs(env, e->Iex.Binop.arg1, e->Iex.Binop.arg2)) {
      vex_printf("vex iropt: fold_Expr: no ident rule for: ");
      ppIRExpr(e);
      vex_printf("\n");
   }

   
   if (DEBUG_IROPT && e2 != e) {
      vex_printf("FOLD: "); 
      ppIRExpr(e); vex_printf("  ->  ");
      ppIRExpr(e2); vex_printf("\n");
   }

   return e2;

 unhandled:
#  if 0
   vex_printf("\n\n");
   ppIRExpr(e);
   vpanic("fold_Expr: no rule for the above");
#  else
   if (vex_control.iropt_verbosity > 0) {
      vex_printf("vex iropt: fold_Expr: no const rule for: ");
      ppIRExpr(e);
      vex_printf("\n");
   }
   return e2;
#  endif
}



static IRExpr* subst_Expr ( IRExpr** env, IRExpr* ex )
{
   switch (ex->tag) {
      case Iex_RdTmp:
         if (env[(Int)ex->Iex.RdTmp.tmp] != NULL) {
            IRExpr *rhs = env[(Int)ex->Iex.RdTmp.tmp];
            if (rhs->tag == Iex_RdTmp)
               return rhs;
            if (rhs->tag == Iex_Const
                && rhs->Iex.Const.con->tag != Ico_F64i)
               return rhs;
         }
         
         return ex;

      case Iex_Const:
      case Iex_Get:
         return ex;

      case Iex_GetI:
         vassert(isIRAtom(ex->Iex.GetI.ix));
         return IRExpr_GetI(
            ex->Iex.GetI.descr,
            subst_Expr(env, ex->Iex.GetI.ix),
            ex->Iex.GetI.bias
         );

      case Iex_Qop: {
         IRQop* qop = ex->Iex.Qop.details;
         vassert(isIRAtom(qop->arg1));
         vassert(isIRAtom(qop->arg2));
         vassert(isIRAtom(qop->arg3));
         vassert(isIRAtom(qop->arg4));
         return IRExpr_Qop(
                   qop->op,
                   subst_Expr(env, qop->arg1),
                   subst_Expr(env, qop->arg2),
                   subst_Expr(env, qop->arg3),
                   subst_Expr(env, qop->arg4)
                );
      }

      case Iex_Triop: {
         IRTriop* triop = ex->Iex.Triop.details;
         vassert(isIRAtom(triop->arg1));
         vassert(isIRAtom(triop->arg2));
         vassert(isIRAtom(triop->arg3));
         return IRExpr_Triop(
                   triop->op,
                   subst_Expr(env, triop->arg1),
                   subst_Expr(env, triop->arg2),
                   subst_Expr(env, triop->arg3)
                );
      }

      case Iex_Binop:
         vassert(isIRAtom(ex->Iex.Binop.arg1));
         vassert(isIRAtom(ex->Iex.Binop.arg2));
         return IRExpr_Binop(
                   ex->Iex.Binop.op,
                   subst_Expr(env, ex->Iex.Binop.arg1),
                   subst_Expr(env, ex->Iex.Binop.arg2)
                );

      case Iex_Unop:
         vassert(isIRAtom(ex->Iex.Unop.arg));
         return IRExpr_Unop(
                   ex->Iex.Unop.op,
                   subst_Expr(env, ex->Iex.Unop.arg)
                );

      case Iex_Load:
         vassert(isIRAtom(ex->Iex.Load.addr));
         return IRExpr_Load(
                   ex->Iex.Load.end,
                   ex->Iex.Load.ty,
                   subst_Expr(env, ex->Iex.Load.addr)
                );

      case Iex_CCall: {
         Int      i;
         IRExpr** args2 = shallowCopyIRExprVec(ex->Iex.CCall.args);
         for (i = 0; args2[i]; i++) {
            vassert(isIRAtom(args2[i]));
            args2[i] = subst_Expr(env, args2[i]);
         }
         return IRExpr_CCall(
                   ex->Iex.CCall.cee,
                   ex->Iex.CCall.retty,
                   args2 
                );
      }

      case Iex_Mux0X:
         vassert(isIRAtom(ex->Iex.Mux0X.cond));
         vassert(isIRAtom(ex->Iex.Mux0X.expr0));
         vassert(isIRAtom(ex->Iex.Mux0X.exprX));
         return IRExpr_Mux0X(
                   subst_Expr(env, ex->Iex.Mux0X.cond),
                   subst_Expr(env, ex->Iex.Mux0X.expr0),
                   subst_Expr(env, ex->Iex.Mux0X.exprX)
                );

      default:
         vex_printf("\n\n"); ppIRExpr(ex);
         vpanic("subst_Expr");
      
   }
}


static IRStmt* subst_and_fold_Stmt ( IRExpr** env, IRStmt* st )
{
#  if 0
   vex_printf("\nsubst and fold stmt\n");
   ppIRStmt(st);
   vex_printf("\n");
#  endif

   switch (st->tag) {
      case Ist_AbiHint:
         vassert(isIRAtom(st->Ist.AbiHint.base));
         vassert(isIRAtom(st->Ist.AbiHint.nia));
         return IRStmt_AbiHint(
                   fold_Expr(env, subst_Expr(env, st->Ist.AbiHint.base)),
                   st->Ist.AbiHint.len,
                   fold_Expr(env, subst_Expr(env, st->Ist.AbiHint.nia))
                );
      case Ist_Put:
         vassert(isIRAtom(st->Ist.Put.data));
         return IRStmt_Put(
                   st->Ist.Put.offset, 
                   fold_Expr(env, subst_Expr(env, st->Ist.Put.data)) 
                );

      case Ist_PutI: {
         IRPutI *puti, *puti2;
         puti = st->Ist.PutI.details;
         vassert(isIRAtom(puti->ix));
         vassert(isIRAtom(puti->data));
         puti2 = mkIRPutI(puti->descr,
                          fold_Expr(env, subst_Expr(env, puti->ix)),
                          puti->bias,
                          fold_Expr(env, subst_Expr(env, puti->data)));
         return IRStmt_PutI(puti2);
      }

      case Ist_WrTmp:
         return IRStmt_WrTmp(
                   st->Ist.WrTmp.tmp,
                   fold_Expr(env, subst_Expr(env, st->Ist.WrTmp.data))
                );

      case Ist_Store:
         vassert(isIRAtom(st->Ist.Store.addr));
         vassert(isIRAtom(st->Ist.Store.data));
         return IRStmt_Store(
                   st->Ist.Store.end,
                   fold_Expr(env, subst_Expr(env, st->Ist.Store.addr)),
                   fold_Expr(env, subst_Expr(env, st->Ist.Store.data))
                );

      case Ist_CAS: {
         IRCAS *cas, *cas2;
         cas = st->Ist.CAS.details;
         vassert(isIRAtom(cas->addr));
         vassert(cas->expdHi == NULL || isIRAtom(cas->expdHi));
         vassert(isIRAtom(cas->expdLo));
         vassert(cas->dataHi == NULL || isIRAtom(cas->dataHi));
         vassert(isIRAtom(cas->dataLo));
         cas2 = mkIRCAS(
                   cas->oldHi, cas->oldLo, cas->end, 
                   fold_Expr(env, subst_Expr(env, cas->addr)),
                   cas->expdHi ? fold_Expr(env, subst_Expr(env, cas->expdHi))
                               : NULL,
                   fold_Expr(env, subst_Expr(env, cas->expdLo)),
                   cas->dataHi ? fold_Expr(env, subst_Expr(env, cas->dataHi))
                               : NULL,
                   fold_Expr(env, subst_Expr(env, cas->dataLo))
                );
         return IRStmt_CAS(cas2);
      }

      case Ist_LLSC:
         vassert(isIRAtom(st->Ist.LLSC.addr));
         if (st->Ist.LLSC.storedata)
            vassert(isIRAtom(st->Ist.LLSC.storedata));
         return IRStmt_LLSC(
                   st->Ist.LLSC.end,
                   st->Ist.LLSC.result,
                   fold_Expr(env, subst_Expr(env, st->Ist.LLSC.addr)),
                   st->Ist.LLSC.storedata
                      ? fold_Expr(env, subst_Expr(env, st->Ist.LLSC.storedata))
                      : NULL
                );

      case Ist_Dirty: {
         Int     i;
         IRDirty *d, *d2;
         d = st->Ist.Dirty.details;
         d2 = emptyIRDirty();
         *d2 = *d;
         d2->args = shallowCopyIRExprVec(d2->args);
         if (d2->mFx != Ifx_None) {
            vassert(isIRAtom(d2->mAddr));
            d2->mAddr = fold_Expr(env, subst_Expr(env, d2->mAddr));
         }
         vassert(isIRAtom(d2->guard));
         d2->guard = fold_Expr(env, subst_Expr(env, d2->guard));
         for (i = 0; d2->args[i]; i++) {
            vassert(isIRAtom(d2->args[i]));
            d2->args[i] = fold_Expr(env, subst_Expr(env, d2->args[i]));
         }
         return IRStmt_Dirty(d2);
      }

      case Ist_IMark:
         return IRStmt_IMark(st->Ist.IMark.addr,
                             st->Ist.IMark.len,
                             st->Ist.IMark.delta);

      case Ist_NoOp:
         return IRStmt_NoOp();

      case Ist_MBE:
         return IRStmt_MBE(st->Ist.MBE.event);

      case Ist_Exit: {
         IRExpr* fcond;
         vassert(isIRAtom(st->Ist.Exit.guard));
         fcond = fold_Expr(env, subst_Expr(env, st->Ist.Exit.guard));
         if (fcond->tag == Iex_Const) {
            vassert(fcond->Iex.Const.con->tag == Ico_U1);
            vassert(fcond->Iex.Const.con->Ico.U1 == False
                    || fcond->Iex.Const.con->Ico.U1 == True);
            if (fcond->Iex.Const.con->Ico.U1 == False) {
               
               return IRStmt_NoOp();
            } else {
               vassert(fcond->Iex.Const.con->Ico.U1 == True);
               
               if (vex_control.iropt_verbosity > 0) 
                  
                  vex_printf("vex iropt: IRStmt_Exit became unconditional\n");
            }
         }
         return IRStmt_Exit(fcond, st->Ist.Exit.jk,
                                   st->Ist.Exit.dst, st->Ist.Exit.offsIP);
      }

   default:
      vex_printf("\n"); ppIRStmt(st);
      vpanic("subst_and_fold_Stmt");
   }
}


IRSB* cprop_BB ( IRSB* in )
{
   Int      i;
   IRSB*    out;
   IRStmt*  st2;
   Int      n_tmps = in->tyenv->types_used;
   IRExpr** env = LibVEX_Alloc(n_tmps * sizeof(IRExpr*));

   out = emptyIRSB();
   out->tyenv = deepCopyIRTypeEnv( in->tyenv );

   for (i = 0; i < n_tmps; i++)
      env[i] = NULL;

   
   for (i = 0; i < in->stmts_used; i++) {


      st2 = in->stmts[i];

      
      if (st2->tag == Ist_NoOp) continue;

      st2 = subst_and_fold_Stmt( env, st2 );

      
      if (st2->tag == Ist_NoOp) continue;

      if (st2->tag == Ist_WrTmp) {
         vassert(env[(Int)(st2->Ist.WrTmp.tmp)] == NULL);
         env[(Int)(st2->Ist.WrTmp.tmp)] = st2->Ist.WrTmp.data;

         
         if (st2->Ist.WrTmp.data->tag == Iex_RdTmp) continue;

         if (st2->Ist.WrTmp.data->tag == Iex_Const
             && st2->Ist.WrTmp.data->Iex.Const.con->tag != Ico_F64i) continue;
      }

      
      addStmtToIRSB( out, st2 );
   }

#if STATS_IROPT
   vex_printf("sameIRExpr: invoked = %u/%u  equal = %u/%u max_nodes = %u\n",
              invocation_count, recursion_count, success_count,
              recursion_success_count, max_nodes_visited);
#endif

   out->next     = subst_Expr( env, in->next );
   out->jumpkind = in->jumpkind;
   out->offsIP   = in->offsIP;
   return out;
}





inline
static void addUses_Temp ( Bool* set, IRTemp tmp )
{
   set[(Int)tmp] = True;
}

static void addUses_Expr ( Bool* set, IRExpr* e )
{
   Int i;
   switch (e->tag) {
      case Iex_GetI:
         addUses_Expr(set, e->Iex.GetI.ix);
         return;
      case Iex_Mux0X:
         addUses_Expr(set, e->Iex.Mux0X.cond);
         addUses_Expr(set, e->Iex.Mux0X.expr0);
         addUses_Expr(set, e->Iex.Mux0X.exprX);
         return;
      case Iex_CCall:
         for (i = 0; e->Iex.CCall.args[i]; i++)
            addUses_Expr(set, e->Iex.CCall.args[i]);
         return;
      case Iex_Load:
         addUses_Expr(set, e->Iex.Load.addr);
         return;
      case Iex_Qop:
         addUses_Expr(set, e->Iex.Qop.details->arg1);
         addUses_Expr(set, e->Iex.Qop.details->arg2);
         addUses_Expr(set, e->Iex.Qop.details->arg3);
         addUses_Expr(set, e->Iex.Qop.details->arg4);
         return;
      case Iex_Triop:
         addUses_Expr(set, e->Iex.Triop.details->arg1);
         addUses_Expr(set, e->Iex.Triop.details->arg2);
         addUses_Expr(set, e->Iex.Triop.details->arg3);
         return;
      case Iex_Binop:
         addUses_Expr(set, e->Iex.Binop.arg1);
         addUses_Expr(set, e->Iex.Binop.arg2);
         return;
      case Iex_Unop:
         addUses_Expr(set, e->Iex.Unop.arg);
         return;
      case Iex_RdTmp:
         addUses_Temp(set, e->Iex.RdTmp.tmp);
         return;
      case Iex_Const:
      case Iex_Get:
         return;
      default:
         vex_printf("\n");
         ppIRExpr(e);
         vpanic("addUses_Expr");
   }
}

static void addUses_Stmt ( Bool* set, IRStmt* st )
{
   Int      i;
   IRDirty* d;
   IRCAS*   cas;
   switch (st->tag) {
      case Ist_AbiHint:
         addUses_Expr(set, st->Ist.AbiHint.base);
         addUses_Expr(set, st->Ist.AbiHint.nia);
         return;
      case Ist_PutI:
         addUses_Expr(set, st->Ist.PutI.details->ix);
         addUses_Expr(set, st->Ist.PutI.details->data);
         return;
      case Ist_WrTmp:
         addUses_Expr(set, st->Ist.WrTmp.data);
         return;
      case Ist_Put:
         addUses_Expr(set, st->Ist.Put.data);
         return;
      case Ist_Store:
         addUses_Expr(set, st->Ist.Store.addr);
         addUses_Expr(set, st->Ist.Store.data);
         return;
      case Ist_CAS:
         cas = st->Ist.CAS.details;
         addUses_Expr(set, cas->addr);
         if (cas->expdHi)
            addUses_Expr(set, cas->expdHi);
         addUses_Expr(set, cas->expdLo);
         if (cas->dataHi)
            addUses_Expr(set, cas->dataHi);
         addUses_Expr(set, cas->dataLo);
         return;
      case Ist_LLSC:
         addUses_Expr(set, st->Ist.LLSC.addr);
         if (st->Ist.LLSC.storedata)
            addUses_Expr(set, st->Ist.LLSC.storedata);
         return;
      case Ist_Dirty:
         d = st->Ist.Dirty.details;
         if (d->mFx != Ifx_None)
            addUses_Expr(set, d->mAddr);
         addUses_Expr(set, d->guard);
         for (i = 0; d->args[i] != NULL; i++)
            addUses_Expr(set, d->args[i]);
         return;
      case Ist_NoOp:
      case Ist_IMark:
      case Ist_MBE:
         return;
      case Ist_Exit:
         addUses_Expr(set, st->Ist.Exit.guard);
         return;
      default:
         vex_printf("\n");
         ppIRStmt(st);
         vpanic("addUses_Stmt");
   }
}


static Bool isZeroU1 ( IRExpr* e )
{
   return toBool( e->tag == Iex_Const
                  && e->Iex.Const.con->tag == Ico_U1
                  && e->Iex.Const.con->Ico.U1 == False );
}

static Bool isOneU1 ( IRExpr* e )
{
   return toBool( e->tag == Iex_Const
                  && e->Iex.Const.con->tag == Ico_U1
                  && e->Iex.Const.con->Ico.U1 == True );
}




 void do_deadcode_BB ( IRSB* bb )
{
   Int     i, i_unconditional_exit;
   Int     n_tmps = bb->tyenv->types_used;
   Bool*   set = LibVEX_Alloc(n_tmps * sizeof(Bool));
   IRStmt* st;

   for (i = 0; i < n_tmps; i++)
      set[i] = False;

   
   addUses_Expr(set, bb->next);

   

   
   i_unconditional_exit = -1;
   for (i = bb->stmts_used-1; i >= 0; i--) {
      st = bb->stmts[i];
      if (st->tag == Ist_NoOp)
         continue;
      
      if (st->tag == Ist_Exit
          && isOneU1(st->Ist.Exit.guard))
         i_unconditional_exit = i;
      if (st->tag == Ist_WrTmp
          && set[(Int)(st->Ist.WrTmp.tmp)] == False) {
          
         if (DEBUG_IROPT) {
            vex_printf("DEAD: ");
            ppIRStmt(st);
            vex_printf("\n");
         }
         bb->stmts[i] = IRStmt_NoOp();
      }
      else
      if (st->tag == Ist_Dirty
          && st->Ist.Dirty.details->guard
          && isZeroU1(st->Ist.Dirty.details->guard)) {
         bb->stmts[i] = IRStmt_NoOp();
       }
       else {
         
         addUses_Stmt(set, st);
      }
   }


   if (i_unconditional_exit != -1) {
      if (0) vex_printf("ZAPPING ALL FORWARDS from %d\n", 
                        i_unconditional_exit);
      vassert(i_unconditional_exit >= 0 
              && i_unconditional_exit < bb->stmts_used);
      bb->next 
         = IRExpr_Const( bb->stmts[i_unconditional_exit]->Ist.Exit.dst );
      bb->jumpkind
         = bb->stmts[i_unconditional_exit]->Ist.Exit.jk;
      bb->offsIP
         = bb->stmts[i_unconditional_exit]->Ist.Exit.offsIP;
      for (i = i_unconditional_exit; i < bb->stmts_used; i++)
         bb->stmts[i] = IRStmt_NoOp();
   }
}



static 
IRSB* spec_helpers_BB(
         IRSB* bb,
         IRExpr* (*specHelper) (HChar*, IRExpr**, IRStmt**, Int)
      )
{
   Int     i;
   IRStmt* st;
   IRExpr* ex;
   Bool    any = False;

   for (i = bb->stmts_used-1; i >= 0; i--) {
      st = bb->stmts[i];

      if (st->tag != Ist_WrTmp
          || st->Ist.WrTmp.data->tag != Iex_CCall)
         continue;

      ex = (*specHelper)( st->Ist.WrTmp.data->Iex.CCall.cee->name,
                          st->Ist.WrTmp.data->Iex.CCall.args,
                          &bb->stmts[0], i );
      if (!ex)
        
        continue;

      
      any = True;
      bb->stmts[i]
         = IRStmt_WrTmp(st->Ist.WrTmp.tmp, ex);

      if (0) {
         vex_printf("SPEC: ");
         ppIRExpr(st->Ist.WrTmp.data);
         vex_printf("  -->  ");
         ppIRExpr(ex);
         vex_printf("\n");
      }
   }

   if (any)
      bb = flatten_BB(bb);
   return bb;
}




typedef
   enum { ExactAlias, NoAlias, UnknownAlias }
   GSAliasing;



static
GSAliasing getAliasingRelation_IC ( IRRegArray* descr1, IRExpr* ix1,
                                    Int offset2, IRType ty2 )
{
   UInt minoff1, maxoff1, minoff2, maxoff2;

   getArrayBounds( descr1, &minoff1, &maxoff1 );
   minoff2 = offset2;
   maxoff2 = minoff2 + sizeofIRType(ty2) - 1;

   if (maxoff1 < minoff2 || maxoff2 < minoff1)
      return NoAlias;

   return UnknownAlias;
}



static
GSAliasing getAliasingRelation_II ( 
              IRRegArray* descr1, IRExpr* ix1, Int bias1,
              IRRegArray* descr2, IRExpr* ix2, Int bias2
           )
{
   UInt minoff1, maxoff1, minoff2, maxoff2;
   Int  iters;

   
   getArrayBounds( descr1, &minoff1, &maxoff1 );
   getArrayBounds( descr2, &minoff2, &maxoff2 );
   if (maxoff1 < minoff2 || maxoff2 < minoff1)
      return NoAlias;

   if (!eqIRRegArray(descr1, descr2))
      return UnknownAlias;

   vassert(isIRAtom(ix1));
   vassert(isIRAtom(ix2));
   if (!eqIRAtom(ix1,ix2))
      return UnknownAlias;


   vassert(descr1->nElems == descr2->nElems);
   vassert(descr1->elemTy == descr2->elemTy);
   vassert(descr1->base   == descr2->base);
   iters = 0;
   while (bias1 < 0 || bias2 < 0) {
      bias1 += descr1->nElems;
      bias2 += descr1->nElems;
      iters++;
      if (iters > 10)
         vpanic("getAliasingRelation: iters");
   }
   vassert(bias1 >= 0 && bias2 >= 0);
   bias1 %= descr1->nElems;
   bias2 %= descr1->nElems;
   vassert(bias1 >= 0 && bias1 < descr1->nElems);
   vassert(bias2 >= 0 && bias2 < descr1->nElems);


   return bias1==bias2 ? ExactAlias : NoAlias;
}





typedef
   struct {
      enum { TCc, TCt } tag;
      union { IRTemp tmp; IRConst* con; } u;
   }
   TmpOrConst;

static Bool eqTmpOrConst ( TmpOrConst* tc1, TmpOrConst* tc2 )
{
   if (tc1->tag != tc2->tag)
      return False;
   switch (tc1->tag) {
      case TCc:
         return eqIRConst(tc1->u.con, tc2->u.con);
      case TCt:
         return tc1->u.tmp == tc2->u.tmp;
      default:
         vpanic("eqTmpOrConst");
   }
}

static Bool eqIRCallee ( IRCallee* cee1, IRCallee* cee2 )
{
   Bool eq = cee1->addr == cee2->addr;
   if (eq) {
      vassert(cee1->regparms == cee2->regparms);
      vassert(cee1->mcx_mask == cee2->mcx_mask);
   }
   return eq;
}

static void irExprVec_to_TmpOrConsts ( TmpOrConst** outs,
                                       Int* nOuts,
                                       IRExpr** ins )
{
   Int i, n;
   
   for (n = 0; ins[n]; n++)
      ;
   *outs  = LibVEX_Alloc(n * sizeof(TmpOrConst));
   *nOuts = n;
   
   for (i = 0; i < n; i++) {
      IRExpr*       arg = ins[i];
      TmpOrConst* dst = &(*outs)[i];
      if (arg->tag == Iex_RdTmp) {
         dst->tag   = TCt;
         dst->u.tmp = arg->Iex.RdTmp.tmp;
      }
      else if (arg->tag == Iex_Const) {
         dst->tag   = TCc;
         dst->u.con = arg->Iex.Const.con;
      }
      else {
         vpanic("irExprVec_to_TmpOrConsts");
      }
   }
}

typedef
   struct {
      enum { Ut, Btt, Btc, Bct, Cf64i, Mttt, GetIt, CCall } tag;
      union {
         
         struct {
            IROp   op;
            IRTemp arg;
         } Ut;
         
         struct {
            IROp   op;
            IRTemp arg1;
            IRTemp arg2;
         } Btt;
         
         struct {
            IROp    op;
            IRTemp  arg1;
            IRConst con2;
         } Btc;
         
         struct {
            IROp    op;
            IRConst con1;
            IRTemp  arg2;
         } Bct;
         
         struct {
            ULong f64i;
         } Cf64i;
         
         struct {
            IRTemp co;
            IRTemp e0;
            IRTemp eX;
         } Mttt;
         
         struct {
            IRRegArray* descr;
            IRTemp      ix;
            Int         bias;
         } GetIt;
         
         struct {
            IRCallee*   cee;
            TmpOrConst* args;
            Int         nArgs;
            IRType      retty;
         } CCall;
      } u;
   }
   AvailExpr;

static Bool eq_AvailExpr ( AvailExpr* a1, AvailExpr* a2 )
{
   if (LIKELY(a1->tag != a2->tag))
      return False;
   switch (a1->tag) {
      case Ut: 
         return toBool(
                a1->u.Ut.op == a2->u.Ut.op 
                && a1->u.Ut.arg == a2->u.Ut.arg);
      case Btt: 
         return toBool(
                a1->u.Btt.op == a2->u.Btt.op
                && a1->u.Btt.arg1 == a2->u.Btt.arg1
                && a1->u.Btt.arg2 == a2->u.Btt.arg2);
      case Btc: 
         return toBool(
                a1->u.Btc.op == a2->u.Btc.op
                && a1->u.Btc.arg1 == a2->u.Btc.arg1
                && eqIRConst(&a1->u.Btc.con2, &a2->u.Btc.con2));
      case Bct: 
         return toBool(
                a1->u.Bct.op == a2->u.Bct.op
                && a1->u.Bct.arg2 == a2->u.Bct.arg2
                && eqIRConst(&a1->u.Bct.con1, &a2->u.Bct.con1));
      case Cf64i: 
         return toBool(a1->u.Cf64i.f64i == a2->u.Cf64i.f64i);
      case Mttt:
         return toBool(a1->u.Mttt.co == a2->u.Mttt.co
                       && a1->u.Mttt.e0 == a2->u.Mttt.e0
                       && a1->u.Mttt.eX == a2->u.Mttt.eX);
      case GetIt:
         return toBool(eqIRRegArray(a1->u.GetIt.descr, a2->u.GetIt.descr) 
                       && a1->u.GetIt.ix == a2->u.GetIt.ix
                       && a1->u.GetIt.bias == a2->u.GetIt.bias);
      case CCall: {
         Int  i, n;
         Bool eq = a1->u.CCall.nArgs == a2->u.CCall.nArgs
                   && eqIRCallee(a1->u.CCall.cee, a2->u.CCall.cee);
         if (eq) {
            n = a1->u.CCall.nArgs;
            for (i = 0; i < n; i++) {
               if (!eqTmpOrConst( &a1->u.CCall.args[i],
                                  &a2->u.CCall.args[i] )) {
                  eq = False;
                  break;
               }
            }
         }
         if (eq) vassert(a1->u.CCall.retty == a2->u.CCall.retty);
         return eq;  
      }
      default: vpanic("eq_AvailExpr");
   }
}

static IRExpr* availExpr_to_IRExpr ( AvailExpr* ae ) 
{
   IRConst* con;
   switch (ae->tag) {
      case Ut:
         return IRExpr_Unop( ae->u.Ut.op, IRExpr_RdTmp(ae->u.Ut.arg) );
      case Btt:
         return IRExpr_Binop( ae->u.Btt.op,
                              IRExpr_RdTmp(ae->u.Btt.arg1),
                              IRExpr_RdTmp(ae->u.Btt.arg2) );
      case Btc:
         con = LibVEX_Alloc(sizeof(IRConst));
         *con = ae->u.Btc.con2;
         return IRExpr_Binop( ae->u.Btc.op,
                              IRExpr_RdTmp(ae->u.Btc.arg1), 
                              IRExpr_Const(con) );
      case Bct:
         con = LibVEX_Alloc(sizeof(IRConst));
         *con = ae->u.Bct.con1;
         return IRExpr_Binop( ae->u.Bct.op,
                              IRExpr_Const(con), 
                              IRExpr_RdTmp(ae->u.Bct.arg2) );
      case Cf64i:
         return IRExpr_Const(IRConst_F64i(ae->u.Cf64i.f64i));
      case Mttt:
         return IRExpr_Mux0X(IRExpr_RdTmp(ae->u.Mttt.co), 
                             IRExpr_RdTmp(ae->u.Mttt.e0), 
                             IRExpr_RdTmp(ae->u.Mttt.eX));
      case GetIt:
         return IRExpr_GetI(ae->u.GetIt.descr,
                            IRExpr_RdTmp(ae->u.GetIt.ix),
                            ae->u.GetIt.bias);
      case CCall: {
         Int i, n = ae->u.CCall.nArgs;
         vassert(n >= 0);
         IRExpr** vec = LibVEX_Alloc((n+1) * sizeof(IRExpr*));
         vec[n] = NULL;
         for (i = 0; i < n; i++) {
            TmpOrConst* tc = &ae->u.CCall.args[i];
            if (tc->tag == TCc) {
               vec[i] = IRExpr_Const(tc->u.con);
            }
            else if (tc->tag == TCt) {
               vec[i] = IRExpr_RdTmp(tc->u.tmp);
            }
            else vpanic("availExpr_to_IRExpr:CCall-arg");
         }
         return IRExpr_CCall(ae->u.CCall.cee,
                             ae->u.CCall.retty,
                             vec);
      }
      default:
         vpanic("availExpr_to_IRExpr");
   }
}

inline
static IRTemp subst_AvailExpr_Temp ( HashHW* env, IRTemp tmp )
{
   HWord res;
   
   if (lookupHHW( env, &res, (HWord)tmp ))
      return (IRTemp)res;
   else
      return tmp;
}

static void subst_AvailExpr ( HashHW* env, AvailExpr* ae )
{
   
   switch (ae->tag) {
      case Ut:
         ae->u.Ut.arg = subst_AvailExpr_Temp( env, ae->u.Ut.arg );
         break;
      case Btt:
         ae->u.Btt.arg1 = subst_AvailExpr_Temp( env, ae->u.Btt.arg1 );
         ae->u.Btt.arg2 = subst_AvailExpr_Temp( env, ae->u.Btt.arg2 );
         break;
      case Btc:
         ae->u.Btc.arg1 = subst_AvailExpr_Temp( env, ae->u.Btc.arg1 );
         break;
      case Bct:
         ae->u.Bct.arg2 = subst_AvailExpr_Temp( env, ae->u.Bct.arg2 );
         break;
      case Cf64i:
         break;
      case Mttt:
         ae->u.Mttt.co = subst_AvailExpr_Temp( env, ae->u.Mttt.co );
         ae->u.Mttt.e0 = subst_AvailExpr_Temp( env, ae->u.Mttt.e0 );
         ae->u.Mttt.eX = subst_AvailExpr_Temp( env, ae->u.Mttt.eX );
         break;
      case GetIt:
         ae->u.GetIt.ix = subst_AvailExpr_Temp( env, ae->u.GetIt.ix );
         break;
      case CCall: {
         Int i, n = ae->u.CCall.nArgs;;
         for (i = 0; i < n; i++) {
            TmpOrConst* tc = &ae->u.CCall.args[i];
            if (tc->tag == TCt) {
               tc->u.tmp = subst_AvailExpr_Temp( env, tc->u.tmp );
            }
         }
         break;
      }
      default: 
         vpanic("subst_AvailExpr");
   }
}

static AvailExpr* irExpr_to_AvailExpr ( IRExpr* e )
{
   AvailExpr* ae;

   if (e->tag == Iex_Unop
       && e->Iex.Unop.arg->tag == Iex_RdTmp) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag      = Ut;
      ae->u.Ut.op  = e->Iex.Unop.op;
      ae->u.Ut.arg = e->Iex.Unop.arg->Iex.RdTmp.tmp;
      return ae;
   }

   if (e->tag == Iex_Binop
       && e->Iex.Binop.arg1->tag == Iex_RdTmp
       && e->Iex.Binop.arg2->tag == Iex_RdTmp) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag        = Btt;
      ae->u.Btt.op   = e->Iex.Binop.op;
      ae->u.Btt.arg1 = e->Iex.Binop.arg1->Iex.RdTmp.tmp;
      ae->u.Btt.arg2 = e->Iex.Binop.arg2->Iex.RdTmp.tmp;
      return ae;
   }

   if (e->tag == Iex_Binop
      && e->Iex.Binop.arg1->tag == Iex_RdTmp
      && e->Iex.Binop.arg2->tag == Iex_Const) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag        = Btc;
      ae->u.Btc.op   = e->Iex.Binop.op;
      ae->u.Btc.arg1 = e->Iex.Binop.arg1->Iex.RdTmp.tmp;
      ae->u.Btc.con2 = *(e->Iex.Binop.arg2->Iex.Const.con);
      return ae;
   }

   if (e->tag == Iex_Binop
      && e->Iex.Binop.arg1->tag == Iex_Const
      && e->Iex.Binop.arg2->tag == Iex_RdTmp) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag        = Bct;
      ae->u.Bct.op   = e->Iex.Binop.op;
      ae->u.Bct.arg2 = e->Iex.Binop.arg2->Iex.RdTmp.tmp;
      ae->u.Bct.con1 = *(e->Iex.Binop.arg1->Iex.Const.con);
      return ae;
   }

   if (e->tag == Iex_Const
       && e->Iex.Const.con->tag == Ico_F64i) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag          = Cf64i;
      ae->u.Cf64i.f64i = e->Iex.Const.con->Ico.F64i;
      return ae;
   }

   if (e->tag == Iex_Mux0X
       && e->Iex.Mux0X.cond->tag == Iex_RdTmp
       && e->Iex.Mux0X.expr0->tag == Iex_RdTmp
       && e->Iex.Mux0X.exprX->tag == Iex_RdTmp) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag       = Mttt;
      ae->u.Mttt.co = e->Iex.Mux0X.cond->Iex.RdTmp.tmp;
      ae->u.Mttt.e0 = e->Iex.Mux0X.expr0->Iex.RdTmp.tmp;
      ae->u.Mttt.eX = e->Iex.Mux0X.exprX->Iex.RdTmp.tmp;
      return ae;
   }

   if (e->tag == Iex_GetI
       && e->Iex.GetI.ix->tag == Iex_RdTmp) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag           = GetIt;
      ae->u.GetIt.descr = e->Iex.GetI.descr;
      ae->u.GetIt.ix    = e->Iex.GetI.ix->Iex.RdTmp.tmp;
      ae->u.GetIt.bias  = e->Iex.GetI.bias;
      return ae;
   }

   if (e->tag == Iex_CCall) {
      ae = LibVEX_Alloc(sizeof(AvailExpr));
      ae->tag = CCall;
      
      ae->u.CCall.cee   = e->Iex.CCall.cee;
      ae->u.CCall.retty = e->Iex.CCall.retty;
      irExprVec_to_TmpOrConsts(
         &ae->u.CCall.args, &ae->u.CCall.nArgs,
         e->Iex.CCall.args
      );
      return ae;
   }

   return NULL;
}



static Bool do_cse_BB ( IRSB* bb )
{
   Int        i, j, paranoia;
   IRTemp     t, q;
   IRStmt*    st;
   AvailExpr* eprime;
   AvailExpr* ae;
   Bool       invalidate;
   Bool       anyDone = False;

   HashHW* tenv = newHHW(); 
   HashHW* aenv = newHHW(); 

   vassert(sizeof(IRTemp) <= sizeof(HWord));

   if (0) { ppIRSB(bb); vex_printf("\n\n"); }

   for (i = 0; i < bb->stmts_used; i++) {
      st = bb->stmts[i];

      
      switch (st->tag) {
         case Ist_Dirty: case Ist_Store: case Ist_MBE:
         case Ist_CAS: case Ist_LLSC:
            paranoia = 2; break;
         case Ist_Put: case Ist_PutI: 
            paranoia = 1; break;
         case Ist_NoOp: case Ist_IMark: case Ist_AbiHint: 
         case Ist_WrTmp: case Ist_Exit: 
            paranoia = 0; break;
         default: 
            vpanic("do_cse_BB(1)");
      }

      if (paranoia > 0) {
         for (j = 0; j < aenv->used; j++) {
            if (!aenv->inuse[j])
               continue;
            ae = (AvailExpr*)aenv->key[j];
            if (ae->tag != GetIt) 
               continue;
            invalidate = False;
            if (paranoia >= 2) {
               invalidate = True;
            } else {
               vassert(paranoia == 1);
               if (st->tag == Ist_Put) {
                  if (getAliasingRelation_IC(
                         ae->u.GetIt.descr, 
                         IRExpr_RdTmp(ae->u.GetIt.ix), 
                         st->Ist.Put.offset, 
                         typeOfIRExpr(bb->tyenv,st->Ist.Put.data) 
                      ) != NoAlias) 
                     invalidate = True;
               }
               else 
               if (st->tag == Ist_PutI) {
                  IRPutI *puti = st->Ist.PutI.details;
                  if (getAliasingRelation_II(
                         ae->u.GetIt.descr, 
                         IRExpr_RdTmp(ae->u.GetIt.ix), 
                         ae->u.GetIt.bias,
                         puti->descr,
                         puti->ix,
                         puti->bias
                      ) != NoAlias)
                     invalidate = True;
               }
               else 
                  vpanic("do_cse_BB(2)");
            }

            if (invalidate) {
               aenv->inuse[j] = False;
               aenv->key[j]   = (HWord)NULL;  
            }
         } 
      } 

      

      
      if (st->tag != Ist_WrTmp)
         continue;

      t = st->Ist.WrTmp.tmp;
      eprime = irExpr_to_AvailExpr(st->Ist.WrTmp.data);
      
      if (!eprime)
         continue;

      

      
      subst_AvailExpr( tenv, eprime );

      
      for (j = 0; j < aenv->used; j++)
         if (aenv->inuse[j] && eq_AvailExpr(eprime, (AvailExpr*)aenv->key[j]))
            break;

      if (j < aenv->used) {
         
         q = (IRTemp)aenv->val[j];
         bb->stmts[i] = IRStmt_WrTmp( t, IRExpr_RdTmp(q) );
         addToHHW( tenv, (HWord)t, (HWord)q );
         anyDone = True;
      } else {
         bb->stmts[i] = IRStmt_WrTmp( t, availExpr_to_IRExpr(eprime) );
         addToHHW( aenv, (HWord)eprime, (HWord)t );
      }
   }

   return anyDone;
}





static Bool isAdd32OrSub32 ( IRExpr* e, IRTemp* tmp, Int* i32 )
{ 
   if (e->tag != Iex_Binop)
      return False;
   if (e->Iex.Binop.op != Iop_Add32 && e->Iex.Binop.op != Iop_Sub32)
      return False;
   if (e->Iex.Binop.arg1->tag != Iex_RdTmp)
      return False;
   if (e->Iex.Binop.arg2->tag != Iex_Const)
      return False;
   *tmp = e->Iex.Binop.arg1->Iex.RdTmp.tmp;
   *i32 = (Int)(e->Iex.Binop.arg2->Iex.Const.con->Ico.U32);
   if (e->Iex.Binop.op == Iop_Sub32)
      *i32 = -*i32;
   return True;
}



static Bool collapseChain ( IRSB* bb, Int startHere,
                            IRTemp tmp,
                            IRTemp* tmp2, Int* i32 )
{
   Int     j, ii;
   IRTemp  vv;
   IRStmt* st;
   IRExpr* e;

   IRTemp var = tmp;
   Int    con = 0;

   for (j = startHere; j >= 0; j--) {
      st = bb->stmts[j];
      if (st->tag != Ist_WrTmp) 
         continue;
      if (st->Ist.WrTmp.tmp != var)
         continue;
      e = st->Ist.WrTmp.data;
      if (!isAdd32OrSub32(e, &vv, &ii))
         break;
      var = vv;
      con += ii;
   }
   if (j == -1)
      
      vpanic("collapseChain");

   
   if (var == tmp)
      return False; 
      
   *tmp2 = var;
   *i32  = con;
   return True;
}



static void collapse_AddSub_chains_BB ( IRSB* bb )
{
   IRStmt *st;
   IRTemp var, var2;
   Int    i, con, con2;

   for (i = bb->stmts_used-1; i >= 0; i--) {
      st = bb->stmts[i];
      if (st->tag == Ist_NoOp)
         continue;

      

      if (st->tag == Ist_WrTmp
          && isAdd32OrSub32(st->Ist.WrTmp.data, &var, &con)) {

         if (collapseChain(bb, i-1, var, &var2, &con2)) {
            if (DEBUG_IROPT) {
               vex_printf("replacing1 ");
               ppIRStmt(st);
               vex_printf(" with ");
            }
            con2 += con;
            bb->stmts[i] 
               = IRStmt_WrTmp(
                    st->Ist.WrTmp.tmp,
                    (con2 >= 0) 
                      ? IRExpr_Binop(Iop_Add32, 
                                     IRExpr_RdTmp(var2),
                                     IRExpr_Const(IRConst_U32(con2)))
                      : IRExpr_Binop(Iop_Sub32, 
                                     IRExpr_RdTmp(var2),
                                     IRExpr_Const(IRConst_U32(-con2)))
                 );
            if (DEBUG_IROPT) {
               ppIRStmt(bb->stmts[i]);
               vex_printf("\n");
            }
         }

         continue;
      }

      

      if (st->tag == Ist_WrTmp
          && st->Ist.WrTmp.data->tag == Iex_GetI
          && st->Ist.WrTmp.data->Iex.GetI.ix->tag == Iex_RdTmp
          && collapseChain(bb, i-1, st->Ist.WrTmp.data->Iex.GetI.ix
                                      ->Iex.RdTmp.tmp, &var2, &con2)) {
         if (DEBUG_IROPT) {
            vex_printf("replacing3 ");
            ppIRStmt(st);
            vex_printf(" with ");
         }
         con2 += st->Ist.WrTmp.data->Iex.GetI.bias;
         bb->stmts[i]
            = IRStmt_WrTmp(
                 st->Ist.WrTmp.tmp,
                 IRExpr_GetI(st->Ist.WrTmp.data->Iex.GetI.descr,
                             IRExpr_RdTmp(var2),
                             con2));
         if (DEBUG_IROPT) {
            ppIRStmt(bb->stmts[i]);
            vex_printf("\n");
         }
         continue;
      }

      
      IRPutI *puti = st->Ist.PutI.details;
      if (st->tag == Ist_PutI
          && puti->ix->tag == Iex_RdTmp
          && collapseChain(bb, i-1, puti->ix->Iex.RdTmp.tmp, 
                               &var2, &con2)) {
         if (DEBUG_IROPT) {
            vex_printf("replacing2 ");
            ppIRStmt(st);
            vex_printf(" with ");
         }
         con2 += puti->bias;
         bb->stmts[i]
            = IRStmt_PutI(mkIRPutI(puti->descr,
                                   IRExpr_RdTmp(var2),
                                   con2,
                                   puti->data));
         if (DEBUG_IROPT) {
            ppIRStmt(bb->stmts[i]);
            vex_printf("\n");
         }
         continue;
      }

   } 
}




static 
IRExpr* findPutI ( IRSB* bb, Int startHere,
                   IRRegArray* descrG, IRExpr* ixG, Int biasG )
{
   Int        j;
   IRStmt*    st;
   GSAliasing relation;

   if (0) {
      vex_printf("\nfindPutI ");
      ppIRRegArray(descrG);
      vex_printf(" ");
      ppIRExpr(ixG);
      vex_printf(" %d\n", biasG);
   }


   for (j = startHere; j >= 0; j--) {
      st = bb->stmts[j];
      if (st->tag == Ist_NoOp) 
         continue;

      if (st->tag == Ist_Put) {

         relation
            = getAliasingRelation_IC(
                 descrG, ixG,
                 st->Ist.Put.offset,
                 typeOfIRExpr(bb->tyenv,st->Ist.Put.data) );

         if (relation == NoAlias) {
            
            continue;
         } else {
            
            vassert(relation != ExactAlias);
            return NULL;
         }
      }

      if (st->tag == Ist_PutI) {
         IRPutI *puti = st->Ist.PutI.details;

         relation = getAliasingRelation_II(
                       descrG, ixG, biasG,
                       puti->descr,
                       puti->ix,
                       puti->bias
                    );

         if (relation == NoAlias) {
            continue; 
         }

         if (relation == UnknownAlias) {
            return NULL;
         }

         
         vassert(relation == ExactAlias);
         return puti->data;

      } 

      if (st->tag == Ist_Dirty) {
         if (st->Ist.Dirty.details->nFxState > 0)
            return NULL;
      }

   } 

   
   return NULL;
}




static Bool identicalPutIs ( IRStmt* pi, IRStmt* s2 )
{
   vassert(pi->tag == Ist_PutI);
   if (s2->tag != Ist_PutI)
      return False;

   IRPutI *p1 = pi->Ist.PutI.details;
   IRPutI *p2 = s2->Ist.PutI.details;

   return toBool(
          getAliasingRelation_II( 
             p1->descr, p1->ix, p1->bias, 
             p2->descr, p2->ix, p2->bias
          )
          == ExactAlias
          );
}



static 
Bool guestAccessWhichMightOverlapPutI ( 
        IRTypeEnv* tyenv, IRStmt* pi, IRStmt* s2 
     )
{
   GSAliasing relation;
   UInt       minoffP, maxoffP;

   vassert(pi->tag == Ist_PutI);

   IRPutI *p1 = pi->Ist.PutI.details;

   getArrayBounds(p1->descr, &minoffP, &maxoffP);
   switch (s2->tag) {

      case Ist_NoOp:
      case Ist_IMark:
         return False;

      case Ist_MBE:
      case Ist_AbiHint:
         
         return True;

      case Ist_CAS:
         return True;

      case Ist_Dirty:
         if (s2->Ist.Dirty.details->nFxState > 0)
            return True;
         return False;

      case Ist_Put:
         vassert(isIRAtom(s2->Ist.Put.data));
         relation 
            = getAliasingRelation_IC(
                 p1->descr, p1->ix,
                 s2->Ist.Put.offset, 
                 typeOfIRExpr(tyenv,s2->Ist.Put.data)
              );
         goto have_relation;

      case Ist_PutI: {
         IRPutI *p2 = s2->Ist.PutI.details;

         vassert(isIRAtom(p2->ix));
         vassert(isIRAtom(p2->data));
         relation
            = getAliasingRelation_II(
                 p1->descr, p1->ix, p1->bias, 
                 p2->descr, p2->ix, p2->bias
              );
         goto have_relation;
      }

      case Ist_WrTmp:
         if (s2->Ist.WrTmp.data->tag == Iex_GetI) {
            relation
               = getAliasingRelation_II(
                    p1->descr, p1->ix, p1->bias, 
                    s2->Ist.WrTmp.data->Iex.GetI.descr,
                    s2->Ist.WrTmp.data->Iex.GetI.ix,
                    s2->Ist.WrTmp.data->Iex.GetI.bias
                 );
            goto have_relation;
         }
         if (s2->Ist.WrTmp.data->tag == Iex_Get) {
            relation
               = getAliasingRelation_IC(
                    p1->descr, p1->ix,
                    s2->Ist.WrTmp.data->Iex.Get.offset,
                    s2->Ist.WrTmp.data->Iex.Get.ty
                 );
            goto have_relation;
         }
         return False;

      case Ist_Store:
         vassert(isIRAtom(s2->Ist.Store.addr));
         vassert(isIRAtom(s2->Ist.Store.data));
         return False;

      default:
         vex_printf("\n"); ppIRStmt(s2); vex_printf("\n");
         vpanic("guestAccessWhichMightOverlapPutI");
   }

  have_relation:
   if (relation == NoAlias)
      return False;
   else
      return True; 
}





static
void do_redundant_GetI_elimination ( IRSB* bb )
{
   Int     i;
   IRStmt* st;

   for (i = bb->stmts_used-1; i >= 0; i--) {
      st = bb->stmts[i];
      if (st->tag == Ist_NoOp)
         continue;

      if (st->tag == Ist_WrTmp
          && st->Ist.WrTmp.data->tag == Iex_GetI
          && st->Ist.WrTmp.data->Iex.GetI.ix->tag == Iex_RdTmp) {
         IRRegArray* descr = st->Ist.WrTmp.data->Iex.GetI.descr;
         IRExpr*     ix    = st->Ist.WrTmp.data->Iex.GetI.ix;
         Int         bias  = st->Ist.WrTmp.data->Iex.GetI.bias;
         IRExpr*     replacement = findPutI(bb, i-1, descr, ix, bias);
         if (replacement 
             && isIRAtom(replacement)
             
             && typeOfIRExpr(bb->tyenv, replacement) == descr->elemTy) {
            if (DEBUG_IROPT) {
               vex_printf("rGI:  "); 
               ppIRExpr(st->Ist.WrTmp.data);
               vex_printf(" -> ");
               ppIRExpr(replacement);
               vex_printf("\n");
            }
            bb->stmts[i] = IRStmt_WrTmp(st->Ist.WrTmp.tmp, replacement);
         }
      }
   }

}



static
void do_redundant_PutI_elimination ( IRSB* bb )
{
   Int    i, j;
   Bool   delete;
   IRStmt *st, *stj;

   vassert
      (vex_control.iropt_register_updates == VexRegUpdUnwindregsAtMemAccess
       || vex_control.iropt_register_updates == VexRegUpdAllregsAtMemAccess);

   for (i = 0; i < bb->stmts_used; i++) {
      st = bb->stmts[i];
      if (st->tag != Ist_PutI)
         continue;
      delete = False;
      for (j = i+1; j < bb->stmts_used; j++) {
         stj = bb->stmts[j];
         if (stj->tag == Ist_NoOp) 
            continue;
         if (identicalPutIs(st, stj)) {
            
            delete = True;
            break;
         }
         if (stj->tag == Ist_Exit)
            
            break;
         if (st->tag == Ist_Dirty)
            
            break;
         if (guestAccessWhichMightOverlapPutI(bb->tyenv, st, stj))
            
           break;
      }

      if (delete) {
         if (DEBUG_IROPT) {
            vex_printf("rPI:  "); 
            ppIRStmt(st); 
            vex_printf("\n");
         }
         bb->stmts[i] = IRStmt_NoOp();
      }

   }
}




static void deltaIRExpr ( IRExpr* e, Int delta )
{
   Int i;
   switch (e->tag) {
      case Iex_RdTmp:
         e->Iex.RdTmp.tmp += delta;
         break;
      case Iex_Get:
      case Iex_Const:
         break;
      case Iex_GetI:
         deltaIRExpr(e->Iex.GetI.ix, delta);
         break;
      case Iex_Qop:
         deltaIRExpr(e->Iex.Qop.details->arg1, delta);
         deltaIRExpr(e->Iex.Qop.details->arg2, delta);
         deltaIRExpr(e->Iex.Qop.details->arg3, delta);
         deltaIRExpr(e->Iex.Qop.details->arg4, delta);
         break;
      case Iex_Triop:
         deltaIRExpr(e->Iex.Triop.details->arg1, delta);
         deltaIRExpr(e->Iex.Triop.details->arg2, delta);
         deltaIRExpr(e->Iex.Triop.details->arg3, delta);
         break;
      case Iex_Binop:
         deltaIRExpr(e->Iex.Binop.arg1, delta);
         deltaIRExpr(e->Iex.Binop.arg2, delta);
         break;
      case Iex_Unop:
         deltaIRExpr(e->Iex.Unop.arg, delta);
         break;
      case Iex_Load:
         deltaIRExpr(e->Iex.Load.addr, delta);
         break;
      case Iex_CCall:
         for (i = 0; e->Iex.CCall.args[i]; i++)
            deltaIRExpr(e->Iex.CCall.args[i], delta);
         break;
      case Iex_Mux0X:
         deltaIRExpr(e->Iex.Mux0X.cond, delta);
         deltaIRExpr(e->Iex.Mux0X.expr0, delta);
         deltaIRExpr(e->Iex.Mux0X.exprX, delta);
         break;
      default: 
         vex_printf("\n"); ppIRExpr(e); vex_printf("\n");
         vpanic("deltaIRExpr");
   }
}


static void deltaIRStmt ( IRStmt* st, Int delta )
{
   Int      i;
   IRDirty* d;
   switch (st->tag) {
      case Ist_NoOp:
      case Ist_IMark:
      case Ist_MBE:
         break;
      case Ist_AbiHint:
         deltaIRExpr(st->Ist.AbiHint.base, delta);
         deltaIRExpr(st->Ist.AbiHint.nia, delta);
         break;
      case Ist_Put:
         deltaIRExpr(st->Ist.Put.data, delta);
         break;
      case Ist_PutI:
         deltaIRExpr(st->Ist.PutI.details->ix, delta);
         deltaIRExpr(st->Ist.PutI.details->data, delta);
         break;
      case Ist_WrTmp: 
         st->Ist.WrTmp.tmp += delta;
         deltaIRExpr(st->Ist.WrTmp.data, delta);
         break;
      case Ist_Exit:
         deltaIRExpr(st->Ist.Exit.guard, delta);
         break;
      case Ist_Store:
         deltaIRExpr(st->Ist.Store.addr, delta);
         deltaIRExpr(st->Ist.Store.data, delta);
         break;
      case Ist_CAS:
         if (st->Ist.CAS.details->oldHi != IRTemp_INVALID)
            st->Ist.CAS.details->oldHi += delta;
         st->Ist.CAS.details->oldLo += delta;
         deltaIRExpr(st->Ist.CAS.details->addr, delta);
         if (st->Ist.CAS.details->expdHi)
            deltaIRExpr(st->Ist.CAS.details->expdHi, delta);
         deltaIRExpr(st->Ist.CAS.details->expdLo, delta);
         if (st->Ist.CAS.details->dataHi)
            deltaIRExpr(st->Ist.CAS.details->dataHi, delta);
         deltaIRExpr(st->Ist.CAS.details->dataLo, delta);
         break;
      case Ist_LLSC:
         st->Ist.LLSC.result += delta;
         deltaIRExpr(st->Ist.LLSC.addr, delta);
         if (st->Ist.LLSC.storedata)
            deltaIRExpr(st->Ist.LLSC.storedata, delta);
         break;
      case Ist_Dirty:
         d = st->Ist.Dirty.details;
         deltaIRExpr(d->guard, delta);
         for (i = 0; d->args[i]; i++)
            deltaIRExpr(d->args[i], delta);
         if (d->tmp != IRTemp_INVALID)
            d->tmp += delta;
         if (d->mAddr)
            deltaIRExpr(d->mAddr, delta);
         break;
      default: 
         vex_printf("\n"); ppIRStmt(st); vex_printf("\n");
         vpanic("deltaIRStmt");
   }
}




static Int calc_unroll_factor( IRSB* bb )
{
   Int n_stmts, i;

   n_stmts = 0;
   for (i = 0; i < bb->stmts_used; i++) {
      if (bb->stmts[i]->tag != Ist_NoOp)
         n_stmts++;
   }

   if (n_stmts <= vex_control.iropt_unroll_thresh/8) {
      if (vex_control.iropt_verbosity > 0)
         vex_printf("vex iropt: 8 x unrolling (%d sts -> %d sts)\n",
                    n_stmts, 8* n_stmts);
      return 8;
   }
   if (n_stmts <= vex_control.iropt_unroll_thresh/4) {
      if (vex_control.iropt_verbosity > 0)
         vex_printf("vex iropt: 4 x unrolling (%d sts -> %d sts)\n",
                    n_stmts, 4* n_stmts);
      return 4;
   }

   if (n_stmts <= vex_control.iropt_unroll_thresh/2) {
      if (vex_control.iropt_verbosity > 0)
         vex_printf("vex iropt: 2 x unrolling (%d sts -> %d sts)\n",
                    n_stmts, 2* n_stmts);
      return 2;
   }

   if (vex_control.iropt_verbosity > 0)
      vex_printf("vex iropt: not unrolling (%d sts)\n", n_stmts);

   return 1;
}


static IRSB* maybe_loop_unroll_BB ( IRSB* bb0, Addr64 my_addr )
{
   Int      i, j, jmax, n_vars;
   Bool     xxx_known;
   Addr64   xxx_value, yyy_value;
   IRExpr*  udst;
   IRStmt*  st;
   IRConst* con;
   IRSB     *bb1, *bb2;
   Int      unroll_factor;

   if (vex_control.iropt_unroll_thresh <= 0)
      return NULL;


   if (bb0->jumpkind != Ijk_Boring)
      return NULL;

   xxx_known = False;
   xxx_value = 0;


   udst = bb0->next;
   if (udst->tag == Iex_Const
       && (udst->Iex.Const.con->tag == Ico_U32
           || udst->Iex.Const.con->tag == Ico_U64)) {
      
      xxx_known = True;
      xxx_value = udst->Iex.Const.con->tag == Ico_U64
                    ?  udst->Iex.Const.con->Ico.U64
                    : (Addr64)(udst->Iex.Const.con->Ico.U32);
   }

   if (!xxx_known)
      return NULL;

   if (xxx_value == my_addr) {
      unroll_factor = calc_unroll_factor( bb0 );
      if (unroll_factor < 2)
         return NULL;
      bb1 = deepCopyIRSB( bb0 );
      bb0 = NULL;
      udst = NULL; 
      goto do_unroll;
   }

   yyy_value = xxx_value;
   for (i = bb0->stmts_used-1; i >= 0; i--)
      if (bb0->stmts[i])
         break;

   if (i < 0)
      return NULL; 

   st = bb0->stmts[i];
   if (st->tag != Ist_Exit)
      return NULL;
   if (st->Ist.Exit.jk != Ijk_Boring)
      return NULL;

   con = st->Ist.Exit.dst;
   vassert(con->tag == Ico_U32 || con->tag == Ico_U64);

   xxx_value = con->tag == Ico_U64 
                  ? st->Ist.Exit.dst->Ico.U64
                  : (Addr64)(st->Ist.Exit.dst->Ico.U32);

   
   vassert(con->tag == udst->Iex.Const.con->tag);

   if (xxx_value != my_addr)
      
      return NULL;


   unroll_factor = calc_unroll_factor( bb0 );
   if (unroll_factor < 2)
      return NULL;

   bb1 = deepCopyIRSB( bb0 );
   bb0 = NULL;
   udst = NULL; 
   for (i = bb1->stmts_used-1; i >= 0; i--)
      if (bb1->stmts[i])
         break;


   vassert(i >= 0);

   st = bb1->stmts[i];
   vassert(st->tag == Ist_Exit);

   con = st->Ist.Exit.dst;
   vassert(con->tag == Ico_U32 || con->tag == Ico_U64);

   udst = bb1->next;
   vassert(udst->tag == Iex_Const);
   vassert(udst->Iex.Const.con->tag == Ico_U32
          || udst->Iex.Const.con->tag == Ico_U64);
   vassert(con->tag == udst->Iex.Const.con->tag);

   
   if (con->tag == Ico_U64) {
      udst->Iex.Const.con->Ico.U64 = xxx_value;
      con->Ico.U64 = yyy_value;
   } else {
      udst->Iex.Const.con->Ico.U32 = (UInt)xxx_value;
      con->Ico.U32 = (UInt)yyy_value;
   }

   
   st->Ist.Exit.guard 
      = IRExpr_Unop(Iop_Not1,deepCopyIRExpr(st->Ist.Exit.guard));

   
   

  do_unroll:

   vassert(unroll_factor == 2 
           || unroll_factor == 4
           || unroll_factor == 8);

   jmax = unroll_factor==8 ? 3 : (unroll_factor==4 ? 2 : 1);
   for (j = 1; j <= jmax; j++) {

      n_vars = bb1->tyenv->types_used;

      bb2 = deepCopyIRSB(bb1);
      for (i = 0; i < n_vars; i++)
         (void)newIRTemp(bb1->tyenv, bb2->tyenv->types[i]);

      for (i = 0; i < bb2->stmts_used; i++) {
         deltaIRStmt(bb2->stmts[i], n_vars);
         addStmtToIRSB(bb1, bb2->stmts[i]);
      }
   }

   if (DEBUG_IROPT) {
      vex_printf("\nUNROLLED (%llx)\n", my_addr);
      ppIRSB(bb1);
      vex_printf("\n");
   }

   return flatten_BB(bb1);
}





#define A_NENV 10

typedef
   struct {
      IRTemp  binder;
      IRExpr* bindee;
      Bool    doesLoad;
      Bool    doesGet;
   }
   ATmpInfo;

__attribute__((unused))
static void ppAEnv ( ATmpInfo* env )
{
   Int i;
   for (i = 0; i < A_NENV; i++) {
      vex_printf("%d  tmp %d  val ", i, (Int)env[i].binder);
      if (env[i].bindee) 
         ppIRExpr(env[i].bindee);
      else 
         vex_printf("(null)");
      vex_printf("\n");
   }
}


static void setHints_Expr (Bool* doesLoad, Bool* doesGet, IRExpr* e )
{
   Int i;
   switch (e->tag) {
      case Iex_CCall:
         for (i = 0; e->Iex.CCall.args[i]; i++)
            setHints_Expr(doesLoad, doesGet, e->Iex.CCall.args[i]);
         return;
      case Iex_Mux0X:
         setHints_Expr(doesLoad, doesGet, e->Iex.Mux0X.cond);
         setHints_Expr(doesLoad, doesGet, e->Iex.Mux0X.expr0);
         setHints_Expr(doesLoad, doesGet, e->Iex.Mux0X.exprX);
         return;
      case Iex_Qop:
         setHints_Expr(doesLoad, doesGet, e->Iex.Qop.details->arg1);
         setHints_Expr(doesLoad, doesGet, e->Iex.Qop.details->arg2);
         setHints_Expr(doesLoad, doesGet, e->Iex.Qop.details->arg3);
         setHints_Expr(doesLoad, doesGet, e->Iex.Qop.details->arg4);
         return;
      case Iex_Triop:
         setHints_Expr(doesLoad, doesGet, e->Iex.Triop.details->arg1);
         setHints_Expr(doesLoad, doesGet, e->Iex.Triop.details->arg2);
         setHints_Expr(doesLoad, doesGet, e->Iex.Triop.details->arg3);
         return;
      case Iex_Binop:
         setHints_Expr(doesLoad, doesGet, e->Iex.Binop.arg1);
         setHints_Expr(doesLoad, doesGet, e->Iex.Binop.arg2);
         return;
      case Iex_Unop:
         setHints_Expr(doesLoad, doesGet, e->Iex.Unop.arg);
         return;
      case Iex_Load:
         *doesLoad = True;
         setHints_Expr(doesLoad, doesGet, e->Iex.Load.addr);
         return;
      case Iex_Get:
         *doesGet = True;
         return;
      case Iex_GetI:
         *doesGet = True;
         setHints_Expr(doesLoad, doesGet, e->Iex.GetI.ix);
         return;
      case Iex_RdTmp:
      case Iex_Const:
         return;
      default: 
         vex_printf("\n"); ppIRExpr(e); vex_printf("\n");
         vpanic("setHints_Expr");
   }
}


static void addToEnvFront ( ATmpInfo* env, IRTemp binder, IRExpr* bindee )
{
   Int i;
   vassert(env[A_NENV-1].bindee == NULL);
   for (i = A_NENV-1; i >= 1; i--)
      env[i] = env[i-1];
   env[0].binder   = binder;
   env[0].bindee   = bindee;
   env[0].doesLoad = False; 
   env[0].doesGet  = False; 
}

static void aoccCount_Expr ( UShort* uses, IRExpr* e )
{
   Int i;

   switch (e->tag) {

      case Iex_RdTmp: 
         uses[e->Iex.RdTmp.tmp]++;
         return;

      case Iex_Mux0X:
         aoccCount_Expr(uses, e->Iex.Mux0X.cond);
         aoccCount_Expr(uses, e->Iex.Mux0X.expr0);
         aoccCount_Expr(uses, e->Iex.Mux0X.exprX);
         return;

      case Iex_Qop: 
         aoccCount_Expr(uses, e->Iex.Qop.details->arg1);
         aoccCount_Expr(uses, e->Iex.Qop.details->arg2);
         aoccCount_Expr(uses, e->Iex.Qop.details->arg3);
         aoccCount_Expr(uses, e->Iex.Qop.details->arg4);
         return;

      case Iex_Triop: 
         aoccCount_Expr(uses, e->Iex.Triop.details->arg1);
         aoccCount_Expr(uses, e->Iex.Triop.details->arg2);
         aoccCount_Expr(uses, e->Iex.Triop.details->arg3);
         return;

      case Iex_Binop: 
         aoccCount_Expr(uses, e->Iex.Binop.arg1);
         aoccCount_Expr(uses, e->Iex.Binop.arg2);
         return;

      case Iex_Unop: 
         aoccCount_Expr(uses, e->Iex.Unop.arg);
         return;

      case Iex_Load:
         aoccCount_Expr(uses, e->Iex.Load.addr);
         return;

      case Iex_CCall:
         for (i = 0; e->Iex.CCall.args[i]; i++)
            aoccCount_Expr(uses, e->Iex.CCall.args[i]);
         return;

      case Iex_GetI:
         aoccCount_Expr(uses, e->Iex.GetI.ix);
         return;

      case Iex_Const:
      case Iex_Get:
         return;

      default: 
         vex_printf("\n"); ppIRExpr(e); vex_printf("\n");
         vpanic("aoccCount_Expr");
    }
}


static void aoccCount_Stmt ( UShort* uses, IRStmt* st )
{
   Int      i;
   IRDirty* d;
   IRCAS*   cas;
   switch (st->tag) {
      case Ist_AbiHint:
         aoccCount_Expr(uses, st->Ist.AbiHint.base);
         aoccCount_Expr(uses, st->Ist.AbiHint.nia);
         return;
      case Ist_WrTmp: 
         aoccCount_Expr(uses, st->Ist.WrTmp.data); 
         return; 
      case Ist_Put: 
         aoccCount_Expr(uses, st->Ist.Put.data);
         return;
      case Ist_PutI:
         aoccCount_Expr(uses, st->Ist.PutI.details->ix);
         aoccCount_Expr(uses, st->Ist.PutI.details->data);
         return;
      case Ist_Store:
         aoccCount_Expr(uses, st->Ist.Store.addr);
         aoccCount_Expr(uses, st->Ist.Store.data);
         return;
      case Ist_CAS:
         cas = st->Ist.CAS.details;
         aoccCount_Expr(uses, cas->addr);
         if (cas->expdHi)
            aoccCount_Expr(uses, cas->expdHi);
         aoccCount_Expr(uses, cas->expdLo);
         if (cas->dataHi)
            aoccCount_Expr(uses, cas->dataHi);
         aoccCount_Expr(uses, cas->dataLo);
         return;
      case Ist_LLSC:
         aoccCount_Expr(uses, st->Ist.LLSC.addr);
         if (st->Ist.LLSC.storedata)
            aoccCount_Expr(uses, st->Ist.LLSC.storedata);
         return;
      case Ist_Dirty:
         d = st->Ist.Dirty.details;
         if (d->mFx != Ifx_None)
            aoccCount_Expr(uses, d->mAddr);
         aoccCount_Expr(uses, d->guard);
         for (i = 0; d->args[i]; i++)
            aoccCount_Expr(uses, d->args[i]);
         return;
      case Ist_NoOp:
      case Ist_IMark:
      case Ist_MBE:
         return;
      case Ist_Exit:
         aoccCount_Expr(uses, st->Ist.Exit.guard);
         return;
      default: 
         vex_printf("\n"); ppIRStmt(st); vex_printf("\n");
         vpanic("aoccCount_Stmt");
   }
}


static IRExpr* atbSubst_Temp ( ATmpInfo* env, IRTemp tmp )
{
   Int i;
   for (i = 0; i < A_NENV; i++) {
      if (env[i].binder == tmp && env[i].bindee != NULL) {
         IRExpr* bindee = env[i].bindee;
         env[i].bindee = NULL;
         return bindee;
      }
   }
   return NULL;
}


static inline Bool is_Unop ( IRExpr* e, IROp op ) {
   return e->tag == Iex_Unop && e->Iex.Unop.op == op;
}
static inline Bool is_Binop ( IRExpr* e, IROp op ) {
   return e->tag == Iex_Binop && e->Iex.Binop.op == op;
}

static IRExpr* fold_IRExpr_Binop ( IROp op, IRExpr* a1, IRExpr* a2 )
{
   switch (op) {
   case Iop_Or32:
      
      if (is_Unop(a1, Iop_CmpwNEZ32) && is_Unop(a2, Iop_CmpwNEZ32))
         return IRExpr_Unop( Iop_CmpwNEZ32,
                             IRExpr_Binop( Iop_Or32, a1->Iex.Unop.arg, 
                                                     a2->Iex.Unop.arg ) );
      break;

   case Iop_CmpNE32:
      if (is_Unop(a1, Iop_1Uto32) && isZeroU32(a2))
         return a1->Iex.Unop.arg;
      break;

   default:
      break;
   }
   
   return IRExpr_Binop( op, a1, a2 );
}

static IRExpr* fold_IRExpr_Unop ( IROp op, IRExpr* aa )
{
   switch (op) {
   case Iop_CmpwNEZ64:
      
      if (is_Binop(aa, Iop_Or64) 
          && is_Unop(aa->Iex.Binop.arg1, Iop_CmpwNEZ64))
         return fold_IRExpr_Unop(
                   Iop_CmpwNEZ64,
                   IRExpr_Binop(Iop_Or64, 
                                aa->Iex.Binop.arg1->Iex.Unop.arg, 
                                aa->Iex.Binop.arg2));
      
      if (is_Binop(aa, Iop_Or64)
          && is_Unop(aa->Iex.Binop.arg2, Iop_CmpwNEZ64))
         return fold_IRExpr_Unop(
                   Iop_CmpwNEZ64,
                   IRExpr_Binop(Iop_Or64, 
                                aa->Iex.Binop.arg1, 
                                aa->Iex.Binop.arg2->Iex.Unop.arg));
      break;
   case Iop_CmpNEZ64:
      
      if (is_Unop(aa, Iop_Left64)) 
         return IRExpr_Unop(Iop_CmpNEZ64, aa->Iex.Unop.arg);
      break;
   case Iop_CmpwNEZ32:
      
      if (is_Unop(aa, Iop_CmpwNEZ32))
         return IRExpr_Unop( Iop_CmpwNEZ32, aa->Iex.Unop.arg );
      break;
   case Iop_CmpNEZ32:
      
      if (is_Unop(aa, Iop_Left32)) 
         return IRExpr_Unop(Iop_CmpNEZ32, aa->Iex.Unop.arg);
      
      if (is_Unop(aa, Iop_1Uto32))
         return aa->Iex.Unop.arg;
      break;
   case Iop_CmpNEZ8:
      
      if (is_Unop(aa, Iop_1Uto8))
         return aa->Iex.Unop.arg;
      break;
   case Iop_Left32:
      
      if (is_Unop(aa, Iop_Left32))
         return IRExpr_Unop( Iop_Left32, aa->Iex.Unop.arg );
      break;
   case Iop_32to1:
      
      if (is_Unop(aa, Iop_1Uto32))
         return aa->Iex.Unop.arg;
      
      if (is_Unop(aa, Iop_CmpwNEZ32))
         return IRExpr_Unop( Iop_CmpNEZ32, aa->Iex.Unop.arg );
      break;
   case Iop_64to1:
      
      if (is_Unop(aa, Iop_1Uto64))
         return aa->Iex.Unop.arg;
      
      if (is_Unop(aa, Iop_CmpwNEZ64))
         return IRExpr_Unop( Iop_CmpNEZ64, aa->Iex.Unop.arg );
      break;
   case Iop_64to32:
      
      if (is_Unop(aa, Iop_32Uto64))
         return aa->Iex.Unop.arg;
      
      if (is_Unop(aa, Iop_8Uto64))
         return IRExpr_Unop(Iop_8Uto32, aa->Iex.Unop.arg);
      break;

   case Iop_32Uto64:
      
      if (is_Unop(aa, Iop_8Uto32))
         return IRExpr_Unop(Iop_8Uto64, aa->Iex.Unop.arg);
      
      if (is_Unop(aa, Iop_16Uto32))
         return IRExpr_Unop(Iop_16Uto64, aa->Iex.Unop.arg);
      if (is_Unop(aa, Iop_64to32)
          && is_Binop(aa->Iex.Unop.arg, Iop_Shr64)
          && is_Unop(aa->Iex.Unop.arg->Iex.Binop.arg1, Iop_32Uto64)
          && is_Unop(aa->Iex.Unop.arg->Iex.Binop.arg1->Iex.Unop.arg,
                     Iop_64to32)) {
         return aa->Iex.Unop.arg;
      }
      if (is_Unop(aa, Iop_64to32)
          && is_Binop(aa->Iex.Unop.arg, Iop_Shl64)
          && is_Unop(aa->Iex.Unop.arg->Iex.Binop.arg1, Iop_32Uto64)
          && is_Unop(aa->Iex.Unop.arg->Iex.Binop.arg1->Iex.Unop.arg,
                     Iop_64to32)) {
         return
            IRExpr_Unop(
               Iop_32Uto64,
               IRExpr_Unop(
                  Iop_64to32,
                  IRExpr_Binop(
                     Iop_Shl64, 
                     aa->Iex.Unop.arg->Iex.Binop.arg1->Iex.Unop.arg->Iex.Unop.arg,
                     aa->Iex.Unop.arg->Iex.Binop.arg2
            )));
      }
      break;

   case Iop_1Sto32:
      
      if (is_Unop(aa, Iop_CmpNEZ8)
          && is_Unop(aa->Iex.Unop.arg, Iop_32to8)
          && is_Unop(aa->Iex.Unop.arg->Iex.Unop.arg, Iop_1Uto32)
          && is_Unop(aa->Iex.Unop.arg->Iex.Unop.arg->Iex.Unop.arg,
                     Iop_CmpNEZ32)) {
         return IRExpr_Unop( Iop_CmpwNEZ32,
                             aa->Iex.Unop.arg->Iex.Unop.arg
                               ->Iex.Unop.arg->Iex.Unop.arg);
      }
      break;

   default:
      break;
   }
   
   return IRExpr_Unop( op, aa );
}

static IRExpr* atbSubst_Expr ( ATmpInfo* env, IRExpr* e )
{
   IRExpr*  e2;
   IRExpr** args2;
   Int      i;

   switch (e->tag) {

      case Iex_CCall:
         args2 = shallowCopyIRExprVec(e->Iex.CCall.args);
         for (i = 0; args2[i]; i++)
            args2[i] = atbSubst_Expr(env,args2[i]);
         return IRExpr_CCall(
                   e->Iex.CCall.cee,
                   e->Iex.CCall.retty,
                   args2
                );
      case Iex_RdTmp:
         e2 = atbSubst_Temp(env, e->Iex.RdTmp.tmp);
         return e2 ? e2 : e;
      case Iex_Mux0X:
         return IRExpr_Mux0X(
                   atbSubst_Expr(env, e->Iex.Mux0X.cond),
                   atbSubst_Expr(env, e->Iex.Mux0X.expr0),
                   atbSubst_Expr(env, e->Iex.Mux0X.exprX)
                );
      case Iex_Qop:
         return IRExpr_Qop(
                   e->Iex.Qop.details->op,
                   atbSubst_Expr(env, e->Iex.Qop.details->arg1),
                   atbSubst_Expr(env, e->Iex.Qop.details->arg2),
                   atbSubst_Expr(env, e->Iex.Qop.details->arg3),
                   atbSubst_Expr(env, e->Iex.Qop.details->arg4)
                );
      case Iex_Triop:
         return IRExpr_Triop(
                   e->Iex.Triop.details->op,
                   atbSubst_Expr(env, e->Iex.Triop.details->arg1),
                   atbSubst_Expr(env, e->Iex.Triop.details->arg2),
                   atbSubst_Expr(env, e->Iex.Triop.details->arg3)
                );
      case Iex_Binop:
         return fold_IRExpr_Binop(
                   e->Iex.Binop.op,
                   atbSubst_Expr(env, e->Iex.Binop.arg1),
                   atbSubst_Expr(env, e->Iex.Binop.arg2)
                );
      case Iex_Unop:
         return fold_IRExpr_Unop(
                   e->Iex.Unop.op,
                   atbSubst_Expr(env, e->Iex.Unop.arg)
                );
      case Iex_Load:
         return IRExpr_Load(
                   e->Iex.Load.end,
                   e->Iex.Load.ty,
                   atbSubst_Expr(env, e->Iex.Load.addr)
                );
      case Iex_GetI:
         return IRExpr_GetI(
                   e->Iex.GetI.descr,
                   atbSubst_Expr(env, e->Iex.GetI.ix),
                   e->Iex.GetI.bias
                );
      case Iex_Const:
      case Iex_Get:
         return e;
      default: 
         vex_printf("\n"); ppIRExpr(e); vex_printf("\n");
         vpanic("atbSubst_Expr");
   }
}


static IRStmt* atbSubst_Stmt ( ATmpInfo* env, IRStmt* st )
{
   Int     i;
   IRDirty *d, *d2;
   IRCAS   *cas, *cas2;
   IRPutI  *puti, *puti2;

   switch (st->tag) {
      case Ist_AbiHint:
         return IRStmt_AbiHint(
                   atbSubst_Expr(env, st->Ist.AbiHint.base),
                   st->Ist.AbiHint.len,
                   atbSubst_Expr(env, st->Ist.AbiHint.nia)
                );
      case Ist_Store:
         return IRStmt_Store(
                   st->Ist.Store.end,
                   atbSubst_Expr(env, st->Ist.Store.addr),
                   atbSubst_Expr(env, st->Ist.Store.data)
                );
      case Ist_WrTmp:
         return IRStmt_WrTmp(
                   st->Ist.WrTmp.tmp,
                   atbSubst_Expr(env, st->Ist.WrTmp.data)
                );
      case Ist_Put:
         return IRStmt_Put(
                   st->Ist.Put.offset,
                   atbSubst_Expr(env, st->Ist.Put.data)
                );
      case Ist_PutI:
         puti  = st->Ist.PutI.details;
         puti2 = mkIRPutI(puti->descr, 
                          atbSubst_Expr(env, puti->ix),
                          puti->bias,
                          atbSubst_Expr(env, puti->data));
         return IRStmt_PutI(puti2);

      case Ist_Exit:
         return IRStmt_Exit(
                   atbSubst_Expr(env, st->Ist.Exit.guard),
                   st->Ist.Exit.jk,
                   st->Ist.Exit.dst,
                   st->Ist.Exit.offsIP
                );
      case Ist_IMark:
         return IRStmt_IMark(st->Ist.IMark.addr,
                             st->Ist.IMark.len,
                             st->Ist.IMark.delta);
      case Ist_NoOp:
         return IRStmt_NoOp();
      case Ist_MBE:
         return IRStmt_MBE(st->Ist.MBE.event);
      case Ist_CAS:
         cas  = st->Ist.CAS.details;
         cas2 = mkIRCAS(
                   cas->oldHi, cas->oldLo, cas->end, 
                   atbSubst_Expr(env, cas->addr),
                   cas->expdHi ? atbSubst_Expr(env, cas->expdHi) : NULL,
                   atbSubst_Expr(env, cas->expdLo),
                   cas->dataHi ? atbSubst_Expr(env, cas->dataHi) : NULL,
                   atbSubst_Expr(env, cas->dataLo)
                );
         return IRStmt_CAS(cas2);
      case Ist_LLSC:
         return IRStmt_LLSC(
                   st->Ist.LLSC.end,
                   st->Ist.LLSC.result,
                   atbSubst_Expr(env, st->Ist.LLSC.addr),
                   st->Ist.LLSC.storedata
                      ? atbSubst_Expr(env, st->Ist.LLSC.storedata) : NULL
                );
      case Ist_Dirty:
         d  = st->Ist.Dirty.details;
         d2 = emptyIRDirty();
         *d2 = *d;
         if (d2->mFx != Ifx_None)
            d2->mAddr = atbSubst_Expr(env, d2->mAddr);
         d2->guard = atbSubst_Expr(env, d2->guard);
         for (i = 0; d2->args[i]; i++)
            d2->args[i] = atbSubst_Expr(env, d2->args[i]);
         return IRStmt_Dirty(d2);
      default: 
         vex_printf("\n"); ppIRStmt(st); vex_printf("\n");
         vpanic("atbSubst_Stmt");
   }
}

 Addr64 ado_treebuild_BB ( IRSB* bb )
{
   Int      i, j, k, m;
   Bool     stmtPuts, stmtStores, invalidateMe;
   IRStmt*  st;
   IRStmt*  st2;
   ATmpInfo env[A_NENV];

   Bool   max_ga_known = False;
   Addr64 max_ga       = 0;

   Int       n_tmps = bb->tyenv->types_used;
   UShort*   uses   = LibVEX_Alloc(n_tmps * sizeof(UShort));


   for (i = 0; i < n_tmps; i++)
      uses[i] = 0;

   for (i = 0; i < bb->stmts_used; i++) {
      st = bb->stmts[i];
      switch (st->tag) {
         case Ist_NoOp:
            continue;
         case Ist_IMark: {
            Int    len = st->Ist.IMark.len;
            Addr64 mga = st->Ist.IMark.addr + (len < 1 ? 1 : len) - 1;
            max_ga_known = True;
            if (mga > max_ga)
               max_ga = mga;
            break;
         }
         default:
            break;
      }
      aoccCount_Stmt( uses, st );
   }
   aoccCount_Expr(uses, bb->next );

#  if 0
   for (i = 0; i < n_tmps; i++) {
      if (uses[i] == 0)
        continue;
      ppIRTemp( (IRTemp)i );
      vex_printf("  used %d\n", (Int)uses[i] );
   }
#  endif


   for (i = 0; i < A_NENV; i++) {
      env[i].bindee = NULL;
      env[i].binder = IRTemp_INVALID;
   }

   /* The stmts in bb are being reordered, and we are guaranteed to
      end up with no more than the number we started with.  Use i to
      be the cursor of the current stmt examined and j <= i to be that
      for the current stmt being written. 
   */
   j = 0;
   for (i = 0; i < bb->stmts_used; i++) {

      st = bb->stmts[i];
      if (st->tag == Ist_NoOp)
         continue;
     
      if (env[A_NENV-1].bindee != NULL) {
         bb->stmts[j] = IRStmt_WrTmp( env[A_NENV-1].binder, 
                                      env[A_NENV-1].bindee );
         j++;
         vassert(j <= i);
         env[A_NENV-1].bindee = NULL;
      }

      
      if (st->tag == Ist_WrTmp && uses[st->Ist.WrTmp.tmp] <= 1) {
         IRExpr *e, *e2;

         if (uses[st->Ist.WrTmp.tmp] == 0) {
	    if (0) vex_printf("DEAD binding\n");
            continue; 
         }
         vassert(uses[st->Ist.WrTmp.tmp] == 1);

         e  = st->Ist.WrTmp.data;
         e2 = atbSubst_Expr(env, e);
         addToEnvFront(env, st->Ist.WrTmp.tmp, e2);
         setHints_Expr(&env[0].doesLoad, &env[0].doesGet, e2);
         continue; 
      }

      
      
      st2 = atbSubst_Stmt(env, st);


      stmtPuts
         = toBool( st->tag == Ist_Put
                   || st->tag == Ist_PutI 
                   || st->tag == Ist_Dirty );

      stmtStores
         = toBool( st->tag == Ist_Store
                   || st->tag == Ist_Dirty
                   || st->tag == Ist_LLSC
                   || st->tag == Ist_CAS );

      for (k = A_NENV-1; k >= 0; k--) {
         if (env[k].bindee == NULL)
            continue;
         invalidateMe
            = toBool(
              
              (env[k].doesLoad && stmtStores)
              
              || (env[k].doesGet && stmtPuts)
              || (env[k].doesLoad && stmtPuts)
              || st->tag == Ist_MBE
              
              || st->tag == Ist_AbiHint
              );
         if (invalidateMe) {
            bb->stmts[j] = IRStmt_WrTmp( env[k].binder, env[k].bindee );
            j++;
            vassert(j <= i);
            env[k].bindee = NULL;
         }
      }

      
      m = 0;
      for (k = 0; k < A_NENV; k++) {
         if (env[k].bindee != NULL) {
            env[m] = env[k];
            m++;
	 }
      }
      for (m = m; m < A_NENV; m++) {
         env[m].bindee = NULL;
      }

      
      bb->stmts[j] = st2;
      
      j++;

      vassert(j <= i+1);
   } 

   bb->next = atbSubst_Expr(env, bb->next);
   bb->stmts_used = j;

   return max_ga_known ? max_ga : ~(Addr64)0;
}



static Bool iropt_verbose = False; 




static 
IRSB* cheap_transformations ( 
         IRSB* bb,
         IRExpr* (*specHelper) (HChar*, IRExpr**, IRStmt**, Int),
         Bool (*preciseMemExnsFn)(Int,Int)
      )
{
   redundant_get_removal_BB ( bb );
   if (iropt_verbose) {
      vex_printf("\n========= REDUNDANT GET\n\n" );
      ppIRSB(bb);
   }

   if (vex_control.iropt_register_updates != VexRegUpdAllregsAtEachInsn) {
      redundant_put_removal_BB ( bb, preciseMemExnsFn );
   }
   if (iropt_verbose) {
      vex_printf("\n========= REDUNDANT PUT\n\n" );
      ppIRSB(bb);
   }

   bb = cprop_BB ( bb );
   if (iropt_verbose) {
      vex_printf("\n========= CPROPD\n\n" );
      ppIRSB(bb);
   }

   do_deadcode_BB ( bb );
   if (iropt_verbose) {
      vex_printf("\n========= DEAD\n\n" );
      ppIRSB(bb);
   }

   bb = spec_helpers_BB ( bb, specHelper );
   do_deadcode_BB ( bb );
   if (iropt_verbose) {
      vex_printf("\n========= SPECd \n\n" );
      ppIRSB(bb);
   }

   return bb;
}



static
IRSB* expensive_transformations( IRSB* bb )
{
   (void)do_cse_BB( bb );
   collapse_AddSub_chains_BB( bb );
   do_redundant_GetI_elimination( bb );
   if (vex_control.iropt_register_updates != VexRegUpdAllregsAtEachInsn) {
      do_redundant_PutI_elimination( bb );
   }
   do_deadcode_BB( bb );
   return bb;
}



static void considerExpensives ( Bool* hasGetIorPutI,
                                 Bool* hasVorFtemps,
                                 IRSB* bb )
{
   Int      i, j;
   IRStmt*  st;
   IRDirty* d;
   IRCAS*   cas;

   *hasGetIorPutI = False;
   *hasVorFtemps  = False;

   for (i = 0; i < bb->stmts_used; i++) {
      st = bb->stmts[i];
      switch (st->tag) {
         case Ist_AbiHint:
            vassert(isIRAtom(st->Ist.AbiHint.base));
            vassert(isIRAtom(st->Ist.AbiHint.nia));
            break;
         case Ist_PutI: 
            *hasGetIorPutI = True;
            break;
         case Ist_WrTmp:  
            if (st->Ist.WrTmp.data->tag == Iex_GetI)
               *hasGetIorPutI = True;
            switch (typeOfIRTemp(bb->tyenv, st->Ist.WrTmp.tmp)) {
               case Ity_I1: case Ity_I8: case Ity_I16: 
               case Ity_I32: case Ity_I64: case Ity_I128: 
                  break;
               case Ity_F32: case Ity_F64: case Ity_F128:
               case Ity_V128: case Ity_V256:
                  *hasVorFtemps = True;
                  break;
               case Ity_D32: case Ity_D64: case Ity_D128:
                  *hasVorFtemps = True;
                  break;
               default: 
                  goto bad;
            }
            break;
         case Ist_Put:
            vassert(isIRAtom(st->Ist.Put.data));
            break;
         case Ist_Store:
            vassert(isIRAtom(st->Ist.Store.addr));
            vassert(isIRAtom(st->Ist.Store.data));
            break;
         case Ist_CAS:
            cas = st->Ist.CAS.details;
            vassert(isIRAtom(cas->addr));
            vassert(cas->expdHi == NULL || isIRAtom(cas->expdHi));
            vassert(isIRAtom(cas->expdLo));
            vassert(cas->dataHi == NULL || isIRAtom(cas->dataHi));
            vassert(isIRAtom(cas->dataLo));
            break;
         case Ist_LLSC:
            vassert(isIRAtom(st->Ist.LLSC.addr));
            if (st->Ist.LLSC.storedata)
               vassert(isIRAtom(st->Ist.LLSC.storedata));
            break;
         case Ist_Dirty:
            d = st->Ist.Dirty.details;
            vassert(isIRAtom(d->guard));
            for (j = 0; d->args[j]; j++)
               vassert(isIRAtom(d->args[j]));
            if (d->mFx != Ifx_None)
               vassert(isIRAtom(d->mAddr));
            break;
         case Ist_NoOp:
         case Ist_IMark:
         case Ist_MBE:
            break;
         case Ist_Exit:
            vassert(isIRAtom(st->Ist.Exit.guard));
            break;
         default: 
         bad:
            ppIRStmt(st);
            vpanic("considerExpensives");
      }
   }
}





IRSB* do_iropt_BB(
         IRSB* bb0,
         IRExpr* (*specHelper) (HChar*, IRExpr**, IRStmt**, Int),
         Bool (*preciseMemExnsFn)(Int,Int),
         Addr64 guest_addr,
         VexArch guest_arch
      )
{
   static Int n_total     = 0;
   static Int n_expensive = 0;

   Bool hasGetIorPutI, hasVorFtemps;
   IRSB *bb, *bb2;

   n_total++;


   bb = flatten_BB ( bb0 );

   if (iropt_verbose) {
      vex_printf("\n========= FLAT\n\n" );
      ppIRSB(bb);
   }

   
   if (vex_control.iropt_level <= 0) return bb;


   bb = cheap_transformations( bb, specHelper, preciseMemExnsFn );

   if (guest_arch == VexArchARM) {
      bb = cprop_BB(bb);
      bb = spec_helpers_BB ( bb, specHelper );
      if (vex_control.iropt_register_updates != VexRegUpdAllregsAtEachInsn) {
         redundant_put_removal_BB ( bb, preciseMemExnsFn );
      }
      do_cse_BB( bb );
      do_deadcode_BB( bb );
   }

   if (vex_control.iropt_level > 1) {

      considerExpensives( &hasGetIorPutI, &hasVorFtemps, bb );

      if (hasVorFtemps && !hasGetIorPutI) {
         (void)do_cse_BB( bb );
         do_deadcode_BB( bb );
      }

      if (hasGetIorPutI) {
         Bool cses;
         n_expensive++;
         if (DEBUG_IROPT)
            vex_printf("***** EXPENSIVE %d %d\n", n_total, n_expensive);
         bb = expensive_transformations( bb );
         bb = cheap_transformations( bb, specHelper, preciseMemExnsFn );
         
         cses = do_cse_BB( bb );
         if (cses)
            bb = cheap_transformations( bb, specHelper, preciseMemExnsFn );
      }


      bb2 = maybe_loop_unroll_BB( bb, guest_addr );
      if (bb2) {
         bb = cheap_transformations( bb2, specHelper, preciseMemExnsFn );
         if (hasGetIorPutI) {
            bb = expensive_transformations( bb );
            bb = cheap_transformations( bb, specHelper, preciseMemExnsFn );
         } else {
            
            do_cse_BB( bb );
            do_deadcode_BB( bb );
         }
         if (0) vex_printf("vex iropt: unrolled a loop\n");
      }

   }

   return bb;
}


