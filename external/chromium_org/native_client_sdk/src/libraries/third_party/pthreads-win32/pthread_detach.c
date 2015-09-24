/*
 * pthread_detach.c
 *
 * Description:
 * This translation unit implements functions related to thread
 * synchronisation.
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

#if !defined(WINCE)
#  include <signal.h>
#endif


int
pthread_detach (pthread_t thread)
{
  int result;
  BOOL destroyIt = PTW32_FALSE;
  ptw32_thread_t * tp = (ptw32_thread_t *) thread.p;
  ptw32_mcs_local_node_t node;

  ptw32_mcs_lock_acquire(&ptw32_thread_reuse_lock, &node);

  if (NULL == tp
      || thread.x != tp->ptHandle.x)
    {
      result = ESRCH;
    }
  else if (PTHREAD_CREATE_DETACHED == tp->detachState)
    {
      result = EINVAL;
    }
  else
    {
      ptw32_mcs_local_node_t stateLock;
      result = 0;

      ptw32_mcs_lock_acquire (&tp->stateLock, &stateLock);
      if (tp->state != PThreadStateLast)
        {
          tp->detachState = PTHREAD_CREATE_DETACHED;
        }
      else if (tp->detachState != PTHREAD_CREATE_DETACHED)
        {
          destroyIt = PTW32_TRUE;
        }
      ptw32_mcs_lock_release (&stateLock);
    }

  ptw32_mcs_lock_release(&node);

  if (result == 0)
    {
      

      if (destroyIt)
	{
	  (void) WaitForSingleObject(tp->threadH, INFINITE);
	  ptw32_threadDestroy (thread);
	}
    }

  return (result);

}				
