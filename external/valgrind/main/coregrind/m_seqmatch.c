

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2008-2012 OpenWorks Ltd
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
*/

#include "pub_core_basics.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcbase.h"    
#include "pub_core_seqmatch.h"    


Bool VG_(generic_match) ( 
        Bool matchAll,
        void* patt,  SizeT szbPatt,  UWord nPatt,  UWord ixPatt,
        void* input, SizeT szbInput, UWord nInput, UWord ixInput,
        Bool (*pIsStar)(void*),
        Bool (*pIsQuery)(void*),
        Bool (*pattEQinp)(void*,void*,void*,UWord),
        void* inputCompleter
     )
{
   /* This is the spec, written in my favourite formal specification
      language.  It specifies non-greedy matching of '*'s.

      ma ('*':ps) (i:is) = ma ps (i:is) || ma ('*':ps) is
      ma ('*':ps) []     = ma ps []

      ma ('?':ps) (i:is) = ma ps is
      ma ('?':ps) []     = False

      ma (p:ps)   (i:is) = p == i && ma ps is

      ma (p:ps)   []     = False
      ma []       (i:is) = False -- m-all, True for m-prefix
      ma []       []     = True
   */
   Bool  havePatt, haveInput;
   void  *currPatt, *currInput;
  tailcall:
   vg_assert(nPatt >= 0   && nPatt  < 1000000); 
   vg_assert(nInput >= 0  && nInput < 1000000); 
   vg_assert(ixPatt >= 0  && ixPatt <= nPatt);
   vg_assert(ixInput >= 0 && ixInput <= nInput);

   havePatt  = ixPatt < nPatt;
   haveInput = ixInput < nInput;

   currPatt  = havePatt  ? ((Char*)patt) + szbPatt * ixPatt    : NULL;
   currInput = haveInput ? ((Char*)input) + szbInput * ixInput : NULL;

   
   
   
   
   
   
   
   
   
   
   
   
   
   
   if (havePatt && pIsStar(currPatt)) {
      if (haveInput) {
         
         
         
         if (VG_(generic_match)( matchAll,
                                 patt, szbPatt, nPatt,  ixPatt+1,
                                 input,szbInput,nInput, ixInput+0,
                                 pIsStar,pIsQuery,pattEQinp,
                                 inputCompleter) ) {
            return True;
         }
         
         ixInput++; goto tailcall;
      } else {
         
         ixPatt++; goto tailcall;
      }
   }

   
   
   
   
   if (havePatt && pIsQuery(currPatt)) {
      if (haveInput) {
         ixPatt++; ixInput++; goto tailcall;
      } else {
         return False;
      }
   }

   
   
   
   if (havePatt && haveInput) {
      if (!pattEQinp(currPatt,currInput,inputCompleter,ixInput)) return False;
      ixPatt++; ixInput++; goto tailcall;
   }

   
   
   if (havePatt && !haveInput) return False;

   
   
   
   
   
   
   
   if (!havePatt && haveInput) {
      return matchAll ? False 
                      : True; 
   }

   
   
   if (!havePatt && !haveInput) return True;

   
   vg_assert(0);
}


static Bool charIsStar  ( void* pV ) { return *(Char*)pV == '*'; }
static Bool charIsQuery ( void* pV ) { return *(Char*)pV == '?'; }
static Bool char_p_EQ_i ( void* pV, void* cV,
                          void* null_completer, UWord ixcV ) {
   Char p = *(Char*)pV;
   Char c = *(Char*)cV;
   vg_assert(p != '*' && p != '?');
   return p == c;
}
Bool VG_(string_match) ( const Char* patt, const Char* input )
{
   return VG_(generic_match)(
             True,
             (void*)patt,  sizeof(UChar), VG_(strlen)(patt), 0,
             (void*)input, sizeof(UChar), VG_(strlen)(input), 0,
             charIsStar, charIsQuery, char_p_EQ_i,
             NULL
          );
}



