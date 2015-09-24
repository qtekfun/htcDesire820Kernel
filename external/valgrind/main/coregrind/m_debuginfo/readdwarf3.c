

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2008-2012 OpenWorks LLP
      info@open-works.co.uk

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

   Neither the names of the U.S. Department of Energy nor the
   University of California nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without prior written permission.
*/

#if defined(VGO_linux) || defined(VGO_darwin)


#include "pub_core_basics.h"
#include "pub_core_debuginfo.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcsetjmp.h"   
#include "pub_core_hashtable.h"
#include "pub_core_options.h"
#include "pub_core_tooliface.h"    
#include "pub_core_xarray.h"
#include "pub_core_wordfm.h"
#include "priv_misc.h"             
#include "priv_tytypes.h"
#include "priv_d3basics.h"
#include "priv_storage.h"
#include "priv_readdwarf3.h"       



#define TRACE_D3(format, args...) \
   if (td3) { VG_(printf)(format, ## args); }

#define D3_INVALID_CUOFF  ((UWord)(-1UL))
#define D3_FAKEVOID_CUOFF ((UWord)(-2UL))

typedef
   struct {
      UChar* region_start_img;
      UWord  region_szB;
      UWord  region_next;
      void (*barf)( HChar* ) __attribute__((noreturn));
      HChar* barfstr;
   }
   Cursor;

static inline Bool is_sane_Cursor ( Cursor* c ) {
   if (!c)                return False;
   if (!c->barf)          return False;
   if (!c->barfstr)       return False;
   return True;
}

static void init_Cursor ( Cursor* c,
                          UChar*  region_start_img,
                          UWord   region_szB,
                          UWord   region_next,
                          __attribute__((noreturn)) void (*barf)( HChar* ),
                          HChar*  barfstr )
{
   vg_assert(c);
   VG_(memset)(c, 0, sizeof(*c));
   c->region_start_img = region_start_img;
   c->region_szB       = region_szB;
   c->region_next      = region_next;
   c->barf             = barf;
   c->barfstr          = barfstr;
   vg_assert(is_sane_Cursor(c));
}

static Bool is_at_end_Cursor ( Cursor* c ) {
   vg_assert(is_sane_Cursor(c));
   return c->region_next >= c->region_szB;
}

static inline UWord get_position_of_Cursor ( Cursor* c ) {
   vg_assert(is_sane_Cursor(c));
   return c->region_next;
}
static inline void set_position_of_Cursor ( Cursor* c, UWord pos ) {
   c->region_next = pos;
   vg_assert(is_sane_Cursor(c));
}

static Word get_remaining_length_Cursor ( Cursor* c ) {
   vg_assert(is_sane_Cursor(c));
   return c->region_szB - c->region_next;
}

static UChar* get_address_of_Cursor ( Cursor* c ) {
   vg_assert(is_sane_Cursor(c));
   return &c->region_start_img[ c->region_next ];
}

static inline UChar get_UChar ( Cursor* c ) {
   UChar r;
   
   if (c->region_next + sizeof(UChar) > c->region_szB) {
      c->barf(c->barfstr);
      
      vg_assert(0);
   }
   r = * (UChar*) &c->region_start_img[ c->region_next ];
   c->region_next += sizeof(UChar);
   return r;
}
static UShort get_UShort ( Cursor* c ) {
   UShort r;
   vg_assert(is_sane_Cursor(c));
   if (c->region_next + sizeof(UShort) > c->region_szB) {
      c->barf(c->barfstr);
      
      vg_assert(0);
   }
   r = ML_(read_UShort)(&c->region_start_img[ c->region_next ]);
   c->region_next += sizeof(UShort);
   return r;
}
static UInt get_UInt ( Cursor* c ) {
   UInt r;
   vg_assert(is_sane_Cursor(c));
   if (c->region_next + sizeof(UInt) > c->region_szB) {
      c->barf(c->barfstr);
      
      vg_assert(0);
   }
   r = ML_(read_UInt)(&c->region_start_img[ c->region_next ]);
   c->region_next += sizeof(UInt);
   return r;
}
static ULong get_ULong ( Cursor* c ) {
   ULong r;
   vg_assert(is_sane_Cursor(c));
   if (c->region_next + sizeof(ULong) > c->region_szB) {
      c->barf(c->barfstr);
      
      vg_assert(0);
   }
   r = ML_(read_ULong)(&c->region_start_img[ c->region_next ]);
   c->region_next += sizeof(ULong);
   return r;
}
static inline ULong get_ULEB128 ( Cursor* c ) {
   ULong result;
   Int   shift;
   UChar byte;
   
   byte = get_UChar( c );
   result = (ULong)(byte & 0x7f);
   if (LIKELY(!(byte & 0x80))) return result;
   shift = 7;
   
   do {
      byte = get_UChar( c );
      result |= ((ULong)(byte & 0x7f)) << shift;
      shift += 7;
   } while (byte & 0x80);
   return result;
}
static Long get_SLEB128 ( Cursor* c ) {
   ULong  result = 0;
   Int    shift = 0;
   UChar  byte;
   do {
      byte = get_UChar(c);
      result |= ((ULong)(byte & 0x7f)) << shift;
      shift += 7;
   } while (byte & 0x80);
   if (shift < 64 && (byte & 0x40))
      result |= -(1ULL << shift);
   return result;
}

static UChar* get_AsciiZ ( Cursor* c ) {
   UChar  uc;
   UChar* res = get_address_of_Cursor(c);
   do { uc = get_UChar(c); } while (uc != 0);
   return res;
}

static ULong peek_ULEB128 ( Cursor* c ) {
   Word here = c->region_next;
   ULong r = get_ULEB128( c );
   c->region_next = here;
   return r;
}
static UChar peek_UChar ( Cursor* c ) {
   Word here = c->region_next;
   UChar r = get_UChar( c );
   c->region_next = here;
   return r;
}

static ULong get_Dwarfish_UWord ( Cursor* c, Bool is_dw64 ) {
   return is_dw64 ? get_ULong(c) : (ULong) get_UInt(c);
}

static UWord get_UWord ( Cursor* c ) {
   vg_assert(sizeof(UWord) == sizeof(void*));
   if (sizeof(UWord) == 4) return get_UInt(c);
   if (sizeof(UWord) == 8) return get_ULong(c);
   vg_assert(0);
}

static ULong get_Initial_Length ( Bool* is64,
                                  Cursor* c, 
                                  HChar* barfMsg )
{
   ULong w64;
   UInt  w32;
   *is64 = False;
   w32 = get_UInt( c );
   if (w32 >= 0xFFFFFFF0 && w32 < 0xFFFFFFFF) {
      c->barf( barfMsg );
   }
   else if (w32 == 0xFFFFFFFF) {
      *is64 = True;
      w64   = get_ULong( c );
   } else {
      *is64 = False;
      w64 = (ULong)w32;
   }
   return w64;
}



#define N_ABBV_CACHE 32

typedef
   struct {
      
      void (*barf)( HChar* ) __attribute__((noreturn));
      
      Bool   is_dw64;
      
      UShort version;
      ULong  unit_length;
      
      UWord  cu_start_offset;
      Addr   cu_svma;
      Bool   cu_svma_known;
      
      UChar* debug_abbv;
      
      UWord  debug_abbv_maxszB;
      
      UChar* debug_str_img;
      UWord  debug_str_sz;
      
      UChar* debug_ranges_img;
      UWord  debug_ranges_sz;
      
      UChar* debug_loc_img;
      UWord  debug_loc_sz;
      
      UChar* debug_line_img;
      UWord  debug_line_sz;
      
      UChar* debug_info_img;
      UWord  debug_info_sz;
      
      UChar* debug_types_img;
      UWord  debug_types_sz;
      
      UChar* debug_info_alt_img;
      UWord  debug_info_alt_sz;
      
      UChar* debug_str_alt_img;
      UWord  debug_str_alt_sz;
      UWord  types_cuOff_bias;
      UWord  alt_cuOff_bias;
      
      struct _DebugInfo* di;
      
      
      struct { ULong abbv_code; UWord posn; } saC_cache[N_ABBV_CACHE];
      UWord saC_cache_queries;
      UWord saC_cache_misses;

      Bool is_type_unit;
      ULong type_signature;
      ULong type_offset;

      VgHashTable signature_types;

      Bool is_alt_info;
   }
   CUConst;


static UWord cook_die( CUConst* cc, UWord die )
{
   if (cc->is_type_unit)
      die += cc->types_cuOff_bias;
   else if (cc->is_alt_info)
      die += cc->alt_cuOff_bias;
   return die;
}

static UWord cook_die_using_form( CUConst *cc, UWord die, DW_FORM form)
{
   if (form == DW_FORM_ref_sig8)
      return die;
   if (form == DW_FORM_GNU_ref_alt)
      return die + cc->alt_cuOff_bias;
   return cook_die( cc, die );
}

static UWord uncook_die( CUConst *cc, UWord die, Bool *type_flag,
                         Bool *alt_flag )
{
   *alt_flag = False;
   *type_flag = False;
   if (die >= cc->debug_info_sz) {
      if (die >= cc->debug_info_sz + cc->debug_types_sz) {
         *alt_flag = True;
         die -= cc->debug_info_sz + cc->debug_types_sz;
      } else {
         *type_flag = True;
         die -= cc->debug_info_sz;
      }
   }
   return die;
}




static void bias_GX ( GExpr* gx, struct _DebugInfo* di )
{
   UShort nbytes;
   UChar* p = &gx->payload[0];
   UChar* pA;
   UChar  uc;
   uc = *p++; 
   if (uc == 0)
      return;
   vg_assert(uc == 1);
   p[-1] = 0; 
   while (True) {
      uc = *p++;
      if (uc == 1)
         break; 
      vg_assert(uc == 0);
      
      pA = (UChar*)p;
      ML_(write_Addr)(pA, ML_(read_Addr)(pA) + di->text_debug_bias);
      p += sizeof(Addr);
      
      pA = (UChar*)p;
      ML_(write_Addr)(pA, ML_(read_Addr)(pA) + di->text_debug_bias);
      p += sizeof(Addr);
      
      nbytes = ML_(read_UShort)(p); p += sizeof(UShort);
      p += nbytes;
   }
}

__attribute__((noinline))
static GExpr* make_singleton_GX ( UChar* block, UWord nbytes )
{
   SizeT  bytesReqd;
   GExpr* gx;
   UChar *p, *pstart;

   vg_assert(sizeof(UWord) == sizeof(Addr));
   vg_assert(nbytes <= 0xFFFF); 
   bytesReqd
      =   sizeof(UChar)      + sizeof(UChar) 
        + sizeof(UWord)        + sizeof(UWord) 
        + sizeof(UShort)     + nbytes
        + sizeof(UChar); 

   gx = ML_(dinfo_zalloc)( "di.readdwarf3.msGX.1", 
                           sizeof(GExpr) + bytesReqd );
   vg_assert(gx);

   p = pstart = &gx->payload[0];

   p = ML_(write_UChar)(p, 0);        
   p = ML_(write_UChar)(p, 0);        
   p = ML_(write_Addr)(p, 0);         
   p = ML_(write_Addr)(p, ~0);        
   p = ML_(write_UShort)(p, nbytes);  
   VG_(memcpy)(p, block, nbytes); p += nbytes;
   p = ML_(write_UChar)(p, 1);        

   vg_assert( (SizeT)(p - pstart) == bytesReqd);
   vg_assert( &gx->payload[bytesReqd] 
              == ((UChar*)gx) + sizeof(GExpr) + bytesReqd );

   return gx;
}

__attribute__((noinline))
static GExpr* make_general_GX ( CUConst* cc,
                                Bool     td3,
                                UWord    debug_loc_offset,
                                Addr     svma_of_referencing_CU )
{
   Addr      base;
   Cursor    loc;
   XArray*   xa; 
   GExpr*    gx;
   Word      nbytes;

   vg_assert(sizeof(UWord) == sizeof(Addr));
   if (cc->debug_loc_sz == 0)
      cc->barf("make_general_GX: .debug_loc is empty/missing");

   init_Cursor( &loc, cc->debug_loc_img, 
                cc->debug_loc_sz, 0, cc->barf,
                "Overrun whilst reading .debug_loc section(2)" );
   set_position_of_Cursor( &loc, debug_loc_offset );

   TRACE_D3("make_general_GX (.debug_loc_offset = %lu, img = %p) {\n",
            debug_loc_offset, get_address_of_Cursor( &loc ) );

   
   xa = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.mgGX.1", 
                    ML_(dinfo_free),
                    sizeof(UChar) );

   { UChar c = 1;  VG_(addBytesToXA)( xa, &c, sizeof(c) ); }

   base = 0;
   while (True) {
      Bool  acquire;
      UWord len;
      UWord w1 = get_UWord( &loc );
      UWord w2 = get_UWord( &loc );

      TRACE_D3("   %08lx %08lx\n", w1, w2);
      if (w1 == 0 && w2 == 0)
         break; 

      if (w1 == -1UL) {
         
         base = w2;
         continue;
      }

      
      
      if (w1 > w2) {
         TRACE_D3("negative range is for .debug_loc expr at "
                  "file offset %lu\n", 
                  debug_loc_offset);
         cc->barf( "negative range in .debug_loc section" );
      }

      
      acquire = w1 < w2;
      len     = (UWord)get_UShort( &loc );

      if (acquire) {
         UWord  w;
         UShort s;
         UChar  c;
         c = 0; 
         VG_(addBytesToXA)( xa, &c, sizeof(c) );
         w = w1    + base + svma_of_referencing_CU;
         VG_(addBytesToXA)( xa, &w, sizeof(w) );
         w = w2 -1 + base + svma_of_referencing_CU;
         VG_(addBytesToXA)( xa, &w, sizeof(w) );
         s = (UShort)len;
         VG_(addBytesToXA)( xa, &s, sizeof(s) );
      }

      while (len > 0) {
         UChar byte = get_UChar( &loc );
         TRACE_D3("%02x", (UInt)byte);
         if (acquire)
            VG_(addBytesToXA)( xa, &byte, 1 );
         len--;
      }
      TRACE_D3("\n");
   }

   { UChar c = 1;  VG_(addBytesToXA)( xa, &c, sizeof(c) ); }

   nbytes = VG_(sizeXA)( xa );
   vg_assert(nbytes >= 1);

   gx = ML_(dinfo_zalloc)( "di.readdwarf3.mgGX.2", sizeof(GExpr) + nbytes );
   vg_assert(gx);
   VG_(memcpy)( &gx->payload[0], (UChar*)VG_(indexXA)(xa,0), nbytes );
   vg_assert( &gx->payload[nbytes] 
              == ((UChar*)gx) + sizeof(GExpr) + nbytes );

   VG_(deleteXA)( xa );

   TRACE_D3("}\n");

   return gx;
}



typedef 
   struct { Addr aMin; Addr aMax; }
   AddrRange;


static Word cmp__XArrays_of_AddrRange ( XArray* rngs1, XArray* rngs2 )
{
   Word n1, n2, i;
   tl_assert(rngs1 && rngs2);
   n1 = VG_(sizeXA)( rngs1 );  
   n2 = VG_(sizeXA)( rngs2 );
   if (n1 < n2) return -1;
   if (n1 > n2) return 1;
   for (i = 0; i < n1; i++) {
      AddrRange* rng1 = (AddrRange*)VG_(indexXA)( rngs1, i );
      AddrRange* rng2 = (AddrRange*)VG_(indexXA)( rngs2, i );
      if (rng1->aMin < rng2->aMin) return -1;
      if (rng1->aMin > rng2->aMin) return 1;
      if (rng1->aMax < rng2->aMax) return -1;
      if (rng1->aMax > rng2->aMax) return 1;
   }
   return 0;
}


__attribute__((noinline))
static XArray*  empty_range_list ( void )
{
   XArray* xa; 
   
   xa = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.erl.1",
                    ML_(dinfo_free),
                    sizeof(AddrRange) );
   return xa;
}


