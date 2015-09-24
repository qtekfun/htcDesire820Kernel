

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
#include "pub_core_options.h"      
#include "pub_core_debuginfo.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcprint.h"
#include "pub_core_xarray.h"
#include "pub_core_oset.h"

#include "priv_misc.h"         
#include "priv_d3basics.h"     
#include "priv_tytypes.h"
#include "priv_storage.h"      



void ML_(symerr) ( struct _DebugInfo* di, Bool serious, HChar* msg )
{
   
   if (VG_(clo_xml))
      return;

   if (serious) {

      VG_(message)(Vg_DebugMsg, "WARNING: Serious error when "
                                "reading debug info\n");
      if (True || VG_(clo_verbosity) < 2) {
         VG_(message)(Vg_DebugMsg, 
                      "When reading debug info from %s:\n",
                      (di && di->fsm.filename) ? di->fsm.filename
                                               : (UChar*)"???");
      }
      VG_(message)(Vg_DebugMsg, "%s\n", msg);

   } else { 

      if (VG_(clo_verbosity) >= 2)
         VG_(message)(Vg_DebugMsg, "%s\n", msg);

   }
}


void ML_(ppSym) ( Int idx, DiSym* sym )
{
   UChar** sec_names = sym->sec_names;
   vg_assert(sym->pri_name);
   if (sec_names)
      vg_assert(sec_names);
   VG_(printf)( "%5d:  %c%c %#8lx .. %#8lx (%d)      %s%s",
                idx,
                sym->isText ? 'T' : '-',
                sym->isIFunc ? 'I' : '-',
                sym->addr, 
                sym->addr + sym->size - 1, sym->size,
                sym->pri_name, sec_names ? " " : "" );
   if (sec_names) {
      while (*sec_names) {
         VG_(printf)("%s%s", *sec_names, *(sec_names+1) ? " " : "");
         sec_names++;
      }
   }
   VG_(printf)("\n");
}

void ML_(ppDiCfSI) ( XArray*  exprs, DiCfSI* si )
{
#  define SHOW_HOW(_how, _off)                   \
      do {                                       \
         if (_how == CFIR_UNKNOWN) {             \
            VG_(printf)("Unknown");              \
         } else                                  \
         if (_how == CFIR_SAME) {                \
            VG_(printf)("Same");                 \
         } else                                  \
         if (_how == CFIR_CFAREL) {              \
            VG_(printf)("cfa+%d", _off);         \
         } else                                  \
         if (_how == CFIR_MEMCFAREL) {           \
            VG_(printf)("*(cfa+%d)", _off);      \
         } else                                  \
         if (_how == CFIR_EXPR) {                \
            VG_(printf)("{");                    \
            ML_(ppCfiExpr)(exprs, _off);         \
            VG_(printf)("}");                    \
         } else {                                \
            vg_assert(0+0);                      \
         }                                       \
      } while (0)

   VG_(printf)("[%#lx .. %#lx]: ", si->base,
                               si->base + (UWord)si->len - 1);
   switch (si->cfa_how) {
      case CFIC_IA_SPREL: 
         VG_(printf)("let cfa=oldSP+%d", si->cfa_off); 
         break;
      case CFIC_IA_BPREL: 
         VG_(printf)("let cfa=oldBP+%d", si->cfa_off); 
         break;
      case CFIC_ARM_R13REL: 
         VG_(printf)("let cfa=oldR13+%d", si->cfa_off); 
         break;
      case CFIC_ARM_R12REL: 
         VG_(printf)("let cfa=oldR12+%d", si->cfa_off); 
         break;
      case CFIC_ARM_R11REL: 
         VG_(printf)("let cfa=oldR11+%d", si->cfa_off); 
         break;
      case CFIR_SAME:
         VG_(printf)("let cfa=Same");
         break;
      case CFIC_ARM_R7REL: 
         VG_(printf)("let cfa=oldR7+%d", si->cfa_off); 
         break;
      case CFIC_EXPR: 
         VG_(printf)("let cfa={"); 
         ML_(ppCfiExpr)(exprs, si->cfa_off);
         VG_(printf)("}"); 
         break;
      default: 
         vg_assert(0);
   }

   VG_(printf)(" in RA=");
   SHOW_HOW(si->ra_how, si->ra_off);
#  if defined(VGA_x86) || defined(VGA_amd64)
   VG_(printf)(" SP=");
   SHOW_HOW(si->sp_how, si->sp_off);
   VG_(printf)(" BP=");
   SHOW_HOW(si->bp_how, si->bp_off);
#  elif defined(VGA_arm)
   VG_(printf)(" R14=");
   SHOW_HOW(si->r14_how, si->r14_off);
   VG_(printf)(" R13=");
   SHOW_HOW(si->r13_how, si->r13_off);
   VG_(printf)(" R12=");
   SHOW_HOW(si->r12_how, si->r12_off);
   VG_(printf)(" R11=");
   SHOW_HOW(si->r11_how, si->r11_off);
   VG_(printf)(" R7=");
   SHOW_HOW(si->r7_how, si->r7_off);
#  elif defined(VGA_ppc32) || defined(VGA_ppc64)
#  elif defined(VGA_s390x) || defined(VGA_mips32)
   VG_(printf)(" SP=");
   SHOW_HOW(si->sp_how, si->sp_off);
   VG_(printf)(" FP=");
   SHOW_HOW(si->fp_how, si->fp_off);
#  else
#    error "Unknown arch"
#  endif
   VG_(printf)("\n");
#  undef SHOW_HOW
}



UChar* ML_(addStr) ( struct _DebugInfo* di, UChar* str, Int len )
{
   struct strchunk *chunk;
   Int    space_needed;
   UChar* p;

   if (len == -1) {
      len = VG_(strlen)(str);
   } else {
      vg_assert(len >= 0);
   }

   space_needed = 1 + len;

   
   if (di->strchunks == NULL || 
       (di->strchunks->strtab_used 
        + space_needed) > SEGINFO_STRCHUNKSIZE) {
      chunk = ML_(dinfo_zalloc)("di.storage.addStr.1", sizeof(*chunk));
      chunk->strtab_used = 0;
      chunk->next = di->strchunks;
      di->strchunks = chunk;
   }
   chunk = di->strchunks;

   p = &chunk->strtab[chunk->strtab_used];
   VG_(memcpy)(p, str, len);
   chunk->strtab[chunk->strtab_used+len] = '\0';
   chunk->strtab_used += space_needed;

   return p;
}


