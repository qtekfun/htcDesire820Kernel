

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
#include "pub_core_debuglog.h"
#include "pub_core_machine.h"    
#include "pub_core_libcbase.h"
#include "pub_core_vki.h"        
#include "pub_core_libcproc.h"   
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_options.h"
#include "pub_core_tooliface.h"  
#include "pub_core_transtab.h"
#include "pub_core_aspacemgr.h"
#include "pub_core_mallocfree.h" 
#include "pub_core_xarray.h"
#include "pub_core_dispatch.h"   






#define N_SECTORS 8

#define N_TTES_PER_SECTOR    65521

#define SECTOR_TT_LIMIT_PERCENT 65

#define N_TTES_PER_SECTOR_USABLE \
           ((N_TTES_PER_SECTOR * SECTOR_TT_LIMIT_PERCENT) / 100)

#define ECLASS_SHIFT 11
#define ECLASS_WIDTH 8
#define ECLASS_MISC  (1 << ECLASS_WIDTH)
#define ECLASS_N     (1 + ECLASS_MISC)

#define EC2TTE_DELETED  0xFFFF 



typedef
   struct {
      UInt from_sNo;   
      UInt from_tteNo; 
      UInt from_offs;  
      Bool to_fastEP;  
   }
   InEdge;


typedef
   struct {
      UInt to_sNo;    
      UInt to_tteNo;  
      UInt from_offs; 
   }
   OutEdge;


#define N_FIXED_IN_EDGE_ARR 3
typedef
   struct {
      UInt     n_fixed; 
      InEdge   fixed[N_FIXED_IN_EDGE_ARR];
      XArray*  var; 
   }
   InEdgeArr;

#define N_FIXED_OUT_EDGE_ARR 2
typedef
   struct {
      UInt    n_fixed; 
      OutEdge fixed[N_FIXED_OUT_EDGE_ARR];
      XArray* var; 
   }
   OutEdgeArr;


typedef
   struct {
      ULong    count;
      UShort   weight;

      enum { InUse, Deleted, Empty } status;

      ULong* tcptr;

      Addr64 entry;

      VexGuestExtents vge;

      UShort n_tte2ec;      
      UShort tte2ec_ec[3];  
      UInt   tte2ec_ix[3];  
      
      
      
      


      /* A translation can disappear for two reasons:
          1. erased (as part of the oldest sector cleanup) when the
             youngest sector is full.
          2. discarded due to calls to VG_(discard_translations).
             VG_(discard_translations) sets the status of the
             translation to 'Deleted'.
             A.o., the gdbserver discards one or more translations
             when a breakpoint is inserted or removed at an Addr,
             or when single stepping mode is enabled/disabled
             or when a translation is instrumented for gdbserver
             (all the target jumps of this translation are
              invalidated).

         So, it is possible that the translation A to be patched
         (to obtain a patched jump from A to B) is invalidated
         after B is translated and before A is patched.
         In case a translation is erased or discarded, the patching
         cannot be done.  VG_(tt_tc_do_chaining) and find_TTEntry_from_hcode
         are checking the 'from' translation still exists before
         doing the patching.

         Is it safe to erase or discard the current translation E being 
         executed ? Amazing, but yes, it is safe.
         Here is the explanation:

         The translation E being executed can only be erased if a new
         translation N is being done. A new translation is done only
         if the host addr is a not yet patched jump to another
         translation. In such a case, the guest address of N is
         assigned to the PC in the VEX state. Control is returned
         to the scheduler. N will be translated. This can erase the
         translation E (in case of sector full). VG_(tt_tc_do_chaining)
         will not do the chaining to a non found translation E.
         The execution will continue at the current guest PC
         (i.e. the translation N).
         => it is safe to erase the current translation being executed.
         
         The current translation E being executed can also be discarded
         (e.g. by gdbserver). VG_(discard_translations) will mark
         this translation E as Deleted, but the translation itself
         is not erased. In particular, its host code can only
         be overwritten or erased in case a new translation is done.
         A new translation will only be done if a not yet translated
         jump is to be executed. The execution of the Deleted translation
         E will continue till a non patched jump is encountered.
         This situation is then similar to the 'erasing' case above :
         the current translation E can be erased or overwritten, as the
         execution will continue at the new translation N.

      */

      InEdgeArr  in_edges;
      OutEdgeArr out_edges;
   }
   TTEntry;


typedef
   struct {
      UChar* start;
      UInt   len;
      UInt   tteNo;
   }
   HostExtent;

typedef
   struct {
      ULong* tc;

      TTEntry* tt;

      
      ULong* tc_next;

      
      Int tt_n_inuse;

      Int     ec2tte_size[ECLASS_N];
      Int     ec2tte_used[ECLASS_N];
      UShort* ec2tte[ECLASS_N];

      XArray* host_extents; 
   }
   Sector;



static Sector sectors[N_SECTORS];
static Int    youngest_sector = -1;

static Int    tc_sector_szQ;


static Int sector_search_order[N_SECTORS];


 __attribute__((aligned(16)))
           FastCacheEntry VG_(tt_fast)[VG_TT_FAST_SIZE];

static Bool init_done = False;



static ULong n_fast_flushes = 0;
static ULong n_fast_updates = 0;

static ULong n_full_lookups = 0;
static ULong n_lookup_probes = 0;

static ULong n_in_count    = 0;
static ULong n_in_osize    = 0;
static ULong n_in_tsize    = 0;
static ULong n_in_sc_count = 0;

static ULong n_dump_count = 0;
static ULong n_dump_osize = 0;

static ULong n_disc_count = 0;
static ULong n_disc_osize = 0;



static void* ttaux_malloc ( HChar* tag, SizeT n )
{
   return VG_(arena_malloc)(VG_AR_TTAUX, tag, n);
}

static void ttaux_free ( void* p )
{
   VG_(arena_free)(VG_AR_TTAUX, p);
}



static inline TTEntry* index_tte ( UInt sNo, UInt tteNo )
{
   vg_assert(sNo < N_SECTORS);
   vg_assert(tteNo < N_TTES_PER_SECTOR);
   Sector* s = &sectors[sNo];
   vg_assert(s->tt);
   TTEntry* tte = &s->tt[tteNo];
   vg_assert(tte->status == InUse);
   return tte;
}

static void InEdge__init ( InEdge* ie )
{
   ie->from_sNo   = -1; 
   ie->from_tteNo = 0;
   ie->from_offs  = 0;
   ie->to_fastEP  = False;
}

static void OutEdge__init ( OutEdge* oe )
{
   oe->to_sNo    = -1; 
   oe->to_tteNo  = 0;
   oe->from_offs = 0;
}

static void TTEntry__init ( TTEntry* tte )
{
   VG_(memset)(tte, 0, sizeof(*tte));
}

static UWord InEdgeArr__size ( InEdgeArr* iea )
{
   if (iea->var) {
      vg_assert(iea->n_fixed == 0);
      return VG_(sizeXA)(iea->var);
   } else {
      vg_assert(iea->n_fixed <= N_FIXED_IN_EDGE_ARR);
      return iea->n_fixed;
   }
}

static void InEdgeArr__makeEmpty ( InEdgeArr* iea )
{
   if (iea->var) {
      vg_assert(iea->n_fixed == 0);
      VG_(deleteXA)(iea->var);
      iea->var = NULL;
   } else {
      vg_assert(iea->n_fixed <= N_FIXED_IN_EDGE_ARR);
      iea->n_fixed = 0;
   }
}

static
InEdge* InEdgeArr__index ( InEdgeArr* iea, UWord i )
{
   if (iea->var) {
      vg_assert(iea->n_fixed == 0);
      return (InEdge*)VG_(indexXA)(iea->var, i);
   } else {
      vg_assert(i < iea->n_fixed);
      return &iea->fixed[i];
   }
}

