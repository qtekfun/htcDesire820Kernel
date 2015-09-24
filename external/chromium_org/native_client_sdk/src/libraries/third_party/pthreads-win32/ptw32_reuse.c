/*
 * ptw32_threadReuse.c
 *
 * Description:
 * This translation unit implements miscellaneous thread functions.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "pthread.h"
#include "implement.h"



pthread_t
ptw32_threadReusePop (void)
{
  pthread_t t = {NULL, 0};
  ptw32_mcs_local_node_t node;

  ptw32_mcs_lock_acquire(&ptw32_thread_reuse_lock, &node);

  if (PTW32_THREAD_REUSE_EMPTY != ptw32_threadReuseTop)
    {
      ptw32_thread_t * tp;

      tp = ptw32_threadReuseTop;

      ptw32_threadReuseTop = tp->prevReuse;

      if (PTW32_THREAD_REUSE_EMPTY == ptw32_threadReuseTop)
        {
          ptw32_threadReuseBottom = PTW32_THREAD_REUSE_EMPTY;
        }

      tp->prevReuse = NULL;

      t = tp->ptHandle;
    }

  ptw32_mcs_lock_release(&node);

  return t;

}

void
ptw32_threadReusePush (pthread_t thread)
{
  ptw32_thread_t * tp = (ptw32_thread_t *) thread.p;
  pthread_t t;
  ptw32_mcs_local_node_t node;

  ptw32_mcs_lock_acquire(&ptw32_thread_reuse_lock, &node);

  t = tp->ptHandle;
  memset(tp, 0, sizeof(ptw32_thread_t));

  
  tp->ptHandle = t;

  
#if defined(PTW32_THREAD_ID_REUSE_INCREMENT)
  tp->ptHandle.x += PTW32_THREAD_ID_REUSE_INCREMENT;
#else
  tp->ptHandle.x++;
#endif

  tp->state = PThreadStateReuse;

  tp->prevReuse = PTW32_THREAD_REUSE_EMPTY;

  if (PTW32_THREAD_REUSE_EMPTY != ptw32_threadReuseBottom)
    {
      ptw32_threadReuseBottom->prevReuse = tp;
    }
  else
    {
      ptw32_threadReuseTop = tp;
    }

  ptw32_threadReuseBottom = tp;

  ptw32_mcs_lock_release(&node);
}
