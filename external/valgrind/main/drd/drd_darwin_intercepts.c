/*
  This file is part of drd, a thread error detector.

  Copyright (C) 2006-2012 Bart Van Assche <bvanassche@acm.org>.

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

#include <stdint.h>
#include <stdio.h>
#include "valgrind.h"
#include "drd.h"
#include "pub_tool_redir.h"

void* VG_WRAP_FUNCTION_ZZ(dyld, ZuZZN4dyld18fastBindLazySymbolEPP11ImageLoaderm)
     (void** imageLoaderCache, uintptr_t lazyBindingInfoOffset);
void* VG_WRAP_FUNCTION_ZZ(dyld, ZuZZN4dyld18fastBindLazySymbolEPP11ImageLoaderm)
     (void** imageLoaderCache, uintptr_t lazyBindingInfoOffset)
{
   void* res;
   OrigFn fn;

   VALGRIND_GET_ORIG_FN(fn);

   ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN();
   CALL_FN_W_WW(res, fn, imageLoaderCache, lazyBindingInfoOffset);
   ANNOTATE_IGNORE_READS_AND_WRITES_END();

   return res;
}
