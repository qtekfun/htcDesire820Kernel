

/*
   This file is part of Helgrind, a Valgrind tool for detecting errors
   in threaded programs.

   Copyright (C) 2007-2012 OpenWorks Ltd
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

#include "pub_tool_basics.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_execontext.h"
#include "pub_tool_threadstate.h"
#include "pub_tool_wordfm.h"

#include "hg_basics.h"
#include "hg_wordset.h"
#include "hg_lock_n_thread.h"            



inline Bool HG_(is_sane_Thread) ( Thread* thr ) {
   return thr != NULL && thr->magic == Thread_MAGIC;
}

static Bool is_sane_Bag_of_Threads ( WordBag* bag )
{
   Thread* thr;
   Word    count;
   VG_(initIterBag)( bag );
   while (VG_(nextIterBag)( bag, (Word*)&thr, &count )) {
      if (count < 1) return False;
      if (!HG_(is_sane_Thread)(thr)) return False;
   }
   VG_(doneIterBag)( bag );
   return True;
}

static Bool is_sane_Lock_BASE ( Lock* lock )
{
   if (lock == NULL
       || (lock->magic != LockN_MAGIC && lock->magic != LockP_MAGIC))
      return False;
   switch (lock->kind) { 
      case LK_mbRec: case LK_nonRec: case LK_rdwr: break; 
      default: return False; 
   }
   if (lock->heldBy == NULL) {
      if (lock->acquired_at != NULL) return False;
      
      return !lock->heldW;
   } else {
      if (lock->acquired_at == NULL) return False;
   }

   if (VG_(isEmptyBag)(lock->heldBy))
      return False;

   
   if (!is_sane_Bag_of_Threads(lock->heldBy)) 
      return False;
   if (lock->heldW) {
      
      if ((lock->kind == LK_nonRec || lock->kind == LK_rdwr)
          && !VG_(isSingletonTotalBag)(lock->heldBy))
         return False;
   } else {
      
      if (lock->kind != LK_rdwr) return False;
   }
   return True;
}

Bool HG_(is_sane_LockP) ( Lock* lock ) {
   return lock != NULL 
          && lock->magic == LockP_MAGIC
          && lock->hbso  == NULL
          && is_sane_Lock_BASE(lock);
}

Bool HG_(is_sane_LockN) ( Lock* lock ) {
   return lock != NULL 
          && lock->magic == LockN_MAGIC
          && lock->hbso  != NULL
          && is_sane_Lock_BASE(lock);
}

Bool HG_(is_sane_LockNorP) ( Lock* lock ) {
   return is_sane_Lock_BASE(lock);
}


