

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2007-2012 OpenWorks LLP
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
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_xarray.h"    



struct _XArray {
   void* (*alloc) ( HChar*, SizeT ); 
   HChar* cc;                       
   void  (*free) ( void* );         
   Int   (*cmpFn) ( void*, void* ); 
   Word  elemSzB;   
   void* arr;       
   Word  usedsizeE; 
   Word  totsizeE;  
   Bool  sorted;    
};


XArray* VG_(newXA) ( void*(*alloc_fn)(HChar*,SizeT), 
                     HChar* cc,
                     void(*free_fn)(void*),
                     Word elemSzB )
{
   struct _XArray* xa;
   vg_assert( sizeof(Word) == sizeof(void*) );
   vg_assert( ((Word)(-1)) < ((Word)(0)) );
   vg_assert( ((SizeT)(-1)) > ((SizeT)(0)) );
   
   vg_assert(alloc_fn);
   vg_assert(free_fn);
   vg_assert(elemSzB > 0);
   xa = alloc_fn( cc, sizeof(struct _XArray) );
   vg_assert(xa);
   xa->alloc     = alloc_fn;
   xa->cc        = cc;
   xa->free      = free_fn;
   xa->cmpFn     = NULL;
   xa->elemSzB   = elemSzB;
   xa->usedsizeE = 0;
   xa->totsizeE  = 0;
   xa->sorted    = False;
   xa->arr       = NULL;
   return xa;
}

XArray* VG_(cloneXA)( HChar* cc, XArray* xao )
{
   struct _XArray* xa = (struct _XArray*)xao;
   struct _XArray* nyu;
   HChar* nyu_cc;
   vg_assert(xa);
   vg_assert(xa->alloc);
   vg_assert(xa->free);
   vg_assert(xa->elemSzB >= 1);
   nyu_cc = cc ? cc : xa->cc;
   nyu = xa->alloc( nyu_cc, sizeof(struct _XArray) );
   if (!nyu)
      return NULL;
   
   *nyu = *xa;
   nyu->cc = nyu_cc;
   
   if (nyu->arr) {
      nyu->totsizeE = nyu->usedsizeE;
      
      nyu->arr = nyu->alloc( nyu->cc, nyu->totsizeE * nyu->elemSzB );
      if (!nyu->arr) {
         nyu->free(nyu);
         return NULL;
      }
      VG_(memcpy)( nyu->arr, xa->arr, nyu->totsizeE * nyu->elemSzB );
   }
   
   return nyu;
}

void VG_(deleteXA) ( XArray* xao )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(xa->free);
   if (xa->arr)
      xa->free(xa->arr);
   xa->free(xa);
}

void VG_(setCmpFnXA) ( XArray* xao, Int (*compar)(void*,void*) )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(compar);
   xa->cmpFn  = compar;
   xa->sorted = False;
}

inline void* VG_(indexXA) ( XArray* xao, Word n )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(n >= 0);
   vg_assert(n < xa->usedsizeE);
   return ((char*)xa->arr) + n * xa->elemSzB;
}

static inline void ensureSpaceXA ( struct _XArray* xa )
{
   if (xa->usedsizeE == xa->totsizeE) {
      void* tmp;
      Word  newsz;
      if (xa->totsizeE == 0)
         vg_assert(!xa->arr);
      if (xa->totsizeE > 0)
         vg_assert(xa->arr);
      if (xa->totsizeE == 0) {
         if (xa->elemSzB == 1) newsz = 8;
         else if (xa->elemSzB == 2) newsz = 4;
         else newsz = 2;
      } else {
         newsz = 2 + (3 * xa->totsizeE) / 2;  
      }
      if (0 && xa->totsizeE >= 10000) 
         VG_(printf)("addToXA: increasing from %ld to %ld\n", 
                     xa->totsizeE, newsz);
      tmp = xa->alloc(xa->cc, newsz * xa->elemSzB);
      vg_assert(tmp);
      if (xa->usedsizeE > 0) 
         VG_(memcpy)(tmp, xa->arr, xa->usedsizeE * xa->elemSzB);
      if (xa->arr)
         xa->free(xa->arr);
      xa->arr = tmp;
      xa->totsizeE = newsz;
   }
}

Word VG_(addToXA) ( XArray* xao, void* elem )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(elem);
   vg_assert(xa->totsizeE >= 0);
   vg_assert(xa->usedsizeE >= 0 && xa->usedsizeE <= xa->totsizeE);
   ensureSpaceXA( xa );
   vg_assert(xa->usedsizeE < xa->totsizeE);
   vg_assert(xa->arr);
   VG_(memcpy)( ((UChar*)xa->arr) + xa->usedsizeE * xa->elemSzB,
                elem, xa->elemSzB );
   xa->usedsizeE++;
   xa->sorted = False;
   return xa->usedsizeE-1;
}