static
void InEdgeArr__deleteIndex ( InEdgeArr* iea, UWord i )
{
   if (iea->var) {
      vg_assert(iea->n_fixed == 0);
      VG_(removeIndexXA)(iea->var, i);
   } else {
      vg_assert(i < iea->n_fixed);
      for (; i+1 < iea->n_fixed; i++) {
         iea->fixed[i] = iea->fixed[i+1];
      }
      iea->n_fixed--;
   }
}

static
void InEdgeArr__add ( InEdgeArr* iea, InEdge* ie )
{
   if (iea->var) {
      vg_assert(iea->n_fixed == 0);
      VG_(addToXA)(iea->var, ie);
   } else {
      vg_assert(iea->n_fixed <= N_FIXED_IN_EDGE_ARR);
      if (iea->n_fixed == N_FIXED_IN_EDGE_ARR) {
         iea->var = VG_(newXA)(ttaux_malloc, "transtab.IEA__add",
                               ttaux_free,
                               sizeof(InEdge));
         UWord i;
         for (i = 0; i < iea->n_fixed; i++) {
            VG_(addToXA)(iea->var, &iea->fixed[i]);
         }
         VG_(addToXA)(iea->var, ie);
         iea->n_fixed = 0;
      } else {
         
         iea->fixed[iea->n_fixed++] = *ie;
      }
   }
}

static UWord OutEdgeArr__size ( OutEdgeArr* oea )
{
   if (oea->var) {
      vg_assert(oea->n_fixed == 0);
      return VG_(sizeXA)(oea->var);
   } else {
      vg_assert(oea->n_fixed <= N_FIXED_OUT_EDGE_ARR);
      return oea->n_fixed;
   }
}

static void OutEdgeArr__makeEmpty ( OutEdgeArr* oea )
{
   if (oea->var) {
      vg_assert(oea->n_fixed == 0);
      VG_(deleteXA)(oea->var);
      oea->var = NULL;
   } else {
      vg_assert(oea->n_fixed <= N_FIXED_OUT_EDGE_ARR);
      oea->n_fixed = 0;
   }
}

static
OutEdge* OutEdgeArr__index ( OutEdgeArr* oea, UWord i )
{
   if (oea->var) {
      vg_assert(oea->n_fixed == 0);
      return (OutEdge*)VG_(indexXA)(oea->var, i);
   } else {
      vg_assert(i < oea->n_fixed);
      return &oea->fixed[i];
   }
}

static
void OutEdgeArr__deleteIndex ( OutEdgeArr* oea, UWord i )
{
   if (oea->var) {
      vg_assert(oea->n_fixed == 0);
      VG_(removeIndexXA)(oea->var, i);
   } else {
      vg_assert(i < oea->n_fixed);
      for (; i+1 < oea->n_fixed; i++) {
         oea->fixed[i] = oea->fixed[i+1];
      }
      oea->n_fixed--;
   }
}

static
void OutEdgeArr__add ( OutEdgeArr* oea, OutEdge* oe )
{
   if (oea->var) {
      vg_assert(oea->n_fixed == 0);
      VG_(addToXA)(oea->var, oe);
   } else {
      vg_assert(oea->n_fixed <= N_FIXED_OUT_EDGE_ARR);
      if (oea->n_fixed == N_FIXED_OUT_EDGE_ARR) {
         oea->var = VG_(newXA)(ttaux_malloc, "transtab.OEA__add",
                               ttaux_free,
                               sizeof(OutEdge));
         UWord i;
         for (i = 0; i < oea->n_fixed; i++) {
            VG_(addToXA)(oea->var, &oea->fixed[i]);
         }
         VG_(addToXA)(oea->var, oe);
         oea->n_fixed = 0;
      } else {
         
         oea->fixed[oea->n_fixed++] = *oe;
      }
   }
}

static
Int HostExtent__cmpOrd ( void* v1, void* v2 )
{
   HostExtent* hx1 = (HostExtent*)v1;
   HostExtent* hx2 = (HostExtent*)v2;
   if (hx1->start + hx1->len <= hx2->start) return -1;
   if (hx2->start + hx2->len <= hx1->start) return 1;
   return 0; 
}

static __attribute__((noinline))
Bool find_TTEntry_from_hcode( UInt* from_sNo,
                              UInt* from_tteNo,
                              void* hcode )
{
   Int i;

   
   for (i = 0; i < N_SECTORS; i++) {
      Int sno = sector_search_order[i];
      if (UNLIKELY(sno == -1))
         return False; 

      Sector* sec = &sectors[sno];
      XArray*  host_extents = sec->host_extents;
      vg_assert(host_extents);

      HostExtent key;
      VG_(memset)(&key, 0, sizeof(key));
      key.start = hcode;
      key.len = 1;
      Word firstW = -1, lastW = -1;
      Bool found  = VG_(lookupXA_UNSAFE)(
                       host_extents, &key, &firstW, &lastW,
                       (Int(*)(void*,void*))HostExtent__cmpOrd
                    );
      vg_assert(firstW == lastW); 
      if (found) {
         HostExtent* hx = VG_(indexXA)(host_extents, firstW);
         UInt tteNo = hx->tteNo;
         
         vg_assert(tteNo <= N_TTES_PER_SECTOR);
         if (sec->tt[tteNo].status == Deleted)
            return False;
         vg_assert(sec->tt[tteNo].status == InUse);
         vg_assert((UChar*)sec->tt[tteNo].tcptr <= (UChar*)hcode);
         
         *from_sNo   = sno;
         *from_tteNo = (UInt)tteNo;
         return True;
      }
   }
   return False;
}


static Bool is_in_the_main_TC ( void* hcode )
{
   Int i, sno;
   for (i = 0; i < N_SECTORS; i++) {
      sno = sector_search_order[i];
      if (sno == -1)
         break; 
      if ((UChar*)hcode >= (UChar*)sectors[sno].tc
          && (UChar*)hcode <= (UChar*)sectors[sno].tc_next
                              + sizeof(ULong) - 1)
         return True;
   }
   return False;
}


void VG_(tt_tc_do_chaining) ( void* from__patch_addr,
                              UInt  to_sNo,
                              UInt  to_tteNo,
                              Bool  to_fastEP )
{
   
   VexArch vex_arch = VexArch_INVALID;
   VG_(machine_get_VexArchInfo)( &vex_arch, NULL );

   
   
   
   
   
   TTEntry* to_tte    = index_tte(to_sNo, to_tteNo);
   void*    host_code = ((UChar*)to_tte->tcptr)
                        + (to_fastEP ? LibVEX_evCheckSzB(vex_arch) : 0);

   
   vg_assert( (UChar*)host_code >= (UChar*)sectors[to_sNo].tc );
   vg_assert( (UChar*)host_code <= (UChar*)sectors[to_sNo].tc_next
                                   + sizeof(ULong) - 1 );

   UInt from_sNo   = (UInt)-1;
   UInt from_tteNo = (UInt)-1;
   Bool from_found
      = find_TTEntry_from_hcode( &from_sNo, &from_tteNo,
                                 from__patch_addr );
   if (!from_found) {
      
      
      
      VG_(debugLog)(1,"transtab",
                    "host code %p not found (discarded? sector recycled?)"
                    " => no chaining done\n",
                    from__patch_addr);
      return;
   }

   TTEntry* from_tte = index_tte(from_sNo, from_tteNo);

   VexInvalRange vir
      = LibVEX_Chain(
           vex_arch,
           from__patch_addr,
           VG_(fnptr_to_fnentry)(
              to_fastEP ? &VG_(disp_cp_chain_me_to_fastEP)
                        : &VG_(disp_cp_chain_me_to_slowEP)),
           (void*)host_code
        );
   VG_(invalidate_icache)( (void*)vir.start, vir.len );


   
   InEdge ie;
   InEdge__init(&ie);
   ie.from_sNo   = from_sNo;
   ie.from_tteNo = from_tteNo;
   ie.to_fastEP  = to_fastEP;
   HWord from_offs = (HWord)( (UChar*)from__patch_addr
                              - (UChar*)from_tte->tcptr );
   vg_assert(from_offs < 100000);
   ie.from_offs  = (UInt)from_offs;

   
   OutEdge oe;
   OutEdge__init(&oe);
   oe.to_sNo    = to_sNo;
   oe.to_tteNo  = to_tteNo;
   oe.from_offs = (UInt)from_offs;

   
   InEdgeArr__add(&to_tte->in_edges, &ie);
   OutEdgeArr__add(&from_tte->out_edges, &oe);
}


