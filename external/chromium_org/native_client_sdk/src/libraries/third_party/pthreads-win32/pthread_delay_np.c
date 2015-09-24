/*
 * pthreads_delay_np.c
 *
 * Description:
 * This translation unit implements non-portable thread functions.
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
pthread_delay_np (struct timespec *interval)
{
  DWORD wait_time;
  DWORD secs_in_millisecs;
  DWORD millisecs;
  DWORD status;
  pthread_t self;
  ptw32_thread_t * sp;

  if (interval == NULL)
    {
      return EINVAL;
    }

  if (interval->tv_sec == 0L && interval->tv_nsec == 0L)
    {
      pthread_testcancel ();
      Sleep (0);
      pthread_testcancel ();
      return (0);
    }

  
  secs_in_millisecs = (DWORD)interval->tv_sec * 1000L;

  
  millisecs = (interval->tv_nsec + 999999L) / 1000000L;

#if defined(__WATCOMC__)
#pragma disable_message (124)
#endif

  if (0 > (wait_time = secs_in_millisecs + millisecs))
    {
      return EINVAL;
    }

#if defined(__WATCOMC__)
#pragma enable_message (124)
#endif

  if (NULL == (self = pthread_self ()).p)
    {
      return ENOMEM;
    }

  sp = (ptw32_thread_t *) self.p;

  if (sp->cancelState == PTHREAD_CANCEL_ENABLE)
    {
      if (WAIT_OBJECT_0 ==
	  (status = WaitForSingleObject (sp->cancelEvent, wait_time)))
	{
          ptw32_mcs_local_node_t stateLock;
	  ptw32_mcs_lock_acquire (&sp->stateLock, &stateLock);
	  if (sp->state < PThreadStateCanceling)
	    {
	      sp->state = PThreadStateCanceling;
	      sp->cancelState = PTHREAD_CANCEL_DISABLE;
	      ptw32_mcs_lock_release (&stateLock);

	      ptw32_throw (PTW32_EPS_CANCEL);
	    }

	  ptw32_mcs_lock_release (&stateLock);
	  return ESRCH;
	}
      else if (status != WAIT_TIMEOUT)
	{
	  return EINVAL;
	}
    }
  else
    {
      Sleep (wait_time);
    }

  return (0);
}