void ML_(addSym) ( struct _DebugInfo* di, DiSym* sym )
{
   UInt   new_sz, i;
   DiSym* new_tab;

   vg_assert(sym->pri_name != NULL);
   vg_assert(sym->sec_names == NULL);

   
   if (sym->size == 0) return;

   if (di->symtab_used == di->symtab_size) {
      new_sz = 2 * di->symtab_size;
      if (new_sz == 0) new_sz = 500;
      new_tab = ML_(dinfo_zalloc)( "di.storage.addSym.1", 
                                   new_sz * sizeof(DiSym) );
      if (di->symtab != NULL) {
         for (i = 0; i < di->symtab_used; i++)
            new_tab[i] = di->symtab[i];
         ML_(dinfo_free)(di->symtab);
      }
      di->symtab = new_tab;
      di->symtab_size = new_sz;
   }

   di->symtab[di->symtab_used++] = *sym;
   vg_assert(di->symtab_used <= di->symtab_size);
}


static void addLoc ( struct _DebugInfo* di, DiLoc* loc )
{
   UInt   new_sz, i;
   DiLoc* new_tab;

   
   vg_assert(loc->size > 0);

   if (di->loctab_used == di->loctab_size) {
      new_sz = 2 * di->loctab_size;
      if (new_sz == 0) new_sz = 500;
      new_tab = ML_(dinfo_zalloc)( "di.storage.addLoc.1",
                                   new_sz * sizeof(DiLoc) );
      if (di->loctab != NULL) {
         for (i = 0; i < di->loctab_used; i++)
            new_tab[i] = di->loctab[i];
         ML_(dinfo_free)(di->loctab);
      }
      di->loctab = new_tab;
      di->loctab_size = new_sz;
   }

   di->loctab[di->loctab_used] = *loc;
   di->loctab_used++;
   vg_assert(di->loctab_used <= di->loctab_size);
}


static void shrinkLocTab ( struct _DebugInfo* di )
{
   DiLoc* new_tab;
   UWord new_sz = di->loctab_used;
   if (new_sz == di->loctab_size) return;
   vg_assert(new_sz < di->loctab_size);

   new_tab = ML_(dinfo_zalloc)( "di.storage.shrinkLocTab", 
                                new_sz * sizeof(DiLoc) );
   VG_(memcpy)(new_tab, di->loctab, new_sz * sizeof(DiLoc));

   ML_(dinfo_free)(di->loctab);
   di->loctab = new_tab;
   di->loctab_size = new_sz;
}


void ML_(addLineInfo) ( struct _DebugInfo* di,
                        UChar*   filename,
                        UChar*   dirname, 
                        Addr     this,
                        Addr     next,
                        Int      lineno,
                        Int      entry 
     )
{
   static const Bool debug = False;
   DiLoc loc;
   UWord size = next - this;

   
   if (this == next) return;

   if (debug)
      VG_(printf)( "  src %s %s line %d %#lx-%#lx\n",
                   dirname ? dirname : (UChar*)"(unknown)",
                   filename, lineno, this, next );

   if (this > next) {
       if (VG_(clo_verbosity) > 2) {
           VG_(message)(Vg_DebugMsg, 
                        "warning: line info addresses out of order "
                        "at entry %d: 0x%lx 0x%lx\n", entry, this, next);
       }
       size = 1;
   }

   if (size > MAX_LOC_SIZE) {
       if (0)
       VG_(message)(Vg_DebugMsg, 
                    "warning: line info address range too large "
                    "at entry %d: %lu\n", entry, size);
       size = 1;
   }

   vg_assert(size >= 1);
   vg_assert(size <= MAX_LOC_SIZE);

   vg_assert(di->fsm.have_rx_map && di->fsm.have_rw_map);
   if (ML_(find_rx_mapping)(di, this, this + size - 1) == NULL) {
       if (0)
          VG_(message)(Vg_DebugMsg, 
                       "warning: ignoring line info entry falling "
                       "outside current DebugInfo: %#lx %#lx %#lx %#lx\n",
                       di->text_avma, 
                       di->text_avma + di->text_size, 
                       this, this + size - 1);
       return;
   }

   vg_assert(lineno >= 0);
   if (lineno > MAX_LINENO) {
      static Bool complained = False;
      if (!complained) {
         complained = True;
         VG_(message)(Vg_UserMsg, 
                      "warning: ignoring line info entry with "
                      "huge line number (%d)\n", lineno);
         VG_(message)(Vg_UserMsg, 
                      "         Can't handle line numbers "
                      "greater than %d, sorry\n", MAX_LINENO);
         VG_(message)(Vg_UserMsg, 
                      "(Nb: this message is only shown once)\n");
      }
      return;
   }

   loc.addr      = this;
   loc.size      = (UShort)size;
   loc.lineno    = lineno;
   loc.filename  = filename;
   loc.dirname   = dirname;

   if (0) VG_(message)(Vg_DebugMsg, 
		       "addLoc: addr %#lx, size %lu, line %d, file %s\n",
		       this,size,lineno,filename);

   addLoc ( di, &loc );
}


