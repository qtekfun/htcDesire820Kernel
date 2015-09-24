/*
 * ptw32_callUserDestroyRoutines.c
 *
 * Description:
 * This translation unit implements routines which are private to
 * the implementation and may be used throughout it.
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

#if defined(__CLEANUP_CXX)
# if defined(_MSC_VER)
#  include <eh.h>
# elif defined(__WATCOMC__)
#  include <eh.h>
#  include <exceptio.h>
# else
#  if defined(__GNUC__) && __GNUC__ < 3
#    include <new.h>
#  else
#    include <new>
     using
       std::terminate;
#  endif
# endif
#endif

void
ptw32_callUserDestroyRoutines (pthread_t thread)
{
  ThreadKeyAssoc * assoc;

  if (thread.p != NULL)
    {
      ptw32_mcs_local_node_t threadLock;
      ptw32_mcs_local_node_t keyLock;
      int assocsRemaining;
      int iterations = 0;
      ptw32_thread_t * sp = (ptw32_thread_t *) thread.p;

      do
	{
	  assocsRemaining = 0;
	  iterations++;

	  ptw32_mcs_lock_acquire(&(sp->threadLock), &threadLock);
	  sp->nextAssoc = sp->keys;
	  ptw32_mcs_lock_release(&threadLock);

	  for (;;)
	    {
	      void * value;
	      pthread_key_t k;
	      void (*destructor) (void *);

	      ptw32_mcs_lock_acquire(&(sp->threadLock), &threadLock);

	      if ((assoc = (ThreadKeyAssoc *)sp->nextAssoc) == NULL)
		{
		  
		  ptw32_mcs_lock_release(&threadLock);
		  break;
		}
	      else
		{
		  if (ptw32_mcs_lock_try_acquire(&(assoc->key->keyLock), &keyLock) == EBUSY)
		    {
		      ptw32_mcs_lock_release(&threadLock);
		      Sleep(0);
		      continue;
		    }
		}

	      

	      sp->nextAssoc = assoc->nextKey;

	      k = assoc->key;
	      destructor = k->destructor;
	      value = TlsGetValue(k->key);
	      TlsSetValue (k->key, NULL);

	      
	      if (value != NULL && iterations <= PTHREAD_DESTRUCTOR_ITERATIONS)
		{
		  ptw32_mcs_lock_release(&threadLock);
		  ptw32_mcs_lock_release(&keyLock);

		  assocsRemaining++;

#if defined(__cplusplus)

		  try
		    {
		      destructor (value);
		    }
		  catch (...)
		    {
		      terminate ();
		    }

#else 

		  destructor (value);

#endif 

		}
	      else
		{
		  ptw32_tkAssocDestroy (assoc);
		  ptw32_mcs_lock_release(&threadLock);
		  ptw32_mcs_lock_release(&keyLock);
		}
	    }
	}
      while (assocsRemaining);
    }
}				
