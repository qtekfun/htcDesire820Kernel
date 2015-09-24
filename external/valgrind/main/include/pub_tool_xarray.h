

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

#ifndef __PUB_TOOL_XARRAY_H
#define __PUB_TOOL_XARRAY_H



typedef  struct _XArray  XArray;

extern XArray* VG_(newXA) ( void*(*alloc_fn)(HChar*,SizeT), 
                            HChar* cc,
                            void(*free_fn)(void*),
                            Word elemSzB );

extern void VG_(deleteXA) ( XArray* );

extern void VG_(setCmpFnXA) ( XArray*, Int (*compar)(void*,void*) );

extern Word VG_(addToXA) ( XArray*, void* elem );

extern Word VG_(addBytesToXA) ( XArray* xao, void* bytesV, Word nbytes );

extern void VG_(sortXA) ( XArray* );

extern Bool VG_(lookupXA) ( XArray*, void* key, 
                            Word* first, Word* last );

extern Bool VG_(lookupXA_UNSAFE) ( XArray* xao, void* key,
                                   Word* first, Word* last,
                                   Int(*cmpFn)(void*,void*) );

extern Word VG_(sizeXA) ( XArray* );

extern void* VG_(indexXA) ( XArray*, Word );

extern void VG_(dropTailXA) ( XArray*, Word );

extern void VG_(dropHeadXA) ( XArray*, Word );

extern void VG_(removeIndexXA)( XArray*, Word );

extern XArray* VG_(cloneXA)( HChar* cc, XArray* xa );

extern void VG_(getContentsXA_UNSAFE)( XArray* sr,
                                       void** ctsP,
                                       Word*  usedP );

extern void VG_(xaprintf)( XArray* dst, const HChar* format, ... )
                         PRINTF_CHECK(2, 3);

#endif   