void ML_(addDiCfSI) ( struct _DebugInfo* di, DiCfSI* cfsi_orig )
{
   static const Bool debug = False;
   UInt    new_sz, i;
   DiCfSI* new_tab;
   SSizeT  delta;
   struct _DebugInfoMapping* map;
   struct _DebugInfoMapping* map2;

   
   DiCfSI cfsi = *cfsi_orig;

   if (debug) {
      VG_(printf)("adding DiCfSI: ");
      ML_(ppDiCfSI)(di->cfsi_exprs, &cfsi);
   }

   
   vg_assert(cfsi.len > 0);
   vg_assert(cfsi.len < 5000000);

   vg_assert(di->fsm.have_rx_map && di->fsm.have_rw_map);
   
   map  = ML_(find_rx_mapping)(di, cfsi.base, cfsi.base);
   map2 = ML_(find_rx_mapping)(di, cfsi.base + cfsi.len - 1,
                                   cfsi.base + cfsi.len - 1);
   if (map == NULL)
      map = map2;
   else if (map2 == NULL)
      map2 = map;

   if (map == NULL || map != map2) {
      static Int complaints = 10;
      if (VG_(clo_trace_cfi) || complaints > 0) {
         complaints--;
         if (VG_(clo_verbosity) > 1) {
            VG_(message)(
               Vg_DebugMsg,
               "warning: DiCfSI %#lx .. %#lx outside mapped rw segments (%s)\n",
               cfsi.base, 
               cfsi.base + cfsi.len - 1,
               di->soname
            );
         }
         if (VG_(clo_trace_cfi)) 
            ML_(ppDiCfSI)(di->cfsi_exprs, &cfsi);
      }
      return;
   }

   if (cfsi.base < map->avma) {
      if (0) VG_(printf)("XXX truncate lower\n");
      vg_assert(cfsi.base + cfsi.len - 1 >= map->avma);
      delta = (SSizeT)(map->avma - cfsi.base);
      vg_assert(delta > 0);
      vg_assert(delta < (SSizeT)cfsi.len);
      cfsi.base += delta;
      cfsi.len -= delta;
   }
   else
   if (cfsi.base + cfsi.len - 1 > map->avma + map->size - 1) {
      if (0) VG_(printf)("XXX truncate upper\n");
      vg_assert(cfsi.base <= map->avma + map->size - 1);
      delta = (SSizeT)( (cfsi.base + cfsi.len - 1) 
                        - (map->avma + map->size - 1) );
      vg_assert(delta > 0);
      vg_assert(delta < (SSizeT)cfsi.len);
      cfsi.len -= delta;
   }

   

   vg_assert(cfsi.len > 0);

   
   vg_assert(cfsi.base >= map->avma);
   vg_assert(cfsi.base + cfsi.len - 1
             <= map->avma + map->size - 1);

   if (di->cfsi_used == di->cfsi_size) {
      new_sz = 2 * di->cfsi_size;
      if (new_sz == 0) new_sz = 20;
      new_tab = ML_(dinfo_zalloc)( "di.storage.addDiCfSI.1",
                                   new_sz * sizeof(DiCfSI) );
      if (di->cfsi != NULL) {
         for (i = 0; i < di->cfsi_used; i++)
            new_tab[i] = di->cfsi[i];
         ML_(dinfo_free)(di->cfsi);
      }
      di->cfsi = new_tab;
      di->cfsi_size = new_sz;
   }

   di->cfsi[di->cfsi_used] = cfsi;
   di->cfsi_used++;
   vg_assert(di->cfsi_used <= di->cfsi_size);
}


Int ML_(CfiExpr_Undef)( XArray* dst )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_Undef;
   return (Int)VG_(addToXA)( dst, &e );
}
Int ML_(CfiExpr_Deref)( XArray* dst, Int ixAddr )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_Deref;
   e.Cex.Deref.ixAddr = ixAddr;
   return (Int)VG_(addToXA)( dst, &e );
}
Int ML_(CfiExpr_Const)( XArray* dst, UWord con )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_Const;
   e.Cex.Const.con = con;
   return (Int)VG_(addToXA)( dst, &e );
}
Int ML_(CfiExpr_Binop)( XArray* dst, CfiOp op, Int ixL, Int ixR )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_Binop;
   e.Cex.Binop.op  = op;
   e.Cex.Binop.ixL = ixL;
   e.Cex.Binop.ixR = ixR;
   return (Int)VG_(addToXA)( dst, &e );
}
Int ML_(CfiExpr_CfiReg)( XArray* dst, CfiReg reg )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_CfiReg;
   e.Cex.CfiReg.reg = reg;
   return (Int)VG_(addToXA)( dst, &e );
}
Int ML_(CfiExpr_DwReg)( XArray* dst, Int reg )
{
   CfiExpr e;
   VG_(memset)( &e, 0, sizeof(e) );
   e.tag = Cex_DwReg;
   e.Cex.DwReg.reg = reg;
   return (Int)VG_(addToXA)( dst, &e );
}

static void ppCfiOp ( CfiOp op ) 
{
   switch (op) {
      case Cop_Add: VG_(printf)("+"); break;
      case Cop_Sub: VG_(printf)("-"); break;
      case Cop_And: VG_(printf)("&"); break;
      case Cop_Mul: VG_(printf)("*"); break;
      case Cop_Shl: VG_(printf)("<<"); break;
      case Cop_Shr: VG_(printf)(">>"); break;
      case Cop_Eq: VG_(printf)("=="); break;
      case Cop_Ge: VG_(printf)(">="); break;
      case Cop_Gt: VG_(printf)(">"); break;
      case Cop_Le: VG_(printf)("<="); break;
      case Cop_Lt: VG_(printf)("<"); break;
      case Cop_Ne: VG_(printf)("!="); break;
      default:      vg_assert(0);
   }
}

static void ppCfiReg ( CfiReg reg )
{
   switch (reg) {
      case Creg_IA_SP:   VG_(printf)("xSP"); break;
      case Creg_IA_BP:   VG_(printf)("xBP"); break;
      case Creg_IA_IP:   VG_(printf)("xIP"); break;
      case Creg_ARM_R13: VG_(printf)("R13"); break;
      case Creg_ARM_R12: VG_(printf)("R12"); break;
      case Creg_ARM_R15: VG_(printf)("R15"); break;
      case Creg_ARM_R14: VG_(printf)("R14"); break;
      case Creg_MIPS_RA: VG_(printf)("RA"); break;
      case Creg_S390_R14: VG_(printf)("R14"); break;
      default: vg_assert(0);
   }
}

void ML_(ppCfiExpr)( XArray* src, Int ix )
{
   CfiExpr* e = (CfiExpr*) VG_(indexXA)( src, ix );
   switch (e->tag) {
      case Cex_Undef: 
         VG_(printf)("Undef"); 
         break;
      case Cex_Deref: 
         VG_(printf)("*("); 
         ML_(ppCfiExpr)(src, e->Cex.Deref.ixAddr);
         VG_(printf)(")"); 
         break;
      case Cex_Const: 
         VG_(printf)("0x%lx", e->Cex.Const.con); 
         break;
      case Cex_Binop: 
         VG_(printf)("(");
         ML_(ppCfiExpr)(src, e->Cex.Binop.ixL);
         VG_(printf)(")");
         ppCfiOp(e->Cex.Binop.op);
         VG_(printf)("(");
         ML_(ppCfiExpr)(src, e->Cex.Binop.ixR);
         VG_(printf)(")");
         break;
      case Cex_CfiReg:
         ppCfiReg(e->Cex.CfiReg.reg);
         break;
      case Cex_DwReg:
         VG_(printf)("dwr%d", e->Cex.DwReg.reg);
         break;
      default: 
         VG_(core_panic)("ML_(ppCfiExpr)"); 
         
         break;
   }
}


