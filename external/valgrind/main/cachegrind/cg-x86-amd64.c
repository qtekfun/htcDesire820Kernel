

/*
   This file is part of Cachegrind, a Valgrind tool for cache
   profiling programs.

   Copyright (C) 2002-2012 Nicholas Nethercote
      njn@valgrind.org

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

#if defined(VGA_x86) || defined(VGA_amd64)

#include "pub_tool_basics.h"
#include "pub_tool_cpuid.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"

#include "cg_arch.h"


static void micro_ops_warn(Int actual_size, Int used_size, Int line_size)
{
   VG_(dmsg)("warning: Pentium 4 with %d KB micro-op instruction trace cache\n", 
             actual_size);
   VG_(dmsg)("         Simulating a %d KB I-cache with %d B lines\n", 
             used_size, line_size);
}

static
Int Intel_cache_info(Int level, cache_t* I1c, cache_t* D1c, cache_t* LLc)
{
   Int cpuid1_eax;
   Int cpuid1_ignore;
   Int family;
   Int model;
   UChar info[16];
   Int   i, j, trials;
   Bool  L2_found = False;
   Bool  L3_found = False;
   cache_t L3c = { 0, 0, 0 };

   if (level < 2) {
      VG_(dmsg)("warning: CPUID level < 2 for Intel processor (%d)\n", level);
      return -1;
   }

   
   VG_(cpuid)(1, 0, &cpuid1_eax, &cpuid1_ignore,
	      &cpuid1_ignore, &cpuid1_ignore);
   family = (((cpuid1_eax >> 20) & 0xff) << 4) + ((cpuid1_eax >> 8) & 0xf);
   model =  (((cpuid1_eax >> 16) & 0xf) << 4) + ((cpuid1_eax >> 4) & 0xf);

   VG_(cpuid)(2, 0, (Int*)&info[0], (Int*)&info[4], 
                    (Int*)&info[8], (Int*)&info[12]);
   trials  = info[0] - 1;   
   info[0] = 0x0;           

   if (0 != trials) {
      VG_(dmsg)("warning: non-zero CPUID trials for Intel processor (%d)\n",
                trials);
      return -1;
   }

   for (i = 0; i < 16; i++) {

      switch (info[i]) {

      case 0x0:       
          break;
          
      
      case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:
      case 0x0b:
      case 0x4f: case 0x50: case 0x51: case 0x52: case 0x55:
      case 0x56: case 0x57: case 0x59:
      case 0x5a: case 0x5b: case 0x5c: case 0x5d:
      case 0x76:
      case 0xb0: case 0xb1: case 0xb2:
      case 0xb3: case 0xb4: case 0xba: case 0xc0:
      case 0xca:
          break;      

      case 0x06: *I1c = (cache_t) {  8, 4, 32 }; break;
      case 0x08: *I1c = (cache_t) { 16, 4, 32 }; break;
      case 0x09: *I1c = (cache_t) { 32, 4, 64 }; break;
      case 0x30: *I1c = (cache_t) { 32, 8, 64 }; break;

      case 0x0a: *D1c = (cache_t) {  8, 2, 32 }; break;
      case 0x0c: *D1c = (cache_t) { 16, 4, 32 }; break;
      case 0x0d: *D1c = (cache_t) { 16, 4, 64 }; break;
      case 0x0e: *D1c = (cache_t) { 24, 6, 64 }; break;
      case 0x2c: *D1c = (cache_t) { 32, 8, 64 }; break;

      
      case 0x10: case 0x15: case 0x1a: 
      case 0x88: case 0x89: case 0x8a: case 0x8d:
      case 0x90: case 0x96: case 0x9b:
         VG_(tool_panic)("IA-64 cache detected?!");

      
      case 0x22: L3c = (cache_t) { 512,    4, 64 }; L3_found = True; break;
      case 0x23: L3c = (cache_t) { 1024,   8, 64 }; L3_found = True; break;
      case 0x25: L3c = (cache_t) { 2048,   8, 64 }; L3_found = True; break;
      case 0x29: L3c = (cache_t) { 4096,   8, 64 }; L3_found = True; break;
      case 0x46: L3c = (cache_t) { 4096,   4, 64 }; L3_found = True; break;
      case 0x47: L3c = (cache_t) { 8192,   8, 64 }; L3_found = True; break;
      case 0x4a: L3c = (cache_t) { 6144,  12, 64 }; L3_found = True; break;
      case 0x4b: L3c = (cache_t) { 8192,  16, 64 }; L3_found = True; break;
      case 0x4c: L3c = (cache_t) { 12288, 12, 64 }; L3_found = True; break;
      case 0x4d: L3c = (cache_t) { 16384, 16, 64 }; L3_found = True; break;
      case 0xd0: L3c = (cache_t) { 512,    4, 64 }; L3_found = True; break;
      case 0xd1: L3c = (cache_t) { 1024,   4, 64 }; L3_found = True; break;
      case 0xd2: L3c = (cache_t) { 2048,   4, 64 }; L3_found = True; break;
      case 0xd6: L3c = (cache_t) { 1024,   8, 64 }; L3_found = True; break;
      case 0xd7: L3c = (cache_t) { 2048,   8, 64 }; L3_found = True; break;
      case 0xd8: L3c = (cache_t) { 4096,   8, 64 }; L3_found = True; break;
      case 0xdc: L3c = (cache_t) { 1536,  12, 64 }; L3_found = True; break;
      case 0xdd: L3c = (cache_t) { 3072,  12, 64 }; L3_found = True; break;
      case 0xde: L3c = (cache_t) { 6144,  12, 64 }; L3_found = True; break;
      case 0xe2: L3c = (cache_t) { 2048,  16, 64 }; L3_found = True; break;
      case 0xe3: L3c = (cache_t) { 4096,  16, 64 }; L3_found = True; break;
      case 0xe4: L3c = (cache_t) { 8192,  16, 64 }; L3_found = True; break;
      case 0xea: L3c = (cache_t) { 12288, 24, 64 }; L3_found = True; break;
      case 0xeb: L3c = (cache_t) { 18432, 24, 64 }; L3_found = True; break;
      case 0xec: L3c = (cache_t) { 24576, 24, 64 }; L3_found = True; break;

      
      case 0x21: *LLc = (cache_t) {  256, 8, 64 }; L2_found = True; break;

      
      case 0x39: *LLc = (cache_t) {  128, 4, 64 }; L2_found = True; break;
      case 0x3c: *LLc = (cache_t) {  256, 4, 64 }; L2_found = True; break;

      case 0x40:
          break;

      case 0x41: *LLc = (cache_t) {  128,  4, 32 }; L2_found = True; break;
      case 0x42: *LLc = (cache_t) {  256,  4, 32 }; L2_found = True; break;
      case 0x43: *LLc = (cache_t) {  512,  4, 32 }; L2_found = True; break;
      case 0x44: *LLc = (cache_t) { 1024,  4, 32 }; L2_found = True; break;
      case 0x45: *LLc = (cache_t) { 2048,  4, 32 }; L2_found = True; break;
      case 0x48: *LLc = (cache_t) { 3072, 12, 64 }; L2_found = True; break;
      case 0x4e: *LLc = (cache_t) { 6144, 24, 64 }; L2_found = True; break;
      case 0x49:
         if (family == 15 && model == 6) {
            
            L3c = (cache_t) { 4096, 16, 64 }; L3_found = True;
         } else {
	    *LLc = (cache_t) { 4096, 16, 64 }; L2_found = True;
         }
         break;

      
      case 0x60: *D1c = (cache_t) { 16, 8, 64 };  break;      
      case 0x66: *D1c = (cache_t) {  8, 4, 64 };  break;      
      case 0x67: *D1c = (cache_t) { 16, 4, 64 };  break;      
      case 0x68: *D1c = (cache_t) { 32, 4, 64 };  break;      

      case 0x70:    
         *I1c = (cache_t) { 16, 8, 32 };  
         micro_ops_warn(12, 16, 32);
         break;  
      case 0x71:    
         *I1c = (cache_t) { 16, 8, 32 };  
         micro_ops_warn(16, 16, 32); 
         break;  
      case 0x72:    
         *I1c = (cache_t) { 32, 8, 32 };  
         micro_ops_warn(32, 32, 32); 
         break;  

      
      case 0x78: *LLc = (cache_t) { 1024, 4,  64 }; L2_found = True;  break;

      
      case 0x79: *LLc = (cache_t) {  128, 8,  64 }; L2_found = True;  break;
      case 0x7a: *LLc = (cache_t) {  256, 8,  64 }; L2_found = True;  break;
      case 0x7b: *LLc = (cache_t) {  512, 8,  64 }; L2_found = True;  break;
      case 0x7c: *LLc = (cache_t) { 1024, 8,  64 }; L2_found = True;  break;
      case 0x7d: *LLc = (cache_t) { 2048, 8,  64 }; L2_found = True;  break;
      case 0x7e: *LLc = (cache_t) {  256, 8, 128 }; L2_found = True;  break;
      case 0x7f: *LLc = (cache_t) {  512, 2,  64 }; L2_found = True;  break;
      case 0x80: *LLc = (cache_t) {  512, 8,  64 }; L2_found = True;  break;
      case 0x81: *LLc = (cache_t) {  128, 8,  32 }; L2_found = True;  break;
      case 0x82: *LLc = (cache_t) {  256, 8,  32 }; L2_found = True;  break;
      case 0x83: *LLc = (cache_t) {  512, 8,  32 }; L2_found = True;  break;
      case 0x84: *LLc = (cache_t) { 1024, 8,  32 }; L2_found = True;  break;
      case 0x85: *LLc = (cache_t) { 2048, 8,  32 }; L2_found = True;  break;
      case 0x86: *LLc = (cache_t) {  512, 4,  64 }; L2_found = True;  break;
      case 0x87: *LLc = (cache_t) { 1024, 8,  64 }; L2_found = True;  break;

      
      case 0xf0: case 0xf1:
         break;

      case 0xff:
         j = 0;
         VG_(cpuid)(4, j++, (Int*)&info[0], (Int*)&info[4], 
                            (Int*)&info[8], (Int*)&info[12]);

         while ((info[0] & 0x1f) != 0) {
            UInt assoc = ((*(UInt *)&info[4] >> 22) & 0x3ff) + 1;
            UInt parts = ((*(UInt *)&info[4] >> 12) & 0x3ff) + 1;
            UInt line_size = (*(UInt *)&info[4] & 0x7ff) + 1;
            UInt sets = *(UInt *)&info[8] + 1;
            cache_t c;

            c.size = assoc * parts * line_size * sets / 1024;
            c.assoc = assoc;
            c.line_size = line_size;

            switch ((info[0] & 0xe0) >> 5)
            {
            case 1:
               switch (info[0] & 0x1f)
               {
               case 1: *D1c = c; break;
               case 2: *I1c = c; break;
               case 3: VG_(dmsg)("warning: L1 unified cache ignored\n"); break;
               default: VG_(dmsg)("warning: L1 cache of unknown type ignored\n"); break;
               }
               break;
            case 2:
               switch (info[0] & 0x1f)
               {
               case 1: VG_(dmsg)("warning: L2 data cache ignored\n"); break;
               case 2: VG_(dmsg)("warning: L2 instruction cache ignored\n"); break;
               case 3: *LLc = c; L2_found = True; break;
               default: VG_(dmsg)("warning: L2 cache of unknown type ignored\n"); break;
               }
               break;
            case 3:
               switch (info[0] & 0x1f)
               {
               case 1: VG_(dmsg)("warning: L3 data cache ignored\n"); break;
               case 2: VG_(dmsg)("warning: L3 instruction cache ignored\n"); break;
               case 3: L3c = c; L3_found = True; break;
               default: VG_(dmsg)("warning: L3 cache of unknown type ignored\n"); break;
               }
               break;
            default:
               VG_(dmsg)("warning: L%u cache ignored\n", (info[0] & 0xe0) >> 5);
               break;
            }

            VG_(cpuid)(4, j++, (Int*)&info[0], (Int*)&info[4], 
                               (Int*)&info[8], (Int*)&info[12]);
         }
         break;

      default:
         VG_(dmsg)("warning: Unknown Intel cache config value (0x%x), ignoring\n",
                   info[i]);
         break;
      }
   }

   
   if (L3_found) {
      VG_(dmsg)("warning: L3 cache found, using its data for the LL simulation.\n");
      *LLc = L3c;
      L2_found = True;
   }

   if (!L2_found)
      VG_(dmsg)("warning: L2 cache not installed, ignore LL results.\n");

   return 0;
}


static Int decode_AMD_cache_L2_L3_assoc ( Int bits_15_12 )
{
   switch (bits_15_12 & 0xF) {
      case 1: return 1;    case 2: return 2;
      case 4: return 4;    case 6: return 8;
      case 8: return 16;   case 0xA: return 32;
      case 0xB: return 48; case 0xC: return 64;
      case 0xD: return 96; case 0xE: return 128;
      case 0xF: 
      case 0: 
      default:
        return 1;
   }
}

static
Int AMD_cache_info(cache_t* I1c, cache_t* D1c, cache_t* LLc)
{
   UInt ext_level;
   UInt dummy, model;
   UInt I1i, D1i, L2i, L3i;
   
   VG_(cpuid)(0x80000000, 0, &ext_level, &dummy, &dummy, &dummy);

   if (0 == (ext_level & 0x80000000) || ext_level < 0x80000006) {
      VG_(dmsg)("warning: ext_level < 0x80000006 for AMD processor (0x%x)\n", 
                ext_level);
      return -1;
   }

   VG_(cpuid)(0x80000005, 0, &dummy, &dummy, &D1i, &I1i);
   VG_(cpuid)(0x80000006, 0, &dummy, &dummy, &L2i, &L3i);

   VG_(cpuid)(0x1, 0, &model, &dummy, &dummy, &dummy);

   
   if (model == 0x630) {
      VG_(dmsg)("warning: Buggy Duron stepping A0. Assuming L2 size=65536 bytes\n");
      L2i = (64 << 16) | (L2i & 0xffff);
   }

   D1c->size      = (D1i >> 24) & 0xff;
   D1c->assoc     = (D1i >> 16) & 0xff;
   D1c->line_size = (D1i >>  0) & 0xff;

   I1c->size      = (I1i >> 24) & 0xff;
   I1c->assoc     = (I1i >> 16) & 0xff;
   I1c->line_size = (I1i >>  0) & 0xff;

   LLc->size      = (L2i >> 16) & 0xffff; 
   LLc->assoc     = decode_AMD_cache_L2_L3_assoc((L2i >> 12) & 0xf);
   LLc->line_size = (L2i >>  0) & 0xff;

   if (((L3i >> 18) & 0x3fff) > 0) {
      
      LLc->size      = ((L3i >> 18) & 0x3fff) * 512;
      LLc->assoc     = decode_AMD_cache_L2_L3_assoc((L3i >> 12) & 0xf);
      LLc->line_size = (L3i >>  0) & 0xff;
      VG_(dmsg)("warning: L3 cache found, using its data for the L2 simulation.\n");
   }

   return 0;
}

static 
Int get_caches_from_CPUID(cache_t* I1c, cache_t* D1c, cache_t* LLc)
{
   Int  level, ret;
   Char vendor_id[13];

   if (!VG_(has_cpuid)()) {
      VG_(dmsg)("CPUID instruction not supported\n");
      return -1;
   }

   VG_(cpuid)(0, 0, &level, (int*)&vendor_id[0], 
	      (int*)&vendor_id[8], (int*)&vendor_id[4]);    
   vendor_id[12] = '\0';

   if (0 == level) {
      VG_(dmsg)("CPUID level is 0, early Pentium?\n");
      return -1;
   }

   
   if (0 == VG_(strcmp)(vendor_id, "GenuineIntel")) {
      ret = Intel_cache_info(level, I1c, D1c, LLc);

   } else if (0 == VG_(strcmp)(vendor_id, "AuthenticAMD")) {
      ret = AMD_cache_info(I1c, D1c, LLc);

   } else if (0 == VG_(strcmp)(vendor_id, "CentaurHauls")) {
      
      D1c->size      = 64;
      D1c->assoc     = 16;
      D1c->line_size = 16;
      I1c->size      = 64;
      I1c->assoc     = 4;
      I1c->line_size = 16;
      LLc->size      = 64;
      LLc->assoc     = 16;
      LLc->line_size = 16;
      ret = 0;

   } else {
      VG_(dmsg)("CPU vendor ID not recognised (%s)\n", vendor_id);
      return -1;
   }

   
   I1c->size *= 1024;
   D1c->size *= 1024;
   LLc->size *= 1024;

   if (LLc->size > 0 && LLc->assoc > 0 && LLc->line_size > 0) {
      Long nSets = (Long)LLc->size / (Long)(LLc->line_size * LLc->assoc);
      if (
          nSets >= 4
          
          && VG_(log2_64)( (ULong)nSets ) == -1
          
          && VG_(log2_64)( (ULong)((2 * nSets) / (Long)3) ) != -1
          
          && (LLc->assoc % 2) == 0
         ) {
         Int new_assoc = LLc->assoc + (LLc->assoc / 2);
         VG_(dmsg)("warning: pretending that LL cache has associativity"
                   " %d instead of actual %d\n", new_assoc, LLc->assoc);
         LLc->assoc = new_assoc;
      }
   }

   return ret;
}


void VG_(configure_caches)(cache_t* I1c, cache_t* D1c, cache_t* LLc,
                           Bool all_caches_clo_defined)
{
   Int res;
   
   
   *I1c = (cache_t) {  65536, 2, 64 };
   *D1c = (cache_t) {  65536, 2, 64 };
   *LLc = (cache_t) { 262144, 8, 64 };

   
   res = get_caches_from_CPUID(I1c, D1c, LLc);

   
   if (res != 0 && !all_caches_clo_defined) {
      VG_(dmsg)("Warning: Couldn't auto-detect cache config, using one "
                "or more defaults \n");
   }
}

#endif 