__attribute__((noinline))
static XArray* unitary_range_list ( Addr aMin, Addr aMax )
{
   XArray*   xa;
   AddrRange pair;
   vg_assert(aMin <= aMax);
   
   xa = VG_(newXA)( ML_(dinfo_zalloc),  "di.readdwarf3.url.1",
                    ML_(dinfo_free),
                    sizeof(AddrRange) );
   pair.aMin = aMin;
   pair.aMax = aMax;
   VG_(addToXA)( xa, &pair );
   return xa;
}


__attribute__((noinline))
static XArray* 
       get_range_list ( CUConst* cc,
                        Bool     td3,
                        UWord    debug_ranges_offset,
                        Addr     svma_of_referencing_CU )
{
   Addr      base;
   Cursor    ranges;
   XArray*   xa; 
   AddrRange pair;

   if (cc->debug_ranges_sz == 0)
      cc->barf("get_range_list: .debug_ranges is empty/missing");

   init_Cursor( &ranges, cc->debug_ranges_img, 
                cc->debug_ranges_sz, 0, cc->barf,
                "Overrun whilst reading .debug_ranges section(2)" );
   set_position_of_Cursor( &ranges, debug_ranges_offset );

   
   xa = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.grl.1", ML_(dinfo_free),
                    sizeof(AddrRange) );
   base = 0;
   while (True) {
      UWord w1 = get_UWord( &ranges );
      UWord w2 = get_UWord( &ranges );

      if (w1 == 0 && w2 == 0)
         break; 

      if (w1 == -1UL) {
         
         base = w2;
         continue;
      }

      
      if (w1 > w2)
         cc->barf( "negative range in .debug_ranges section" );
      if (w1 < w2) {
         pair.aMin = w1     + base + svma_of_referencing_CU;
         pair.aMax = w2 - 1 + base + svma_of_referencing_CU;
         vg_assert(pair.aMin <= pair.aMax);
         VG_(addToXA)( xa, &pair );
      }
   }
   return xa;
}


static __attribute__((noinline))
void parse_CU_Header ( CUConst* cc,
                       Bool td3,
                       Cursor* c, 
                       UChar* debug_abbv_img, UWord debug_abbv_sz,
		       Bool type_unit,
                       Bool alt_info )
{
   UChar  address_size;
   UWord  debug_abbrev_offset;
   Int    i;

   VG_(memset)(cc, 0, sizeof(*cc));
   vg_assert(c && c->barf);
   cc->barf = c->barf;

   
   cc->unit_length 
      = get_Initial_Length( &cc->is_dw64, c, 
           "parse_CU_Header: invalid initial-length field" );

   TRACE_D3("   Length:        %lld\n", cc->unit_length );

   
   cc->version = get_UShort( c );
   if (cc->version != 2 && cc->version != 3 && cc->version != 4)
      cc->barf( "parse_CU_Header: is neither DWARF2 nor DWARF3 nor DWARF4" );
   TRACE_D3("   Version:       %d\n", (Int)cc->version );

   
   debug_abbrev_offset = get_Dwarfish_UWord( c, cc->is_dw64 );
   if (debug_abbrev_offset >= debug_abbv_sz)
      cc->barf( "parse_CU_Header: invalid debug_abbrev_offset" );
   TRACE_D3("   Abbrev Offset: %ld\n", debug_abbrev_offset );

   address_size = get_UChar( c );
   if (address_size != sizeof(void*))
      cc->barf( "parse_CU_Header: invalid address_size" );
   TRACE_D3("   Pointer Size:  %d\n", (Int)address_size );

   cc->is_type_unit = type_unit;
   cc->is_alt_info = alt_info;

   if (type_unit) {
      cc->type_signature = get_ULong( c );
      cc->type_offset = get_Dwarfish_UWord( c, cc->is_dw64 );
   }

   cc->debug_abbv        = debug_abbv_img + debug_abbrev_offset;
   cc->debug_abbv_maxszB = debug_abbv_sz  - debug_abbrev_offset;
   
   if (0) VG_(printf)("XXXXXX initialise set_abbv_Cursor cache\n");
   for (i = 0; i < N_ABBV_CACHE; i++) {
      cc->saC_cache[i].abbv_code = (ULong)-1; 
      cc->saC_cache[i].posn = 0;
   }
   cc->saC_cache_queries = 0;
   cc->saC_cache_misses = 0;
}


static __attribute__((noinline))
void set_abbv_Cursor ( Cursor* c, Bool td3,
                       CUConst* cc, ULong abbv_code )
{
   Int   i;
   ULong acode;

   if (abbv_code == 0)
      cc->barf("set_abbv_Cursor: abbv_code == 0" );

   vg_assert(abbv_code != (ULong)-1);

   
   if (0) VG_(printf)("XXXXXX search set_abbv_Cursor cache\n");
   cc->saC_cache_queries++;
   for (i = 0; i < N_ABBV_CACHE; i++) {
     if (cc->saC_cache[i].abbv_code == abbv_code) {
        if (0) VG_(printf)("XXXXXX found in set_abbv_Cursor cache\n");
        init_Cursor( c, cc->debug_abbv,
                     cc->debug_abbv_maxszB, cc->saC_cache[i].posn, 
                     cc->barf,
                     "Overrun whilst parsing .debug_abbrev section(1)" );
        if (i > 0) {
           ULong t_abbv_code = cc->saC_cache[i].abbv_code;
           UWord t_posn = cc->saC_cache[i].posn;
           while (i > 0) {
              cc->saC_cache[i] = cc->saC_cache[i-1];
              cc->saC_cache[0].abbv_code = t_abbv_code;
              cc->saC_cache[0].posn = t_posn;
              i--;
           }
        }
        return;
     }
   }


   cc->saC_cache_misses++;
   init_Cursor( c, cc->debug_abbv, cc->debug_abbv_maxszB, 0, cc->barf,
               "Overrun whilst parsing .debug_abbrev section(2)" );

   while (True) {
      
      
      acode = get_ULEB128( c );
      if (acode == 0) break; 
      if (acode == abbv_code) break; 
       get_ULEB128( c );
       get_UChar( c );
      
      
      while (True) {
         ULong at_name = get_ULEB128( c );
         ULong at_form = get_ULEB128( c );
         if (at_name == 0 && at_form == 0) break;
         
         
      }
   }

   if (acode == 0) {
      
      cc->barf("set_abbv_Cursor: abbv_code not found");
   }

   for (i = N_ABBV_CACHE-1; i > N_ABBV_CACHE/2; i--) {
      cc->saC_cache[i] = cc->saC_cache[i-1];
   }
   if (0) VG_(printf)("XXXXXX update set_abbv_Cursor cache\n");
   cc->saC_cache[N_ABBV_CACHE/2].abbv_code = abbv_code;
   cc->saC_cache[N_ABBV_CACHE/2].posn = get_position_of_Cursor(c);
}

typedef
   struct D3SignatureType {
      struct D3SignatureType *next;
      UWord data;
      ULong type_signature;
      UWord die;
   }
   D3SignatureType;

static void record_signatured_type ( VgHashTable tab,
                                     ULong type_signature,
                                     UWord die )
{
   D3SignatureType *dstype = ML_(dinfo_zalloc) ( "di.readdwarf3.sigtype",
                                                 sizeof(D3SignatureType) );
   dstype->data = (UWord) type_signature;
   dstype->type_signature = type_signature;
   dstype->die = die;
   VG_(HT_add_node) ( tab, dstype );
}

static UWord lookup_signatured_type ( VgHashTable tab,
                                      ULong type_signature,
                                      void (*barf)( HChar* ) __attribute__((noreturn)) )
{
   D3SignatureType *dstype = VG_(HT_lookup) ( tab, (UWord) type_signature );
   while ( dstype != NULL && dstype->type_signature != type_signature)
      dstype = dstype->next;
   if (dstype == NULL) {
      barf("lookup_signatured_type: could not find signatured type");
      
      vg_assert(0);
   }
   return dstype->die;
}

static
void get_Form_contents ( ULong* cts,
                         Int*   ctsSzB,
                         UWord* ctsMemSzB,
                         CUConst* cc, Cursor* c,
                         Bool td3, DW_FORM form )
{
   *cts       = 0;
   *ctsSzB    = 0;
   *ctsMemSzB = 0;
   switch (form) {
      case DW_FORM_data1:
         *cts = (ULong)(UChar)get_UChar(c);
         *ctsSzB = 1;
         TRACE_D3("%u", (UInt)*cts);
         break;
      case DW_FORM_data2:
         *cts = (ULong)(UShort)get_UShort(c);
         *ctsSzB = 2;
         TRACE_D3("%u", (UInt)*cts);
         break;
      case DW_FORM_data4:
         *cts = (ULong)(UInt)get_UInt(c);
         *ctsSzB = 4;
         TRACE_D3("%u", (UInt)*cts);
         break;
      case DW_FORM_data8:
         *cts = get_ULong(c);
         *ctsSzB = 8;
         TRACE_D3("%llu", *cts);
         break;
      case DW_FORM_sec_offset:
         *cts = (ULong)get_Dwarfish_UWord( c, cc->is_dw64 );
         *ctsSzB = cc->is_dw64 ? 8 : 4;
         TRACE_D3("%llu", *cts);
         break;
      case DW_FORM_sdata:
         *cts = (ULong)(Long)get_SLEB128(c);
         *ctsSzB = 8;
         TRACE_D3("%lld", (Long)*cts);
         break;
      case DW_FORM_udata:
         *cts = (ULong)(Long)get_ULEB128(c);
         *ctsSzB = 8;
         TRACE_D3("%llu", (Long)*cts);
         break;
      case DW_FORM_addr:
         *cts = (ULong)(UWord)get_UWord(c);
         *ctsSzB = sizeof(UWord);
         TRACE_D3("0x%lx", (UWord)*cts);
         break;

      case DW_FORM_ref_addr:
         
         if (cc->version == 2) {
            *cts = (ULong)(UWord)get_UWord(c);
            *ctsSzB = sizeof(UWord);
         } else {
            *cts = get_Dwarfish_UWord(c, cc->is_dw64);
            *ctsSzB = cc->is_dw64 ? sizeof(ULong) : sizeof(UInt);
         }
         TRACE_D3("0x%lx", (UWord)*cts);
         if (0) VG_(printf)("DW_FORM_ref_addr 0x%lx\n", (UWord)*cts);
         if (
             cc->debug_info_img == NULL || cc->debug_info_sz == 0
             || *cts >= (ULong)cc->debug_info_sz) {
            cc->barf("get_Form_contents: DW_FORM_ref_addr points "
                     "outside .debug_info");
         }
         break;

      case DW_FORM_strp: {
         
         UChar* str;
         UWord uw = (UWord)get_Dwarfish_UWord( c, cc->is_dw64 );
         if (cc->debug_str_img == NULL || uw >= cc->debug_str_sz)
            cc->barf("get_Form_contents: DW_FORM_strp "
                     "points outside .debug_str");
         str = (UChar*)cc->debug_str_img + uw;
         TRACE_D3("(indirect string, offset: 0x%lx): %s", uw, str);
         *cts = (ULong)(UWord)str;
         *ctsMemSzB = 1 + (ULong)VG_(strlen)(str);
         break;
      }
      case DW_FORM_string: {
         UChar* str = get_AsciiZ(c);
         TRACE_D3("%s", str);
         *cts = (ULong)(UWord)str;
         *ctsMemSzB = 1 + (ULong)VG_(strlen)(str);
         break;
      }
      case DW_FORM_ref1: {
         UChar  u8 = get_UChar(c);
         UWord res = cc->cu_start_offset + (UWord)u8;
         *cts = (ULong)res;
         *ctsSzB = sizeof(UWord);
         TRACE_D3("<%lx>", res);
         break;
      }
      case DW_FORM_ref2: {
         UShort  u16 = get_UShort(c);
         UWord res = cc->cu_start_offset + (UWord)u16;
         *cts = (ULong)res;
         *ctsSzB = sizeof(UWord);
         TRACE_D3("<%lx>", res);
         break;
      }
      case DW_FORM_ref4: {
         UInt  u32 = get_UInt(c);
         UWord res = cc->cu_start_offset + (UWord)u32;
         *cts = (ULong)res;
         *ctsSzB = sizeof(UWord);
         TRACE_D3("<%lx>", res);
         break;
      }
      case DW_FORM_ref8: {
         ULong  u64 = get_ULong(c);
         UWord res = cc->cu_start_offset + (UWord)u64;
         *cts = (ULong)res;
         *ctsSzB = sizeof(UWord);
         TRACE_D3("<%lx>", res);
         break;
      }
      case DW_FORM_ref_udata: {
         ULong  u64 = get_ULEB128(c);
         UWord res = cc->cu_start_offset + (UWord)u64;
         *cts = (ULong)res;
         *ctsSzB = sizeof(UWord);
         TRACE_D3("<%lx>", res);
         break;
      }
      case DW_FORM_flag: {
         UChar u8 = get_UChar(c);
         TRACE_D3("%u", (UInt)u8);
         *cts = (ULong)u8;
         *ctsSzB = 1;
         break;
      }
      case DW_FORM_flag_present:
         TRACE_D3("1");
         *cts = 1;
         *ctsSzB = 1;
         break;
      case DW_FORM_block1: {
         ULong  u64b;
         ULong  u64 = (ULong)get_UChar(c);
         UChar* block = get_address_of_Cursor(c);
         TRACE_D3("%llu byte block: ", u64);
         for (u64b = u64; u64b > 0; u64b--) {
            UChar u8 = get_UChar(c);
            TRACE_D3("%x ", (UInt)u8);
         }
         *cts = (ULong)(UWord)block;
         *ctsMemSzB = (UWord)u64;
         break;
      }
      case DW_FORM_block2: {
         ULong  u64b;
         ULong  u64 = (ULong)get_UShort(c);
         UChar* block = get_address_of_Cursor(c);
         TRACE_D3("%llu byte block: ", u64);
         for (u64b = u64; u64b > 0; u64b--) {
            UChar u8 = get_UChar(c);
            TRACE_D3("%x ", (UInt)u8);
         }
         *cts = (ULong)(UWord)block;
         *ctsMemSzB = (UWord)u64;
         break;
      }
      case DW_FORM_block4: {
         ULong  u64b;
         ULong  u64 = (ULong)get_UInt(c);
         UChar* block = get_address_of_Cursor(c);
         TRACE_D3("%llu byte block: ", u64);
         for (u64b = u64; u64b > 0; u64b--) {
            UChar u8 = get_UChar(c);
            TRACE_D3("%x ", (UInt)u8);
         }
         *cts = (ULong)(UWord)block;
         *ctsMemSzB = (UWord)u64;
         break;
      }
      case DW_FORM_exprloc:
      case DW_FORM_block: {
         ULong  u64b;
         ULong  u64 = (ULong)get_ULEB128(c);
         UChar* block = get_address_of_Cursor(c);
         TRACE_D3("%llu byte block: ", u64);
         for (u64b = u64; u64b > 0; u64b--) {
            UChar u8 = get_UChar(c);
            TRACE_D3("%x ", (UInt)u8);
         }
         *cts = (ULong)(UWord)block;
         *ctsMemSzB = (UWord)u64;
         break;
      }
      case DW_FORM_ref_sig8: {
         ULong  u64b;
         ULong  signature = get_ULong (c);
         ULong  work = signature;
         TRACE_D3("8 byte signature: ");
         for (u64b = 8; u64b > 0; u64b--) {
            UChar u8 = work & 0xff;
            TRACE_D3("%x ", (UInt)u8);
            work >>= 8;
         }
         *cts = lookup_signatured_type (cc->signature_types, signature,
                                        c->barf);
         *ctsSzB = sizeof(UWord);
         break;
      }
      case DW_FORM_indirect:
         get_Form_contents (cts, ctsSzB, ctsMemSzB, cc, c, td3,
                            (DW_FORM)get_ULEB128(c));
         return;

      case DW_FORM_GNU_ref_alt:
         *cts = get_Dwarfish_UWord(c, cc->is_dw64);
         *ctsSzB = cc->is_dw64 ? sizeof(ULong) : sizeof(UInt);
         TRACE_D3("0x%lx", (UWord)*cts);
         if (0) VG_(printf)("DW_FORM_GNU_ref_alt 0x%lx\n", (UWord)*cts);
         if (
             cc->debug_info_alt_img == NULL || cc->debug_info_alt_sz == 0
             || *cts >= (ULong)cc->debug_info_alt_sz) {
            cc->barf("get_Form_contents: DW_FORM_ref_addr points "
                     "outside alternate .debug_info");
         }
         break;

      case DW_FORM_GNU_strp_alt: {
         
         UChar* str;
         UWord uw = (UWord)get_Dwarfish_UWord( c, cc->is_dw64 );
         if (cc->debug_str_alt_img == NULL || uw >= cc->debug_str_alt_sz)
            cc->barf("get_Form_contents: DW_FORM_GNU_strp_alt "
                     "points outside alternate .debug_str");
         str = (UChar*)cc->debug_str_alt_img + uw;
         TRACE_D3("(indirect alt string, offset: 0x%lx): %s", uw, str);
         *cts = (ULong)(UWord)str;
         *ctsMemSzB = 1 + (ULong)VG_(strlen)(str);
         break;
      }

      default:
         VG_(printf)(
            "get_Form_contents: unhandled %d (%s) at <%lx>\n",
            form, ML_(pp_DW_FORM)(form), get_position_of_Cursor(c));
         c->barf("get_Form_contents: unhandled DW_FORM");
   }
}



