

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

#ifndef __PUB_TOOL_SEQMATCH_H
#define __PUB_TOOL_SEQMATCH_H

Bool VG_(generic_match) ( 
        Bool matchAll,
        void* patt,  SizeT szbPatt,  UWord nPatt,  UWord ixPatt,
        void* input, SizeT szbInput, UWord nInput, UWord ixInput,
        Bool (*pIsStar)(void*),
        Bool (*pIsQuery)(void*),
        Bool (*pattEQinp)(void*,void*,void*,UWord),
        void* inputCompleter
     );

Bool VG_(string_match) ( const Char* pat, const Char* str );

#endif   