__attribute__((noinline))
static void unchain_one ( VexArch vex_arch,
                          InEdge* ie,
                          void* to_fastEPaddr, void* to_slowEPaddr )
{
   vg_assert(ie);
   TTEntry* tte
      = index_tte(ie->from_sNo, ie->from_tteNo);
   UChar* place_to_patch
      = ((HChar*)tte->tcptr) + ie->from_offs;
   UChar* disp_cp_chain_me
      = VG_(fnptr_to_fnentry)(
           ie->to_fastEP ? &VG_(disp_cp_chain_me_to_fastEP)
                         : &VG_(disp_cp_chain_me_to_slowEP)
        );
   UChar* place_to_jump_to_EXPECTED
      = ie->to_fastEP ? to_fastEPaddr : to_slowEPaddr;

   
   
   vg_assert( is_in_the_main_TC(place_to_patch) ); 
   vg_assert( is_in_the_main_TC(place_to_jump_to_EXPECTED) ); 
   
   
   
   VexInvalRange vir
       = LibVEX_UnChain( vex_arch, place_to_patch, 
                         place_to_jump_to_EXPECTED, disp_cp_chain_me );
   VG_(invalidate_icache)( (void*)vir.start, vir.len );
}


static
void unchain_in_preparation_for_deletion ( VexArch vex_arch,
                                           UInt here_sNo, UInt here_tteNo )
{
   if (0)
      VG_(printf)("QQQ unchain_in_prep %u.%u\n", here_sNo, here_tteNo);
   UWord    i, j, n, m;
   Int      evCheckSzB = LibVEX_evCheckSzB(vex_arch);
   TTEntry* here_tte   = index_tte(here_sNo, here_tteNo);
   vg_assert(here_tte->status == InUse);

   
   n = InEdgeArr__size(&here_tte->in_edges);
   for (i = 0; i < n; i++) {
      InEdge* ie = InEdgeArr__index(&here_tte->in_edges, i);
      
      UChar* here_slow_EP = (UChar*)here_tte->tcptr;
      UChar* here_fast_EP = here_slow_EP + evCheckSzB;
      unchain_one(vex_arch, ie, here_fast_EP, here_slow_EP);
      
      
      TTEntry* from_tte = index_tte(ie->from_sNo, ie->from_tteNo);
      m = OutEdgeArr__size(&from_tte->out_edges);
      vg_assert(m > 0); 
      for (j = 0; j < m; j++) {
         OutEdge* oe = OutEdgeArr__index(&from_tte->out_edges, j);
         if (oe->to_sNo == here_sNo && oe->to_tteNo == here_tteNo
             && oe->from_offs == ie->from_offs)
           break;
      }
      vg_assert(j < m); 
      OutEdgeArr__deleteIndex(&from_tte->out_edges, j);
   }

   
   n = OutEdgeArr__size(&here_tte->out_edges);
   for (i = 0; i < n; i++) {
      OutEdge* oe = OutEdgeArr__index(&here_tte->out_edges, i);
      
      
      TTEntry* to_tte = index_tte(oe->to_sNo, oe->to_tteNo);
      m = InEdgeArr__size(&to_tte->in_edges);
      vg_assert(m > 0); 
      for (j = 0; j < m; j++) {
         InEdge* ie = InEdgeArr__index(&to_tte->in_edges, j);
         if (ie->from_sNo == here_sNo && ie->from_tteNo == here_tteNo
             && ie->from_offs == oe->from_offs)
           break;
      }
      vg_assert(j < m); 
      InEdgeArr__deleteIndex(&to_tte->in_edges, j);
   }

   InEdgeArr__makeEmpty(&here_tte->in_edges);
   OutEdgeArr__makeEmpty(&here_tte->out_edges);
}




static Int range_to_eclass ( Addr64 start, UInt len )
{
   UInt mask   = (1 << ECLASS_WIDTH) - 1;
   UInt lo     = (UInt)start;
   UInt hi     = lo + len - 1;
   UInt loBits = (lo >> ECLASS_SHIFT) & mask;
   UInt hiBits = (hi >> ECLASS_SHIFT) & mask;
   if (loBits == hiBits) {
      vg_assert(loBits < ECLASS_N-1);
      return loBits;
   } else {
      return ECLASS_MISC;
   }
}


/* Calculates the equivalence class numbers for any VexGuestExtent.
   These are written in *eclasses, which must be big enough to hold 3
   Ints.  The number written, between 1 and 3, is returned.  The
   eclasses are presented in order, and any duplicates are removed.
*/

static 
Int vexGuestExtents_to_eclasses ( Int* eclasses,
                                  VexGuestExtents* vge )
{
#  define SWAP(_lv1,_lv2) \
      do { Int t = _lv1; _lv1 = _lv2; _lv2 = t; } while (0)

   Int i, j, n_ec, r;

   vg_assert(vge->n_used >= 1 && vge->n_used <= 3);

   n_ec = 0;
   for (i = 0; i < vge->n_used; i++) {
      r = range_to_eclass( vge->base[i], (UInt)vge->len[i] );
      if (r == ECLASS_MISC) 
         goto bad;
      
      for (j = 0; j < n_ec; j++)
         if (eclasses[j] == r)
            break;
      if (j == n_ec)
         eclasses[n_ec++] = r;
   }

   if (n_ec == 1)
      return 1;

   if (n_ec == 2) {
      
      if (eclasses[0] > eclasses[1])
         SWAP(eclasses[0], eclasses[1]);
      return 2;
   }

   if (n_ec == 3) {
      
      if (eclasses[0] > eclasses[2])
         SWAP(eclasses[0], eclasses[2]);
      if (eclasses[0] > eclasses[1])
         SWAP(eclasses[0], eclasses[1]);
      if (eclasses[1] > eclasses[2])
         SWAP(eclasses[1], eclasses[2]);
      return 3;
   }

   
   vg_assert(0);

  bad:
   eclasses[0] = ECLASS_MISC;
   return 1;

#  undef SWAP
}