Word ML_(cmp_for_DiAddrRange_range) ( const void* keyV,
                                      const void* elemV ) {
   const Addr* key = (const Addr*)keyV;
   const DiAddrRange* elem = (const DiAddrRange*)elemV;
   if (0)
      VG_(printf)("cmp_for_DiAddrRange_range: %#lx vs %#lx\n",
                  *key, elem->aMin);
   if ((*key) < elem->aMin) return -1;
   if ((*key) > elem->aMax) return 1;
   return 0;
}

static
void show_scope ( OSet*  scope, HChar* who )
{
   DiAddrRange* range;
   VG_(printf)("Scope \"%s\" = {\n", who);
   VG_(OSetGen_ResetIter)( scope );
   while (True) {
      range = VG_(OSetGen_Next)( scope );
      if (!range) break;
      VG_(printf)("   %#lx .. %#lx: %lu vars\n", range->aMin, range->aMax,
                  range->vars ? VG_(sizeXA)(range->vars) : 0);
   }
   VG_(printf)("}\n");
}

static void add_var_to_arange ( 
               OSet*  scope,
               Addr aMin, 
               Addr aMax,
               DiVariable* var
            )
{
   DiAddrRange *first, *last, *range;
   DiAddrRange *xxRangep, *xxFirst, *xxLast;
   UWord       xxIters;

   vg_assert(aMin <= aMax);

   if (0) VG_(printf)("add_var_to_arange: %#lx .. %#lx\n", aMin, aMax);
   if (0) show_scope( scope, "add_var_to_arange(1)" );

   first = VG_(OSetGen_Lookup)( scope, &aMin );
   vg_assert(first);
   vg_assert(first->aMin <= first->aMax);
   vg_assert(first->aMin <= aMin && aMin <= first->aMax);

   if (first->aMin == aMin && first->aMax == aMax) {
      vg_assert(first->vars);
      VG_(addToXA)( first->vars, var );
      return;
   }

   if (first->aMin < aMin) {
      DiAddrRange* nyu;
      
      
      Addr tmp = first->aMax;
      first->aMax = aMin-1;
      vg_assert(first->aMin <= first->aMax);
      
      nyu = VG_(OSetGen_AllocNode)( scope, sizeof(DiAddrRange) );
      vg_assert(nyu);
      nyu->aMin = aMin;
      nyu->aMax = tmp;
      vg_assert(nyu->aMin <= nyu->aMax);
      
      vg_assert(first->vars);
      nyu->vars = VG_(cloneXA)( "di.storage.avta.1", first->vars );
      vg_assert(nyu->vars);
      VG_(OSetGen_Insert)( scope, nyu );
      first = nyu;
   }

   vg_assert(first->aMin == aMin);

   last = VG_(OSetGen_Lookup)( scope, &aMax );
   vg_assert(last->aMin <= last->aMax);
   vg_assert(last->aMin <= aMax && aMax <= last->aMax);

   if (aMax < last->aMax) {
      DiAddrRange* nyu;
      
      
      Addr tmp = last->aMin;
      last->aMin = aMax+1;
      vg_assert(last->aMin <= last->aMax);
      
      nyu = VG_(OSetGen_AllocNode)( scope, sizeof(DiAddrRange) );
      vg_assert(nyu);
      nyu->aMin = tmp;
      nyu->aMax = aMax;
      vg_assert(nyu->aMin <= nyu->aMax);
      
      vg_assert(last->vars);
      nyu->vars = VG_(cloneXA)( "di.storage.avta.2", last->vars );
      vg_assert(nyu->vars);
      VG_(OSetGen_Insert)( scope, nyu );
      last = nyu;
   }

   vg_assert(aMax == last->aMax);

   xxFirst = (DiAddrRange*)VG_(OSetGen_Lookup)(scope, &aMin);
   xxLast  = (DiAddrRange*)VG_(OSetGen_Lookup)(scope, &aMax);
   vg_assert(xxFirst);
   vg_assert(xxLast);
   vg_assert(xxFirst->aMin == aMin);
   vg_assert(xxLast->aMax == aMax);
   if (xxFirst != xxLast)
      vg_assert(xxFirst->aMax < xxLast->aMin);

   if (0) {
      static UWord ctr = 0;
      ctr++;
      VG_(printf)("ctr = %lu\n", ctr);
      if (ctr >= 33263) show_scope( scope, "add_var_to_arange(2)" );
   }

   xxIters = 0;
   range = xxRangep = NULL;
   VG_(OSetGen_ResetIterAt)( scope, &aMin );
   while (True) {
      xxRangep = range;
      range    = VG_(OSetGen_Next)( scope );
      if (!range) break;
      if (range->aMin > aMax) break;
      xxIters++;
      if (0) VG_(printf)("have range %#lx %#lx\n",
                         range->aMin, range->aMax);

      
      if (!xxRangep) {
         
         vg_assert(range->aMin == aMin);
      } else {
         vg_assert(xxRangep->aMax + 1 == range->aMin);
      }

      vg_assert(range->vars);
      VG_(addToXA)( range->vars, var );
   }
   
   vg_assert(xxIters >= 1);
   if (xxIters == 1) vg_assert(xxFirst == xxLast);
   if (xxFirst == xxLast) vg_assert(xxIters == 1);
   vg_assert(xxRangep);
   vg_assert(xxRangep->aMax == aMax);
   vg_assert(xxRangep == xxLast);
}


