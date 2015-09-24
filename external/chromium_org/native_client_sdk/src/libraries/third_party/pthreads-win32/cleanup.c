/*
 * cleanup.c
 *
 * Description:
 * This translation unit implements routines associated
 * with cleaning up threads.
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
 */

#include "pthread.h"
#include "implement.h"


/*
 * The functions ptw32_pop_cleanup and ptw32_push_cleanup
 * are implemented here for applications written in C with no
 * SEH or C++ destructor support. 
 */

ptw32_cleanup_t *
ptw32_pop_cleanup (int execute)
{
  ptw32_cleanup_t *cleanup;

  cleanup = (ptw32_cleanup_t *) pthread_getspecific (ptw32_cleanupKey);

  if (cleanup != NULL)
    {
      if (execute && (cleanup->routine != NULL))
	{

	  (*cleanup->routine) (cleanup->arg);

	}

      pthread_setspecific (ptw32_cleanupKey, (void *) cleanup->prev);

    }

  return (cleanup);

}				


void
ptw32_push_cleanup (ptw32_cleanup_t * cleanup,
		    ptw32_cleanup_callback_t routine, void *arg)
{
  cleanup->routine = routine;
  cleanup->arg = arg;

  cleanup->prev = (ptw32_cleanup_t *) pthread_getspecific (ptw32_cleanupKey);

  pthread_setspecific (ptw32_cleanupKey, (void *) cleanup);

}				