static 
UInt addEClassNo ( Sector* sec, Int ec, UShort tteno )
{
   Int    old_sz, new_sz, i, r;
   UShort *old_ar, *new_ar;

   vg_assert(ec >= 0 && ec < ECLASS_N);
   vg_assert(tteno < N_TTES_PER_SECTOR);

   if (0) VG_(printf)("ec %d  gets %d\n", ec, (Int)tteno);

   if (sec->ec2tte_used[ec] >= sec->ec2tte_size[ec]) {

      vg_assert(sec->ec2tte_used[ec] == sec->ec2tte_size[ec]);

      old_sz = sec->ec2tte_size[ec];
      old_ar = sec->ec2tte[ec];
      new_sz = old_sz==0 ? 8 : old_sz<64 ? 2*old_sz : (3*old_sz)/2;
      new_ar = ttaux_malloc("transtab.aECN.1",
                            new_sz * sizeof(UShort));
      for (i = 0; i < old_sz; i++)
         new_ar[i] = old_ar[i];
      if (old_ar)
         ttaux_free(old_ar);
      sec->ec2tte_size[ec] = new_sz;
      sec->ec2tte[ec] = new_ar;

      if (0) VG_(printf)("expand ec %d to %d\n", ec, new_sz);
   }

   
   r = sec->ec2tte_used[ec]++;
   vg_assert(r >= 0 && r < sec->ec2tte_size[ec]);
   sec->ec2tte[ec][r] = tteno;
   return (UInt)r;
}



static 
void upd_eclasses_after_add ( Sector* sec, Int tteno )
{
   Int i, r, eclasses[3];
   TTEntry* tte;
   vg_assert(tteno >= 0 && tteno < N_TTES_PER_SECTOR);

   tte = &sec->tt[tteno];
   r = vexGuestExtents_to_eclasses( eclasses, &tte->vge );

   vg_assert(r >= 1 && r <= 3);
   tte->n_tte2ec = r;

   for (i = 0; i < r; i++) {
      tte->tte2ec_ec[i] = eclasses[i];
      tte->tte2ec_ix[i] = addEClassNo( sec, eclasses[i], (UShort)tteno );
   }
}



static Bool sanity_check_eclasses_in_sector ( Sector* sec )
{
#  define BAD(_str) do { whassup = (_str); goto bad; } while (0)

   HChar*   whassup = NULL;
   Int      i, j, k, n, ec_num, ec_idx;
   TTEntry* tte;
   UShort   tteno;
   ULong*   tce;

   
   if (sec->tt_n_inuse < 0 || sec->tt_n_inuse > N_TTES_PER_SECTOR_USABLE)
      BAD("invalid sec->tt_n_inuse");
   tce = sec->tc_next;
   if (tce < &sec->tc[0] || tce > &sec->tc[tc_sector_szQ])
      BAD("sec->tc_next points outside tc");

   
   for (i = 0; i < ECLASS_N; i++) {
      if (sec->ec2tte_size[i] == 0 && sec->ec2tte[i] != NULL)
         BAD("ec2tte_size/ec2tte mismatch(1)");
      if (sec->ec2tte_size[i] != 0 && sec->ec2tte[i] == NULL)
         BAD("ec2tte_size/ec2tte mismatch(2)");
      if (sec->ec2tte_used[i] < 0 
          || sec->ec2tte_used[i] > sec->ec2tte_size[i])
         BAD("implausible ec2tte_used");
      if (sec->ec2tte_used[i] == 0)
         continue;


      for (j = 0; j < sec->ec2tte_used[i]; j++) {
         tteno = sec->ec2tte[i][j];
         if (tteno == EC2TTE_DELETED)
            continue;
         if (tteno >= N_TTES_PER_SECTOR)
            BAD("implausible tteno");
         tte = &sec->tt[tteno];
         if (tte->status != InUse)
            BAD("tteno points to non-inuse tte");
         if (tte->n_tte2ec < 1 || tte->n_tte2ec > 3)
            BAD("tte->n_tte2ec out of range");
         n = 0;
         for (k = 0; k < tte->n_tte2ec; k++) {
            if (k < tte->n_tte2ec-1
                && tte->tte2ec_ec[k] >= tte->tte2ec_ec[k+1])
               BAD("tte->tte2ec_ec[..] out of order");
            ec_num = tte->tte2ec_ec[k];
            if (ec_num < 0 || ec_num >= ECLASS_N)
               BAD("tte->tte2ec_ec[..] out of range");
            if (ec_num != i)
               continue;
            ec_idx = tte->tte2ec_ix[k];
            if (ec_idx < 0 || ec_idx >= sec->ec2tte_used[i])
               BAD("tte->tte2ec_ix[..] out of range");
            if (ec_idx == j)
               n++;
         }
         if (n != 1)
            BAD("tteno does not point back at eclass");
      }
   }


   for (i = 0; i < N_TTES_PER_SECTOR_USABLE; i++) {

      tte = &sec->tt[i];
      if (tte->status == Empty || tte->status == Deleted) {
         if (tte->n_tte2ec != 0)
            BAD("tte->n_eclasses nonzero for unused tte");
         continue;
      }

      vg_assert(tte->status == InUse);

      if (tte->n_tte2ec < 1 || tte->n_tte2ec > 3)
         BAD("tte->n_eclasses out of range(2)");

      for (j = 0; j < tte->n_tte2ec; j++) {
         ec_num = tte->tte2ec_ec[j];
         if (ec_num < 0 || ec_num >= ECLASS_N)
            BAD("tte->eclass[..] out of range");
         ec_idx = tte->tte2ec_ix[j];
         if (ec_idx < 0 || ec_idx >= sec->ec2tte_used[ec_num])
            BAD("tte->ec_idx[..] out of range(2)");
         if (sec->ec2tte[ec_num][ec_idx] != i)
            BAD("ec2tte does not point back to tte");
      }
   }

   return True;

  bad:
   if (whassup)
      VG_(debugLog)(0, "transtab", "eclass sanity fail: %s\n", whassup);

#  if 0
   VG_(printf)("eclass = %d\n", i);
   VG_(printf)("tteno = %d\n", (Int)tteno);
   switch (tte->status) {
      case InUse:   VG_(printf)("InUse\n"); break;
      case Deleted: VG_(printf)("Deleted\n"); break;
      case Empty:   VG_(printf)("Empty\n"); break;
   }
   if (tte->status != Empty) {
      for (k = 0; k < tte->vge.n_used; k++)
         VG_(printf)("0x%llx %d\n", tte->vge.base[k], 
                                    (Int)tte->vge.len[k]);
   }
#  endif

   return False;

#  undef BAD
}



static Bool sanity_check_redir_tt_tc ( void );

static Bool sanity_check_sector_search_order ( void )
{
   Int i, j, nListed;
   
   vg_assert(N_SECTORS == (sizeof(sector_search_order) 
                           / sizeof(sector_search_order[0])));
   
   for (i = 0; i < N_SECTORS; i++) {
      if (sector_search_order[i] < 0 || sector_search_order[i] >= N_SECTORS)
         break;
   }
   nListed = i;
   for (; i < N_SECTORS; i++) {
      if (sector_search_order[i] != -1)
         break;
   }
   if (i != N_SECTORS)
      return False;
   
   for (i = 0; i < N_SECTORS; i++) {
      if (sector_search_order[i] == -1)
         continue;
      for (j = i+1; j < N_SECTORS; j++) {
         if (sector_search_order[j] == sector_search_order[i])
            return False;
      }
   }
   for (i = 0; i < N_SECTORS; i++) {
      if (sectors[i].tc != NULL)
         nListed--;
   }
   if (nListed != 0)
      return False;
   return True;
}

static Bool sanity_check_all_sectors ( void )
{
   Int     sno;
   Bool    sane;
   Sector* sec;
   for (sno = 0; sno < N_SECTORS; sno++) {
      sec = &sectors[sno];
      if (sec->tc == NULL)
         continue;
      sane = sanity_check_eclasses_in_sector( sec );
      if (!sane)
         return False;
   }
   if ( !sanity_check_redir_tt_tc() )
      return False;
   if ( !sanity_check_sector_search_order() )
      return False;
   return True;
}




static UInt vge_osize ( VexGuestExtents* vge )
{
   UInt i, n = 0;
   for (i = 0; i < vge->n_used; i++)
      n += (UInt)vge->len[i];
   return n;
}