typedef
   struct _TempVar {
      UChar*  name; 
      UWord   nRanges;
      Addr    rngOneMin;
      Addr    rngOneMax;
      XArray* rngMany; 
      
      Int     level;
      UWord   typeR; 
      GExpr*  gexpr; 
      GExpr*  fbGX;  
      UChar*  fName; 
      Int     fLine; 
      UWord   dioff;
      UWord   absOri; 
   }
   TempVar;

#define N_D3_VAR_STACK 48

typedef
   struct {
      Int     sp; 
      XArray* ranges[N_D3_VAR_STACK]; 
      Int     level[N_D3_VAR_STACK];  
      Bool    isFunc[N_D3_VAR_STACK]; 
      GExpr*  fbGX[N_D3_VAR_STACK];   
      XArray*  filenameTable;
   }
   D3VarParser;

static void varstack_show ( D3VarParser* parser, HChar* str ) {
   Word i, j;
   VG_(printf)("  varstack (%s) {\n", str);
   for (i = 0; i <= parser->sp; i++) {
      XArray* xa = parser->ranges[i];
      vg_assert(xa);
      VG_(printf)("    [%ld] (level %d)", i, parser->level[i]);
      if (parser->isFunc[i]) {
         VG_(printf)(" (fbGX=%p)", parser->fbGX[i]);
      } else {
         vg_assert(parser->fbGX[i] == NULL);
      }
      VG_(printf)(": ");
      if (VG_(sizeXA)( xa ) == 0) {
         VG_(printf)("** empty PC range array **");
      } else {
         for (j = 0; j < VG_(sizeXA)( xa ); j++) {
            AddrRange* range = (AddrRange*) VG_(indexXA)( xa, j );
            vg_assert(range);
            VG_(printf)("[%#lx,%#lx] ", range->aMin, range->aMax);
         }
      }
      VG_(printf)("\n");
   }
   VG_(printf)("  }\n");
}

static 
void varstack_preen ( D3VarParser* parser, Bool td3, Int level )
{
   Bool changed = False;
   vg_assert(parser->sp < N_D3_VAR_STACK);
   while (True) {
      vg_assert(parser->sp >= -1);
      if (parser->sp == -1) break;
      if (parser->level[parser->sp] <= level) break;
      if (0) 
         TRACE_D3("BBBBAAAA varstack_pop [newsp=%d]\n", parser->sp-1);
      vg_assert(parser->ranges[parser->sp]);
      VG_(deleteXA)( parser->ranges[parser->sp] );
      parser->ranges[parser->sp] = NULL;
      parser->level[parser->sp]  = 0;
      parser->isFunc[parser->sp] = False;
      parser->fbGX[parser->sp]   = NULL;
      parser->sp--;
      changed = True;
   }
   if (changed && td3)
      varstack_show( parser, "after preen" );
}

static void varstack_push ( CUConst* cc,
                            D3VarParser* parser,
                            Bool td3,
                            XArray* ranges, Int level,
                            Bool    isFunc, GExpr* fbGX ) {
   if (0)
   TRACE_D3("BBBBAAAA varstack_push[newsp=%d]: %d  %p\n",
            parser->sp+1, level, ranges);

   varstack_preen(parser, False, level-1);

   vg_assert(parser->sp >= -1);
   vg_assert(parser->sp < N_D3_VAR_STACK);
   if (parser->sp == N_D3_VAR_STACK-1)
      cc->barf("varstack_push: N_D3_VAR_STACK is too low; "
               "increase and recompile");
   if (parser->sp >= 0)
      vg_assert(parser->level[parser->sp] < level);
   parser->sp++;
   vg_assert(parser->ranges[parser->sp] == NULL);
   vg_assert(parser->level[parser->sp]  == 0);
   vg_assert(parser->isFunc[parser->sp] == False);
   vg_assert(parser->fbGX[parser->sp]   == NULL);
   vg_assert(ranges != NULL);
   if (!isFunc) vg_assert(fbGX == NULL);
   parser->ranges[parser->sp] = ranges;
   parser->level[parser->sp]  = level;
   parser->isFunc[parser->sp] = isFunc;
   parser->fbGX[parser->sp]   = fbGX;
   if (td3)
      varstack_show( parser, "after push" );
}


__attribute__((noinline))
static GExpr* get_GX ( CUConst* cc, Bool td3, 
                       ULong cts, Int ctsSzB, UWord ctsMemSzB )
{
   GExpr* gexpr = NULL;
   if (ctsMemSzB > 0 && ctsSzB == 0) {
      gexpr = make_singleton_GX( (UChar*)(UWord)cts, ctsMemSzB );
   }
   else 
   if (ctsMemSzB == 0 && ctsSzB > 0) {
      if (!cc->cu_svma_known)
         cc->barf("get_GX: location list, but CU svma is unknown");
      gexpr = make_general_GX( cc, td3, (UWord)cts, cc->cu_svma );
   }
   else {
      vg_assert(0); 
   }
   return gexpr;
}


static 
void read_filename_table( D3VarParser* parser,
                          CUConst* cc, UWord debug_line_offset,
                          Bool td3 )
{
   Bool   is_dw64;
   Cursor c;
   Word   i;
   UShort version;
   UChar  opcode_base;
   UChar* str;

   vg_assert(parser && cc && cc->barf);
   if ((!cc->debug_line_img) 
       || cc->debug_line_sz <= debug_line_offset)
      cc->barf("read_filename_table: .debug_line is missing?");

   init_Cursor( &c, cc->debug_line_img, 
                cc->debug_line_sz, debug_line_offset, cc->barf, 
                "Overrun whilst reading .debug_line section(1)" );

   
      get_Initial_Length( &is_dw64, &c,
           "read_filename_table: invalid initial-length field" );
   version = get_UShort( &c );
   if (version != 2 && version != 3 && version != 4)
     cc->barf("read_filename_table: Only DWARF version 2, 3 and 4 line info "
              "is currently supported.");
    get_Dwarfish_UWord( &c, is_dw64 );
    get_UChar( &c );
   if (version >= 4)
       get_UChar( &c );
    get_UChar( &c );
    get_UChar( &c );
    get_UChar( &c );
   opcode_base                = get_UChar( &c );
   
   for (i = 1; i < (Word)opcode_base; i++)
     (void)get_UChar( &c );

   
   while (peek_UChar(&c) != 0) {
     (void)get_AsciiZ(&c);
   }
   (void)get_UChar(&c); 

   
   vg_assert(parser->filenameTable);
   vg_assert( VG_(sizeXA)( parser->filenameTable ) == 0 );
   str = ML_(addStr)( cc->di, "<unknown_file>", -1 );
   VG_(addToXA)( parser->filenameTable, &str );
   while (peek_UChar(&c) != 0) {
      str = get_AsciiZ(&c);
      TRACE_D3("  read_filename_table: %ld %s\n",
               VG_(sizeXA)(parser->filenameTable), str);
      str = ML_(addStr)( cc->di, str, -1 );
      VG_(addToXA)( parser->filenameTable, &str );
      (void)get_ULEB128( &c ); 
      (void)get_ULEB128( &c ); 
      (void)get_ULEB128( &c ); 
   }
   
}


__attribute__((noinline))
static void parse_var_DIE (
   WordFM*  rangestree,
   XArray*  tempvars,
   XArray*  gexprs,
   D3VarParser* parser,
   DW_TAG dtag,
   UWord posn,
   Int level,
   Cursor* c_die,
   Cursor* c_abbv,
   CUConst* cc,
   Bool td3
)
{
   ULong       cts;
   Int         ctsSzB;
   UWord       ctsMemSzB;

   UWord saved_die_c_offset  = get_position_of_Cursor( c_die );
   UWord saved_abbv_c_offset = get_position_of_Cursor( c_abbv );
   Bool  debug_types_flag;
   Bool  alt_flag;

   varstack_preen( parser, td3, level-1 );

   if (dtag == DW_TAG_compile_unit
       || dtag == DW_TAG_type_unit
       || dtag == DW_TAG_partial_unit) {
      Bool have_lo    = False;
      Bool have_hi1   = False;
      Bool hiIsRelative = False;
      Bool have_range = False;
      Addr ip_lo    = 0;
      Addr ip_hi1   = 0;
      Addr rangeoff = 0;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_low_pc && ctsSzB > 0) {
            ip_lo   = cts;
            have_lo = True;
         }
         if (attr == DW_AT_high_pc && ctsSzB > 0) {
            ip_hi1   = cts;
            have_hi1 = True;
            if (form != DW_FORM_addr)
               hiIsRelative = True;
         }
         if (attr == DW_AT_ranges && ctsSzB > 0) {
            rangeoff = cts;
            have_range = True;
         }
         if (attr == DW_AT_stmt_list && ctsSzB > 0) {
            read_filename_table( parser, cc, (UWord)cts, td3 );
         }
      }
      if (have_lo && have_hi1 && hiIsRelative)
         ip_hi1 += ip_lo;
#if 0
      if (level == 0 && have_lo) {
         vg_assert(!cc->cu_svma_known); 
         cc->cu_svma_known = True;
         cc->cu_svma = ip_lo;
         if (1)
            TRACE_D3("BBBBAAAA acquire CU_SVMA of %p\n", cc->cu_svma);
      }
#else
      if (level == 0) {
         vg_assert(!cc->cu_svma_known);
         cc->cu_svma_known = True;
         if (have_lo)
            cc->cu_svma = ip_lo;
         else
            cc->cu_svma = 0;
      }
