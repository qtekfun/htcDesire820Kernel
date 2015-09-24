

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
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"     
#include "pub_core_mallocfree.h"
#include "pub_core_options.h"
#include "pub_core_stacktrace.h"
#include "pub_core_machine.h"       
#include "pub_core_vki.h"           
#include "pub_core_libcsetjmp.h"    
#include "pub_core_threadstate.h"   
#include "pub_core_execontext.h"    





#define N_EC_PRIMES 18

static SizeT ec_primes[N_EC_PRIMES] = {
         769UL,         1543UL,         3079UL,          6151UL,
       12289UL,        24593UL,        49157UL,         98317UL,
      196613UL,       393241UL,       786433UL,       1572869UL,
     3145739UL,      6291469UL,     12582917UL,      25165843UL,
    50331653UL,    100663319UL
};



struct _ExeContext {
   struct _ExeContext* chain;
   UInt ecu;
   UInt n_ips;
   Addr ips[0];
};


static ExeContext** ec_htab; 
static SizeT        ec_htab_size;     
static SizeT        ec_htab_size_idx; 

static UInt ec_next_ecu = 4; 


static ULong ec_searchreqs;

static ULong ec_searchcmps;

static ULong ec_totstored;

static ULong ec_cmp2s;
static ULong ec_cmp4s;
static ULong ec_cmpAlls;




static void init_ExeContext_storage ( void )
{
   Int i;
   static Bool init_done = False;
   if (LIKELY(init_done))
      return;
   ec_searchreqs = 0;
   ec_searchcmps = 0;
   ec_totstored = 0;
   ec_cmp2s = 0;
   ec_cmp4s = 0;
   ec_cmpAlls = 0;

   ec_htab_size_idx = 0;
   ec_htab_size = ec_primes[ec_htab_size_idx];
   ec_htab = VG_(arena_malloc)(VG_AR_EXECTXT, "execontext.iEs1",
                               sizeof(ExeContext*) * ec_htab_size);
   for (i = 0; i < ec_htab_size; i++)
      ec_htab[i] = NULL;

   init_done = True;
}


void VG_(print_ExeContext_stats) ( void )
{
   init_ExeContext_storage();
   VG_(message)(Vg_DebugMsg, 
      "   exectx: %'lu lists, %'llu contexts (avg %'llu per list)\n",
      ec_htab_size, ec_totstored, ec_totstored / (ULong)ec_htab_size
   );
   VG_(message)(Vg_DebugMsg, 
      "   exectx: %'llu searches, %'llu full compares (%'llu per 1000)\n",
      ec_searchreqs, ec_searchcmps, 
      ec_searchreqs == 0 
         ? 0ULL 
         : ( (ec_searchcmps * 1000ULL) / ec_searchreqs ) 
   );
   VG_(message)(Vg_DebugMsg, 
      "   exectx: %'llu cmp2, %'llu cmp4, %'llu cmpAll\n",
      ec_cmp2s, ec_cmp4s, ec_cmpAlls 
   );
}


void VG_(pp_ExeContext) ( ExeContext* ec )
{
   VG_(pp_StackTrace)( ec->ips, ec->n_ips );
}


Bool VG_(eq_ExeContext) ( VgRes res, ExeContext* e1, ExeContext* e2 )
{
   Int i;

   if (e1 == NULL || e2 == NULL) 
      return False;

   
   tl_assert(e1->n_ips >= 1 && e2->n_ips >= 1);

   switch (res) {
   case Vg_LowRes:
      
      ec_cmp2s++;
      for (i = 0; i < 2; i++) {
         if ( (e1->n_ips <= i) &&  (e2->n_ips <= i)) return True;
         if ( (e1->n_ips <= i) && !(e2->n_ips <= i)) return False;
         if (!(e1->n_ips <= i) &&  (e2->n_ips <= i)) return False;
         if (e1->ips[i] != e2->ips[i])               return False;
      }
      return True;

   case Vg_MedRes:
      
      ec_cmp4s++;
      for (i = 0; i < 4; i++) {
         if ( (e1->n_ips <= i) &&  (e2->n_ips <= i)) return True;
         if ( (e1->n_ips <= i) && !(e2->n_ips <= i)) return False;
         if (!(e1->n_ips <= i) &&  (e2->n_ips <= i)) return False;
         if (e1->ips[i] != e2->ips[i])               return False;
      }
      return True;

   case Vg_HighRes:
      ec_cmpAlls++;
      
      if (e1 != e2) return False;
      return True;

   default:
      VG_(core_panic)("VG_(eq_ExeContext): unrecognised VgRes");
   }
}