static Bool isValidSector ( Int sector )
{
   if (sector < 0 || sector >= N_SECTORS)
      return False;
   return True;
}

static inline UInt HASH_TT ( Addr64 key )
{
   UInt kHi = (UInt)(key >> 32);
   UInt kLo = (UInt)key;
   UInt k32 = kHi ^ kLo;
   UInt ror = 7;
   if (ror > 0)
      k32 = (k32 >> ror) | (k32 << (32-ror));
   return k32 % N_TTES_PER_SECTOR;
}

static void setFastCacheEntry ( Addr64 key, ULong* tcptr )
{
   UInt cno = (UInt)VG_TT_FAST_HASH(key);
   VG_(tt_fast)[cno].guest = (Addr)key;
   VG_(tt_fast)[cno].host  = (Addr)tcptr;
   n_fast_updates++;
   vg_assert(VG_(tt_fast)[cno].guest != TRANSTAB_BOGUS_GUEST_ADDR);
}

static void invalidateFastCache ( void )
{
   UInt j;
   vg_assert(VG_TT_FAST_SIZE > 0 && (VG_TT_FAST_SIZE % 4) == 0);
   for (j = 0; j < VG_TT_FAST_SIZE; j += 4) {
      VG_(tt_fast)[j+0].guest = TRANSTAB_BOGUS_GUEST_ADDR;
      VG_(tt_fast)[j+1].guest = TRANSTAB_BOGUS_GUEST_ADDR;
      VG_(tt_fast)[j+2].guest = TRANSTAB_BOGUS_GUEST_ADDR;
      VG_(tt_fast)[j+3].guest = TRANSTAB_BOGUS_GUEST_ADDR;
   }

   vg_assert(j == VG_TT_FAST_SIZE);
   n_fast_flushes++;
}

static void initialiseSector ( Int sno )
{
   Int     i;
   SysRes  sres;
   Sector* sec;
   vg_assert(isValidSector(sno));

   { Bool sane = sanity_check_sector_search_order();
     vg_assert(sane);
   }
   sec = &sectors[sno];

   if (sec->tc == NULL) {

      vg_assert(sec->tt == NULL);
      vg_assert(sec->tc_next == NULL);
      vg_assert(sec->tt_n_inuse == 0);
      for (i = 0; i < ECLASS_N; i++) {
         vg_assert(sec->ec2tte_size[i] == 0);
         vg_assert(sec->ec2tte_used[i] == 0);
         vg_assert(sec->ec2tte[i] == NULL);
      }
      vg_assert(sec->host_extents == NULL);

      VG_(debugLog)(1,"transtab", "allocate sector %d\n", sno);

      sres = VG_(am_mmap_anon_float_valgrind)( 8 * tc_sector_szQ );
      if (sr_isError(sres)) {
         VG_(out_of_memory_NORETURN)("initialiseSector(TC)", 
                                     8 * tc_sector_szQ );
	 
      }
      sec->tc = (ULong*)(AddrH)sr_Res(sres);

      sres = VG_(am_mmap_anon_float_valgrind)
                ( N_TTES_PER_SECTOR * sizeof(TTEntry) );
      if (sr_isError(sres)) {
         VG_(out_of_memory_NORETURN)("initialiseSector(TT)", 
                                     N_TTES_PER_SECTOR * sizeof(TTEntry) );
	 
      }
      sec->tt = (TTEntry*)(AddrH)sr_Res(sres);

      for (i = 0; i < N_TTES_PER_SECTOR; i++) {
         sec->tt[i].status   = Empty;
         sec->tt[i].n_tte2ec = 0;
      }

      
      sec->host_extents
         = VG_(newXA)(ttaux_malloc, "transtab.initialiseSector(host_extents)",
                      ttaux_free,
                      sizeof(HostExtent));

      
      for (i = 0; i < N_SECTORS; i++) {
         if (sector_search_order[i] == -1)
            break;
      }
      vg_assert(i >= 0 && i < N_SECTORS);
      sector_search_order[i] = sno;

      if (VG_(clo_verbosity) > 2)
         VG_(message)(Vg_DebugMsg, "TT/TC: initialise sector %d\n", sno);

   } else {

      
      VG_(debugLog)(1,"transtab", "recycle sector %d\n", sno);
      vg_assert(sec->tt != NULL);
      vg_assert(sec->tc_next != NULL);
      n_dump_count += sec->tt_n_inuse;

      VexArch vex_arch = VexArch_INVALID;
      VG_(machine_get_VexArchInfo)( &vex_arch, NULL );

      
      if (0) VG_(printf)("QQQ unlink-entire-sector: %d START\n", sno);
      for (i = 0; i < N_TTES_PER_SECTOR; i++) {
         if (sec->tt[i].status == InUse) {
            vg_assert(sec->tt[i].n_tte2ec >= 1);
            vg_assert(sec->tt[i].n_tte2ec <= 3);
            n_dump_osize += vge_osize(&sec->tt[i].vge);
            
            if (VG_(needs).superblock_discards) {
               VG_TDICT_CALL( tool_discard_superblock_info,
                              sec->tt[i].entry,
                              sec->tt[i].vge );
            }
            unchain_in_preparation_for_deletion(vex_arch, sno, i);
         } else {
            vg_assert(sec->tt[i].n_tte2ec == 0);
         }
         sec->tt[i].status   = Empty;
         sec->tt[i].n_tte2ec = 0;
      }
      if (0) VG_(printf)("QQQ unlink-entire-sector: %d END\n", sno);

      
      for (i = 0; i < ECLASS_N; i++) {
         if (sec->ec2tte_size[i] == 0) {
            vg_assert(sec->ec2tte_used[i] == 0);
            vg_assert(sec->ec2tte[i] == NULL);
         } else {
            vg_assert(sec->ec2tte[i] != NULL);
            ttaux_free(sec->ec2tte[i]);
            sec->ec2tte[i] = NULL;
            sec->ec2tte_size[i] = 0;
            sec->ec2tte_used[i] = 0;
         }
      }

      
      vg_assert(sec->host_extents != NULL);
      VG_(dropTailXA)(sec->host_extents, VG_(sizeXA)(sec->host_extents));
      vg_assert(VG_(sizeXA)(sec->host_extents) == 0);

      for (i = 0; i < N_SECTORS; i++) {
         if (sector_search_order[i] == sno)
            break;
      }
      vg_assert(i >= 0 && i < N_SECTORS);

      if (VG_(clo_verbosity) > 2)
         VG_(message)(Vg_DebugMsg, "TT/TC: recycle sector %d\n", sno);
   }

   sec->tc_next = sec->tc;
   sec->tt_n_inuse = 0;

   invalidateFastCache();

   { Bool sane = sanity_check_sector_search_order();
     vg_assert(sane);
   }
}