void ML_(addVar)( struct _DebugInfo* di,
                  Int    level,
                  Addr   aMin,
                  Addr   aMax,
                  UChar* name, 
                  UWord  typeR, 
                  GExpr* gexpr,
                  GExpr* fbGX,
                  UChar* fileName, 
                  Int    lineNo, 
                  Bool   show )
{
   OSet*  scope;
   DiVariable var;
   Bool       all;
   TyEnt*     ent;
   MaybeULong mul;
   HChar*     badness;

   tl_assert(di && di->admin_tyents);

   if (0) {
      VG_(printf)("  ML_(addVar): level %d  %#lx-%#lx  %s :: ",
                  level, aMin, aMax, name );
      ML_(pp_TyEnt_C_ishly)( di->admin_tyents, typeR );
      VG_(printf)("\n  Var=");
      ML_(pp_GX)(gexpr);
      VG_(printf)("\n");
      if (fbGX) {
         VG_(printf)("  FrB=");
         ML_(pp_GX)( fbGX );
         VG_(printf)("\n");
      } else {
         VG_(printf)("  FrB=none\n");
      }
      VG_(printf)("\n");
   }

   vg_assert(level >= 0);
   vg_assert(aMin <= aMax);
   vg_assert(name);
   vg_assert(gexpr);

   ent = ML_(TyEnts__index_by_cuOff)( di->admin_tyents, NULL, typeR);
   tl_assert(ent);
   vg_assert(ML_(TyEnt__is_type)(ent));

   vg_assert(di->fsm.have_rx_map && di->fsm.have_rw_map);
   if (level > 0 && ML_(find_rx_mapping)(di, aMin, aMax) == NULL) {
      if (VG_(clo_verbosity) >= 0) {
         VG_(message)(Vg_DebugMsg, 
            "warning: addVar: in range %#lx .. %#lx outside "
            "all rx mapped areas (%s)\n",
            aMin, aMax, name
         );
      }
      return;
   }

   mul = ML_(sizeOfType)(di->admin_tyents, typeR);

   badness = NULL;
   if (mul.b != True) 
      badness = "unknown size";
   else if (mul.ul == 0)
      badness = "zero size   ";
   else if (sizeof(void*) == 4 && mul.ul >= (1ULL<<32))
      badness = "implausibly large";

   if (badness) {
      static Int complaints = 10;
      if (VG_(clo_verbosity) >= 2 && complaints > 0) {
         VG_(message)(Vg_DebugMsg, "warning: addVar: %s (%s)\n",
                                   badness, name );
         complaints--;
      }
      return;
   }

   if (!di->varinfo) {
      di->varinfo = VG_(newXA)( ML_(dinfo_zalloc), 
                                "di.storage.addVar.1",
                                ML_(dinfo_free),
                                sizeof(OSet*) );
   }

   vg_assert(level < 256); 
   
   while ( VG_(sizeXA)(di->varinfo) <= level ) {
      DiAddrRange* nyu;
      scope = VG_(OSetGen_Create)( offsetof(DiAddrRange,aMin), 
                                   ML_(cmp_for_DiAddrRange_range),
                                   ML_(dinfo_zalloc), "di.storage.addVar.2",
                                   ML_(dinfo_free) );
      vg_assert(scope);
      if (0) VG_(printf)("create: scope = %p, adding at %ld\n",
                         scope, VG_(sizeXA)(di->varinfo));
      VG_(addToXA)( di->varinfo, &scope );
      nyu = VG_(OSetGen_AllocNode)( scope, sizeof(DiAddrRange) );
      vg_assert(nyu);
      nyu->aMin = (Addr)0;
      nyu->aMax = ~(Addr)0;
      nyu->vars = VG_(newXA)( ML_(dinfo_zalloc), "di.storage.addVar.3",
                              ML_(dinfo_free),
                              sizeof(DiVariable) );
      vg_assert(nyu->vars);
      VG_(OSetGen_Insert)( scope, nyu );
   }

   vg_assert( VG_(sizeXA)(di->varinfo) > level );
   scope = *(OSet**)VG_(indexXA)( di->varinfo, level );
   vg_assert(scope);

   var.name     = name;
   var.typeR    = typeR;
   var.gexpr    = gexpr;
   var.fbGX     = fbGX;
   var.fileName = fileName;
   var.lineNo   = lineNo;

   all = aMin == (Addr)0 && aMax == ~(Addr)0;
   vg_assert(level == 0 ? all : !all);

   add_var_to_arange( scope, aMin, aMax, &var );
}


static void canonicaliseVarInfo ( struct _DebugInfo* di )
{
   Word i, nInThisScope;

   if (!di->varinfo)
      return;

   for (i = 0; i < VG_(sizeXA)(di->varinfo); i++) {

      DiAddrRange *range, *rangep;
      OSet* scope = *(OSet**)VG_(indexXA)(di->varinfo, i);
      if (!scope) continue;

      
      if (i == 0) {
         Addr zero = 0;
         vg_assert(VG_(OSetGen_Size)( scope ) == 1);
         range = VG_(OSetGen_Lookup)( scope, &zero );
         vg_assert(range);
         vg_assert(range->aMin == (Addr)0);
         vg_assert(range->aMax == ~(Addr)0);
         continue;
      }

      
      
      nInThisScope = 0;
      rangep = NULL;
      VG_(OSetGen_ResetIter)(scope);
      while (True) {
         range = VG_(OSetGen_Next)(scope);
         if (!range) {
           vg_assert(rangep);
           vg_assert(rangep->aMax == ~(Addr)0);
           break;
         }

         vg_assert(range->aMin <= range->aMax);
         vg_assert(range->vars);

         if (!rangep) {
           
           vg_assert(range->aMin == 0);
         } else {
           vg_assert(rangep->aMax + 1 == range->aMin);
         }

         rangep = range;
         nInThisScope++;
      } 


      vg_assert(nInThisScope > 0);
      if (nInThisScope == 1) {
         Addr zero = 0;
         vg_assert(VG_(OSetGen_Size)( scope ) == 1);
         range = VG_(OSetGen_Lookup)( scope, &zero );
         vg_assert(range);
         vg_assert(range->aMin == (Addr)0);
         vg_assert(range->aMax == ~(Addr)0);
         vg_assert(range->vars);
         vg_assert(VG_(sizeXA)(range->vars) == 0);
      }

   } 
}



static Int compare_DiSym ( void* va, void* vb ) 
{
   DiSym* a = (DiSym*)va;
   DiSym* b = (DiSym*)vb;
   if (a->addr < b->addr) return -1;
   if (a->addr > b->addr) return  1;
   return 0;
}


