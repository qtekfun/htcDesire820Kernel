/*
 * -------------------------------------------------------------
 *
 * Module: sem_wait.c
 *
 * Purpose:
 *	Semaphores aren't actually part of the PThreads standard.
 *	They are defined by the POSIX Standard:
 *
 *		POSIX 1003.1b-1993	(POSIX.1b)
 *
 * -------------------------------------------------------------
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
#include "semaphore.h"
#include "implement.h"


static void PTW32_CDECL
ptw32_sem_wait_cleanup(void * sem)
{
  sem_t s = (sem_t) sem;

  if (pthread_mutex_lock (&s->lock) == 0)
    {
      if (*((sem_t *)sem) != NULL && !(WaitForSingleObject(s->sem, 0) == WAIT_OBJECT_0))
	{
	  ++s->value;
#if defined(NEED_SEM)
	  if (s->value > 0)
	    {
	      s->leftToUnblock = 0;
	    }
#else
#endif 
	}
      (void) pthread_mutex_unlock (&s->lock);
    }
}

int
sem_wait (sem_t * sem)
{
  int result = 0;
  sem_t s = *sem;

  pthread_testcancel();

  if (s == NULL)
    {
      result = EINVAL;
    }
  else
    {
      if ((result = pthread_mutex_lock (&s->lock)) == 0)
	{
	  int v;

	  if (*sem == NULL)
	    {
	      (void) pthread_mutex_unlock (&s->lock);
	      errno = EINVAL;
	      return -1;
	    }

          v = --s->value;
	  (void) pthread_mutex_unlock (&s->lock);

	  if (v < 0)
	    {
#if defined(_MSC_VER) && _MSC_VER < 1400
#pragma inline_depth(0)
#endif
	      
	      pthread_cleanup_push(ptw32_sem_wait_cleanup, (void *) s);
	      result = pthreadCancelableWait (s->sem);
	      
	      pthread_cleanup_pop(result);
#if defined(_MSC_VER) && _MSC_VER < 1400
#pragma inline_depth()
#endif
	    }
#if defined(NEED_SEM)

	  if (!result && pthread_mutex_lock (&s->lock) == 0)
	    {
	      if (*sem == NULL)
	        {
	          (void) pthread_mutex_unlock (&s->lock);
	          errno = EINVAL;
	          return -1;
	        }

	      if (s->leftToUnblock > 0)
		{
		  --s->leftToUnblock;
		  SetEvent(s->sem);
		}
	      (void) pthread_mutex_unlock (&s->lock);
	    }

#endif 

	}

    }

  if (result != 0)
    {
      errno = result;
      return -1;
    }

  return 0;

}				