void VG_(add_to_transtab)( VexGuestExtents* vge,
                           Addr64           entry,
                           AddrH            code,
                           UInt             code_len,
                           Bool             is_self_checking,
                           Int              offs_profInc,
                           UInt             n_guest_instrs,
                           VexArch          arch_host )
{
   Int    tcAvailQ, reqdQ, y, i;
   ULong  *tcptr, *tcptr2;
   UChar* srcP;
   UChar* dstP;

   vg_assert(init_done);
   vg_assert(vge->n_used >= 1 && vge->n_used <= 3);

   
   vg_assert(code_len > 0 && code_len < 60000);

   
   vg_assert(n_guest_instrs < 200); 

   if (0)
      VG_(printf)("add_to_transtab(entry = 0x%llx, len = %d)\n",
                  entry, code_len);

   n_in_count++;
   n_in_tsize += code_len;
   n_in_osize += vge_osize(vge);
   if (is_self_checking)
      n_in_sc_count++;

   y = youngest_sector;
   vg_assert(isValidSector(y));

   if (sectors[y].tc == NULL)
      initialiseSector(y);

   
   reqdQ = (code_len + 7) >> 3;

   
   tcAvailQ = ((ULong*)(&sectors[y].tc[tc_sector_szQ]))
              - ((ULong*)(sectors[y].tc_next));
   vg_assert(tcAvailQ >= 0);
   vg_assert(tcAvailQ <= tc_sector_szQ);

   if (tcAvailQ < reqdQ 
       || sectors[y].tt_n_inuse >= N_TTES_PER_SECTOR_USABLE) {
      vg_assert(tc_sector_szQ > 0);
      VG_(debugLog)(1,"transtab", 
                      "declare sector %d full "
                      "(TT loading %2d%%, TC loading %2d%%)\n",
                      y,
                      (100 * sectors[y].tt_n_inuse) 
                         / N_TTES_PER_SECTOR,
                      (100 * (tc_sector_szQ - tcAvailQ)) 
                         / tc_sector_szQ);
      youngest_sector++;
      if (youngest_sector >= N_SECTORS)
         youngest_sector = 0;
      y = youngest_sector;
      initialiseSector(y);
   }

   
   tcAvailQ = ((ULong*)(&sectors[y].tc[tc_sector_szQ]))
              - ((ULong*)(sectors[y].tc_next));
   vg_assert(tcAvailQ >= 0);
   vg_assert(tcAvailQ <= tc_sector_szQ);
   vg_assert(tcAvailQ >= reqdQ);
   vg_assert(sectors[y].tt_n_inuse < N_TTES_PER_SECTOR_USABLE);
   vg_assert(sectors[y].tt_n_inuse >= 0);
 
   
   tcptr = sectors[y].tc_next;
   vg_assert(tcptr >= &sectors[y].tc[0]);
   vg_assert(tcptr <= &sectors[y].tc[tc_sector_szQ]);

   dstP = (UChar*)tcptr;
   srcP = (UChar*)code;
   VG_(memcpy)(dstP, srcP, code_len);
   sectors[y].tc_next += reqdQ;
   sectors[y].tt_n_inuse++;

   
   tcptr2 = sectors[y].tc_next;
   vg_assert(tcptr2 >= &sectors[y].tc[0]);
   vg_assert(tcptr2 <= &sectors[y].tc[tc_sector_szQ]);

   i = HASH_TT(entry);
   vg_assert(i >= 0 && i < N_TTES_PER_SECTOR);
   while (True) {
      if (sectors[y].tt[i].status == Empty
          || sectors[y].tt[i].status == Deleted)
         break;
      i++;
      if (i >= N_TTES_PER_SECTOR)
         i = 0;
   }

   TTEntry__init(&sectors[y].tt[i]);
   sectors[y].tt[i].status = InUse;
   sectors[y].tt[i].tcptr  = tcptr;
   sectors[y].tt[i].count  = 0;
   sectors[y].tt[i].weight = n_guest_instrs == 0 ? 1 : n_guest_instrs;
   sectors[y].tt[i].vge    = *vge;
   sectors[y].tt[i].entry  = entry;

   
   if (offs_profInc != -1) {
      vg_assert(offs_profInc >= 0 && offs_profInc < code_len);
      VexInvalRange vir
         = LibVEX_PatchProfInc( arch_host,
                                dstP + offs_profInc,
                                &sectors[y].tt[i].count );
      VG_(invalidate_icache)( (void*)vir.start, vir.len );
   }

   VG_(invalidate_icache)( dstP, code_len );

   { HostExtent hx;
     hx.start = (UChar*)tcptr;
     hx.len   = code_len;
     hx.tteNo = i;
     vg_assert(hx.len > 0); 
     XArray* hx_array = sectors[y].host_extents;
     vg_assert(hx_array);
     Word n = VG_(sizeXA)(hx_array);
     if (n > 0) {
        HostExtent* hx_prev = (HostExtent*)VG_(indexXA)(hx_array, n-1);
        vg_assert(hx_prev->start + hx_prev->len <= hx.start);
     }
     VG_(addToXA)(hx_array, &hx);
   }

   
   setFastCacheEntry( entry, tcptr );

   
   upd_eclasses_after_add( &sectors[y], i );
}


Bool VG_(search_transtab) ( AddrH* res_hcode,
                            UInt*  res_sNo,
                            UInt*  res_tteNo,
                            Addr64        guest_addr, 
                            Bool          upd_cache )
{
   Int i, j, k, kstart, sno;

   vg_assert(init_done);
   n_full_lookups++;
   k      = -1;
   kstart = HASH_TT(guest_addr);
   vg_assert(kstart >= 0 && kstart < N_TTES_PER_SECTOR);

   for (i = 0; i < N_SECTORS; i++) {

      sno = sector_search_order[i];
      if (UNLIKELY(sno == -1))
         return False; 

      k = kstart;
      for (j = 0; j < N_TTES_PER_SECTOR; j++) {
         n_lookup_probes++;
         if (sectors[sno].tt[k].status == InUse
             && sectors[sno].tt[k].entry == guest_addr) {
            
            if (upd_cache)
               setFastCacheEntry( 
                  guest_addr, sectors[sno].tt[k].tcptr );
            if (res_hcode)
               *res_hcode = (AddrH)sectors[sno].tt[k].tcptr;
            if (res_sNo)
               *res_sNo = sno;
            if (res_tteNo)
               *res_tteNo = k;
            if (i > 0) {
               Int tmp = sector_search_order[i-1];
               sector_search_order[i-1] = sector_search_order[i];
               sector_search_order[i] = tmp;
            }
            return True;
         }
         if (sectors[sno].tt[k].status == Empty)
            break; 
         k++;
         if (k == N_TTES_PER_SECTOR)
            k = 0;
      }

   }

   
   return False;
}



static void unredir_discard_translations( Addr64, ULong );


static inline
Bool overlap1 ( Addr64 s1, ULong r1, Addr64 s2, ULong r2 )
{
   Addr64 e1 = s1 + r1 - 1ULL;
   Addr64 e2 = s2 + r2 - 1ULL;
   if (e1 < s2 || e2 < s1) 
      return False;
   return True;
}

static inline
Bool overlaps ( Addr64 start, ULong range, VexGuestExtents* vge )
{
   if (overlap1(start, range, vge->base[0], (UInt)vge->len[0]))
      return True;
   if (vge->n_used < 2)
      return False;
   if (overlap1(start, range, vge->base[1], (UInt)vge->len[1]))
      return True;
   if (vge->n_used < 3)
      return False;
   if (overlap1(start, range, vge->base[2], (UInt)vge->len[2]))
      return True;
   return False;
}



static void delete_tte ( Sector* sec, UInt secNo, Int tteno,
                         VexArch vex_arch )
{
   Int      i, ec_num, ec_idx;
   TTEntry* tte;

   
   vg_assert(sec == &sectors[secNo]);

   vg_assert(tteno >= 0 && tteno < N_TTES_PER_SECTOR);
   tte = &sec->tt[tteno];
   vg_assert(tte->status == InUse);
   vg_assert(tte->n_tte2ec >= 1 && tte->n_tte2ec <= 3);

   
   unchain_in_preparation_for_deletion(vex_arch, secNo, tteno);

   
   for (i = 0; i < tte->n_tte2ec; i++) {
      ec_num = (Int)tte->tte2ec_ec[i];
      ec_idx = tte->tte2ec_ix[i];
      vg_assert(ec_num >= 0 && ec_num < ECLASS_N);
      vg_assert(ec_idx >= 0);
      vg_assert(ec_idx < sec->ec2tte_used[ec_num]);
      
      vg_assert(sec->ec2tte[ec_num][ec_idx] == (UShort)tteno);
      
      sec->ec2tte[ec_num][ec_idx] = EC2TTE_DELETED;
   }

   
   tte->status   = Deleted;
   tte->n_tte2ec = 0;

   
   sec->tt_n_inuse--;
   n_disc_count++;
   n_disc_osize += vge_osize(&tte->vge);

   
   if (VG_(needs).superblock_discards) {
      VG_TDICT_CALL( tool_discard_superblock_info,
                     tte->entry,
                     tte->vge );
   }
}



