/*
 * pthread_cond_signal.c
 *
 * Description:
 * This translation unit implements condition variables and their primitives.
 *
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
 *
 * -------------------------------------------------------------
 * Algorithm:
 * See the comments at the top of pthread_cond_wait.c.
 */

#include "pthread.h"
#include "implement.h"

static INLINE int
ptw32_cond_unblock (pthread_cond_t * cond, int unblockAll)
{
  int result;
  pthread_cond_t cv;
  int nSignalsToIssue;

  if (cond == NULL || *cond == NULL)
    {
      return EINVAL;
    }

  cv = *cond;

  if (cv == PTHREAD_COND_INITIALIZER)
    {
      return 0;
    }

  if ((result = pthread_mutex_lock (&(cv->mtxUnblockLock))) != 0)
    {
      return result;
    }

  if (0 != cv->nWaitersToUnblock)
    {
      if (0 == cv->nWaitersBlocked)
	{
	  return pthread_mutex_unlock (&(cv->mtxUnblockLock));
	}
      if (unblockAll)
	{
	  cv->nWaitersToUnblock += (nSignalsToIssue = cv->nWaitersBlocked);
	  cv->nWaitersBlocked = 0;
	}
      else
	{
	  nSignalsToIssue = 1;
	  cv->nWaitersToUnblock++;
	  cv->nWaitersBlocked--;
	}
    }
  else if (cv->nWaitersBlocked > cv->nWaitersGone)
    {
      
      if (ptw32_semwait (&(cv->semBlockLock)) != 0)
	{
	  result = errno;
	  (void) pthread_mutex_unlock (&(cv->mtxUnblockLock));
	  return result;
	}
      if (0 != cv->nWaitersGone)
	{
	  cv->nWaitersBlocked -= cv->nWaitersGone;
	  cv->nWaitersGone = 0;
	}
      if (unblockAll)
	{
	  nSignalsToIssue = cv->nWaitersToUnblock = cv->nWaitersBlocked;
	  cv->nWaitersBlocked = 0;
	}
      else
	{
	  nSignalsToIssue = cv->nWaitersToUnblock = 1;
	  cv->nWaitersBlocked--;
	}
    }
  else
    {
      return pthread_mutex_unlock (&(cv->mtxUnblockLock));
    }

  if ((result = pthread_mutex_unlock (&(cv->mtxUnblockLock))) == 0)
    {
      if (sem_post_multiple (&(cv->semBlockQueue), nSignalsToIssue) != 0)
	{
	  result = errno;
	}
    }

  return result;

}				

int
pthread_cond_signal (pthread_cond_t * cond)
{
  return (ptw32_cond_unblock (cond, 0));

}				

int
pthread_cond_broadcast (pthread_cond_t * cond)
{
  return (ptw32_cond_unblock (cond, PTW32_TRUE));

}				
