

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


#if (defined(VGO_linux) && \
    !(defined(VGPV_arm_linux_android) || defined(VGPV_x86_linux_android)) \
    || defined(VGO_darwin))

#include "pub_core_basics.h"
#include "pub_core_debuginfo.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_xarray.h"
#include "priv_misc.h"             
#include "priv_tytypes.h"
#include "priv_d3basics.h"
#include "priv_storage.h"
#include "priv_readstabs.h"        

#if defined(VGO_linux)
#  include <a.out.h> 
#elif defined(VGO_darwin)
#  include <mach-o/nlist.h>
#  define n_other n_sect
#  if VG_WORDSIZE == 8
#     define nlist nlist_64
#  endif
#else
#  error "Unknown OS"
#endif


typedef enum { N_UNDEF = 0,	
	       N_GSYM  = 32,    
               N_FUN   = 36,    
               N_STSYM = 38,    
               N_LCSYM = 40,    
               N_RSYM  = 64,    
               N_SLINE = 68,    
               N_SO    = 100,   
               N_LSYM  = 128,   
	       N_BINCL = 130,	
               N_SOL   = 132,   
	       N_PSYM  = 160,   
	       N_EINCL = 162,	
               N_LBRAC = 192,   
	       N_EXCL  = 194,	
               N_RBRAC = 224    
             } stab_types;
      