#endif
      
      if (have_lo && have_hi1 && (!have_range)) {
         if (ip_lo < ip_hi1)
            varstack_push( cc, parser, td3, 
                           unitary_range_list(ip_lo, ip_hi1 - 1),
                           level,
                           False, NULL );
      } else
      if ((!have_lo) && (!have_hi1) && have_range) {
         varstack_push( cc, parser, td3, 
                        get_range_list( cc, td3,
                                        rangeoff, cc->cu_svma ),
                        level,
                        False, NULL );
      } else
      if ((!have_lo) && (!have_hi1) && (!have_range)) {
         
         varstack_push( cc, parser, td3, 
                        empty_range_list(),
                        level,
                        False, NULL );
      } else
      if (have_lo && (!have_hi1) && have_range && ip_lo == 0) {
         varstack_push( cc, parser, td3, 
                        get_range_list( cc, td3,
                                        rangeoff, cc->cu_svma ),
                        level,
                        False, NULL );
      } else {
         if (0) VG_(printf)("I got hlo %d hhi1 %d hrange %d\n",
                            (Int)have_lo, (Int)have_hi1, (Int)have_range);
         goto bad_DIE;
      }
   }

   if (dtag == DW_TAG_lexical_block || dtag == DW_TAG_subprogram) {
      Bool   have_lo    = False;
      Bool   have_hi1   = False;
      Bool   have_range = False;
      Bool   hiIsRelative = False;
      Addr   ip_lo      = 0;
      Addr   ip_hi1     = 0;
      Addr   rangeoff   = 0;
      Bool   isFunc     = dtag == DW_TAG_subprogram;
      GExpr* fbGX       = NULL;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_low_pc && ctsSzB > 0) {
            ip_lo   = cts;
            have_lo = True;
         }
         if (attr == DW_AT_high_pc && ctsSzB > 0) {
            ip_hi1   = cts;
            have_hi1 = True;
            if (form != DW_FORM_addr)
               hiIsRelative = True;
         }
         if (attr == DW_AT_ranges && ctsSzB > 0) {
            rangeoff = cts;
            have_range = True;
         }
         if (isFunc
             && attr == DW_AT_frame_base
             && ((ctsMemSzB > 0 && ctsSzB == 0)
                 || (ctsMemSzB == 0 && ctsSzB > 0))) {
            fbGX = get_GX( cc, False, cts, ctsSzB, ctsMemSzB );
            vg_assert(fbGX);
            VG_(addToXA)(gexprs, &fbGX);
         }
      }
      if (have_lo && have_hi1 && hiIsRelative)
         ip_hi1 += ip_lo;
      
      if (dtag == DW_TAG_subprogram 
          && (!have_lo) && (!have_hi1) && (!have_range)) {
      } else
      if (dtag == DW_TAG_lexical_block
          && (!have_lo) && (!have_hi1) && (!have_range)) {
      } else
      if (have_lo && have_hi1 && (!have_range)) {
         
         if (ip_lo < ip_hi1)
            varstack_push( cc, parser, td3, 
                           unitary_range_list(ip_lo, ip_hi1 - 1),
                           level, isFunc, fbGX );
      } else
      if ((!have_lo) && (!have_hi1) && have_range) {
         varstack_push( cc, parser, td3, 
                        get_range_list( cc, td3,
                                        rangeoff, cc->cu_svma ),
                        level, isFunc, fbGX );
      } else
      if (have_lo && (!have_hi1) && (!have_range)) {
         
      } else
         goto bad_DIE;
   }

   if (dtag == DW_TAG_variable || dtag == DW_TAG_formal_parameter) {
      UChar* name        = NULL;
      UWord  typeR       = D3_INVALID_CUOFF;
      Bool   external    = False;
      GExpr* gexpr       = NULL;
      Int    n_attrs     = 0;
      UWord  abs_ori     = (UWord)D3_INVALID_CUOFF;
      Int    lineNo      = 0;
      UChar* fileName    = NULL;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         n_attrs++;
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            name = ML_(addStr)( cc->di, (UChar*)(UWord)cts, -1 );
         }
         if (attr == DW_AT_location
             && ((ctsMemSzB > 0 && ctsSzB == 0)
                 || (ctsMemSzB == 0 && ctsSzB > 0))) {
            gexpr = get_GX( cc, False, cts, ctsSzB, ctsMemSzB );
            vg_assert(gexpr);
            VG_(addToXA)(gexprs, &gexpr);
         }
         if (attr == DW_AT_type && ctsSzB > 0) {
            typeR = cook_die_using_form( cc, (UWord)cts, form );
         }
         if (attr == DW_AT_external && ctsSzB > 0 && cts > 0) {
            external = True;
         }
         if (attr == DW_AT_abstract_origin && ctsSzB > 0) {
            abs_ori = (UWord)cts;
         }
         if (attr == DW_AT_declaration && ctsSzB > 0 && cts > 0) {
            
         }
         if (attr == DW_AT_decl_line && ctsSzB > 0) {
            lineNo = (Int)cts;
         }
         if (attr == DW_AT_decl_file && ctsSzB > 0) {
            Int ftabIx = (Int)cts;
            if (ftabIx >= 1
                && ftabIx < VG_(sizeXA)( parser->filenameTable )) {
               fileName = *(UChar**)
                          VG_(indexXA)( parser->filenameTable, ftabIx );
               vg_assert(fileName);
            }
            if (0) VG_(printf)("XXX filename = %s\n", fileName);
         }
      }
      if (  (gexpr && typeR != D3_INVALID_CUOFF) 
            || (typeR != D3_INVALID_CUOFF)
            || (gexpr && abs_ori != (UWord)D3_INVALID_CUOFF) ) {

         GExpr*   fbGX = NULL;
         Word     i, nRanges;
         XArray*   xa;
         TempVar* tv;
         vg_assert(parser->sp >= 0);

         if (!external) {
            Bool found = False;
            for (i = parser->sp; i >= 0; i--) {
               if (parser->isFunc[i]) {
                  fbGX = parser->fbGX[i];
                  found = True;
                  break;
               }
            }
            if (!found) {
               if (0 && VG_(clo_verbosity) >= 0) {
                  VG_(message)(Vg_DebugMsg, 
                     "warning: parse_var_DIE: non-external variable "
                     "outside DW_TAG_subprogram\n");
               }
               
            }
         }

         xa = parser->ranges[external ? 0 : parser->sp];
         nRanges = VG_(sizeXA)(xa);
         vg_assert(nRanges >= 0);

         tv = ML_(dinfo_zalloc)( "di.readdwarf3.pvD.1", sizeof(TempVar) );
         tv->name   = name;
         tv->level  = external ? 0 : parser->sp;
         tv->typeR  = typeR;
         tv->gexpr  = gexpr;
         tv->fbGX   = fbGX;
         tv->fName  = fileName;
         tv->fLine  = lineNo;
         tv->dioff  = posn;
         tv->absOri = abs_ori;

         tv->nRanges = nRanges;
         tv->rngOneMin = 0;
         tv->rngOneMax = 0;
         tv->rngMany = NULL;
         if (nRanges == 1) {
            AddrRange* range = VG_(indexXA)(xa, 0);
            tv->rngOneMin = range->aMin;
            tv->rngOneMax = range->aMax;
         }
         else if (nRanges > 1) {
            UWord keyW, valW;
            if (VG_(lookupFM)( rangestree, &keyW, &valW, (UWord)xa )) {
               XArray* old = (XArray*)keyW;
               tl_assert(valW == 0);
               tl_assert(old != xa);
               tv->rngMany = old;
            } else {
               XArray* cloned = VG_(cloneXA)( "di.readdwarf3.pvD.2", xa );
               tv->rngMany = cloned;
               VG_(addToFM)( rangestree, (UWord)cloned, 0 );
            }
         }

         VG_(addToXA)( tempvars, &tv );

         TRACE_D3("  Recording this variable, with %ld PC range(s)\n",
                  VG_(sizeXA)(xa) );
         if (0) {
            static Int ntot=0, ngt=0;
            ntot++;
            if (tv->rngMany) ngt++;
            if (0 == (ntot % 100000))
               VG_(printf)("XXXX %d tot, %d cloned\n", ntot, ngt);
         }

      }

      /* Here are some other weird cases seen in the wild:

            We have a variable with a name and a type, but no
            location.  I guess that's a sign that it has been
            optimised away.  Ignore it.  Here's an example:

            static Int lc_compar(void* n1, void* n2) {
               MC_Chunk* mc1 = *(MC_Chunk**)n1;
               MC_Chunk* mc2 = *(MC_Chunk**)n2;
               return (mc1->data < mc2->data ? -1 : 1);
            }

            Both mc1 and mc2 are like this
            <2><5bc>: Abbrev Number: 21 (DW_TAG_variable)
                DW_AT_name        : mc1
                DW_AT_decl_file   : 1
                DW_AT_decl_line   : 216
                DW_AT_type        : <5d3>

            whereas n1 and n2 do have locations specified.

            ---------------------------------------------

            We see a DW_TAG_formal_parameter with a type, but
            no name and no location.  It's probably part of a function type
            construction, thusly, hence ignore it:
         <1><2b4>: Abbrev Number: 12 (DW_TAG_subroutine_type)
             DW_AT_sibling     : <2c9>
             DW_AT_prototyped  : 1
             DW_AT_type        : <114>
         <2><2be>: Abbrev Number: 13 (DW_TAG_formal_parameter)
             DW_AT_type        : <13e>
         <2><2c3>: Abbrev Number: 13 (DW_TAG_formal_parameter)
             DW_AT_type        : <133>

            ---------------------------------------------

            Is very minimal, like this:
            <4><81d>: Abbrev Number: 44 (DW_TAG_variable)
                DW_AT_abstract_origin: <7ba>
            What that signifies I have no idea.  Ignore.

            ----------------------------------------------

            Is very minimal, like this:
            <200f>: DW_TAG_formal_parameter
                DW_AT_abstract_ori: <1f4c>
                DW_AT_location    : 13440
            What that signifies I have no idea.  Ignore. 
            It might be significant, though: the variable at least
            has a location and so might exist somewhere.
            Maybe we should handle this.

            ---------------------------------------------

            <22407>: DW_TAG_variable
              DW_AT_name        : (indirect string, offset: 0x6579):
                                  vgPlain_trampoline_stuff_start
              DW_AT_decl_file   : 29
              DW_AT_decl_line   : 56
              DW_AT_external    : 1
              DW_AT_declaration : 1

            Nameless and typeless variable that has a location?  Who
            knows.  Not me.
            <2><3d178>: Abbrev Number: 22 (DW_TAG_variable)
                 DW_AT_location    : 9 byte block: 3 c0 c7 13 38 0 0 0 0
                                     (DW_OP_addr: 3813c7c0)

            No, really.  Check it out.  gcc is quite simply borked.
            <3><168cc>: Abbrev Number: 141 (DW_TAG_variable)
            // followed by no attributes, and the next DIE is a sibling,
            // not a child
            */
   }
   return;

  bad_DIE:
   set_position_of_Cursor( c_die,  saved_die_c_offset );
   set_position_of_Cursor( c_abbv, saved_abbv_c_offset );
   VG_(printf)("\nparse_var_DIE: confused by:\n");
   posn = uncook_die( cc, posn, &debug_types_flag, &alt_flag );
   VG_(printf)(" <%d><%lx>: %s", level, posn, ML_(pp_DW_TAG)( dtag ) );
   if (debug_types_flag) {
      VG_(printf)(" (in .debug_types)");
   }
   else if (alt_flag) {
      VG_(printf)(" (in alternate .debug_info)");
   }
   VG_(printf)("\n");
   while (True) {
      DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
      DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
      if (attr == 0 && form == 0) break;
      VG_(printf)("     %18s: ", ML_(pp_DW_AT)(attr));
      
      get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                         cc, c_die, True, form );
      VG_(printf)("\t\n");
   }
   VG_(printf)("\n");
   cc->barf("parse_var_DIE: confused by the above DIE");
   
}



#define N_D3_TYPE_STACK 16

typedef
   struct {
      UChar language;
      
      Int   sp; 
      TyEnt qparentE[N_D3_TYPE_STACK]; 
      Int   qlevel[N_D3_TYPE_STACK];

   }
   D3TypeParser;

static void typestack_show ( D3TypeParser* parser, HChar* str ) {
   Word i;
   VG_(printf)("  typestack (%s) {\n", str);
   for (i = 0; i <= parser->sp; i++) {
      VG_(printf)("    [%ld] (level %d): ", i, parser->qlevel[i]);
      ML_(pp_TyEnt)( &parser->qparentE[i] );
      VG_(printf)("\n");
   }
   VG_(printf)("  }\n");
}

static 
void typestack_preen ( D3TypeParser* parser, Bool td3, Int level )
{
   Bool changed = False;
   vg_assert(parser->sp < N_D3_TYPE_STACK);
   while (True) {
      vg_assert(parser->sp >= -1);
      if (parser->sp == -1) break;
      if (parser->qlevel[parser->sp] <= level) break;
      if (0) 
         TRACE_D3("BBBBAAAA typestack_pop [newsp=%d]\n", parser->sp-1);
      vg_assert(ML_(TyEnt__is_type)(&parser->qparentE[parser->sp]));
      VG_(memset)(&parser->qparentE[parser->sp], 0, sizeof(TyEnt));
      parser->qparentE[parser->sp].cuOff = D3_INVALID_CUOFF;
      parser->qparentE[parser->sp].tag = Te_EMPTY;
      parser->qlevel[parser->sp] = 0;
      parser->sp--;
      changed = True;
   }
   if (changed && td3)
      typestack_show( parser, "after preen" );
}

static Bool typestack_is_empty ( D3TypeParser* parser ) {
   vg_assert(parser->sp >= -1 && parser->sp < N_D3_TYPE_STACK);
   return parser->sp == -1;
}

static void typestack_push ( CUConst* cc,
                             D3TypeParser* parser,
                             Bool td3,
                             TyEnt* parentE, Int level ) {
   if (0)
   TRACE_D3("BBBBAAAA typestack_push[newsp=%d]: %d  %05lx\n",
            parser->sp+1, level, parentE->cuOff);

   typestack_preen(parser, False, level-1);

   vg_assert(parser->sp >= -1);
   vg_assert(parser->sp < N_D3_TYPE_STACK);
   if (parser->sp == N_D3_TYPE_STACK-1)
      cc->barf("typestack_push: N_D3_TYPE_STACK is too low; "
               "increase and recompile");
   if (parser->sp >= 0)
      vg_assert(parser->qlevel[parser->sp] < level);
   parser->sp++;
   vg_assert(parser->qparentE[parser->sp].tag == Te_EMPTY);
   vg_assert(parser->qlevel[parser->sp]  == 0);
   vg_assert(parentE);
   vg_assert(ML_(TyEnt__is_type)(parentE));
   vg_assert(parentE->cuOff != D3_INVALID_CUOFF);
   parser->qparentE[parser->sp] = *parentE;
   parser->qlevel[parser->sp]  = level;
   if (td3)
      typestack_show( parser, "after push" );
}

static Bool subrange_type_denotes_array_bounds ( D3TypeParser* parser,
                                                 DW_TAG dtag ) {
   vg_assert(dtag == DW_TAG_subrange_type);
   if (parser->language != 'A')
      
      return True;
   else
      
      return (! typestack_is_empty(parser)
              && parser->qparentE[parser->sp].tag == Te_TyArray);
}

