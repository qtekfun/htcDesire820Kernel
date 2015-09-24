

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
#include "libvex.h"

#include "main_util.h"
#include "host_generic_regs.h"

#define DEBUG_REGALLOC 0




typedef
   struct {
      
      Short live_after;
      
      Short dead_before;
      
      Short spill_offset;
      Short spill_size;
      
      HRegClass reg_class;
   }
   VRegLR;


typedef
   struct {
      HReg rreg;
      
      Short live_after;
      
      Short dead_before;
   }
   RRegLR;


typedef
   struct {
      
      
      HReg rreg;
      
      Bool has_hlrs;
      
      Bool is_spill_cand;
      Bool eq_spill_slot;
      
      enum { Free,     
             Unavail,  
             Bound     
           }
           disp;
      
      HReg vreg;
   }
   RRegState;



#define INVALID_RREG_NO ((Short)(-1))

#define IS_VALID_VREGNO(_zz) ((_zz) >= 0 && (_zz) < n_vregs)
#define IS_VALID_RREGNO(_zz) ((_zz) >= 0 && (_zz) < n_rregs)


static Bool instrMentionsReg ( 
   void (*getRegUsage) (HRegUsage*, HInstr*, Bool),
   HInstr* instr, 
   HReg r,
   Bool mode64
)
{
   Int       i;
   HRegUsage reg_usage;
   (*getRegUsage)(&reg_usage, instr, mode64);
   for (i = 0; i < reg_usage.n_used; i++)
      if (reg_usage.hreg[i] == r)
         return True;
   return False;
}


static
Int findMostDistantlyMentionedVReg ( 
   void (*getRegUsage) (HRegUsage*, HInstr*, Bool),
   HInstrArray* instrs_in,
   Int          search_from_instr,
   RRegState*   state,
   Int          n_state,
   Bool         mode64
)
{
   Int k, m;
   Int furthest_k = -1;
   Int furthest   = -1;
   vassert(search_from_instr >= 0);
   for (k = 0; k < n_state; k++) {
      if (!state[k].is_spill_cand)
         continue;
      vassert(state[k].disp == Bound);
      for (m = search_from_instr; m < instrs_in->arr_used; m++) {
         if (instrMentionsReg(getRegUsage, 
                              instrs_in->arr[m], state[k].vreg, mode64))
            break;
      }
      if (m > furthest) {
         furthest   = m;
         furthest_k = k;
      }
   }
   return furthest_k;
}


static inline void sanity_check_spill_offset ( VRegLR* vreg )
{
   switch (vreg->reg_class) {
      case HRcVec128: case HRcFlt64:
         vassert(0 == ((UShort)vreg->spill_offset % 16)); break;
      default:
         vassert(0 == ((UShort)vreg->spill_offset % 8)); break;
   }
}


static void ensureRRLRspace ( RRegLR** info, Int* size, Int used )
{
   Int     k;
   RRegLR* arr2;
   if (used < *size) return;
   if (0)
      vex_printf("ensureRRISpace: %d -> %d\n", *size, 2 * *size);
   vassert(used == *size);
   arr2 = LibVEX_Alloc(2 * *size * sizeof(RRegLR));
   for (k = 0; k < *size; k++)
      arr2[k] = (*info)[k];
   *size *= 2;
   *info = arr2;
}


static void sortRRLRarray ( RRegLR* arr, 
                            Int size, Bool by_live_after )
{
   Int    incs[14] = { 1, 4, 13, 40, 121, 364, 1093, 3280,
                       9841, 29524, 88573, 265720,
                       797161, 2391484 };
   Int    lo = 0;
   Int    hi = size-1;
   Int    i, j, h, bigN, hp;
   RRegLR v;

   vassert(size >= 0);
   if (size == 0)
      return;

   bigN = hi - lo + 1; if (bigN < 2) return;
   hp = 0; while (hp < 14 && incs[hp] < bigN) hp++; hp--;

   if (by_live_after) {

      for ( ; hp >= 0; hp--) {
         h = incs[hp];
         for (i = lo + h; i <= hi; i++) {
            v = arr[i];
            j = i;
            while (arr[j-h].live_after > v.live_after) {
               arr[j] = arr[j-h];
               j = j - h;
               if (j <= (lo + h - 1)) break;
            }
            arr[j] = v;
         }
      }

   } else {

      for ( ; hp >= 0; hp--) {
         h = incs[hp];
         for (i = lo + h; i <= hi; i++) {
            v = arr[i];
            j = i;
            while (arr[j-h].dead_before > v.dead_before) {
               arr[j] = arr[j-h];
               j = j - h;
               if (j <= (lo + h - 1)) break;
            }
            arr[j] = v;
         }
      }

   }
}