static inline UWord ROLW ( UWord w, Int n )
{
   Int bpw = 8 * sizeof(UWord);
   w = (w << n) | (w >> (bpw-n));
   return w;
}

static UWord calc_hash ( Addr* ips, UInt n_ips, UWord htab_sz )
{
   UInt  i;
   UWord hash = 0;
   vg_assert(htab_sz > 0);
   for (i = 0; i < n_ips; i++) {
      hash ^= ips[i];
      hash = ROLW(hash, 19);
   }
   return hash % htab_sz;
}

static void resize_ec_htab ( void )
{
   SizeT        i;
   SizeT        new_size;
   ExeContext** new_ec_htab;

   vg_assert(ec_htab_size_idx >= 0 && ec_htab_size_idx < N_EC_PRIMES);
   if (ec_htab_size_idx == N_EC_PRIMES-1)
      return; 

   new_size = ec_primes[ec_htab_size_idx + 1];
   new_ec_htab = VG_(arena_malloc)(VG_AR_EXECTXT, "execontext.reh1",
                                   sizeof(ExeContext*) * new_size);

   VG_(debugLog)(
      1, "execontext",
         "resizing htab from size %lu to %lu (idx %lu)  Total#ECs=%llu\n",
         ec_htab_size, new_size, ec_htab_size_idx + 1, ec_totstored);

   for (i = 0; i < new_size; i++)
      new_ec_htab[i] = NULL;

   for (i = 0; i < ec_htab_size; i++) {
      ExeContext* cur = ec_htab[i];
      while (cur) {
         ExeContext* next = cur->chain;
         UWord hash = calc_hash(cur->ips, cur->n_ips, new_size);
         vg_assert(hash < new_size);
         cur->chain = new_ec_htab[hash];
         new_ec_htab[hash] = cur;
         cur = next;
      }
   }

   VG_(arena_free)(VG_AR_EXECTXT, ec_htab);
   ec_htab      = new_ec_htab;
   ec_htab_size = new_size;
   ec_htab_size_idx++;
}

static ExeContext* record_ExeContext_wrk2 ( Addr* ips, UInt n_ips ); 
static ExeContext* record_ExeContext_wrk ( ThreadId tid, Word first_ip_delta,
                                           Bool first_ip_only )
{
   Addr ips[VG_(clo_backtrace_size)];
   UInt n_ips;

   init_ExeContext_storage();

   vg_assert(sizeof(void*) == sizeof(UWord));
   vg_assert(sizeof(void*) == sizeof(Addr));

   vg_assert(VG_(is_valid_tid)(tid));

   if (first_ip_only) {
      n_ips = 1;
      ips[0] = VG_(get_IP)(tid) + first_ip_delta;
   } else {
      n_ips = VG_(get_StackTrace)( tid, ips, VG_(clo_backtrace_size),
                                   NULL,
                                   NULL,
                                   first_ip_delta );
   }

   return record_ExeContext_wrk2 ( ips, n_ips );
}