__attribute__((noinline))
static void parse_type_DIE ( XArray*  tyents,
                             D3TypeParser* parser,
                             DW_TAG dtag,
                             UWord posn,
                             Int level,
                             Cursor* c_die,
                             Cursor* c_abbv,
                             CUConst* cc,
                             Bool td3 )
{
   ULong cts;
   Int   ctsSzB;
   UWord ctsMemSzB;
   TyEnt typeE;
   TyEnt atomE;
   TyEnt fieldE;
   TyEnt boundE;
   Bool  debug_types_flag;
   Bool  alt_flag;

   UWord saved_die_c_offset  = get_position_of_Cursor( c_die );
   UWord saved_abbv_c_offset = get_position_of_Cursor( c_abbv );

   VG_(memset)( &typeE,  0xAA, sizeof(typeE) );
   VG_(memset)( &atomE,  0xAA, sizeof(atomE) );
   VG_(memset)( &fieldE, 0xAA, sizeof(fieldE) );
   VG_(memset)( &boundE, 0xAA, sizeof(boundE) );

   typestack_preen( parser, td3, level-1 );

   if (dtag == DW_TAG_compile_unit
       || dtag == DW_TAG_type_unit
       || dtag == DW_TAG_partial_unit) {
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr != DW_AT_language)
            continue;
         if (ctsSzB == 0)
           goto bad_DIE;
         switch (cts) {
            case DW_LANG_C89: case DW_LANG_C:
            case DW_LANG_C_plus_plus: case DW_LANG_ObjC:
            case DW_LANG_ObjC_plus_plus: case DW_LANG_UPC:
            case DW_LANG_Upc: case DW_LANG_C99:
               parser->language = 'C'; break;
            case DW_LANG_Fortran77: case DW_LANG_Fortran90:
            case DW_LANG_Fortran95:
               parser->language = 'F'; break;
            case DW_LANG_Ada83: case DW_LANG_Ada95: 
               parser->language = 'A'; break;
            case DW_LANG_Cobol74:
            case DW_LANG_Cobol85: case DW_LANG_Pascal83:
            case DW_LANG_Modula2: case DW_LANG_Java:
            case DW_LANG_PLI:
            case DW_LANG_D: case DW_LANG_Python:
            case DW_LANG_Mips_Assembler:
               parser->language = '?'; break;
            default:
               goto bad_DIE;
         }
      }
   }

   if (dtag == DW_TAG_base_type) {
      
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = D3_INVALID_CUOFF;
      typeE.tag   = Te_TyBase;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            typeE.Te.TyBase.name
               = ML_(dinfo_strdup)( "di.readdwarf3.ptD.base_type.1",
                                    (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_byte_size && ctsSzB > 0) {
            typeE.Te.TyBase.szB = cts;
         }
         if (attr == DW_AT_encoding && ctsSzB > 0) {
            switch (cts) {
               case DW_ATE_unsigned: case DW_ATE_unsigned_char:
               case DW_ATE_UTF: 
               case DW_ATE_boolean:
               case DW_ATE_unsigned_fixed:
                  typeE.Te.TyBase.enc = 'U'; break;
               case DW_ATE_signed: case DW_ATE_signed_char:
               case DW_ATE_signed_fixed:
                  typeE.Te.TyBase.enc = 'S'; break;
               case DW_ATE_float:
                  typeE.Te.TyBase.enc = 'F'; break;
               case DW_ATE_complex_float:
                  typeE.Te.TyBase.enc = 'C'; break;
               default:
                  goto bad_DIE;
            }
         }
      }

      if (!typeE.Te.TyBase.name)
         typeE.Te.TyBase.name 
            = ML_(dinfo_strdup)( "di.readdwarf3.ptD.base_type.2",
                                 "<anon_base_type>" );

      
      if (
          typeE.Te.TyBase.name == NULL
          || typeE.Te.TyBase.szB < 0 || typeE.Te.TyBase.szB > 32
          
          || (typeE.Te.TyBase.enc != 'U'
              && typeE.Te.TyBase.enc != 'S' 
              && typeE.Te.TyBase.enc != 'F'
              && typeE.Te.TyBase.enc != 'C'))
         goto bad_DIE;
      if (typeE.Te.TyBase.szB == 0
          && 0 == VG_(strcmp)("void", typeE.Te.TyBase.name)) {
         ML_(TyEnt__make_EMPTY)(&typeE);
         typeE.tag = Te_TyVoid;
         typeE.Te.TyVoid.isFake = False; 
      }

      goto acquire_Type;
   }

   if (dtag == DW_TAG_pointer_type || dtag == DW_TAG_reference_type
       || dtag == DW_TAG_ptr_to_member_type
       || dtag == DW_TAG_rvalue_reference_type) {
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = D3_INVALID_CUOFF;
      switch (dtag) {
      case DW_TAG_pointer_type:
         typeE.tag = Te_TyPtr;
         break;
      case DW_TAG_reference_type:
         typeE.tag = Te_TyRef;
         break;
      case DW_TAG_ptr_to_member_type:
         typeE.tag = Te_TyPtrMbr;
         break;
      case DW_TAG_rvalue_reference_type:
         typeE.tag = Te_TyRvalRef;
         break;
      default:
         vg_assert(False);
      }
      
      typeE.Te.TyPorR.typeR = D3_FAKEVOID_CUOFF;
      typeE.Te.TyPorR.szB = sizeof(UWord);
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_byte_size && ctsSzB > 0) {
            typeE.Te.TyPorR.szB = cts;
         }
         if (attr == DW_AT_type && ctsSzB > 0) {
            typeE.Te.TyPorR.typeR = cook_die_using_form( cc, (UWord)cts, form );
         }
      }
      
      if (typeE.Te.TyPorR.szB != sizeof(UWord))
         goto bad_DIE;
      else
         goto acquire_Type;
   }

   if (dtag == DW_TAG_enumeration_type) {
      
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = posn;
      typeE.tag   = Te_TyEnum;
      typeE.Te.TyEnum.atomRs
         = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.ptD.enum_type.1", 
                       ML_(dinfo_free),
                       sizeof(UWord) );
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            typeE.Te.TyEnum.name
              = ML_(dinfo_strdup)( "di.readdwarf3.pTD.enum_type.2",
                                   (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_byte_size && ctsSzB > 0) {
            typeE.Te.TyEnum.szB = cts;
         }
      }

      if (!typeE.Te.TyEnum.name)
         typeE.Te.TyEnum.name 
            = ML_(dinfo_strdup)( "di.readdwarf3.pTD.enum_type.3",
                                 "<anon_enum_type>" );

      
      if (typeE.Te.TyEnum.szB == 0 
          
          && parser->language != 'A') {
         if (cc->is_type_unit) {
            typeE.Te.TyEnum.szB = sizeof(int);
         } else {
            goto bad_DIE;
         }
      }

      
      typestack_push( cc, parser, td3, &typeE, level );
      goto acquire_Type;
   }

   if (dtag == DW_TAG_enumerator) {
      VG_(memset)( &atomE, 0, sizeof(atomE) );
      atomE.cuOff = posn;
      atomE.tag   = Te_Atom;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            atomE.Te.Atom.name 
              = ML_(dinfo_strdup)( "di.readdwarf3.pTD.enumerator.1",
                                   (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_const_value && ctsSzB > 0) {
            atomE.Te.Atom.value = cts;
            atomE.Te.Atom.valueKnown = True;
         }
      }
      
      if (atomE.Te.Atom.name == NULL)
         goto bad_DIE;
      
      if (typestack_is_empty(parser)) goto bad_DIE;
      vg_assert(ML_(TyEnt__is_type)(&parser->qparentE[parser->sp]));
      vg_assert(parser->qparentE[parser->sp].cuOff != D3_INVALID_CUOFF);
      if (level != parser->qlevel[parser->sp]+1) goto bad_DIE;
      if (parser->qparentE[parser->sp].tag != Te_TyEnum) goto bad_DIE;
      
      vg_assert(parser->qparentE[parser->sp].Te.TyEnum.atomRs);
      VG_(addToXA)( parser->qparentE[parser->sp].Te.TyEnum.atomRs,
                    &atomE );
      
      goto acquire_Atom;
   }

   if (dtag == DW_TAG_structure_type || dtag == DW_TAG_class_type
       || dtag == DW_TAG_union_type) {
      Bool have_szB = False;
      Bool is_decl  = False;
      Bool is_spec  = False;
      
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = posn;
      typeE.tag   = Te_TyStOrUn;
      typeE.Te.TyStOrUn.name = NULL;
      typeE.Te.TyStOrUn.fieldRs
         = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.pTD.struct_type.1", 
                       ML_(dinfo_free),
                       sizeof(UWord) );
      typeE.Te.TyStOrUn.complete = True;
      typeE.Te.TyStOrUn.isStruct = dtag == DW_TAG_structure_type 
                                   || dtag == DW_TAG_class_type;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            typeE.Te.TyStOrUn.name
               = ML_(dinfo_strdup)( "di.readdwarf3.ptD.struct_type.2",
                                    (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_byte_size && ctsSzB >= 0) {
            typeE.Te.TyStOrUn.szB = cts;
            have_szB = True;
         }
         if (attr == DW_AT_declaration && ctsSzB > 0 && cts > 0) {
            is_decl = True;
         }
         if (attr == DW_AT_specification && ctsSzB > 0 && cts > 0) {
            is_spec = True;
         }
      }
      
      if (is_decl && (!is_spec)) {
         if (typeE.Te.TyStOrUn.name == NULL)
            typeE.Te.TyStOrUn.name
               = ML_(dinfo_strdup)( "di.readdwarf3.ptD.struct_type.3",
                                    "<anon_struct_type>" );
         typeE.Te.TyStOrUn.complete = False;
         
         typestack_push( cc, parser, td3, &typeE, level );
         
         goto acquire_Type;
      }
      if ((!is_decl) ) {
         
         if ((!have_szB) 
             )
            goto bad_DIE;
         
         typestack_push( cc, parser, td3, &typeE, level );
         goto acquire_Type;
      }
      else {
         
         goto bad_DIE;
      }
   }

   if (dtag == DW_TAG_member) {
      Bool parent_is_struct;
      VG_(memset)( &fieldE, 0, sizeof(fieldE) );
      fieldE.cuOff = posn;
      fieldE.tag   = Te_Field;
      fieldE.Te.Field.typeR = D3_INVALID_CUOFF;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            fieldE.Te.Field.name
               = ML_(dinfo_strdup)( "di.readdwarf3.ptD.member.1",
                                    (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_type && ctsSzB > 0) {
            fieldE.Te.Field.typeR = cook_die_using_form( cc, (UWord)cts, form );
         }
         if (attr == DW_AT_data_member_location && ctsSzB > 0) {
            fieldE.Te.Field.nLoc = -1;
            fieldE.Te.Field.pos.offset = cts;
         } else if (attr == DW_AT_data_member_location && ctsMemSzB > 0) {
            fieldE.Te.Field.nLoc = (UWord)ctsMemSzB;
            fieldE.Te.Field.pos.loc
               = ML_(dinfo_memdup)( "di.readdwarf3.ptD.member.2",
                                    (UChar*)(UWord)cts, 
                                    (SizeT)fieldE.Te.Field.nLoc );
         }
      }
      
      if (typestack_is_empty(parser)) goto bad_DIE;
      vg_assert(ML_(TyEnt__is_type)(&parser->qparentE[parser->sp]));
      vg_assert(parser->qparentE[parser->sp].cuOff != D3_INVALID_CUOFF);
      if (level != parser->qlevel[parser->sp]+1) goto bad_DIE;
      if (parser->qparentE[parser->sp].tag != Te_TyStOrUn) goto bad_DIE;
      parent_is_struct
         = parser->qparentE[parser->sp].Te.TyStOrUn.isStruct;
      if (!fieldE.Te.Field.name)
         fieldE.Te.Field.name
            = ML_(dinfo_strdup)( "di.readdwarf3.ptD.member.3",
                                 "<anon_field>" );
      vg_assert(fieldE.Te.Field.name);
      if (fieldE.Te.Field.typeR == D3_INVALID_CUOFF)
         goto bad_DIE;
      if (fieldE.Te.Field.nLoc) {
         if (!parent_is_struct) {
            if (fieldE.Te.Field.nLoc > 0)
               ML_(dinfo_free)(fieldE.Te.Field.pos.loc);
            fieldE.Te.Field.pos.loc = NULL;
            fieldE.Te.Field.nLoc = 0;
         }
         
         fieldE.Te.Field.isStruct = parent_is_struct;
         vg_assert(parser->qparentE[parser->sp].Te.TyStOrUn.fieldRs);
         VG_(addToXA)( parser->qparentE[parser->sp].Te.TyStOrUn.fieldRs,
                       &posn );
         
         goto acquire_Field;
      } else {
         ML_(TyEnt__make_EMPTY)(&fieldE);
      }
   }

   if (dtag == DW_TAG_array_type) {
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = posn;
      typeE.tag   = Te_TyArray;
      typeE.Te.TyArray.typeR = D3_INVALID_CUOFF;
      typeE.Te.TyArray.boundRs
         = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.ptD.array_type.1",
                       ML_(dinfo_free),
                       sizeof(UWord) );
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_type && ctsSzB > 0) {
            typeE.Te.TyArray.typeR = cook_die_using_form( cc, (UWord)cts,
                                                          form );
         }
      }
      if (typeE.Te.TyArray.typeR == D3_INVALID_CUOFF)
         goto bad_DIE;
      
      typestack_push( cc, parser, td3, &typeE, level );
      goto acquire_Type;
   }

   
   if (dtag == DW_TAG_subrange_type 
       && subrange_type_denotes_array_bounds(parser, dtag)) {
      Bool have_lower = False;
      Bool have_upper = False;
      Bool have_count = False;
      Long lower = 0;
      Long upper = 0;

      switch (parser->language) {
         case 'C': have_lower = True;  lower = 0; break;
         case 'F': have_lower = True;  lower = 1; break;
         case '?': have_lower = False; break;
         case 'A': have_lower = False; break;
         default:  vg_assert(0); 
      }

      VG_(memset)( &boundE, 0, sizeof(boundE) );
      boundE.cuOff = D3_INVALID_CUOFF;
      boundE.tag   = Te_Bound;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_lower_bound && ctsSzB > 0) {
            lower      = (Long)cts;
            have_lower = True;
         }
         if (attr == DW_AT_upper_bound && ctsSzB > 0) {
            upper      = (Long)cts;
            have_upper = True;
         }
         if (attr == DW_AT_count && ctsSzB > 0) {
            
            have_count = True;
         }
      }
      
      if (typestack_is_empty(parser)) goto bad_DIE;
      vg_assert(ML_(TyEnt__is_type)(&parser->qparentE[parser->sp]));
      vg_assert(parser->qparentE[parser->sp].cuOff != D3_INVALID_CUOFF);
      if (level != parser->qlevel[parser->sp]+1) goto bad_DIE;
      if (parser->qparentE[parser->sp].tag != Te_TyArray) goto bad_DIE;

      
      if (have_lower && have_upper && (!have_count)) {
         boundE.Te.Bound.knownL = True;
         boundE.Te.Bound.knownU = True;
         boundE.Te.Bound.boundL = lower;
         boundE.Te.Bound.boundU = upper;
      }
      else if (have_lower && (!have_upper) && (!have_count)) {
         boundE.Te.Bound.knownL = True;
         boundE.Te.Bound.knownU = False;
         boundE.Te.Bound.boundL = lower;
         boundE.Te.Bound.boundU = 0;
      }
      else if ((!have_lower) && have_upper && (!have_count)) {
         boundE.Te.Bound.knownL = False;
         boundE.Te.Bound.knownU = True;
         boundE.Te.Bound.boundL = 0;
         boundE.Te.Bound.boundU = upper;
      }
      else if ((!have_lower) && (!have_upper) && (!have_count)) {
         boundE.Te.Bound.knownL = False;
         boundE.Te.Bound.knownU = False;
         boundE.Te.Bound.boundL = 0;
         boundE.Te.Bound.boundU = 0;
      } else {
         
         goto bad_DIE;
      }

      
      boundE.cuOff = posn;
      vg_assert(parser->qparentE[parser->sp].Te.TyArray.boundRs);
      VG_(addToXA)( parser->qparentE[parser->sp].Te.TyArray.boundRs,
                    &boundE.cuOff );
      
      goto acquire_Bound;
   }

   
   if (dtag == DW_TAG_typedef 
       || (dtag == DW_TAG_subrange_type 
           && !subrange_type_denotes_array_bounds(parser, dtag))) {
      
      vg_assert (dtag == DW_TAG_typedef || parser->language == 'A');
      
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = D3_INVALID_CUOFF;
      typeE.tag   = Te_TyTyDef;
      typeE.Te.TyTyDef.name = NULL;
      typeE.Te.TyTyDef.typeR = D3_INVALID_CUOFF;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_name && ctsMemSzB > 0) {
            typeE.Te.TyTyDef.name
               = ML_(dinfo_strdup)( "di.readdwarf3.ptD.typedef.1",
                                    (UChar*)(UWord)cts );
         }
         if (attr == DW_AT_type && ctsSzB > 0) {
            typeE.Te.TyTyDef.typeR = cook_die_using_form( cc, (UWord)cts,
                                                          form );
         }
      }
      
      if (
          typeE.Te.TyTyDef.name == NULL
          && parser->language != 'A'
          )
         goto bad_DIE;
      else
         goto acquire_Type;
   }

   if (dtag == DW_TAG_subroutine_type) {
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = D3_INVALID_CUOFF;
      typeE.tag   = Te_TyFn;
      goto acquire_Type;
   }

   if (dtag == DW_TAG_volatile_type || dtag == DW_TAG_const_type) {
      Int have_ty = 0;
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff = D3_INVALID_CUOFF;
      typeE.tag   = Te_TyQual;
      typeE.Te.TyQual.qual
         = dtag == DW_TAG_volatile_type ? 'V' : 'C';
      
      typeE.Te.TyQual.typeR = D3_FAKEVOID_CUOFF;
      while (True) {
         DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
         DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
         if (attr == 0 && form == 0) break;
         get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                            cc, c_die, False, form );
         if (attr == DW_AT_type && ctsSzB > 0) {
            typeE.Te.TyQual.typeR = cook_die_using_form( cc, (UWord)cts, form );
            have_ty++;
         }
      }
      if (have_ty == 1 || have_ty == 0)
         goto acquire_Type;
      else
         goto bad_DIE;
   }

   if (dtag == DW_TAG_unspecified_type) {
      VG_(memset)(&typeE, 0, sizeof(typeE));
      typeE.cuOff           = D3_INVALID_CUOFF;
      typeE.tag             = Te_TyQual;
      typeE.Te.TyQual.typeR = D3_FAKEVOID_CUOFF;
      goto acquire_Type;
   }

   
   return;
   

  acquire_Type:
   if (0) VG_(printf)("YYYY Acquire Type\n");
   vg_assert(ML_(TyEnt__is_type)( &typeE ));
   vg_assert(typeE.cuOff == D3_INVALID_CUOFF || typeE.cuOff == posn);
   typeE.cuOff = posn;
   VG_(addToXA)( tyents, &typeE );
   return;
   

  acquire_Atom:
   if (0) VG_(printf)("YYYY Acquire Atom\n");
   vg_assert(atomE.tag == Te_Atom);
   vg_assert(atomE.cuOff == D3_INVALID_CUOFF || atomE.cuOff == posn);
   atomE.cuOff = posn;
   VG_(addToXA)( tyents, &atomE );
   return;
   

  acquire_Field:
   
   if (0) VG_(printf)("YYYY Acquire Field\n");
   vg_assert(fieldE.tag == Te_Field);
   vg_assert(fieldE.Te.Field.nLoc <= 0 || fieldE.Te.Field.pos.loc != NULL);
   vg_assert(fieldE.Te.Field.nLoc != 0 || fieldE.Te.Field.pos.loc == NULL);
   if (fieldE.Te.Field.isStruct) {
      vg_assert(fieldE.Te.Field.nLoc != 0);
   } else {
      vg_assert(fieldE.Te.Field.nLoc == 0);
   }
   vg_assert(fieldE.cuOff == D3_INVALID_CUOFF || fieldE.cuOff == posn);
   fieldE.cuOff = posn;
   VG_(addToXA)( tyents, &fieldE );
   return;
   

  acquire_Bound:
   if (0) VG_(printf)("YYYY Acquire Bound\n");
   vg_assert(boundE.tag == Te_Bound);
   vg_assert(boundE.cuOff == D3_INVALID_CUOFF || boundE.cuOff == posn);
   boundE.cuOff = posn;
   VG_(addToXA)( tyents, &boundE );
   return;
   

  bad_DIE:
   set_position_of_Cursor( c_die,  saved_die_c_offset );
   set_position_of_Cursor( c_abbv, saved_abbv_c_offset );
   VG_(printf)("\nparse_type_DIE: confused by:\n");
   posn = uncook_die( cc, posn, &debug_types_flag, &alt_flag );
   VG_(printf)(" <%d><%lx>: %s", level, posn, ML_(pp_DW_TAG)( dtag ) );
   if (debug_types_flag) {
      VG_(printf)(" (in .debug_types)");
   } else if (alt_flag) {
      VG_(printf)(" (in alternate .debug_info)");
   }
   VG_(printf)("\n");
   while (True) {
      DW_AT   attr = (DW_AT)  get_ULEB128( c_abbv );
      DW_FORM form = (DW_FORM)get_ULEB128( c_abbv );
      if (attr == 0 && form == 0) break;
      VG_(printf)("     %18s: ", ML_(pp_DW_AT)(attr));
      
      get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                         cc, c_die, True, form );
      VG_(printf)("\t\n");
   }
   VG_(printf)("\n");
   cc->barf("parse_type_DIE: confused by the above DIE");
   
}