static 
Bool delete_translations_in_sector_eclass ( Sector* sec, UInt secNo,
                                            Addr64 guest_start, ULong range,
                                            Int ec,
                                            VexArch vex_arch )
{
   Int      i;
   UShort   tteno;
   Bool     anyDeld = False;
   TTEntry* tte;

   vg_assert(ec >= 0 && ec < ECLASS_N);

   for (i = 0; i < sec->ec2tte_used[ec]; i++) {

      tteno = sec->ec2tte[ec][i];
      if (tteno == EC2TTE_DELETED) {
         
         continue;
      }

      vg_assert(tteno < N_TTES_PER_SECTOR);

      tte = &sec->tt[tteno];
      vg_assert(tte->status == InUse);

      if (overlaps( guest_start, range, &tte->vge )) {
         anyDeld = True;
         delete_tte( sec, secNo, (Int)tteno, vex_arch );
      }

   }

   return anyDeld;
}



static 
Bool delete_translations_in_sector ( Sector* sec, UInt secNo,
                                     Addr64 guest_start, ULong range,
                                     VexArch vex_arch )
{
   Int  i;
   Bool anyDeld = False;

   for (i = 0; i < N_TTES_PER_SECTOR; i++) {
      if (sec->tt[i].status == InUse
          && overlaps( guest_start, range, &sec->tt[i].vge )) {
         anyDeld = True;
         delete_tte( sec, secNo, i, vex_arch );
      }
   }

   return anyDeld;
} 


void VG_(discard_translations) ( Addr64 guest_start, ULong range,
                                 HChar* who )
{
   Sector* sec;
   Int     sno, ec;
   Bool    anyDeleted = False;

   vg_assert(init_done);

   VG_(debugLog)(2, "transtab",
                    "discard_translations(0x%llx, %lld) req by %s\n",
                    guest_start, range, who );

   
   if (VG_(clo_sanity_level >= 4)) {
      Bool sane = sanity_check_all_sectors();
      vg_assert(sane);
   }

   if (range == 0)
      return;

   VexArch vex_arch = VexArch_INVALID;
   VG_(machine_get_VexArchInfo)( &vex_arch, NULL );



   ec = ECLASS_MISC;
   if (range < (1ULL << ECLASS_SHIFT))
      ec = range_to_eclass( guest_start, (UInt)range );


   if (ec != ECLASS_MISC) {

      VG_(debugLog)(2, "transtab",
                       "                    FAST, ec = %d\n", ec);

      
      vg_assert(ec >= 0 && ec < ECLASS_MISC);

      for (sno = 0; sno < N_SECTORS; sno++) {
         sec = &sectors[sno];
         if (sec->tc == NULL)
            continue;
         anyDeleted |= delete_translations_in_sector_eclass( 
                          sec, sno, guest_start, range, ec, 
                          vex_arch
                       );
         anyDeleted |= delete_translations_in_sector_eclass( 
                          sec, sno, guest_start, range, ECLASS_MISC,
                          vex_arch
                       );
      }

   } else {

      

      VG_(debugLog)(2, "transtab",
                       "                    SLOW, ec = %d\n", ec);

      for (sno = 0; sno < N_SECTORS; sno++) {
         sec = &sectors[sno];
         if (sec->tc == NULL)
            continue;
         anyDeleted |= delete_translations_in_sector( 
                          sec, sno, guest_start, range, vex_arch );
      }

   }

   if (anyDeleted)
      invalidateFastCache();

   
   unredir_discard_translations( guest_start, range );

   
   if (VG_(clo_sanity_level >= 4)) {
      Int      i;
      TTEntry* tte;
      Bool     sane = sanity_check_all_sectors();
      vg_assert(sane);
      for (sno = 0; sno < N_SECTORS; sno++) {
         sec = &sectors[sno];
         if (sec->tc == NULL)
            continue;
         for (i = 0; i < N_TTES_PER_SECTOR; i++) {
            tte = &sec->tt[i];
            if (tte->status != InUse)
               continue;
            vg_assert(!overlaps( guest_start, range, &tte->vge ));
         }
      }
   }
}





#define UNREDIR_SZB   1000

#define N_UNREDIR_TT  500
#define N_UNREDIR_TCQ (N_UNREDIR_TT * UNREDIR_SZB / sizeof(ULong))

typedef
   struct {
      VexGuestExtents vge;
      Addr            hcode;
      Bool            inUse;
   }
   UTCEntry;

static ULong    *unredir_tc;
static Int      unredir_tc_used = N_UNREDIR_TCQ;

static UTCEntry unredir_tt[N_UNREDIR_TT];
static Int      unredir_tt_highwater;


static void init_unredir_tt_tc ( void )
{
   Int i;
   if (unredir_tc == NULL) {
      SysRes sres = VG_(am_mmap_anon_float_valgrind)
                       ( N_UNREDIR_TT * UNREDIR_SZB );
      if (sr_isError(sres)) {
         VG_(out_of_memory_NORETURN)("init_unredir_tt_tc",
                                     N_UNREDIR_TT * UNREDIR_SZB);
         
      }
      unredir_tc = (ULong *)(AddrH)sr_Res(sres);
   }
   unredir_tc_used = 0;
   for (i = 0; i < N_UNREDIR_TT; i++)
      unredir_tt[i].inUse = False;
   unredir_tt_highwater = -1;
}

static Bool sanity_check_redir_tt_tc ( void )
{
   Int i;
   if (unredir_tt_highwater < -1) return False;
   if (unredir_tt_highwater >= N_UNREDIR_TT) return False;

   for (i = unredir_tt_highwater+1; i < N_UNREDIR_TT; i++)
      if (unredir_tt[i].inUse)
         return False;

   if (unredir_tc_used < 0) return False;
   if (unredir_tc_used > N_UNREDIR_TCQ) return False;

   return True;
}


void VG_(add_to_unredir_transtab)( VexGuestExtents* vge,
                                   Addr64           entry,
                                   AddrH            code,
                                   UInt             code_len )
{
   Int   i, j, code_szQ;
   HChar *srcP, *dstP;

   vg_assert(sanity_check_redir_tt_tc());

   
   vg_assert(entry == vge->base[0]);

      
   code_szQ = (code_len + 7) / 8;

   
   for (i = 0; i < N_UNREDIR_TT; i++)
      if (!unredir_tt[i].inUse)
         break;

   if (i >= N_UNREDIR_TT || code_szQ > (N_UNREDIR_TCQ - unredir_tc_used)) {
      
      init_unredir_tt_tc();
      i = 0;
   }

   vg_assert(unredir_tc_used >= 0);
   vg_assert(unredir_tc_used <= N_UNREDIR_TCQ);
   vg_assert(code_szQ > 0);
   vg_assert(code_szQ + unredir_tc_used <= N_UNREDIR_TCQ);
   vg_assert(i >= 0 && i < N_UNREDIR_TT);
   vg_assert(unredir_tt[i].inUse == False);

   if (i > unredir_tt_highwater)
      unredir_tt_highwater = i;

   dstP = (HChar*)&unredir_tc[unredir_tc_used];
   srcP = (HChar*)code;
   for (j = 0; j < code_len; j++)
      dstP[j] = srcP[j];

   VG_(invalidate_icache)( dstP, code_len );

   unredir_tt[i].inUse = True;
   unredir_tt[i].vge   = *vge;
   unredir_tt[i].hcode = (Addr)dstP;

   unredir_tc_used += code_szQ;
   vg_assert(unredir_tc_used >= 0);
   vg_assert(unredir_tc_used <= N_UNREDIR_TCQ);

   vg_assert(&dstP[code_len] <= (HChar*)&unredir_tc[unredir_tc_used]);
}

