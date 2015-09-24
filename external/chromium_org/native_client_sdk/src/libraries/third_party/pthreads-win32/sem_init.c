/*
 * -------------------------------------------------------------
 *
 * Module: sem_init.c
 *
 * Purpose:
 *	Semaphores aren't actually part of PThreads.
 *	They are defined by the POSIX Standard:
 *
 *		POSIX 1003.1-2001
 *
 * -------------------------------------------------------------
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

int
sem_init (sem_t * sem, int pshared, unsigned int value)
{
  int result = 0;
  sem_t s = NULL;

  if (pshared != 0)
    {
      result = EPERM;
    }
  else if (value > (unsigned int)SEM_VALUE_MAX)
    {
      result = EINVAL;
    }
  else
    {
      s = (sem_t) calloc (1, sizeof (*s));

      if (NULL == s)
	{
	  result = ENOMEM;
	}
      else
	{

	  s->value = value;
	  if (pthread_mutex_init(&s->lock, NULL) == 0)
	    {

#if defined(NEED_SEM)

	  s->sem = CreateEvent (NULL,
				PTW32_FALSE,	
				PTW32_FALSE,	
				NULL);

	  if (0 == s->sem)
	    {
	      free (s);
	      (void) pthread_mutex_destroy(&s->lock);
	      result = ENOSPC;
	    }
	  else
	    {
	      s->leftToUnblock = 0;
	    }

#else 

	      if ((s->sem = CreateSemaphore (NULL,	
					     (long) 0,	
					     (long) SEM_VALUE_MAX,	
					     NULL)) == 0)	
		{
		  (void) pthread_mutex_destroy(&s->lock);
		  result = ENOSPC;
		}

#endif 

	    }
	  else
	    {
	      result = ENOSPC;
	    }

	  if (result != 0)
	    {
	      free(s);
	    }
	}
    }

  if (result != 0)
    {
      errno = result;
      return -1;
    }

  *sem = s;

  return 0;

}				