static UWord chase_cuOff ( Bool* changed,
                           XArray*  ents,
                           TyEntIndexCache* ents_cache,
                           UWord cuOff )
{
   TyEnt* ent;
   ent = ML_(TyEnts__index_by_cuOff)( ents, ents_cache, cuOff );

   if (!ent) {
      VG_(printf)("chase_cuOff: no entry for 0x%05lx\n", cuOff);
      *changed = False;
      return cuOff;
   }

   vg_assert(ent->tag != Te_EMPTY);
   if (ent->tag != Te_INDIR) {
      *changed = False;
      return cuOff;
   } else {
      vg_assert(ent->Te.INDIR.indR < cuOff);
      *changed = True;
      return ent->Te.INDIR.indR;
   }
}

static
void chase_cuOffs_in_XArray ( Bool* changed,
                              XArray*  ents,
                              TyEntIndexCache* ents_cache,
                              XArray*  cuOffs )
{
   Bool b2 = False;
   Word i, n = VG_(sizeXA)( cuOffs );
   for (i = 0; i < n; i++) {
      Bool   b = False;
      UWord* p = VG_(indexXA)( cuOffs, i );
      *p = chase_cuOff( &b, ents, ents_cache, *p );
      if (b)
         b2 = True;
   }
   *changed = b2;
}

static Bool TyEnt__subst_R_fields ( XArray*  ents,
                                    TyEntIndexCache* ents_cache,
                                    TyEnt* te )
{
   Bool b, changed = False;
   switch (te->tag) {
      case Te_EMPTY:
         break;
      case Te_INDIR:
         te->Te.INDIR.indR
            = chase_cuOff( &b, ents, ents_cache, te->Te.INDIR.indR );
         if (b) changed = True;
         break;
      case Te_UNKNOWN:
         break;
      case Te_Atom:
         break;
      case Te_Field:
         te->Te.Field.typeR
            = chase_cuOff( &b, ents, ents_cache, te->Te.Field.typeR );
         if (b) changed = True;
         break;
      case Te_Bound:
         break;
      case Te_TyBase:
         break;
      case Te_TyPtr:
      case Te_TyRef:
      case Te_TyPtrMbr:
      case Te_TyRvalRef:
         te->Te.TyPorR.typeR
            = chase_cuOff( &b, ents, ents_cache, te->Te.TyPorR.typeR );
         if (b) changed = True;
         break;
      case Te_TyTyDef:
         te->Te.TyTyDef.typeR
            = chase_cuOff( &b, ents, ents_cache, te->Te.TyTyDef.typeR );
         if (b) changed = True;
         break;
      case Te_TyStOrUn:
         chase_cuOffs_in_XArray( &b, ents, ents_cache, te->Te.TyStOrUn.fieldRs );
         if (b) changed = True;
         break;
      case Te_TyEnum:
         chase_cuOffs_in_XArray( &b, ents, ents_cache, te->Te.TyEnum.atomRs );
         if (b) changed = True;
         break;
      case Te_TyArray:
         te->Te.TyArray.typeR
            = chase_cuOff( &b, ents, ents_cache, te->Te.TyArray.typeR );
         if (b) changed = True;
         chase_cuOffs_in_XArray( &b, ents, ents_cache, te->Te.TyArray.boundRs );
         if (b) changed = True;
         break;
      case Te_TyFn:
         break;
      case Te_TyQual:
         te->Te.TyQual.typeR
            = chase_cuOff( &b, ents, ents_cache, te->Te.TyQual.typeR );
         if (b) changed = True;
         break;
      case Te_TyVoid:
         break;
      default:
         ML_(pp_TyEnt)(te);
         vg_assert(0);
   }
   return changed;
}

static
Word dedup_types_substitution_pass ( XArray*  ents,
                                     TyEntIndexCache* ents_cache )
{
   Word i, n, nChanged = 0;
   Bool b;
   n = VG_(sizeXA)( ents );
   for (i = 0; i < n; i++) {
      TyEnt* ent = VG_(indexXA)( ents, i );
      vg_assert(ent->tag != Te_EMPTY);
      b = TyEnt__subst_R_fields( ents, ents_cache, ent );
      if (b)
         nChanged++;
   }

   return nChanged;
}


static
Word dedup_types_commoning_pass ( XArray*  ents )
{
   Word    n, i, nDeleted;
   WordFM* dict; 
   TyEnt*  ent;
   UWord   keyW, valW;

   dict = VG_(newFM)(
             ML_(dinfo_zalloc), "di.readdwarf3.dtcp.1", 
             ML_(dinfo_free),
             (Word(*)(UWord,UWord)) ML_(TyEnt__cmp_by_all_except_cuOff)
          );

   nDeleted = 0;
   n = VG_(sizeXA)( ents );
   for (i = 0; i < n; i++) {
      ent = VG_(indexXA)( ents, i );
      vg_assert(ent->tag != Te_EMPTY);
     
      if (ent->tag == Te_INDIR) {
         vg_assert(ent->Te.INDIR.indR < ent->cuOff);
         continue;
      }

      keyW = valW = 0;
      if (VG_(lookupFM)( dict, &keyW, &valW, (UWord)ent )) {
         
         TyEnt* old = (TyEnt*)keyW;
         vg_assert(valW == 0);
         vg_assert(old != ent);
         vg_assert(old->tag != Te_INDIR);
         vg_assert(old->cuOff < ent->cuOff); 
         ML_(TyEnt__make_EMPTY)( ent );
         ent->tag = Te_INDIR;
         ent->Te.INDIR.indR = old->cuOff;
         nDeleted++;
      } else {
         
         VG_(addToFM)( dict, (UWord)ent, 0 );
      }
   }

   VG_(deleteFM)( dict, NULL, NULL );

   return nDeleted;
}


static
void dedup_types ( Bool td3, 
                   XArray*  ents,
                   TyEntIndexCache* ents_cache )
{
   Word m, n, i, nDel, nSubst, nThresh;
   if (0) td3 = True;

   n = VG_(sizeXA)( ents );

   nThresh = n / 200;

   VG_(setCmpFnXA)(
      ents,
      (Int(*)(void*,void*)) ML_(TyEnt__cmp_by_cuOff_only)
   );
   VG_(sortXA)( ents );

   do {
      nDel   = dedup_types_commoning_pass ( ents );
      nSubst = dedup_types_substitution_pass ( ents, ents_cache );
      vg_assert(nDel >= 0 && nSubst >= 0);
      TRACE_D3("   %ld deletions, %ld substitutions\n", nDel, nSubst);
   } while (nDel > nThresh || nSubst > nThresh);

   m = 0;
   n = VG_(sizeXA)( ents );
   for (i = 0; i < n; i++) {
      TyEnt *ent, *ind;
      ent = VG_(indexXA)( ents, i );
      if (ent->tag != Te_INDIR) continue;
      m++;
      ind = ML_(TyEnts__index_by_cuOff)( ents, ents_cache,
                                         ent->Te.INDIR.indR );
      vg_assert(ind);
      vg_assert(ind->tag != Te_INDIR);
   }

   TRACE_D3("Overall: %ld before, %ld after\n", n, n-m);
}




__attribute__((noinline))
static void resolve_variable_types (
               void (*barf)( HChar* ) __attribute__((noreturn)),
               XArray*  ents,
               TyEntIndexCache* ents_cache,
               XArray*  vars
            )
{
   Word i, n;
   n = VG_(sizeXA)( vars );
   for (i = 0; i < n; i++) {
      TempVar* var = *(TempVar**)VG_(indexXA)( vars, i );
      TyEnt* ent = ML_(TyEnts__index_by_cuOff)( ents, ents_cache,
                                                var->typeR );
      if (ent && ent->tag == Te_INDIR) {
         ent = ML_(TyEnts__index_by_cuOff)( ents, ents_cache, 
                                            ent->Te.INDIR.indR );
         vg_assert(ent);
         vg_assert(ent->tag != Te_INDIR);
      }

      
      if (ent && ML_(TyEnt__is_type)(ent)) {
         var->typeR = ent->cuOff;
         continue;
      }

      if (ent == NULL) {
         VG_(printf)("\n: Invalid cuOff = 0x%05lx\n", var->typeR );
         barf("resolve_variable_types: "
              "cuOff does not refer to a known type");
      }
      vg_assert(ent);
      vg_assert(ent->tag == Te_UNKNOWN);
      var->typeR = ent->cuOff;
   }
}



static Int cmp_TempVar_by_dioff ( void* v1, void* v2 ) {
   TempVar* t1 = *(TempVar**)v1;
   TempVar* t2 = *(TempVar**)v2;
   if (t1->dioff < t2->dioff) return -1;
   if (t1->dioff > t2->dioff) return 1;
   return 0;
}

static void read_DIE ( 
   WordFM*  rangestree,
   XArray*  tyents,
   XArray*  tempvars,
   XArray*  gexprs,
   D3TypeParser* typarser,
   D3VarParser* varparser,
   Cursor* c, Bool td3, CUConst* cc, Int level
)
{
   Cursor abbv;
   ULong  atag, abbv_code;
   UWord  posn;
   UInt   has_children;
   UWord  start_die_c_offset, start_abbv_c_offset;
   UWord  after_die_c_offset, after_abbv_c_offset;

   
   posn      = cook_die( cc, get_position_of_Cursor( c ) );
   abbv_code = get_ULEB128( c );
   set_abbv_Cursor( &abbv, td3, cc, abbv_code );
   atag      = get_ULEB128( &abbv );
   TRACE_D3("\n");
   TRACE_D3(" <%d><%lx>: Abbrev Number: %llu (%s)\n",
            level, posn, abbv_code, ML_(pp_DW_TAG)( atag ) );

   if (atag == 0)
      cc->barf("read_DIE: invalid zero tag on DIE");

   has_children = get_UChar( &abbv );
   if (has_children != DW_children_no && has_children != DW_children_yes)
      cc->barf("read_DIE: invalid has_children value");


   start_die_c_offset  = get_position_of_Cursor( c );
   start_abbv_c_offset = get_position_of_Cursor( &abbv );

   while (True) {
      ULong cts;
      Int   ctsSzB;
      UWord ctsMemSzB;
      ULong at_name = get_ULEB128( &abbv );
      ULong at_form = get_ULEB128( &abbv );
      if (at_name == 0 && at_form == 0) break;
      TRACE_D3("     %18s: ", ML_(pp_DW_AT)(at_name));
      get_Form_contents( &cts, &ctsSzB, &ctsMemSzB,
                         cc, c, td3, (DW_FORM)at_form );
      TRACE_D3("\t");
      TRACE_D3("\n");
   }

   after_die_c_offset  = get_position_of_Cursor( c );
   after_abbv_c_offset = get_position_of_Cursor( &abbv );

   set_position_of_Cursor( c,     start_die_c_offset );
   set_position_of_Cursor( &abbv, start_abbv_c_offset );

   parse_type_DIE( tyents,
                   typarser,
                   (DW_TAG)atag,
                   posn,
                   level,
                   c,     
                   &abbv, 
                   cc,
                   td3 );

   set_position_of_Cursor( c,     start_die_c_offset );
   set_position_of_Cursor( &abbv, start_abbv_c_offset );

   parse_var_DIE( rangestree,
                  tempvars,
                  gexprs,
                  varparser,
                  (DW_TAG)atag,
                  posn,
                  level,
                  c,     
                  &abbv, 
                  cc,
                  td3 );

   set_position_of_Cursor( c,     after_die_c_offset );
   set_position_of_Cursor( &abbv, after_abbv_c_offset );

   
   if (has_children == DW_children_yes) {
      if (0) TRACE_D3("BEGIN children of level %d\n", level);
      while (True) {
         atag = peek_ULEB128( c );
         if (atag == 0) break;
         read_DIE( rangestree, tyents, tempvars, gexprs,
                   typarser, varparser,
                   c, td3, cc, level+1 );
      }
      
      atag = get_ULEB128( c );
      vg_assert(atag == 0);
      if (0) TRACE_D3("END children of level %d\n", level);
   }

}