HInstrArray* doRegisterAllocation (

    
   HInstrArray* instrs_in,

   HReg* available_real_regs,
   Int   n_available_real_regs,

   Bool (*isMove) ( HInstr*, HReg*, HReg* ),

   
   void (*getRegUsage) ( HRegUsage*, HInstr*, Bool ),

   
   void (*mapRegs) ( HRegRemap*, HInstr*, Bool ),

   void    (*genSpill)  ( HInstr**, HInstr**, HReg, Int, Bool ),
   void    (*genReload) ( HInstr**, HInstr**, HReg, Int, Bool ),
   HInstr* (*directReload) ( HInstr*, HReg, Short ),
   Int     guest_sizeB,

   
   void (*ppInstr) ( HInstr*, Bool ),
   void (*ppReg) ( HReg ),

   
   Bool mode64
)
{
#  define N_SPILL64S  (LibVEX_N_SPILL_BYTES / 8)

   const Bool eq_spill_opt = True;

   
   Int       ii, j, k, m, spillee, k_suboptimal;
   HReg      rreg, vreg, vregS, vregD;
   HRegUsage reg_usage;

   Int     n_vregs;
   VRegLR* vreg_lrs; 

   RRegLR* rreg_lrs_la;
   RRegLR* rreg_lrs_db;
   Int     rreg_lrs_size;
   Int     rreg_lrs_used;
   Int     rreg_lrs_la_next;
   Int     rreg_lrs_db_next;

   Int ss_busy_until_before[N_SPILL64S];

   
   Int* rreg_live_after;
   Int* rreg_dead_before;

   
   RRegState* rreg_state;  
   Int        n_rregs;

   
   Short*     vreg_state;  

   HRegRemap remap;

   
   HInstrArray* instrs_out;

   Bool do_sanity_check;

   vassert(0 == (guest_sizeB % 32));
   vassert(0 == (LibVEX_N_SPILL_BYTES % 32));
   vassert(0 == (N_SPILL64S % 4));

   vassert(instrs_in->arr_used <= 15000);

#  define INVALID_INSTRNO (-2)

#  define EMIT_INSTR(_instr)                  \
      do {                                    \
        HInstr* _tmp = (_instr);              \
        if (DEBUG_REGALLOC) {                 \
           vex_printf("**  ");                \
           (*ppInstr)(_tmp, mode64);          \
           vex_printf("\n\n");                \
        }                                     \
        addHInstr ( instrs_out, _tmp );       \
      } while (0)

#   define PRINT_STATE						   \
      do {							   \
         Int z, q;						   \
         for (z = 0; z < n_rregs; z++) {			   \
            vex_printf("  rreg_state[%2d] = ", z);		   \
            (*ppReg)(rreg_state[z].rreg);	       		   \
            vex_printf("  \t");					   \
            switch (rreg_state[z].disp) {			   \
               case Free:    vex_printf("Free\n"); break;	   \
               case Unavail: vex_printf("Unavail\n"); break;	   \
               case Bound:   vex_printf("BoundTo "); 		   \
                             (*ppReg)(rreg_state[z].vreg);	   \
                             vex_printf("\n"); break;		   \
            }							   \
         }							   \
         vex_printf("\n  vreg_state[0 .. %d]:\n    ", n_vregs-1);  \
         q = 0;                                                    \
         for (z = 0; z < n_vregs; z++) {                           \
            if (vreg_state[z] == INVALID_RREG_NO)                  \
               continue;                                           \
            vex_printf("[%d] -> %d   ", z, vreg_state[z]);         \
            q++;                                                   \
            if (q > 0 && (q % 6) == 0)                             \
               vex_printf("\n    ");                               \
         }                                                         \
         vex_printf("\n");                                         \
      } while (0)


   
   

   instrs_out = newHInstrArray();

   
   
   n_rregs = n_available_real_regs;
   n_vregs = instrs_in->n_vregs;

   
   vassert(n_vregs < 32767);

   rreg_state = LibVEX_Alloc(n_rregs * sizeof(RRegState));
   vreg_state = LibVEX_Alloc(n_vregs * sizeof(Short));

   for (j = 0; j < n_rregs; j++) {
      rreg_state[j].rreg          = available_real_regs[j];
      rreg_state[j].has_hlrs      = False;
      rreg_state[j].disp          = Free;
      rreg_state[j].vreg          = INVALID_HREG;
      rreg_state[j].is_spill_cand = False;
      rreg_state[j].eq_spill_slot = False;
   }

   for (j = 0; j < n_vregs; j++)
      vreg_state[j] = INVALID_RREG_NO;


   
   

   


   vreg_lrs = NULL;
   if (n_vregs > 0)
      vreg_lrs = LibVEX_Alloc(sizeof(VRegLR) * n_vregs);

   for (j = 0; j < n_vregs; j++) {
      vreg_lrs[j].live_after     = INVALID_INSTRNO;
      vreg_lrs[j].dead_before    = INVALID_INSTRNO;
      vreg_lrs[j].spill_offset   = 0;
      vreg_lrs[j].spill_size     = 0;
      vreg_lrs[j].reg_class      = HRcINVALID;
   }

   

   


   rreg_lrs_used = 0;
   rreg_lrs_size = 4;
   rreg_lrs_la = LibVEX_Alloc(rreg_lrs_size * sizeof(RRegLR));
   rreg_lrs_db = NULL; 

   vassert(n_available_real_regs > 0);
   rreg_live_after  = LibVEX_Alloc(n_available_real_regs * sizeof(Int));
   rreg_dead_before = LibVEX_Alloc(n_available_real_regs * sizeof(Int));

   for (j = 0; j < n_available_real_regs; j++) {
      rreg_live_after[j] = 
      rreg_dead_before[j] = INVALID_INSTRNO;
   }

   

   

   for (ii = 0; ii < instrs_in->arr_used; ii++) {

      (*getRegUsage)( &reg_usage, instrs_in->arr[ii], mode64 );

#     if 0
      vex_printf("\n%d  stage1: ", ii);
      (*ppInstr)(instrs_in->arr[ii], mode64);
      vex_printf("\n");
      ppHRegUsage(&reg_usage);
#     endif

      

      
      for (j = 0; j < reg_usage.n_used; j++) {

         vreg = reg_usage.hreg[j];
         
         if (!hregIsVirtual(vreg))
            continue;
         k = hregNumber(vreg);
         if (k < 0 || k >= n_vregs) {
            vex_printf("\n");
            (*ppInstr)(instrs_in->arr[ii], mode64);
            vex_printf("\n");
            vex_printf("vreg %d, n_vregs %d\n", k, n_vregs);
            vpanic("doRegisterAllocation: out-of-range vreg");
         }

         if (vreg_lrs[k].reg_class == HRcINVALID) {
            
            vreg_lrs[k].reg_class = hregClass(vreg);
         } else {
            
            vassert(vreg_lrs[k].reg_class == hregClass(vreg));
         }

         
         switch (reg_usage.mode[j]) {
            case HRmRead: 
               if (vreg_lrs[k].live_after == INVALID_INSTRNO) {
                  vex_printf("\n\nOFFENDING VREG = %d\n", k);
                  vpanic("doRegisterAllocation: "
                         "first event for vreg is Read");
               }
               vreg_lrs[k].dead_before = toShort(ii + 1);
               break;
            case HRmWrite:
               if (vreg_lrs[k].live_after == INVALID_INSTRNO)
                  vreg_lrs[k].live_after = toShort(ii);
               vreg_lrs[k].dead_before = toShort(ii + 1);
               break;
            case HRmModify:
               if (vreg_lrs[k].live_after == INVALID_INSTRNO) {
                  vex_printf("\n\nOFFENDING VREG = %d\n", k);
                  vpanic("doRegisterAllocation: "
                         "first event for vreg is Modify");
               }
               vreg_lrs[k].dead_before = toShort(ii + 1);
               break;
            default:
               vpanic("doRegisterAllocation(1)");
         } 

      } 

      

      

      
      for (j = 0; j < reg_usage.n_used; j++) {

         Int  flush_la = INVALID_INSTRNO, flush_db = INVALID_INSTRNO;
         Bool flush;

         rreg = reg_usage.hreg[j];

         
         if (hregIsVirtual(rreg))
            continue;

         for (k = 0; k < n_available_real_regs; k++)
            if (available_real_regs[k] == rreg)
               break;
         if (k == n_available_real_regs) 
            continue; 
         flush = False;
         switch (reg_usage.mode[j]) {
            case HRmWrite:
               flush_la = rreg_live_after[k];
               flush_db = rreg_dead_before[k];
               if (flush_la != INVALID_INSTRNO 
                   && flush_db != INVALID_INSTRNO)
                  flush = True;
               rreg_live_after[k]  = ii;
               rreg_dead_before[k] = ii+1;
               break;
            case HRmRead:
               if (rreg_live_after[k] == INVALID_INSTRNO) {
                  vex_printf("\nOFFENDING RREG = ");
                  (*ppReg)(available_real_regs[k]);
                  vex_printf("\n");
                  vex_printf("\nOFFENDING instr = ");
                  (*ppInstr)(instrs_in->arr[ii], mode64);
                  vex_printf("\n");
                  vpanic("doRegisterAllocation: "
                         "first event for rreg is Read");
               }
               rreg_dead_before[k] = ii+1;
               break;
            case HRmModify:
               if (rreg_live_after[k] == INVALID_INSTRNO) {
                  vex_printf("\nOFFENDING RREG = ");
                  (*ppReg)(available_real_regs[k]);
                  vex_printf("\n");
                  vex_printf("\nOFFENDING instr = ");
                  (*ppInstr)(instrs_in->arr[ii], mode64);
                  vex_printf("\n");
                  vpanic("doRegisterAllocation: "
                         "first event for rreg is Modify");
               }
               rreg_dead_before[k] = ii+1;
               break;
            default:
               vpanic("doRegisterAllocation(2)");
         }

         if (flush) {
            vassert(flush_la != INVALID_INSTRNO);
            vassert(flush_db != INVALID_INSTRNO);
            ensureRRLRspace(&rreg_lrs_la, &rreg_lrs_size, rreg_lrs_used);
            if (0) 
               vex_printf("FLUSH 1 (%d,%d)\n", flush_la, flush_db);
            rreg_lrs_la[rreg_lrs_used].rreg        = rreg;
            rreg_lrs_la[rreg_lrs_used].live_after  = toShort(flush_la);
            rreg_lrs_la[rreg_lrs_used].dead_before = toShort(flush_db);
            rreg_lrs_used++;
         }

      } 

      

   } 

   

   

   
   for (j = 0; j < n_available_real_regs; j++) {

#     if 0
      vex_printf("residual %d:  %d %d\n", j, rreg_live_after[j],
                                             rreg_dead_before[j]);
#     endif 
      vassert( (rreg_live_after[j] == INVALID_INSTRNO 
               && rreg_dead_before[j] == INVALID_INSTRNO)
              ||
              (rreg_live_after[j] != INVALID_INSTRNO 
               && rreg_dead_before[j] != INVALID_INSTRNO)
            );

      if (rreg_live_after[j] == INVALID_INSTRNO)
         continue;

      ensureRRLRspace(&rreg_lrs_la, &rreg_lrs_size, rreg_lrs_used);
      if (0)
         vex_printf("FLUSH 2 (%d,%d)\n", 
                    rreg_live_after[j], rreg_dead_before[j]);
      rreg_lrs_la[rreg_lrs_used].rreg        = available_real_regs[j];
      rreg_lrs_la[rreg_lrs_used].live_after  = toShort(rreg_live_after[j]);
      rreg_lrs_la[rreg_lrs_used].dead_before = toShort(rreg_dead_before[j]);
      rreg_lrs_used++;
   }


   for (j = 0; j < rreg_lrs_used; j++) {
      rreg = rreg_lrs_la[j].rreg;
      vassert(!hregIsVirtual(rreg));
      for (k = 0; k < n_rregs; k++)
         if (rreg_state[k].rreg == rreg)
            break;
      vassert(k < n_rregs); 
      rreg_state[k].has_hlrs = True;
   }
   if (0) {
      for (j = 0; j < n_rregs; j++) {
         if (!rreg_state[j].has_hlrs)
            continue;
         ppReg(rreg_state[j].rreg);
         vex_printf(" hinted\n");
      }
   }

   rreg_lrs_db = LibVEX_Alloc(rreg_lrs_used * sizeof(RRegLR));
   for (j = 0; j < rreg_lrs_used; j++)
      rreg_lrs_db[j] = rreg_lrs_la[j];

   sortRRLRarray( rreg_lrs_la, rreg_lrs_used, True   );
   sortRRLRarray( rreg_lrs_db, rreg_lrs_used, False );

   
   rreg_lrs_la_next = 0;
   rreg_lrs_db_next = 0;

   for (j = 1; j < rreg_lrs_used; j++) {
      vassert(rreg_lrs_la[j-1].live_after  <= rreg_lrs_la[j].live_after);
      vassert(rreg_lrs_db[j-1].dead_before <= rreg_lrs_db[j].dead_before);
   }

   

#  if DEBUG_REGALLOC
   for (j = 0; j < n_vregs; j++) {
      vex_printf("vreg %d:  la = %d,  db = %d\n", 
                 j, vreg_lrs[j].live_after, vreg_lrs[j].dead_before );
   }
#  endif

#  if DEBUG_REGALLOC
   vex_printf("RRegLRs by LA:\n");
   for (j = 0; j < rreg_lrs_used; j++) {
      vex_printf("  ");
      (*ppReg)(rreg_lrs_la[j].rreg);
      vex_printf("      la = %d,  db = %d\n",
                 rreg_lrs_la[j].live_after, rreg_lrs_la[j].dead_before );
   }
   vex_printf("RRegLRs by DB:\n");
   for (j = 0; j < rreg_lrs_used; j++) {
      vex_printf("  ");
      (*ppReg)(rreg_lrs_db[j].rreg);
      vex_printf("      la = %d,  db = %d\n",
                 rreg_lrs_db[j].live_after, rreg_lrs_db[j].dead_before );
   }
#  endif

   

   

   for (j = 0; j < N_SPILL64S; j++)
      ss_busy_until_before[j] = 0;

   for (j = 0; j < n_vregs; j++) {

      if (vreg_lrs[j].live_after == INVALID_INSTRNO) {
         vassert(vreg_lrs[j].reg_class == HRcINVALID);
         continue;
      }

      switch (vreg_lrs[j].reg_class) {

         case HRcVec128: case HRcFlt64:
            for (k = 0; k < N_SPILL64S-1; k += 2)
               if (ss_busy_until_before[k+0] <= vreg_lrs[j].live_after
                   && ss_busy_until_before[k+1] <= vreg_lrs[j].live_after)
                  break;
            if (k >= N_SPILL64S-1) {
               vpanic("LibVEX_N_SPILL_BYTES is too low.  " 
                      "Increase and recompile.");
            }
            if (0) vex_printf("16-byte spill offset in spill slot %d\n",
                              (Int)k);
            ss_busy_until_before[k+0] = vreg_lrs[j].dead_before;
            ss_busy_until_before[k+1] = vreg_lrs[j].dead_before;
            break;

         default:
            
            for (k = 0; k < N_SPILL64S; k++)
               if (ss_busy_until_before[k] <= vreg_lrs[j].live_after)
                  break;
            if (k == N_SPILL64S) {
               vpanic("LibVEX_N_SPILL_BYTES is too low.  " 
                      "Increase and recompile.");
            }
            ss_busy_until_before[k] = vreg_lrs[j].dead_before;
            break;

      } 

      vreg_lrs[j].spill_offset = toShort(guest_sizeB * 3 + k * 8);

      
      sanity_check_spill_offset( &vreg_lrs[j] );
      
      
   }

#  if 0
   vex_printf("\n\n");
   for (j = 0; j < n_vregs; j++)
      vex_printf("vreg %d    --> spill offset %d\n",
                 j, vreg_lrs[j].spill_offset);
#  endif

   



   


   

   for (ii = 0; ii < instrs_in->arr_used; ii++) {

#     if DEBUG_REGALLOC
      vex_printf("\n====----====---- Insn %d ----====----====\n", ii);
      vex_printf("---- ");
      (*ppInstr)(instrs_in->arr[ii], mode64);
      vex_printf("\n\nInitial state:\n");
      PRINT_STATE;
      vex_printf("\n");
#     endif

      

      do_sanity_check
         = toBool(
              False  
              || ii == instrs_in->arr_used-1
              || (ii > 0 && (ii % 7) == 0)
           );

      if (do_sanity_check) {

         for (j = 0; j < rreg_lrs_used; j++) {
            if (rreg_lrs_la[j].live_after < ii 
                && ii < rreg_lrs_la[j].dead_before) {

#              if 0
               vex_printf("considering la %d .. db %d   reg = ", 
                          rreg_lrs[j].live_after, 
                          rreg_lrs[j].dead_before);
               (*ppReg)(rreg_lrs[j].rreg);
               vex_printf("\n");
#              endif

               
               for (k = 0; k < n_rregs; k++)
                  if (rreg_state[k].rreg == rreg_lrs_la[j].rreg)
                     break;

               
               vassert(rreg_state[k].disp == Unavail);
            }
         }

         for (j = 0; j < n_available_real_regs; j++) {
            vassert(rreg_state[j].disp == Bound
                    || rreg_state[j].disp == Free
                    || rreg_state[j].disp == Unavail);
            if (rreg_state[j].disp != Unavail)
               continue;
            for (k = 0; k < rreg_lrs_used; k++) 
               if (rreg_lrs_la[k].rreg == rreg_state[j].rreg
                   && rreg_lrs_la[k].live_after < ii 
                   && ii < rreg_lrs_la[k].dead_before) 
                  break;
            vassert(k < rreg_lrs_used);
         }

         for (j = 0; j < n_rregs; j++) {
            if (rreg_state[j].disp != Bound) {
               vassert(rreg_state[j].eq_spill_slot == False);
               continue;
            }
            vassert(hregClass(rreg_state[j].rreg) 
                    == hregClass(rreg_state[j].vreg));
            vassert( hregIsVirtual(rreg_state[j].vreg));
            vassert(!hregIsVirtual(rreg_state[j].rreg));
         }

         for (j = 0; j < n_rregs; j++) {
            if (rreg_state[j].disp != Bound)
               continue;
            k = hregNumber(rreg_state[j].vreg);
            vassert(IS_VALID_VREGNO(k));
            vassert(vreg_state[k] == j);
         }
         for (j = 0; j < n_vregs; j++) {
            k = vreg_state[j];
            if (k == INVALID_RREG_NO)
               continue;
            vassert(IS_VALID_RREGNO(k));
            vassert(rreg_state[k].disp == Bound);
            vassert(hregNumber(rreg_state[k].vreg) == j);
         }

      } 

      

      if ( (*isMove)( instrs_in->arr[ii], &vregS, &vregD ) ) {
         if (!hregIsVirtual(vregS)) goto cannot_coalesce;
         if (!hregIsVirtual(vregD)) goto cannot_coalesce;
         
         vassert(hregClass(vregS) == hregClass(vregD));
         k = hregNumber(vregS);
         m = hregNumber(vregD);
         vassert(IS_VALID_VREGNO(k));
         vassert(IS_VALID_VREGNO(m));
         if (vreg_lrs[k].dead_before != ii + 1) goto cannot_coalesce;
         if (vreg_lrs[m].live_after != ii) goto cannot_coalesce;
#        if DEBUG_REGALLOC
         vex_printf("COALESCE ");
         (*ppReg)(vregS);
         vex_printf(" -> ");
         (*ppReg)(vregD);
         vex_printf("\n\n");
#        endif
         
         for (m = 0; m < n_rregs; m++)
            if (rreg_state[m].disp == Bound && rreg_state[m].vreg == vregS)
               break;
         if (m == n_rregs)
            goto cannot_coalesce;

         rreg_state[m].vreg = vregD;
         vassert(IS_VALID_VREGNO(hregNumber(vregD)));
         vassert(IS_VALID_VREGNO(hregNumber(vregS)));
         vreg_state[hregNumber(vregD)] = toShort(m);
         vreg_state[hregNumber(vregS)] = INVALID_RREG_NO;

         rreg_state[m].eq_spill_slot = False;

         continue;
      }
     cannot_coalesce:

      


      for (j = 0; j < n_rregs; j++) {
         if (rreg_state[j].disp != Bound)
            continue;
         vreg = hregNumber(rreg_state[j].vreg);
         vassert(IS_VALID_VREGNO(vreg));
         if (vreg_lrs[vreg].dead_before <= ii) {
            rreg_state[j].disp = Free;
            rreg_state[j].eq_spill_slot = False;
            m = hregNumber(rreg_state[j].vreg);
            vassert(IS_VALID_VREGNO(m));
            vreg_state[m] = INVALID_RREG_NO;
            if (DEBUG_REGALLOC) {
               vex_printf("free up "); 
               (*ppReg)(rreg_state[j].rreg); 
               vex_printf("\n");
            }
         }
      }

      

      while (True) {
         vassert(rreg_lrs_la_next >= 0);
         vassert(rreg_lrs_la_next <= rreg_lrs_used);
         if (rreg_lrs_la_next == rreg_lrs_used)
            break; 
         if (ii < rreg_lrs_la[rreg_lrs_la_next].live_after)
            break; 
         vassert(ii == rreg_lrs_la[rreg_lrs_la_next].live_after);
#        if DEBUG_REGALLOC
         vex_printf("need to free up rreg: ");
         (*ppReg)(rreg_lrs_la[rreg_lrs_la_next].rreg);
         vex_printf("\n\n");
#        endif
         for (k = 0; k < n_rregs; k++)
            if (rreg_state[k].rreg == rreg_lrs_la[rreg_lrs_la_next].rreg)
               break;
         vassert(IS_VALID_RREGNO(k));
         m = hregNumber(rreg_state[k].vreg);
         if (rreg_state[k].disp == Bound) {
            vassert(IS_VALID_VREGNO(m));
            vreg_state[m] = INVALID_RREG_NO;
            if (vreg_lrs[m].dead_before > ii) {
               vassert(vreg_lrs[m].reg_class != HRcINVALID);
               if ((!eq_spill_opt) || !rreg_state[k].eq_spill_slot) {
                  HInstr* spill1 = NULL;
                  HInstr* spill2 = NULL;
                  (*genSpill)( &spill1, &spill2, rreg_state[k].rreg,
                               vreg_lrs[m].spill_offset, mode64 );
                  vassert(spill1 || spill2); 
                  if (spill1)
                     EMIT_INSTR(spill1);
                  if (spill2)
                     EMIT_INSTR(spill2);
               }
               rreg_state[k].eq_spill_slot = True;
            }
         }
         rreg_state[k].disp = Unavail;
         rreg_state[k].vreg = INVALID_HREG;
         rreg_state[k].eq_spill_slot = False;

         
         rreg_lrs_la_next++;
      }


#     if DEBUG_REGALLOC
      vex_printf("After pre-insn actions for fixed regs:\n");
      PRINT_STATE;
      vex_printf("\n");
#     endif


      

      
      (*getRegUsage)( &reg_usage, instrs_in->arr[ii], mode64 );

      initHRegRemap(&remap);

      

      
      if (directReload && reg_usage.n_used <= 2) { 
         Bool  debug_direct_reload = True && False;
         HReg  cand     = INVALID_HREG;
         Bool  nreads   = 0;
         Short spilloff = 0;

         for (j = 0; j < reg_usage.n_used; j++) {

            vreg = reg_usage.hreg[j];

            if (!hregIsVirtual(vreg)) 
               continue;

            if (reg_usage.mode[j] == HRmRead) {
               nreads++;
               m = hregNumber(vreg);
               vassert(IS_VALID_VREGNO(m));
               k = vreg_state[m];
               if (!IS_VALID_RREGNO(k)) {
                  
                  vassert(vreg_lrs[m].dead_before >= ii+1);
                  if (vreg_lrs[m].dead_before == ii+1
                      && cand == INVALID_HREG) {
                     spilloff = vreg_lrs[m].spill_offset;
                     cand = vreg;
                  }
               }
            }
         }

         if (nreads == 1 && cand != INVALID_HREG) {
            HInstr* reloaded;
            if (reg_usage.n_used == 2)
               vassert(reg_usage.hreg[0] != reg_usage.hreg[1]);

            reloaded = directReload ( instrs_in->arr[ii], cand, spilloff );
            if (debug_direct_reload && !reloaded) {
               vex_printf("[%3d] ", spilloff); ppHReg(cand); vex_printf(" "); 
               ppInstr(instrs_in->arr[ii], mode64); 
            }
            if (reloaded) {
               instrs_in->arr[ii] = reloaded;
               (*getRegUsage)( &reg_usage, instrs_in->arr[ii], mode64 );
               if (debug_direct_reload && !reloaded) {
                  vex_printf("  -->  ");
                  ppInstr(reloaded, mode64);
               }
            }

            if (debug_direct_reload && !reloaded)
               vex_printf("\n");
         }

      }

      

      
      for (j = 0; j < reg_usage.n_used; j++) {

         vreg = reg_usage.hreg[j];

         
         if (!hregIsVirtual(vreg)) 
            continue;

#        if 0
         vex_printf("considering "); (*ppReg)(vreg); vex_printf("\n");
#        endif

         m = hregNumber(vreg);
         vassert(IS_VALID_VREGNO(m));
         k = vreg_state[m];
         if (IS_VALID_RREGNO(k)) {
            vassert(rreg_state[k].disp == Bound);
            addToHRegRemap(&remap, vreg, rreg_state[k].rreg);
            /* If this rreg is written or modified, mark it as different
               from any spill slot value. */
            if (reg_usage.mode[j] != HRmRead)
               rreg_state[k].eq_spill_slot = False;
            continue;
         } else {
            vassert(k == INVALID_RREG_NO);
         }

         k_suboptimal = -1;
         for (k = 0; k < n_rregs; k++) {
            if (rreg_state[k].disp != Free
                || hregClass(rreg_state[k].rreg) != hregClass(vreg))
               continue;
            if (rreg_state[k].has_hlrs) {
               k_suboptimal = k;
            } else {
               
               k_suboptimal = -1;
               break;
            }
         }
         if (k_suboptimal >= 0)
            k = k_suboptimal;

         if (k < n_rregs) {
            rreg_state[k].disp = Bound;
            rreg_state[k].vreg = vreg;
            m = hregNumber(vreg);
            vassert(IS_VALID_VREGNO(m));
            vreg_state[m] = toShort(k);
            addToHRegRemap(&remap, vreg, rreg_state[k].rreg);
            if (reg_usage.mode[j] != HRmWrite) {
               vassert(vreg_lrs[m].reg_class != HRcINVALID);
               HInstr* reload1 = NULL;
               HInstr* reload2 = NULL;
               (*genReload)( &reload1, &reload2, rreg_state[k].rreg,
                             vreg_lrs[m].spill_offset, mode64 );
               vassert(reload1 || reload2); 
               if (reload1)
                  EMIT_INSTR(reload1);
               if (reload2)
                  EMIT_INSTR(reload2);
               if (reg_usage.mode[j] == HRmRead) {
                  rreg_state[k].eq_spill_slot = True;
               } else {
                  vassert(reg_usage.mode[j] == HRmModify);
                  rreg_state[k].eq_spill_slot = False;
               }
            } else {
               rreg_state[k].eq_spill_slot = False;
            }

            continue;
         }


         for (k = 0; k < n_rregs; k++) {
            rreg_state[k].is_spill_cand = False;
            if (rreg_state[k].disp != Bound)
               continue;
            if (hregClass(rreg_state[k].rreg) != hregClass(vreg))
               continue;
            rreg_state[k].is_spill_cand = True;
            for (m = 0; m < reg_usage.n_used; m++) {
               if (rreg_state[k].vreg == reg_usage.hreg[m]) {
                  rreg_state[k].is_spill_cand = False;
                  break;
               }
            }
         }

         spillee
            = findMostDistantlyMentionedVReg ( 
                 getRegUsage, instrs_in, ii+1, rreg_state, n_rregs, mode64 );

         if (spillee == -1) {
            vex_printf("reg_alloc: can't find a register in class: ");
            ppHRegClass(hregClass(vreg));
            vex_printf("\n");
            vpanic("reg_alloc: can't create a free register.");
         }

         
         vassert(IS_VALID_RREGNO(spillee));
         vassert(rreg_state[spillee].disp == Bound);
         
         vassert(hregClass(rreg_state[spillee].rreg) == hregClass(vreg));
         vassert(rreg_state[spillee].vreg != vreg);

         m = hregNumber(rreg_state[spillee].vreg);
         vassert(IS_VALID_VREGNO(m));

         vassert(vreg_lrs[m].dead_before > ii);
         vassert(vreg_lrs[m].reg_class != HRcINVALID);
         if ((!eq_spill_opt) || !rreg_state[spillee].eq_spill_slot) {
            HInstr* spill1 = NULL;
            HInstr* spill2 = NULL;
            (*genSpill)( &spill1, &spill2, rreg_state[spillee].rreg,
                         vreg_lrs[m].spill_offset, mode64 );
            vassert(spill1 || spill2); 
            if (spill1)
               EMIT_INSTR(spill1);
            if (spill2)
               EMIT_INSTR(spill2);
         }

         rreg_state[spillee].vreg = vreg;
         vreg_state[m] = INVALID_RREG_NO;

         rreg_state[spillee].eq_spill_slot = False; 

         m = hregNumber(vreg);
         vassert(IS_VALID_VREGNO(m));
         vreg_state[m] = toShort(spillee);

         /* Now, if this vreg is being read or modified (as opposed to
            written), we have to generate a reload for it. */
         if (reg_usage.mode[j] != HRmWrite) {
            vassert(vreg_lrs[m].reg_class != HRcINVALID);
            HInstr* reload1 = NULL;
            HInstr* reload2 = NULL;
            (*genReload)( &reload1, &reload2, rreg_state[spillee].rreg,
                          vreg_lrs[m].spill_offset, mode64 );
            vassert(reload1 || reload2); 
            if (reload1)
               EMIT_INSTR(reload1);
            if (reload2)
               EMIT_INSTR(reload2);
            if (reg_usage.mode[j] == HRmRead) {
               rreg_state[spillee].eq_spill_slot = True;
            } else {
               vassert(reg_usage.mode[j] == HRmModify);
               rreg_state[spillee].eq_spill_slot = False;
            }
         }

         addToHRegRemap(&remap, vreg, rreg_state[spillee].rreg);

      } 


      
      (*mapRegs)( &remap, instrs_in->arr[ii], mode64 );
      EMIT_INSTR( instrs_in->arr[ii] );

#     if DEBUG_REGALLOC
      vex_printf("After dealing with current insn:\n");
      PRINT_STATE;
      vex_printf("\n");
#     endif

      

      while (True) {
         vassert(rreg_lrs_db_next >= 0);
         vassert(rreg_lrs_db_next <= rreg_lrs_used);
         if (rreg_lrs_db_next == rreg_lrs_used)
            break; 
         if (ii+1 < rreg_lrs_db[rreg_lrs_db_next].dead_before)
            break; 
         vassert(ii+1 == rreg_lrs_db[rreg_lrs_db_next].dead_before);
         for (k = 0; k < n_rregs; k++)
            if (rreg_state[k].rreg == rreg_lrs_db[rreg_lrs_db_next].rreg)
               break;
         vassert(k < n_rregs);
         vassert(rreg_state[k].disp == Unavail);
         rreg_state[k].disp = Free;
         rreg_state[k].vreg = INVALID_HREG;
         rreg_state[k].eq_spill_slot = False;

         
         rreg_lrs_db_next++;
      }

#     if DEBUG_REGALLOC
      vex_printf("After post-insn actions for fixed regs:\n");
      PRINT_STATE;
      vex_printf("\n");
#     endif

   } 

   

   
   
   

   
   for (j = 0; j < n_rregs; j++)
      vassert(rreg_state[j].rreg == available_real_regs[j]);

   vassert(rreg_lrs_la_next == rreg_lrs_used);
   vassert(rreg_lrs_db_next == rreg_lrs_used);

   return instrs_out;

#  undef INVALID_INSTRNO
#  undef EMIT_INSTR
#  undef PRINT_STATE
}