static
Bool preferName ( struct _DebugInfo* di,
                  UChar* a_name, UChar* b_name,
                  Addr sym_avma )
{
   Word cmp;
   Word vlena, vlenb;		
   const UChar *vpa, *vpb;

   Bool preferA = False;
   Bool preferB = False;

   vg_assert(a_name);
   vg_assert(b_name);
   vg_assert(a_name != b_name);

   vlena = VG_(strlen)(a_name);
   vlenb = VG_(strlen)(b_name);

#  if defined(VGO_linux)
#    define VERSION_CHAR '@'
#  elif defined(VGO_darwin)
#    define VERSION_CHAR '$'
#  else
#    error Unknown OS
#  endif

   vpa = VG_(strchr)(a_name, VERSION_CHAR);
   vpb = VG_(strchr)(b_name, VERSION_CHAR);

#  undef VERSION_CHAR

   if (vpa)
      vlena = vpa - a_name;
   if (vpb)
      vlenb = vpb - b_name;

   
   if (0==VG_(strncmp)(a_name, "MPI_", 4)
       && 0==VG_(strncmp)(b_name, "PMPI_", 5)
       && 0==VG_(strcmp)(a_name, 1+b_name)) {
      preferB = True; goto out;
   } 
   if (0==VG_(strncmp)(b_name, "MPI_", 4)
       && 0==VG_(strncmp)(a_name, "PMPI_", 5)
       && 0==VG_(strcmp)(b_name, 1+a_name)) {
      preferA = True; goto out;
   }

   
   if (vlena  &&  !vlenb) {
      preferA = True; goto out;
   }
   if (vlenb  &&  !vlena) {
      preferB = True; goto out;
   }

   
   {
      Bool blankA = True;
      Bool blankB = True;
      Char *s;
      s = a_name;
      while (*s) {
         if (!VG_(isspace)(*s++)) {
            blankA = False;
            break;
         }
      }
      s = b_name;
      while (*s) {
         if (!VG_(isspace)(*s++)) {
            blankB = False;
            break;
         }
      }

      if (!blankA  &&  blankB) {
         preferA = True; goto out;
      }
      if (!blankB  &&  blankA) {
         preferB = True; goto out;
      }
   }

   
   if (vlena < vlenb) {
      preferA = True; goto out;
   } 
   if (vlenb < vlena) {
      preferB = True; goto out;
   }

   
   if (vpa && !vpb) {
      preferA = True; goto out;
   }
   if (vpb && !vpa) {
      preferB = True; goto out;
   }

   cmp = VG_(strcmp)(a_name, b_name);
   if (cmp < 0) {
      preferA = True; goto out;
   }
   if (cmp > 0) {
      preferB = True; goto out;
   }

   

   return a_name <= b_name  ? True  : False;

   
   vg_assert(0);
  out:
   if (preferA && !preferB) {
      TRACE_SYMTAB("sym at %#lx: prefer '%s' to '%s'\n",
                   sym_avma, a_name, b_name );
      return True;
   }
   if (preferB && !preferA) {
      TRACE_SYMTAB("sym at %#lx: prefer '%s' to '%s'\n",
                   sym_avma, b_name, a_name );
      return False;
   }
   
   vg_assert(0);
}


static
void add_DiSym_names_to_from ( DebugInfo* di, DiSym* to, DiSym* from )
{
   vg_assert(to->pri_name);
   vg_assert(from->pri_name);
   UChar** to_sec   = to->sec_names;
   UChar** from_sec = from->sec_names;
   Word n_new_sec = 1;
   if (from_sec) {
      while (*from_sec) {
         n_new_sec++;
         from_sec++;
      }
   }
   if (to_sec) {
      while (*to_sec) {
         n_new_sec++;
         to_sec++;
      }
   }
   if (0)
      TRACE_SYMTAB("merge: -> %ld\n", n_new_sec);
   UChar** new_sec = ML_(dinfo_zalloc)( "di.storage.aDntf.1",
                                        (n_new_sec+1) * sizeof(UChar*) );
   from_sec = from->sec_names;
   to_sec   = to->sec_names;
   Word i = 0;
   if (to_sec) {
      while (*to_sec) {
         new_sec[i++] = *to_sec;
         to_sec++;
      }
   }
   new_sec[i++] = from->pri_name;
   if (from_sec) {
      while (*from_sec) {
         new_sec[i++] = *from_sec;
         from_sec++;
      }
   }
   vg_assert(i == n_new_sec);
   vg_assert(new_sec[i] == NULL);
   
   if (to->sec_names) {
      ML_(dinfo_free)(to->sec_names);
   }
   to->sec_names = new_sec;
}