Bool VG_(search_unredir_transtab) ( AddrH* result,
                                    Addr64        guest_addr )
{
   Int i;
   for (i = 0; i < N_UNREDIR_TT; i++) {
      if (!unredir_tt[i].inUse)
         continue;
      if (unredir_tt[i].vge.base[0] == guest_addr) {
         *result = (AddrH)unredir_tt[i].hcode;
         return True;
      }
   }
   return False;
}

static void unredir_discard_translations( Addr64 guest_start, ULong range )
{
   Int i;

   vg_assert(sanity_check_redir_tt_tc());

   for (i = 0; i <= unredir_tt_highwater; i++) {
      if (unredir_tt[i].inUse
          && overlaps( guest_start, range, &unredir_tt[i].vge))
         unredir_tt[i].inUse = False;
   }
}



void VG_(init_tt_tc) ( void )
{
   Int i, j, avg_codeszQ;

   vg_assert(!init_done);
   init_done = True;

   
   vg_assert(sizeof(ULong) == 8);
   vg_assert(sizeof(Addr64) == 8);
   
   vg_assert(sizeof(Addr) == sizeof(void*));
   vg_assert(sizeof(FastCacheEntry) == 2 * sizeof(Addr));
   
   vg_assert(sizeof( VG_(tt_fast) ) == VG_TT_FAST_SIZE * sizeof(FastCacheEntry));
   vg_assert(VG_IS_16_ALIGNED( ((Addr) & VG_(tt_fast)[0]) ));

   if (VG_(clo_verbosity) > 2)
      VG_(message)(Vg_DebugMsg, 
                   "TT/TC: VG_(init_tt_tc) "
                   "(startup of code management)\n");

   
   avg_codeszQ   = (VG_(details).avg_translation_sizeB + 7) / 8;
   tc_sector_szQ = N_TTES_PER_SECTOR_USABLE * (1 + avg_codeszQ);

   
   vg_assert(tc_sector_szQ >= 2 * N_TTES_PER_SECTOR_USABLE);
   vg_assert(tc_sector_szQ <= 100 * N_TTES_PER_SECTOR_USABLE);

   
   youngest_sector = 0;
   for (i = 0; i < N_SECTORS; i++) {
      sectors[i].tc = NULL;
      sectors[i].tt = NULL;
      sectors[i].tc_next = NULL;
      sectors[i].tt_n_inuse = 0;
      for (j = 0; j < ECLASS_N; j++) {
         sectors[i].ec2tte_size[j] = 0;
         sectors[i].ec2tte_used[j] = 0;
         sectors[i].ec2tte[j] = NULL;
      }
      sectors[i].host_extents = NULL;
   }

   
   for (i = 0; i < N_SECTORS; i++)
      sector_search_order[i] = -1;

   
   invalidateFastCache();

   
   init_unredir_tt_tc();

   if (VG_(clo_verbosity) > 2) {
      VG_(message)(Vg_DebugMsg,
         "TT/TC: cache: %d sectors of %d bytes each = %d total\n", 
          N_SECTORS, 8 * tc_sector_szQ,
          N_SECTORS * 8 * tc_sector_szQ );
      VG_(message)(Vg_DebugMsg,
         "TT/TC: table: %d total entries, max occupancy %d (%d%%)\n",
         N_SECTORS * N_TTES_PER_SECTOR,
         N_SECTORS * N_TTES_PER_SECTOR_USABLE, 
         SECTOR_TT_LIMIT_PERCENT );
   }

   VG_(debugLog)(2, "transtab",
      "cache: %d sectors of %d bytes each = %d total\n", 
       N_SECTORS, 8 * tc_sector_szQ,
       N_SECTORS * 8 * tc_sector_szQ );
   VG_(debugLog)(2, "transtab",
      "table: %d total entries, max occupancy %d (%d%%)\n",
      N_SECTORS * N_TTES_PER_SECTOR,
      N_SECTORS * N_TTES_PER_SECTOR_USABLE, 
      SECTOR_TT_LIMIT_PERCENT );
}



static ULong safe_idiv( ULong a, ULong b )
{
   return (b == 0 ? 0 : a / b);
}

UInt VG_(get_bbs_translated) ( void )
{
   return n_in_count;
}

void VG_(print_tt_tc_stats) ( void )
{
   VG_(message)(Vg_DebugMsg,
      "    tt/tc: %'llu tt lookups requiring %'llu probes\n",
      n_full_lookups, n_lookup_probes );
   VG_(message)(Vg_DebugMsg,
      "    tt/tc: %'llu fast-cache updates, %'llu flushes\n",
      n_fast_updates, n_fast_flushes );

   VG_(message)(Vg_DebugMsg,
                " transtab: new        %'lld "
                "(%'llu -> %'llu; ratio %'llu:10) [%'llu scs]\n",
                n_in_count, n_in_osize, n_in_tsize,
                safe_idiv(10*n_in_tsize, n_in_osize),
                n_in_sc_count);
   VG_(message)(Vg_DebugMsg,
                " transtab: dumped     %'llu (%'llu -> ?" "?)\n",
                n_dump_count, n_dump_osize );
   VG_(message)(Vg_DebugMsg,
                " transtab: discarded  %'llu (%'llu -> ?" "?)\n",
                n_disc_count, n_disc_osize );

   if (0) {
      Int i;
      VG_(printf)("\n");
      for (i = 0; i < ECLASS_N; i++) {
         VG_(printf)(" %4d", sectors[0].ec2tte_used[i]);
         if (i % 16 == 15)
            VG_(printf)("\n");
      }
      VG_(printf)("\n\n");
   }
}


static ULong score ( TTEntry* tte )
{
   return ((ULong)tte->weight) * ((ULong)tte->count);
}

ULong VG_(get_BB_profile) ( BBProfEntry tops[], UInt n_tops )
{
   Int   sno, i, r, s;
   ULong score_total;

   for (i = 0; i < n_tops; i++) {
      tops[i].addr  = 0;
      tops[i].score = 0;
   }

   score_total = 0;

   for (sno = 0; sno < N_SECTORS; sno++) {
      if (sectors[sno].tc == NULL)
         continue;
      for (i = 0; i < N_TTES_PER_SECTOR; i++) {
         if (sectors[sno].tt[i].status != InUse)
            continue;
         score_total += score(&sectors[sno].tt[i]);
         
         r = n_tops-1;
         while (True) {
            if (r == -1)
               break;
             if (tops[r].addr == 0) {
               r--; 
               continue;
             }
             if ( score(&sectors[sno].tt[i]) > tops[r].score ) {
                r--;
                continue;
             }
             break;
         }
         r++;
         vg_assert(r >= 0 && r <= n_tops);
         if (r < n_tops) {
            for (s = n_tops-1; s > r; s--)
               tops[s] = tops[s-1];
            tops[r].addr  = sectors[sno].tt[i].entry;
            tops[r].score = score( &sectors[sno].tt[i] );
         }
      }
   }

   return score_total;
}

