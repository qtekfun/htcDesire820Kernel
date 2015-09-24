/*
 * pthread_barrier_wait.c
 *
 * Description:
 * This translation unit implements barrier primitives.
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


int
pthread_barrier_wait (pthread_barrier_t * barrier)
{
  int result;
  pthread_barrier_t b;

  ptw32_mcs_local_node_t node;

  if (barrier == NULL || *barrier == (pthread_barrier_t) PTW32_OBJECT_INVALID)
    {
      return EINVAL;
    }

  ptw32_mcs_lock_acquire(&(*barrier)->lock, &node);

  b = *barrier;
  if (--b->nCurrentBarrierHeight == 0)
    {
      ptw32_mcs_node_transfer(&b->proxynode, &node);

      result = (b->nInitialBarrierHeight > 1
                ? sem_post_multiple (&(b->semBarrierBreeched),
				     b->nInitialBarrierHeight - 1) : 0);
    }
  else
    {
      ptw32_mcs_lock_release(&node);
      result = ptw32_semwait (&(b->semBarrierBreeched));
    }

  if ((PTW32_INTERLOCKED_LONG)PTW32_INTERLOCKED_INCREMENT_LONG((PTW32_INTERLOCKED_LONGPTR)&b->nCurrentBarrierHeight)
		  == (PTW32_INTERLOCKED_LONG)b->nInitialBarrierHeight)
    {
      ptw32_mcs_lock_release(&b->proxynode);
      if (0 == result)
        {
          result = PTHREAD_BARRIER_SERIAL_THREAD;
        }
    }

  return (result);
}