static void canonicaliseSymtab ( struct _DebugInfo* di )
{
   Word  i, j, n_truncated;
   Addr  sta1, sta2, end1, end2, toc1, toc2;
   UChar *pri1, *pri2, **sec1, **sec2;
   Bool  ist1, ist2, isf1, isf2;

#  define SWAP(ty,aa,bb) \
      do { ty tt = (aa); (aa) = (bb); (bb) = tt; } while (0)

   if (di->symtab_used == 0)
      return;

   
   for (i = 0; i < di->symtab_used; i++) {
      DiSym* sym = &di->symtab[i];
      vg_assert(sym->pri_name);
      vg_assert(!sym->sec_names);
   }

   
   VG_(ssort)(di->symtab, di->symtab_used, 
                          sizeof(*di->symtab), compare_DiSym);

  cleanup_more:
 
   while (1) {
      Word r, w, n_merged;
      n_merged = 0;
      w = 0;
      
      for (r = 1; r < di->symtab_used; r++) {
         vg_assert(w < r);
         if (   di->symtab[w].addr      == di->symtab[r].addr
             && di->symtab[w].size      == di->symtab[r].size
             && !!di->symtab[w].isText  == !!di->symtab[r].isText) {
            
            n_merged++;
            if (di->symtab[r].sec_names 
                || (0 != VG_(strcmp)(di->symtab[r].pri_name,
                                     di->symtab[w].pri_name))) {
               add_DiSym_names_to_from(di, &di->symtab[w], &di->symtab[r]);
            }
            
            di->symtab[w].isIFunc = di->symtab[w].isIFunc || di->symtab[r].isIFunc;
            
            di->symtab[r].pri_name = NULL;
            if (di->symtab[r].sec_names) {
               ML_(dinfo_free)(di->symtab[r].sec_names);
               di->symtab[r].sec_names = NULL;
            }
            VG_(memset)(&di->symtab[r], 0, sizeof(DiSym));
         } else {
            w = r;
         }
      }
      TRACE_SYMTAB( "canonicaliseSymtab: %ld symbols merged\n", n_merged);
      if (n_merged == 0)
         break;
      
      w = 0;
      for (r = 0; r < di->symtab_used; r++) {
         vg_assert(w <= r);
         if (di->symtab[r].pri_name == NULL)
            continue;
         if (w < r) {
            di->symtab[w] = di->symtab[r];
         }
         w++;
      }
      vg_assert(w + n_merged == di->symtab_used);
      di->symtab_used = w;
   }

   
   n_truncated = 0;

   for (i = 0; i < ((Word)di->symtab_used) -1; i++) {

      vg_assert(di->symtab[i].addr <= di->symtab[i+1].addr);

       
      if (di->symtab[i].addr + di->symtab[i].size 
          <= di->symtab[i+1].addr)
         continue;

      
      if (di->trace_symtab) {
         VG_(printf)("overlapping address ranges in symbol table\n\t");
         ML_(ppSym)( i, &di->symtab[i] );
         VG_(printf)("\t");
         ML_(ppSym)( i+1, &di->symtab[i+1] );
         VG_(printf)("\n");
      }

      
      sta1 = di->symtab[i].addr;
      end1 = sta1 + di->symtab[i].size - 1;
      toc1 = di->symtab[i].tocptr;
      pri1 = di->symtab[i].pri_name;
      sec1 = di->symtab[i].sec_names;
      ist1 = di->symtab[i].isText;
      isf1 = di->symtab[i].isIFunc;

      sta2 = di->symtab[i+1].addr;
      end2 = sta2 + di->symtab[i+1].size - 1;
      toc2 = di->symtab[i+1].tocptr;
      pri2 = di->symtab[i+1].pri_name;
      sec2 = di->symtab[i+1].sec_names;
      ist2 = di->symtab[i+1].isText;
      isf2 = di->symtab[i+1].isIFunc;

      if (sta1 < sta2) {
         end1 = sta2 - 1;
      } else {
         vg_assert(sta1 == sta2);
         if (end1 > end2) { 
            sta1 = end2 + 1;
            SWAP(Addr,sta1,sta2); SWAP(Addr,end1,end2); SWAP(Addr,toc1,toc2);
            SWAP(UChar*,pri1,pri2); SWAP(UChar**,sec1,sec2);
            SWAP(Bool,ist1,ist2); SWAP(Bool,isf1,isf2);
         } else 
         if (end1 < end2) {
            sta2 = end1 + 1;
         } else {
	 }
      }
      di->symtab[i].addr      = sta1;
      di->symtab[i].size      = end1 - sta1 + 1;
      di->symtab[i].tocptr    = toc1;
      di->symtab[i].pri_name  = pri1;
      di->symtab[i].sec_names = sec1;
      di->symtab[i].isText    = ist1;
      di->symtab[i].isIFunc   = isf1;

      di->symtab[i+1].addr      = sta2;
      di->symtab[i+1].size      = end2 - sta2 + 1;
      di->symtab[i+1].tocptr    = toc2;
      di->symtab[i+1].pri_name  = pri2;
      di->symtab[i+1].sec_names = sec2;
      di->symtab[i+1].isText    = ist2;
      di->symtab[i+1].isIFunc   = isf2;

      vg_assert(sta1 <= sta2);
      vg_assert(di->symtab[i].size > 0);
      vg_assert(di->symtab[i+1].size > 0);
      j = i+1;
      while (j < ((Word)di->symtab_used)-1 
             && di->symtab[j].addr > di->symtab[j+1].addr) {
         SWAP(DiSym,di->symtab[j],di->symtab[j+1]);
         j++;
      }
      n_truncated++;
   }

   if (n_truncated > 0) goto cleanup_more;

   
   for (i = 0; i < ((Word)di->symtab_used)-1; i++) {
      
      vg_assert(di->symtab[i].size > 0);
      
      vg_assert(di->symtab[i].addr < di->symtab[i+1].addr);
      
      vg_assert(di->symtab[i].addr + di->symtab[i].size - 1
                < di->symtab[i+1].addr);
      
      vg_assert(di->symtab[i].pri_name);
      if (di->symtab[i].sec_names) {
         vg_assert(di->symtab[i].sec_names[0]);
      }
   }

   for (i = 0; i < ((Word)di->symtab_used)-1; i++) {
      DiSym*  sym = &di->symtab[i];
      UChar** sec = sym->sec_names;
      if (!sec)
         continue;
      Word n_tmp = 1;
      while (*sec) { n_tmp++; sec++; }
      j = 0;
      UChar** tmp = ML_(dinfo_zalloc)( "di.storage.cS.1",
                                       (n_tmp+1) * sizeof(UChar*) );
      tmp[j++] = sym->pri_name;
      sec = sym->sec_names;
      while (*sec) { tmp[j++] = *sec; sec++; }
      vg_assert(j == n_tmp);
      vg_assert(tmp[n_tmp] == NULL); 
      
      Word best = 0;
      for (j = 1; j < n_tmp; j++) {
         if (preferName(di, tmp[best], tmp[j], di->symtab[i].addr)) {
            
         } else {
            best = j;
         }
      }
      vg_assert(best >= 0 && best < n_tmp);
      
      sym->pri_name = tmp[best];
      UChar** cursor = sym->sec_names;
      for (j = 0; j < n_tmp; j++) {
         if (j == best)
            continue;
         *cursor = tmp[j];
         cursor++;
      }
      vg_assert(*cursor == NULL);
      ML_(dinfo_free)( tmp );
   }

#  undef SWAP
}


static Int compare_DiLoc ( void* va, void* vb ) 
{
   DiLoc* a = (DiLoc*)va;
   DiLoc* b = (DiLoc*)vb;
   if (a->addr < b->addr) return -1;
   if (a->addr > b->addr) return  1;
   return 0;
}

static void canonicaliseLoctab ( struct _DebugInfo* di )
{
   Word i, j;

#  define SWAP(ty,aa,bb) \
      do { ty tt = (aa); (aa) = (bb); (bb) = tt; } while (0);

   if (di->loctab_used == 0)
      return;

   
   VG_(ssort)(di->loctab, di->loctab_used, 
                          sizeof(*di->loctab), compare_DiLoc);

   
   for (i = 0; i < ((Word)di->loctab_used)-1; i++) {
      vg_assert(di->loctab[i].size < 10000);
      if (di->loctab[i].addr + di->loctab[i].size > di->loctab[i+1].addr) {
         Int new_size = di->loctab[i+1].addr - di->loctab[i].addr;
         if (new_size < 0) {
            di->loctab[i].size = 0;
         } else
         if (new_size > MAX_LOC_SIZE) {
           di->loctab[i].size = MAX_LOC_SIZE;
         } else {
           di->loctab[i].size = (UShort)new_size;
         }
      }
   }

   j = 0;
   for (i = 0; i < (Word)di->loctab_used; i++) {
      if (di->loctab[i].size > 0) {
         if (j != i)
            di->loctab[j] = di->loctab[i];
         j++;
      }
   }
   di->loctab_used = j;

   
   for (i = 0; i < ((Word)di->loctab_used)-1; i++) {
      
      vg_assert(di->loctab[i].size > 0);
      
      vg_assert(di->loctab[i].addr < di->loctab[i+1].addr);
      
      vg_assert(di->loctab[i].addr + di->loctab[i].size - 1
                < di->loctab[i+1].addr);
   }
#  undef SWAP

   
   shrinkLocTab(di);
}


