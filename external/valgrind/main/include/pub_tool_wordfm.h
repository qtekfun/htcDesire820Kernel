

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2007-2012 Julian Seward
      jseward@acm.org

   This code is based on previous work by Nicholas Nethercote
   (coregrind/m_oset.c) which is

   Copyright (C) 2005-2012 Nicholas Nethercote
       njn@valgrind.org

   which in turn was derived partially from:

      AVL C library
      Copyright (C) 2000,2002  Daniel Nagy

      This program is free software; you can redistribute it and/or
      modify it under the terms of the GNU General Public License as
      published by the Free Software Foundation; either version 2 of
      the License, or (at your option) any later version.
      [...]

      (taken from libavl-0.4/debian/copyright)

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

#ifndef __PUB_TOOL_WORDFM_H
#define __PUB_TOOL_WORDFM_H



typedef  struct _WordFM  WordFM; 

WordFM* VG_(newFM) ( void* (*alloc_nofail)( HChar* cc, SizeT ),
                     HChar* cc,
                     void  (*dealloc)(void*),
                     Word  (*kCmp)(UWord,UWord) );

void VG_(deleteFM) ( WordFM*, void(*kFin)(UWord), void(*vFin)(UWord) );

Bool VG_(addToFM) ( WordFM* fm, UWord k, UWord v );

Bool VG_(delFromFM) ( WordFM* fm,
                      UWord* oldK, UWord* oldV, UWord key );

Bool VG_(lookupFM) ( WordFM* fm, 
                     UWord* keyP, UWord* valP, UWord key );

Bool VG_(findBoundsFM)( WordFM* fm,
                        UWord* kMinP, UWord* vMinP,
                        UWord* kMaxP, UWord* vMaxP,
                        UWord minKey, UWord minVal,
                        UWord maxKey, UWord maxVal,
                        UWord key );

UWord VG_(sizeFM) ( WordFM* fm );


void VG_(initIterFM) ( WordFM* fm );

void VG_(initIterAtFM) ( WordFM* fm, UWord start_at );

Bool VG_(nextIterFM) ( WordFM* fm,
                       UWord* pKey, UWord* pVal );

void VG_(doneIterFM) ( WordFM* fm );

WordFM* VG_(dopyFM) ( WordFM* fm,
                      UWord(*dopyK)(UWord), UWord(*dopyV)(UWord) );

SizeT VG_(getNodeSizeFM)( void );



typedef  struct _WordBag  WordBag; 

WordBag* VG_(newBag) ( void* (*alloc_nofail)( HChar* cc, SizeT ),
                       HChar* cc,
                       void  (*dealloc)(void*) );

void VG_(deleteBag) ( WordBag* );

void VG_(addToBag)( WordBag*, UWord );

UWord VG_(elemBag) ( WordBag*, UWord );

Bool VG_(delFromBag)( WordBag*, UWord );

Bool VG_(isEmptyBag)( WordBag* );

Bool VG_(isSingletonTotalBag)( WordBag* );

UWord VG_(anyElementOfBag)( WordBag* );

UWord VG_(sizeUniqueBag)( WordBag* ); 
UWord VG_(sizeTotalBag)( WordBag* );  

void VG_(initIterBag)( WordBag* );
Bool VG_(nextIterBag)( WordBag*, UWord* pVal, UWord* pCount );
void VG_(doneIterBag)( WordBag* );


#endif 