Word VG_(addBytesToXA) ( XArray* xao, void* bytesV, Word nbytes )
{
   Word r, i;
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(xa->elemSzB == 1);
   vg_assert(nbytes >= 0);
   vg_assert(xa->totsizeE >= 0);
   vg_assert(xa->usedsizeE >= 0 && xa->usedsizeE <= xa->totsizeE);
   r = xa->usedsizeE;
   for (i = 0; i < nbytes; i++) {
      ensureSpaceXA( xa );
      vg_assert(xa->usedsizeE < xa->totsizeE);
      vg_assert(xa->arr);
      * (((UChar*)xa->arr) + xa->usedsizeE) = ((UChar*)bytesV)[i];
      xa->usedsizeE++;
   }
   xa->sorted = False;
   return r;
}

void VG_(sortXA) ( XArray* xao )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(xa->cmpFn);
   VG_(ssort)( xa->arr, xa->usedsizeE, xa->elemSzB, xa->cmpFn );
   xa->sorted = True;
}

Bool VG_(lookupXA_UNSAFE) ( XArray* xao, void* key,
                            Word* first, Word* last,
                            Int(*cmpFn)(void*,void*) )
{
   Word  lo, mid, hi, cres;
   void* midv;
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   lo = 0;
   hi = xa->usedsizeE-1;
   while (True) {
      
      if (lo > hi) return False; 
      mid  = (lo + hi) / 2;
      midv = VG_(indexXA)( xa, mid );
      cres = cmpFn( key, midv );
      if (cres < 0)  { hi = mid-1; continue; }
      if (cres > 0)  { lo = mid+1; continue; }
      
      vg_assert(cmpFn( key, VG_(indexXA)(xa, lo) ) >= 0);
      vg_assert(cmpFn( key, VG_(indexXA)(xa, hi) ) <= 0);
      if (first) {
         *first = mid;
         while (*first > 0 
                && 0 == cmpFn( key, VG_(indexXA)(xa, (*first)-1))) {
            (*first)--;
         }
      }
      if (last) {
         *last = mid;
         while (*last < xa->usedsizeE-1
                && 0 == cmpFn( key, VG_(indexXA)(xa, (*last)+1))) {
            (*last)++;
         }
      }
      return True;
   }
}

Bool VG_(lookupXA) ( XArray* xao, void* key,
                     Word* first, Word* last )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(xa->cmpFn);
   vg_assert(xa->sorted);
   return VG_(lookupXA_UNSAFE)(xao, key, first, last, xa->cmpFn);
}

Word VG_(sizeXA) ( XArray* xao )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   return xa->usedsizeE;
}

void VG_(dropTailXA) ( XArray* xao, Word n )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(n >= 0);
   vg_assert(n <= xa->usedsizeE);
   xa->usedsizeE -= n;
}

void VG_(dropHeadXA) ( XArray* xao, Word n )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(n >= 0);
   vg_assert(n <= xa->usedsizeE);
   if (n == 0) {
      return;
   }
   if (n == xa->usedsizeE) {
      xa->usedsizeE = 0;
      return;
   }
   vg_assert(n > 0);
   vg_assert(xa->usedsizeE - n > 0);
   VG_(memcpy)( (char*)xa->arr,
                ((char*)xa->arr) + n * xa->elemSzB, 
                (xa->usedsizeE - n) * xa->elemSzB );
   xa->usedsizeE -= n;
}

void VG_(removeIndexXA)( XArray* xao, Word n )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   vg_assert(n >= 0);
   vg_assert(n < xa->usedsizeE);
   if (n+1 < xa->usedsizeE) {
      VG_(memmove)( ((char*)xa->arr) + (n+0) * xa->elemSzB,
                    ((char*)xa->arr) + (n+1) * xa->elemSzB,
                    (xa->usedsizeE - n - 1) * xa->elemSzB );
   }
   xa->usedsizeE--;
}

void VG_(getContentsXA_UNSAFE)( XArray* xao,
                                void** ctsP,
                                Word* usedP )
{
   struct _XArray* xa = (struct _XArray*)xao;
   vg_assert(xa);
   *ctsP  = (void*)xa->arr;
   *usedP = xa->usedsizeE;
}


static void add_char_to_XA ( HChar c, void* opaque )
{
   XArray* dst = (XArray*)opaque;
   (void) VG_(addBytesToXA)( dst, &c, 1 );
}

void VG_(xaprintf)( XArray* dst, const HChar* format, ... )
{
   va_list vargs;
   va_start(vargs, format);
   VG_(vcbprintf)( add_char_to_XA, (void*)dst, format, vargs );
   va_end(vargs);
}