static Int compare_DiCfSI ( void* va, void* vb )
{
   DiCfSI* a = (DiCfSI*)va;
   DiCfSI* b = (DiCfSI*)vb;
   if (a->base < b->base) return -1;
   if (a->base > b->base) return  1;
   return 0;
}

void ML_(canonicaliseCFI) ( struct _DebugInfo* di )
{
   Word  i, j;
   const Addr minAvma = 0;
   const Addr maxAvma = ~minAvma;

   if (di->cfsi == NULL) {
      vg_assert(di->cfsi_used == 0);
      vg_assert(di->cfsi_size == 0);
   }

   di->cfsi_minavma = maxAvma; 
   di->cfsi_maxavma = minAvma;
   for (i = 0; i < (Word)di->cfsi_used; i++) {
      Addr here_min = di->cfsi[i].base;
      Addr here_max = di->cfsi[i].base + di->cfsi[i].len - 1;
      if (here_min < di->cfsi_minavma)
         di->cfsi_minavma = here_min;
      if (here_max > di->cfsi_maxavma)
         di->cfsi_maxavma = here_max;
   }

   if (di->trace_cfi)
      VG_(printf)("canonicaliseCfiSI: %ld entries, %#lx .. %#lx\n",
                  di->cfsi_used,
	          di->cfsi_minavma, di->cfsi_maxavma);

   
   VG_(ssort)(di->cfsi, di->cfsi_used, sizeof(*di->cfsi), compare_DiCfSI);

   
   for (i = 0; i < (Word)di->cfsi_used-1; i++) {
      if (di->cfsi[i].base + di->cfsi[i].len > di->cfsi[i+1].base) {
         Word new_len = di->cfsi[i+1].base - di->cfsi[i].base;
         
         vg_assert(new_len >= 0);
	 vg_assert(new_len <= di->cfsi[i].len);
         di->cfsi[i].len = new_len;
      }
   }

   j = 0;
   for (i = 0; i < (Word)di->cfsi_used; i++) {
      if (di->cfsi[i].len > 0) {
         if (j != i)
            di->cfsi[j] = di->cfsi[i];
         j++;
      }
   }
   
   di->cfsi_used = j;

   
   for (i = 0; i < (Word)di->cfsi_used; i++) {
      
      vg_assert(di->cfsi[i].len > 0);
      
      vg_assert(di->cfsi[i].base >= di->cfsi_minavma);
      vg_assert(di->cfsi[i].base + di->cfsi[i].len - 1
                <= di->cfsi_maxavma);

      if (i < di->cfsi_used - 1) {
         
         vg_assert(di->cfsi[i].base < di->cfsi[i+1].base);
         
         vg_assert(di->cfsi[i].base + di->cfsi[i].len - 1
                   < di->cfsi[i+1].base);
      }
   }

}


void ML_(canonicaliseTables) ( struct _DebugInfo* di )
{
   canonicaliseSymtab ( di );
   canonicaliseLoctab ( di );
   ML_(canonicaliseCFI) ( di );
   canonicaliseVarInfo ( di );
}




Word ML_(search_one_symtab) ( struct _DebugInfo* di, Addr ptr,
                              Bool match_anywhere_in_sym,
                              Bool findText )
{
   Addr a_mid_lo, a_mid_hi;
   Word mid, size, 
        lo = 0, 
        hi = di->symtab_used-1;
   while (True) {
      
      if (lo > hi) return -1; 
      mid      = (lo + hi) / 2;
      a_mid_lo = di->symtab[mid].addr;
      size = ( match_anywhere_in_sym
             ? di->symtab[mid].size
             : 1);
      a_mid_hi = ((Addr)di->symtab[mid].addr) + size - 1;

      if (ptr < a_mid_lo) { hi = mid-1; continue; } 
      if (ptr > a_mid_hi) { lo = mid+1; continue; }
      vg_assert(ptr >= a_mid_lo && ptr <= a_mid_hi);
      if (  findText   &&   di->symtab[mid].isText  ) return mid;
      if ( (!findText) && (!di->symtab[mid].isText) ) return mid;
      return -1;
   }
}



Word ML_(search_one_loctab) ( struct _DebugInfo* di, Addr ptr )
{
   Addr a_mid_lo, a_mid_hi;
   Word mid, 
        lo = 0, 
        hi = di->loctab_used-1;
   while (True) {
      
      if (lo > hi) return -1; 
      mid      = (lo + hi) / 2;
      a_mid_lo = di->loctab[mid].addr;
      a_mid_hi = ((Addr)di->loctab[mid].addr) + di->loctab[mid].size - 1;

      if (ptr < a_mid_lo) { hi = mid-1; continue; } 
      if (ptr > a_mid_hi) { lo = mid+1; continue; }
      vg_assert(ptr >= a_mid_lo && ptr <= a_mid_hi);
      return mid;
   }
}



Word ML_(search_one_cfitab) ( struct _DebugInfo* di, Addr ptr )
{
   Addr a_mid_lo, a_mid_hi;
   Word mid, size, 
        lo = 0, 
        hi = di->cfsi_used-1;
   while (True) {
      
      if (lo > hi) return -1; 
      mid      = (lo + hi) / 2;
      a_mid_lo = di->cfsi[mid].base;
      size     = di->cfsi[mid].len;
      a_mid_hi = a_mid_lo + size - 1;
      vg_assert(a_mid_hi >= a_mid_lo);
      if (ptr < a_mid_lo) { hi = mid-1; continue; } 
      if (ptr > a_mid_hi) { lo = mid+1; continue; }
      vg_assert(ptr >= a_mid_lo && ptr <= a_mid_hi);
      return mid;
   }
}



Word ML_(search_one_fpotab) ( struct _DebugInfo* di, Addr ptr )
{
   Addr const addr = ptr - di->fpo_base_avma;
   Addr a_mid_lo, a_mid_hi;
   Word mid, size,
        lo = 0,
        hi = di->fpo_size-1;
   while (True) {
      
      if (lo > hi) return -1; 
      mid      = (lo + hi) / 2;
      a_mid_lo = di->fpo[mid].ulOffStart;
      size     = di->fpo[mid].cbProcSize;
      a_mid_hi = a_mid_lo + size - 1;
      vg_assert(a_mid_hi >= a_mid_lo);
      if (addr < a_mid_lo) { hi = mid-1; continue; }
      if (addr > a_mid_hi) { lo = mid+1; continue; }
      vg_assert(addr >= a_mid_lo && addr <= a_mid_hi);
      return mid;
   }
}

