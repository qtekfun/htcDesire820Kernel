/*
 * pthread_key_delete.c
 *
 * Description:
 * POSIX thread functions which implement thread-specific data (TSD).
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
pthread_key_delete (pthread_key_t key)
{
  ptw32_mcs_local_node_t keyLock;
  int result = 0;

  if (key != NULL)
    {
      if (key->threads != NULL && key->destructor != NULL)
	{
	  ThreadKeyAssoc *assoc;
	  ptw32_mcs_lock_acquire (&(key->keyLock), &keyLock);
	  while ((assoc = (ThreadKeyAssoc *) key->threads) != NULL)
	    {
              ptw32_mcs_local_node_t threadLock;
	      ptw32_thread_t * thread = assoc->thread;

	      if (assoc == NULL)
		{
		  
		  break;
		}

	      ptw32_mcs_lock_acquire (&(thread->threadLock), &threadLock);
	      ptw32_tkAssocDestroy (assoc);
	      ptw32_mcs_lock_release (&threadLock);
	      ptw32_mcs_lock_release (&keyLock);
	    }
	}

      TlsFree (key->key);
      if (key->destructor != NULL)
	{
	  
	  ptw32_mcs_lock_acquire (&(key->keyLock), &keyLock);
	  ptw32_mcs_lock_release (&keyLock);
	}

#if defined( _DEBUG )
      memset ((char *) key, 0, sizeof (*key));
#endif
      free (key);
    }

  return (result);
}