static
void new_dwarf3_reader_wrk ( 
   struct _DebugInfo* di,
   __attribute__((noreturn)) void (*barf)( HChar* ),
   UChar* debug_info_img,   SizeT debug_info_sz,
   UChar* debug_types_img,  SizeT debug_types_sz,
   UChar* debug_abbv_img,   SizeT debug_abbv_sz,
   UChar* debug_line_img,   SizeT debug_line_sz,
   UChar* debug_str_img,    SizeT debug_str_sz,
   UChar* debug_ranges_img, SizeT debug_ranges_sz,
   UChar* debug_loc_img,    SizeT debug_loc_sz,
   UChar* debug_info_alt_img, SizeT debug_info_alt_sz,
   UChar* debug_abbv_alt_img, SizeT debug_abbv_alt_sz,
   UChar* debug_line_alt_img, SizeT debug_line_alt_sz,
   UChar* debug_str_alt_img,  SizeT debug_str_alt_sz
)
{
   XArray*      tyents;
   XArray*      tyents_to_keep;
   XArray*     gexprs;
   XArray*   tempvars;
   WordFM*  rangestree;
   TyEntIndexCache* tyents_cache = NULL;
   TyEntIndexCache* tyents_to_keep_cache = NULL;
   TempVar *varp, *varp2;
   GExpr* gexpr;
   Cursor abbv; 
   Cursor info; 
   Cursor ranges; 
   D3TypeParser typarser;
   D3VarParser varparser;
   Addr  dr_base;
   UWord dr_offset;
   Word  i, j, n;
   Bool td3 = di->trace_symtab;
   XArray*  dioff_lookup_tab;
   Int pass;
   VgHashTable signature_types;
#if 0
   
   Addr  dl_base;
   UWord dl_offset;
   Cursor loc; 
   TRACE_SYMTAB("\n");
   TRACE_SYMTAB("\n------ The contents of .debug_loc ------\n");
   TRACE_SYMTAB("    Offset   Begin    End      Expression\n");
   init_Cursor( &loc, debug_loc_img, 
                debug_loc_sz, 0, barf, 
                "Overrun whilst reading .debug_loc section(1)" );
   dl_base = 0;
   dl_offset = 0;
   while (True) {
      UWord  w1, w2;
      UWord  len;
      if (is_at_end_Cursor( &loc ))
         break;

      w1 = get_UWord( &loc );
      w2 = get_UWord( &loc );

      if (w1 == 0 && w2 == 0) {
         
         TRACE_D3("    %08lx <End of list>\n", dl_offset);
         dl_base = 0;
         dl_offset = get_position_of_Cursor( &loc );
         continue;
      }

      if (w1 == -1UL) {
         
         TRACE_D3("    %08lx %16lx %08lx (base address)\n",
                  dl_offset, w1, w2);
         dl_base = w2;
         continue;
      }

      
      TRACE_D3("    %08lx %08lx %08lx ",
               dl_offset, w1 + dl_base, w2 + dl_base);
      len = (UWord)get_UShort( &loc );
      while (len > 0) {
         UChar byte = get_UChar( &loc );
         TRACE_D3("%02x", (UInt)byte);
         len--;
      }
      TRACE_SYMTAB("\n");
   }
#endif

   
   TRACE_SYMTAB("\n");
   TRACE_SYMTAB("\n------ The contents of .debug_ranges ------\n");
   TRACE_SYMTAB("    Offset   Begin    End\n");
   init_Cursor( &ranges, debug_ranges_img, 
                debug_ranges_sz, 0, barf, 
                "Overrun whilst reading .debug_ranges section(1)" );
   dr_base = 0;
   dr_offset = 0;
   while (True) {
      UWord  w1, w2;

      if (is_at_end_Cursor( &ranges ))
         break;

      w1 = get_UWord( &ranges );
      w2 = get_UWord( &ranges );

      if (w1 == 0 && w2 == 0) {
         
         TRACE_D3("    %08lx <End of list>\n", dr_offset);
         dr_base = 0;
         dr_offset = get_position_of_Cursor( &ranges );
         continue;
      }

      if (w1 == -1UL) {
         
         TRACE_D3("    %08lx %16lx %08lx (base address)\n",
                  dr_offset, w1, w2);
         dr_base = w2;
         continue;
      }

      
      TRACE_D3("    %08lx %08lx %08lx\n",
               dr_offset, w1 + dr_base, w2 + dr_base);
   }

   
   init_Cursor( &abbv, debug_abbv_img, debug_abbv_sz, 0, barf, 
                "Overrun whilst reading .debug_abbrev section" );
   TRACE_SYMTAB("\n");
   TRACE_SYMTAB("\n------ The contents of .debug_abbrev ------\n");
   while (True) {
      if (is_at_end_Cursor( &abbv ))
         break;
      
      TRACE_D3("  Number TAG\n");
      while (True) {
         ULong atag;
         UInt  has_children;
         ULong acode = get_ULEB128( &abbv );
         if (acode == 0) break; 
         atag = get_ULEB128( &abbv );
         has_children = get_UChar( &abbv );
         TRACE_D3("   %llu      %s    [%s]\n", 
                  acode, ML_(pp_DW_TAG)(atag),
                         ML_(pp_DW_children)(has_children));
         while (True) {
            ULong at_name = get_ULEB128( &abbv );
            ULong at_form = get_ULEB128( &abbv );
            if (at_name == 0 && at_form == 0) break;
            TRACE_D3("    %18s %s\n", 
                     ML_(pp_DW_AT)(at_name), ML_(pp_DW_FORM)(at_form));
         }
      }
   }
   TRACE_SYMTAB("\n");

   tyents = VG_(newXA)( ML_(dinfo_zalloc), 
                        "di.readdwarf3.ndrw.1 (TyEnt temp array)",
                        ML_(dinfo_free), sizeof(TyEnt) );
   { TyEnt tyent;
     VG_(memset)(&tyent, 0, sizeof(tyent));
     tyent.tag   = Te_TyVoid;
     tyent.cuOff = D3_FAKEVOID_CUOFF;
     tyent.Te.TyVoid.isFake = True;
     VG_(addToXA)( tyents, &tyent );
   }
   { TyEnt tyent;
     VG_(memset)(&tyent, 0, sizeof(tyent));
     tyent.tag   = Te_UNKNOWN;
     tyent.cuOff = D3_INVALID_CUOFF;
     VG_(addToXA)( tyents, &tyent );
   }

   rangestree = VG_(newFM)( ML_(dinfo_zalloc),
                            "di.readdwarf3.ndrw.2 (rangestree)",
                            ML_(dinfo_free),
                            (Word(*)(UWord,UWord))cmp__XArrays_of_AddrRange );

   tempvars = VG_(newXA)( ML_(dinfo_zalloc),
                          "di.readdwarf3.ndrw.3 (TempVar*s array)",
                          ML_(dinfo_free), 
                          sizeof(TempVar*) );

   gexprs = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.ndrw.4",
                        ML_(dinfo_free), sizeof(GExpr*) );

   VG_(memset)( &typarser, 0, sizeof(typarser) );
   typarser.sp = -1;
   typarser.language = '?';
   for (i = 0; i < N_D3_TYPE_STACK; i++) {
      typarser.qparentE[i].tag   = Te_EMPTY;
      typarser.qparentE[i].cuOff = D3_INVALID_CUOFF;
   }

   VG_(memset)( &varparser, 0, sizeof(varparser) );
   varparser.sp = -1;

   signature_types = VG_(HT_construct) ("signature_types");
   
   if (debug_types_img != NULL) {
      init_Cursor( &info, debug_types_img, debug_types_sz, 0, barf,
                   "Overrun whilst reading .debug_types section" );
      TRACE_D3("\n------ Collecting signatures from .debug_types section ------\n");

      while (True) {
         UWord   cu_start_offset, cu_offset_now;
         CUConst cc;

         cu_start_offset = get_position_of_Cursor( &info );
         TRACE_D3("\n");
         TRACE_D3("  Compilation Unit @ offset 0x%lx:\n", cu_start_offset);
         parse_CU_Header( &cc, td3, &info,
                          (UChar*)debug_abbv_img, debug_abbv_sz,
                          True, False );

         
         cc.types_cuOff_bias = debug_info_sz;

         record_signatured_type( signature_types, cc.type_signature,
                                 cook_die( &cc, cc.type_offset ));

         cu_offset_now = (cu_start_offset + cc.unit_length
                          + (cc.is_dw64 ? 12 : 4));

         if (cu_offset_now == debug_types_sz)
            break;

         set_position_of_Cursor ( &info, cu_offset_now );
      }
   }

   for (pass = 0; pass < 3; ++pass) {
      UWord section_size;

      if (pass == 0) {
         if (debug_info_alt_img == NULL)
	    continue;
         init_Cursor( &info, debug_info_alt_img, debug_info_alt_sz, 0, barf,
                      "Overrun whilst reading alternate .debug_info section" );
         section_size = debug_info_alt_sz;

         TRACE_D3("\n------ Parsing alternate .debug_info section ------\n");
      } else if (pass == 1) {
         init_Cursor( &info, debug_info_img, debug_info_sz, 0, barf,
                      "Overrun whilst reading .debug_info section" );
         section_size = debug_info_sz;

         TRACE_D3("\n------ Parsing .debug_info section ------\n");
      } else {
         if (debug_types_img == NULL)
            continue;
         init_Cursor( &info, debug_types_img, debug_types_sz, 0, barf,
                      "Overrun whilst reading .debug_types section" );
         section_size = debug_types_sz;

         TRACE_D3("\n------ Parsing .debug_types section ------\n");
      }

      while (True) {
         UWord   cu_start_offset, cu_offset_now;
         CUConst cc;
         UWord cu_size_including_IniLen, cu_amount_used;

         Word avail = get_remaining_length_Cursor( &info );
         if (avail < 11) {
            if (avail > 0)
               TRACE_D3("new_dwarf3_reader_wrk: warning: "
                        "%ld unused bytes after end of DIEs\n", avail);
            break;
         }

         
         vg_assert(varparser.sp == -1);
         for (i = 0; i < N_D3_VAR_STACK; i++) {
            vg_assert(varparser.ranges[i] == NULL);
            vg_assert(varparser.level[i] == 0);
         }
         for (i = 0; i < N_D3_TYPE_STACK; i++) {
            vg_assert(typarser.qparentE[i].cuOff == D3_INVALID_CUOFF);
            vg_assert(typarser.qparentE[i].tag   == Te_EMPTY);
            vg_assert(typarser.qlevel[i] == 0);
         }

         cu_start_offset = get_position_of_Cursor( &info );
         TRACE_D3("\n");
         TRACE_D3("  Compilation Unit @ offset 0x%lx:\n", cu_start_offset);
         if (pass == 0)
            parse_CU_Header( &cc, td3, &info,
                             (UChar*)debug_abbv_alt_img, debug_abbv_alt_sz,
                             False, True );
         else
            parse_CU_Header( &cc, td3, &info,
                             (UChar*)debug_abbv_img, debug_abbv_sz,
                             pass == 2, False );
         cc.debug_str_img    = pass == 0 ? debug_str_alt_img : debug_str_img;
         cc.debug_str_sz     = pass == 0 ? debug_str_alt_sz : debug_str_sz;
         cc.debug_ranges_img = debug_ranges_img;
         cc.debug_ranges_sz  = debug_ranges_sz;
         cc.debug_loc_img    = debug_loc_img;
         cc.debug_loc_sz     = debug_loc_sz;
         cc.debug_line_img   = pass == 0 ? debug_line_alt_img : debug_line_img;
         cc.debug_line_sz    = pass == 0 ? debug_line_alt_sz : debug_line_sz;
         cc.debug_info_img   = pass == 0 ? debug_info_alt_img : debug_info_img;
         cc.debug_info_sz    = pass == 0 ? debug_info_alt_sz : debug_info_sz;
         cc.debug_types_img  = debug_types_img;
         cc.debug_types_sz   = debug_types_sz;
         cc.debug_info_alt_img = debug_info_alt_img;
         cc.debug_info_alt_sz = debug_info_alt_sz;
         cc.debug_str_alt_img = debug_str_alt_img;
         cc.debug_str_alt_sz = debug_str_alt_sz;
         cc.types_cuOff_bias = debug_info_sz;
         cc.alt_cuOff_bias   = debug_info_sz + debug_types_sz;
         cc.cu_start_offset  = cu_start_offset;
         cc.di = di;
         cc.cu_svma_known = False;
         cc.cu_svma       = 0;

         cc.signature_types = signature_types;

         varstack_push( &cc, &varparser, td3, 
                        unitary_range_list(0UL, ~0UL),
                        -1, False, NULL );

         vg_assert(!varparser.filenameTable );
         varparser.filenameTable 
            = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.ndrw.5",
                          ML_(dinfo_free),
                          sizeof(UChar*) );
         vg_assert(varparser.filenameTable);

         
         vg_assert(varparser.sp == 0);
         read_DIE( rangestree,
                   tyents, tempvars, gexprs,
                   &typarser, &varparser,
                   &info, td3, &cc, 0 );

         cu_offset_now = get_position_of_Cursor( &info );

         if (0) VG_(printf)("Travelled: %lu  size %llu\n",
                            cu_offset_now - cc.cu_start_offset,
                            cc.unit_length + (cc.is_dw64 ? 12 : 4));

         
         cu_size_including_IniLen = cc.unit_length + (cc.is_dw64 ? 12 : 4);
         
         cu_amount_used = cu_offset_now - cc.cu_start_offset;

         if (1) TRACE_D3("offset now %ld, d-i-size %ld\n",
                         cu_offset_now, section_size);
         if (cu_offset_now > section_size)
            barf("toplevel DIEs beyond end of CU");

         if (cu_amount_used > cu_size_including_IniLen)
            barf("CU's actual size appears to be larger than it claims it is");

         while (cu_amount_used < cu_size_including_IniLen
                && get_remaining_length_Cursor( &info ) > 0) {
            if (0) VG_(printf)("SKIP\n");
            (void)get_UChar( &info );
            cu_offset_now = get_position_of_Cursor( &info );
            cu_amount_used = cu_offset_now - cc.cu_start_offset;
         }

         TRACE_D3("\n");
         varstack_preen( &varparser, td3, -2 );
         
         typestack_preen( &typarser, td3, -2 );

         TRACE_D3("set_abbv_Cursor cache: %lu queries, %lu misses\n",
                  cc.saC_cache_queries, cc.saC_cache_misses);

         vg_assert(varparser.filenameTable );
         VG_(deleteXA)( varparser.filenameTable );
         varparser.filenameTable = NULL;

         if (cu_offset_now == section_size)
            break;
         
      }
   }

   if (td3) {
      TRACE_D3("\n");
      ML_(pp_TyEnts)(tyents, "Initial type entity (TyEnt) array");
      TRACE_D3("\n");
      TRACE_D3("------ Compressing type entries ------\n");
   }

   tyents_cache = ML_(dinfo_zalloc)( "di.readdwarf3.ndrw.6",
                                     sizeof(TyEntIndexCache) );
   ML_(TyEntIndexCache__invalidate)( tyents_cache );
   dedup_types( td3, tyents, tyents_cache );
   if (td3) {
      TRACE_D3("\n");
      ML_(pp_TyEnts)(tyents, "After type entity (TyEnt) compression");
   }

   TRACE_D3("\n");
   TRACE_D3("------ Resolving the types of variables ------\n" );
   resolve_variable_types( barf, tyents, tyents_cache, tempvars );

   tyents_to_keep
      = VG_(newXA)( ML_(dinfo_zalloc), 
                    "di.readdwarf3.ndrw.7 (TyEnt to-keep array)",
                    ML_(dinfo_free), sizeof(TyEnt) );
   n = VG_(sizeXA)( tyents );
   for (i = 0; i < n; i++) {
      TyEnt* ent = VG_(indexXA)( tyents, i );
      if (ent->tag != Te_INDIR)
         VG_(addToXA)( tyents_to_keep, ent );
   }

   VG_(deleteXA)( tyents );
   tyents = NULL;
   ML_(dinfo_free)( tyents_cache );
   tyents_cache = NULL;

   VG_(setCmpFnXA)(
      tyents_to_keep,
      (Int(*)(void*,void*)) ML_(TyEnt__cmp_by_cuOff_only)
   );
   VG_(sortXA)( tyents_to_keep );

   
   tyents_to_keep_cache
      = ML_(dinfo_zalloc)( "di.readdwarf3.ndrw.8",
                           sizeof(TyEntIndexCache) );
   ML_(TyEntIndexCache__invalidate)( tyents_to_keep_cache );

   vg_assert(!di->admin_tyents);
   di->admin_tyents = tyents_to_keep;

   
   TRACE_D3("\n");
   TRACE_D3("------ Biasing the location expressions ------\n" );

   n = VG_(sizeXA)( gexprs );
   for (i = 0; i < n; i++) {
      gexpr = *(GExpr**)VG_(indexXA)( gexprs, i );
      bias_GX( gexpr, di );
   }

   TRACE_D3("\n");
   TRACE_D3("------ Acquired the following variables: ------\n\n");

   dioff_lookup_tab
      = VG_(newXA)( ML_(dinfo_zalloc), "di.readdwarf3.ndrw.9",
                    ML_(dinfo_free), 
                    sizeof(TempVar*) );
   vg_assert(dioff_lookup_tab);

   n = VG_(sizeXA)( tempvars );
   Word first_primary_var;
   for (first_primary_var = 0;
        debug_info_alt_sz && first_primary_var < n;
        first_primary_var++) {
      varp = *(TempVar**)VG_(indexXA)( tempvars, first_primary_var );
      if (varp->dioff < debug_info_sz + debug_types_sz)
         break;
   }
   for (i = 0; i < n; i++) {
      varp = *(TempVar**)VG_(indexXA)( tempvars, (i + first_primary_var) % n );
      if (i > first_primary_var) {
         varp2 = *(TempVar**)VG_(indexXA)( tempvars,
                                           (i + first_primary_var - 1) % n );
         vg_assert(varp2->dioff < varp->dioff);
      }
      VG_(addToXA)( dioff_lookup_tab, &varp );
   }
   VG_(setCmpFnXA)( dioff_lookup_tab, cmp_TempVar_by_dioff );
   VG_(sortXA)( dioff_lookup_tab ); 

   n = VG_(sizeXA)( tempvars );
   for (j = 0; j < n; j++) {
      TyEnt* ent;
      varp = *(TempVar**)VG_(indexXA)( tempvars, j );

      
      if (td3) {
         VG_(printf)("<%lx> addVar: level %d: %s :: ",
                     varp->dioff,
                     varp->level,
                     varp->name ? varp->name : (UChar*)"<anon_var>" );
         if (varp->typeR) {
            ML_(pp_TyEnt_C_ishly)( tyents_to_keep, varp->typeR );
         } else {
            VG_(printf)("NULL");
         }
         VG_(printf)("\n  Loc=");
         if (varp->gexpr) {
            ML_(pp_GX)(varp->gexpr);
         } else {
            VG_(printf)("NULL");
         }
         VG_(printf)("\n");
         if (varp->fbGX) {
            VG_(printf)("  FrB=");
            ML_(pp_GX)( varp->fbGX );
            VG_(printf)("\n");
         } else {
            VG_(printf)("  FrB=none\n");
         }
         VG_(printf)("  declared at: %s:%d\n",
                     varp->fName ? varp->fName : (UChar*)"NULL",
                     varp->fLine );
         if (varp->absOri != (UWord)D3_INVALID_CUOFF)
            VG_(printf)("  abstract origin: <%lx>\n", varp->absOri);
      }

      if (!varp->gexpr) {
         TRACE_D3("  SKIP (no location)\n\n");
         continue;
      }

      if (varp->absOri != (UWord)D3_INVALID_CUOFF) {
         Bool found;
         Word ixFirst, ixLast;
         TempVar key;
         TempVar* keyp = &key;
         TempVar *varAI;
         VG_(memset)(&key, 0, sizeof(key)); 
         key.dioff = varp->absOri; 
         found = VG_(lookupXA)( dioff_lookup_tab, &keyp,
                                &ixFirst, &ixLast );
         if (!found) {
            
            TRACE_D3("  SKIP (DW_AT_abstract_origin can't be resolved)\n\n");
            continue;
         }
         vg_assert(ixFirst == ixLast);
         varAI = *(TempVar**)VG_(indexXA)( dioff_lookup_tab, ixFirst );
         
         vg_assert(varAI);
         vg_assert(varAI->dioff == varp->absOri);

         
         if (varAI->typeR && !varp->typeR)
            varp->typeR = varAI->typeR;
         if (varAI->name && !varp->name)
            varp->name = varAI->name;
         if (varAI->fName && !varp->fName)
            varp->fName = varAI->fName;
         if (varAI->fLine > 0 && varp->fLine == 0)
            varp->fLine = varAI->fLine;
      }

      
      if (!varp->name)
         varp->name = ML_(addStr)( di, "<anon_var>", -1 );

      
      ent = ML_(TyEnts__index_by_cuOff)( tyents_to_keep,
                                         tyents_to_keep_cache, 
                                         varp->typeR );
      vg_assert(ent);
      vg_assert(ML_(TyEnt__is_type)(ent) || ent->tag == Te_UNKNOWN);

      if (ent->tag == Te_UNKNOWN) continue;

      vg_assert(varp->gexpr);
      vg_assert(varp->name);
      vg_assert(varp->typeR);
      vg_assert(varp->level >= 0);

      TRACE_D3("  ACQUIRE for range(s) ");
      { AddrRange  oneRange;
        AddrRange* varPcRanges;
        Word       nVarPcRanges;
        if (varp->nRanges == 0 || varp->nRanges == 1) {
           vg_assert(!varp->rngMany);
           if (varp->nRanges == 0) {
              vg_assert(varp->rngOneMin == 0);
              vg_assert(varp->rngOneMax == 0);
           }
           nVarPcRanges = varp->nRanges;
           oneRange.aMin = varp->rngOneMin;
           oneRange.aMax = varp->rngOneMax;
           varPcRanges = &oneRange;
        } else {
           vg_assert(varp->rngMany);
           vg_assert(varp->rngOneMin == 0);
           vg_assert(varp->rngOneMax == 0);
           nVarPcRanges = VG_(sizeXA)(varp->rngMany);
           vg_assert(nVarPcRanges >= 2);
           vg_assert(nVarPcRanges == (Word)varp->nRanges);
           varPcRanges = VG_(indexXA)(varp->rngMany, 0);
        }
        if (varp->level == 0)
           vg_assert( nVarPcRanges == 1 );
        
        for (i = 0; i < nVarPcRanges; i++) {
           Addr pcMin = varPcRanges[i].aMin;
           Addr pcMax = varPcRanges[i].aMax;
           vg_assert(pcMin <= pcMax);
           if (varp->level == 0) {
              vg_assert(pcMin == (Addr)0);
              vg_assert(pcMax == ~(Addr)0);
           } else {
              vg_assert(pcMax < ~(Addr)0);
           }

           
           if (varp->level > 0) {
              pcMin += di->text_debug_bias;
              pcMax += di->text_debug_bias;
           } 

           if (i > 0 && (i%2) == 0) 
              TRACE_D3("\n                       ");
           TRACE_D3("[%#lx,%#lx] ", pcMin, pcMax );

           ML_(addVar)(
              di, varp->level, 
                  pcMin, pcMax,
                  varp->name,  varp->typeR,
                  varp->gexpr, varp->fbGX,
                  varp->fName, varp->fLine, td3 
           );
        }
      }

      TRACE_D3("\n\n");
      
   }

   
   n = VG_(sizeXA)( tempvars );
   for (i = 0; i < n; i++) {
      varp = *(TempVar**)VG_(indexXA)( tempvars, i );
      ML_(dinfo_free)(varp);
   }
   VG_(deleteXA)( tempvars );
   tempvars = NULL;

   
   VG_(deleteXA)( dioff_lookup_tab );

   VG_(deleteFM)( rangestree, (void(*)(UWord))VG_(deleteXA), NULL );

   
   ML_(dinfo_free)( tyents_to_keep_cache );
   tyents_to_keep_cache = NULL;

   vg_assert( varparser.filenameTable == NULL );

   
   VG_(HT_destruct) ( signature_types, ML_(dinfo_free) );

   
   vg_assert(!di->admin_gexprs);
   di->admin_gexprs = gexprs;
}