static ExeContext* record_ExeContext_wrk2 ( Addr* ips, UInt n_ips )
{
   Int         i;
   Bool        same;
   UWord       hash;
   ExeContext* new_ec;
   ExeContext* list;
   ExeContext  *prev2, *prev;

   static UInt ctr = 0;

   tl_assert(n_ips >= 1 && n_ips <= VG_(clo_backtrace_size));

   hash = calc_hash( ips, n_ips, ec_htab_size );

   

   ec_searchreqs++;

   prev2 = NULL;
   prev  = NULL;
   list  = ec_htab[hash];

   while (True) {
      if (list == NULL) break;
      ec_searchcmps++;
      same = True;
      for (i = 0; i < n_ips; i++) {
         if (list->ips[i] != ips[i]) {
            same = False;
            break; 
         }
      }
      if (same) break;
      prev2 = prev;
      prev  = list;
      list  = list->chain;
   }

   if (list != NULL) {
      if (0 == ((ctr++) & 7)) {
         if (prev2 != NULL && prev != NULL) {
            
            vg_assert(prev2->chain == prev);
            vg_assert(prev->chain  == list);
            prev2->chain = list;
            prev->chain  = list->chain;
            list->chain  = prev;
         }
         else if (prev2 == NULL && prev != NULL) {
            
            vg_assert(ec_htab[hash] == prev);
            vg_assert(prev->chain == list);
            prev->chain = list->chain;
            list->chain = prev;
            ec_htab[hash] = list;
         }
      }
      return list;
   }

   
   ec_totstored++;

   new_ec = VG_(arena_malloc)( VG_AR_EXECTXT, "execontext.rEw2.2",
                               sizeof(struct _ExeContext) 
                               + n_ips * sizeof(Addr) );

   for (i = 0; i < n_ips; i++)
      new_ec->ips[i] = ips[i];

   vg_assert(VG_(is_plausible_ECU)(ec_next_ecu));
   new_ec->ecu = ec_next_ecu;
   ec_next_ecu += 4;
   if (ec_next_ecu == 0) {
      VG_(core_panic)("m_execontext: more than 2^30 ExeContexts created");
   }

   new_ec->n_ips = n_ips;
   new_ec->chain = ec_htab[hash];
   ec_htab[hash] = new_ec;

   
   if ( ((ULong)ec_totstored) > ((ULong)ec_htab_size) ) {
      vg_assert(ec_htab_size_idx >= 0 && ec_htab_size_idx < N_EC_PRIMES);
      if (ec_htab_size_idx < N_EC_PRIMES-1)
         resize_ec_htab();
   }

   return new_ec;
}

ExeContext* VG_(record_ExeContext)( ThreadId tid, Word first_ip_delta ) {
   return record_ExeContext_wrk( tid, first_ip_delta,
                                      False );
}

ExeContext* VG_(record_depth_1_ExeContext)( ThreadId tid, Word first_ip_delta )
{
   return record_ExeContext_wrk( tid, first_ip_delta,
                                      True );
}

ExeContext* VG_(make_depth_1_ExeContext_from_Addr)( Addr a ) {
   init_ExeContext_storage();
   return record_ExeContext_wrk2( &a, 1 );
}

StackTrace VG_(get_ExeContext_StackTrace) ( ExeContext* e ) {
   return e->ips;
}  

UInt VG_(get_ECU_from_ExeContext)( ExeContext* e ) {
   vg_assert(VG_(is_plausible_ECU)(e->ecu));
   return e->ecu;
}

Int VG_(get_ExeContext_n_ips)( ExeContext* e ) {
   vg_assert(e->n_ips >= 1);
   return e->n_ips;
}

ExeContext* VG_(get_ExeContext_from_ECU)( UInt ecu )
{
   UWord i;
   ExeContext* ec;
   vg_assert(VG_(is_plausible_ECU)(ecu));
   vg_assert(ec_htab_size > 0);
   for (i = 0; i < ec_htab_size; i++) {
      for (ec = ec_htab[i]; ec; ec = ec->chain) {
         if (ec->ecu == ecu)
            return ec;
      }
   }
   return NULL;
}

ExeContext* VG_(make_ExeContext_from_StackTrace)( Addr* ips, UInt n_ips )
{
   return record_ExeContext_wrk2(ips, n_ips);
}