void ML_(read_debuginfo_stabs) ( DebugInfo* di,
                                 UChar* stabC,   Int stab_sz, 
                                 UChar* stabstr, Int stabstr_sz )
{
   const Bool debug     = False;
   const Bool contdebug = False;
   Int    i;
   Int    n_stab_entries;
   struct nlist* stab = (struct nlist*)stabC;
   UChar *next_stabstr = NULL;
   
   struct {
      Addr     start;         
      Addr     end;           
      Int      line;          
   } func = { 0, 0, -1 };
   struct {
      Char     *name;
      Bool     same;
   } file = { NULL, True };
   struct {
      Int      prev;          
      Int      no;            
      Int      ovf;           
      Addr     addr;          
      Bool     first;         
   } line = { 0, 0, 0, 0, False };

   file.name     = ML_(addStr)(di,"???", -1);

   n_stab_entries = stab_sz/(int)sizeof(struct nlist);

   for (i = 0; i < n_stab_entries; i++) {
      const struct nlist *st = &stab[i];
      Char *string;

      if (di->trace_symtab) {
         VG_(printf) ( "%2d  type=%d   othr=%d   desc=%d   "
                       "value=0x%x   strx=%d  %s\n", i,
                       st->n_type, st->n_other, st->n_desc, 
                       (Int)st->n_value,
                       (Int)st->n_un.n_strx, 
                       stabstr + st->n_un.n_strx );
      }

      
      {
         Int   qbuflen = 0;
         Int   qidx = 0;
         Char* qbuf = NULL;
         Int   qlen;
         Bool  qcontinuing = False;
         UInt  qstringidx;

         qstringidx = st->n_un.n_strx;
         string = stabstr + qstringidx;
         qlen = VG_(strlen)(string);

         while (string 
                && qlen > 0 
                && (qcontinuing || string[qlen-1] == '\\')) {

            qcontinuing = string[qlen-1] == '\\';

            
            while (string[qlen-1] == '\\' && qlen > 0)
               qlen--;

            if (contdebug)
               VG_(printf)("found extension string: \"%s\" "
                           "len=%d(%c) idx=%d buflen=%d\n", 
                           string, qlen, string[qlen-1], qidx, qbuflen);

            if ((qidx + qlen) >= qbuflen) {
               Char *n;
               
               if (qbuflen == 0)
                  qbuflen = 16;
               while ((qidx + qlen) >= qbuflen)
                  qbuflen *= 2;
               n = ML_(dinfo_zalloc)("di.readstabs.rds.1", qbuflen);
               VG_(memcpy)(n, qbuf, qidx);
               
               if (qbuf != NULL)
                  ML_(dinfo_free)(qbuf);
               qbuf = n;
            }

            VG_(memcpy)(&qbuf[qidx], string, qlen);
            qidx += qlen;
            if (contdebug) {
               qbuf[qidx] = '\0';
               VG_(printf)("working buf=\"%s\"\n", qbuf);
            }

            i++;
            if (i >= n_stab_entries)
               break;

            if (stab[i].n_un.n_strx) {
               string = stabstr + stab[i].n_un.n_strx;
               qlen = VG_(strlen)(string);
            } else {
               string = NULL;
               qlen = 0;
            }
         }

         if (qbuf != NULL) {
            i--;                        
            string = ML_(addStr)(di, qbuf, qidx);
            ML_(dinfo_free)(qbuf);
            if (contdebug)
               VG_(printf)("made composite: \"%s\"\n", string);
         }
      }

      switch(st->n_type) {
         case N_UNDEF:
            
            if (next_stabstr != NULL) {
               stabstr_sz -= next_stabstr - stabstr;
               stabstr = next_stabstr;
               if (stabstr_sz <= 0) {
                  VG_(printf)(" @@ bad stabstr size %d\n", stabstr_sz);
                  return;
               }
            }
            next_stabstr = stabstr + st->n_value;
            break;

         case N_BINCL: {
            break;
         }

         case N_EINCL:
            break;

         case N_EXCL:
            break;

         case N_SOL:                
            if (line.ovf != 0) 
               VG_(message)(Vg_UserMsg, 
                            "Warning: file %s is very big (> 65535 lines) "
                            "Line numbers and annotation for this file might "
                            "be wrong.  Sorry.\n",
                            file.name);
            

         case N_SO: {                
            UChar *nm = string;
            UInt len = VG_(strlen)(nm);
            Addr addr = func.start + st->n_value;

            if (line.addr != 0) {
               
               ML_(addLineInfo)(di, file.name, NULL, line.addr,
                                addr, line.no + line.ovf * LINENO_OVERFLOW, i);
            }

            
            line.ovf = 0;            
            line.addr = 0;
            line.prev = 0;
            line.no = 0;

            if (len > 0 && nm[len-1] != '/') {
               file.name = ML_(addStr)(di, nm, -1);
               if (debug)
                  VG_(printf)("new source: %s\n", file.name);
            } else if (len == 0)
               file.name = ML_(addStr)(di, "?1\0", -1);

            break;
         }

         case N_SLINE: {        
            Addr addr = func.start + st->n_value;

            if (line.addr != 0) {
               
               ML_(addLineInfo)(di, file.name, NULL, line.addr,
                                addr, line.no + line.ovf * LINENO_OVERFLOW, i);
            }

            line.addr = addr;
            line.prev = line.no;
            line.no = (Int)((UShort)st->n_desc);

            if (line.prev > line.no + OVERFLOW_DIFFERENCE && file.same) {
               VG_(message)(Vg_DebugMsg, 
                  "Line number overflow detected (%d --> %d) in %s\n", 
                  line.prev, line.no, file.name);
               line.ovf++;
            }
            file.same = True;

            if (line.first) {
               line.first = False;
               
               
               if (func.start != 0) {
                  func.line = line.no;
               }
            }
            break;
         }

         case N_FUN: {                
            Addr addr = 0;        

            if (*string == '\0' || func.start != 0) {
               
               line.first = False;

               
               addr = func.start + st->n_value;

               
               func.start = 0;

               
            }

            if (*string != '\0') {
               
               line.first = True;

               
               addr = di->text_debug_bias + st->n_value;

               func.start = addr;
            }

            if (line.addr) {
               ML_(addLineInfo)(di, file.name, NULL, line.addr,
                                addr, line.no + line.ovf * LINENO_OVERFLOW, i);
               line.addr = 0;
            }

            
            
            break;
         }

         case N_LBRAC: {
            
            
            break;
         }

         case N_RBRAC: {
            
            
            break;
         }

         case N_GSYM:                
         case N_STSYM:                
         case N_LCSYM:                
         case N_PSYM:                
         case N_LSYM:                
         case N_RSYM:                  
            break;
      }
   }
}

#endif 