static Bool               d3rd_jmpbuf_valid  = False;
static HChar*             d3rd_jmpbuf_reason = NULL;
static VG_MINIMAL_JMP_BUF(d3rd_jmpbuf);

static __attribute__((noreturn)) void barf ( HChar* reason ) {
   vg_assert(d3rd_jmpbuf_valid);
   d3rd_jmpbuf_reason = reason;
   VG_MINIMAL_LONGJMP(d3rd_jmpbuf);
   
   vg_assert(0);
}


void 
ML_(new_dwarf3_reader) (
   struct _DebugInfo* di,
   UChar* debug_info_img,   SizeT debug_info_sz,
   UChar* debug_types_img,  SizeT debug_types_sz,
   UChar* debug_abbv_img,   SizeT debug_abbv_sz,
   UChar* debug_line_img,   SizeT debug_line_sz,
   UChar* debug_str_img,    SizeT debug_str_sz,
   UChar* debug_ranges_img, SizeT debug_ranges_sz,
   UChar* debug_loc_img,    SizeT debug_loc_sz,
   UChar* debug_info_alt_img, SizeT debug_info_alt_sz,
   UChar* debug_abbv_alt_img, SizeT debug_abbv_alt_sz,
   UChar* debug_line_alt_img, SizeT debug_line_alt_sz,
   UChar* debug_str_alt_img,  SizeT debug_str_alt_sz
)
{
   volatile Int  jumped;
   volatile Bool td3 = di->trace_symtab;

   vg_assert(d3rd_jmpbuf_valid  == False);
   vg_assert(d3rd_jmpbuf_reason == NULL);

   d3rd_jmpbuf_valid = True;
   jumped = VG_MINIMAL_SETJMP(d3rd_jmpbuf);
   if (jumped == 0) {
      
      new_dwarf3_reader_wrk( di, barf,
                             debug_info_img,   debug_info_sz,
                             debug_types_img,  debug_types_sz,
                             debug_abbv_img,   debug_abbv_sz,
                             debug_line_img,   debug_line_sz,
                             debug_str_img,    debug_str_sz,
                             debug_ranges_img, debug_ranges_sz,
                             debug_loc_img,    debug_loc_sz,
                             debug_info_alt_img, debug_info_alt_sz,
                             debug_abbv_alt_img, debug_abbv_alt_sz,
                             debug_line_alt_img, debug_line_alt_sz,
                             debug_str_alt_img,  debug_str_alt_sz);
      d3rd_jmpbuf_valid = False;
      TRACE_D3("\n------ .debug_info reading was successful ------\n");
   } else {
      
      d3rd_jmpbuf_valid = False;
      
      vg_assert(d3rd_jmpbuf_reason != NULL);

      TRACE_D3("\n------ .debug_info reading failed ------\n");

      ML_(symerr)(di, True, d3rd_jmpbuf_reason);
   }

   d3rd_jmpbuf_valid  = False;
   d3rd_jmpbuf_reason = NULL;
}




#if 0
   
   TRACE_SYMTAB("\n");
   TRACE_SYMTAB("\n------ The contents of .debug_arange ------\n");
   init_Cursor( &aranges, debug_aranges_img, 
                debug_aranges_sz, 0, barf, 
                "Overrun whilst reading .debug_aranges section" );
   while (True) {
      ULong  len, d_i_offset;
      Bool   is64;
      UShort version;
      UChar  asize, segsize;

      if (is_at_end_Cursor( &aranges ))
         break;
      
      
      len = get_Initial_Length( &is64, &aranges, 
               "in .debug_aranges: invalid initial-length field" );
      version    = get_UShort( &aranges );
      d_i_offset = get_Dwarfish_UWord( &aranges, is64 );
      asize      = get_UChar( &aranges );
      segsize    = get_UChar( &aranges );
      TRACE_D3("  Length:                   %llu\n", len);
      TRACE_D3("  Version:                  %d\n", (Int)version);
      TRACE_D3("  Offset into .debug_info:  %llx\n", d_i_offset);
      TRACE_D3("  Pointer Size:             %d\n", (Int)asize);
      TRACE_D3("  Segment Size:             %d\n", (Int)segsize);
      TRACE_D3("\n");
      TRACE_D3("    Address            Length\n");

      while ((get_position_of_Cursor( &aranges ) % (2 * asize)) > 0) {
         (void)get_UChar( & aranges );
      }
      while (True) {
         ULong address = get_Dwarfish_UWord( &aranges, asize==8 );
         ULong length = get_Dwarfish_UWord( &aranges, asize==8 );
         TRACE_D3("    0x%016llx 0x%llx\n", address, length);
         if (address == 0 && length == 0) break;
      }
   }
   TRACE_SYMTAB("\n");
#endif

#endif 

